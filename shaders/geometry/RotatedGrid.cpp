#include "RotatedGrid.h"
using namespace frames;

RotatedGrid::RotatedGrid(float width, float height) : FrameShader(width, height)
{
	name = "Rotate dGrid";
	spoutChannelName = "Shader Art";
	setupShader();
}

RotatedGrid::~RotatedGrid()
{

}

void RotatedGrid::setup()
{
	// setup init values
}

void RotatedGrid::GUISetup()
{
	FrameShader::GUISetup();

	//code for gui setup below
	//...
}

void RotatedGrid::update()
{
	FrameShader::update();
}

// SHADER CODE
string RotatedGrid::getFragSrc()
{
	return GLSL_STRING(120,
	uniform vec2 u_resolution;
	uniform float u_time;
	const float divs = 16.0;

	void main(void)
	{
		float t = u_time*0.05; // rotation in radians/s
									// t = 0.1;
		float st = sin(t);
		float ct = cos(t);
		vec2 div = vec2(divs, divs*u_resolution.y / u_resolution.x);
		vec2 uv = gl_FragCoord.xy / u_resolution.xy;
		vec2 xy0 = div*uv;
		vec2 xy;
		xy.x = ct*xy0.x + st*xy0.y; // rotate grid by t
		xy.y = -st*xy0.x + ct*xy0.y;
		xy.x -= u_time*0.2;
		vec2 sxy = sin(3.14159*xy);
		vec2 b = fwidth(sxy)*0.7071; // jacobian
		vec2 pxy = smoothstep(-b, b, sxy);
		pxy = 2.0*pxy - 1.0; // remap to [-1..1]
		float a = 0.5*(pxy.x * pxy.y) + 0.5; // combine sine waves and remap to [0..1]
		float c = sqrt(a); // correct for gamma
		gl_FragColor = vec4(c, c, c, 1.0);
	});
}

