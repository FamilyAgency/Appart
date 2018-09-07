#include "Timer.h"
using namespace synapse;

Timer::Timer()
{
	
}

Timer::~Timer()
{

}

void Timer::start(int time)
{
	startTime = ofGetElapsedTimef();//ofGetElapsedTimeMillis();
	endTime = startTime + time;
	isRunning = true; 
	update();
}

void Timer::update()
{
	if (isRunning)
	{
		currentTime = ofGetElapsedTimef();
		if (currentTime >= endTime)
		{
			timeOutEvent.notify(this);
			isRunning = false;
		}
	}
}
