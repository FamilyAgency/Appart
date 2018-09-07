#include "GeometryTemplate.h"

using namespace frames;

GeometryTemplate::GeometryTemplate(float width, float height) : FrameGeometry(width, height)
{
	spoutChannelName = "Geometry Art";
	name = "Template name";
}

GeometryTemplate::~GeometryTemplate()
{

}

void GeometryTemplate::setup()
{
	// setup init values
}

void GeometryTemplate::GUISetup()
{
#ifndef release
	FrameGeometry::GUISetup();

	//code for gui setup below
	//...
#endif
}

void GeometryTemplate::draw()
{	
	// draw in FBO
	fboArt.begin();
	ofClear(255, 255, 255, 0);

	// code here

	fboArt.end();

	//FBO draws in parent
	FrameGeometry::draw();		
}

void GeometryTemplate::update()
{
	// update algorithm here
}
