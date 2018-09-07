#include "FamilyTrain.h"
#include "../config/Config.h"

using namespace frames;

FamilyTrain::FamilyTrain(float width, float height, synapse::ConfigPtr config) 
	: FrameGeometry(width, height)
{
	this->config = config;
	spoutChannelName = "Geometry Art";
	name = "Family Train";
	needForceUpdateFeature = true;
	//setlocale(LC_ALL, "Russian");

	roboto.setup("fonts/roboto.ttf", 1.0, 1024, false, 8, 1.5);
	robotoLight.setup("fonts/robotoLight.ttf", 1.0, 1024, false, 8, 1.5);
	robotoBold.setup("fonts/robotoBold.ttf", 1.0, 1024, false, 8, 1.5);
	roboto.setCharacterSpacing(0);
	robotoLight.setCharacterSpacing(0);
	robotoBold.setCharacterSpacing(0);
	
	trainTimeline = ofxCinderTimeline::Timeline::create();
	trainTimeline->stepTo(ofGetElapsedTimef());

	ofSetLogLevel(OF_LOG_VERBOSE);
	time(&currentTime);

	
	initNextTrain();
	
	initialRequest();
}

FamilyTrain::~FamilyTrain()
{

}

void FamilyTrain::setup()
{

}

void FamilyTrain::GUISetup()
{
#ifndef release
	FrameGeometry::GUISetup();
	//code for gui setup below
	//...
#endif
}

void FamilyTrain::draw()
{
	// draw in FBO
	fboArt.begin();
	ofClear(255, 255, 255, 0);
	{
		ofSetColor(0, 0, 0);
		ofBackground(255, 255, 255);
		for (size_t i = 0; i < schedule.size(); i++)
		{
			if (schedule[i].timeStatus == TrainRequest::TrainTimeStatus::FUTURE)
			{
				ofSetColor(255, 0, 0);
			}
			else
			{
				ofSetColor(0, 0, 255);
			}
			if (schedule[i].direction == TrainRequest::TransportDirections::ARRIVAL)
			{
				roboto.draw(schedule[i].title + " -- " + schedule[i].timeStr + " ARRIVAL", 12, 30, 20 + 10 * i);
			}
			else
			{
				roboto.draw(schedule[i].title + " -- " + schedule[i].timeStr + " DEPARTURE	", 12, 30, 20 + 10 * i);
			}
		}
		ofSetColor(0, 0, 0);
		drawCurrentTime();
		drawCurrentTrain();
	}
	fboArt.end();

	//FBO draws in parent
	FrameGeometry::draw();
}

void FamilyTrain::update()
{
	// update algorithm here
}

void FamilyTrain::forceUpdate()
{
	time(&currentTime);
	trainTimeline->stepTo(ofGetElapsedTimef());
}

void FamilyTrain::initNextTrain()
{
	TrainRequest::Train tempTrain;
	tempTrain.shortTitle = "";
	tempTrain.timeAbs = 0;
	schedule.push_back(tempTrain);
	//schedule[0]=(tempTrain);

	nextTrain = &schedule[0];
	//schedule[0];
}

void FamilyTrain::initialRequest()
{
	time(&currentTime);
	string todaDate = convertToString(currentTime, "%Y-%m-%d");
	ofAddListener(trainRequestPool.onScheduleComplete, this, &FamilyTrain::scheduleCompleteHandler);
	ofAddListener(trainRequestPool.onBadRequest, this, &FamilyTrain::badRequestHandler);
	ofAddListener(trainRequestPool.onNoInternet, this, &FamilyTrain::noInternetHandler);
	ofAddListener(trainRequestPool.onBadResponse, this, &FamilyTrain::badResponseHandler);
	trainRequestPool.setup(	todaDate, directionParameter, transportTypeParameter, config);
}

void FamilyTrain::nextDayRequest()
{
	time(&currentTime);
	currentTime = currentTime + 60 * 60 * 12;
	string tomorrowDate = convertToString(currentTime, "%Y-%m-%d");
	trainRequestPool.setup(tomorrowDate, directionParameter, transportTypeParameter, config);

}

void FamilyTrain::scheduleCompleteHandler(vector<TrainRequest::Train>& data)
{
	schedule = data;
	//addTrain(5);
	sortSchedule();
	setNextTrain();
}

void FamilyTrain::badRequestHandler()
{
	
}

void FamilyTrain::badResponseHandler()
{
	ofLogError("BAD response!!!!!!!!!!!!!!!!!");
}

void FamilyTrain::noInternetHandler()
{
	//ofLogError("NO internet", "from inside");
}

void FamilyTrain::sortSchedule()
{	
	sort(schedule.begin(), schedule.end(), isEarlier());

	findNextTrain();
	//nextTrain = schedule.begin();
	
}

void FamilyTrain::findNextTrain()
{
	time(&currentTime);
	for (size_t i = 0; i < schedule.size(); i++)
	{
		if (schedule[i].timeAbs >= currentTime)
		{
			schedule[i].timeStatus = TrainRequest::TrainTimeStatus::PAST;
			nextTrain = &schedule[i];
			break;
		}
		else
		{
			schedule[i].timeStatus = TrainRequest::TrainTimeStatus::PAST;
		}
	}
	nextTrainSetup();
}

void FamilyTrain::setNextTrain()
{
	int index = nextTrain - &schedule[0];
	if (index < schedule.size())
	{
		index++;
		nextTrain = &schedule[index];
		nextTrainSetup();
	}
	else
	{
		nextDayRequest();
	}
	
	
}

//void FamilyTrain::checkTrainStatus()
//{
//	vector<int> trainSeconds;
//	trainSeconds.clear();
//	for (size_t i = 0; i < schedule.size(); i++)
//	{
//		trainSeconds.push_back(schedule[i].timeAbs);
//	}
//	sort(trainSeconds.begin(), trainSeconds.end());
//	double closestTime = closest(trainSeconds, currentTime + minCooldown);
//
//	if (closestTime != -1)
//	{
//		for (size_t i = 0; i < schedule.size(); i++)
//		{
//			if (schedule[i].timeAbs == closestTime)
//			{
//				nextTrain = schedule[i];
//			}
//		}
//		nextTrainSetup();
//	}
//	//sortSchedule(schedule);
//}

void FamilyTrain::drawCurrentTime()
{
	time_t timer;
	time(&timer);
	//cout << timer << " -- " << convertToString(timer) << endl;
	string timeText = "date: " + convertToString(timer, "%Y %m %d %H:%M:%S");// + "  ¬рема: " + convertToString(timer, "%H.%M.%S");
	roboto.draw(timeText, 50, 50, 250);
}

void FamilyTrain::drawCurrentTrain()
{
	string nextTrainText = "Train: " + nextTrain->shortTitle + " Ч " + convertToString(nextTrain->timeAbs);
	robotoBold.draw(nextTrainText, 50, 50, 300);
}



//int FamilyTrain::closest(vector<int> const& vec, int value)
//{
//	auto const it = upper_bound(vec.begin(), vec.end(), value);
//	if (it == vec.end())
//	{
//		//TODO out of trains
//		return -1;
//	}
//	return *it;
//}



void FamilyTrain::trainIsComing()
{
	showMeForceNotifyCall();
	cout << "SHOWING TRAIN - " << nextTrain->shortTitle << nextTrain->timeStr << " for " << config->getChangeArtSeconds() << endl;
	//checkTrainStatus();
}

void FamilyTrain::showTrainTimeline()
{
	float animtime = config->getChangeArtSeconds();
	float startPosition = 0.;
	float endPosition = 1.;
	trainTimeline->apply(&waitFrac,
		startPosition,
		endPosition,
		animtime)
		.finishFn(std::bind(&FamilyTrain::setNextTrain, this));
}

void FamilyTrain::nextTrainSetup()
{
	float animtime = nextTrain->timeAbs - currentTime;
	if (animtime > minCooldown)
	{
		cout << "timer to the next:" << animtime << endl;
		float startPosition = 0.;
		float endPosition = 1.;
		trainTimeline->apply(&waitFrac,
			startPosition,
			endPosition,
			animtime)
			.finishFn(std::bind(&FamilyTrain::trainIsComing, this));
	}
	else
	{
		ofLogNotice("next train it too close");
	}

}

bool frames::FamilyTrain::compareTrainByTime(const TrainRequest::Train& train1, const TrainRequest::Train& train2)
{
	return train1.timeAbs < train2.timeAbs;
}


//struct isEarlier
//{
//	bool operator()(const TrainRequest::Train& train1, TrainRequest::Train& train2) const
//	{
//		return train1.timeAbs < train2.timeAbs;
//	}
//};

//void FamilyTrain::addTrain()
//{
//	TrainRequest::Train tempTrain;
//	tempTrain.title = "Test train - full title";
//	tempTrain.shortTitle = "Test train";
//	tempTrain.timeAbs = currentTime + ofRandom(rndCooldown);
//	tempTrain.timeStr = convertToString(tempTrain.timeAbs);
//	schedule.push_back(tempTrain);
//}
//
//void FamilyTrain::addTrain(time_t time)
//{
//	TrainRequest::Train tempTrain;
//	tempTrain.title = "Test train - full title";
//	tempTrain.shortTitle = "Test train";
//	tempTrain.timeAbs = time;
//	tempTrain.timeStr = convertToString(tempTrain.timeAbs);
//	addTrain(tempTrain);
//}
//
void FamilyTrain::addTrain(const int& amount)
{
	time_t tempTime = currentTime;
	for (int i = 0; i < amount; i++)
	{
		TrainRequest::Train tempTrain;
		tempTrain.title = "Test train #" + to_string(i) + " - full title";
		tempTrain.shortTitle = "Test train#" + to_string(i);
		tempTime = tempTime + int(ofRandom(rndCooldown));
		cout << ofRandom(30, 60) << endl;
		tempTrain.timeAbs = tempTime;
		tempTrain.timeStr = convertToString(tempTrain.timeAbs);
		addTrain(tempTrain);
	}
}

void FamilyTrain::addTrain(const TrainRequest::Train & train)
{
	schedule.push_back(train);

}


void FamilyTrain::scheduleStatus()
{
	cout << "=====================================scheduleStatus=====================================" << endl;
	for (size_t i = 0; i < schedule.size(); i++)
	{
		cout << to_string(i) << ":\t" << schedule[i].shortTitle << endl << "\t" << schedule[i].timeStr << endl;
	}
	cout << "=====================================END=====================================" << endl;
}

time_t FamilyTrain::convertToSeconds(const string& timeTo)
{
	struct tm tm;
	istringstream ss(timeTo);
	ss >> get_time(&tm, "%Y-%m-%d %H:%M:%S");
	time_t time = mktime(&tm);
	return time;
}

string FamilyTrain::convertToString(time_t timeTo)
{
	time_t rawtime = timeTo;
	struct tm * timeinfo;
	char buffer[80];
	timeinfo = localtime(&rawtime);
	strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
	return buffer;
}

string FamilyTrain::convertToString(time_t timeTo, const string& format)
{
	time_t rawtime = timeTo;
	struct tm * timeinfo;
	char buffer[80];
	timeinfo = localtime(&rawtime);
	strftime(buffer, 80, format.c_str(), timeinfo);
	return buffer;
}