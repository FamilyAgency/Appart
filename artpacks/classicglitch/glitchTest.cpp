#include "glitchTest.h"

using namespace frames;

GlitchTest::GlitchTest(float width, float height) : FrameGeometry(width, height)
{	
	spoutChannelName = "Glitch art pack";
	image.loadImage(ofToDataPath("classic1.jpg"));
}

GlitchTest::~GlitchTest()
{

}

void GlitchTest::setup()
{
	//fitImage(img, image);
	width = image.getWidth();
	height = image.getHeight();
	//// setup init values
	//ofImage img;
	//img.loadImage(ofToDataPath("classic1.jpg"));
	//aps.setup(img);

	////ofSetWindowShape(getWidth(), getHeight());

	//saved = false;
}

void GlitchTest::GUISetup()
{
	FrameGeometry::GUISetup();

	//code for gui setup below
	//...
}

void GlitchTest::draw()
{
	// draw in FBO
	fboArt.begin();
	ofClear(255, 255, 255, 0);

	// code here
	//aps.draw();
	//image.draw(0,0);

	//fitImage(img, image);
	//TODO better scaling of image to fit screen
	image.draw(0, 0, getWidth(), getHeight());


	fboArt.end();
	//FBO draws in parent
	FrameGeometry::draw();
}

void GlitchTest::update()
{

	int count = 0;
	while (steps == 0 || count++ < steps)
	{
		column = ofRandom(0, image.getWidth());
		brigthnessValue = ofRandom(minBrigthnessValue, 255);
		sortColumn(column);
	}

	if (ofGetFrameNum() % brigthnessTick == 0 && minBrigthnessValue > stopBrigthnessValue)
	{
		minBrigthnessValue--;
	}
	image.update();
}

//won't be in use, coz we can use fbos
//void GlitchTest::fitImage(ofFbo fbo, ofImage img)
//{
//	if (img.getWidth() > img.getHeight())
//	{
//		//hor
//		int imgScale = fbo.getHeight() / img.getHeight();
//		img.draw(
//			(float)(img.getWidth()*imgScale - fbo.getWidth()) / 2,
//			0,
//			img.getWidth()*imgScale,
//			img.getHeight()*imgScale
//		);
//		//cout << "hor" << endl;
//	}
//	//else if (img.getWidth() == img.getHeight())
//	//{
//	//	//square
//	//}
//	else
//	{
//		//ver
//		float imgScale = (float)fbo.getWidth() / (float)img.getWidth();
//		img.draw(
//			(float)(img.getHeight()*imgScale - fbo.getHeight()) / 2,
//			0,
//			img.getWidth()*imgScale,
//			img.getHeight()*imgScale
//		);
//		//cout << "ver imgH-" << img.getHeight()*imgScale << "  canvas H-" << fbo.getHeight() << "scale" << imgScale << endl;
//	}
//}

//float GlitchTest::getScale(ofFbo fbo, ofImage img)
//{
//	if (img.getWidth() > img.getHeight())
//	{
//		return (float)fbo.getHeight() / (float)img.getHeight();
//	}
//	else
//	{
//		return (float)fbo.getWidth() / (float)img.getWidth();
//	}
//	return 1.0f;
//}
//ofPoint GlitchTest::getMargin(ofFbo fbo, ofImage img)
//{
//	if (img.getWidth() > img.getHeight())
//	{
//		ofPoint tempPoint;
//		tempPoint.set(
//			(float)(img.getWidth()*getScale(fbo, img) - fbo.getWidth()) / 2,
//			0
//		);
//		
//	}
//	else
//	{
//		(float)(img.getHeight()*getScale(fbo, img) - fbo.getHeight()) / 2;
//	}
//	return ofPoint();
//}

void GlitchTest::sortColumn(int column)
{
	int x = column;
	int y = 0;
	int yend = 0;

	while (yend < image.getHeight() - 1)
	{
		y = getFirstBright(x, y);
		yend = getNextDark(x, y);


		if (y < 0) break;

		int sortLength = yend - y;
		if (sortLength < 0) break;
		vector<int> unsorted(sortLength);

		for (int i = 0; i<sortLength; i++)
		{
			unsorted[i] = image.getColor(x, y + i).getHex();
		}

		ofSort<int>(unsorted);

		for (int i = 0; i<sortLength; i++)
		{
			ofColor c;
			c.setHex(unsorted[i]);
			image.setColor(x, y + i, c);
		}

		y = yend + 1;
	}
}


int GlitchTest::getFirstBright(int _x, int _y)
{
	int x = _x;
	int y = _y;

	if (x >= width) return -1;
	if (y >= height) return -1;

	while (image.getColor(x, y).getBrightness() < brigthnessValue)
	{
		y++;
		if (x >= width) return -1;
		if (y >= height) return -1;
	}
	 return y;
}

int GlitchTest::getNextDark(int _x, int _y)
{
	int x = _x;
	int y = _y;

	y++;

	if (x >= width) return width - 1;
	if (y >= height) return height - 1;

	while (image.getColor(x, y).getBrightness() > brigthnessValue)
	{
		y++;
		if (x >= width) return width - 1;
		if (y >= height) return height - 1;
	}

	return y - 1;
}