#pragma once
#include "FrameGeometry.h"
#include "ofxCinderTimeline.h"
#include "FrameShader.h"

#include <vector>

namespace frames
{
	typedef shared_ptr<class GameOfLife> GameOfLifePtr;

	struct cell
	{
		bool currState;
		bool nextState;
		int activeNeighbors;
		ofColor color;
		ofColor nextColor;
	};

	enum type { STILL, INFIN, DEAD, SHIP };
	//enum drawType { CIRCLE, BOX };
	//drawType drawtyper = drawType::CIRCLE;

	struct Pattern
	{
		int w;
		int h;
		string data;
		type type;
	};	

	class GameOfLife : public FrameGeometry
	{
	public:
		GameOfLife(float width, float height);
		virtual ~GameOfLife();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;
		virtual void draw() override;
		virtual void clean() override;

		ofColor bgColor;
		//ofxColorSlider lifeColor;
		//ofxColorSlider background;
		ofColor lifeColor;
		ofColor background;
		ofxToggle highlightSwitch;
		ofColor noLifeColor;
		ofxColorSlider maskColor;
		ofxFloatSlider outputTrans;

		void tick();
		void clear();
		void create();
		
		void initSpawn(int times);
		void gliderGun(vector<vector<cell>> grid, int startPosX, int startPosY);
		void pufferTrain(vector<vector<cell>> grid, int startPosX, int startPosY);
		void setRandom(vector<vector<cell>> grid);
		void placeRandom(vector<vector<cell>> grid);

	private:
		enum class Renew {SINGLE, FULL, INFIN, PATTERNS};
		Renew renew;

		const int renewFreq = 25;
		const int maxTick = 40;
		const float cellSize = 20;
		const int initSpawnCount = 50;
		const int circlePadding = 2;
		bool isCircle = true;

		ofFbo currentFrame;
		ofFbo pastFrame;
		ofFbo mask;
		vector<Pattern> patternBase;

		vector<vector<cell>> grid;

		int rows, cols;
		int setupCount = 0;
		vector<ofColor> bgColors;
		vector<ofColor> mainColors;
		float cellWidth, cellHeight;
		bool highlight;

		ofPtr<class PixelAlphaShader> pixelAlphaShader;

		int getNumActiveNeighbors(const int& colIndex, const int& rowIndex);
		int currState(const int& colIndex, const int& rowIndex);
		void frameToPast();
		void frameToCurrent();
		void setupColors();
		void setColors();
		void drawPattern(const Pattern& pattern);
		void drawPattern(const Pattern& pattern, int startX, int startY);
		void makeNextStateCurrent();

		vector<Pattern> getPatterns();
		Pattern selectPattern(vector<Pattern> patternBase);	
		shared_ptr<ofxCinderTimeline::Timeline> lerpTimeline;
		ofxCinderTimeline::Anim<float> colorLerp;
		void colorLerpSetup();
		void colorLerpFin();		
	};

	class Patterns
	{
	public:
		Patterns();
		virtual ~Patterns();
		
		vector<Pattern> patternBase;
		vector<Pattern> getPatternBase();
		
	private:		
	};

	class PixelAlphaShader : public FrameShader
	{
	public:
		PixelAlphaShader(float width, float height);		
		virtual void  update()  override;
		virtual void  setup()  override;
		void setupTextures(ofTexture& tex1, ofTexture& tex2, ofTexture& tex3);

	protected:
		virtual string getFragSrc();

	};
}
