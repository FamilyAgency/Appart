#pragma once
#include "FrameGeometry.h"

namespace frames
{
	typedef shared_ptr<class MondrianAnimated> MondrianAnimatedPtr;

	class MondrianAnimated : public FrameGeometry
	{
	public:
		MondrianAnimated(float width, float height);
		virtual ~MondrianAnimated();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;
		virtual void draw() override;

		static float strWeight;

	private:
		float factor, inc;
		int counter;
		bool move = true;
		int splitCount = 4;		

		vector<int> movingIndexes;		
		vector<class MondrianRect> rectangles;	

		vector<MondrianRect> splitRectangles(const vector<MondrianRect>& rectangles);
		void initMovers();
		void initRectangles();
		int generateUniqueRandomInt(int size, const set<int>& _set);

		ofxColorSlider colorGui1, colorGui2, colorGui3;
		ofColor color1, color2, color3;
	};

	class MondrianRect
	{
	public:
		float rx, ry, rw, rh;
		float strWeight;
		
		MondrianRect();
		MondrianRect(float x, float y, float width, float height);

		void set(float x, float y, float width, float height);
		void draw();
		void drawW(float factor);
		void drawH(float factor);
	};
}
