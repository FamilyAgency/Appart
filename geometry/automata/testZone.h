#pragma once
#include "FrameGeometry.h"

namespace frames
{
	// short name for smart pointer
	typedef shared_ptr<class testZone> testZonePtr;

	class testZone : public FrameGeometry
	{
	public:
		testZone(float width, float height);
		virtual ~testZone();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;
		virtual void draw() override;

	protected:
		ofFbo fbo1;
		ofFbo fbo2;
		ofFbo mask;
		ofFbo shaderFbo;
		ofImage img1;
		ofImage img2;
		ofShader shader;

		ofxFloatSlider lolfloat;
		ofxColorSlider maskColor;

		ofColor maskColorRGB;
	private:
		string testZone::getFragSrc();
		string testZone::getVertSrc();
	};
}
