#include "TwoCircles.h"

using namespace frames;

TwoCircles::TwoCircles(float width, float height) : FrameGeometry(width, height)
{
	spoutChannelName = "Geometry Art";
	name = "Two Circles Art";

	timeline = ofxCinderTimeline::Timeline::create();
	timeline->stepTo(ofGetElapsedTimef());
	color1.set(200, 200, 200);
	color2.set(55, 55, 55);
	setupCount = 0;
	setupColors();
}

TwoCircles::~TwoCircles()
{

}

void TwoCircles::setup()
{
	setupCount++;
	setColors();

	activeColor.set(color1);
	passiveColor.set(color2);
	rightScale();
}

void TwoCircles::GUISetup()
{
#ifndef release
	FrameGeometry::GUISetup();
#endif
}

void TwoCircles::draw()
{	
	//slider = circleScale;
	// draw in FBO
	fboArt.begin();
	ofClear(255, 255, 255, 0);
	ofBackground(passiveColor);

	// code here
	ofPushMatrix();
	{	//center of the screen
		ofTranslate(getWidth() / 2, getHeight() / 2);
		ofSetColor(255, 0, 0);
		//ofLine(0, -2000, 0, 2000);
		ofPushMatrix();
		{//circle matrix
			if (statePos == StatePos::RIGHT)
			{
				float upper = 0.2;
				float lower = 0.;
				ofTranslate(circleScale - scaleMin*(1 - animLin), 0);
			}
			else
			{
				ofTranslate(circleScale*-1 + scaleMin*(1 - animLin), 0);
			}
			
			ofSetColor(activeColor);
			ofSetCircleResolution(500);
			ofCircle(0, 0, circleScale);
			ofSetCircleResolution(100);
		}
		ofPopMatrix();
	}
	ofPopMatrix();

	fboArt.end();

	//FBO draws in parent
	FrameGeometry::draw();		
}

void TwoCircles::update()
{
	timeline->stepTo(ofGetElapsedTimef());
	circleScale = tan(anim * PI / 2) + scaleMin;
}

void frames::TwoCircles::rightScale()
{
	statePos = StatePos::RIGHT;
	float animtime = scaleTime;
	float startPosition = animMin;
	float endPosition = animMax;
	timeline->apply(&anim,
		startPosition,
		endPosition,
		animtime,
		ofxCinderTimeline::EaseOutExpo())
		.finishFn(std::bind(&TwoCircles::rightScaleFin, this));
	timeline->apply(&animLin,
		startPosition,
		endPosition,
		animtime);
}

void frames::TwoCircles::rightScaleFin()
{
	switchColor();
	leftScale();
}

void frames::TwoCircles::leftScale()
{
	statePos = StatePos::LEFT;
	float animtime = scaleTime;
	float startPosition = animMax;
	float endPosition = animMin;
	timeline->apply(&anim,
		startPosition,
		endPosition,
		animtime,
		ofxCinderTimeline::EaseInCirc())
		.finishFn(std::bind(&TwoCircles::leftScaleFin, this));
	timeline->apply(&animLin,
		startPosition,
		endPosition,
		animtime);
}

void frames::TwoCircles::leftScaleFin()
{	
	rightScale();
}

void frames::TwoCircles::switchColor()
{
	if (activeColor == color1)
	{
		activeColor.set(color2);
		passiveColor.set(color1);
	}
	else
	{
		activeColor.set(color1);
		passiveColor.set(color2);
	}
}

void TwoCircles::setupColors()
{
	//activeColor
	//passiveColor
	if (colorBase.empty())
	{
		vector<ofColor> colorPair;

		colorPair.clear();
		colorPair.push_back(ofColor(137, 210, 220));
		colorPair.push_back(ofColor(35, 46, 209));
		colorBase.push_back(colorPair);

		colorPair.clear();
		colorPair.push_back(ofColor(137, 210, 220));
		colorPair.push_back(ofColor(35, 46, 209));
		colorBase.push_back(colorPair);

		colorPair.clear();
		colorPair.push_back(ofColor(217, 3, 104));
		colorPair.push_back(ofColor(130, 2, 99));
		colorBase.push_back(colorPair);

		colorPair.clear();
		colorPair.push_back(ofColor(229, 98, 94));
		colorPair.push_back(ofColor(255, 255, 255));
		colorBase.push_back(colorPair);

		colorPair.clear();
		colorPair.push_back(ofColor(133, 199, 242));
		colorPair.push_back(ofColor(99, 99, 99));
		colorBase.push_back(colorPair);
	}
}

void frames::TwoCircles::setColors()
{
	float colorIndex = (setupCount) % colorBase.size();
	//cout << colorIndex << endl;
	color1 = colorBase[colorIndex][0];
	color2 = colorBase[colorIndex][1];
}
