#pragma once
#include "FrameGeometry.h"
#include "ofxCinderTimeline.h"

namespace frames
{
	typedef shared_ptr<class CubesSteps> CubesStepsPtr;

	class CubesSteps : public FrameGeometry
	{
	public:
		enum class ColorCodes
		{
			COLOR1,
			COLOR2,
			COLOR3
		};

		CubesSteps(float width, float height);
		CubesSteps(float width, float height, ColorCodes colorSetID);//colorSetID = 0-3
		virtual ~CubesSteps();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;
		virtual void draw() override;

		virtual void drawBox();
		const float boxW = 15;
		int colorPresetID;
		
		
	private:
		
		const int maxBoxW = 10;
		const int maxBoxH = 10;

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
		ofColor boxColor1, boxColor2, boxColor3;
		vector<vector<ofColor>>boxColorsBase;
		void setBoxColorsBase();
		int setupCount = 0;
		bool presetColorsFlag;
		void setupBoxColors();
		void setBoxColors();
		ofEasyCam cam;
		ofLight pointLight;
		ofLight ambientLight;
		ofMaterial material;

	
		//const float lineWidth = 2;

		std::shared_ptr<ofxCinderTimeline::Timeline> timeline;
		ofxCinderTimeline::Anim<float> moveZ;
		void moveZSetup();
		void moveZFinish();
		void drawColorBox(float size);
		void drawModel(ofMesh& mesh);

		float spaceDiagonal(float size);
		float faceDiagonal(float size);
		float remap(float value, float min, float max);
	};

	class CubesStepsLines : public CubesSteps
	{
	public:
		CubesStepsLines(float width, float height);
		virtual ~CubesStepsLines();

		void drawBox();
	protected:
		//ofRectangle screen;
		const float lineWidth = 2;

		ofMesh boxMesh;
		ofMesh boxFillMesh;
		ofMesh setBoxMesh(float size);
		ofMesh setBoxFillMesh(float size);
	};


}
