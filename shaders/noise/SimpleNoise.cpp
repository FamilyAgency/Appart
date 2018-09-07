#include "SimpleNoise.h"
using namespace frames;

SimpleNoise::SimpleNoise(float width, float height, int octaves, float mult, const ofVec2f& shift, const ofColor& color1, const ofColor& color2)
	: FrameShader(width, height),
	defaultOctaves(octaves),
	defaultShift(shift),
	defaultMult(mult),
	defaultColor1(color1),
	defaultColor2(color2)
{	
	name = "Simple noise";
	spoutChannelName = "Shader Art";

	addParameter(new ParameterI("u_numOctaves", octaves));
	addParameter(new Parameter2f("u_shift", shift));
	addParameter(new ParameterF("u_mult", mult));
	addParameter(new Parameter3f("u_mixColor1", ofVec3f(color1.r, color1.g, color1.b)));
	addParameter(new Parameter3f("u_mixColor2", ofVec3f(color2.r, color2.g, color2.b)));
	setupShader();	
}

SimpleNoise::~SimpleNoise()
{
	
}

void SimpleNoise::setup()
{

}

void SimpleNoise::GUISetup()
{
#ifndef release
	FrameShader::GUISetup();

	gui.add(numOctaves.setup("numOctaves", defaultOctaves, 40, 100));
	gui.add(mult.setup("mult", defaultMult, 0., 3.));
	gui.add(shift.setup("shift", defaultShift, ofVec2f(0., 0.), ofVec2f(200., 200.)));

	gui.add(colorGui1.setup("color1", defaultColor1, ofColor(0, 0), ofColor(255, 255)));
	gui.add(colorGui2.setup("color2", defaultColor2, ofColor(0, 0), ofColor(255, 255)));
#else
	numOctaves.setup("numOctaves", defaultOctaves, 40, 100);
	mult.setup("mult", defaultMult, 0., 3.);
	shift.setup("shift", defaultShift, ofVec2f(0., 0.), ofVec2f(200., 200.));

	colorGui1.setup("color1", defaultColor1, ofColor(0, 0), ofColor(255, 255));
	colorGui2.setup("color2", defaultColor2, ofColor(0, 0), ofColor(255, 255));
#endif
}

void SimpleNoise::update()
{
	FrameShader::update();

	ofColor color1(colorGui1);
	ofColor color2(colorGui2);
	updateParameter("u_mixColor1", ofVec3f((int)color1.r / 255.0, (int)color1.g / 255.0, (int)color1.b / 255.0));
	updateParameter("u_mixColor2", ofVec3f((int)color2.r / 255.0, (int)color2.g / 255.0, (int)color2.b / 255.0));
	updateParameter("u_numOctaves", int(numOctaves));
	updateParameter("u_shift", ofVec2f(shift));
	updateParameter("u_mult", float(mult));
}

string SimpleNoise::getFragSrc()
{
	return GLSL_STRING(120,
	uniform vec2 u_resolution;
	uniform vec2 u_mouse;
	uniform float u_time;
	uniform float u_mult;
	uniform int u_numOctaves;

	uniform vec3 u_mixColor1;
	uniform vec3 u_mixColor2;
	uniform vec2 u_shift;

	float random(in vec2 _st)
	{
		return fract(sin(dot(_st.xy,
			vec2(12.9898, 78.233)))*
			43758.5453123);
	}

	float noise(in vec2 _st)
	{
		vec2 i = floor(_st);
		vec2 f = fract(_st);

		float a = random(i);
		float b = random(i + vec2(1.0, 0.0));
		float c = random(i + vec2(0.0, 1.0));
		float d = random(i + vec2(1.0, 1.0));

		vec2 u = f * f * (3.0 - 2.0 * f);

		return mix(a, b, u.x) +
			(c - a)* u.y * (1.0 - u.x) +
			(d - b)* u.x * u.y;
	}

	float fbm(in vec2 _st)
	{
		float v = 0.0;
		float a = 0.5;
	
		mat2 rot = mat2(cos(0.5), sin(0.5),
			-sin(0.5), cos(0.50));
		for (int i = 0; i < u_numOctaves; ++i)
		{
			v += a * noise(_st);
			_st = rot * _st * u_mult + u_shift;
			a *= 0.5;
		}
		return v;
	}

	void main()
	{
		vec2 st = gl_FragCoord.xy / u_resolution.xy*3.;
		vec3 color = vec3(0.0);

		vec2 q = vec2(0.);
		q.x = fbm(st + 0.0*u_time);
		q.y = fbm(st + vec2(1.));

		vec2 r = vec2(0.);
		r.x = fbm(st + 1.0*q + vec2(1.7, 9.2) + 0.15*u_time);
		r.y = fbm(st + 1.0*q + vec2(8.3, 2.8) + 0.126*u_time);

		float f = fbm(st + r);

		color = mix(vec3(0.101961, 0.619608, 0.666667),
			vec3(0.666667, 0.666667, 0.498039),
			clamp((f*f)*4.0, 0.0, 1.0));

		color = mix(color,
			u_mixColor1,
			clamp(length(q), 0.0, 1.0));

		color = mix(color,
			u_mixColor2,
			clamp(length(r.x), 0.0, 1.0));

		gl_FragColor = vec4((f*f*f + .6*f*f + .5*f)*color, 1.);
	}
	);
}
