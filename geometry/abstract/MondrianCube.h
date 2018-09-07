#pragma once
#include "FrameGeometry.h"
#include "MondrianAnimated.h"
#include "ofxCubeMap.h"

namespace frames
{
	// short name for smart pointer
	typedef shared_ptr<class MondrianCube> MondrianCubePtr;

	class MondrianCube : public FrameGeometry
	{
	public:
		MondrianCube(float width, float height);
		virtual ~MondrianCube();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;
		virtual void draw() override;

	protected:

	private:
		ofBoxPrimitive box;
		ofLight pointLight;
		ofLight pointLight2;
		ofLight pointLight3;
		ofLight directionalLight;
		ofMaterial material;

		ofxColorSlider bgColor;
		ofxIntSlider size;

		ofVboMesh boxSides[ofBoxPrimitive::SIDES_TOTAL];

		vector<frames::ArtDrawablePtr> textureMondrian;
		vector<ofFbo> textureMondrianFBO;

		ofxCubeMap cubeMap;
	};
}
