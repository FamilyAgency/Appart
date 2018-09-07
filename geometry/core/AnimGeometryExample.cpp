#include "AnimGeometryExample.h"

using namespace frames;

AnimGeometryExample::AnimGeometryExample(float width, float height) : FrameGeometry(width, height)
{
	spoutChannelName = "Geometry Template";
	name = "Anim Geometry Template name";
	mTimeline = ofxCinderTimeline::Timeline::create();
	mTimeline->stepTo(ofGetElapsedTimef());
}

AnimGeometryExample::~AnimGeometryExample()
{

}

void AnimGeometryExample::setup()
{
	// setup init values	
	firstAnimationSetup();	
}

void AnimGeometryExample::GUISetup()
{
#ifndef release
	FrameGeometry::GUISetup();

	//code for gui setup below
	//...
#endif
}

void AnimGeometryExample::draw()
{
	// draw in FBO
	fboArt.begin();
	ofClear(255, 255, 255, 0);

	ofSetColor(ofColor::black);
	ofFill();
	ofCircle(circlePosition, 20.0f);

	fboArt.end();

	//FBO draws in parent
	FrameGeometry::draw();
}

void AnimGeometryExample::update()
{
	// update algorithm here
	//mTimeline->stepTo(ofGetElapsedTimef());
}

void AnimGeometryExample::firstAnimationSetup()
{
	float animtime = 3.f;
	auto startPosition = ofPoint(100., 100.);
	auto endPosition = ofPoint(getWidth()*.5, getHeight()*.5);
	mTimeline->apply(&circlePosition,
		startPosition,
		endPosition,
		animtime,
		ofxCinderTimeline::EaseInOutCubic())
		.finishFn(std::bind(&AnimGeometryExample::animationFinishedHandler1, this));

	//mTimeline->clear()
}

void AnimGeometryExample::secondAnimationSetup()
{
	float animtime = 7.f;
	auto endPosition = ofPoint(100., 100.);
	auto startPosition = ofPoint(getWidth()*.5, getHeight()*.5);
	mTimeline->apply(&circlePosition,
		startPosition,
		endPosition,
		animtime,
		ofxCinderTimeline::EaseOutExpo())
		.finishFn(std::bind(&AnimGeometryExample::animationFinishedHandler2, this));
}

void AnimGeometryExample::animationFinishedHandler1()
{
	ofLog(ofLogLevel::OF_LOG_NOTICE, "Finished handler 1");
	secondAnimationSetup();
}

void AnimGeometryExample::animationFinishedHandler2()
{
	ofLog(ofLogLevel::OF_LOG_NOTICE, "Finished handler 2");
	firstAnimationSetup();
}
