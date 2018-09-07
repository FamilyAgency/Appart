#include "AliceRoom.h"
using namespace frames;

AliceRoom::AliceRoom(float width, float height) : FrameShader(width, height)
{
	name = "Alice Room";
	spoutChannelName = "Alice art pack";
	setupShader();
}

AliceRoom::~AliceRoom()
{

}

void AliceRoom::setup()
{

}

void AliceRoom::GUISetup()
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

void AliceRoom::update()
{
	FrameShader::update();

}

string AliceRoom::getFragSrc()
{
	return GLSL_STRING(120,
	uniform float u_time;
	uniform vec2 u_resolution;
	float PI = 3.14159265359;


	vec3 col1 = vec3(0., 0., 0.);
	vec3 col2 = vec3(1., 1.0, 1.);
	vec3 col3 = vec3(1., 1., 1.);

	float hash(float n) { return fract(sin(n)*13.5453123); }
	vec2 hash2(vec2 p) { return vec2(hash(p.x), hash(p.y)); }

	mat2 rot(float x)
	{
		return mat2(cos(x), sin(x), -sin(x), cos(x));
	}

	float sdBox(vec3 p, vec3 b)
	{
		vec3 d = abs(p) - b;
		return min(max(d.x, max(d.y, d.z)), 0.0) + length(max(d, 0.0));
	}

	float sdBoxZ(vec3 p, vec3 b)
	{
		vec3 d = abs(p) - b;
		return min(max(d.x, d.y), 0.0) + length(max(d.xy, 0.0));
	}

	float cylinder(vec3 p, vec2 b)
	{
		float d = length(p.yz) - b.x;
		float k = b.y - abs(p.x);
		return max(d, -k);
	}

	float mid = 0.0;
	vec3 mpos = vec3(0.0, 0., 0.);

	float rs = 11.0;
	float hs = 6.0;

	float picpos(vec3 p)
	{
		return floor(p.z / hs + 0.5) + floor(p.x / rs * 7.0 + 0.5);
	}

	float cam = 0.25;

	float map(vec3 p)
	{
		p.y += sin(p.z * cam);

		mid = 0.0;
		mpos = p;

		float rw = 4.2;
		float rh = 4.0;
		float fx = (fract(p.x / rs - 0.5) - 0.5) * rs;
		vec3 rp = vec3(p.x, p.y, p.z);
		float rd = -sdBoxZ(rp, vec3(rw, rh, 1.0));

		vec3 kp = vec3(abs(p.x) - rw * 1.75 - 1.0, p.y, p.z);
		float kd = -sdBoxZ(kp, vec3(rw * 0.75, rh, 1.0));

		if (kd > rd)
		{
			rd = kd;
			mid = 3.0;
		}

		float ct = 0.1;
		float ax = abs(p.x) - rw + ct;
		float cx = (fract(ax / rs - 0.5) - 0.5) * rs;
		vec3 hp = vec3(ax, p.y, (fract(p.z / hs) - 0.5) * hs);
		float hd = -sdBox(hp, vec3(2.0, 2.0, 1.5));

		float d = rd;
		if (hd > rd) 
		{
			d = hd;
			mid = 1.0;
		}

		vec3 cp = vec3(ax, p.y, (fract(p.z / hs + 0.5) - 0.5) * hs);
		cp.yz *= rot(u_time * 1.25 + picpos(p));
		float cd = cylinder(cp, vec2(2.0, ct));

		if (cd < d) {
			// d = cd;
			//  mid = 2.0;
			//  mpos = cp;
		}

		return d;
	}

	vec3 normal(vec3 p)
	{
		vec3 o = vec3(0.01, 0.0, 0.0);
		return normalize(vec3(map(p + o.xyy) - map(p - o.xyy),	map(p + o.yxy) - map(p - o.yxy), map(p + o.yyx) - map(p - o.yyx)));
	}

	float trace(vec3 o, vec3 r)
	{
		float t = 0.0;
		for (int i = 0; i < 40; ++i) 
		{
			t += map(o + r * t);
		}

		return t;
	}

	vec3 strips(vec2 p)
	{
		float gap = 0.25;
		float fy = fract(p.y);
		float kx = max(fy - gap, 0.0) / (1.0 - gap);
		float ky = min(fy, gap) / gap;
		float ku = 4.0 * kx * (1.0 - kx);
		ku = 1.0 - pow(1.0 - ku, 5.0);
		vec3 tex = vec3(0., 0., 0.); //texture(iChannel0, p * 0.1).xyz;
		tex *= tex;
		float dark = 1.0 - ky;
		dark = dark * dark;
		vec3 gs = mix(col3 * 0.125, col3, dark);
		return tex * (col3 * ku + gs);
	}

	vec3 tiles(vec2 p)
	{
		p *= 0.5;
		vec2 f = fract(p);
		float gap = 0.01;
		vec2 kx = max(f - gap, 0.0) / (1.0 - gap);
		vec2 ky = min(f, gap) / gap;
		vec2 ku = 4.0 * kx * (1.0 - kx);
		ku = pow(1.0 - ku, vec2(5.0));
		vec2 fp = floor(p);
		vec2 tu = hash2(fp) * 1000.0 + fract(p);
		vec3 tex = vec3(0., 0., 0.); //texture(iChannel1, tu * 0.1).xyz;
		tex *= tex;
		float bwt = dot(tex, vec3(0.299, 0.587, 0.114));
		float alt = mod(fp.x + fp.y, 2.0);
		float gc = max(ku.x, ku.y);
		vec3 gl = mix(col3 * bwt * 0.5, col3 * (1.0 - bwt), alt);
		vec3 gs = mix(col1, col1 * 0.125, max(ku.x, ku.y));
		return mix(gl, gs, gc);
	}

	vec3 picture(vec3 p, vec3 w)
	{
		vec2 q = p.yz;
		q *= 0.5;
		float d = 1000.0;
		vec2 tuv = vec2(0.0, 0.0);
		float fid = 0.0;
		float pp = picpos(w) * 3.14159 * 0.125;
		const int n = 5;
		for (int i = 0; i < n; ++i) {
			float fi = float(i) / float(n);
			q = abs(q) - 0.25;
			q *= rot(3.14159 * 0.25 + pp * 3.345);
			q.y = abs(q.y) - 0.25;
			q *= rot(pp);
			float b = sdBoxZ(vec3(q, 0.0), vec3(0.5, 0.125, 1.0));
			if (b < d) {
				d = b;
				tuv = q;
				fid = fi;
			}
		}
		vec3 tex = vec3(1., 1., 1.); //texture(iChannel1, tuv * 0.1).xyz;
		tex *= tex;
		tex = mix(col3, tex * col3, 1.0 - fid);
		return tex;
	}

	void main(void)
	{
		vec2 uv = gl_FragCoord.xy / u_resolution.xy;
		uv = uv * 2.0 - 1.0;
		uv.x *= u_resolution.x / u_resolution.y;

		vec3 o = vec3(0.0, 0.0, u_time * 2.0);
		vec3 r = normalize(vec3(uv, 0.7 - dot(uv, uv) * 0.5));

		o.y -= sin(o.z * cam);
		r.xy *= rot(sin(u_time * 0.125) * 3.14159 * 0.125);

		float t = trace(o, r);
		vec3 w = o + r * t;
		vec3 sn = normal(w);

		vec3 tex = vec3(1.0);

		if (mid == 1.0) {
			if (abs(sn.y) < 0.1) {
				tex = strips(mpos.xy);
			}
			else {
				tex = strips(mpos.xy);
			}
		}
		else if (mid == 2.0) {
			if (abs(sn.x) < 0.1) {
				tex = col1 * 0.25;
			}
			else {
				tex = picture(mpos, w);
			}
		}
		else if (mid == 3.0) {
			vec2 st = mpos.yz - 2.0 * vec2(0.0, o.z);
			tex = tiles(st);
		}
		else if (abs(sn.y) < 0.1) {
			tex = strips(mpos.zy);
		}
		else {
			vec2 st = mpos.xz + vec2(0.0, o.z);
			tex = tiles(st);
		}

		vec3 lit = vec3(0.3);

		vec3 lpos = o + vec3(0.0, 0.0, 0.0);
		lpos += vec3(0.0, 0.0, 0.0);
		vec3 ldel = w - lpos;
		float ldist = length(ldel);
		ldel /= ldist;
		float lm = max(dot(ldel, -sn), 0.0);
		lm /= 1.0 + ldist * ldist * 0.1;
		lit += vec3(lm) * col3 * 2.0;

		float aoc = map(w + sn * 1.2);
		float fog = 1.0 / (1.0 + t * t * 0.001);
		vec3 fc = lit * tex * aoc * fog;
		gl_FragColor = vec4(sqrt(fc), 1.0);
	}
	);
}
