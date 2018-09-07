#pragma once
#include "FrameGeometry.h"
#include "ofxCinderTimeline.h"

namespace frames
{
	// short name for smart pointer
	typedef shared_ptr<class AnimGeometryExample> AnimGeometryExamplePtr;

	class AnimGeometryExample : public FrameGeometry
	{
	public:
		AnimGeometryExample(float width, float height);
		virtual ~AnimGeometryExample();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;
		virtual void draw() override;

	protected:

	private:
		std::shared_ptr<ofxCinderTimeline::Timeline> mTimeline;
		ofxCinderTimeline::Anim<ofPoint> circlePosition;

		void animationFinishedHandler1();
		void animationFinishedHandler2();

		void firstAnimationSetup();
		void secondAnimationSetup();
	};
}
