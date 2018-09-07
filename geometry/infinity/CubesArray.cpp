#include "CubesArray.h"

using namespace frames;

CubesArray::CubesArray(float width, float height) : FrameGeometry(width, height)
{
	spoutChannelName = "Geometry Art";
	name = "Cubes Array";

	timeline = ofxCinderTimeline::Timeline::create();
	timeline->stepTo(ofGetElapsedTimef());
	rotateSetup();	

}

CubesArray::~CubesArray()
{

}

void CubesArray::setup()
{
	if (isHorizontal())
	{
		maxBoxW = 50;
		maxBoxH = 50;
		camY = -350;
		camZ = -50;
	}	

	bgColor.set(25, 25, 25);
	mainColor.set(240, 240, 240);

	cam.setDistance(100);
	ofBackground(bgColor);

	ofSetSmoothLighting(true);
	pointLight.setSpotlight(45.f, 0.f);
	pointLight.setAmbientColor(ofColor(0, 0, 0, 255));
	pointLight.setDiffuseColor(ofFloatColor(.85, .85, .85));
	pointLight.setSpecularColor(ofFloatColor(.7f, .7f, .7f));
	pointLight.lookAt(ofPoint(0, 0, 0));

	material.setShininess(120);
	material.setSpecularColor(ofColor(50, 50, 50, 255));
	material.setAmbientColor(ofColor(10, 10, 10, 255));
	material.setDiffuseColor(ofColor(10, 10, 10, 255));
}

void CubesArray::GUISetup()
{
#ifndef release
	FrameGeometry::GUISetup();

	gui.add(rotateX.setup("rotateX ", 130, 0., 360.));//was 145
	gui.add(rotateY.setup("rotateY ", 0, 0., 360.)); // was 0
	gui.add(rotateZ.setup("rotateZ ", 45, 0., 360.));// was 45

	gui.add(lightX.setup("light x ", 50, -50., 50.));
	gui.add(lightY.setup("light y ", 50, -50., 50.));
	gui.add(lightZ.setup("light z ", 225, -50., 300.));

	gui.add(period.setup("period ", 1, 0.00001, 2));
	gui.add(animTime.setup("animTime ", 3.5, 0.00001, 5.));
	gui.add(waveHeight.setup("waveHeight ", 45., 0.0001, 180.));

	gui.add(camX.setup("camX ", 0, -100., 100.));
	gui.add(camY.setup("camY ", -110, -200., 100.));
	gui.add(camZ.setup("camZ ", 60, -100., 100.));
#else
	rotateX.setup("rotateX ", 130, 0., 360.);//was 145
	rotateY.setup("rotateY ", 0, 0., 360.); // was 0
	rotateZ.setup("rotateZ ", 45, 0., 360.);// was 45

	lightX.setup("light x ", 50, -50., 50.);
	lightY.setup("light y ", 50, -50., 50.);
	lightZ.setup("light z ", 225, -50., 300.);

	period.setup("period ", 1, 0.00001, 2);
	animTime.setup("animTime ", 3.5, 0.00001, 5.);
	waveHeight.setup("waveHeight ", 45., 0.0001, 180.);

	camX.setup("camX ", 0, -100., 100.);
	camY.setup("camY ", -110, -200., 100.);
	camZ.setup("camZ ", 60, -100., 100.);
#endif
}

void CubesArray::draw()
{	
	fboArt.begin();
	ofClear(255, 255, 255, 0);
	ofEnableDepthTest();
	ofEnableLighting();
	cam.begin();
	{
		pointLight.enable();
		pointLight.setPosition(ofPoint(lightX, lightY, lightZ));
		pointLight.lookAt(ofPoint(0, 0, 0));

		ofPushMatrix();
		{
			ofRotateX(rotateX);
			ofRotateY(rotateY);
			ofRotateZ(rotateZ);
			for (int y = 0; y < maxBoxH; y++)
			{
				ofPushMatrix();
				{
					ofTranslate(0, boxW*(y - (maxBoxH - 1)*.5), boxW*y);
					for (int x = 0; x < maxBoxW; x++)
					{
						ofPushMatrix();
						{
							ofTranslate(boxW*(x - (maxBoxW - 1)*.5), 0, boxW*x);
							float cx = x - (float)maxBoxW*.5;
							float cy = y - (float)maxBoxH*.5;
							float state = sqrt((cx * cx + cy * cy) / (maxBoxW * maxBoxW + maxBoxH * maxBoxH));// / sqrt(maxBoxW * maxBoxW + maxBoxH * maxBoxH);
							float sinState = remap(state, 0, PI*.5);
							float sinRotate = remap(rotate, -PI, PI);

							float offset = sin(ofClamp((sinState*period * 2 * PI + sinRotate * 2), -PI*.5, PI*.5))*waveHeight + 45.;
							//cout << offset << endl;
							//float offset = sin(clamp(state*period, -PI*.5, PI*.5) + rotate * 2 * PI)*waveHeight;
							//ofTranslate(0, 0, offset);
							ofRotateZ(offset);
							//ofRotateY(offset * 2);
							ofPushMatrix();
							{
								ofDrawBox(boxW);
							}
							ofPopMatrix();
						}
						ofPopMatrix();
					}
				}
				ofPopMatrix();
			}
		}
		ofPopMatrix();
		pointLight.draw();
	}
	cam.end();

	ofDisableDepthTest();	
	ofDisableLighting();
	//ofDisableNormalizedTexCoords();

	fboArt.end();

	pointLight.disable();
	//FBO draws in parent
	FrameGeometry::draw();
}

void CubesArray::update()
{
	timeline->stepTo(ofGetElapsedTimef());
	cam.setPosition(camX, camY, camZ);
}

float CubesArray::spaceDiagonal(float size)
{
	return sqrt(3) * size;
}

float CubesArray::faceDiagonal(float size)
{
	return sqrt(2) * size;
}

void CubesArray::rotateSetup()
{
	float startDotsScale = 1.;
	float endDotsScale = 0.;
	timeline->apply(&rotate,
		startDotsScale,
		endDotsScale,
		animTime)
		.finishFn(std::bind(&CubesArray::rotateFinish, this));
}

void CubesArray::rotateFinish()
{
	rotateSetup();
}

float CubesArray::remap(float value, float min, float max)
{
	return value * (max - min) + min;
}

bool frames::CubesArray::isHorizontal()
{
	return width > height;
}