#pragma once
#include "FrameShader.h"

namespace frames
{
	typedef shared_ptr<class RocksShader> RocksShaderPtr;

	class RocksShader : public FrameShader
	{
	public:
		RocksShader(float width, float height);
		virtual ~RocksShader();

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
