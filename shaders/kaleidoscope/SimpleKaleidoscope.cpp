#include "SimpleKaleidoscope.h"
using namespace frames;

SimpleKaleidoscope::SimpleKaleidoscope(float width, float height) : FrameShader(width, height)
{
	name = "Simple Kaleidoscope Shader";
	spoutChannelName = "Kaleidoscope Art";

	// add shaders uniforms
	addParameter(new ParameterF("u_param", 0.0));

	texture.load("color.jpg");
	addParameter(new ParameterT("u_texture", texture.getTexture(), 1));

	setupShader();
}

SimpleKaleidoscope::~SimpleKaleidoscope()
{

}

void SimpleKaleidoscope::setup()
{
	// setup init values
}

void SimpleKaleidoscope::GUISetup()
{
	FrameShader::GUISetup();

	//code for gui setup below
	//...
}

void SimpleKaleidoscope::update()
{
	FrameShader::update();

	// update shader uniforms
	updateParameter("u_param", 1.0);
}

// SHADER CODE
string SimpleKaleidoscope::getFragSrc()
{
	return GLSL_STRING(120,
	uniform vec3 u_param;
	uniform vec2 u_resolution;
	uniform float u_time;
	uniform sampler2DRect u_texture;

	const float PI = 3.141592658;
	const float TAU = 2.0 * PI;
	const float sections = 10.0;

	void main(void)
	{
		vec2 uv = gl_FragCoord.xy;//// u_resolution.xy;
		gl_FragColor = vec4(1., 1., 1., 1.);

		vec2 pos = vec2(gl_FragCoord.st - 0.5 * u_resolution.xy);// / u_resolution.y;

		float rad = length(pos);
		float angle = atan(pos.y, pos.x);

		float ma = mod(angle, TAU / sections);
		ma = abs(ma - PI / sections);
	
		float x = cos(ma) * rad;
		float y = sin(ma) * rad;

		float time = sin(u_time * 3.0);
		
		gl_FragColor = texture2DRect(u_texture, vec2(x - time, y - time));
	};);
}

