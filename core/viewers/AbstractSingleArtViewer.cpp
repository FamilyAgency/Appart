#include "AbstractSingleArtViewer.h"

using namespace frames;

void AbstractSingleArtViewer::setup(MainArtCreatorPtr artCreator, synapse::ConfigPtr config)
{
	AbstractViewer::setup(artCreator, config);

	//currentArtType = frames::config().defaultArtType;
	currentArtNumber = 0;

	switchArt(currentArtNumber);

	if (config->getArtRotateMode())
	{
		setArtRotator(config->getChangeArtSeconds());
	}
}

void AbstractSingleArtViewer::update()
{
	ofSetWindowTitle("FPS: " + ofToString(ofGetFrameRate()));
	art->update();
}

void AbstractSingleArtViewer::draw()
{	
	art->draw();
	drawFader();
	//TODO: 
	timeRotatorCheck();
}

void AbstractSingleArtViewer::setArtRotator(int seconds)
{
	secondsArtShow = seconds;

	for (auto& it : artVector)
	{
		it->GUIvisible(false);
	}

	curentSaveTime = ofGetElapsedTimef();
}

void AbstractSingleArtViewer::nextArt()
{
	if (++currentArtNumber >= artVector.size())
	{
		currentArtNumber = 0;
	}

	switchArt(currentArtNumber);

	curentSaveTime = ofGetElapsedTimef();
}

void AbstractSingleArtViewer::switchArt(int value)
{
	if (art)
	{
		art->clean();
	}

	art = artVector[value];
	art->setup();
	spoutChannelName = art->getSpoutChannelName();
}

ofTexture AbstractSingleArtViewer::getTexture() const
{
	return art->getTexture();
}

void AbstractSingleArtViewer::clean()
{

}

void AbstractSingleArtViewer::setForceArt(FRAME_TYPES type)
{
	for (size_t i = 0; i < artVector.size(); i++)
	{
		if (artVector[i]->getType() == type)
		{
			switchArt(i);
			break;
		}
	}
}