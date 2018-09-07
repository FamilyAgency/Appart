#include "ArtDrawable.h"
#include "../Config/Config.h"

using namespace frames;

ArtDrawable::ArtDrawable(int width, int height)
{
	this->width = width;
	this->height = height;	
	guiVisible = true;

	fboArtSamples = 2;
	fboArt.allocate(width, height, GL_RGBA, fboArtSamples);
}

void ArtDrawable::initFbo(int samples)
{
	//TODO
}

ArtDrawable::~ArtDrawable()
{

}

void ArtDrawable::GUIvisible(bool value)
{
	guiVisible = value;
}

void ArtDrawable::draw()
{
	fboArt.draw(0, 0);
}

ofTexture ArtDrawable::getTexture() const
{
	return fboArt.getTexture();
}

string ArtDrawable::getSpoutChannelName() const
{
	return spoutChannelName;
}

void ArtDrawable::setResolution(int width, int height)
{
	if (width == this->width && height == this->height)
	{
		return;
	}

	setWidth(width);
	setHeight(height);
	fboArt.destroy();
	fboArt.allocate(width, height, GL_RGBA , fboArtSamples);
	auto vec = ofVec2f(width, height);
	//ofNotifyEvent(resolutionChangedEvent, vec);
	resolutionChangedListener(vec);
}

void ArtDrawable::setWidth(int width)
{
	this->width = width;
}

void ArtDrawable::setHeight(int height)
{
	this->height = height;
}

int ArtDrawable::getWidth() const
{
	return width;
}

int ArtDrawable::getHeight() const
{
	return height;
}

void ArtDrawable::resolutionChangedListener(const ofVec2f& resolution)
{

}

string ArtDrawable::getName() const
{
	return name; 
}


