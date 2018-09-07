#include "MixedArtViwer.h"
#include "Config.h"

using namespace frames;

void MixedArtViwer::setup(MainArtCreatorPtr artCreator, synapse::ConfigPtr config)
{
	this->config = config;
	
	ofSetWindowShape(config->getScreenWidthLarge(), config->getScreenHeight());

	setLargeFader();

	viewerLarge = OneLargeArtworkPtr(new OneLargeArtwork());
	viewerLarge->setup(artCreator, config);

	viewerThreeArt = ThreeArtsViewerPtr(new ThreeArtsViewer());
	viewerThreeArt->setup(artCreator, config);
	
	changeArt();	
}

void MixedArtViwer::changeArt()
{
	if (config->configRotator.size() == 0)
	{
		return;
	}

	currentRotator = config->configRotator[index++];
	
	if (index > config->configRotator.size() - 1)
	{
		index = 0;
	}

	switch (currentRotator.viewType)
	{
	case frames::VIEW_TYPES::ONE_LARGE_ARTWORK:
		viewerCurrent = viewerLarge;
		break;

	case frames::VIEW_TYPES::THREE_ARTWORKS:
		viewerCurrent = viewerThreeArt;
		break;
	}

	viewerCurrent->setOwnResolution();
	viewerCurrent->setArtRotator(currentRotator.oneArtSeconds);
	viewerCurrent->nextArt();

	curentSaveTime = ofGetElapsedTimef();
}

void MixedArtViwer::setOwnResolution()
{
	
}

void MixedArtViwer::update()
{
	viewerCurrent->update();
}

void MixedArtViwer::draw()
{
	viewerCurrent->draw();
	drawFader();
	timeRotatorCheck();
}

void MixedArtViwer::clean()
{

}

void MixedArtViwer::setArtRotator(int seconds)
{
	
}

void MixedArtViwer::nextArt()
{
	changeArt();
}

ofTexture MixedArtViwer::getTexture() const
{
	return viewerCurrent->getTexture();
}

void MixedArtViwer::timeRotatorCheck()
{
	mTimeline->stepTo(ofGetElapsedTimef());

	const float timeLeft = ofGetElapsedTimef() - curentSaveTime - currentRotator.typeSeconds;
	const float secondsBeforeChangeArt = 2;
	const float fadeInTime = 1.8f;

	if (timeLeft > -secondsBeforeChangeArt && !faderAnimated)
	{
		faderAnimated = true;
		mTimeline->apply(&faderAlpha,
			0.0f,
			1.0f,
			fadeInTime,
			ofxCinderTimeline::EaseInOutCubic())
			.finishFn(std::bind(&MixedArtViwer::fadeinCompleteHandler, this));
	}

	if (timeLeft > 0)
	{
		changeArt();
	}
}

void MixedArtViwer::fadeinCompleteHandler()
{
	const float fadeOutTime = 1.8f;

	mTimeline->apply(&faderAlpha,
		1.0f,
		0.0f,
		fadeOutTime,
		ofxCinderTimeline::EaseInOutCubic())
		.finishFn(std::bind(&MixedArtViwer::fadeOutCompleteHandler, this));
}

void MixedArtViwer::fadeOutCompleteHandler()
{
	faderAnimated = false;
}
