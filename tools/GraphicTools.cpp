#include "GraphicTools.h"
using namespace frames;

void AsdfPixelSort::setup(ofImage& _img, int _mode, int _steps, int _blackValue, int _brightnessValue, int _whiteValue)
{
	img = _img;

	width = img.getWidth();
	height = img.getHeight();
	mode = _mode;
	steps = _steps;
	blackValue = _blackValue;
	brigthnessValue = _brightnessValue;
	whiteValue = _whiteValue;

	row = 0;
	column = 0;
	sortDone = false;
	minBrigthnessValue = 254;
}

//--------------------------------------------------------------
void AsdfPixelSort::update()
{
	int count = 0;


	//if (count < 100)
	//{
	//	column = ofRandom(0, img.getWidth());
	//	sortColumn();
	//	count++;
	//}
	//else
	//{
	//	brigthnessValue--;
	//}
	while (steps == 0 || count++ < steps)
	{
		column = ofRandom(0, img.getWidth());
		brigthnessValue = ofRandom(minBrigthnessValue, 255);
		sortColumn();
	}

	if (ofGetFrameNum() % brigthnessTick == 0)
	{
		minBrigthnessValue--;
		cout << "here" << endl;
	}


	//while (steps == 0 || count++ < steps)
	//{
	//	if (column < img.getWidth() - 1)
	//	{
	//		sortColumn();
	//		column++;
	//		//cout << "sorting " << endl;

	//	}
	//	/*else if (row < img.getHeight() - 1)
	//	{
	//		sortRow();
	//		row++;
	//	}*/
	//	else
	//	{
	//		
	//		column = 0;
	//		row = 0;
	//		cout << "we are here " << endl;
	//		brigthnessValue--;
	//		//sortDone = true;
	//		//break;
	//	}
	//}
	img.update();
}

//--------------------------------------------------------------
void AsdfPixelSort::draw()
{
	img.draw(0, 0);
}

void AsdfPixelSort::sortRow()
{

	int x = 0;
	int y = row;
	int xend = 0;

	while (xend < img.getWidth() - 1)
	{

		switch (mode)
		{
		case 0:
			x = getFirstNotBlack(X, x, y);
			xend = getNextBlack(X, x, y);
			break;
		case 1:
			x = getFirstBright(X, x, y);
			xend = getNextDark(X, x, y);
			break;
		case 2:
			x = getFirstNotWhite(X, x, y);
			xend = getNextWhite(X, x, y);
			break;
		default:
			break;
		}

		if (x < 0) break;

		int sortLength = xend - x;
		if (sortLength < 0) break;
		vector<int> unsorted(sortLength);
		vector<int> sorted(sortLength);

		for (int i = 0; i < sortLength; i++)
		{
			unsorted[i] = img.getColor(x + i, y).getHex();
		}

		ofSort<int>(unsorted);

		for (int i = 0; i < sortLength; i++)
		{
			ofColor c;
			c.setHex(unsorted[i]);
			img.setColor(x + i, y, c);
		}

		x = xend + 1;
	}
}

void AsdfPixelSort::sortColumn()
{
	int x = column;
	int y = 0;
	int yend = 0;

	while (yend < img.getHeight() - 1)
	{
		switch (mode)
		{
		case 0:
			y = getFirstNotBlack(Y, x, y);
			yend = getNextBlack(Y, x, y);
			break;
		case 1:
			y = getFirstBright(Y, x, y);
			yend = getNextDark(Y, x, y);
			break;
		case 2:
			y = getFirstNotWhite(Y, x, y);
			yend = getNextWhite(Y, x, y);
			break;
		}

		if (y < 0) break;

		int sortLength = yend - y;
		if (sortLength < 0) break;

		vector<int> unsorted(sortLength);

		for (int i = 0; i < sortLength; i++)
		{
			unsorted[i] = img.getColor(x, y + i).getHex();
		}

		ofSort<int>(unsorted);

		for (int i = 0; i < sortLength; i++)
		{
			ofColor c;
			c.setHex(unsorted[i]);
			img.setColor(x, y + i, c);
		}

		y = yend + 1;
	}
}

//BLACK
int AsdfPixelSort::getFirstNotBlack(Direction d, int _x, int _y)
{
	int x = _x;
	int y = _y;

	if (x >= width) return -1;
	if (y >= height) return -1;

	while (getPcolor(img.getColor(x, y)) < blackValue)
	{
		if (d == X) x++;
		else y++;
		if (x >= width) return -1;
		if (y >= height) return -1;
	}

	if (d == X) return x;
	else return y;
}

int AsdfPixelSort::getNextBlack(Direction d, int _x, int _y)
{
	int x = _x;
	int y = _y;

	if (d == X) x++;
	else y++;

	if (x >= width) return width - 1;
	if (y >= height) return height - 1;

	while (getPcolor(img.getColor(x, y)) > blackValue)
	{
		if (d == X) x++;
		else y++;
		if (x >= width) return width - 1;
		if (y >= height) return height - 1;
	}

	if (d == X) return x - 1;
	else return y - 1;
}

int AsdfPixelSort::getFirstBright(Direction d, int _x, int _y)
{
	int x = _x;
	int y = _y;

	if (x >= width) return -1;
	if (y >= height) return -1;

	while (img.getColor(x, y).getBrightness() < brigthnessValue)
	{
		if (d == X) x++;
		else y++;
		if (x >= width) return -1;
		if (y >= height) return -1;
	}

	if (d == X) return x;
	else return y;
}

int AsdfPixelSort::getNextDark(Direction d, int _x, int _y)
{
	int x = _x;
	int y = _y;

	if (d == X) x++;
	else y++;

	if (x >= width) return width - 1;
	if (y >= height) return height - 1;

	while (img.getColor(x, y).getBrightness() > brigthnessValue)
	{
		if (d == X) x++;
		else y++;
		if (x >= width) return width - 1;
		if (y >= height) return height - 1;
	}

	if (d == X) return x - 1;
	else return y - 1;
}

//WHITE
int AsdfPixelSort::getFirstNotWhite(Direction d, int _x, int _y)
{
	int x = _x;
	int y = _y;

	if (x >= width) return -1;
	if (y >= height) return -1;

	while (getPcolor(img.getColor(x, y)) > whiteValue)
	{
		if (d == X) x++;
		else y++;
		if (x >= width) return -1;
		if (y >= height) return -1;
	}

	if (d == X) return x;
	else return y;
}

int AsdfPixelSort::getNextWhite(Direction d, int _x, int _y)
{
	int x = _x;
	int y = _y;

	if (d == X) x++;
	else y++;

	if (x >= width) return width - 1;
	if (y >= height) return height - 1;

	while (getPcolor(img.getColor(x, y)) < whiteValue)
	{
		if (d == X) x++;
		else y++;
		if (x >= width) return width - 1;
		if (y >= height) return height - 1;
	}

	if (d == X) return x - 1;
	else return y - 1;
}