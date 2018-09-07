#include "AliceTunnel2.h"
using namespace frames;

AliceTunnel2::AliceTunnel2(float width, float height) : FrameShader(width, height)
{
	name = "Alice Tunnel 2";
	spoutChannelName = "Alice art pack";
	setupShader();
}

AliceTunnel2::~AliceTunnel2()
{

}

void AliceTunnel2::setup()
{

}

void AliceTunnel2::GUISetup()
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

void AliceTunnel2::update()
{
	FrameShader::update();
}

string AliceTunnel2::getFragSrc()
{
	return GLSL_STRING(120,
	uniform float u_time;
	uniform vec2 u_resolution;	

	bool POSTPROCESS = true;
	bool RENDER_DEPTH = true;
	bool RENDER_AO = true;
	bool RENDER_NORMAL = true;

	int RAYMARCHING_STEP = 30;
	float RAYMARCHING_JUMP = 1.;

	const float PI = 3.14159265359;
	float time = 50.;

	//------------------------------------------------------------------  OPERATIONS / PRIMITIVES
	void pR(vec2 p, float a) { p = cos(a)*p + sin(a)*vec2(p.y, -p.x); }
	float vmax(vec2 v) { return max(v.x, v.y); }
	float vmax(vec3 v) { return max(max(v.x, v.y), v.z); }
	float fBox2Cheap(vec2 p, vec2 b) { return vmax(abs(p) - b); }
	float fBoxCheap(vec3 p, vec3 b) { return vmax(abs(p) - b); }
	float pMod1(float p, float size) 
	{
		float halfsize = size*0.5;
		float c = floor((p + halfsize) / size);
		p = mod(p + halfsize, size) - halfsize;
		return c;
	}
	
	//------------------------------------------------------------------ MAP
	float map(vec3 pos)
	{
		pos.y -= 23.;
		pR(pos.xy, pos.z / 20. - time);
		vec3 bp = pos;
		pMod1(bp.z, 40.);
		float b = fBoxCheap(bp, vec3(10., 10., 2.));
		b = max(b, -fBox2Cheap(pos.xy, vec2(8. + sin(pos.z / 10.))));
		float d = min(b, -fBox2Cheap(pos.xy, vec2(10.)));
		return d;
	}

	//------------------------------------------------------------------ RAYMARCHING
	float castRay(vec3 ro, vec3 rd, float depth)
	{
		float t = 0.0;
		float res;
		for (int i = 0; i<RAYMARCHING_STEP; i++)
		{
			vec3 pos = ro + rd*t;
			res = map(pos);
			if (res < 0.01 || t > 150.) break;
			t += res*RAYMARCHING_JUMP;
			if (RENDER_DEPTH)
			{
				depth += 1. / float(RAYMARCHING_STEP);
			}
		}
		return t;
	}

	vec3 calcNormal(vec3 p) {
		float eps = 0.0001;
		const vec3 v1 = vec3(1.0, -1.0, -1.0);
		const vec3 v2 = vec3(-1.0, -1.0, 1.0);
		const vec3 v3 = vec3(-1.0, 1.0, -1.0);
		const vec3 v4 = vec3(1.0, 1.0, 1.0);
		return normalize(v1 * map(p + v1*eps) +
			v2 * map(p + v2*eps) +
			v3 * map(p + v3*eps) +
			v4 * map(p + v4*eps));
	}

	float hash(float n) 
	{
		return fract(sin(n)*3538.5453);
	}

	float calcAO(vec3 p, vec3 n, float maxDist, float falloff) 
	{
		float ao = 0.0;
		const int nbIte = 6;
		for (int i = 0; i<nbIte; i++)
		{
			float l = hash(float(i))*maxDist;
			vec3 rd = n*l;
			ao += (l - map(p + rd)) / pow(1. + l, falloff);
		}
		return clamp(1. - ao / float(nbIte), 0., 1.);
	}

	// calculate local thickness
	float thickness(vec3 p, vec3 n, float maxDist, float falloff)
	{
		float ao = 0.0;
		const int nbIte = 6;
		for (int i = 0; i<nbIte; i++)
		{
			float l = hash(float(i))*maxDist;
			vec3 rd = -n*l;
			ao += (l + map(p + rd)) / pow(1. + l, falloff);
		}
		return clamp(1. - ao / float(nbIte), 0., 1.);
	}

	vec3 postEffects(vec3 col, vec2 uv, float time)
	{
		col *= 0.5 + 0.5*pow(16.0*uv.x*uv.y*(1.0 - uv.x)*(1.0 - uv.y), 0.5);
		return col;
	}

	vec3 render(in vec3 ro, in vec3 rd, in vec2 uv)
	{
		vec3 col = vec3(.0, .0, 1.2);

		float t = 0.;
		float depth = 0.;
		if (RENDER_DEPTH)
		{			
			t = castRay(ro, rd, depth);
		}
		else
		{
			
		}

		if(RENDER_DEPTH)
			return vec3(depth / 10., depth / 5., depth);

		vec3 pos = ro + t * rd;
		vec3 nor = calcNormal(pos);

		if(RENDER_NORMAL)
			return nor;

		float ao = calcAO(pos, nor, 10., 1.2);

		if(RENDER_AO)
			return vec3(ao);

		float thi = thickness(pos, nor, 4., 2.5);

		vec3 lpos1 = vec3(0., 27.5, -time*50.);
		vec3 ldir1 = normalize(lpos1 - pos);
		float latt1 = pow(length(lpos1 - pos)*.1, 1.);
		float trans1 = pow(clamp(max(0., dot(-rd, -ldir1 + nor)), 0., 1.), 1.) + 1.;
		vec3 diff1 = vec3(.1, .1, .1) * (max(dot(nor, ldir1), 0.)) / latt1;
		col = diff1;
		col += vec3(.2, .2, .3) * (trans1 / latt1)*thi;

		vec3 lpos = vec3(80., 0., -time*50.);
		vec3 ldir = normalize(lpos - pos);
		float latt = pow(length(lpos - pos)*.03, .1);
		float trans = pow(clamp(max(0., dot(-rd, -ldir + nor)), 0., 1.), 1.) + 1.;
		col += vec3(.1, .1, .1) * (trans / latt)*thi;

		float d = distance(pos.xyz, vec3(0.));
		col = max(vec3(.05), col);
		col *= ao;

		return col;
	}

	mat3 setCamera(vec3 ro, vec3 ta, float cr)
	{
		vec3 cw = normalize(ta - ro);
		vec3 cp = vec3(sin(cr), cos(cr), 0.0);
		vec3 cu = normalize(cross(cw, cp));
		vec3 cv = normalize(cross(cu, cw));
		return mat3(cu, cv, cw);
	}

	vec3 orbit(float phi, float theta, float radius)
	{
		return vec3(
			radius * sin(phi) * cos(theta),
			radius * cos(phi),
			radius * sin(phi) * sin(theta)
		);
	}

	//------------------------------------------------------------------ MAIN
	void main(void)
	{
		vec2 uv = gl_FragCoord.xy / u_resolution.xy;
		vec2 p = -1. + 2. * uv;
		p.x *= u_resolution.x / u_resolution.y;

		time = 42. + u_time;
		//Camera
		float radius = 50.;
		vec3 ro = orbit(PI / 2. - .5, PI / 2., radius);
		ro.z -= time*50.;
		vec3 ta = vec3(ro.x, ro.y, ro.z - time*50.);
		mat3 ca = setCamera(ro, ta, 0.);
		vec3 rd = ca * normalize(vec3(p.xy, 1.5));

		// Raymarching
		vec3 color = render(ro, rd, uv);
		if(POSTPROCESS)
			color = postEffects(color, uv, time);
		else
			gl_FragColor = vec4(color, 1.0);
	}
	);
}
