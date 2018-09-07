#pragma once
#include "FrameGeometry.h"
#include "ofxCinderTimeline.h"

namespace frames
{
	typedef shared_ptr<class TwoCircles> TwoCirclesPtr;

	class TwoCircles : public FrameGeometry
	{
	public:
		TwoCircles(float width, float height);
		virtual ~TwoCircles();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;
		virtual void draw() override;

	private:
		enum class StatePos
		{
			RIGHT,
			LEFT
		};
		StatePos statePos;
		enum class stateColor
		{
			WHITE,
			BLACK	
		};
		ofColor color1;
		ofColor color2;
		ofColor activeColor;
		ofColor passiveColor;
		vector<vector<ofColor>> colorBase;

		const float scaleMin = 100.;
		const float scaleMax = 10000;
		const float animMin = 0.;
		const float animMax = 0.999999;
		const float scaleTime = 1.5;

		int setupCount;
		float circleScale;

		std::shared_ptr<ofxCinderTimeline::Timeline> timeline;
		ofxCinderTimeline::Anim<float> anim;
		ofxCinderTimeline::Anim<float> animLin;

		void rightScale();
		void rightScaleFin();

		void leftScale();
		void leftScaleFin();

		void switchColor();

		void setupColors();
		void setColors();
	};
}
