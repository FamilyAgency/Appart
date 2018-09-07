#include "FrameGeometry.h"
using namespace frames;

FrameGeometry::FrameGeometry(float width, float height):ArtDrawable(width, height)
{
	spoutChannelName = "Geometry Art";
	name = "Concrete name";
}

FrameGeometry::~FrameGeometry()
{
#ifndef release
	gui.unregisterMouseEvents();
	gui.clear();
#endif
}

void FrameGeometry::GUISetup()
{
#ifndef release
	gui.setup();
#endif
}

void FrameGeometry::setup()
{

}

void FrameGeometry::update()
{

}

void FrameGeometry::draw()
{	
	ArtDrawable::draw();
#ifndef release
	if (guiVisible)
	{
		gui.draw();
	}
#endif
}

void FrameGeometry::clean()
{

}

void FrameGeometry::resolutionChangedListener(const ofVec2f& resolution)
{
	ArtDrawable::resolutionChangedListener(resolution);
}

