#include "ClassicGlitch.h"
using namespace frames;

ClassicGlitch::ClassicGlitch(float width, float height) : FrameShader(width, height)
{
	name = "Classic Glitch";
	spoutChannelName = "Glitch art pack";

	img.load("classicglitch1.jpg");
	images.push_back(img);
	img.load("classicglitch2.jpg");
	images.push_back(img);
	img.load("classicglitch3.jpg");
	images.push_back(img);
	img.load("classicglitch4.jpg");
	images.push_back(img);
	//img.resize(720, 1280);
	addParameter(new ParameterT("u_texture", images[index].getTexture(), 1));

	setupShader();
}

ClassicGlitch::~ClassicGlitch()
{

}

void ClassicGlitch::setup()
{	
	startTime = ofGetElapsedTimeMillis();
	//img.resize(getHeight(), getWidth());	
	updateParameter("u_texture", images[index].getTexture());
	if (++index >= images.size())
	{
		index = 0;
	}
}

void ClassicGlitch::GUISetup()
{
#ifndef release
	FrameShader::GUISetup();
#endif
}

void ClassicGlitch::update()
{
	FrameShader::update();
}

string ClassicGlitch::getFragSrc()
{
	return GLSL_STRING(120,	
	uniform vec2 u_resolution;
	uniform float u_time;
	uniform sampler2DRect u_texture;

	// effect 3
	float random(float x)
	{
		return fract(sin(x) * 10000.);
	}

	float noise(vec2 p)
	{
		return random(p.x + p.y * 10000.);
	}

	vec2 sw(vec2 p) { return vec2(floor(p.x), floor(p.y)); }
	vec2 se(vec2 p) { return vec2(ceil(p.x), floor(p.y)); }
	vec2 nw(vec2 p) { return vec2(floor(p.x), ceil(p.y)); }
	vec2 ne(vec2 p) { return vec2(ceil(p.x), ceil(p.y)); }

	float smoothNoise(vec2 p)
	{
		vec2 interp = smoothstep(0., 1., fract(p));
		float s = mix(noise(sw(p)), noise(se(p)), interp.x);
		float n = mix(noise(nw(p)), noise(ne(p)), interp.x);
		return mix(s, n, interp.y);
	}

	float fractalNoise(vec2 p)
	{
		float x = 0.;
		x += smoothNoise(p);
		x += smoothNoise(p * 2.) / 2.;
		x += smoothNoise(p * 4.) / 4.;
		x += smoothNoise(p * 8.) / 8.;
		x += smoothNoise(p * 16.) / 16.;
		x /= 1. + 1. / 2. + 1. / 4. + 1. / 8. + 1. / 16.;
		return x;
	}

	float movingNoise(vec2 p)
	{
		float x = fractalNoise(p + u_time);
		float y = fractalNoise(p - u_time);
		return fractalNoise(p + vec2(x, y));
	}

	float nestedNoise(vec2 p)
	{
		float x = movingNoise(p);
		float y = movingNoise(p + 100.);
		return movingNoise(p + vec2(x, y));
	}

	////
	
	void main(void)
	{
		vec2 uv = gl_FragCoord.xy;// / u_resolution.xy;
		vec2 uvn = gl_FragCoord.xy / u_resolution.xy;
		int mode = 1;

		if (mode == 1)
		{
			vec2 uv1 = vec2(gl_FragCoord.x / u_resolution.x, gl_FragCoord.y / u_resolution.y);

			float time = sin(u_time * 0.05);

			if (uv1.y > time)
				gl_FragColor = texture2DRect(u_texture, uv);
			else
				gl_FragColor = texture2DRect(u_texture, vec2(uv.x, time * u_resolution.y));
		}
		else if (mode == 2)
		{
			const float speed = 2.;

			float time = u_time*speed;

			float f = 1. - floor(time)*4. + (1. - uvn.y)*300.;
			f = clamp(f, 0., 1.);

			gl_FragColor = texture2DRect(u_texture, uv);
			float f2 = (fract(time)>uvn.x) ? 1. : 0.;

			vec4 rowColor = mix(vec4(0.), gl_FragColor, f2);
			gl_FragColor = mix(gl_FragColor, rowColor, f);

			f = 1. - floor(time + 1.)*4. + (1. - uvn.y)*300.;
			f = clamp(f, 0., 1.);

			gl_FragColor = mix(gl_FragColor, vec4(0.), f);
		}
		else if (mode == 3)
		{
			float n = nestedNoise(uvn * 6.) * 1.0;
			float lerp = (sin(u_time * 0.5) + 1.0) / 2.0;
			float offset = mix(0.0, 2.0, lerp);

			vec2 offsetVector = normalize(vec2(0.5, 0.5) - uvn) * (n * offset);	
			gl_FragColor = texture2DRect(u_texture, uv + offsetVector);
		}
	};
	);
}
