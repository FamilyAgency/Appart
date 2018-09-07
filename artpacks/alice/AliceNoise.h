#pragma once
#include "FrameShader.h"

namespace frames
{
	typedef shared_ptr<class AliceNoise> AliceNoisePtr;

	class AliceNoise : public FrameShader
	{
	public:
		AliceNoise(float width, float height);
		virtual ~AliceNoise();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;

	protected:
		virtual string getFragSrc();

	private:
	};
}
