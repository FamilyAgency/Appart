#pragma once
#include "FrameGeometry.h"
#include "ofxCinderTimeline.h"
//#include "graphicTools.h"

namespace frames
{
	// short name for smart pointer
	typedef shared_ptr<class GlitchTest> GlitchTestPtr;

	class GlitchTest : public FrameGeometry
	{
	public:
		GlitchTest(float width, float height);
		virtual ~GlitchTest();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;
		virtual void draw() override;

	protected:
		//int canvasW, canvasH;
		ofImage image;
		ofFbo img;
		ofFbo tempFbo;
		const int steps = 50; //slides per update
		const int brigthnessTick = 10; // more every N updates
		const int blackValue = -16000000;
		int brigthnessValue;
		int whiteValue = 240;
		int minBrigthnessValue = 254;
		int stopBrigthnessValue = 25;

		//void tick();
		void sortColumn(int column);
		int getFirstBright(int x, int y);
		int getNextDark(int x, int y);
		int width, height;
		int column;

		//void fitImage(ofFbo fbo, ofImage img);
		//float getScale(ofFbo fbo, ofImage img);
		//ofPoint getMargin(ofFbo fbo, ofImage img);

		template<typename T> inline int getPcolor(ofColor_<T> c)
		{
			return c.getHex() | 0xff000000;
		};

		//AsdfPixelSort aps;
		//bool saved;
	private:
	};
}
