#pragma once
#include "FrameGeometry.h"
#include <vector>
#include <algorithm>
#include <random>
#include "ofxCinderTimeline.h"

namespace frames
{
	typedef shared_ptr<class SnakesLife> SnakesLifePtr;

	typedef shared_ptr<struct PointData> PointDataPtr;
	struct PointData
	{
		ofPoint point;
		PointDataPtr top;
		PointDataPtr right;
		PointDataPtr bot;
		PointDataPtr left;
		bool isTop = false;
		bool isRight = false;
		bool isBot = false;
		bool isLeft = false;
		bool addedTop = false;
		bool addedRight = false;
		bool addedBot = false;
		bool addedLeft = false;
	};
	struct SquareData
	{
		ofPoint point;
		PointDataPtr topLeft;
		PointDataPtr topRight;
		PointDataPtr botLeft;
		PointDataPtr botRight;
		bool isFilled = false;
		bool inTrans = false;
		ofColor color;
	};
	struct Line
	{
		ofPoint p1;
		ofPoint p2;
	};

	struct Snake
	{
		PointDataPtr currentPoint;
		PointDataPtr nextPoint;
		vector<int> selector;
	};

	class SnakesLife : public FrameGeometry
	{
	public:
		SnakesLife(float width, float height);
		virtual ~SnakesLife();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;
		virtual void draw() override;

	private:
		const int gridSpacing = 45;
		const int tickRate = 30;
		const int numSnakes = 6;
		ofVec2f gridSize;

		vector<ofColor> squareColorsVec;
		ofColor squareActiveColor;
		ofColor squarePassiveColor;
		ofColor snakeColor;
		ofColor lineColor;

		Snake snake;
		vector<Snake> snakes;
		vector<Line> lines;
		vector<vector<PointDataPtr>> grid;
		vector<vector<SquareData>> squareGrid;

		int squareCount;

		void createGrid(int width, int height);
		void createSquareGrid(int width, int height);
		void resetSquaresAnim();
		void tick();
		void createSnake();
		void findNext();
		void findNext(int index);
		void stepNext();
		void respawn(int index);
		void drawSquares();
		void drawSnake();
		void checkSquares(PointDataPtr point1, PointDataPtr point2);
		void checkSquare(ofPoint point);
		void reset();

		shared_ptr<ofxCinderTimeline::Timeline> timeline;
		ofxCinderTimeline::Anim<float> anim;
		void animSetup();
		void animFinish();
		ofxFloatSlider animation;
	};
}
