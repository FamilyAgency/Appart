#include "TCPAppMessageClient.h"

using namespace synapse;

TCPAppMessageClient::TCPAppMessageClient()
{	
	commandMap.insert(pair<string, CommandType>("clientAuth",		CommandType::ClientAuth));
	commandMap.insert(pair<string, CommandType>("keepAliveToggle",	CommandType::KeepAliveToggle));
	commandMap.insert(pair<string, CommandType>("nextArt",			CommandType::NextArt));
	commandMap.insert(pair<string, CommandType>("randomArt",		CommandType::RandomArt));
	commandMap.insert(pair<string, CommandType>("getInfo",			CommandType::GetInfo));

	ofAddListener(TCPClient::newMessageEvent, this, &TCPAppMessageClient::onNewMessage);
}

TCPAppMessageClient::~TCPAppMessageClient()
{

}

void TCPAppMessageClient::onNewMessage(string& message)
{
	cout << "message  " << message;
	ofxJSONElement messageResult;
	bool parseSeuccess = messageResult.parse(message);

	if (parseSeuccess)
	{
		CommandType newCommandType = commandMap[messageResult["command"]["type"].asString()];

		switch (newCommandType)
		{
		case CommandType::ClientAuth:
			sendAuthMessage();
			break;

		case CommandType::KeepAliveToggle:
			break;

		case CommandType::NextArt:
			ofNotifyEvent(nextArtEvent, this);
			break;

		case CommandType::RandomArt:
			ofNotifyEvent(randomArtEvent, this);
			break;

		case CommandType::GetInfo:
			sendAppInfo();
			break;
		}
		newCommandEvent.notify(this, newCommandType);
	}
}

void TCPAppMessageClient::sendAuthMessage()
{
	auto appId = to_string(config->getAppData().appId);
	auto appTypeId = to_string(config->getAppData().appTypeId);
	string authMessage = "{\"response\":{\"type\":\"clientAuth\",\"appTypeId\":" + appTypeId + ",\"appId\":" + appId + "}}";

	tcp.send(authMessage);
}

void TCPAppMessageClient::sendAppInfo()
{
	auto appId = to_string(config->getAppData().appId);
	auto appTypeId = to_string(config->getAppData().appTypeId);
	string infoString = "FPS: " + ofToString(ofGetFrameRate());
	string authMessage = "{\"response\":{\"type\":\"getInfo\", \"info\":\""+ infoString +"\",\"appTypeId\":" + appTypeId + ",\"appId\":" + appId + "}}";

	tcp.send(authMessage);	
}
