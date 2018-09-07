#include "TrainRequestPool.h"

using namespace frames;

TrainRequestPool::TrainRequestPool()
{
	schedule.clear();
}

TrainRequestPool::~TrainRequestPool()
{

}

void TrainRequestPool::setup(const string& date, TrainRequest::TransportDirections transportDirection, TrainRequest::TransportTypes transportType, synapse::ConfigPtr config)
{
	this->date = date;
	this->transportDirection = transportDirection;
	this->transportType = transportType;

	schedule.clear();
	requests.clear();

	switch (transportDirection)
	{
	case TrainRequest::TransportDirections::ALL:
		requests.push_back(TrainRequestShared(new TrainRequest(TrainRequest::TransportDirections::DEPARTURE)));
		requests.push_back(TrainRequestShared(new TrainRequest(TrainRequest::TransportDirections::ARRIVAL)));
		break;

	case TrainRequest::TransportDirections::ARRIVAL:
		requests.push_back(TrainRequestShared(new TrainRequest(TrainRequest::TransportDirections::ARRIVAL)));
		break;
	case TrainRequest::TransportDirections::DEPARTURE:
		requests.push_back(TrainRequestShared(new TrainRequest(TrainRequest::TransportDirections::DEPARTURE)));
		break;
	}

	for (size_t i = 0; i < requests.size(); i++)
	{
		ofAddListener(requests[i]->onScheduleComplete,	this, &TrainRequestPool::scheduleAddedHandler);
		ofAddListener(requests[i]->onBadRequest,		this, &TrainRequestPool::badRequestHandler);
		ofAddListener(requests[i]->onNoInternet,		this, &TrainRequestPool::noInternetHandler);
		ofAddListener(requests[i]->onBadResponse,		this, &TrainRequestPool::badResponseHandler);
		requests[i]->setup(date, transportType, config->getYandexdata());
		//trainRequestDeparture->setup(date, transportType);
	}
}

void TrainRequestPool::scheduleAddedHandler(vector<TrainRequest::Train>& data)
{
	schedule.insert(schedule.end(), data.begin(), data.end());
	//if
	bool isAllComplete = true;
	for (size_t i = 0; i <  requests.size(); i++)
	{
		if (!requests[i]->isComplete())
		{
			isAllComplete = false;
		}
	}	
	if (isAllComplete)
	{
		ofNotifyEvent(onScheduleComplete, schedule);
	}
}

void TrainRequestPool::badRequestHandler()
{
	ofNotifyEvent(onBadRequest);
}

void TrainRequestPool::noInternetHandler()
{
	ofNotifyEvent(onNoInternet);
}

vector<TrainRequest::Train> TrainRequestPool::getSchedule() const
{
	return schedule;
}

void TrainRequestPool::badResponseHandler()
{
	ofNotifyEvent(onBadResponse);
}
