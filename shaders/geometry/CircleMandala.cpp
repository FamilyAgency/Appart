#include "CircleMandala.h"
using namespace frames;

CircleMandala::CircleMandala(float width, float height) : FrameShader(width, height)
{
	name = "Shader Template";
	spoutChannelName = "Shader Art";
	addParameter(new ParameterI("u_isRed", 1));
	addParameter(new ParameterF("div1", 10.));
	addParameter(new ParameterF("div2", 2.));

	Paramdiv param;
	param.direction = 1;
	param.max = 10.;
	param.min = 1.;
	param.value = 4.;
	param.increment = 0.001;
	params.push_back(param);

	param.direction = 1;
	param.max = 8.;
	param.min = 2.;
	param.value = 2.;
	param.increment = 0.001;
	params.push_back(param);


	setupShader();
}

CircleMandala::~CircleMandala()
{

}

void CircleMandala::setup()
{
	// setup init values
}

void CircleMandala::GUISetup()
{
#ifndef release
	FrameShader::GUISetup();

	//code for gui setup below
	//...
#endif
}

void CircleMandala::update()
{
	FrameShader::update();
	updateParameter("div1", params[0].value);
	updateParameter("div2", params[1].value);

	for (size_t i = 0; i < params.size(); i++)
	{		
		if (params[i].direction == 1 && params[i].value >= params[i].max)
		{
			params[i].direction = -1;
		}
		else if (params[i].direction == -1 && params[i].value <= params[i].min)
		{
			params[i].direction = 1;
		}

		params[i].value += params[i].direction * params[i].increment;
	}

}

// SHADER CODE
string CircleMandala::getFragSrc()
{
	return GLSL_STRING(120,
	uniform vec3 u_param;
	uniform vec2 u_resolution;
	uniform float u_time;
	uniform sampler2DRect u_texture;
	uniform int u_isRed;

	uniform float div1;
	uniform float div2;

	float circleedge(vec2 p, vec2 o, float radius)
	{
		float s = length(p - o) - radius;
		return 1.0 - exp2(-28000.0 * s*s);
	}

	// polynomial smooth min (k = 0.1) from inigo quilez:
	float smin(float a, float b, float k)
	{
		float h = clamp(0.5 + 0.5 * (b - a) / k, 0.0, 1.0);
		return mix(b, a, h) - k * h * (1.0 - h);
	}

	float circlething(vec2 p)
	{
		float a0 = u_time * 0.065;
		float r_sin = sin(u_time * 0.35);

		float result = 1.0;

		float num_a_circles = 2.0 * div1;
		float inv_num_a_circles = 1.0 / num_a_circles;
		float num_r_circles = 1.0 * div2;
		float inv_num_r_circles = 1.0 / num_r_circles;

		for (float i = 0.0; i < num_r_circles; i += 1.0)
			for (float j = 0.0; j < num_a_circles; j += 1.0)
			{
				float a = a0 + j * 6.283185307179586476925286766559 * inv_num_a_circles;

				float r = (i + 1.0) * inv_num_r_circles * 0.5;
				vec2 o_i = vec2(cos(a), sin(a)) * r;

				float r2 = 0.13 + (1.0 - r) * (1.0 + r_sin * r_sin * r_sin) * 0.136;
				float r_ = sqrt(r2);
				result = smin(result, circleedge(p, o_i, r_), 0.75);
			}

		return max(0.0, result);
	}

	void main(void)
	{
		vec2 pixel_scale = vec2(0.75 / u_resolution.x);

		float s = 0.0;
		const float samples = 5.0;
		const float norm = 1.0 / samples;
		for (float z = 0.0; z < samples; z += 1.0)
		{
			float a = z * norm * 3.0 * 6.283185307179586476925286766559;
			float r0 = sqrt(z * norm);
			float r = r0 * 1.15;
			vec2 aa = vec2(cos(a), sin(a)) * r;
			float w = 1.0 - r0;

			s += circlething((gl_FragCoord.xy - u_resolution.xy * 0.5 + aa) * pixel_scale) * w;
		}
		s *= norm;

		vec3 c = vec3(15.8, 1.9, 0.4) * (s * 64.0);

		float gamma_r = sqrt(c.x);
		float gamma_g = sqrt(c.y);
		float gamma_b = sqrt(c.z);
		gl_FragColor = u_isRed == 1 ? vec4(gamma_r, 0., 0., 1.0) : vec4(gamma_r, gamma_g, gamma_b, 1.0);
	});
}
