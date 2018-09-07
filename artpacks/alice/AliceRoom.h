#pragma once
#include "FrameShader.h"

namespace frames
{
	typedef shared_ptr<class AliceRoom> AliceRoomPtr;

	class AliceRoom : public FrameShader
	{
	public:
		AliceRoom(float width, float height);
		virtual ~AliceRoom();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;

	protected:
		virtual string getFragSrc();

	private:
		ofxFloatSlider plastic;
		ofxFloatSlider swirlness;
		ofxFloatSlider displacement;
		ofxFloatSlider scale;
		ofxColorSlider colorGui1, colorGui2, colorGui3;

		ofImage img;
	};
}
