#include "ShaderTemplate.h"
using namespace frames;

ShaderTemplate::ShaderTemplate(float width, float height) : FrameShader(width, height)
{
	name = "Shader Template";
	spoutChannelName = "Shader Art";

	addParameter(new ParameterF("EndTime", 0.0));
	
	for (size_t i = 0; i < cellsNum; i++)
	{
		mat1[i] = 1.;
		mat2[i] = 0.;
	}
	
	addParameter(new ParameterV("mat1", mat1, cellsNum));
	addParameter(new ParameterV("mat2", mat2, cellsNum));

	setupShader();
}

ShaderTemplate::~ShaderTemplate()
{

}

void ShaderTemplate::setup()
{
	// setup init values
}

void ShaderTemplate::GUISetup()
{
#ifndef release
	//code for gui setup below
	//...
	FrameShader::GUISetup();
	//gui.add(scale.setup("u_scale", 2., 0., 20.0));
	//gui.add(param.setup("u_param", 2., 0., 20.0));
	//gui.add(param1.setup("u_param1", 2.195, 0., 20.0));
#endif	
}

void ShaderTemplate::update()
{
	FrameShader::update();

	if (ofGetMousePressed())
	{
		updateParameter("EndTime", float((ofGetElapsedTimeMillis() - startTime) / 1000.0f + animSeconds));
	}

	updateParameter("mat1", mat1);
	updateParameter("mat2", mat2);
}

// SHADER CODE
string ShaderTemplate::getFragSrc()
{
	return GLSL_STRING(120,
	uniform vec2 u_resolution;
	uniform float u_time;
	uniform float EndTime = 0.;

	const float animSeconds = 3.;
	const int size = 4;
	uniform float mat1[4];
	uniform float mat2[4];		

	float circle(in vec2 _st, in float _radius, vec2 center)
	{
		vec2 dist = _st - center;
		return 1. - smoothstep(_radius - (_radius*0.01), _radius + (_radius*0.01), dot(dist, dist)*4.0);
	}

	void main(void)
	{
		vec2 st = gl_FragCoord.xy / u_resolution.xy;

		vec3 color = vec3(0.);
		
		float radius = 0.01;

		for (int i = 1; i < size; i++)
		{
			for (int j = 1; j < size; j++)
			{
				int index = (i - 1) * (size - 1) + (j - 1);
				float inradius = radius;

				if (mat1[index] != mat2[index])
				{
					float percent = 0.9;// 1. - ((EndTime - u_time) / animSeconds);

					if (percent >= 1.)
					{
						inradius = mat2[index] * radius;
					}
					else
					{
						if (mat2[index] == 0.)
						{
							percent = 1. - percent;
						}

						inradius = radius * percent;
					}
				}
				else
				{
					inradius = mat2[0] * radius;
				}
				
				inradius = radius*mat2[index];
				color += vec3(circle(st, inradius, vec2(float(j) / size, 1 - float(i) / size)));
			}
		}

		gl_FragColor = vec4(color, 1.0);

	});

	//return GLSL_STRING(120,
	//uniform float u_scale;
	//uniform float u_param;
	//uniform float u_param1;
	//uniform vec2 u_resolution;
	//uniform float u_time;
	//uniform sampler2DRect u_texture;

	//float scale = 20.;
	//float scale1 = 10.;

	//float alpha = 1.0;

	//float square(float percent) 
	//{
	//	vec2 st = gl_FragCoord.xy / u_resolution.xy;
	//	vec3 color = vec3(0.0);
	//	//st = max(st.x, st.y);
	//	vec2 bl = step(vec2(percent), st);
	//	float side = min(bl.x, bl.y);
	//	float pct = side * side;

	//	vec2 tr = step(vec2(percent), 1.0 - st);
	//	side = min(tr.x, tr.y);
	//	color = vec3(pct * side* side);

	//	return color;
	//}
	//
	//float rect(in vec2 st, in vec2 size) 
	//{
	//	size = 0.25 - size*0.25;
	//	vec2 uv = smoothstep(size, size + size*vec2(0.002), st*(1.0 - st));
	//	return uv.x*uv.y;
	//}

	//void main(void)
	//{	
	//	float color = square(0.1);

	//	float color1 = square(0.2);
	//	float color2 = square(0.3);

	//	vec3 color_1 = vec3(1., 1., 1.);
	//	vec3 color_2 = vec3(0., 0., 1.);
	//	vec3 color_3 = vec3(1., 0., 0.);

	//	vec3 colorA = vec3(0.149, 0.141, 0.912);
	//	vec3 colorB = vec3(1.000, 0.833, 0.224);
	//	vec3 comonent = mix(color_1, color_2, square(0.01));
	//	for (int i = 1; i < 8; i++)
	//	{
	//		comonent = mix(comonent, (i % 2 == 0) ? color_2 : color_1, square(i/float(8.)));
	//	}

	//	gl_FragColor = vec4(comonent, 1.);

	//});
}
