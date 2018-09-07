#include "MetaballShader.h"
#include "MathTools.h"

using namespace frames;

MetaballShader::MetaballShader(float width, float height) : FrameShader(width, height)
{
	name = "Metaball Shader";
	spoutChannelName = "Shader Art";

	addParameter(new Parameter3f("u_color1", mathTools().normalizeColor(ofColor(0, 0, 0))));
	addParameter(new Parameter3f("u_color2", mathTools().normalizeColor(ofColor(106, 11, 15))));
	addParameter(new ParameterF("u_scale", 100.0));

	setupShader();
}

MetaballShader::~MetaballShader()
{

}

void MetaballShader::setup()
{
	// setup init values	
}

void MetaballShader::GUISetup()
{
#ifndef release
	FrameShader::GUISetup();

	gui.add(colorGui1.setup("meta ball color", ofColor(0, 0, 0), ofColor(0, 0), ofColor(255, 255)));
	gui.add(colorGui2.setup("background color", ofColor(106, 11, 15), ofColor(0, 0), ofColor(255, 255)));
	gui.add(scale.setup("iterations", 100., 10., 500.0));
#endif
}

void MetaballShader::update()
{
	FrameShader::update();
#ifndef release
	ofColor color1(colorGui1);
	updateParameter("u_color1", ofVec3f((int)color1.r / 255.0, (int)color1.g / 255.0, (int)color1.b / 255.0));
	ofColor color2(colorGui2);
	updateParameter("u_color2", ofVec3f((int)color2.r / 255.0, (int)color2.g / 255.0, (int)color2.b / 255.0));
	updateParameter("u_scale", float(scale));
#endif
}

string MetaballShader::getFragSrc()
{
	return GLSL_STRING(120,
		uniform vec3 u_color1;
	uniform vec3 u_color2;
	uniform vec2 u_resolution;
	uniform float u_time;
	uniform float u_scale;

	float meta = 0.0;
	float g = 2.125;
	vec3 globalP;

	const float zNear = 1.0;
	const float zFar = 400.0;
	const float SHININESS = 20.0;

	const float NBSOURCES = 4.0;

	vec3 getSource(float i)
	{
		return vec3(8.0 - 0.7*(i*sqrt(i)), -2.0 + 0.8*mod(i + 1.0, 4.0), 40)+ vec3(mod(i, 8.0)*sin(u_time*mod(i, 10.0)*0.3)*3.0, mod(i, 7.0)*sin(u_time*mod(i, 9.0)*0.2)*2.0, 0);
	}

	float getFieldIntensity(in vec3 m, out vec3 normal)
	{
		normal = vec3(0, 0, 0);
		float 	fieldIntensity = 0.0;

		for (float i = 0.0; i < NBSOURCES; i += 1.0)
		{
			vec3 source = getSource(i);
			float d = length(m - source);

			float intensity = 1.0 / (d*d);
			vec3 localNormal = normalize(m - source);

			normal += localNormal * intensity;
			fieldIntensity += intensity;
		}

		normal = normalize(normal);
		return fieldIntensity;
	}

	float getFieldIntensity(in vec3 origin, in vec3 ray, out vec3 m, out vec3 normal)
	{
		float	marchingDist = 30.0;
		float 	nbIter = 0.0;

		m = origin;
		float ii = 0.0;

		for (float nbIter = 0.0; nbIter < u_scale; nbIter += 1.0)
		{
			float fieldIntensity = getFieldIntensity(m, normal);
			ii += fieldIntensity;
			marchingDist += 0.4;
			m = origin + ray * marchingDist;
		}

		return ii;
	}

	bool rayMarching(in vec3 origin, in vec3 ray, out vec3 m, out vec3 normal)
	{
		float	marchingDist = 38.0;
		float 	nbIter = 0.0;

		m = origin;

		for (int i = 0; i < 100; i++)
		{
			float fieldIntensity = getFieldIntensity(m, normal);

			if (fieldIntensity > 0.5)
			{
				return true;
			}
			else
			{
				marchingDist += 0.075;
				m = origin + ray * marchingDist;
			}
		}

		return false;
	}
	
	vec4 computeColor(in vec3 ray, in vec3 m, in vec3 normal, in vec3 light)
	{
		vec3 lightRay = normalize(m - light);
		float diffuse = dot(normal, -lightRay);

		vec3 reflectedLight = reflect(lightRay, normal);
		float hilight = pow(max(dot(reflectedLight, -ray), 0.0), SHININESS);
		
		vec3 diffuseComponent = u_color1 * 0.8 * diffuse;
		vec3 hilightComponent = vec3(1.0, 1.0, 1.0) * hilight;
		vec3 ambiantComponent = u_color1 * 0.4;

		return vec4(diffuseComponent + hilightComponent + ambiantComponent, 1.0);
	}

	float shadow(in vec3 light, in vec3 eye, in vec3 ray, in vec3 m, in vec3 normal)
	{
		const float wallDist = 40.0;
		vec3 wallPoint = ray / ray.z * wallDist;

		vec3 lightdir = normalize(wallPoint - light);

		const float e = 0.2;
		float nbColl = 1.0;

		float i = getFieldIntensity(light, lightdir, m, normal);

		return 1.0 - smoothstep(7.5, 17.5, i);	
	}

	vec4 background(in vec3 viewportCoord, in vec3 light, in vec3 eye, in vec3 ray, in vec3 m, in vec3 normal)
	{
		vec3 finalColor = mix(u_color2, vec3(0, 0, 0), 1.0 - shadow(light, eye, ray, m, normal));

		return vec4(finalColor, 1);
	}

	void main(void)
	{
		float 	ratio = u_resolution.x / u_resolution.y;
		vec3 	viewportCoord = vec3(gl_FragCoord.xy / u_resolution.y - vec2(ratio / 4.0, 0.5), zNear);
		vec3	eye = vec3(0.0, 0.0, -0.8);
		vec3	ray = normalize(viewportCoord - eye);
		vec3	light = vec3(-15.0, 5.0, 0.0);
		vec3	m;
		vec3 	normal;

		if (rayMarching(eye, ray, m, normal))
		{
			gl_FragColor = computeColor(ray, m, normal, light);
		}
		else
		{
			gl_FragColor = background(viewportCoord, light, eye, ray, m, normal);
		}
	}
	);
}