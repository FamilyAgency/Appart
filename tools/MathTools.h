#pragma once
#include <random>
#include "ofMain.h"

namespace frames
{
	class MathTools
	{
		public:
			static MathTools& getInstance();

			template<class T>				
			T random(T min, T max)
			{
				return ((T(rand()) / T(RAND_MAX)) * (max - min)) + min;		
			}

			template<class T>
			T random(T max)
			{
				return ((T(rand()) / T(RAND_MAX)) * (max));
			}

			template<typename T>
			float constrain(T value, T min, T max)
			{
				return value > max ? max : (value < min ? min : value);
			}

			//bool getCrossPoint(const ofPoint& p1, const ofPoint& p2, const ofPoint& p3, const ofPoint& p4, ofPoint& cross);
			//bool getCrossPoint(const ofPoint& p1, const ofPoint& p2, const ofPoint& p3, const ofPoint& p4);
			//bool isParallel(const ofPoint& p1, const ofPoint& p2, const ofPoint& p3, const ofPoint& p4);
			
			
			bool isCrossing(		const ofPoint p1, const ofPoint p2, const ofPoint p3, const ofPoint p4);
			ofPoint getCrossPoint2(	const ofPoint& p1, const ofPoint& p2, const ofPoint& p3, const ofPoint& p4);
			ofColor randomColor();
			ofVec3f randomColorNormalize();
			ofVec3f normalizeColor(const ofColor& color);

			
		private:
			ofPoint point;		
	};

	inline MathTools& mathTools()
	{
		return MathTools::getInstance();
	};
}