#include "AliceNoise.h"
using namespace frames;

AliceNoise::AliceNoise(float width, float height) : FrameShader(width, height)
{
	name = "Alice Noise";
	spoutChannelName = "Alice art pack";

	setupShader();
}

AliceNoise::~AliceNoise()
{

}

void AliceNoise::setup()
{
	// setup init values
}

void AliceNoise::GUISetup()
{
#ifndef release
	FrameShader::GUISetup();

	//code for gui setup below
	//...
#endif
}

void AliceNoise::update()
{
	FrameShader::update();
}

// SHADER CODE
string AliceNoise::getFragSrc()
{
	return GLSL_STRING(120,
	uniform vec3 u_param;
	uniform vec2 u_resolution;
	uniform float u_time;
	uniform sampler2DRect u_texture;

	void main(void)
	{
		float aspect = u_resolution.x / u_resolution.y;
		vec2 uv = gl_FragCoord.xy / u_resolution.xy;
		vec2 position = 0.5 - uv;

		vec2 uva = vec2(position.x, position.y / aspect);

		float r = 1.0 * sqrt(dot(uva, uva));
		vec2 uvd = uva;
		uvd.x += .1 * cos(10. * uvd.y + 0.5 * u_time);
		uvd.y += .1 * sin(10. * uvd.x + 0.5 * u_time);

		float r1 = 8.0 * sqrt(dot(uvd, uvd));
		float value = (1.0 * sin(20.0 * r1));

		float col = smoothstep(0.01, 0.5, value);
		vec3 color = vec3(col, 0., 0.);
		gl_FragColor = vec4(color, 1.0);
	};
	);
}
