#pragma once
#include "FrameGeometry.h"

namespace frames
{
	// short name for smart pointer
	typedef shared_ptr<class CubesRing> CubesRingPtr;

	class CubesRing : public FrameGeometry
	{
	public:
		CubesRing(float width, float height);
		virtual ~CubesRing();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;
		virtual void draw() override;
		ofxFloatSlider rotation;
		//ofxIntSlider numCubes;
	private:
		const float numCubes = 500;//num of individual slices
		const int totalRotate = 0;// only 0 or 180 or 360 //gif has 0
		const float innerRotateSpeed = 1.5;
		const float offSetRotateMax = 60.;

		ofxToggle wireframe;

		float innerRotate;

		ofNode corners[4];
		ofNode center;
		float planeW = 10;
		float planeH = 50;
		ofMesh mesh;

		ofMesh normalsMesh;
		

		ofColor background;
		ofColor mainColor;
		ofColor lightDiff;
		ofColor lightSpec;	

		ofVec3f nullVec;
		ofVec3f upVec;

		float width;
		float height;
		float offsetRotate;
		//float spinX, spinY;


		ofPlanePrimitive plane;


		ofLight pointLight;
		ofMaterial material;

		ofColor orbitColorDiff;
		ofColor orbitColorSpec;
		ofLight orbitLight;

		ofEasyCam cam;
		//ofCamera cam;


		ofPoint pointFromAngle(const float& angle, const float& d);
		void calcNormals(ofMesh & mesh);

		void drawNormals(ofMesh mesh);
		ofVec3f calculateCenter(ofMeshFace *face);
	};
}
