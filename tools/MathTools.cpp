#include "MathTools.h"
using namespace frames;

MathTools& MathTools::getInstance()
{
	static MathTools tools;
	return tools;
}

//bool MathTools::getCrossPoint(const ofPoint& p1, const ofPoint& p2, const ofPoint& p3, const ofPoint& p4, ofPoint &cross)
//{
//	ofPoint dp1 = p2 - p1;
//	ofPoint dp2 = p4 - p3;
//
//	cross.x = dp1.y * dp2.x - dp2.y * dp1.x;
//
//	if (!(cross.x && dp2.x))
//		return false;
//
//	cross.y = p3.x * p4.y - p3.y * p4.x;
//	cross.x = ((p1.x * p2.y - p1.y * p2.x) * dp2.x - cross.y * dp1.x) / cross.x;
//	cross.y = (dp2.y * cross.x - cross.y) / dp2.x;
//
//	bool intersect = ((p1.x <= cross.x && p2.x >= cross.x) || 
//					 (p2.x <= cross.x && p1.x >= cross.x)) && 
//					 ((p3.x <= cross.x && p4.x >= cross.x) || 
//					 (p4.x <= cross.x && p3.x >= cross.x));
//	return intersect;
//}

//bool MathTools::getCrossPoint(const ofPoint& p1, const ofPoint& p2, const ofPoint& p3, const ofPoint& p4)
//{
//	ofPoint dp1 = p2 - p1;
//	ofPoint dp2 = p4 - p3;
//	ofPoint point;
//	point.x = dp1.y * dp2.x - dp2.y * dp1.x;
//
//	if (!(point.x && dp2.x))
//		return false;
//
//	point.y = p3.x * p4.y - p3.y * p4.x;
//	point.x = ((p1.x * p2.y - p1.y * p2.x) * dp2.x - point.y * dp1.x) / point.x;
//	point.y = (dp2.y * point.x - point.y) / dp2.x;
//
//	bool intersect = ((p1.x <= point.x && p2.x >= point.x) ||
//		(p2.x <= point.x && p1.x >= point.x)) &&
//		((p3.x <= point.x && p4.x >= point.x) ||
//		(p4.x <= point.x && p3.x >= point.x));
//	return intersect;
//}
ofPoint MathTools::getCrossPoint2(const ofPoint& p1, const ofPoint& p2, const ofPoint& p3, const ofPoint& p4)
{
	// bad function. only works in case line2 is ver or hor
	ofPoint crossPoint;
	if (p3.x == p4.x)   // vert
	{
		crossPoint.y = p1.y + ((p2.y - p1.y) * (p3.x - p1.x)) / (p2.x - p1.x);
		if (crossPoint.y > std::max(p3.y, p4.y) || crossPoint.y < std::min(p3.y, p4.y) || crossPoint.y > std::max(p1.y, p2.y) || crossPoint.y < std::min(p1.y, p2.y))
		{
			ofPoint cross;
			cross.set(0, 0);
			return cross;
		}
		else
		{
			ofPoint cross;
			cross.set(p3.x, crossPoint.y);
			return cross;
		}
	}
	else            // horizontal
	{
		crossPoint.x = p1.x + ((p2.x - p1.x) * (p3.y - p1.y)) / (p2.y - p1.y);
		if (crossPoint.x > std::max(p3.x, p4.x) || crossPoint.x < std::min(p3.x, p4.x) || crossPoint.x > std::max(p1.x, p2.x) || crossPoint.x < std::min(p1.x, p2.x))
		{
			ofPoint cross;
			cross.set(0, 0);
			return cross;
		}
		else//crossP.x, p3.y
		{
			ofPoint cross;
			cross.set(crossPoint.x, p3.y);
			return cross;
		}	
	}
}
// TODO: const&
float area(ofPoint p1, ofPoint p2, ofPoint p3)
{
	return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
}

bool intersect_1(float a, float b, float c, float d)
{
	if (a > b)
	{
		swap(a, b);
	}
	if (c > d)
	{
		swap(c, d);
	}
	return std::max(a, c) <= std::min(b, d);
}

bool MathTools::isCrossing(const ofPoint p1, const ofPoint p2, const ofPoint p3, const ofPoint p4)
{
	return intersect_1(p1.x, p2.x, p3.x, p4.x)
		&& intersect_1(p1.y, p2.y, p3.y, p4.y)
		&& area(p1, p2, p3) * area(p1, p2, p4) <= 0
		&& area(p3, p4, p1) * area(p3, p4, p2) <= 0;
}

ofColor MathTools::randomColor()
{
	return ofColor((int)ofRandom(255), (int)ofRandom(255), (int)ofRandom(255));
}

ofVec3f MathTools::randomColorNormalize()
{
	return ofVec3f(ofRandom(1.), ofRandom(1.), ofRandom(1.));
}

ofVec3f MathTools::normalizeColor(const ofColor& color)
{
	return ofVec3f(color.r/255., color.g/255., color.b/255.);
}

//bool frames::MathTools::isParallel(const ofPoint& p1, const ofPoint& p2, const ofPoint& p3, const ofPoint& p4)
//{
//	ofVec2f vec1;
//	ofVec2f vec2;
//
//	vec1.set(p1.x - p2.x, p1.y - p2.y);
//	vec2.set(p3.x - p3.x, p4.y - p4.y);
//
//	return vec1.isAligned(vec2);
//}
