#include "ApolloFractal.h"
using namespace frames;

ApolloFractal::ApolloFractal(float width, float height, float scale, float param1, float param2, float param3)
	: FrameShader(width, height),
	defaultscale(scale),
	defaultparam1(param1),
	defaultparam2(param2),
	defaultparam3(param3)

{
	name = "Apollo Fractal";
	spoutChannelName = "Shader Art";
	addParameter(new ParameterF("u_scale", defaultscale));
	addParameter(new ParameterF("u_param", defaultparam1));
	addParameter(new ParameterF("u_param1", defaultparam2));
	addParameter(new ParameterF("u_param2", defaultparam3));
	setupShader();
}

ApolloFractal::~ApolloFractal()
{

}

void ApolloFractal::setup()
{
	// setup init values
}

void ApolloFractal::GUISetup()
{
#ifndef release
	//code for gui setup below
	//...
	FrameShader::GUISetup();
	gui.add(scale.setup("u_scale", defaultscale, 0., 20.0));
	gui.add(param.setup("u_param", defaultparam1, 0., 20.0));
	gui.add(param1.setup("u_param1", defaultparam2, 0., 20.0));
	gui.add(param2.setup("u_param2", defaultparam3, 0., 20.0));
#endif

}

void ApolloFractal::update()
{
	FrameShader::update();
	updateParameter("u_scale", float(scale));
	updateParameter("u_param", float(param));
	updateParameter("u_param1", float(param1));
	updateParameter("u_param2", float(param2));
}

// SHADER CODE
string ApolloFractal::getFragSrc()
{
	return GLSL_STRING(120,
	uniform float u_scale;
	uniform float u_param;
	uniform float u_param1;
	uniform float u_param2;
	uniform vec2 u_resolution;
	uniform float u_time;
	uniform sampler2DRect u_texture;

	void main(void)
	{
		float MinRes = min(u_resolution.x, u_resolution.y);
		float MaxRes = max(u_resolution.x, u_resolution.y);

		float h = 0.9*cos(sin(-u_time*.03)*u_param1 + 1.5);//Slice Height

		float Scale = max(1. - h, 0.2);

		vec2 sp = (gl_FragCoord.xy*2. - u_resolution.xy) / MinRes*Scale;

		vec3  p = vec3(sp.x, h, sp.y);

		//Apollonian fractal formula
		float s = 1.;
		for (int i = 0; i < 500; i++)
		{
			p = mod(p + u_scale, 2.) - 1.;
			float k = u_param2 / dot(p, p);
			p *= k;
			s *= k;
		}
		//distance
		float d = max(0.49*abs(p.y) / s, 0.000001);

		vec4 col = vec4(smoothstep(MinRes*u_param / Scale, 1., 1. / d));

		gl_FragColor = vec4(col) + vec4(1., 0., 0., 1.);
	});
}
