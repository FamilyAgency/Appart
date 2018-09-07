#include "DistortGrid.h"
using namespace frames;

DistortGrid::DistortGrid(float width, float height) : FrameShader(width, height)
{
	name = "Distort Grid Shader";
	spoutChannelName = "Shader Art";
	setupShader();
}

DistortGrid::~DistortGrid()
{

}

void DistortGrid::setup()
{
	// setup init values
}

void DistortGrid::GUISetup()
{
	FrameShader::GUISetup();

	//code for gui setup below
	//...
}

void DistortGrid::update()
{
	FrameShader::update();
}

// SHADER CODE
string DistortGrid::getFragSrc()
{
	return GLSL_STRING(120,
		uniform vec2 u_resolution;
		uniform float u_time;

		float PI = 3.14159265358;
		float TWO_PI = 6.28318530718;

		float random(vec2 st) 
		{
			return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
		}
	
		float noise(in vec2 st)
		{
			vec2 i = floor(st);
			vec2 f = fract(st);
			float a = random(i);
			float b = random(i + vec2(1.0, 0.0));
			float c = random(i + vec2(0.0, 1.0));
			float d = random(i + vec2(1.0, 1.0));
			vec2 u = f*f*(3.0 - 2.0*f);
			return mix(a, b, u.x) +	(c - a)* u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
		}

	void main(void)
	{
		float time = u_time * 1.;
		vec2 uv = (2. * gl_FragCoord.xy - u_resolution.xy) / u_resolution.y;
		float dist = pow(length(uv), 0.5);
		float uvDeformMult = 1. + dist * cos(noise(uv * 2.) + 2. * noise(uv * 3.) + time);
		uv *= 1. + 0.15 * sin(time) * uvDeformMult;
		float divisor = 40.;
		float col = min(
			smoothstep(0.1, 0.25, abs(sin(uv.x * divisor))),
			smoothstep(0.1, 0.25, abs(sin(uv.y * divisor))));
		gl_FragColor = vec4(vec3(col), 1.0);
	});
}

