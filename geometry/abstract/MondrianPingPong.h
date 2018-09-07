#pragma once
#include "FrameGeometry.h"
#include "MathTools.h"
#include "ofxCinderTimeline.h"

namespace frames
{
	typedef shared_ptr<class MondrianPingPong> MondrianPingPongPtr;
	typedef shared_ptr<class MondrianPingPongHor> MondrianPingPongHorPtr;

	class MondrianPingPong : public FrameGeometry
	{
	public:
		MondrianPingPong(float width, float height);
		virtual ~MondrianPingPong();

		virtual void setup()	override;
		virtual void GUISetup() override;
		virtual void update()	override;
		virtual void draw()		override;	
		virtual void clean()	override;

	private:
		enum class PlayerPosition
		{
			TOP,
			BOTTOM
		};

		ofxFloatSlider passiveSpeed;
		ofxFloatSlider passiveMovWidth;
		ofxFloatSlider lineWidthgui;

		const float playerWidth = 400;
		const float generalScale = 0.8;
		const float playerHeight = 100;
		const float threshold = 300;
		const float ballSize = 100;
		const float merge = 25;
		const float speed = 5.0;
		float lineWidth = 10.;

		float zoneHorOffset;

		ofColor ballColor;
		ofColor player1Color;
		ofColor player2Color;

		ofPoint zoneTopLeft;
		ofPoint zoneTopRight;
		ofPoint zoneBotLeft;
		ofPoint zoneBotRight;
	
		ofPoint TopLeft;
		ofPoint TopRight;
		ofPoint BotLeft;
		ofPoint BotRight;
		
		ofRectangle screen;
		ofRectangle ballZone;

		friend class Ball;
		friend class Player;

		ofPoint getFinPoint();
		void drawLines(const ofPoint& topLeft, const ofPoint& botRight, float width);
		void startCatching();
		void changeRoles();
		void passiveSpeedListener(float& value);
		void passiveMovWidthListener(float& value);

		virtual ofRectangle setupOrientation();
		virtual void drawSetOrientation();
		virtual float setupZoneHorOffset();
	public:		
		//    ****** PLAYER ******
		class Player
		{
			ofPoint testPoint;
			string testString;

		public:
			Player(enum class PlayerPosition,
				ofRectangle screen,
				ofRectangle ballZone,
				float playerHeight,
				float playerWidth,
				float passiveSpeed,
				float passiveMovWidth);

			virtual ~Player();

			Ball* ball;
			friend class MondrianPingPong;		
			
			enum class PlayerState { WANDERING, STARTCATCH, CATCHING };
			PlayerState state;

			ofPoint checkForBorders(const ofPoint& position);
			ofColor playerColor;
			ofPoint playerPoint1, playerPoint2;
			ofPoint playerZonePoint1, playerZonePoint2;
			ofPoint position;
			ofPoint cathPosition;

			void passiveUpdate();
			void activeUpdate(const ofPoint& ballPos, const ofPoint& ballVec);
			void setActive();
			void setFinalePosition(const ofPoint& finPos);
			void setPassive();
			void draw();
			void drawLines();
			void drawBody();
			void setupAnimation(const ofPoint& finalePos);
			
			
			ofPtr<ofxCinderTimeline::Timeline> playerTimeline;
			ofxCinderTimeline::Anim<float> animPosx;
			float playerTimer;	
			float passiveSpeed;
			float passiveMovWidth;
			
		private:
			//MondrianPingPong* game;
			ofPoint finalePosition;

			float width;
			float height;
			float playerHeight;
			float playerWidth;

			ofPoint zoneTopLeft;
			ofPoint zoneTopRight;
			ofPoint zoneBotLeft;
			ofPoint zoneBotRight;

			bool isCatching = false;
			float passiveVecDir = 1;
		};
		
		//    ******* BALL *******
		class Ball
		{
			friend class MondrianPingPong;
			friend class Player;

		public:
			Ball(ofRectangle screen, ofRectangle ballZone);
			virtual ~Ball();

			ofPoint Pos;
			ofPoint Vec;
			ofPoint finPos;
			ofEvent<void> halfWay;
			ofEvent<void> hitBot;
			ofEvent<void> hitTop;
			ofEvent<void> hitLeft;
			ofEvent<void> hitRight;


			void update();
			void draw();
			void drawLines();
			void drawBody();
		private:
			const float searchForward = 600;
			const float radius = 50;
			const float ballSize = 100;
			const float speed = 5;

			//MondrianPingPong* game;

			float width;
			float height;
			bool halfWayFlag;
			ofPoint cross;
					
			ofPoint zoneTopLeft;
			ofPoint zoneTopRight;
			ofPoint zoneBotLeft;
			ofPoint zoneBotRight;

			void movementUpdate();					
			//ofPoint getFinPoint();
		};

		//    ******* ELSE *******
		ofPtr<Ball> ball;
		ofPtr<Player> playerTop, playerBot, activePlayer, passivePlayer, tempPlayer;
	};	

	class MondrianPingPongHor : public MondrianPingPong
	{
	public:
		MondrianPingPongHor(float width, float height);
		virtual ~MondrianPingPongHor();

		ofRectangle setupOrientation();
		float setupZoneHorOffset();
		void drawSetOrientation();
	protected:
		ofRectangle screen;
	};
}