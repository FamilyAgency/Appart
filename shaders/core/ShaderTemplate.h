#pragma once
#include "FrameShader.h"

namespace frames
{
	typedef shared_ptr<class ShaderTemplate> ShaderTemplatePtr;

	class ShaderTemplate : public FrameShader
	{
	public:
		ShaderTemplate(float width, float height);
		virtual ~ShaderTemplate();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;

	protected:
		virtual string getFragSrc();

	private:		
		ofxFloatSlider scale, param, param1;
		static const int cellsNum = 4;
		const float animSeconds = 3.;
		float mat1[cellsNum];
		float mat2[cellsNum];
	};
}
