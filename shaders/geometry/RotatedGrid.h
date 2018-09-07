#pragma once
#include "FrameShader.h"

namespace frames
{
	typedef shared_ptr<class RotatedGrid> RotatedGridPtr;

	class RotatedGrid : public FrameShader
	{
	public:
		RotatedGrid(float width, float height);
		virtual ~RotatedGrid();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;

	protected:
		virtual string getFragSrc();
	};
}
