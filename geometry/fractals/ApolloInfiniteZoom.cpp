#include "ApolloInfiniteZoom.h"

using namespace frames;

ApolloInfiniteZoom::ApolloInfiniteZoom(float width, float height) : FrameGeometry(width, height)
{
	spoutChannelName = "Geometry Art";
	name = "Apollo Infinite Zoom";
}

ApolloInfiniteZoom::~ApolloInfiniteZoom()
{

}

void ApolloInfiniteZoom::setup()
{
	// setup init values
	
	for (int i = 0; i<num; i++) 
	{
		locatios.push_back(ofPoint());
		radius.push_back(0.0);
	}

	for (int i = 0; i<num; i++) 
	{		
		getCircle(i);
	}

	//ofEnableSmoothing();
}

void ApolloInfiniteZoom::getCircle(int i)
{
	int attempt = 0;
	bool solution = false;
	const int attemptMax = 1000;

	while (attempt < attemptMax && solution == false)
	{
		attempt++;

		float newRad = ofRandom(0.f, 0.2f);
		float distance = ofRandom(0.f, 0.4f);

		if (distance > newRad) 
		{
			float angle = ofRandom(0.f, TWO_PI);

			ofPoint newLoc(distance*cos(angle), distance*sin(angle));
			bool intersection = false;

			for (int j = 0; j < num; j++)
			{
				if (i != j) 
				{
					ofPoint delta = newLoc - locatios[j];		
					if (mag(delta.x , delta.y) < newRad + radius[j]) 
					{
						intersection = true;
					}
				}
			}

			if (intersection == false) 
			{
				solution = true;
				locatios[i] = newLoc;
				radius[i] = newRad;
			}
		}
	}	
}

void ApolloInfiniteZoom::GUISetup()
{
#ifndef release
	FrameGeometry::GUISetup();

	//code for gui setup below
	//...
#endif
}

void ApolloInfiniteZoom::draw()
{
	
	// draw in FBO
	fboArt.begin();
	ofClear(255, 255, 255, 0);

	// code here

	rate = (1 - 1.f*ofGetMouseY() / height)*0.1 + 1;
	
	for (int i = 0; i<num; i++)
	{		
		ofSetColor(ofColor(1., 0., 0.));	
		ofDrawEllipse(width / 2. + locatios[i].x, height / 2. + locatios[i].y, 2. * radius[i], 2. * radius[i]);

		float angle = atan2(locatios[i].y, locatios[i].x) +(ofGetMouseX() - width / 2.)*PI / 100000;
		float distance = mag(locatios[i].y, locatios[i].x)*rate;
		
		locatios[i] = ofPoint(distance*cos(angle), distance*sin(angle));		
		radius[i] *= rate;
	
		if (mag(locatios[i].x, locatios[i].y) - radius[i] > mag(width / 2., height / 2.))
		{
			getCircle(i);
		}
	}

	fboArt.end();

	//FBO draws in parent
	FrameGeometry::draw();
}

float ApolloInfiniteZoom::mag(float x, float y)
{
	return sqrt(x*x + y*y);
}

void ApolloInfiniteZoom::update()
{
	// update algorithm here
}
