#pragma once
#include "FrameShader.h"

namespace frames
{
	typedef shared_ptr<class SimpleNoise> SimpleNoisePtr;

	class SimpleNoise : public FrameShader
	{
	public:
		SimpleNoise(float width, float height, int octaves = 45, float mult = 2., const ofVec2f& shift = ofVec2f(100., 100.), const ofColor& color1 = ofColor(120., 100., 140.), const ofColor& color2 = ofColor(100, 10, 14));

		virtual ~SimpleNoise();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;

	protected:
		virtual string getFragSrc();

	private:
		ofxVec2Slider shift;
		ofxFloatSlider mult;
		ofxIntSlider numOctaves;
		ofxColorSlider colorGui1, colorGui2;

		int defaultOctaves;
		ofVec2f defaultShift;
		float	defaultMult;
		ofColor	defaultColor1, defaultColor2;

	};
}