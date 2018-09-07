#pragma once
#include "FrameShader.h"

namespace frames
{
	typedef shared_ptr<class PlasticNoise> PlasticNoisePtr;

	class PlasticNoise : public FrameShader
	{
	public:
		PlasticNoise(float width, float height, float timedamp = 1., float scale = 5., float plastic = 0.07, const ofColor& color = ofColor(255., 210., 120.));
		virtual ~PlasticNoise();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;

	protected:
		virtual string getFragSrc();

	private:
		ofImage img;

		ofxFloatSlider plastic;
		ofxFloatSlider scale;
		ofxFloatSlider timeDamp;
		ofxColorSlider colorGui1;

		float timedampDefault, scaleDefault, plasticDefault;
		ofColor	colorDefault;
	};
}