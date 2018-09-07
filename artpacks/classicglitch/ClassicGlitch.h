#pragma once
#include "FrameShader.h"

namespace frames
{
	typedef shared_ptr<class ClassicGlitch> ClassicGlitchPtr;

	class ClassicGlitch : public FrameShader
	{
	public:
		ClassicGlitch(float width, float height);
		virtual ~ClassicGlitch();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;

	protected:
		virtual string getFragSrc();

	private:
		ofImage img;
		vector<ofImage> images;
		int index = 0;
	};
}
