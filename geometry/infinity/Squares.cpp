#include "Squares.h"

using namespace frames;

Squares::Squares(float width, float height) : FrameGeometry(width, height)
{	
	spoutChannelName = "Geometry Art";
	name = "Geometry Squares";
	rotateAngle = 0;
}

Squares::~Squares()
{
	
}

void Squares::setup()
{
	generalScale = 1;
	squaresSetup(size);
}

void Squares::squaresSetup(float size)
{
	struct Transformation tempTrans;
	squares.clear();

	// top left
	tempTrans.x = (1 * size) / 6;
	tempTrans.y = (1 * size) / 6;
	tempTrans.angle = -(2 * PI) / 3;
	tempTrans.dist = 1.0;
	tempTrans.delay = 0;
	tempTrans.scale = (2. * size / 6);
	squares.push_back(tempTrans);

	//top center
	tempTrans.x = (3 * size) / 6;
	tempTrans.y = (1 * size) / 6;
	tempTrans.angle = -(1 * PI) / 2;
	tempTrans.dist = 1.0;
	tempTrans.delay = 100;
	tempTrans.scale = (2. * size / 6);
	squares.push_back(tempTrans);

	//top right
	tempTrans.x = (5 * size) / 6;
	tempTrans.y = (1 * size) / 6;
	tempTrans.angle = -(1 * PI) / 4;
	tempTrans.dist = 1.0;
	tempTrans.delay = 200;
	tempTrans.scale = (2. * size / 6);
	squares.push_back(tempTrans);

	//right middle
	tempTrans.x = (5 * size) / 6;
	tempTrans.y = (3 * size) / 6;
	tempTrans.angle = (0 * PI);
	tempTrans.dist = 1.0;
	tempTrans.delay = 300;
	tempTrans.scale = (2. * size / 6);
	squares.push_back(tempTrans);

	//right bottom
	tempTrans.x = (5 * size) / 6;
	tempTrans.y = (5 * size) / 6;
	tempTrans.angle = -((-1) * PI / 4);
	tempTrans.dist = 1.0;
	tempTrans.delay = 400;
	tempTrans.scale = (2. * size / 6);
	squares.push_back(tempTrans);

	//bottom middle
	tempTrans.x = (3 * size) / 6;
	tempTrans.y = (5 * size) / 6;
	tempTrans.angle = -((-1) * PI / 2);
	tempTrans.dist = 1.0;
	tempTrans.delay = 500;
	tempTrans.scale = (2. * size / 6);
	squares.push_back(tempTrans);

	//bottom left
	tempTrans.x = (1 * size) / 6;
	tempTrans.y = (5 * size) / 6;
	tempTrans.angle = -((-2) * PI / 3);
	tempTrans.dist = 0.8;
	tempTrans.delay = 600;
	tempTrans.scale = (2. * size / 6);
	squares.push_back(tempTrans);

	//left middle
	tempTrans.x = (1 * size) / 6;
	tempTrans.y = (3 * size) / 6;
	tempTrans.angle = -(PI);
	tempTrans.dist = 1.0;
	tempTrans.delay = 700;
	tempTrans.scale = (2. * size / 6);
	squares.push_back(tempTrans);

	//main square
	tempTrans.x = (3 * size) / 6;
	tempTrans.y = (3 * size) / 6;
	tempTrans.angle = 0;
	tempTrans.dist = 1;
	tempTrans.scale = (2. * size / 6);
	squares.push_back(tempTrans);

	last = &squares.back();
	defaultSquares = squares;
}

void Squares::GUISetup()
{
#ifndef release
	FrameGeometry::GUISetup();

	gui.add(speed.setup("Fly away speed", 5., 1., 10.0));
	gui.add(delaySpeed.setup("Delay speed", 7.8, 2, 30));
	gui.add(size.setup("Square size", 250, 100., 1000.0));

	size.addListener(this, &Squares::sizeChanged);

	gui.add(rotateSpeed.setup("Rotate speed", 0.5, 0.01, 5.0));
	gui.add(generalScaleSpeed.setup("General scale speed", 0.015, 0.001, 0.03));
	gui.add(generalScaleDefault.setup("General scale deafult", 1.0, 0.1, 3.0));
	gui.add(colorGui1.setup("Square color", ofColor(255, 255, 255), ofColor(0, 0), ofColor(255, 255)));
	gui.add(bgColor.setup("BG color", ofColor(0, 0, 0), ofColor(0, 0), ofColor(255, 255)));
#else
	speed.setup("Fly away speed", 5., 1., 10.0);
	delaySpeed.setup("Delay speed", 7.8, 2, 30);
	size.setup("Square size", 250, 100., 1000.0);

	rotateSpeed.setup("Rotate speed", 0.5, 0.01, 5.0);
	generalScaleSpeed.setup("General scale speed", 0.015, 0.001, 0.03);
	generalScaleDefault.setup("General scale deafult", 1.0, 0.1, 3.0);
	colorGui1.setup("Square color", ofColor(255, 255, 255), ofColor(0, 0), ofColor(255, 255));
	bgColor.setup("BG color", ofColor(0, 0, 0), ofColor(0, 0), ofColor(255, 255));
#endif
}

void Squares::sizeChanged(float& value)
{
	squaresSetup(value);
}

void Squares::update()
{
	for (int i = 0; i < squares.size() - 1; i++)
	{
		if (squares[i].delay >= 0)
		{
			squares[i].delay= squares[i].delay- delaySpeed;
		}
		else
		{
			squares[i].x = squares[i].x + speed*squares[i].dist * cos(squares[i].angle);
			squares[i].y = squares[i].y + speed*squares[i].dist * sin(squares[i].angle);
		}		
	}

	generalScale = generalScale + generalScaleSpeed;

	if (generalScale > 3)
	{
		generalScale = generalScaleDefault;
		Squares::setup();
		squares = defaultSquares;	
	}
}

void Squares::draw()
{
	fboArt.begin();
	ofClear(255, 255, 255, 0);
	ofPushMatrix();
		ofBackground(bgColor);
		ofTranslate(getWidth() * 0.5, getHeight() * 0.5);
		rotate(rotateSpeed);

		ofPushMatrix();
			ofScale(generalScale, generalScale, 1);
			ofTranslate(-size * 0.5, -size * 0.5);
			ofSetRectMode(OF_RECTMODE_CENTER);

			for (int i = 0; i < squares.size(); i++)
			{
				ofSetColor(colorGui1);
				ofDrawRectangle(squares[i].x, squares[i].y, squares[i].scale, squares[i].scale);
				ofSetColor(255);
			}
			ofSetRectMode(OF_RECTMODE_CORNER);

		ofPopMatrix();
	ofPopMatrix();
	fboArt.end();	

	FrameGeometry::draw();
}

void Squares::rotate(float rotateSpeed)
{
	rotateAngle += rotateSpeed;
	ofRotate(rotateAngle);
}

void Squares::status()
{
	cout << "\n\n        ********** STATUS **********         ";
	for (int i = 0; i < squares.size(); i++)
	{
		cout << "\n           *Square_" << i
			 << "     x=" << squares[i].x
			 << "     y=" << squares[i].y
			 << " scale=" << squares[i].scale
			 << "  dist=" << squares[i].dist
			 << " angle=" << squares[i].angle;
	}
	cout << "\n\n        ****************************         ";
}