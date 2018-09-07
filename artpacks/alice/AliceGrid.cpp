#include "AliceGrid.h"
using namespace frames;

AliceGrid::AliceGrid(float width, float height) : FrameShader(width, height)
{
	name = "Alice Tunnel";
	spoutChannelName = "Alice art pack";
	setupShader();
}

AliceGrid::~AliceGrid()
{

}

void AliceGrid::setup()
{

}

void AliceGrid::GUISetup()
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

void AliceGrid::update()
{
	FrameShader::update();
}

string AliceGrid::getFragSrc()
{
	return GLSL_STRING(120,
	uniform float u_time;
	uniform vec2 u_resolution;
	float PI = 3.14159265359;

	vec3 permute(vec3 x) { return mod(((x*34.0) + 1.0)*x, 289.0); }

	float snoise(vec2 v) {
		const vec4 C = vec4(0.211324865405187, 0.366025403784439,
			-0.577350269189626, 0.024390243902439);
		vec2 i = floor(v + dot(v, C.yy));
		vec2 x0 = v - i + dot(i, C.xx);
		vec2 i1;
		i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
		vec4 x12 = x0.xyxy + C.xxzz;
		x12.xy -= i1;
		i = mod(i, 289.0);
		vec3 p = permute(permute(i.y + vec3(0.0, i1.y, 1.0))
			+ i.x + vec3(0.0, i1.x, 1.0));
		vec3 m = max(0.5 - vec3(dot(x0, x0), dot(x12.xy, x12.xy),
			dot(x12.zw, x12.zw)), 0.0);
		m = m*m;
		m = m*m;
		vec3 x = 2.0 * fract(p * C.www) - 1.0;
		vec3 h = abs(x) - 0.5;
		vec3 ox = floor(x + 0.5);
		vec3 a0 = x - ox;
		m *= 1.79284291400159 - 0.85373472095314 * (a0*a0 + h*h);
		vec3 g;
		g.x = a0.x  * x0.x + h.x  * x0.y;
		g.yz = a0.yz * x12.xz + h.yz * x12.yw;
		return 130.0 * dot(m, g);
	}

	void main(void)
	{
		vec2 uv = gl_FragCoord.xy / u_resolution.xy;

		float d = .2;
		mat2 t = mat2(1.0, d, -d, 1.0);
		uv = t * uv;
		uv.x += snoise(uv.xy + u_time / 10.0) / 10.0;
		uv *= 10.0;

		uv = ceil(uv + 1.0);
		vec2 m = mod(ceil(uv), 2.0);
		float x = float(bool(m.x)^^bool(m.y));
		vec3 clr = vec3(x);
		gl_FragColor = vec4(clr, 1.0);
	}
	);
}
