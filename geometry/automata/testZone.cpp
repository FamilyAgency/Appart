#include "testZone.h"
#define GLSL_STRING(version, shader) #shader
using namespace frames;

testZone::testZone(float width, float height) : FrameGeometry(width, height)
{
	//shader.load("shadersGL3/shader");

	//shader.load("shadersGL3/shader.frag","");
	
	spoutChannelName = "Geometry Template";	
	//img1.loadImage(ofToDataPath("img (1).jpg"));
	//img2.loadImage(ofToDataPath("img (2).jpg"));
	fbo1.allocate(ofGetWidth(), ofGetHeight());
	fbo2.allocate(ofGetWidth(), ofGetHeight());
	mask.allocate(ofGetWidth(), ofGetHeight());
	//shaderFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

	img1.loadImage("gameoflife.jpg");
	img2.loadImage("gameoflife2.jpg");
	
	//img2.loadImage("img_mask.png");
	//img2.loadImage("classic1.png");
	//shader.load("shadersGL3/shader");

	shader.setupShaderFromSource(GL_FRAGMENT_SHADER, getFragSrc());
	shader.linkProgram();
	
	maskColorRGB.set(255, 255, 255, 255);

}


string testZone::getFragSrc()
{
	return GLSL_STRING(120,
		uniform sampler2DRect tex0;
		uniform sampler2DRect imageMask;

	void main()
	{
		vec2 uv = gl_FragCoord.xy;// / vec2(800., 600.);
		vec4 texel0 = texture2DRect(tex0, uv);
		vec4 texel1 = texture2DRect(imageMask, uv);
		gl_FragColor = vec4(texel0.rgb, texel0.a * texel1.a);
	}
	);
}

string testZone::getVertSrc()
{
	return GLSL_STRING(150,
	uniform mat4 modelViewProjectionMatrix;
	in vec4 position;

	void main()
	{
		gl_Position = modelViewProjectionMatrix * position;
	}
	);
}

testZone::~testZone()
{

}

void testZone::setup()
{
	// setup init values
	//FrameGeometry::GUISetup();
}

void testZone::GUISetup()
{
	FrameGeometry::GUISetup();
	gui.add(lolfloat.setup("lolfloat ", 1., -.5, 1.));
	gui.add(maskColor.setup("maskColor", ofColor(255, 255, 255, 200), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));

	//code for gui setup below
	//...
}

void testZone::draw()
{	


	// draw in FBO
	fboArt.begin();
	//ofClear(255, 255, 255, 0);
	//ofBackground()



	fbo1.begin();
	{
		ofClear(255, 255, 255, 0);
		img1.draw(0, 0);
		//ofRectangle(0, 0, 300, 400);
	}
	fbo1.end();

	fbo2.begin();
	{
		ofClear(255, 255, 255, 0);
		img2.draw(0, 0);
		//ofRectangle(0, 0, 300, 400);
	}
	fbo1.end();

	mask.begin();
	{
		ofClear(255, 255, 255, 0);
		ofSetColor(maskColorRGB);
		ofDrawRectangle(0, 0, getWidth(), getHeight());// ofGetWidth(), ofGetHeight());
		//ofCircle(100, 100, 300);

	}
	mask.end();

	//fbo2.begin();
	//{
	//	ofClear(255, 255, 255, 0);
	//	//img2.draw(0, 0);
	//	ofSetColor(maskColor);
	//	ofCircle(100, 100, 300);
	//	//ofRectangle(0, 0, 500, 500);
	//	//ofClear(255, 255, 255, 0);
	//	//ofClearAlpha();
	//}
	//fbo2.end();



	fbo1.draw(0,0);

	shader.begin();
	{
		shader.setUniformTexture("imageMask", mask.getTextureReference(), 1);
		shader.setUniformTexture("tex0", fbo1.getTextureReference(), 2);
		//ofDrawRectangle(0, 0, 800, 600);
		img1.draw(0, 0);
	}
	shader.end();

	//fbo2.draw(0, 500);
	//mask.draw(0,0);

	fboArt.end();
	FrameGeometry::draw();

	
	
}

void testZone::update()
{
	// update algorithm here
	if (maskColorRGB.a > 255)
	{
		maskColorRGB.a = 0;
	}
	else
	{
		maskColorRGB.a++;
	}
	maskColor = maskColorRGB;

}
