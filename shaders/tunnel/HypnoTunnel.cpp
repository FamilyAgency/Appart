#include "HypnoTunnel.h"
using namespace frames;

HypnoTunnel::HypnoTunnel(float width, float height, float speed, float fov, const ofColor& color)
	: FrameShader(width, height),
	defaultSpeed(speed),
	defaultFov(fov),
	defaultColor(color)

{
	name = "Hypno Tunnel";
	spoutChannelName = "Tunnel Art";

	// add shaders uniforms
	addParameter(new ParameterF("u_speed", speed));
	addParameter(new ParameterF("u_fov", fov));
	addParameter(new Parameter3f("u_mixColor1", ofVec3f(color.r, color.g, color.b)));

	setupShader();
}

HypnoTunnel::~HypnoTunnel()
{

}

void HypnoTunnel::setup()
{
	// setup init values
}

void HypnoTunnel::GUISetup()
{
	FrameShader::GUISetup();

	gui.add(speed.setup("u_speed", defaultSpeed, 0.0, 10.0));
	gui.add(fov.setup("u_fov", defaultFov, 0.1, 1.0));
	gui.add(colorGui1.setup("color1", defaultColor, ofColor(0, 0), ofColor(255, 255)));
}

void HypnoTunnel::update()
{
	FrameShader::update();

	updateParameter("u_fov", float(fov));
	updateParameter("u_speed", float(speed));
	ofColor color1(colorGui1);
	updateParameter("u_mixColor1", ofVec3f((int)color1.r / 255.0, (int)color1.g / 255.0, (int)color1.b / 255.0));
}

// SHADER CODE
string HypnoTunnel::getFragSrc()
{
	return GLSL_STRING(120,	
	uniform float u_speed;
	uniform vec2 u_resolution;
	uniform float u_time;

	float PI = 3.141592653589793;
	float TAU = 6.283185307179586;

	// from iq / bookofshaders
	float cubicPulse(float c, float w, float x)
	{
		x = abs(x - c);
		if (x>w) return 0.0;
		x /= w;
		return 1.0 - x*x*(3.0 - 2.0*x);
	}

	void main(void)
	{
		float time = u_time * 0.55;
		vec2 p = (-u_resolution.xy + 2.0*gl_FragCoord.xy) / u_resolution.y;
		vec2 uvOrig = p;
		
		float rotZ = 1. - 0.93 * sin(1. * cos(length(p * 1.5)));
		p *= mat2(cos(rotZ), sin(rotZ), -sin(rotZ), cos(rotZ));

		//-------------------------------
		float a = atan(p.y, p.x); 
		float rSquare = pow(pow(p.x*p.x, 4.0) + pow(p.y*p.y, 4.0), 1.0 / 8.0); 
		float rRound = length(p);
		float r = mix(rSquare, rRound, 0.5 + 0.5 * sin(time * 2.));
		vec2 uv = vec2(0.3 / r + time, a / 3.1415927); 
		uv += vec2(0., 0.25 * sin(time + uv.x * 1.2)); 
		uv /= vec2(1. + 0.0002 * length(uvOrig));
		vec2 uvDraw = fract(uv * 22.); 

		float col = cubicPulse(0.5, 0.06, uvDraw.x);
		col = max(col, cubicPulse(0.5, 0.06, uvDraw.y));

		// darker towards center, light towards outer
		col = col * r * 0.8;
		col += 0.15 * length(uvOrig);
		gl_FragColor = vec4(vec3(1. - col, 1. - col, 1. - col), 1.);
	});
}