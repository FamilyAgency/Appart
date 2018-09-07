#include "NoiseSphere.h"
using namespace frames;

NoiseSphere::NoiseSphere(float width, float height) : FrameShader(width, height)
{
	name = "Curl noise";
	spoutChannelName = "Noise Art";

	addParameter(new ParameterF("u_scale", 4.0));
	addParameter(new ParameterF("u_swirlness", 1.0));
	addParameter(new ParameterF("u_displacement", 0.5));
	addParameter(new Parameter3f("u_mixColor1", ofVec3f(1.0, 0.0, 0.0)));
	addParameter(new Parameter3f("u_mixColor2", ofVec3f(1.0, 0.0, 0.0)));
	addParameter(new Parameter3f("u_mixColor3", ofVec3f(1.0, 0.0, 0.0)));

	img.load("classic1.jpg");
	addParameter(new ParameterT("u_texture", img.getTexture(), 1));

	setupShader();
}

NoiseSphere::~NoiseSphere()
{

}

void NoiseSphere::setup()
{

}

void NoiseSphere::GUISetup()
{
	FrameShader::GUISetup();

	//gui.add(swirlness.setup("swirlness", 0.6, 0.1, 10.0));
	//gui.add(scale.setup("scale", 4.1, 1., 20.0));
	//gui.add(displacement.setup("displacement", 0.5, 0.0, 10.0));

	//gui.add(colorGui1.setup("color1", ofColor(36, 255, 255), ofColor(0, 0), ofColor(255, 255)));
	//gui.add(colorGui2.setup("color2", ofColor(59, 245, 189), ofColor(0, 0), ofColor(255, 255)));
	//gui.add(colorGui3.setup("color3", ofColor(179, 120, 240), ofColor(0, 0), ofColor(255, 255)));
}

void NoiseSphere::update()
{
	FrameShader::update();

	//updateParameter("u_displacement", float(displacement));
	//updateParameter("u_swirlness", float(swirlness));
	//updateParameter("u_scale", float(scale));

	//ofColor color1(colorGui1);
	//updateParameter("u_mixColor1", ofVec3f((int)color1.r / 255.0, (int)color1.g / 255.0, (int)color1.b / 255.0));
	//ofColor color2(colorGui2);
	//updateParameter("u_mixColor2", ofVec3f((int)color2.r / 255.0, (int)color2.g / 255.0, (int)color2.b / 255.0));
	//ofColor color3(colorGui3);
	//updateParameter("u_mixColor3", ofVec3f((int)color3.r / 255.0, (int)color3.g / 255.0, (int)color3.b / 255.0));
}

string NoiseSphere::getFragSrc()
{
	return GLSL_STRING(120,
	uniform vec3 u_mixColor1;
	uniform vec3 u_mixColor2;
	uniform vec3 u_mixColor3;
	uniform vec2 u_resolution;
	uniform float u_time;
	uniform float u_plastic;
	uniform float u_swirlness;
	uniform float u_displacement;
	uniform float u_scale;
	uniform sampler2DRect u_texture;

	float EPS = 0.01;
	int MAX_ITR = 100;
	float MAX_DIS = 10.0;

	vec3 rgb(int r, int g, int b)
	{
		return vec3(float(r) / 255., float(g) / 255., float(b) / 255.);
	}
	
	float sd_sph(vec3 p, float r) 
	{
		return length(p) - r;
	}

	float map(vec3 p)
	{
		vec2 u = p.xy*0.2;
		vec2 um = u*0.3;
		um.x += u_time*0.1*gl_FragCoord.x;
		um.y += -u_time*0.025*gl_FragCoord.y;
		float h = texture2DRect(u_texture, um).x;		//Non twisted height
		um.x += (um.y)*2.0;

		float hlg = texture2DRect(u_texture, um).x;		//Large details
		float hfn = texture2DRect(u_texture, u).x;		//Fine details

		float disp = hlg*0.4 + hfn*0.1*(1.0 - hlg);	//Accumulative displacement

		return sd_sph(p, 1.5) + disp;
	}

	//Lighting 
	float fresnel(float bias, float scale, float power, vec3 I, vec3 N)
	{
		return bias + scale * pow(1.0 + dot(I, N), power);
	}
	
	void main(void)
	{
		vec2 uv = gl_FragCoord.xy;
		vec2 uvn = gl_FragCoord.xy / u_resolution.xy;
		float t = u_time;
		vec3 col = vec3(1. - length(uvn - vec2(0.5))*.5);
		
		//Marching Setup
		vec2 d = 1. - 2.*uvn;
		d.x *= u_resolution.x / u_resolution.y;
		
		float tdist = 0.;
		float dist = EPS;
		
		vec3 campos = vec3(0.0, 0.0, -2.9);
		vec3 pos = campos;
		vec3 raydir = vec3(d.x, -d.y, 1.0);
		
		//Raymarching
		for (int i = 0; i < MAX_ITR; i++)
		{
			if (dist < EPS || dist > MAX_DIS)
				break;
			dist = map(pos);
			tdist += dist;
			pos += dist * raydir;
		}
		//Shading
		if (dist < EPS)
		{
			vec3  lig = normalize(vec3(0., 0.7, -2.));
			vec2 eps = vec2(0.0, EPS);
			vec3 normal = normalize(vec3(
				map(pos + eps.yxx) - map(pos - eps.yxx),
				map(pos + eps.xyx) - map(pos - eps.xyx),
				map(pos + eps.xxy) - map(pos - eps.xxy)
			));
			float diffuse = max(0.0, dot(lig, normal)) / 1.0;
			float specular = pow(diffuse, 256.);
		
			vec3 I = normalize(pos - campos);
			float R = fresnel(0.2, 1.4, 2.0, I, normal);
		
			//vec3 r = texture(iChannel1, reflect(raydir, normal)).rgb;
			//Good Colors: rgb(284, 111, 1)
			col = vec3(diffuse * rgb(84, 118, 145) + specular*0.1 + R*0.5);
		}
		
		gl_FragColor = vec4(col, 1.0);
	};

	
	);
}
