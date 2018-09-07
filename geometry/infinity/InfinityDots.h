#pragma once
#include "FrameGeometry.h"
#include "ofxCinderTimeline.h"

namespace frames
{
	typedef shared_ptr<class InfinityDots> InfinityDotsPtr;

	class InfinityDots : public FrameGeometry
	{
	public:
		InfinityDots(float width, float height);
		virtual ~InfinityDots();

		virtual void setup()	override;
		virtual void GUISetup() override;
		virtual void update()	override;
		virtual void draw()		override;
		virtual void clean()	override;

	private:
		struct DotParam
		{
			float x;
			float y;
		};
		
		const int numDots = 10;
		float circleScale;
		float generalRotate;
		float radius;
		float merge;
		float cicrleSpacing;
		vector<DotParam> dots;

		std::shared_ptr<ofxCinderTimeline::Timeline> myTimeline;
		ofxCinderTimeline::Anim<float> scale;
		ofxCinderTimeline::Anim<float> dotsScale;

		ofFbo mask;
		ofFbo image;

		ofxColorSlider bgColor;
		ofxColorSlider crColor;
		ofxFloatSlider endScalePar;
		ofxFloatSlider scaleTime;
		ofxFloatSlider generalRotateSpeed;
		ofxLabel	  endScaleParLabel;	

		void setDots();	
		void scaleCallback();
		void scaleAnimSetup();
		void dotsScaleCallback();
		void dotsScaleSetup();
	};
}
