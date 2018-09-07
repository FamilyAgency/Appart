#include "PlasticNoise.h"
using namespace frames;

PlasticNoise::PlasticNoise(float width, 
	float height,
	float timedamp,
	float scale, 
	float plastic, 
	const ofColor& color) 
	:FrameShader(width, height),
	timedampDefault(timedamp),
	scaleDefault(scale),
	plasticDefault(plastic),
	colorDefault(color)
{
	name = "Plastic noise";
	spoutChannelName = "Shader Art";

	addParameter(new ParameterF("u_time_damp", timedamp));
	addParameter(new ParameterF("u_scale", scale));
	addParameter(new ParameterF("u_plastic", plastic));
	addParameter(new Parameter3f("u_mixColor1", ofVec3f(color.r, color.g, color.b)));

	setupShader();
}

PlasticNoise::~PlasticNoise()
{

}

void PlasticNoise::setup()
{

}

void PlasticNoise::GUISetup()
{
#ifndef release
	FrameShader::GUISetup();

	gui.add(plastic.setup("plastic", plasticDefault, 0.001, 1.0));
	gui.add(scale.setup("scale", scaleDefault, 1., 100.0));
	gui.add(timeDamp.setup("timeDamp", timedampDefault, 0.01, 1.0));
	gui.add(colorGui1.setup("color1", colorDefault, ofColor(0, 0), ofColor(255, 255)));
#else
	plastic.setup("plastic", plasticDefault, 0.001, 1.0);
	scale.setup("scale", scaleDefault, 1., 100.0);
	timeDamp.setup("timeDamp", timedampDefault, 0.01, 1.0);
	colorGui1.setup("color1", colorDefault, ofColor(0, 0), ofColor(255, 255));
#endif
}

void PlasticNoise::update()
{
	FrameShader::update();

	updateParameter("u_time_damp", float(timeDamp));
	updateParameter("u_plastic", float(plastic));
	updateParameter("u_scale", float(scale));

	ofColor color1(colorGui1);
	updateParameter("u_mixColor1", ofVec3f((int)color1.r / 255.0, (int)color1.g / 255.0, (int)color1.b / 255.0));

}

string PlasticNoise::getFragSrc()
{
	return GLSL_STRING(120,

	uniform vec3 u_mixColor1;
	uniform vec2 u_resolution;
	uniform float u_time;
	uniform float u_time_damp;
	uniform float u_plastic;
	uniform float u_scale;

	float hash(float n)
	{
		return fract(sin(n)*43758.5453);
	}

	float noise(in vec2 x)
	{
		vec2 p = floor(x);
		vec2 f = fract(x);
		f = f*f*(3.0 - 2.0*f);
		float n = p.x + p.y*57.0;
		return mix(mix(hash(n + 0.0), hash(n + 1.0), f.x),
			mix(hash(n + 57.0), hash(n + 58.0), f.x), f.y);
	}

	vec2 map(vec2 p, in float offset)
	{
		p.x += 0.1*sin(u_time + 2.0*p.y);
		p.y += 0.1*sin(u_time + 2.0*p.x);

		float a = noise(p*1.5 + sin(0.1*u_time * u_time_damp)) * u_scale;//10.2831;
		a -= offset;
		return vec2(cos(a), sin(a));
	}

	void main(void)
	{
		//vec2 pos = gl_TexCoord[0].st;// gl_FragCoord.xy
		vec2 pos = gl_FragCoord.xy;// gl_FragCoord.xy
		vec2 p = pos / u_resolution.xy;
		vec2 uv = -1.0 - 2.0*p;
		//uv.x *= u_resolution.x / u_resolution.y;

		float offset = u_time*u_time_damp + pos.x / u_resolution.x;

		float acc = 0.0;
		vec3  col = vec3(0.0);
		for (int i = 0; i < 32; i++)
		{
			vec2 dir = map(uv, offset);

			float h = float(i) / 32.0;
			float w = 4.0*h*(1.0 - h);

			vec3 ttt = w*u_mixColor1;

			ttt *= mix(vec3(0.6, 0.7, 0.7), vec3(1.0, 0.95, 0.9), 0.5 - 0.5*dot(reflect(vec3(dir, 0.0), vec3(1.0, 0.0, 0.0)).xy, vec2(0.707)));
			col += w*ttt;
			acc += w;

			uv += u_plastic*dir;
		}
		col /= acc;

		float gg = dot(col, vec3(0.133));
		vec3 nor = normalize(vec3(dFdx(gg), 0.5, dFdy(gg)));
		col += vec3(0.4)*dot(nor, vec3(0.7, 0.01, 0.7));

		vec2 di = map(uv, offset);
		col *= 0.65 + 0.35*dot(di, vec2(0.707));
		col *= 0.20 + 0.80*pow(4.0*p.x*(1.0 - p.x), 0.1);
		col *= 1.7;

		gl_FragColor = vec4(col, 1.);
	};
	);
}
