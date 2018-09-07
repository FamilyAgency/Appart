#include "MondrianPingPong.h"

using namespace frames;

MondrianPingPong::MondrianPingPong(float width, float height) : FrameGeometry(width, height)
{
	spoutChannelName = "Geometry Art";
	name = "Mondrian Ping-Pong Art";
}

MondrianPingPongHor::MondrianPingPongHor(float width, float height) : MondrianPingPong(width, height)
{
	screen = ofRectangle(0, 0, height, width);
}

MondrianPingPong::~MondrianPingPong()
{	
	
}

MondrianPingPongHor::~MondrianPingPongHor()
{

}

void MondrianPingPong::setup()
{
	screen = setupOrientation();
	zoneHorOffset = setupZoneHorOffset();

	zoneTopLeft.set(ballSize*.5 - zoneHorOffset + lineWidth, ballSize*.5 + playerHeight + lineWidth);
	zoneTopRight.set(screen.getWidth() - ballSize*.5 + zoneHorOffset - lineWidth, ballSize*.5 + playerHeight + lineWidth);
	zoneBotLeft.set(ballSize*.5 - zoneHorOffset + lineWidth, screen.getHeight() - ballSize*.5 - playerHeight - lineWidth);
	zoneBotRight.set(screen.getWidth() - ballSize*.5 + zoneHorOffset - lineWidth, screen.getHeight() - ballSize*.5 - playerHeight - lineWidth);

	ballZone.set(zoneTopLeft, zoneBotRight);

	playerTop = ofPtr<Player>(new Player(PlayerPosition::TOP, screen, ballZone, playerHeight, playerWidth, passiveSpeed, passiveMovWidth));
	playerBot = ofPtr<Player>(new Player(PlayerPosition::BOTTOM, screen, ballZone, playerHeight, playerWidth, passiveSpeed, passiveMovWidth));
	passivePlayer = playerTop;
	activePlayer = playerBot;
	passivePlayer->state = Player::PlayerState::WANDERING;
	activePlayer->state = Player::PlayerState::WANDERING;

	ball = ofPtr<Ball>(new Ball(screen, ballZone));

	ballColor.set(255, 255, 0);
	player1Color.set(255, 0, 0);
	player2Color.set(0, 0, 255);

	ofAddListener(ball->halfWay, this, &MondrianPingPong::startCatching);
	ofAddListener(ball->hitBot,  this, &MondrianPingPong::changeRoles);
	ofAddListener(ball->hitTop,  this, &MondrianPingPong::changeRoles);
}

void MondrianPingPong::clean()
{
	ofRemoveListener(ball->halfWay,  this, &MondrianPingPong::startCatching);
	ofRemoveListener(ball->hitBot,   this, &MondrianPingPong::changeRoles);
	ofRemoveListener(ball->hitTop,   this, &MondrianPingPong::changeRoles);
}

ofRectangle frames::MondrianPingPong::setupOrientation()
{
	return ofRectangle(0, 0, getWidth(), getHeight());
}

ofRectangle frames::MondrianPingPongHor::setupOrientation()
{
	return ofRectangle(0, 0, getHeight(), getWidth());
}

float frames::MondrianPingPong::setupZoneHorOffset()
{
	return screen.getWidth() * 0.2;
}

float frames::MondrianPingPongHor::setupZoneHorOffset()
{
	return screen.getWidth() * 0.25;
}

void frames::MondrianPingPong::startCatching()
{	
	ofPoint finPos = getFinPoint();
	activePlayer->setFinalePosition(finPos);
	activePlayer->state = Player::PlayerState::STARTCATCH;
}

void frames::MondrianPingPong::changeRoles()
{
	tempPlayer = passivePlayer;
	passivePlayer = activePlayer;
	activePlayer = tempPlayer;
	activePlayer->setActive();
	passivePlayer->setPassive();
}

ofPoint frames::MondrianPingPong::getFinPoint()
{
	ofPoint tempCross;
	ofPoint tempPos = ball->Pos;
	ofPoint tempVec = ball->Vec;
	float width = screen.getWidth();
	float height = screen.getHeight();

	float vectorLong = sqrt(width*width + height*height);
	ofVec2f VecSearch = (tempVec / speed)  *vectorLong;
	int trys = 0;
	if ((mathTools().isCrossing(
		activePlayer->playerZonePoint1,
		activePlayer->playerZonePoint2,
		tempPos,
		tempPos + VecSearch))
		== true)
	{
		//crossing with player
		//cout << "crossing with player" << endl;
		ofPoint crossPoint;
		crossPoint = mathTools().getCrossPoint2(tempPos, tempPos + VecSearch, activePlayer->playerZonePoint1, activePlayer->playerZonePoint2);
		//cout << "return fin pos " << crossPoint << endl;
		return crossPoint;
	}
	else
	{
		//cout << "crossing with WALL" << endl;
		//not crossing with player
		while (!(mathTools().isCrossing(tempPos, tempPos + VecSearch, activePlayer->playerZonePoint1, activePlayer->playerZonePoint2)))
		{
			if (VecSearch.x > 0)
			{
				//right
				//cout << "rigt wall" << endl;
				tempPos = mathTools().getCrossPoint2(tempPos, tempPos + VecSearch, zoneTopRight, zoneBotRight);
				VecSearch.x = -VecSearch.x;
			}
			else
			{
				//left
				//cout << "left wall" << endl;
				tempPos = mathTools().getCrossPoint2(tempPos, tempPos + VecSearch, zoneTopLeft, zoneBotLeft);
				VecSearch.x = -VecSearch.x;
			}
			trys++;
			if (trys >= 10)
			{
				ofLogError("Can't find finale position");
				activePlayer->state = Player::PlayerState::STARTCATCH;
				break;
			}
		}
		//cout << "FINISH!" << endl;
		//finally it is crossing
		ofPoint crossPoint;
		crossPoint = mathTools().getCrossPoint2(tempPos, tempPos + VecSearch, activePlayer->playerZonePoint1, activePlayer->playerZonePoint2);
		//cout << "return fin pos " << crossPoint << endl;
		return crossPoint;
	}
	//return ofPoint();
}

void MondrianPingPong::GUISetup()
{
#ifndef release
	FrameGeometry::GUISetup();

	gui.add(passiveSpeed.setup("passive speed", 8., 3., 20.));
	gui.add(passiveMovWidth.setup("passive mov freq", 5., 1., 50.));
	gui.add(lineWidthgui.setup("line width", 10., 1., 20.));
	passiveSpeed.addListener(this, &MondrianPingPong::passiveSpeedListener);
	passiveMovWidth.addListener(this, &MondrianPingPong::passiveMovWidthListener);
#endif
}

void MondrianPingPong::passiveSpeedListener(float& value)
{
	playerBot->passiveSpeed = value;
	playerTop->passiveSpeed = value;
}

void MondrianPingPong::passiveMovWidthListener(float& value)
{
	playerBot->passiveMovWidth = value;
	playerTop->passiveMovWidth = value;
}

void MondrianPingPong::draw()
{
	// draw in FBO
	fboArt.begin();
	ofClear(255, 255, 255, 0);
	ofBackground(255);
	ofSetColor(0);

#ifndef release
	lineWidth = lineWidthgui;
#endif

	ofPushMatrix();
	{
		ofSetRectMode(OF_RECTMODE_CENTER);

		ofTranslate(screen.getWidth()*0.1, screen.getHeight()*0.1);
		ofScale(generalScale, generalScale);

		drawSetOrientation();
		
		
		ofSetRectMode(OF_RECTMODE_CORNER);

		ofPoint tempTopLeft;
		ofPoint tempBotRight;
		tempTopLeft.set(ball->Pos.x - (ballSize*0.5), ball->Pos.y - (ballSize*0.5));
		tempBotRight.set(ball->Pos.x + (ballSize*0.5), ball->Pos.y + (ballSize*0.5));
		drawLines(tempTopLeft, tempBotRight, lineWidth);

		tempTopLeft.set(playerTop->position.x - (playerWidth*0.5), playerTop->position.y - (playerHeight*0.5));
		tempBotRight.set(playerTop->position.x + (playerWidth*0.5), playerTop->position.y + (playerHeight*0.5));
		drawLines(tempTopLeft, tempBotRight, lineWidth);

		tempTopLeft.set(playerBot->position.x - (playerWidth*0.5), playerBot->position.y - (playerHeight*0.5));
		tempBotRight.set(playerBot->position.x + (playerWidth*0.5), playerBot->position.y + (playerHeight*0.5));
		drawLines(tempTopLeft, tempBotRight, lineWidth);

		ofSetColor(ballColor);
		ball->drawBody();

		ofSetColor(player1Color);
		playerTop->drawBody();


		ofSetColor(player2Color);
		playerBot->drawBody();

	}
	ofPopMatrix();
	fboArt.end();

	FrameGeometry::draw();
}

void MondrianPingPong::drawSetOrientation()
{
	//ofTranslate(screen.getWidth()*0.1, screen.getHeight()*0.1);
	//ofScale(generalScale, generalScale);
}

void frames::MondrianPingPongHor::drawSetOrientation()
{
	//ofTranslate(screen.getWidth()*0.1, screen.getHeight()*0.1);
	//ofScale(generalScale, generalScale);

	ofRotateZ(-90);
	ofTranslate(-1 * screen.getWidth(), 0, 0);
}

void MondrianPingPong::drawLines(const ofPoint& topLeft, const ofPoint& botRight, float width)
{

#ifndef release
	lineWidth = lineWidthgui;
#endif

	const int sidenums = 4;
	ofPushMatrix();
	{
		ofSetColor(0);
		ofTranslate(topLeft);
		for (int i = 0; i < sidenums; i++)
		{
			ofPushMatrix();
			{
				ofTranslate(-width*.5, -width*.5);
				ofSetRectMode(OF_RECTMODE_CENTER);
				ofRotateZ(i * 90);
				ofDrawRectangle(0, 0, 7000, lineWidth);
				ofSetRectMode(OF_RECTMODE_CORNER);

			}
			ofPopMatrix();
		}
	}
	ofPopMatrix();

	ofPushMatrix();
	{
		ofSetColor(0);
		ofTranslate(botRight);
		for (int i = 0; i < sidenums; i++)
		{
			ofPushMatrix();
			{
				ofTranslate(width*.5, width*.5);
				ofSetRectMode(OF_RECTMODE_CENTER);
				ofRotateZ(i * 90);
				ofDrawRectangle(0, 0, 7000, lineWidth);
				ofSetRectMode(OF_RECTMODE_CORNER);

			}
			ofPopMatrix();
		}
	}
	ofPopMatrix();
}

void MondrianPingPong::update()
{
	ball->update();
	activePlayer->activeUpdate(ball->Pos, ball->Vec);
	passivePlayer->passiveUpdate();
}


MondrianPingPong::Player::Player(MondrianPingPong::PlayerPosition position,
	ofRectangle screen,
	ofRectangle ballZone,
	float playerHeight,
	float playerWidth,
	float passiveSpeed,
	float passiveMovWidth)
{
	//this->game = game;
	this->passiveSpeed = passiveSpeed;
	this->passiveMovWidth = passiveMovWidth;
	height = screen.getHeight();
	width = screen.getWidth();

	zoneTopLeft = ballZone.getTopLeft();
	zoneTopRight = ballZone.getTopRight();
	zoneBotLeft = ballZone.getBottomLeft();
	zoneBotRight = ballZone.getBottomRight();

	this->playerHeight = playerHeight;
	this->playerWidth = playerWidth;

	playerTimeline = ofxCinderTimeline::Timeline::create();

	playerTimer = 0;
	if (position == PlayerPosition::TOP)
	{
		playerPoint1.set(0, playerHeight*0.5);
		playerPoint2.set(width, playerPoint1.y);

		playerZonePoint1.set(zoneTopLeft.x, zoneTopLeft.y);
		playerZonePoint2.set(zoneTopRight.x, zoneTopRight.y);

		//this->position.x = playerPoint2.x / 2;
		this->position.x =  ofRandom(playerPoint2.x);
		this->position.y = playerPoint2.y;
		playerColor = ofColor(255, 0, 0);
	}
	else
	{
		playerPoint1.set(0, height - playerHeight*.5);
		playerPoint2.set(width, playerPoint1.y);

		playerZonePoint1.set(zoneBotLeft.x, zoneBotLeft.y);
		playerZonePoint2.set(zoneBotRight.x, zoneBotRight.y);

		this->position.x = playerPoint2.x / 2;
		this->position.y = (playerPoint2.y);
		playerColor = ofColor(0, 0, 255);
	}
}

MondrianPingPong::Player::~Player()
{

}

void MondrianPingPong::Player::passiveUpdate()
{
	playerTimer += 0.01;
	float playerTempVecx = ofNoise(playerTimer, 50, 200, passiveMovWidth) * passiveSpeed - passiveSpeed*.5;
	position.x = position.x + playerTempVecx;

	position = checkForBorders(position);
	playerTimeline->stepTo(ofGetElapsedTimef());
}

ofPoint MondrianPingPong::Player::checkForBorders(const ofPoint& position)
{
	ofPoint localposition = position;

	if (position.x <= playerWidth*0.5)
	{
		localposition.x = playerWidth*0.5;
	}
	else if (position.x >= (width - playerWidth*0.5))
	{
		localposition.x = (width - playerWidth*0.5);
	}

	return localposition;
}

void MondrianPingPong::Player::activeUpdate(const ofPoint& ballPos, const ofPoint& ballVec)
{
	switch (state)
	{
		case PlayerState::WANDERING:
			passiveUpdate();
			break;

		case PlayerState::STARTCATCH:
			setupAnimation(finalePosition);
			position.x = animPosx;
			position = checkForBorders(position);
			state = PlayerState::CATCHING;
			break;

		case PlayerState::CATCHING:
			position.x = animPosx;
			position = checkForBorders(position);
			break;
	}
	playerTimeline->stepTo(ofGetElapsedTimef());
}

void MondrianPingPong::Player::draw()
{

}

void MondrianPingPong::Player::drawLines()
{

}

void MondrianPingPong::Player::drawBody()
{
	ofPushMatrix();
	{
		ofSetColor(playerColor);
		ofDrawRectangle(position.x - playerWidth*.5,
			position.y - playerHeight*.5,
			playerWidth,
			playerHeight);
	}
	ofPopMatrix();
}

void MondrianPingPong::Player::setActive()
{
	state = PlayerState::WANDERING;
}

void MondrianPingPong::Player::setFinalePosition(const ofPoint& finPos)
{
	finalePosition = finPos;
}

void MondrianPingPong::Player::setPassive()
{
	state = PlayerState::WANDERING;
}

void MondrianPingPong::Player::setupAnimation(const ofPoint& finalePos)
{
	float animtime = 1.5;
	float startPosition = position.x;
	float endPosition = finalePos.x;
	animPosx = startPosition;
	playerTimeline->apply(&animPosx,
		startPosition,
		endPosition,
		animtime,
		ofxCinderTimeline::EaseOutCubic());

	//cout << endl << "Setup animation from " << startPosition << " to " << endPosition << " in time " << time << endl;
}

MondrianPingPong::Ball::Ball(ofRectangle screen, ofRectangle ballZone)
{
	width = screen.getWidth();
	height = screen.getHeight();
	zoneTopLeft = ballZone.getTopLeft();
	zoneTopRight = ballZone.getTopRight();
	zoneBotLeft = ballZone.getBottomLeft();
	zoneBotRight = ballZone.getBottomRight();

	Pos = 0.5 * ofPoint(width, height);
	//Pos.x = ofRandom(zoneTopLeft.x, zoneTopRight.x);
	//Pos.y = ofRandom(zoneTopLeft.y, zoneTopRight.y);

	float angle = mathTools().random<float>(0.15f, 0.4f)*PI;
	Vec.x = cos(angle)*speed;
	Vec.y = sin(angle)*speed;
	halfWayFlag = false;
}

MondrianPingPong::Ball::~Ball()
{

}

void MondrianPingPong::Ball::update()
{
	movementUpdate();
}

void MondrianPingPong::Ball::draw()
{

}

void MondrianPingPong::Ball::drawLines()
{	

}

void MondrianPingPong::Ball::drawBody()
{
	ofPushMatrix();
	{
		ofDrawRectangle(Pos.x - (ballSize*0.5), Pos.y - (ballSize*0.5), ballSize, ballSize);
		//ofSetColor(ofColor(0, 0, 0,80));
		//ofDrawRectangle(zoneTopLeft.x, zoneTopLeft.y, zoneBotRight.x, zoneBotRight.y);
		//ofDrawLine(cross, Pos);
	}
	ofPopMatrix();
}

void MondrianPingPong::Ball::movementUpdate()
{
	if (Vec.y > 0)
	{//moving down
		if (Pos.y > (height*0.5) && !halfWayFlag)
		{//half way
			halfWayFlag = true;
			//finPos = getFinPoint();
			ofNotifyEvent(halfWay, this);
		}

		if (Pos.y >= zoneBotLeft.y)
		{//hit BOTTOM
			halfWayFlag = false;
			Vec.y = -Vec.y;
			ofNotifyEvent(hitBot, this);
		}
	}
	else
	{//moving up
		if (Pos.y < (height*0.5) && !halfWayFlag)
		{//half way		 
			halfWayFlag = true;
			//finPos = getFinPoint();
			ofNotifyEvent(halfWay, this);
		}
		if (Pos.y <= zoneTopLeft.y)
		{//hit TOP
			halfWayFlag = false;
			Vec.y = -Vec.y;
			ofNotifyEvent(hitTop, this);
		}
	}
	// LEFT <==
	if ((Pos.x - radius) < zoneTopLeft.x)
	{
		ofNotifyEvent(hitLeft, this);
		Vec.x = -Vec.x;
	}
	// RIGHT ==>
	if ((Pos.x + radius) > zoneTopRight.x)
	{
		ofNotifyEvent(hitRight, this);
		Vec.x = -Vec.x;
	}
	Pos += Vec;

}
