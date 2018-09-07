#pragma once
#include "FrameShader.h"

namespace frames
{
	typedef shared_ptr<class ApolloFractal> ApolloFractalPtr;

	class ApolloFractal : public FrameShader
	{
	public:
		ApolloFractal::ApolloFractal(float width, float height, float scale = 2., float param1 = 2., float param2 = 2.195, float param3 = 1.07);
		virtual ~ApolloFractal();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;

	protected:
		virtual string getFragSrc();

	private:
		ofxFloatSlider scale, param, param1, param2;
		float defaultscale, defaultparam1, defaultparam2, defaultparam3;
	};
}
