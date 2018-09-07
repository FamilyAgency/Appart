#pragma once
#include "FrameGeometry.h"
#include "ofxCinderTimeline.h"

namespace frames
{
	typedef shared_ptr<class CubesWave> CubesWavePtr;

	class CubesWave : public FrameGeometry
	{
	public:
		CubesWave(float width, float height);
		virtual ~CubesWave();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;
		virtual void draw() override;

	private:
		const float boxW = 5;
		const int maxBoxW = 50;
		const int maxBoxH = 50;

		ofxFloatSlider rotateX;
		ofxFloatSlider rotateY;
		ofxFloatSlider rotateZ;

		ofxFloatSlider lightX;
		ofxFloatSlider lightY;
		ofxFloatSlider lightZ;

		ofxFloatSlider period;
		ofxFloatSlider animTime;
		ofxFloatSlider waveHeight;

		ofColor bgColor;
		ofColor mainColor;
		ofEasyCam cam;
		ofLight pointLight;
		ofMaterial material;

		std::shared_ptr<ofxCinderTimeline::Timeline> timeline;
		ofxCinderTimeline::Anim<float> moveZ;
		void moveZSetup();
		void moveZFinish();

		float spaceDiagonal(float size);
		float faceDiagonal(float size);

	};
}
