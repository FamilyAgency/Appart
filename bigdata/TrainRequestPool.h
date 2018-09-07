#pragma once
#include "TrainRequest.h"
#include "../config/Config.h"

namespace frames
{
	typedef std::shared_ptr<TrainRequest> TrainRequestShared;

	class TrainRequestPool
	{
	public:
		TrainRequestPool();
		~TrainRequestPool();
		void setup(const string& date, TrainRequest::TransportDirections transportDirection, TrainRequest::TransportTypes transportType, synapse::ConfigPtr config);
		
		ofEvent<vector<TrainRequest::Train>> onScheduleComplete;
		ofEvent<void> onBadRequest;
		ofEvent<void> onNoInternet;
		ofEvent<void> onBadResponse;
		vector<TrainRequest::Train> getSchedule() const;
	private:
		//TrainRequest dataTrainRequest;
		vector<TrainRequest::Train> schedule;
		string date;
		TrainRequest::TransportDirections transportDirection;
		TrainRequest::TransportTypes transportType;

		void scheduleAddedHandler(vector<TrainRequest::Train>&);
		void badRequestHandler();
		void noInternetHandler();
		void badResponseHandler();

		TrainRequestShared trainRequestArrival;
		TrainRequestShared trainRequestDeparture;
		vector<TrainRequestShared> requests;


	};
}
