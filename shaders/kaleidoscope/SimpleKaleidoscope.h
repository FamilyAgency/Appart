#pragma once
#include "FrameShader.h"

namespace frames
{
	typedef shared_ptr<class SimpleKaleidoscope> SimpleKaleidoscopePtr;

	class SimpleKaleidoscope : public FrameShader
	{
	public:
		SimpleKaleidoscope(float width, float height);
		virtual ~SimpleKaleidoscope();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;

	protected:
		virtual string getFragSrc();

	private:
		ofImage texture;
	};
}
