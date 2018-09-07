#pragma once
#include "FrameGeometry.h"
#include "ofxCinderTimeline.h"

namespace frames
{
	typedef shared_ptr<class CubesArray> CubesArrayPtr;

	class CubesArray : public FrameGeometry
	{
	public:
		CubesArray(float width, float height);
		virtual ~CubesArray();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;
		virtual void draw() override;

	private:
		const float boxW = 10;
		int maxBoxW = 15;
		int maxBoxH = 15;

		ofxFloatSlider rotateX;
		ofxFloatSlider rotateY;
		ofxFloatSlider rotateZ;

		ofxFloatSlider lightX;
		ofxFloatSlider lightY;
		ofxFloatSlider lightZ;

		ofxFloatSlider period;
		ofxFloatSlider animTime;
		ofxFloatSlider waveHeight;

		ofxFloatSlider camX;
		ofxFloatSlider camY;
		ofxFloatSlider camZ;

		ofColor bgColor;
		ofColor mainColor;
		ofEasyCam cam;
		ofLight pointLight;
		ofMaterial material;

		std::shared_ptr<ofxCinderTimeline::Timeline> timeline;
		ofxCinderTimeline::Anim<float> rotate;
		void rotateSetup();
		void rotateFinish();

		float spaceDiagonal(float size);
		float faceDiagonal(float size);

		bool isHorizontal();
	
		float remap(float value, float min, float max);
	};
}
