#pragma once
#include "ofxXmlSettings.h"
#include "../config/Config.h"
//#include "FamilyTrain.h"

#define PAIRTRAIN(type, str) insert(make_pair<TransportTypes, std::string>((type), (str)));
#define PAIRTDIR(dir, str) insert(make_pair<TransportDirections, std::string>((dir), (str)));

namespace frames
{
	class TrainRequest
	{
	public:
		static const int STATUS_SUCCESS = 200;

		enum class RequestStatus
		{
			NO_INTERNET = -1,
			SUCCESS = 200,
			BAD_REQUEST = 400
		};

		enum class TransportTypes
		{
			ALL,
			SUBURBAN,
			TRAIN
		};
		enum class TransportDirections
		{
			ALL,
			ARRIVAL,
			DEPARTURE
		};
		enum TrainRequestStatus
		{
			EMPTY,
			COMPLETE,
			PROGRESS,
		};
		enum TrainTimeStatus
		{
			PAST,
			NOW,
			FUTURE
		};
		struct Train
		{
			string title;
			string shortTitle;
			string number;
			time_t timeAbs;
			string timeStr;
			TransportDirections direction;
			TrainTimeStatus timeStatus;
		};

		TrainRequest(TransportDirections transportDirection);
		~TrainRequest();
		void setup(const string& datePar, const TransportTypes& transportTypePar, const synapse::Config::YandexData& yandexData);
		void setup(const string& datePar, const TransportTypes& transportTypePar);
		vector<Train> getSchedule() const;
		ofEvent<vector<Train>> onScheduleChange;
		ofEvent<vector<Train>> onScheduleComplete;
		ofEvent<void> onBadRequest;
		ofEvent<void> onNoInternet;
		ofEvent<void> onBadResponse;

		//ofEvent
		void urlResponse(ofHttpResponse &httpResponse);
		void scheduleStatus();
		void scheduleClear();
		bool isComplete();

		string direction();
		string type();
		string pagination();
		string yandexApiKey;		

		time_t convertToSeconds(const string& timeTo);
		string convertToString(time_t timeTo);
		string convertToString(time_t timeTo, const string& format);
		
	private:
		std::map<TransportTypes, std::string> typeStringAlias;
		std::map<TransportDirections, std::string> directionsSringAlias;
		std::map<TransportDirections, std::string> directionsSringCheck;

		synapse::Config::YandexData yandexData;	
		const time_t secondsOffset = 100;
		const bool needNextPage = true;

		bool hasNext;
		int page;
		string date;
		//string checkEvent;
		TransportDirections transportDirection;
		TransportTypes transportType;
		vector<Train> schedule;
		TrainRequestStatus status;

		RequestStatus requestStatus;

		string getUrlForLoad();
		int requestID;
		
		ofxXmlSettings xml;
	};
}
