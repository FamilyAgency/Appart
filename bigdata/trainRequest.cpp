#include "trainRequest.h"
#include "../config/config.h"

using namespace frames;

TrainRequest::TrainRequest(TransportDirections transportDirectionPar):
	page(1), 
	status(TrainRequestStatus::EMPTY),
	transportDirection(transportDirectionPar)
{
	typeStringAlias.PAIRTRAIN(TransportTypes::ALL, "&transport_types=all");
	typeStringAlias.PAIRTRAIN(TransportTypes::SUBURBAN, "&transport_types=suburban");
	typeStringAlias.PAIRTRAIN(TransportTypes::TRAIN, "&transport_types=train");

	directionsSringAlias.PAIRTDIR(TransportDirections::ALL, "&event=all");
	directionsSringAlias.PAIRTDIR(TransportDirections::ARRIVAL, "&event=arrival");
	directionsSringAlias.PAIRTDIR(TransportDirections::DEPARTURE, "&event=departure");

	directionsSringCheck.PAIRTDIR(TransportDirections::ALL, "all");
	directionsSringCheck.PAIRTDIR(TransportDirections::ARRIVAL, "arrival");
	directionsSringCheck.PAIRTDIR(TransportDirections::DEPARTURE, "departure");
}

TrainRequest::~TrainRequest()
{

}
//const todo
void TrainRequest::setup(const string& datePar, const TransportTypes& transportTypePar,const synapse::Config::YandexData& yandexData)
{
	this->yandexData = yandexData;

	date = datePar;	
	transportType = transportTypePar;
	status = TrainRequestStatus::PROGRESS;

	ofRegisterURLNotification(this);
	//requestID = ofLoadURLAsync(getUrlForLoad(), "name " + to_string(ofGetElapsedTimeMillis()));
	urlResponse(ofLoadURL(getUrlForLoad()));
}

//local
void TrainRequest::setup(const string& datePar, const TransportTypes& transportTypePar)
{
	date = datePar;
	transportType = transportTypePar;
	status = TrainRequestStatus::PROGRESS;

	ofRegisterURLNotification(this);
	//requestID = ofLoadURLAsync(getUrlForLoad(), "name " + to_string(ofGetElapsedTimeMillis()));
	urlResponse(ofLoadURL(getUrlForLoad()));
}

string TrainRequest::TrainRequest::getUrlForLoad()
{
	string url;	
	url.append(yandexData.protocol);
	url.append("://");
	url.append(yandexData.host);
	url.append("?");
	url.append(yandexData.apiKey);
	url.append("&");
	url.append(yandexData.format);
	url.append("&");
	url.append(yandexData.station);
	url.append("&");
	url.append(yandexData.lang);
	url.append("&");
	url.append(yandexData.date + date);

	url.append(direction());
	url.append(type());
	url.append(pagination());
	ofLogNotice("url cheching: ", url);
	return url;
}

string TrainRequest::direction()// const
{
	return directionsSringAlias[transportDirection];
}

string TrainRequest::type()// const
{
	return typeStringAlias[transportType];
}

string TrainRequest::pagination()
{
	return "&page=" + to_string(page);
}

void TrainRequest::urlResponse(ofHttpResponse& httpResponse)
{
	ofUnregisterURLNotification(this);

	//auto name = httpResponse.request.name;
	//try {
	//}
	//catch (std::runtime_error)
	//{
	//	ofLogError("There is error in Config file");
	//}
	if (httpResponse.status == (int)RequestStatus::SUCCESS)
	{
		xml.loadFromBuffer(httpResponse.data.getText());
		
		if (xml.getNumTags("response") > 0)
		{
			xml.pushTag("response");
			{
				ofLogNotice(directionsSringCheck[transportDirection], string(xml.getValue("event", "") + to_string(requestID)));
				{
					for (int i = 0; i < xml.getNumTags("schedule") - 1; i++)
					{
						Train tempTrain;
						xml.pushTag("schedule", i);
						{
							time_t timeAbs = 0;
							if (!xml.getValue("arrival", "").empty())
							{
								timeAbs = convertToSeconds(xml.getValue("arrival", "")) - secondsOffset;
							}
							else if (!xml.getValue("departure", "").empty())
							{
								timeAbs = convertToSeconds(xml.getValue("departure", "")) + secondsOffset;
							}
							else
							{
								// throw ParsingError
							}
							tempTrain.timeAbs = timeAbs;
							tempTrain.timeStr = convertToString(tempTrain.timeAbs);

							tempTrain.title = xml.getValue("thread:title", "");
							tempTrain.shortTitle = xml.getValue("thread:short_title", "");

							tempTrain.direction = transportDirection;

							tempTrain.timeStatus = TrainTimeStatus::FUTURE;
						}
						xml.popTag();
						schedule.push_back(tempTrain);
					}


					if (istringstream(xml.getValue("pagination:has_next", "")))
					{

						//ofNotifyEvent(onScheduleChange, schedule);

						if (xml.getValue("pagination:has_next", "") == "True")
						{
							status = TrainRequestStatus::PROGRESS;
							page++;
							//ofNotifyEvent(onScheduleChange, schedule);
							ofLogNotice(to_string(page), xml.getValue("pagination:has_next", ""));
							setup(date, transportType);
							Train train;
						}
						else
						{
							status = TrainRequestStatus::COMPLETE;
							ofNotifyEvent(onScheduleComplete, schedule);
							ofLogNotice("size of schedule from request ", to_string(schedule.size()));
						}
					}
				}
				//parse(xml);
				//ofLogNotice(to_string(xml.getNumTags("schedule")));

				xml.popTag();
			}
		}
		else
		{
			ofNotifyEvent(onBadResponse);
		}
		
		//scheduleStatus();
	}
	else
	{
		//requestStatus = RequestStatus::SUCCESS
		switch (httpResponse.status)
		{
		case (int)RequestStatus::BAD_REQUEST:
			ofNotifyEvent(onBadRequest);
			//ofLogError("BAD REQUEST", (httpResponse.error));
			break;
		case (int)RequestStatus::NO_INTERNET:
			ofNotifyEvent(onNoInternet);
			//ofLogError("NO INTERNET", (httpResponse.error));
			break;
		default:
			ofNotifyEvent(onNoInternet);
			//ofLogError("SOMETHING WRONG", (httpResponse.error));
			break;
		}

	}
}

vector<TrainRequest::Train> TrainRequest::getSchedule() const
{
	return schedule;
}

time_t TrainRequest::convertToSeconds(const string& timeTo)
{
	struct tm tm;
	istringstream ss(timeTo);
	ss >> get_time(&tm, "%Y-%m-%d %H:%M:%S");
	time_t time = mktime(&tm);
	return time;
}

string TrainRequest::convertToString(time_t timeTo)
{
	time_t rawtime = timeTo;
	struct tm * timeinfo;
	char buffer[80];
	timeinfo = localtime(&rawtime);
	strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
	return buffer;
}

string TrainRequest::convertToString(time_t timeTo, const string& format)
{
	time_t rawtime = timeTo;
	struct tm * timeinfo;
	char buffer[80];
	timeinfo = localtime(&rawtime);
	strftime(buffer, 80, format.c_str(), timeinfo);
	return buffer;
}

void TrainRequest::scheduleStatus()
{
	//cout << "=====================================scheduleStatus=====================================" << endl;
	for (size_t i = 0; i < schedule.size(); i++)
	{
		cout << to_string(i) << ":\t" << schedule[i].shortTitle << endl << "\t" << schedule[i].timeStr << endl;
	}
}

void TrainRequest::scheduleClear()
{
	schedule.clear();
}

bool TrainRequest::isComplete()
{
	return status == TrainRequestStatus::COMPLETE;	
}








	//else// if (istringstream(xml.getValue("pagination:has_next", "")))
	//{
	//	string nextDate = convertToString(currentTime + 60 * 60 * 24 - 1, "%Y-%m-%d");
	//	url = getTrainURL(nextDate);
	//}


