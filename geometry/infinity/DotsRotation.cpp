#include "DotsRotation.h"

using namespace frames;

DotsRotation::DotsRotation(float width, float height) : FrameGeometry(width, height)
{
	spoutChannelName = "Geometry Art";
	name = "Template name";
	mainColor.set(10, 10, 10, 255);
	bgColor.set(255, 255, 255, 255);

	timeline = ofxCinderTimeline::Timeline::create();
	timeline->stepTo(ofGetElapsedTimef());
}

DotsRotation::~DotsRotation()
{

}

void DotsRotation::setup()
{
	screenCenter.set(getWidth()*.5, getHeight()*.5);
	timeline->stepTo(ofGetElapsedTimef());
	animSetup();
	ofBackground(bgColor);
}

void DotsRotation::GUISetup()
{
#ifndef release
	FrameGeometry::GUISetup();
#endif
}

void DotsRotation::draw()
{	
	fboArt.begin();
	ofClear(255, 255, 255, 0);

	ofSetColor(mainColor);

	ofTranslate(screenCenter);
	ofDrawCircle(0,0,radius);

	ofPushMatrix();
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < i+5; j++)
		{
			ofPushMatrix();
			{
				float rotation = anim * 360 * (float)(rows - i);
				ofRotate(rotation);
				ofRotate(360. / ((float)i + 5.)*(float)j);

				ofTranslate(step*(i+1), 0, 0);
				ofDrawCircle(0, 0, radius);
			}
			ofPopMatrix();
		}
	}
	ofPopMatrix();

	fboArt.end();
	FrameGeometry::draw();		
}

void DotsRotation::update()
{
	timeline->stepTo(ofGetElapsedTimef());
}


void DotsRotation::animSetup()
{
	float start = 0.;
	float end = 1.;
	timeline->apply(&anim,
		start,
		end,
		animTime)
		.finishFn(std::bind(&DotsRotation::animFinish, this));
}

void DotsRotation::animFinish()
{
	animSetup();
}
