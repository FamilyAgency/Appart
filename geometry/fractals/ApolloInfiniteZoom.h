#pragma once
#pragma once
#include "FrameGeometry.h"

namespace frames
{
	// short name for smart pointer
	typedef shared_ptr<class ApolloInfiniteZoom> ApolloInfiniteZoomPtr;

	class ApolloInfiniteZoom : public FrameGeometry
	{
	public:
		ApolloInfiniteZoom(float width, float height);
		virtual ~ApolloInfiniteZoom();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;
		virtual void draw() override;

	private:
		const int num = 200;
		float rate = 0.05;

		vector<ofPoint> locatios;
		vector<float> radius;	

		void getCircle(int i);
		float mag(float x, float y);
	};
}
