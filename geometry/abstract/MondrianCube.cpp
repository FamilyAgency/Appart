#include "MondrianCube.h"

using namespace frames;

MondrianCube::MondrianCube(float width, float height) : FrameGeometry(width, height)
{	
	spoutChannelName = "Geometry Art";
	name = "Mondrian Cube Art";

	textureMondrian.resize(ofBoxPrimitive::SIDES_TOTAL);
	textureMondrianFBO.resize(ofBoxPrimitive::SIDES_TOTAL);

	int artRes = 500;

	for (size_t i = 0; i < ofBoxPrimitive::SIDES_TOTAL; i++)
	{
		auto tex = MondrianAnimatedPtr(new MondrianAnimated(artRes, artRes));
		tex->setup();
		tex->GUISetup();
		tex->GUIvisible(false);
		textureMondrian[i] = tex;
		textureMondrianFBO[i].allocate(artRes, artRes, GL_RGBA, 8);
	}	
	
	cubeMap.loadImages("cubemap/posx.jpg", "cubemap/negx.jpg",		
		"cubemap/posy.jpg",
		"cubemap/negy.jpg",
		"cubemap/posz.jpg",
		"cubemap/negz.jpg");
}

MondrianCube::~MondrianCube()
{

}

void MondrianCube::setup()
{
	// setup init values

	// shininess is a value between 0 - 128, 128 being the most shiny //
	material.setShininess(12);
	// the light highlight of the material //
	material.setSpecularColor(ofColor(255, 255, 255, 255));
	material.setDiffuseColor(ofColor(255, 255, 255, 255));

	ofSetSmoothLighting(true);
	//pointLight.setDiffuseColor(ofFloatColor(.85, .85, .55));
	pointLight.setDiffuseColor(ofFloatColor(238.f / 255.f, 250.f / 255.f, 235.f / 255.f));

	pointLight.setSpecularColor(ofFloatColor(1.f, 1.f, 1.f));

	pointLight2.setDiffuseColor(ofFloatColor(238.f / 255.f, 250.f / 255.f, 235.f / 255.f));
	pointLight2.setSpecularColor(ofFloatColor(.8f, .8f, .9f));


	pointLight3.setDiffuseColor(ofFloatColor(255.f / 255.f, 255.f / 255.f, 255.f / 255.f));
	//pointLight3.setSpecularColor(ofFloatColor(18.f / 255.f, 150.f / 255.f, 135.f / 255.f));
	pointLight3.setSpecularColor(ofFloatColor(1.f, 1.f, 1.f));
	
	
	directionalLight.setDiffuseColor(ofFloatColor(255.f / 255.f, 255.f / 255.f, 255.f / 255.f));
	directionalLight.setSpecularColor(ofFloatColor(1.f, 1.f, 1.f));
	
	//directionalLight.setDirectional();

	// store the box sides so that we can manipulate them later //
	
}

void MondrianCube::GUISetup()
{
	FrameGeometry::GUISetup();

	gui.add(size.setup("Cube size", 250., 10., 1000.0));
	gui.add(bgColor.setup("BG color", ofColor(0, 0, 0), ofColor(0, 0), ofColor(255, 255)));

	//code for gui setup below
	//...
}

void MondrianCube::draw()
{
//	cubeMap.setPosition(getWidth() * 0.5, getHeight() * 0.5, 0);
	
	for (size_t i = 0; i < ofBoxPrimitive::SIDES_TOTAL; i++)
	{
		textureMondrianFBO[i].begin();
		ofClear(255, 255, 255, 0);
		textureMondrian[i]->draw();
		textureMondrianFBO[i].end();
	}

	// draw in FBO
	fboArt.begin();
	ofClear(255, 255, 255, 0);
	ofBackground(bgColor);

	float spinX = sin(ofGetElapsedTimef()*.15f) * 0.6;
	float spinY = cos(ofGetElapsedTimef()*.025f) * 0.6;

	ofEnableDepthTest();
	ofEnableLighting();
	ofEnableNormalizedTexCoords();
	//ofSetVerticalSync(true);
	ofDisableArbTex();
	//glEnable(GL_CULL_FACE);

	//pointLight.enable();
	//pointLight2.enable();
	//pointLight3.enable();

	directionalLight.enable();

	// Box //
	box.set(size);
	box.setPosition(getWidth()*.5, getHeight()*.5, 0);
	box.rotate(spinX, 1.0, 0.0, 0.0);
	box.rotate(spinY, 0, 1.0, 0.0);	

	directionalLight.setPosition(ofVec3f(getWidth()*.5, getHeight()*.5, 900));

	for (int i = 0; i < ofBoxPrimitive::SIDES_TOTAL; i++)
	{
		boxSides[i] = box.getSideMesh(i);
	}

	ofPushMatrix();
	cubeMap.bind();
	ofTranslate(getWidth() * 0.5, getHeight() * 0.5, 800);
	ofScale(1, -1, 1);
	cubeMap.drawSkybox(2000);
	cubeMap.unbind();
	ofPopMatrix();
	
	//box.draw();
	box.transformGL();
	for (int i = 0; i < ofBoxPrimitive::SIDES_TOTAL; i++)
	{
		ofPushMatrix();					
		material.begin();
		textureMondrianFBO[i].getTextureReference().bind();
		ofFill();
		ofSetColor(255);
		
		boxSides[i].draw();
		ofPopMatrix();		
		textureMondrianFBO[i].getTextureReference().unbind();
		material.end();		
	}
	box.restoreTransformGL();

	ofDisableDepthTest();
	ofDisableLighting();
	ofDisableNormalizedTexCoords();
	ofEnableArbTex();
	//glDisable(GL_CULL_FACE);
	fboArt.end();	

	directionalLight.disable();

	FrameGeometry::draw();
}

void MondrianCube::update()
{
	/*pointLight.setPosition((ofGetWidth()*.5) + cos(ofGetElapsedTimef()*.5)*(ofGetWidth()*.3), ofGetHeight() / 2, 500);
	pointLight2.setPosition((ofGetWidth()*.5) + cos(ofGetElapsedTimef()*.15)*(ofGetWidth()*.3),
		ofGetHeight()*.5 + sin(ofGetElapsedTimef()*.7)*(ofGetHeight()), -300);

	pointLight3.setPosition(
		cos(ofGetElapsedTimef()*1.5) * ofGetWidth()*.5,
		sin(ofGetElapsedTimef()*1.5f) * ofGetWidth()*.5,
		cos(ofGetElapsedTimef()*.2) * ofGetWidth()
	);*/

	for (int i = 0; i < ofBoxPrimitive::SIDES_TOTAL; i++)
	{
		textureMondrian[i]->update();
	}

	FrameGeometry::update();
}
