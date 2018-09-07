#include "PixelMetaballShader.h"
#include "MathTools.h"

using namespace frames;

PixelMetaballShader::PixelMetaballShader(float width, float height) 
	: FrameShader(width, height),
	scale(50.)
{
	name = "Pixel Metaball Shader";
	spoutChannelName = "Shader Art";

	addParameter(new Parameter3f("u_color", ofVec3f(1.0, 0.0, 0.0)));
	addParameter(new ParameterF("u_scale", scale));
	setupShader();
}

PixelMetaballShader::~PixelMetaballShader()
{

}

void PixelMetaballShader::setup()
{
	// setup init values
	scale = 50.;
	direction = 1;
#ifndef release
	guiscale = 50.;
	scale = guiscale;
#endif

	auto color = mathTools().randomColorNormalize();
	updateParameter("u_color", color);
	updateParameter("u_scale", float(scale));
}

void PixelMetaballShader::GUISetup()
{
#ifndef release
	FrameShader::GUISetup();
	gui.add(guiscale.setup("scale", 50., 1., 500.0));
#endif
}

void PixelMetaballShader::update()
{
	FrameShader::update();	
	
	float diff = 0.08;

	if (scale > 70. && direction == 1)
	{
		direction = -1;		
	}
	else if (scale < 20. && direction == -1)
	{
		direction = 1;
	}

	diff *= direction;

	scale += diff;
	updateParameter("u_scale", float(scale));
}

string PixelMetaballShader::getFragSrc()
{
	return GLSL_STRING(120,
	uniform vec3 u_color;
	uniform vec2 u_resolution;
	uniform float u_time;
	uniform float u_scale;

	float ellipse(vec2 uv, vec2 o, float r, float n)
	{
		float res = pow(abs((uv.x - o.x) / r), n) + pow(abs((uv.y - o.y) / r), n);
		return res <= 1. ? sqrt(1. - res) : .0;
	}

	vec3 setPixel(vec2 uv)
	{
		return ellipse(fract(uv), vec2(.5), .5, 3.5)*u_color;
	}

	void main(void)
	{
		vec2 R = u_resolution.xy;
		vec2 uv = (2.*gl_FragCoord.xy - R) / R.y*u_scale;
		vec2 fuv = floor(uv) + .5;
		vec2 t = vec2(sin(u_time), cos(u_time))*u_scale / 2.4;

		vec2 o1 = vec2(0, 1.5*t.x);
		vec2 o2 = vec2(2.7*t.x, 0);
		vec2 o3 = 1.8*t.yx;

		vec3 l = vec3(distance(o1, fuv), distance(o2, fuv), distance(o3, fuv)) * .8;
		vec3 g = u_scale*vec3(.5 / (l.x*l.x), 1. / (l.y*l.y), .75 / (l.z*l.z));

		gl_FragColor = g.x + g.y + g.z > 20. / u_scale ? vec4(setPixel(uv), 1.) : vec4(0., 0., 0., 1.);
	}
	);
}