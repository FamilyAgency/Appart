#include "RandomCircles.h"
using namespace frames;

RandomCircles::RandomCircles(float width, float height) : FrameShader(width, height)
{
	name = "SRandomC ircles";
	spoutChannelName = "Shader Art";

	setupShader();
}

RandomCircles::~RandomCircles()
{

}

void RandomCircles::setup()
{
	// setup init values
}

void RandomCircles::GUISetup()
{
#ifndef release
	FrameShader::GUISetup();

	//code for gui setup below
	//...
#endif
}

void RandomCircles::update()
{
	FrameShader::update();
}

// SHADER CODE
string RandomCircles::getFragSrc()
{
	return GLSL_STRING(120,
	uniform vec3 u_param;
	uniform vec2 u_resolution;
	uniform float u_time;
	uniform sampler2DRect u_texture;

	float circle(vec2 coord, vec2 seed)
	{
		float reso = 60.0;
		float cw = u_resolution.x / reso;

		vec2 p = mod(coord, cw);
		float d = distance(p, vec2(cw / 2.0));

		float rnd = dot(floor(coord / cw), seed);
		float t = u_time * 1.0 + rnd;// +(sin(rnd)) * 9.2;

		float l = cw * (sin(t) * 0.25 + 0.25);
		return clamp(l - d, 0.0, 2.0);
	}

	void main(void)
	{
		vec2 p = gl_FragCoord.xy;
		vec2 dp = vec2(0.9438, 0.3335) * u_time;
		float c1 = circle(p - dp, vec2(3., 2.));
		float c2 = circle(p + dp, vec2(80., 9.));
		float c = min(1.0, c1 - c2);
		gl_FragColor = vec4(c, c, c, 1);
	});
}

