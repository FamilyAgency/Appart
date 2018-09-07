#include "CubesWave.h"

using namespace frames;

CubesWave::CubesWave(float width, float height) : FrameGeometry(width, height)
{
	spoutChannelName = "Geometry Art";
	name = "Cubes Wave";

	bgColor.set(25, 25, 25);
	mainColor.set(240, 240, 240);
	timeline = ofxCinderTimeline::Timeline::create();
	timeline->stepTo(ofGetElapsedTimef());
	moveZSetup();

	cam.lookAt(ofPoint(0, 0, 0));
	cam.setDistance(100);

	pointLight.setSpotlight(45.f, 0.f);
	pointLight.setAmbientColor(ofColor(0, 0, 0, 255));
	pointLight.setDiffuseColor(ofFloatColor(.85, .85, .85));
	pointLight.setSpecularColor(ofFloatColor(.7f, .7f, .7f));
	//pointLight.setPosition(ofPoint(10, 0, 100));
	pointLight.lookAt(ofPoint(0, 0, 0));

	material.setShininess(120);
	material.setSpecularColor(ofColor(50, 50, 50, 255));
	material.setAmbientColor(ofColor(10, 10, 10, 255));
	material.setDiffuseColor(ofColor(10, 10, 10, 255));
}

CubesWave::~CubesWave()
{
	
}

void CubesWave::setup()
{	
	ofBackground(bgColor);

	ofSetSmoothLighting(true);	
}

void CubesWave::GUISetup()
{
#ifndef release
	FrameGeometry::GUISetup();
	gui.add(rotateX.setup("rotateX ", 145, 0., 360.));
	gui.add(rotateY.setup("rotateY ", 0, 0., 360.));
	gui.add(rotateZ.setup("rotateZ ", 45, 0., 360.));

	gui.add(lightX.setup("light x ", 50, -50., 50.));
	gui.add(lightY.setup("light y ",50, -50., 50.));
	gui.add(lightZ.setup("light z ", 175, -50., 200.));


	gui.add(period.setup("period ", 0.1, 0.00001, 2));
	gui.add(animTime.setup("animTime ", 2.5, 0.00001, 5.));
	gui.add(waveHeight.setup("waveHeight ", 30., 0.0001, 250.));
#else
	rotateX.setup("rotateX ", 145, 0., 360.);
	rotateY.setup("rotateY ", 0, 0., 360.);
	rotateZ.setup("rotateZ ", 45, 0., 360.);

	lightX.setup("light x ", 50, -50., 50.);
	lightY.setup("light y ", 50, -50., 50.);
	lightZ.setup("light z ", 175, -50., 200.);

	period.setup("period ", 0.1, 0.00001, 2);
	animTime.setup("animTime ", 2.5, 0.00001, 5.);
	waveHeight.setup("waveHeight ", 30., 0.0001, 250.);
#endif
}

void CubesWave::draw()
{
	fboArt.begin();
	ofClear(255, 255, 255, 0);
	
	ofEnableLighting();
	ofEnableDepthTest();

	cam.begin();
	{
		pointLight.enable();
		pointLight.setPosition(ofPoint(lightX, lightY, lightZ));
		pointLight.lookAt(ofPoint(0, 0, 0));
		//cout << "light " << pointLight.getPosition() << endl;
		ofPushMatrix();
		{
			ofRotateX(rotateX);
			ofRotateY(rotateY);
			ofRotateZ(rotateZ);
			for (int y = 0; y < maxBoxW; y++)
			{
				ofPushMatrix();
				{
					ofTranslate(0, boxW*(y - (maxBoxW - 1)*.5), 0);
					for (int x = 0; x < maxBoxW; x++)
					{
						ofPushMatrix();
						{
							ofTranslate(boxW*(x - (maxBoxW - 1)*.5), 0,0);
							float cx = x - (float)maxBoxW*.5;
							float cy = y - (float)maxBoxH*.5;
							float offset = sin(sqrt(cx * cx + cy * cy)*period + moveZ*2*PI)*waveHeight;
							ofTranslate(0, 0, offset);
							ofPushMatrix();
							{
								ofDrawBox(boxW);
							}
							ofPopMatrix();
						}
						ofPopMatrix();
					}//for end
				}
				ofPopMatrix();
			}//dor end
			
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

void CubesWave::update()
{
	timeline->stepTo(ofGetElapsedTimef());
}

float CubesWave::spaceDiagonal(float size)
{
	return sqrt(3)*size;
}

float CubesWave::faceDiagonal(float size)
{
	return sqrt(2)*size;
}

void CubesWave::moveZSetup()
{
	float startDotsScale = 1.;
	float endDotsScale = 0.;
	timeline->apply(&moveZ,
		startDotsScale,
		endDotsScale,
		animTime)
		.finishFn(std::bind(&CubesWave::moveZFinish, this));
}

void CubesWave::moveZFinish()
{
	moveZSetup();
}