#pragma once
#include "FrameShader.h"

namespace frames
{
	typedef shared_ptr<class CircleMandala> CircleMandalaPtr;

	class CircleMandala : public FrameShader
	{
	public:
		CircleMandala(float width, float height);
		virtual ~CircleMandala();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;

	protected:
		virtual string getFragSrc();

	private:
		struct Paramdiv
		{
			float value = 0.;
			float increment = 0.1;
			int direction = 1;
			float min = 1.;
			float max = 10.;
		};
		vector<Paramdiv> params;
	};
}
