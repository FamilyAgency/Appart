#include "Triangles.h"

using namespace frames;

Triangles::Triangles(float width, float height) : FrameGeometry(width, height)
{
	spoutChannelName = "Geometry Art";
	name = "Geometry Triangles";

	// 1st row
	addTriangle(-widthHalf * 3, pieceHeight, true);
	addTriangle(-widthHalf * 2, pieceHeight, false);
	addTriangle(-widthHalf, pieceHeight, true);
	addTriangle(0, pieceHeight, false);
	addTriangle(widthHalf, pieceHeight, true);
	addTriangle(widthHalf * 2, pieceHeight, false);
	addTriangle(widthHalf * 3, pieceHeight, true);

	// 2nd row
	addTriangle(-widthHalf * 2, 0, true);
	addTriangle(-widthHalf, 0, false);
	addTriangle(widthHalf, 0, false);
	addTriangle(widthHalf * 2, 0, true);

	// 3rd row
	addTriangle(-widthHalf, -pieceHeight, true);
	addTriangle(0, -pieceHeight, false);
	addTriangle(widthHalf, -pieceHeight, true);

	// 4th row
	addTriangle(0, -pieceHeight * 2, true);
}

Triangles::~Triangles()
{

}

void Triangles::setup()
{

}

void Triangles::GUISetup()
{
#ifndef release
	FrameGeometry::GUISetup();

	gui.add(colorGui1.setup("Triangle color", ofColor(0, 0, 0), ofColor(0, 0), ofColor(255, 255)));
	gui.add(colorGui2.setup("Background color", ofColor(255, 255, 255), ofColor(0, 0), ofColor(255, 255)));
#else
	colorGui1.setup("Triangle color", ofColor(0, 0, 0), ofColor(0, 0), ofColor(255, 255));
	colorGui2.setup("Background color", ofColor(255, 255, 255), ofColor(0, 0), ofColor(255, 255));
#
#endif
}

void Triangles::draw()
{
	// draw in FBO
	fboArt.begin();

	ofBackground(colorGui2);
	ofEnableAlphaBlending();

	ofTranslate(getWidth() * 0.5f, getHeight() * 0.5f);
	ofRotate((ofGetFrameNum()*0.45));

	ofPushMatrix();

	ofScale(allScale, allScale);
	allScale -= 0.01;

	if (allScale <= 1)
	{
		allScale = 3;
	}
	
	timeScale = ofClamp(ofMap(allScale, 3, 1, 0, 1), 0.f, 1.f);

	for (int i = 0; i < triangles.size(); i++)
	{
		Triangle piece = triangles[i];

		ofPushMatrix();
			ofTranslate(piece.location.x, piece.location.y);
			// 1
			ofPushMatrix();
				float pieceScale = ofClamp(-0.05f*i + timeScale * 2.f, 0.f, 1.f);
				ofScale(pieceScale, pieceScale);				
				ofSetColor(colorGui1);// pieceScale * 255);
				piece.draw();
			ofPopMatrix();
			// 2
			ofPushMatrix();
				float innerPieceScale = ofClamp(1.0f - (-0.2f * i + timeScale * 4), 0.f, 1.f);
				ofScale(innerPieceScale, innerPieceScale);
				ofSetColor(colorGui2);
				piece.draw();
			ofPopMatrix();
		ofPopMatrix();
	}
	ofPopMatrix();

	//// 
	ofPushMatrix();
		innerScale = ofMap(timeScale, 0, 1, 1, 0);
		ofScale(innerScale, innerScale);
		if (innerScale < 0.9)
		{
			ofTranslate(0, (0.9 - innerScale) * 25);
		}	
		ofFill();
		ofSetColor(colorGui1);
		ofTriangle(-widthHalf * 4, heightHalf * 3, 0, -heightHalf * 5, widthHalf * 4, heightHalf * 3);
	ofPopMatrix();

	ofSetColor(colorGui2);
	ofTriangle(-widthHalf, heightHalf, 0, -heightHalf, widthHalf, heightHalf);
	fboArt.end();

	//FBO draws in parent
	FrameGeometry::draw();
}

void Triangles::update()
{
	// update algorithm here
}

void Triangles::addTriangle(float x, float y, bool up)
{	
	triangles.push_back(Triangle(x, y, up, this));
}

Triangle::Triangle(float x, float y, bool up, Triangles* parent):
	location(ofVec2f(x, y)),
	up(up),
	parent(parent)
{
	
}

void Triangle::draw()
{
	int minus = up ? 1 : -1;
	ofTriangle(-parent->widthHalf, minus*parent->heightHalf, 0, -minus*parent->heightHalf, parent->widthHalf, minus*parent->heightHalf);
}