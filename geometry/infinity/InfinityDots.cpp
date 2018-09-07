#include "InfinityDots.h"

using namespace frames;

InfinityDots::InfinityDots(float width, float height) : FrameGeometry(width, height)
{	
	spoutChannelName = "Geometry Art";
	name = "Geometry Infinity Dots";

	myTimeline = ofxCinderTimeline::Timeline::create();
	myTimeline->stepTo(ofGetElapsedTimef());
	radius = getHeight() * 0.25 * 0.5;
	merge = radius * 4;
}

InfinityDots::~InfinityDots()
{

}

void InfinityDots::setup()
{
	cicrleSpacing = radius * 2 + merge;

	setDots();
	mask.allocate(getWidth(), getHeight(), GL_RGBA);
	image.allocate(getWidth(), getHeight(), GL_RGBA);
	scaleAnimSetup();
	dotsScaleSetup();
	generalRotate = 0;
	ofSetCircleResolution(100);
}

void InfinityDots::clean()
{
	mask.destroy();
	image.destroy();
}

void InfinityDots::GUISetup()
{
#ifndef release
	FrameGeometry::GUISetup();

	gui.add(bgColor.setup("BG color", ofColor(0, 0, 0), ofColor(0, 0), ofColor(255, 255)));
	gui.add(crColor.setup("circle color", ofColor(255, 255, 255), ofColor(0, 0, 0), ofColor(255, 255, 255)));
	gui.add(endScalePar.setup("endScale testing", 0.0245229, 0.02, 0.03));
	gui.add(scaleTime.setup("scale time ", 3.f, 0.5, 10.));
	gui.add(generalRotateSpeed.setup("Rotate Speed", 1.0, 0.1, 10.));
	gui.add(endScaleParLabel.setup("EndScaleParLabel", to_string(endScalePar)));
#else
	bgColor.setup("BG color", ofColor(0, 0, 0), ofColor(0, 0), ofColor(255, 255));
	crColor.setup("circle color", ofColor(255, 255, 255), ofColor(0, 0, 0), ofColor(255, 255, 255));
	endScalePar.setup("endScale testing", 0.0245229, 0.02, 0.03);
	scaleTime.setup("scale time ", 3.f, 0.5, 10.);
	generalRotateSpeed.setup("Rotate Speed", 1.0, 0.1, 10.);
	endScaleParLabel.setup("EndScaleParLabel", to_string(endScalePar));
#endif
}

void InfinityDots::draw()
{	
	fboArt.begin();
	ofClear(0, 0, 0, 0);

	image.begin();
	{
		ofClear(0, 0, 0, 0);

		ofSetColor(crColor);
		ofSetRectMode(OF_RECTMODE_CENTER);
		ofTranslate(getWidth()*0.5, getHeight()*0.5);
		generalRotate = generalRotate + generalRotateSpeed;
		ofRotateZ(generalRotate);
		ofPushMatrix();
		{
			ofScale(scale, scale);
			ofSetColor(0, 0, 255);
			ofCircle(0, 0, 10);
			ofSetColor(crColor);
			for (int i = 0; i < dots.size(); i++)
			{
				ofDrawCircle(dots[i].x, dots[i].y, dotsScale*radius);
			}
		}
		ofPopMatrix();
	}
	image.end();
	
	mask.begin();
	{
		ofPushMatrix();
		{
			ofClear(0, 0, 0, 0);
			ofSetColor(255);
			ofTranslate(getWidth()*0.5, getHeight()*0.5);
			ofScale(scale, scale);
			ofSetCircleResolution(100);
			ofDrawCircle(0, 0, cicrleSpacing *10);
		}
		ofPopMatrix();
	}
	mask.end();
	
	ofBackground(bgColor);

	image.getTexture().setAlphaMask(mask.getTexture());
	image.draw(0, 0);

	fboArt.end();

	FrameGeometry::draw();
}

void InfinityDots::update()
{
	endScaleParLabel = to_string(endScalePar);
	setDots();
	myTimeline->stepTo(ofGetElapsedTimef());
}

void InfinityDots::setDots()
{
	if (dots.size() > 0)
	{
		dots.clear();
	}
	
	DotParam tempDot;
	for (int row = -numDots; row <= numDots; row++)
	{
		for (int col = -numDots; col <= numDots; col++)
		{
			//TODO:(done) radius * 2 + merge in var
			tempDot.x = col * cicrleSpacing;
			tempDot.y = row * cicrleSpacing;
			dots.push_back(tempDot);
		}
	}
}

void InfinityDots::scaleCallback()
{
	dotsScale = 1.;
	scaleAnimSetup();
	dotsScaleSetup();
}

void InfinityDots::scaleAnimSetup()
{
	float animTime = scaleTime;
	float startScale = 1.;
	float endScale = endScalePar;
	myTimeline->apply(&scale,
		startScale,
		endScale,
		animTime,
		ofxCinderTimeline::EaseNone())
		.finishFn(std::bind(&InfinityDots::scaleCallback, this));
}

void InfinityDots::dotsScaleCallback()
{

}

void InfinityDots::dotsScaleSetup()
{
	float animTime = scaleTime - 0.1f;
	float startDotsScale = 1.;
	float endDotsScale = 4.5;
	myTimeline->apply(&dotsScale,
		startDotsScale,
		endDotsScale,
		animTime,
		ofxCinderTimeline::EaseInExpo())
		.finishFn(std::bind(&InfinityDots::dotsScaleCallback, this));
}