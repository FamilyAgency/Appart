#include "GameOfLife.h"
#include "ofxEasing.h"

using namespace frames;

GameOfLife::GameOfLife(float width, float height) : FrameGeometry(width, height)
{
	spoutChannelName = "Geometry Art";
	name = "Game Of Life Art";

	lerpTimeline = ofxCinderTimeline::Timeline::create();
	lerpTimeline->stepTo(ofGetElapsedTimef());

	currentFrame.allocate(width, height, GL_RGBA);
	pastFrame.allocate(width, height, GL_RGBA);
	mask.allocate(width, height, GL_RGBA);

	pastFrame.begin();
	pastFrame.end();

	pixelAlphaShader = ofPtr<PixelAlphaShader>(new PixelAlphaShader(width, height));
	pixelAlphaShader->setupTextures(currentFrame.getTextureReference(), pastFrame.getTextureReference(), mask.getTextureReference());

	setupColors();
}

GameOfLife::~GameOfLife()
{
	currentFrame.destroy();
	pastFrame.destroy();
	mask.destroy();

	for (int i = 0; i<cols; i++)
	{
		//TODO: wtf here? - thanks xD
		//delete  grid[i];
	}

	//if (grid)
	{
		//TODO: wtf here?
		//	delete grid;
	}
}

void GameOfLife::setup()
{
	//TODO:  currentFrame.allocate(getWidth(), getHeight(), GL_RGBA, 8);
	currentFrame.allocate(getWidth(), getHeight(), GL_RGBA);
	pastFrame.allocate(getWidth(), getHeight(), GL_RGBA);
	mask.allocate(getWidth(), getHeight(), GL_RGBA);

	pixelAlphaShader = ofPtr<PixelAlphaShader>(new PixelAlphaShader(getWidth(), getHeight()));
	pixelAlphaShader->setupTextures(currentFrame.getTextureReference(), pastFrame.getTextureReference(), mask.getTextureReference());

	setColors();
	cols = getWidth() / cellSize;
	rows = getHeight() / cellSize;
	float ratio = getWidth() / getHeight();
	bgColor.set(50, 50, 50, 0);
	cellWidth = cellSize;
	cellHeight = cellSize;
	renew = Renew::PATTERNS;
	
	create();
	


	patternBase = getPatterns();
	initSpawn(initSpawnCount);

	frameToPast();
	tick();
	frameToCurrent();

	colorLerpSetup();
}

void GameOfLife::clean()
{
	currentFrame.destroy();
	pastFrame.destroy();
	mask.destroy();
}

void frames::GameOfLife::setColors()
{
	setupCount++;
	int colorId = ofClamp(setupCount, 0, mainColors.size()-1);
	background = mainColors[colorId];
	lifeColor = bgColors[colorId];
	//cout << "colors changed BG:" << background << " main:" << lifeColor << endl;
}

void frames::GameOfLife::setupColors()
{
	ofColor tempMainColor;
	ofColor tempBgColor;

	tempMainColor.set(210, 255, 7);
	tempBgColor.set(0, 39, 13);
	mainColors.push_back(tempMainColor);
	bgColors.push_back(tempBgColor);

	tempMainColor.set(62, 78, 80);
	tempBgColor.set(250, 207, 173);
	mainColors.push_back(tempMainColor);
	bgColors.push_back(tempBgColor);

	tempMainColor.set(29, 32, 31);
	tempBgColor.set(234, 210, 172);
	mainColors.push_back(tempMainColor);
	bgColors.push_back(tempBgColor);

	tempMainColor.set(255, 255, 255);
	tempBgColor.set(130, 9, 25);
	mainColors.push_back(tempMainColor);
	bgColors.push_back(tempBgColor);

	tempMainColor.set(207, 92, 54);
	tempBgColor.set(0, 0, 0);
	mainColors.push_back(tempMainColor);
	bgColors.push_back(tempBgColor);

	tempMainColor.set(252, 161, 125);
	tempBgColor.set(25, 11, 40);
	mainColors.push_back(tempMainColor);
	bgColors.push_back(tempBgColor);
}

void GameOfLife::initSpawn(int time)
{
	for (int i = 0; i < time; i++)
	{
		switch (renew)
		{
		case GameOfLife::Renew::SINGLE:
			placeRandom(grid);
			break;

		case GameOfLife::Renew::FULL:
			if (ofGetFrameNum() % 500 == 0)
			{
				setRandom(grid);
			}
			break;

		case GameOfLife::Renew::INFIN:

			break;
		case GameOfLife::Renew::PATTERNS:
			drawPattern(selectPattern(patternBase));
			break;
		}
	}
}

void GameOfLife::setRandom(vector<vector<cell>> grid)
{
	for (int i = 0; i<cols; i++)
	{
		for (int j = 0; j<rows; j++)
		{
			bool thisRnd = rand() & 1;
			grid[i][j].currState = thisRnd;
		}
	}
}

void GameOfLife::placeRandom(vector<vector<cell>> grid)
{
	int rndCol = rand() % (cols - 2) + 1;
	int rndRow = rand() % (rows - 2) + 1;
	grid[rndCol + 1][rndRow].currState = true;//right
	grid[rndCol][rndRow + 1].currState = true;//top
	grid[rndCol - 1][rndRow].currState = true;//left
	grid[rndCol][rndRow - 1].currState = true;//bot
}

void GameOfLife::GUISetup()
{
#ifndef release
	FrameGeometry::GUISetup();
	//gui.add(background.setup("BG color", ofColor(0, 0, 0, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
	//gui.add(lifeColor.setup("life color", ofColor(255, 255, 255), ofColor(0, 0, 0), ofColor(255, 255, 255)));
	gui.add(highlightSwitch.setup("highlight", true));
	gui.add(outputTrans.setup("outputTrans ", 0., 0., 1.));
#else
	background.setup("BG color", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255));
	lifeColor.setup("life color", ofColor(0, 0, 0), ofColor(0, 0, 0), ofColor(255, 255, 255));
	highlightSwitch.setup("highlight", true);
	outputTrans.setup("outputTrans ", 0., 0., 1.);
#endif

}

void GameOfLife::draw()
{
	outputTrans = colorLerp;
	fboArt.begin();
	ofClear(255, 255, 255, 0);
	ofBackground(background);

	mask.begin();
	{
		ofClear(255, 255, 255, 0);
		ofSetColor(255, 255, 255, colorLerp * 255);
		ofDrawRectangle(0, 0, getWidth(), getHeight());
	}
	mask.end();

	ofSetColor(bgColor);
	ofDrawRectangle(0, 0, currentFrame.getWidth(), currentFrame.getHeight());
	ofSetColor(lifeColor);

	pixelAlphaShader->getTexture().draw(0, 0);

	fboArt.end();
	FrameGeometry::draw();
}

void GameOfLife::gliderGun(vector<vector<cell>> grid, int startPosX, int startPosY)
{

}

void GameOfLife::pufferTrain(vector<vector<cell>> grid, int startPosX, int startPosY)
{

}

void GameOfLife::create()
{
	cell emtpyCell;
	emtpyCell.currState = false;
	emtpyCell.nextState = false;
	emtpyCell.color = bgColor;

	grid.resize(cols, vector<cell>(rows, emtpyCell));
}

void GameOfLife::clear()
{
	for (int i = 0; i<cols; i++)
	{
		for (int j = 0; j<rows; j++)
		{
			grid[i][j].currState = false;
			grid[i][j].nextState = false;
			grid[i][j].color = bgColor;
		}
	}
}

int GameOfLife::currState(const int& col, const int& row)
{
	return (col >= 0 && row >= 0 &&
		col < cols && row < rows &&
		grid[col][row].currState == true) ? 1 : 0;
}

int GameOfLife::getNumActiveNeighbors(const int& colIndex, const int& rowIndex)
{
	int ret = 0;

	int prevCol = colIndex - 1;
	int nextCol = colIndex + 1;
	int prevRow = rowIndex - 1;
	int nextRow = rowIndex + 1;

	ret += currState(prevCol, prevRow);
	ret += currState(prevCol, rowIndex);
	ret += currState(prevCol, nextRow);

	ret += currState(colIndex, prevRow);
	ret += currState(colIndex, nextRow);

	ret += currState(nextCol, prevRow);
	ret += currState(nextCol, rowIndex);
	ret += currState(nextCol, nextRow);

	return ret;
}

void GameOfLife::update()
{
	pixelAlphaShader->update();
	lerpTimeline->stepTo(ofGetElapsedTimef());

	if ((ofGetFrameNum() % maxTick) == 0)
	{
		frameToPast();
		tick();
		frameToCurrent();
		//startTransition();
	}
	if (ofGetFrameNum() % renewFreq == 0)
	{
		switch (renew)
		{
		case GameOfLife::Renew::SINGLE:
			placeRandom(grid);
			break;

		case GameOfLife::Renew::FULL:
			if (ofGetFrameNum() % 500 == 0)
			{
				setRandom(grid);
			}
			break;

		case GameOfLife::Renew::INFIN:
			break;

		case GameOfLife::Renew::PATTERNS:
			drawPattern(selectPattern(patternBase));
			break;
		}
	}
}

void GameOfLife::tick()
{
	for (int i = 0; i<cols; i++)
	{
		for (int j = 0; j<rows; j++)
		{
			cell *thisCell = &grid[i][j];
			thisCell->activeNeighbors = getNumActiveNeighbors(i, j);
			bool currState = thisCell->currState;
			int activeNeighbors = thisCell->activeNeighbors;

			if (currState == true && activeNeighbors < 2)
			{
				thisCell->nextState = false;
				thisCell->nextColor = bgColor;
			}
			else if (currState == true && activeNeighbors > 3)
			{
				thisCell->nextState = false;
				thisCell->nextColor = bgColor;
			}
			else if (currState == true && activeNeighbors > 1 && activeNeighbors < 4)
			{
				thisCell->nextState = true;
				thisCell->color = bgColor;
				thisCell->nextColor = lifeColor;
			}
			else if (currState == false && activeNeighbors == 3)
			{
				thisCell->nextState = true;
				thisCell->nextColor = lifeColor;
				if (highlightSwitch == true)
				{
					thisCell->nextColor = lifeColor;
				}
				else
				{
					thisCell->nextColor = lifeColor;
				}
			}
		}
	}

	makeNextStateCurrent();
}

void GameOfLife::frameToPast()
{
	colorLerp = 0;
	colorLerpSetup();
	pastFrame.begin();
	{
		ofClear(255, 255, 255, 0);
		currentFrame.draw(0, 0);
	}
	pastFrame.end();

	currentFrame.begin();
	{
		ofClear(255, 255, 255, 0);
	}
	currentFrame.end();
}

void GameOfLife::frameToCurrent()
{
	currentFrame.begin();
	{
		ofClear(255, 255, 255, 0);
		for (int i = 0; i<cols; i++)
		{
			for (int j = 0; j<rows; j++)
			{
				cell thisCell = grid[i][j];
				if (thisCell.currState == true)
				{
					ofSetColor(lifeColor);
					if (isCircle)
					{
						ofCircle(i*cellWidth + cellWidth*.5, j*cellHeight + cellHeight*.5, cellHeight*.5 - circlePadding);
					}
					else
					{
						ofRect(i*cellWidth, j*cellHeight, cellWidth, cellHeight);
					}

				}
			}
		}
	}
	currentFrame.end();
}

void GameOfLife::makeNextStateCurrent()
{
	for (int i = 0; i<cols; i++)
	{
		for (int j = 0; j<rows; j++)
		{
			grid[i][j].currState = grid[i][j].nextState;
		}
	}
}

void GameOfLife::colorLerpSetup()
{
	float animTime = (1. / 60.)*(float)maxTick;
	float startPos = 0.;
	float endPos = 1.;
	lerpTimeline->apply(&colorLerp,
		startPos,
		endPos,
		animTime)
		.finishFn(std::bind(&GameOfLife::colorLerpFin, this));
}

void GameOfLife::colorLerpFin()
{

}

vector<Pattern> GameOfLife::getPatterns()
{
	shared_ptr<Patterns> patternBase = shared_ptr<Patterns>(new Patterns());
	vector<Pattern> patternsVec = patternBase->patternBase;
	return patternsVec;
}

Pattern GameOfLife::selectPattern(vector<Pattern> patternBase)
{
	int rndSelect = ofRandom(patternBase.size());
	return patternBase[rndSelect];
}

void GameOfLife::drawPattern(const Pattern& pattern)
{
	int startPosX = ofRandom(cols - pattern.w);
	int startPosY = ofRandom(rows - pattern.h);
	int col;
	int row;
	for (int i = 0; i < pattern.data.length(); i++)
	{
		col = i % pattern.w + startPosX;
		row = i / pattern.w + startPosY;
		grid[col][row].currState = (bool)pattern.data.at(i);
		grid[col][row].nextColor = lifeColor;
	}
}

void GameOfLife::drawPattern(const Pattern& pattern, int startPosX, int startPosY)
{
	if ((startPosX + pattern.w) > cols)
	{
		startPosX = cols - pattern.w;
	}
	else if (startPosX < 0)
	{
		startPosX = 0;
	}

	if ((startPosY + pattern.h) > rows)
	{
		startPosY = rows - pattern.h;
	}
	else if (startPosY < 0)
	{
		startPosY = 0;
	}

	for (int i = 0; i < pattern.data.length(); i++)
	{
		int col = i % pattern.w + startPosX;
		int row = i / pattern.w + startPosY;
		grid[col][row].currState = true;
	}
}

Patterns::Patterns()
{
	Pattern block;//1
	block.type = type::STILL;
	block.w = 2;
	block.h = 2;
	block.data = "1111";
	patternBase.push_back(block);

	Pattern beehive;//2
	beehive.type = type::STILL;
	beehive.w = 4;
	beehive.h = 3;
	beehive.data = "011010010110";
	patternBase.push_back(beehive);

	Pattern loaf;//3
	loaf.type = type::STILL;
	loaf.w = 4;
	loaf.h = 4;
	loaf.data = "0110100101010010";
	patternBase.push_back(loaf);

	Pattern boat;//4
	boat.type = type::STILL;
	boat.w = 3;
	boat.h = 3;
	boat.data = "110101010";
	patternBase.push_back(boat);

	Pattern tub;//5
	tub.type = type::STILL;
	tub.w = 3;
	tub.h = 3;
	tub.data = "010101010";
	patternBase.push_back(tub);

	Pattern blinker;//6
	blinker.type = type::INFIN;
	blinker.w = 3;
	blinker.h = 3;
	blinker.data = "000111000";
	patternBase.push_back(blinker);

	Pattern toad;//7
	toad.type = type::INFIN;
	toad.w = 4;
	toad.h = 4;
	toad.data = "0000011111100000";
	patternBase.push_back(toad);

	Pattern beacon;//8
	beacon.type = type::INFIN;
	beacon.w = 4;
	beacon.h = 4;
	beacon.data = "1100110000110011";
	patternBase.push_back(beacon);

	Pattern pentadecathlon;//9
	pentadecathlon.type = type::INFIN;
	pentadecathlon.w = 3;
	pentadecathlon.h = 12;
	pentadecathlon.data = "111010010111000111111000111010010111";
	patternBase.push_back(pentadecathlon);

	Pattern ship1;//10
	ship1.type = type::SHIP;
	ship1.w = 16;
	ship1.h = 5;
	ship1.data = "00000001101000000000110101101110011110011000000110000100010001100110000000000000";
	patternBase.push_back(ship1);

	Pattern ship2;
	ship2.type = type::SHIP;
	ship2.w = 8;
	ship2.h = 12;
	ship2.data = "011001100001100000011000101001011000000100000000100000010110011000111100000000000001100000011000";
	patternBase.push_back(ship2);
}

Patterns::~Patterns()
{
	patternBase.clear();
}

vector<Pattern> Patterns::getPatternBase()
{
	return patternBase;
}

////////////////////////////////////////////////////////
//////////////////////// Shader ////////////////////////
////////////////////////////////////////////////////////

PixelAlphaShader::PixelAlphaShader(float width, float height) : FrameShader(width, height)
{
	name = "Pixel Shader";
	spoutChannelName = "none";
}

void PixelAlphaShader::setupTextures(ofTexture& tex1, ofTexture& tex2, ofTexture& tex3)
{
	addParameter(new ParameterT("texnew", tex1, 1));
	addParameter(new ParameterT("texold", tex2, 2));
	addParameter(new ParameterT("texmask", tex3, 3));

	setupShader();
}

void PixelAlphaShader::setup()
{

}

void PixelAlphaShader::update()
{
	FrameShader::update();
}

string PixelAlphaShader::getFragSrc()
{
	return	GLSL_STRING(120,
		uniform sampler2DRect texold;
	uniform sampler2DRect texnew;
	uniform sampler2DRect texmask;
	void main()
	{
		vec2 uv = gl_FragCoord.xy;
		vec4 texelold = texture2DRect(texold, uv);
		vec4 texelnew = texture2DRect(texnew, uv);
		vec4 texelmask = texture2DRect(texmask, uv);

		if (((texelnew.a == 1.) && (texelold.a == 1.)) || ((texelnew.a == 0.) && (texelold.a == 0.)))
		{
			//both ON. do nothing
		}
		else if ((texelnew.a == 0.) && (texelold.a == 1.))
		{
			//goes from ON to OFF
			texelnew.a = 1. - texelmask.a;
			texelnew.rgb = texelold.rgb;
		}
		else if ((texelnew.a == 1.) && (texelold.a == 0.))
		{
			//goes from OFF to ON
			texelnew.a = texelnew.a * texelmask.a;

		}
		gl_FragColor = vec4(texelnew.rgb, texelnew.a);
	}
	);
}
