#include "FrameShader.h"
using namespace frames;

FrameShader::FrameShader(float width, float height)
	: AbstractShader(),
	  ArtDrawable(width, height)
{
	spoutChannelName = "Shader Art";	
	name = "Concrete name";

	addParameter(new Parameter2f("u_resolution", ofVec2f(width, height)));
	addParameter(new ParameterF("u_time", 0.0));	

	//ofAddListener(resolutionChangedEvent, this, &FrameShader::resolutionChangedListener);
}

FrameShader::~FrameShader()
{
#ifndef release
	gui.unregisterMouseEvents();
	gui.clear();
#endif
}

void FrameShader::GUISetup()
{
#ifndef release
	gui.setup();
#endif
}

void FrameShader::draw()
{
	ArtDrawable::draw();
#ifndef release
	if (guiVisible)
	{
		gui.draw();
	}
#endif
}

void FrameShader::update()
{
	updateParameter("u_time", float((ofGetElapsedTimeMillis() - startTime) / 1000.0));

	fboArt.begin();
	ofClear(255, 255, 255, 0);
	begin();
	ofDrawRectangle(0, 0, getWidth(), getHeight());
	//fboMask.draw(0, 0);	
	end();
	fboArt.end();
}

void  FrameShader::clean()
{

}

void FrameShader::resolutionChangedListener(const ofVec2f& resolution)
{	
	//ofLog(ofLogLevel::OF_LOG_NOTICE, "resolution changed");	
	updateParameter("u_resolution", resolution);	
	ArtDrawable::resolutionChangedListener(resolution);
}

