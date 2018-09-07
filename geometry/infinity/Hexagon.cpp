#include "Hexagon.h"

using namespace frames;

Hexagon::Hexagon(float width, float height) : FrameGeometry(width, height)
{
	spoutChannelName = "Geometry Art";
	name = "Geometry Hexagon";
}

Hexagon::~Hexagon()
{

}

void Hexagon::setup()
{
	numAround = sides * perSide;

	setTrianglePoints(length);

	setDelay();
	generalScale = 1;
	resetDelay();
}

void Hexagon::GUISetup()
{

#ifndef release
	FrameGeometry::GUISetup();

	gui.add(delaySpeed.setup("delay speed", 1, 0.05, 5));
	gui.add(triangleSpeed.setup("triangle speed", 0.01, 0.005, 0.05));
	gui.add(scaleSpeed.setup("scale speed", 0.001, 0.0005, 0.005));
	gui.add(scaleTrigger.setup("scale trigger", 18, 1, 18));
	gui.add(hexColor.setup("Hex color", ofColor(0, 0, 0), ofColor(0, 0, 0), ofColor(255, 255, 255)));
	gui.add(bgColor.setup("BG color", ofColor(255, 255, 255), ofColor(0, 0, 0), ofColor(255, 255, 255)));
#else
	delaySpeed.setup("delay speed", 1, 0.05, 5);
	triangleSpeed.setup("triangle speed", 0.04, 0.005, 0.05);
	scaleSpeed.setup("scale speed", 0.001, 0.0005, 0.005);
	scaleTrigger.setup("scale trigger", 18, 1, 18);
	hexColor.setup("Hex color", ofColor(0, 0, 0), ofColor(0, 0, 0), ofColor(255, 255, 255));
	bgColor.setup("BG color", ofColor(255, 255, 255), ofColor(0, 0, 0), ofColor(255, 255, 255));
#endif

}

void Hexagon::draw()
{
	// draw in  FBO
	fboArt.begin();
	ofClear(bgColor);

	ofTranslate(ofGetViewportWidth()*0.5f, ofGetViewportHeight()*0.5f);
	ofSetColor(hexColor);

	if (generalScale <= 0.5)
	{
		generalScale = 1;
		resetDelay();
	}
	else
	{
		ofScale(generalScale, generalScale);
	}

	for (int x = 0; x < sides; x++)
	{
		ofRotateZ(60);

		ofPushMatrix();
		ofTranslate(0, -tempPoints.h*1.5);
		for (int i = 0; i < perSide; i++)
		{
			ofPushMatrix();
			if (i == 0)
			{
				ofTranslate(-length*0.5, 0);
			}
			else if (i == 1)
			{
				ofRotateZ(180);
			}
			else
			{
				ofTranslate(length*0.5, 0);
			}

			int current = i + perSide * x;
			if (triangleParams[current].delay > 0)
			{
				triangleParams[current].delay = triangleParams[current].delay - delaySpeed;
				ofScale(0, 0);
			}
			else
			{
				if (triangleParams[current].scale < 1)
				{
					triangleParams[current].scale = triangleParams[current].scale + triangleSpeed + 0.001;
				}

				ofScale(triangleParams[current].scale, triangleParams[current].scale);
			}

			ofDrawTriangle(tempPoints.x1, tempPoints.y1,
				tempPoints.x2, tempPoints.y2,
				tempPoints.x3, tempPoints.y3);
			ofPopMatrix();

		}

		if ((generalScale > 0.5) && (triangleParams[scaleTrigger - 1].scale >= 1))
		{
			generalScale -= scaleSpeed;
		}
		ofPopMatrix();
		drawHexagon(length);
	}

	fboArt.end();

	FrameGeometry::draw();
}

void Hexagon::update()
{

}

void Hexagon::calculateHexagon(float len, float x, float y)
{
	float x1 = x;
	float y1 = y - len;

}
void Hexagon::setTrianglePoints(float length)
{
	tempPoints.h = sqrt(pow(length, 2) - pow(length * 0.5, 2));
	tempPoints.x1 = -length*0.5;
	tempPoints.y1 = 0 - tempPoints.h * 0.5;
	tempPoints.x2 = 0;
	tempPoints.y2 = length*0.5*sqrt(3) - tempPoints.h * 0.5;
	tempPoints.x3 = length * 0.5;
	tempPoints.y3 = 0 - tempPoints.h*0.5;
}

void Hexagon::setDelay()
{
	for (int i = 0; i < numAround; i++)
	{
		TriangleParam TempParam;
		TempParam.scale = 0.;
		TempParam.delay = i * delayFactor;
		TempParam.defaultDelay = TempParam.delay;

		triangleParams.push_back(TempParam);
	}
}

void Hexagon::resetDelay()
{
	for (int i = 0; i < triangleParams.size(); i++)
	{
		triangleParams[i].delay = triangleParams[i].defaultDelay;
		triangleParams[i].scale = 0;
	}
}

void Hexagon::drawHexagon(int size)
{
	ofPath hexagon;
	for (float i = 0; i < sides; i++)
	{
		int hexx = size*cos((i / sides) * TWO_PI);
		int hexy = size*sin((i / sides) * TWO_PI);
		hexagon.lineTo(hexx, hexy);
	}

	hexagon.close();
	hexagon.setFillColor(hexColor);
	ofFill();
	hexagon.draw();
}