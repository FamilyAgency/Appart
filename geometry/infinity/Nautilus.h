#pragma once
#include "FrameGeometry.h"
#include "ofxCinderTimeline.h"

namespace frames
{
	// short name for smart pointer
	typedef shared_ptr<class Nautilus> NautilusPtr;

	class Nautilus : public FrameGeometry
	{
	public:
		Nautilus(
			float widt,
			float height,
			ofColor mainColor,
			ofColor fillColor,
			ofColor bgColor,
			float camDist,
			float longt,
			float latit,
			float maxRotZ,
			float maxRotX
		);

		virtual ~Nautilus();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;
		virtual void draw() override;

	private:
		const int sphereResolutin = 30;
		const double lineWidth = 5.0f;
		const float radius = 200;
		const int spheresNum = 10;
		const float animTime = 2.0f;
		float maxRotZ = 25;
		float maxRotX = 45;
		ofColor bgColor;
		ofColor mainColor;
		ofColor fillColor;

		ofFloatColor mainColorGL;
		ofFloatColor fillColorGL;

		ofEasyCam cam;

		ofSpherePrimitive sphere;
		vector<ofMeshFace> halfSphereMesh;
		ofMesh sphereMesh;
		ofMesh sphereRingMesh;

		ofxFloatSlider camDist;
		std::shared_ptr<ofxCinderTimeline::Timeline> timeline;
		ofxCinderTimeline::Anim<float> anim;

		void drawModel(ofMesh& mesh);		
		void animSetup();
		void animFinish();	
	};
}
