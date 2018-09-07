#pragma once

#include "ofMain.h"
#include "FrameGeometry.h"

namespace frames
{
	typedef shared_ptr<class Squares> SquaresPtr;

	class Squares : public FrameGeometry
	{
	public:
		Squares(float width, float height);
		virtual ~Squares();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;
		virtual void draw() override;		

		void status();

	private:
		enum class squareType
		{
			MAIN,
			PIECE,
			SIZE
		};
	
		float rotateAngle;
		//float generalScaleDefault = 1;
		float generalScale;// = generalScaleDefault;
	
		void rotate(float rotateSpeed);
		void squaresSetup(float size);

		struct Transformation
		{
			float x;
			float y;
			float scale;
			float dist;
			float angle;
			int delay;
			squareType squareType;
		} trans;

		vector<Transformation> defaultSquares;
		vector<Transformation> squares;
		vector<int> position;
		vector<float> scale;

		Transformation* last;

		ofxColorSlider bgColor;
		ofxColorSlider colorGui1;
		ofxFloatSlider rotateSpeed;
		ofxFloatSlider size;
		ofxFloatSlider speed;
		ofxFloatSlider generalScaleSpeed;
		ofxFloatSlider generalScaleDefault;
		ofxIntSlider delaySpeed;

		void sizeChanged(float& value);
	};
}
