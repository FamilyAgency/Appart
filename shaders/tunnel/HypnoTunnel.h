#pragma once
#include "FrameShader.h"

namespace frames
{
	typedef shared_ptr<class HypnoTunnel> HypnoTunnelPtr;

	class HypnoTunnel : public FrameShader
	{
	public:
		HypnoTunnel(float width, float height, float speed = 2.5, float fov = 0.3, const ofColor& color = ofColor(225, 120, 20));
		virtual ~HypnoTunnel();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;

	protected:
		virtual string getFragSrc();

	private:
		ofxFloatSlider speed;
		ofxFloatSlider fov;
		ofxColorSlider colorGui1;

		float 	defaultSpeed, defaultFov;
		ofColor	defaultColor;
	};
}
