#include "SplineTunnel.h"
using namespace frames;

SplineTunnel::SplineTunnel(float width, float height, float speed, float fov, const ofColor& color)
	: FrameShader(width, height),
	defaultSpeed(speed),
	defaultFov(fov),
	defaultColor(color)

{
	name = "Shader Template";
	spoutChannelName = "Template Art";

	// add shaders uniforms
	addParameter(new ParameterF("u_speed", speed));
	addParameter(new ParameterF("u_fov", fov));
	addParameter(new Parameter3f("u_mixColor1", ofVec3f(color.r, color.g, color.b)));

	setupShader();
}

SplineTunnel::~SplineTunnel()
{

}

void SplineTunnel::setup()
{
	// setup init values
}

void SplineTunnel::GUISetup()
{
	FrameShader::GUISetup();

	gui.add(speed.setup("u_speed", defaultSpeed, 0.0, 10.0));
	gui.add(fov.setup("u_fov", defaultFov, 0.1, 1.0));
	gui.add(colorGui1.setup("color1", defaultColor, ofColor(0, 0), ofColor(255, 255)));
}

void SplineTunnel::update()
{
	FrameShader::update();

	updateParameter("u_fov", float(fov));
	updateParameter("u_speed", float(speed));
	ofColor color1(colorGui1);	
	updateParameter("u_mixColor1", ofVec3f((int)color1.r / 255.0, (int)color1.g / 255.0, (int)color1.b / 255.0));
}

// SHADER CODE
string SplineTunnel::getFragSrc()
{
	return GLSL_STRING(120,
	uniform vec3 u_mixColor1;
	uniform float u_speed;
	uniform vec2 u_resolution;
	uniform float u_time;
	uniform float u_fov;
	const vec3 ld = vec3(0., 1., .5);
	float dstepf = 0.0;
	float t = 0.;

	vec2 path(float z)
	{
		return sin(z*.3 + vec2(1.6, 0));
	}

	float fractus(vec3 p)
	{
		vec2 z = p.xy;
		vec2 c = vec2(0.028, -0.056) * sin(p.z - cos(p.z));
		float k = 1.;
		float h = 1.0;
		for (float i = 0.; i<5.; i++)
		{
			h *= 4.*k;
			k = dot(z, z);
			z = vec2(z.x * z.x - z.y * z.y, 1.5 * z.x * z.y) + c;
		}
		return 1. - sqrt(k / h)*log(h);
	}

	vec2 df(vec3 p)
	{
		p.xy += path(p.z);
		float obj = min(1., fractus(p));
		vec2 res = vec2(obj, 1.);
		return res;
	}

	vec3 nor(vec3 p, float prec)
	{
		vec2 e = vec2(prec, 0.);
		vec3 n = vec3(
			df(p + e.xyy).x - df(p - e.xyy).x,
			df(p + e.yxy).x - df(p - e.yxy).x,
			df(p + e.yyx).x - df(p - e.yyx).x);
		return normalize(n);
	}

	float calcAO(in vec3 pos, in vec3 nor)
	{
		float occ = 0.0;
		float sca = 1.0;
		for (int i = 0; i<10; i++)
		{
			float hr = 0.01 + 0.12*float(i) / 4.0;
			vec3 aopos = nor * hr + pos;
			float dd = df(aopos).x;
			occ += -(dd - hr)*sca;
			sca *= 0.95;
		}
		return clamp(1.0 - 3.0*occ, 0.0, 1.0);
	}

	vec3 lighting(vec3 p, vec3 lp, vec3 rd, float prec)
	{
		vec3 l = lp - p;
		float dist = max(length(l), 0.01);
		float atten = min(1. / (1. + dist*0.5), 0.2);
		l /= dist;

		vec3 n = nor(p, prec);
		vec3 r = reflect(-l, n);

		float dif = clamp(dot(l, n), 0.0, 1.0);
		float spe = pow(clamp(dot(r, -rd), 0.0, 1.0), 8.0);
		float fre = pow(clamp(1.0 + dot(n, rd), 0.0, 1.0), 2.0);
		float dom = smoothstep(-1.0, 1.0, r.y);

		vec3 lin = vec3(0.08, 0.32, 0.47);//vec3(0.2);
		lin += 1.0*dif*vec3(1, 1, 0.84);//vec3(1, .97, .85);
		lin += 2.5*spe*dif*vec3(1, 1, 0.84);//vec3(1, .97, .85);
		lin += 2.5*fre*vec3(1);
		lin += 0.5*dom*vec3(1);

		return lin*atten*calcAO(p, n);
	}

	void main(void)
	{
		vec2 si = u_resolution.xy;
		vec2 g = gl_FragCoord.xy;
		vec2 uv = (2.*g - si) / min(si.x, si.y);

		vec4 f = vec4(0.0, 0.0, 0.0, 0.0);

		t = u_time * u_speed;

		dstepf = 1.5;

		vec3 rayOrg = vec3(vec2(0, 0), t);
		rayOrg.xy -= path(t);

		vec3 camUp = vec3(0, 1, 0);
	
		vec3 camOrg = vec3(vec2(0, 0), t + .1);
		camOrg.xy -= path(t + .1);
		
		vec3 axisZ = normalize(camOrg - rayOrg);
		vec3 axisX = normalize(cross(camUp, axisZ));
		vec3 axisY = normalize(cross(axisZ, axisX));
		vec3 rayDir = normalize(axisZ + u_fov * uv.x * axisX + u_fov * uv.y * axisY);

		vec2 s = vec2(0.1, 0.01);
		float d = 0.;
		vec3 p = rayOrg + rayDir * d;
		float dMax = 40.;
		float sMin = 0.00001;

		for (float i = 0.; i<250.; i++)
		{
			if (s.x<sMin || d>dMax) break;
			s = df(p);
			d += s.x * 0.3;
			p = rayOrg + rayDir * d;
			dstepf += 0.005;
		}

		gl_FragColor = vec4(u_mixColor1 * lighting(p, rayOrg, rayDir, 0.001) * dstepf, 1.0);
		gl_FragColor = vec4(mix(gl_FragColor.rgb, u_mixColor1, 1.0 - exp(-0.007*d*d)), 1.0);
	};
	);
}