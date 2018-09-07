#pragma once
#include "FrameGeometry.h"

namespace frames
{
	typedef shared_ptr<class Hexagon> HexagonPtr;

	class Hexagon : public FrameGeometry
	{
	public:
		Hexagon(float width, float height);
		virtual ~Hexagon();

		virtual void setup()	override;
		virtual void GUISetup() override;
		virtual void update()	override;
		virtual void draw()		override;

	private:
		const float length = 120;
		const int sides = 6;
		const int perSide = 3;
		const float delayFactor = 10.;
		void calculateHexagon(float len, float x, float y);

		struct HexagonPoint
		{
			float x;
			float y;
		};
		
		struct TrianglePoints
		{
			float x1;
			float y1;
			float x2;
			float y2;
			float x3;
			float y3;
			float h;
		};
		TrianglePoints tempPoints;

		struct TriangleParam
		{
			float delay;
			float defaultDelay;
			float scale;
		};

		float generalScale;
		float numAround;
		vector<TrianglePoints> Triangles;
		vector<TriangleParam> triangleParams;

		ofxFloatSlider delaySpeed;
		ofxFloatSlider triangleSpeed;
		ofxFloatSlider scaleSpeed;
		ofxFloatSlider scaleTrigger;

		ofxColorSlider hexColor;
		ofxColorSlider bgColor;

		void setTrianglePoints(float length);
		void setDelay();
		void resetDelay();
		void drawHexagon(int size);		
	};
}
