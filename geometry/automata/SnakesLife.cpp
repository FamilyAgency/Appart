#include "SnakesLife.h"

using namespace frames;

SnakesLife::SnakesLife(float width, float height) : FrameGeometry(width, height)
{
	spoutChannelName = "SnakesLife";
	name = "SnakesLife";

	//cout << "w" << width << " h" << height << endl;
	
	squareActiveColor.set(255, 0, 0, 255);
	squarePassiveColor.set(255, 0, 0, 0);
	snakeColor.set(0, 0, 0);
	lineColor.set(0, 0, 0);

	squareColorsVec.push_back(ofColor(175, 194, 213));
	squareColorsVec.push_back(ofColor(204, 221, 211));
	squareColorsVec.push_back(ofColor(223, 239, 202));
	squareColorsVec.push_back(ofColor(255, 249, 165));
	squareColorsVec.push_back(ofColor(180, 139, 125));

	
}

SnakesLife::~SnakesLife()
{

}

void SnakesLife::setup()
{
	reset();

	gridSize.set((int)width / gridSpacing, (int)height / gridSpacing);
	squareCount = 0;

	timeline = ofxCinderTimeline::Timeline::create();
	timeline->stepTo(ofGetElapsedTimef());

	// setup init values
	createGrid(gridSize.x, gridSize.y);
	createSquareGrid(gridSize.x, gridSize.y);
	createSnake();

	//animSetup();
	
}

void SnakesLife::GUISetup()
{
#ifndef release
	FrameGeometry::GUISetup();

	gui.add(animation.setup("animatio", 0., 0., 1.));
	//code for gui setup below
	//...
#endif
}

void SnakesLife::draw()
{	
	// draw in FBO
	fboArt.begin();
	ofClear(255, 255, 255, 255);

	// code here
	ofTranslate(10, 10);

	drawSquares();

	ofSetColor(0, 0, 0);


	//ofSetLineWidth(3);
	//for (int x = 0; x < gridSize.x; x++)
	//{
	//	for (int y = 0; y < gridSize.y; y++)
	//	{
	//		ofDrawCircle(grid[x][y]->point.x*gridSpacing, grid[x][y]->point.y*gridSpacing,1);
	//		//ofSetLineWidth(1);
	//		if (grid[x][y]->isTop)
	//		{
	//			ofDrawLine(grid[x][y]->point*gridSpacing, grid[x][y]->point*gridSpacing +ofPoint(0, -0.2*gridSpacing));
	//		}
	//		if (grid[x][y]->isRight)
	//		{
	//			ofDrawLine(grid[x][y]->point*gridSpacing, grid[x][y]->point*gridSpacing + ofPoint(0.2*gridSpacing, 0));
	//		}
	//		if (grid[x][y]->isBot)
	//		{
	//			ofDrawLine(grid[x][y]->point*gridSpacing, grid[x][y]->point*gridSpacing + ofPoint(0, 0.2*gridSpacing));
	//		}
	//		if (grid[x][y]->isLeft)
	//		{
	//			ofDrawLine(grid[x][y]->point*gridSpacing, grid[x][y]->point*gridSpacing + ofPoint(-0.2*gridSpacing, 0));
	//		}
	//	}
	//}
	//ofSetLineWidth(1);



	for (int x = 0; x < gridSize.x; x++)
	{
		for (int y = 0; y < gridSize.y; y++)
		{
			ofDrawCircle(grid[x][y]->point.x*gridSpacing, grid[x][y]->point.y*gridSpacing, 1);
		}
	}
	ofSetLineWidth(1);
	ofSetColor(0, 0, 0, 255);
	for (int i = 0; i < lines.size(); i++)
	{
		ofDrawLine(lines[i].p1*gridSpacing, lines[i].p2*gridSpacing);
	}
	
	drawSnake();

	fboArt.end();

	//FBO draws in parent
	FrameGeometry::draw();		
}

void SnakesLife::update()
{
	// update algorithm here
	timeline->stepTo(ofGetElapsedTimef());
	animation = anim;
	if ((ofGetFrameNum() % tickRate) == 0 && !(squareCount == (gridSize.x - 1) * (gridSize.y - 1)) )
	{
		//cout << squareCount << " / " << (gridSize.x - 1) * (gridSize.y - 1) << endl;
		tick();
	}
}

void SnakesLife::createGrid(int width, int height)
{
	if (!grid.empty())
	{
		grid.clear();
	}
	//setSize
	grid.resize(width);
	for (int x = 0; x < width; x++)
	{
		grid[x].resize(height);
	}

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			PointDataPtr tempPointData = PointDataPtr(new PointData());
			//tempPointData->neighbors.clear();
			tempPointData->point.set(x, y);
			grid[x][y] = tempPointData;
		}
	}
	for (int x = 0; x < grid.size(); x++)
	{
		for (int y = 0; y < grid[x].size(); y++)
		{

			if (x > 0)
			{
				grid[x][y]->left = grid[x - 1][y];//adding LEFT
				grid[x][y]->addedLeft = true;
			}
			else if (x == 0)
			{//disable left
				grid[x][y]->isLeft = true;
			}

			if (x <  grid.size()-1)
			{
				grid[x][y]->right = grid[x + 1][y];//adding RIGHT 
				grid[x][y]->addedRight = true;
			}
			else if (x ==  grid.size() - 1)
			{//disable right
				grid[x][y]->isRight = true;
			}

			if (y > 0)
			{
				grid[x][y]->top = grid[x][y - 1];//adding TOP
				grid[x][y]->addedTop = true;
			}
			else if (y == 0)
			{
				grid[x][y]->isTop = true;
			}

			if (y < grid[x].size()-1)
			{ 
				grid[x][y]->bot = grid[x][y + 1];//adding BOT
				grid[x][y]->addedBot = true;
			}
			else if (y == grid[x].size() - 1)
			{
				grid[x][y]->isBot = true;
			}
			//auto engine = default_random_engine{};
			//shuffle(begin(grid[x][y]->neighbors), end(grid[x][y]->neighbors), engine);
		}
	}

}
void SnakesLife::createSquareGrid(int width, int height)
{
	if (!squareGrid.empty())
	{
		squareGrid.clear();
	}

	int sWidth = width - 1;
	int sHeight = height - 1;

	if (!squareGrid.empty())
	{
		squareGrid.clear();
	}
	//setSize
	squareGrid.resize(sWidth);
	for (int x = 0; x < sWidth; x++)
	{
		squareGrid[x].resize(sHeight);
	}

	for (int x = 0; x < sWidth; x++)
	{
		for (int y = 0; y < sHeight; y++)
		{
			SquareData tempSquare;
			tempSquare.point = grid[x][y]->point;
			tempSquare.topLeft = grid[x][y];
			tempSquare.topRight = grid[x + 1][y];
			tempSquare.botLeft = grid[x][y + 1];
			tempSquare.botRight = grid[x + 1][y + 1];
			tempSquare.color.set(0, 0, 0, 50);
			squareGrid[x][y] = tempSquare;
		}
	}
}

void SnakesLife::tick()
{
	stepNext();
		
	animSetup();

}

void SnakesLife::createSnake()
{
	if (!snakes.empty())
	{
		snakes.clear();
	}

	for (int i = 0; i < numSnakes; i++)
	{
		snake.currentPoint = grid[ofRandom(0, gridSize.x - 1)][ofRandom(0, gridSize.y - 1)];
		snake.selector = { 1, 2, 3, 4 };
		snakes.push_back(snake);
	}
	//snake.currentPoint = grid[ofRandom(0,gridSize.x-1)][ofRandom(0,gridSize.y - 1)];
	//snake.selector = { 1, 2, 3, 4 };
	findNext();
	animSetup();
}

void SnakesLife::findNext()
{
	for (int j = 0; j < snakes.size(); j++)
	{
		int x = snakes[j].currentPoint->point.x;
		int y = snakes[j].currentPoint->point.y;
		PointDataPtr curP = grid[x][y];
		snake.selector = { 1, 2, 3, 4 };
		//auto engine = default_random_engine{};
		//shuffle(begin(snake.selector), end(snake.selector), engine);
		random_shuffle(begin(snakes[j].selector), end(snakes[j].selector));

		for (int i = 0; i < snakes[j].selector.size(); i++)
		{
			int index = snakes[j].selector[i];

			if (snakes[j].currentPoint->isTop
				&&  snakes[j].currentPoint->isRight
				&&  snakes[j].currentPoint->isBot
				&&  snakes[j].currentPoint->isLeft)
			{
				//cout << "DEAD !!!" << endl;

				Line tempLine;
				tempLine.p1.set(snakes[j].currentPoint->point);
				tempLine.p2.set(snakes[j].nextPoint->point);
				lines.push_back(tempLine);

				respawn(j);

				break;
			}

			if (index == 1 && !grid[x][y]->isTop &&  grid[x][y]->addedTop)//top
			{
				snakes[j].nextPoint = grid[x][y - 1];
				snakes[j].currentPoint->isTop = true;
				snakes[j].nextPoint->isBot = true;
				break;
			}

			if (index == 2 && !grid[x][y]->isRight &&  grid[x][y]->addedRight)//right
			{
				snakes[j].nextPoint = grid[x + 1][y];
				snakes[j].currentPoint->isRight = true;
				snakes[j].nextPoint->isLeft = true;
				break;
			}

			if (index == 3 && !grid[x][y]->isBot &&  grid[x][y]->addedBot)//bot
			{
				snakes[j].nextPoint = grid[x][y + 1];
				snakes[j].currentPoint->isBot = true;
				snakes[j].nextPoint->isTop = true;
				break;
			}

			if (index == 4 && !grid[x][y]->isLeft &&  grid[x][y]->addedLeft)//left
			{
				snakes[j].nextPoint = grid[x - 1][y];
				snakes[j].currentPoint->isLeft = true;
				snakes[j].nextPoint->isRight = true;
				break;
			}
		}//end loop for neighbors;
	}
	
}

void SnakesLife::findNext(int j)
{
	int x = snakes[j].currentPoint->point.x;
	int y = snakes[j].currentPoint->point.y;
	PointDataPtr curP = grid[x][y];
	snake.selector = { 1, 2, 3, 4 };
	random_shuffle(begin(snakes[j].selector), end(snakes[j].selector));

	for (int i = 0; i < snakes[j].selector.size(); i++)
	{
		int index = snakes[j].selector[i];

		if (snakes[j].currentPoint->isTop
			&&  snakes[j].currentPoint->isRight
			&&  snakes[j].currentPoint->isBot
			&&  snakes[j].currentPoint->isLeft)
		{
			//cout << "DEAD !!!" << endl;
			Line tempLine;
			tempLine.p1.set(snakes[j].currentPoint->point);
			tempLine.p2.set(snakes[j].nextPoint->point);
			lines.push_back(tempLine);

			respawn(j);

			break;
		}

		if (index == 1 && !grid[x][y]->isTop &&  grid[x][y]->addedTop)//top
		{
			snakes[j].nextPoint = grid[x][y - 1];
			snakes[j].currentPoint->isTop = true;
			snakes[j].nextPoint->isBot = true;
			break;
		}

		if (index == 2 && !grid[x][y]->isRight &&  grid[x][y]->addedRight)//right
		{
			snakes[j].nextPoint = grid[x + 1][y];
			snakes[j].currentPoint->isRight = true;
			snakes[j].nextPoint->isLeft = true;
			break;
		}

		if (index == 3 && !grid[x][y]->isBot &&  grid[x][y]->addedBot)//bot
		{
			snakes[j].nextPoint = grid[x][y + 1];
			snakes[j].currentPoint->isBot = true;
			snakes[j].nextPoint->isTop = true;
			break;
		}

		if (index == 4 && !grid[x][y]->isLeft &&  grid[x][y]->addedLeft)//left
		{
			snakes[j].nextPoint = grid[x - 1][y];
			snakes[j].currentPoint->isLeft = true;
			snakes[j].nextPoint->isRight = true;
			break;
		}
	}//end loop for neighbors;
}

void SnakesLife::stepNext()
{
	for (int i = 0; i < snakes.size(); i++)
	{
		Line tempLine;
		tempLine.p1.set(snakes[i].currentPoint->point);
		tempLine.p2.set(snakes[i].nextPoint->point);
		lines.push_back(tempLine);

		snakes[i].currentPoint = snakes[i].nextPoint;
	}
	resetSquaresAnim();
	anim = 0;
	findNext();
	animSetup();
	for (int i = 0; i < snakes.size(); i++)
	{
		checkSquares(snakes[i].currentPoint, snakes[i].nextPoint);
	}

	
}
void SnakesLife::respawn(int index)
{
	
	snakes[index].currentPoint = grid[ofRandom(gridSize.x)][ofRandom(gridSize.y)];
	if (snakes[index].currentPoint->isLeft && snakes[index].currentPoint->isRight && snakes[index].currentPoint->isTop && snakes[index].currentPoint->isBot)
	{
		respawn(index);
	}
	else
	{
		findNext(index);
	}
}

void SnakesLife::drawSquares()
{
	for (int x = 0; x < gridSize.x - 1; x++)
	{
		for (int y = 0; y < gridSize.y - 1; y++)
		{
			if (squareGrid[x][y].inTrans)
			{
				
				ofSetColor(ofColor(squareGrid[x][y].color.r, squareGrid[x][y].color.g, squareGrid[x][y].color.b, ofLerp(0, squareGrid[x][y].color.a, anim)));
			}
			else
			{
				ofSetColor(squareGrid[x][y].color);
			}
			if (squareGrid[x][y].isFilled)
			{
				ofDrawRectangle(ofRectangle(squareGrid[x][y].topLeft->point*gridSpacing, squareGrid[x][y].botRight->point*gridSpacing));
			}
			
		}
	}
}

void SnakesLife::checkSquares(PointDataPtr point1ptr, PointDataPtr point2ptr)
{
	ofPoint point1 = point1ptr->point;
	ofPoint point2 = point2ptr->point;
	if (point1.x + 1 == point2.x)
	{//right
		//cout << "right" << endl;
		checkSquare(ofPoint(point1.x, point1.y - 1));
		checkSquare(ofPoint(point1.x, point1.y));
	}
	else if (point1.x - 1 == point2.x)
	{//left
		//cout << "left" << endl;
		checkSquare(ofPoint(point2.x, point2.y - 1));
		checkSquare(ofPoint(point2.x, point2.y));
	}
	else if (point1.y - 1 == point2.y)
	{//top
		//cout << "top" << endl;
		checkSquare(ofPoint(point2.x - 1, point2.y));
		checkSquare(ofPoint(point2.x, point2.y));
	}
	else if (point1.y + 1 == point2.y)
	{//bot
		//cout << "bot" << endl;
		checkSquare(ofPoint(point1.x - 1, point1.y));
		checkSquare(ofPoint(point1.x, point1.y));
	}
}

void SnakesLife::checkSquare(ofPoint point)
{
	if (point.x < 0)
	{
		return;
	}
	if (point.x > squareGrid.size()-1)
	{
		return;
	}
	if (point.y < 0)
	{
		return;
	}
	if (point.y > squareGrid[0].size()-1)
	{
		return;
	}
	if (!squareGrid[point.x][point.y].isFilled)
	{
		int count;
		bool alredyDone = squareGrid[point.x][point.y].isFilled;
		count = (int)squareGrid[point.x][point.y].topLeft->isBot + (int)squareGrid[point.x][point.y].topLeft->isRight;
		count = count + (int)squareGrid[point.x][point.y].botRight->isLeft + (int)squareGrid[point.x][point.y].botRight->isTop;
		if (count == 4 && !alredyDone)
		{
			squareCount++;
			squareGrid[point.x][point.y].isFilled = true;
			squareGrid[point.x][point.y].inTrans = true;
			squareGrid[point.x][point.y].color.set(squareColorsVec[ofRandom(squareColorsVec.size())]);
		}
	}
}

void SnakesLife::animSetup()
{
	float animTime = tickRate / 60.;
	float startAnim = 0.;
	float endAnim = 1.;

	timeline->apply(
		&anim,
		startAnim,
		endAnim,
		animTime//,
		//ofxCinderTimeline::EaseInOutExpo()
	);// .finishFn(bind(&SnakesLife::animFinish, this));
}

void SnakesLife::animFinish()
{
	animSetup();
}

void SnakesLife::drawSnake()
{
	for (int i = 0; i < snakes.size(); i++)
	{
		ofPoint pos;
		pos.x = ofLerp(snakes[i].currentPoint->point.x, snakes[i].nextPoint->point.x, anim);
		pos.y = ofLerp(snakes[i].currentPoint->point.y, snakes[i].nextPoint->point.y, anim);

		ofSetColor(lineColor);
		ofDrawLine(snakes[i].currentPoint->point * gridSpacing, pos * gridSpacing);

		ofSetColor(snakeColor);
		ofDrawCircle(pos.x*gridSpacing, pos.y*gridSpacing, 5);
	}
	
}

void SnakesLife::resetSquaresAnim()
{
	for (int x = 0; x < gridSize.x - 1; x++)
	{
		for (int y = 0; y < gridSize.y - 1; y++)
		{
			if (squareGrid[x][y].inTrans)
			{
				squareGrid[x][y].inTrans = false;
			}
		}
	}
}

void SnakesLife::reset()
{ 
	snakes.clear();
	lines.clear();
	grid.clear();
	squareGrid.clear();

}