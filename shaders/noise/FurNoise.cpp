#include "FurNoise.h"
using namespace frames;

FurNoise::FurNoise(float width, 
	float height,
	float swirlness,
	float scale, 
	float displacement,
	const ofColor& color1,
	const ofColor& color2,
	const ofColor& color3)
	: FrameShader(width, height),
	defaultSwirlness(swirlness),
	defaultScale(scale),
	defaultDisplacement(displacement),
	defaultColor1(color1),
	defaultColor2(color2),
	defaultColor3(color3)
{
	name = "Fur noise";
	spoutChannelName = "Shader Art";
	
	addParameter(new ParameterF("u_scale", scale));
	addParameter(new ParameterF("u_swirlness", swirlness));
	addParameter(new ParameterF("u_displacement", displacement));
	addParameter(new Parameter3f("u_mixColor1", ofVec3f(color1.r, color1.g, color1.b)));
	addParameter(new Parameter3f("u_mixColor2", ofVec3f(color2.r, color2.g, color2.b)));
	addParameter(new Parameter3f("u_mixColor3", ofVec3f(color3.r, color3.g, color3.b)));

	setupShader();
}

FurNoise::~FurNoise()
{

}

void FurNoise::setup()
{

}

void FurNoise::GUISetup()
{
#ifndef release
	FrameShader::GUISetup();
	
	gui.add(swirlness.setup("swirlness", defaultSwirlness, 0.1, 10.0));
	gui.add(scale.setup("scale", defaultScale, 1., 20.0));
	gui.add(displacement.setup("displacement", defaultDisplacement, 0.0, 10.0));

	gui.add(colorGui1.setup("color1", defaultColor1, ofColor(0, 0), ofColor(255, 255)));
	gui.add(colorGui2.setup("color2", defaultColor2, ofColor(0, 0), ofColor(255, 255)));
	gui.add(colorGui3.setup("color3", defaultColor3, ofColor(0, 0), ofColor(255, 255)));
#else
	swirlness.setup("swirlness", defaultSwirlness, 0.1, 10.0);
	scale.setup("scale", defaultScale, 1., 20.0);
	displacement.setup("displacement", defaultDisplacement, 0.0, 10.0);

	colorGui1.setup("color1", defaultColor1, ofColor(0, 0), ofColor(255, 255));
	colorGui2.setup("color2", defaultColor2, ofColor(0, 0), ofColor(255, 255));
	colorGui3.setup("color3", defaultColor3, ofColor(0, 0), ofColor(255, 255));
#endif
}

void FurNoise::update()
{
	FrameShader::update();

	updateParameter("u_displacement", float(displacement));
	updateParameter("u_swirlness", float(swirlness));
	updateParameter("u_scale", float(scale));

	ofColor color1(colorGui1);
	updateParameter("u_mixColor1", ofVec3f((int)color1.r / 255.0, (int)color1.g / 255.0, (int)color1.b / 255.0));
	ofColor color2(colorGui2);
	updateParameter("u_mixColor2", ofVec3f((int)color2.r / 255.0, (int)color2.g / 255.0, (int)color2.b / 255.0));
	ofColor color3(colorGui3);
	updateParameter("u_mixColor3", ofVec3f((int)color3.r / 255.0, (int)color3.g / 255.0, (int)color3.b / 255.0));

}

string FurNoise::getFragSrc()
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
	
	float hash(float n)
	{
		return fract(sin(n)*43758.5453);
	}

	vec3 noise(vec2 x)
	{
		vec2 p = floor(x);
		vec2 f = fract(x);
		f = f*f*(3.0 - 2.0*f);
		float n = p.x + p.y*57.0;
		float res = mix(mix(hash(n + 0.0), hash(n + 1.0), f.x),	mix(hash(n + 57.0), hash(n + 58.0), f.x), f.y);
		return vec3(res);
	}

	vec2 rotate(vec2 p, float a)
	{
		float i = cos(a) * p.x + sin(a) * p.y;
		float j = -sin(a) * p.x + cos(a) * p.y;
		return vec2(i, j);
	}

	vec3 nice(vec2 p)
	{
		vec3 warp = u_swirlness*noise(p + vec2(u_displacement * u_time, u_displacement * u_time));
		float s = 0.1;
		float A = 1.0;
		float F = 1.0;


		for (int i = 0; i < 10; i++)
		{
			p = rotate(p, float(i)*u_swirlness);
			p += warp.y + s;
			s += A * noise(p * F).x;
			F = F * 2.0;
			A = A / 2.0;
		}

		float a = 1.0 / sqrt(3.0);
		float b = a;
		float c = a;

		return  vec3(a * smoothstep(0.6, 0.9, s*s), b * smoothstep(1.1, 1.3, s*s), c * smoothstep(1.5, 1.51, s*s));
	}

	void main(void)
	{
		vec2 uv = gl_FragCoord.xy / u_resolution.xy;
		
		vec2 p = 2.0 * uv - 1.0;
		vec3 col;
		vec3 n;

		p.x *= u_resolution.x / u_resolution.y;
		p += u_time * 0.01;

		float A = 1.0;
		float F = u_scale;

		for (int i = 0; i < 1; ++i)
		{
			n += A * nice(p * F);
			A *= 0.5;
			F *= 2.0;
		}

		col += (n.x) * u_mixColor1;
		col += (n.y) * u_mixColor2;
		col += (n.z) * u_mixColor3;

		gl_FragColor = vec4(col * sqrt(length(col)), 1.0);
	}
	);
}
