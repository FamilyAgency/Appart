#pragma once
#include "FrameShader.h"

namespace frames
{
	typedef shared_ptr<class DistortGrid> DistortGridPtr;

	class DistortGrid : public FrameShader
	{
	public:
		DistortGrid(float width, float height);
		virtual ~DistortGrid();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;

	protected:
		virtual string getFragSrc();

	};
}
