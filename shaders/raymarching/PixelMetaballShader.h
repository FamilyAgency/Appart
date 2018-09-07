#pragma once
#include "FrameShader.h"

namespace frames
{
	typedef shared_ptr<class PixelMetaballShader> PixelMetaballShaderPtr;

	class PixelMetaballShader : public FrameShader
	{
	public:
		PixelMetaballShader(float width, float height);
		virtual ~PixelMetaballShader();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;

	protected:
		virtual string getFragSrc();

	private:
		ofxFloatSlider guiscale;
		float scale;
		int direction;
	};
}
