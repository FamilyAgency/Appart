#pragma once
#include "ofMain.h"

namespace synapse
{
	class Timer
	{
	public:
		Timer();
		~Timer();
		void start(int time);
		ofEvent<void> timeOutEvent;
		void update();

	private:
		float startTime;
		float endTime;
		float currentTime;
		bool isRunning;
	};
}