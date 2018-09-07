#pragma once
#include "FrameGeometry.h"
#include "ofxCinderTimeline.h"

namespace frames
{
	// short name for smart pointer
	typedef shared_ptr<class DotsRotation> DotsRotationPtr;

	class DotsRotation : public FrameGeometry
	{
	public:
		DotsRotation(float width, float height);
		virtual ~DotsRotation();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;
		virtual void draw() override;
	private:
		int rows = 20;
		float step = 15;
		float radius = 5;
		float animTime = 40.f;

		ofColor mainColor;
		ofColor bgColor;

		ofPoint screenCenter;

		std::shared_ptr<ofxCinderTimeline::Timeline> timeline;
		ofxCinderTimeline::Anim<float> anim;
		void animSetup();
		void animFinish();

	};
}
