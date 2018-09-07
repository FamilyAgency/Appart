#pragma once
#include "FrameShader.h"

namespace frames
{
	typedef shared_ptr<class FurNoise> FurNoisePtr;

	class FurNoise : public FrameShader
	{
	public:
		FurNoise(float width,
			float height,
			float swirlness = 1.,
			float scale = 4., 
			float displacement = 0.5,
			const ofColor& color1 = ofColor(255., 123, 100.),
			const ofColor& color2 = ofColor(255., 123, 100.),
			const ofColor& color3 = ofColor(255., 123, 100.));

		virtual ~FurNoise();

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

		float defaultSwirlness, defaultScale, defaultDisplacement;
		ofColor defaultColor1, defaultColor2, defaultColor3;
	};
}
