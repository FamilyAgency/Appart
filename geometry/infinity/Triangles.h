#pragma once
#include "FrameGeometry.h"

namespace frames
{
	// short name for smart pointer
	typedef shared_ptr<class Triangles> TrianglesPtr;	

	class Triangles : public FrameGeometry
	{
	public:
		Triangles(float width, float height);
		virtual ~Triangles();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;
		virtual void draw() override;		

	protected:

	private:
		friend class Triangle;
		float pieceWidth = 40;
		float pieceHeight = 40;
		float widthHalf = pieceWidth * 0.5;
		float heightHalf = pieceHeight * 0.5;

		float allScale = 3;
		float innerScale = 0;
		float timeScale = 0;
		vector<class Triangle> triangles;		

		void addTriangle(float x, float y, bool up);	

		ofxColorSlider colorGui1, colorGui2;
	};

	class Triangle
	{
	public:
		ofVec2f location;
		bool up;
		Triangles* parent;

		Triangle(float x, float y, bool up, Triangles* parent);
		void draw();
	};
}
