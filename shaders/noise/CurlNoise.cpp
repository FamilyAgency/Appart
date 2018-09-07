#include "CurlNoise.h"
using namespace frames;

CurlNoise::CurlNoise(float width, float height) : FrameShader(width, height)
{
	name = "Curl noise";
	spoutChannelName = "Noise Art";

	addParameter(new ParameterF("u_scale", 4.0));
	addParameter(new ParameterF("u_swirlness", 1.0));
	addParameter(new ParameterF("u_displacement", 0.5));
	addParameter(new Parameter3f("u_mixColor1", ofVec3f(1.0, 0.0, 0.0)));
	addParameter(new Parameter3f("u_mixColor2", ofVec3f(1.0, 0.0, 0.0)));
	addParameter(new Parameter3f("u_mixColor3", ofVec3f(1.0, 0.0, 0.0)));

	//img.load("color.jpg");
	//addParameter(new ParameterT("u_texture", img.getTexture(), 1));

	setupShader();
}

CurlNoise::~CurlNoise()
{

}

void CurlNoise::setup()
{

}

void CurlNoise::GUISetup()
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

void CurlNoise::update()
{
	FrameShader::update();
#ifndef release
	updateParameter("u_displacement", float(displacement));
	updateParameter("u_swirlness", float(swirlness));
	updateParameter("u_scale", float(scale));

	ofColor color1(colorGui1);
	updateParameter("u_mixColor1", ofVec3f((int)color1.r / 255.0, (int)color1.g / 255.0, (int)color1.b / 255.0));
	ofColor color2(colorGui2);
	updateParameter("u_mixColor2", ofVec3f((int)color2.r / 255.0, (int)color2.g / 255.0, (int)color2.b / 255.0));
	ofColor color3(colorGui3);
	updateParameter("u_mixColor3", ofVec3f((int)color3.r / 255.0, (int)color3.g / 255.0, (int)color3.b / 255.0));
#endif
}

string CurlNoise::getFragSrc()
{
	return GLSL_STRING(120,
		uniform vec3 u_mixColor1;
	uniform vec3 u_mixColor2;
	uniform vec3 u_mixColor3;
	uniform vec2 u_resolution;
	uniform float u_time;
	uniform float u_plastic;
	uniform float u_swirlness;
	uniform float u_displacement;
	uniform float u_scale;
	uniform sampler2DRect u_texture;

	/* skew constants for 3d simplex functions */
	const float F3 = 0.3333333;
	const float G3 = 0.1666667;

	vec2 Rot(vec2 p, float t)
	{
		float c = cos(t);
		float s = sin(t);
		return vec2(p.x*c + p.y*s, -p.x*s + p.y*c);
	}

	/* discontinuous pseudorandom uniformly distributed in [-0.5, +0.5]^3 */
	vec3 random3(vec3 c)
	{
		float j = 4096.0*sin(dot(c, vec3(17.0, 59.4, 15.0)));
		vec3 r;
		r.z = fract(512.0*j);
		j *= .125;
		r.x = fract(512.0*j);
		j *= .125;
		r.y = fract(512.0*j);
		r = r - 0.5;

		//rotate for extra flow!
		float t = -u_time*.5;
		r.xy = Rot(r.xy, t);
		return r;
	}

	//iq 2d simplex noise

	vec2 hash(vec2 p)
	{
		p = vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3)));
		vec2 h = -1.0 + 2.0*fract(sin(p)*43758.5453123);
		return h;
	}

	/* 3d simplex noise */
	float noise(vec3 p)
	{
		/* 1. find current tetrahedron T and its four vertices */
		/* s, s+i1, s+i2, s+1.0 - absolute skewed (integer) coordinates of T vertices */
		/* x, x1, x2, x3 - unskewed coordinates of p relative to each of T vertices*/

		/* calculate s and x */
		vec3 s = floor(p + dot(p, vec3(F3)));
		vec3 x = p - s + dot(s, vec3(G3));

		/* calculate i1 and i2 */
		vec3 e = step(vec3(0.0), x - x.yzx);
		vec3 i1 = e*(1.0 - e.zxy);
		vec3 i2 = 1.0 - e.zxy*(1.0 - e);

		/* x1, x2, x3 */
		vec3 x1 = x - i1 + G3;
		vec3 x2 = x - i2 + 2.0*G3;
		vec3 x3 = x - 1.0 + 3.0*G3;

		/* 2. find four surflets and store them in d */
		vec4 w;
		vec4 d;

		/* calculate surflet weights */
		w.x = dot(x, x);
		w.y = dot(x1, x1);
		w.z = dot(x2, x2);
		w.w = dot(x3, x3);

		/* w fades from 0.6 at the center of the surflet to 0.0 at the margin */
		w = max(0.6 - w, 0.0);

		/* calculate surflet components */
		d.x = dot(random3(s), x);
		d.y = dot(random3(s + i1), x1);
		d.z = dot(random3(s + i2), x2);
		d.w = dot(random3(s + 1.0), x3);

		/* multiply d by w^4 */
		w *= w;
		w *= w;
		d *= w;

		/* 3. return the sum of the four surflets */
		return dot(d, vec4(52.0));
	}

	float noise(vec2 p)
	{
		const float K1 = 0.366025404;
		const float K2 = 0.211324865;

		vec2 i = floor(p + (p.x + p.y)*K1);

		vec2 a = p - i + (i.x + i.y)*K2;
		vec2 o = (a.x>a.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
		vec2 b = a - o + K2;
		vec2 c = a - 1.0 + 2.0*K2;

		vec3 h = max(0.5 - vec3(dot(a, a), dot(b, b), dot(c, c)), 0.0);

		vec3 n = h*h*h*h*vec3(dot(a, hash(i + 0.0)), dot(b, hash(i + o)), dot(c, hash(i + 1.0)));

		return dot(n, vec3(70.0));
	}

	vec2 RotCS(vec2 p, float c, float s)
	{
		return vec2(p.x*c + p.y*s, -p.x*s + p.y*c);
	}

	float pot(vec2 pos)
	{
		float t = u_time*.1;
		vec3 p = vec3(pos + vec2(u_time*.4, 0.), t);

		float n = noise(p);
		n += 0.5 * noise(p*2.13);
		n += 3. * noise(pos*0.333);
		return n;
	}

	vec2 field(vec2 pos)
	{
		float s = 1.5;
		pos *= s;

		float n = pot(pos);

		float e = 0.1;
		float nx = pot(vec2(pos + vec2(e, 0.)));
		float ny = pot(vec2(pos + vec2(0., e)));

		return vec2(-(ny - n), nx - n) / e;
	}	

	void main(void)
	{
		float lod = 0.;

		vec2 uv = gl_FragCoord.xy;
		uv /= u_resolution.xy;
		uv.x *= u_resolution.x / u_resolution.y;
		uv.y = 1. - uv.y;
		vec2 src_uv = uv;

		vec3 d = vec3(0.);
		vec3 e = vec3(0.);
		for (int i = 0; i<55; i++)
		{
			d += texture2DRect(u_texture, uv + u_time*0.05).xyz *lod;
			e += texture2DRect(u_texture, (-uv.yx*3. + u_time*0.0125)).xyz*lod;

			vec2 new_uv = field(uv)*.00625*.5;

			lod += length(new_uv)*5.;
			uv += new_uv;
		}

		vec3 c = texture2DRect(u_texture, uv*.1 + u_time*0.25).xyz*lod;

		d *= (1. / 50.);
		e *= (1. / 50.);
		c = mix(c, d, length(d));
		c = mix(c, e, length(e));

		gl_FragColor = vec4(c, 1);
	};
	);
}
