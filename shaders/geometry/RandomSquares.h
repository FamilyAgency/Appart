#pragma once
#include "FrameShader.h"

namespace frames
{
	typedef shared_ptr<class RandomSquares> RandomSquaresPtr;

	class RandomSquares : public FrameShader
	{
	public:
		RandomSquares(float width, float height);
		virtual ~RandomSquares();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;

	protected:
		virtual string getFragSrc();
	};
}
