#include "AbstractViewer.h"
#include "Config.h"

using namespace frames;

void AbstractViewer::setup(MainArtCreatorPtr artCreator, synapse::ConfigPtr config)
{
	this->config = config;
}

void AbstractViewer::setLargeFader()
{
	int w = config->getScreenWidthLarge();
	int h = config->getScreenHeight();
	setFader(w, h);
}

void AbstractViewer::setSmallFader()
{
	int w = config->getScreenWidth();
	int h = config->getScreenHeight();
	setFader(w, h);
}

void AbstractViewer::setFader(int w, int h)
{
	faderAnimated = false;
	faderAlpha = 0.0f;
	faderRectangle = ofRectangle(0, 0, w, h);
	mTimeline = ofxCinderTimeline::Timeline::create();
	mTimeline->stepTo(ofGetElapsedTimef());
}

void AbstractViewer::timeRotatorCheck()
{
	mTimeline->stepTo(ofGetElapsedTimef());

	const float timeLeft = ofGetElapsedTimef() - curentSaveTime - secondsArtShow;
	const float secondsBeforeChangeArt = 2;
	const float fadeInTime = 1.8f;

	if (config->getArtRotateMode() && timeLeft > - secondsBeforeChangeArt && !faderAnimated)
	{
		faderAnimated = true;
		mTimeline->apply(&faderAlpha,
			0.0f,
			1.0f,
			fadeInTime,
			ofxCinderTimeline::EaseInOutCubic())
			.finishFn(std::bind(&AbstractViewer::fadeinCompleteHandler, this));
	}

	if (config->getArtRotateMode() && timeLeft > 0)
	{
		nextArt();
	}
}

void AbstractViewer::fadeinCompleteHandler()
{
	const float fadeOutTime = 1.8f;

	mTimeline->apply(&faderAlpha,
		1.0f,
		0.0f,
		fadeOutTime,
		ofxCinderTimeline::EaseInOutCubic())
		.finishFn(std::bind(&AbstractViewer::fadeOutCompleteHandler, this));
}

void AbstractViewer::fadeOutCompleteHandler()
{
	faderAnimated = false;
}

void AbstractViewer::drawFader()
{
	ofSetColor(0, 0, 0, faderAlpha * 255);
	ofDrawRectangle(faderRectangle);
	ofSetColor(255, 255, 255, 255);
}

string AbstractViewer::getSpoutChannelName() const
{
	return spoutChannelName;
}


