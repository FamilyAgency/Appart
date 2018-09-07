#include "UlamSpiral.h"

using namespace frames;

UlamSpiral::UlamSpiral(float width, float height) : FrameGeometry(width, height)
{
	spoutChannelName = "Geometry Art";
	name = "Ulam Spiral Art";	
}

UlamSpiral::~UlamSpiral()
{

}

void UlamSpiral::setup()
{
	
}

void UlamSpiral::GUISetup()
{
#ifndef release
	FrameGeometry::GUISetup();

	//gui.add(size.setup("Cube size", 250., 10., 1000.0));
	//gui.add(bgColor.setup("BG color", ofColor(0, 0, 0), ofColor(0, 0), ofColor(255, 255)));

	//code for gui setup below
	//...
#endif
}

void UlamSpiral::draw()
{
	// draw in FBO
	fboArt.begin();
	ofClear(255, 255, 255, 0);
	ofBackground(bgColor);
	fboArt.end();

	FrameGeometry::draw();
}

void UlamSpiral::update()
{

	FrameGeometry::update();
}
