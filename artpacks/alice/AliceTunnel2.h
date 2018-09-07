#pragma once
#include "FrameShader.h"

namespace frames
{
	typedef shared_ptr<class AliceTunnel2> AliceTunnel2Ptr;

	class AliceTunnel2 : public FrameShader
	{
	public:
		AliceTunnel2(float width, float height);
		virtual ~AliceTunnel2();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;

	protected:
		virtual string getFragSrc();

	private:
		ofxFloatSlider plastic;
		ofxFloatSlider swirlness;
		ofxFloatSlider displacement;
		ofxFloatSlider scale;
		ofxColorSlider colorGui1, colorGui2, colorGui3;

		ofImage img;
	};
}
