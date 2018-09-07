#pragma once
#include "FrameShader.h"

namespace frames
{
	typedef shared_ptr<class MetaballShader> MetaballShaderPtr;

	class MetaballShader : public FrameShader
	{
	public:
		MetaballShader(float width, float height);
		virtual ~MetaballShader();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;

	protected:
		virtual string getFragSrc();

	private:
		ofxColorSlider colorGui1;
		ofxColorSlider colorGui2;
		ofxFloatSlider scale;

	};
}
