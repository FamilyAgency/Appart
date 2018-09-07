#pragma once
#include "FrameGeometry.h"
#include <time.h>
#include "ofxCinderTimeline.h"

#include "ofxXmlSettings.h"
#include "ofxFontStash.h"
#include "bigdata\TrainRequestPool.h"
#include "../config/Config.h"

#include <algorithm>


namespace frames
{
	// short name for smart pointer
	typedef shared_ptr<class FamilyTrain> FamilyTrainPtr;

	class FamilyTrain : public FrameGeometry
	{
	public:
		FamilyTrain(float width, float height, synapse::ConfigPtr config);
		virtual ~FamilyTrain();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;
		virtual void forceUpdate() override;
		virtual void draw() override;

	protected:

	private:
		ofxFontStash roboto;
		ofxFontStash robotoLight;
		ofxFontStash robotoBold;

		const int minCooldown = 15;
		const int rndCooldown = 15;
		const TrainRequest::TransportDirections directionParameter = TrainRequest::TransportDirections::ALL;
		const TrainRequest::TransportTypes transportTypeParameter = TrainRequest::TransportTypes::TRAIN;

		void initNextTrain();
		void drawCurrentTime();
		void drawCurrentTrain();
		//void checkTrainStatus();
		void scheduleCompleteHandler(vector<TrainRequest::Train>&);

		void badRequestHandler();
		void badResponseHandler();
		void noInternetHandler();
		

		TrainRequestPool trainRequestPool;
		TrainRequest::Train* nextTrain;
		//TrainRequest::Train 
		vector<TrainRequest::Train> schedule;
		time_t currentTime;

		//int closest(std::vector<int> const & vec, int value);
		void sortSchedule();
		void findNextTrain();
		void setNextTrain();
		bool compareTrainByTime(const TrainRequest::Train& train1, const TrainRequest::Train& train2);

		/*time_t convertToSeconds(const string& timeTo);
		string convertToString(time_t timeTo);
		string convertToString(time_t timeTo, const string& format);*/

		void addTrain(const TrainRequest::Train& train);
		void addTrain(const int& amount);
		/*void addTrain();
		void addTrain(time_t time);*/
		
		void initialRequest();
		void nextDayRequest();
		void scheduleStatus();

		std::shared_ptr<ofxCinderTimeline::Timeline> trainTimeline;
		ofxCinderTimeline::Anim<float> waitFrac;
		void showTrainTimeline();
		void trainIsComing();
		void nextTrainSetup();

		time_t convertToSeconds(const string& timeTo);
		string convertToString(time_t timeTo);
		string convertToString(time_t timeTo, const string& format);

		struct isEarlier
		{
			bool operator()(const TrainRequest::Train& train1, TrainRequest::Train& train2) const
			{
				return train1.timeAbs < train2.timeAbs;
			}
		};

		synapse::ConfigPtr config;
	};
}
