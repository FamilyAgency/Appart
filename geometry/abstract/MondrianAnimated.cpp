#include "MondrianAnimated.h"

using namespace frames;

float MondrianAnimated::strWeight = 8;

MondrianAnimated::MondrianAnimated(float width, float height) 
	: FrameGeometry(width, height),
	color1(ofColor(255, 0, 0)),
	color2(ofColor(0, 0, 255)),
	color3(ofColor(255, 255, 0))
{
	spoutChannelName = "Geometry Art";
	name = "Mondrian Animated Art";

	movingIndexes.resize(7);
}

MondrianAnimated::~MondrianAnimated()
{

}

void MondrianAnimated::setup()
{
	inc = 0.005;
	counter = 1;
	initRectangles();
}

void MondrianAnimated::GUISetup()
{
#ifndef release
	FrameGeometry::GUISetup();

	gui.add(colorGui1.setup("Color1", ofColor(255, 0, 0), ofColor(0, 0), ofColor(255, 255)));
	gui.add(colorGui2.setup("Color2", ofColor(0, 0, 255), ofColor(0, 0), ofColor(255, 255)));
	gui.add(colorGui3.setup("Color3", ofColor(255, 255, 0), ofColor(0, 0), ofColor(255, 255)));

	//code for gui setup below
	//...
#endif
}

void MondrianAnimated::initRectangles()
{
	rectangles.clear();
	MondrianRect mondrianrect = MondrianRect(strWeight * 0.5, strWeight * 0.5, getWidth() - strWeight, getHeight() - strWeight);
	rectangles.push_back(mondrianrect);

	for (int i = 0; i < splitCount; i++)
	{
		rectangles = splitRectangles(rectangles);
	}

	std::random_shuffle(rectangles.begin(), rectangles.end());
	
	set<int> exclude;
	movingIndexes[2] = generateUniqueRandomInt(rectangles.size(), exclude);
	exclude.insert(movingIndexes[2]);

	movingIndexes[4] = generateUniqueRandomInt(rectangles.size(), exclude);
	exclude.insert(movingIndexes[4]);

	movingIndexes[6] = generateUniqueRandomInt(rectangles.size(), exclude);

	initMovers();
	factor = 1;
}

void MondrianAnimated::initMovers()
{
	factor = 0;
	move = true;

	set<int> exclude;
	movingIndexes[1] = movingIndexes[2];
	movingIndexes[3] = movingIndexes[4];
	movingIndexes[5] = movingIndexes[6];	

	exclude.insert(movingIndexes[1]);
	exclude.insert(movingIndexes[3]);
	exclude.insert(movingIndexes[5]);


	movingIndexes[2] = generateUniqueRandomInt(rectangles.size(), exclude);
	exclude.insert(movingIndexes[2]);

	movingIndexes[4] = generateUniqueRandomInt(rectangles.size(), exclude);
	exclude.insert(movingIndexes[4]);

	movingIndexes[6] = generateUniqueRandomInt(rectangles.size(), exclude);	
}

int MondrianAnimated::generateUniqueRandomInt(int size, const set<int>& _set)
{
	int rand = (int)ofRandom(size);

	while (_set.find(rand) != _set.end())
	{
		rand = (int)ofRandom(size);
	}

	return rand;
}

vector<MondrianRect> MondrianAnimated::splitRectangles(const vector<MondrianRect>& rectangles)
{
	vector<MondrianRect> rs;
	MondrianRect r, r1, r2;
	float factor;

	for (int i = 0; i < rectangles.size(); i++)
	{
		r = rectangles[i];
		factor = ofRandom(0.2, 0.81);

		// split at the longer side
		if (r.rw > r.rh)
		{
			r1.set(r.rx, r.ry, r.rw * factor, r.rh);
			r2.set(r.rx + r.rw * factor, r.ry, r.rw - (r.rw * factor), r.rh);
		}
		else
		{
			r1.set(r.rx, r.ry, r.rw, r.rh * factor);
			r2.set(r.rx, r.ry + r.rh * factor, r.rw, r.rh - (r.rh * factor));
		}

		rs.push_back(r1);
		rs.push_back(r2);
	}

	return rs;
}

void MondrianAnimated::draw()
{
	// draw in FBO
	fboArt.begin();
	ofBackground(255);
	ofClear(255, 255, 255, 0);
	ofEnableAlphaBlending();

		if (counter % 100 == 0 && move == false)
		{
			initMovers();
			counter = 1;
		}
	
		ofSetLineWidth(strWeight);		
	
		MondrianRect rd;
		for (int i = 0; i < rectangles.size(); i++)
		{
			rectangles[i].draw();
		}

#ifndef release
		color1 = colorGui1;
		color2 = colorGui2;
		color3 = colorGui3;
#endif
	
		ofSetColor(color1);
		rd = rectangles[movingIndexes[1]];
		rd.drawW(1 - factor);
		rd = rectangles[movingIndexes[2]];
		rd.drawW(factor);
	
		ofSetColor(color2);
		rd = rectangles[movingIndexes[3]];
		rd.drawH(1 - factor);
		rd = rectangles[movingIndexes[4]];
		rd.drawH(factor);
	
		ofSetColor(color3);
		rd = rectangles[movingIndexes[5]];
		rd.drawW(1 - factor);
		rd = rectangles[movingIndexes[6]];
		rd.drawW(factor);
	
		if (move)
		{
			factor += inc;
			if (factor > 1)
			{
				move = false;
			}
		}
		counter++;

	fboArt.end();

	//FBO draws in parent
	FrameGeometry::draw();
}

void MondrianAnimated::update()
{
	// update algorithm here
}

// Help Class
MondrianRect::MondrianRect()
{
	strWeight = MondrianAnimated::strWeight;
}

MondrianRect::MondrianRect(float x, float y, float width, float height)
{
	strWeight = MondrianAnimated::strWeight;
	set(x, y, width, height);
}

void MondrianRect::set(float x, float y, float width, float height)
{
	rx = x;
	ry = y;
	rw = width;
	rh = height;
}

void MondrianRect::draw()
{
	ofSetColor(255);
	ofDrawRectangle(rx, ry, rw, rh);
	ofSetColor(0);

	ofLine(rx, ry - strWeight*0.5, rx, ry + rh + strWeight*0.5);
	ofLine(rx, ry, rx + rw, ry);
	ofLine(rx, ry + rh, rx + rw, ry + rh);
	ofLine(rx + rw, ry - strWeight*0.5, rx + rw, ry + rh + strWeight*0.5);
}

void MondrianRect::drawW(float factor)
{
	float actWidth;
	actWidth = ofLerp(0, rw - strWeight, factor);
	ofDrawRectangle(rx + strWeight / 2, ry + strWeight / 2, actWidth, rh - strWeight);
}

void MondrianRect::drawH(float factor)
{
	float actHeight;
	actHeight = ofLerp(0, rh - strWeight, factor);
	ofDrawRectangle(rx + strWeight / 2, ry + strWeight / 2, rw - strWeight, actHeight);
}
