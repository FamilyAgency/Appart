#include "AliceTunnel.h"
using namespace frames;

AliceTunnel::AliceTunnel(float width, float height) : FrameShader(width, height)
{
	name = "Alice Tunnel";
	spoutChannelName = "Alice art pack";
	setupShader();
}

AliceTunnel::~AliceTunnel()
{

}

void AliceTunnel::setup()
{

}

void AliceTunnel::GUISetup()
{
#ifndef release
	FrameShader::GUISetup();

	//gui.add(swirlness.setup("swirlness", 0.6, 0.1, 10.0));
	//gui.add(scale.setup("scale", 4.1, 1., 20.0));
	//gui.add(displacement.setup("displacement", 0.5, 0.0, 10.0));

	//gui.add(colorGui1.setup("color1", ofColor(36, 255, 255), ofColor(0, 0), ofColor(255, 255)));
	//gui.add(colorGui2.setup("color2", ofColor(59, 245, 189), ofColor(0, 0), ofColor(255, 255)));
	//gui.add(colorGui3.setup("color3", ofColor(179, 120, 240), ofColor(0, 0), ofColor(255, 255)));
#endif
}

void AliceTunnel::update()
{
	FrameShader::update();
}

string AliceTunnel::getFragSrc()
{
	return GLSL_STRING(120,
	uniform float u_time;
	uniform vec2 u_resolution;
	float PI = 3.14159265359;

	void main(void)
	{
		vec2 uv = gl_FragCoord.xy / u_resolution.xy / .5 - 1.;
		uv.x *= u_resolution.x / u_resolution.y;

		float dist = 1. / length(uv);
		dist += atan(uv.x, uv.y) / acos(0.);
		dist -= u_time;

		dist = 1. - clamp(sin(dist * PI * 2.) * dot(uv, uv) * u_resolution.y / 15. + .5, 0., 1.);
		dist *= sin(length(uv) - .1);

		gl_FragColor = vec4(dist, dist, dist, 1.0);
	}
	);
}
