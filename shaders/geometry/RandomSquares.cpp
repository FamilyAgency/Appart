#include "RandomSquares.h"
using namespace frames;

RandomSquares::RandomSquares(float width, float height) : FrameShader(width, height)
{
	name = "Random Squares Shader";
	spoutChannelName = "Shader Art";
	setupShader();
}

RandomSquares::~RandomSquares()
{

}

void RandomSquares::setup()
{
	// setup init values
}

void RandomSquares::GUISetup()
{
	FrameShader::GUISetup();

	//code for gui setup below
	//...
}

void RandomSquares::update()
{
	FrameShader::update();
}

// SHADER CODE
string RandomSquares::getFragSrc()
{
	return GLSL_STRING(120,	
	uniform vec2 u_resolution;
	uniform float u_time;

	float widthRange = 0.04;
	float scale = 20.0;
	float alpha = 1.0;

	float round(float x)
	{
		return floor(x + 0.5);
	}

	float square(vec2 r, vec2 center, float width, float a) 
	{
		r = abs(mat2(cos(a), -sin(a), sin(a), cos(a)) *(r - center));
		return alpha*smoothstep(0.05, -0.05, max(r.x, r.y) - width / 2.);
	}

	float rand(vec2 co) 
	{
		return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
	}

	float grid(vec2 r, float scale)
	{
		vec2 rr;

		float angle = 0.0;
		float mask = 0.0;
		float widthFreq;
		float widthPhase;
		float squareWidth;

		for (int i = -1; i < 2; ++i)
		{
			for (float j = -1.0; j < 2.0; ++j)
			{
				float tempi = round(r.x*scale + float(i));
				float tempj = round(r.y*scale + j);
				rr = vec2(tempi, tempj);

				//angle = 3.0*(rand(vec2(rr + 0.1)) - 1.0)*u_time /3.0;
				widthPhase = rand(vec2(rr + 0.2));
				widthFreq = 5.0*rand(vec2(rr + 0.3));
				squareWidth = rand(vec2(rr))*scale*(widthRange + widthRange*sin(u_time*widthFreq + widthPhase));

				mask += square(r*scale, rr, squareWidth, angle);
			}
		}

		return mask;
	}

	void main(void)
	{
		vec2 r = vec2(2.*gl_FragCoord.xy - u_resolution.xy) / u_resolution.y;
		float comonent = grid(r, scale);
		gl_FragColor = vec4(comonent, comonent, comonent, 1.);
	});
}

