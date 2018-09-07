#include "RocksShader.h"
using namespace frames;

RocksShader::RocksShader(float width, float height) : FrameShader(width, height)
{
	name = "Rocks Shader";
	spoutChannelName = "Raymarching Art";

	addParameter(new ParameterF("between", 54.));
	addParameter(new ParameterF("blur1", .3));


	setupShader();
}

RocksShader::~RocksShader()
{

}

void RocksShader::setup()
{
	// setup init values
}

void RocksShader::GUISetup()
{
	FrameShader::GUISetup();

	//gui.add(colorGui1.setup("meta ball color", ofColor(255, 0, 0), ofColor(0, 0), ofColor(255, 255)));
	//gui.add(colorGui2.setup("background color", ofColor(79, 130, 91), ofColor(0, 0), ofColor(255, 255)));
	//gui.add(scale.setup("iterations", 100., 10., 500.0));
}

void RocksShader::update()
{
	FrameShader::update();

	//ofColor color1(colorGui1);
	//updateParameter("u_color1", ofVec3f((int)color1.r / 255.0, (int)color1.g / 255.0, (int)color1.b / 255.0));
	//ofColor color2(colorGui2);
	//updateParameter("u_color2", ofVec3f((int)color2.r / 255.0, (int)color2.g / 255.0, (int)color2.b / 255.0));
	//updateParameter("u_scale", float(scale));
}


string RocksShader::getFragSrc()
{
	return GLSL_STRING(120,
	uniform vec2 u_resolution;
	uniform float u_time;

	int STEP = 256;
	float EPS = .001;
	uniform float between;
	uniform float blur1;

	float smin(float a, float b, float k)
	{
		float h = clamp(0.5 + 0.5*(b - a) / k, 0.0, 1.0);
		return mix(b, a, h) - k*h*(1.0 - h);
	}

	const mat2 m = mat2(.8, .6, -.6, .8);

	float noise(vec2 x)
	{
		return sin(1.5*x.x)*sin(1.5*x.y);
	}

	float fbm6(vec2 p)
	{
		float f = 0.0;
		f += 0.500000*(0.5 + 0.5*noise(p)); p = m*p*2.02;
		f += 0.250000*(0.5 + 0.5*noise(p)); p = m*p*2.03;
		f += 0.125000*(0.5 + 0.5*noise(p)); p = m*p*2.01;
		f += 0.062500*(0.5 + 0.5*noise(p)); p = m*p*2.04;
		//f += 0.031250*(0.5+0.5*noise( p )); p = m*p*2.01;
		f += 0.015625*(0.5 + 0.5*noise(p));
		return f / 0.96875;
	}

	mat2 getRot(float a)
	{
		float sa = sin(a);
		float ca = cos(a);

		return mat2(ca, -sa, sa, ca);
	}

	vec3 _position;

	float sphere(vec3 center, float radius)
	{
		return distance(_position, center) - radius;
	}

	float hozPlane(float height)
	{
		return distance(_position.y, height);
	}

	float swingPlane(float height)
	{
		vec3 pos = _position + vec3(0., 0., u_time * -1.5);
		float def = fbm6(pos.xz * blur1) * 1.;

		float way = pow(abs(pos.x) * between, 2.5) *.0000125;
		def *= way;

		float ch = height + def;
		return max(pos.y - ch, 0.);
	}

	float map(vec3 pos)
	{
		_position = pos;

		float dist;
		dist = swingPlane(0.);

		float sminFactor = 5.25;
		dist = smin(dist, sphere(vec3(0., -15., 80.), 45.), sminFactor);
		return dist;
	}
	
	vec3 getNormal(vec3 pos)
	{
		vec3 nor = vec3(0., 0., 0.);
		vec3 vv = vec3(0., 1., -1.)*.01;
		nor.x = map(pos + vv.zxx) - map(pos + vv.yxx);
		nor.y = map(pos + vv.xzx) - map(pos + vv.xyx);
		nor.z = map(pos + vv.xxz) - map(pos + vv.xxy);
		nor /= 2.;
		return normalize(nor);
	}

	void main(void)
	{
		vec2 uv = (gl_FragCoord.xy - .5*u_resolution.xy) / u_resolution.y;

		vec3 rayOrigin = vec3(uv + vec2(0., 6.), -1.);

		vec3 rayDir = normalize(vec3(uv, 1.));

		rayDir.zy = getRot(.05) * rayDir.zy;
		rayDir.xy = getRot(.075) * rayDir.xy;

		vec3 position = rayOrigin;

		float curDist;
		int nbStep = 0;

		for (int i = 0; nbStep < STEP; ++nbStep)
		{
			curDist = map(position);

			if (curDist < EPS)
				break;
			position += rayDir * curDist * .5;
		}

		float f;

		float dist = distance(rayOrigin, position);
		f = dist / (98.);

		f = float(nbStep) / float(STEP);
		vec3 col = vec3(f, f, f);

		gl_FragColor = vec4(col, 1.0);
	}
	);
}