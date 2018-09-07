#include "Config/ConfigParser.h"
using namespace synapse;

ConfigParser::ConfigParser()
{

}

ConfigParser::~ConfigParser()
{

}

void ConfigParser::parse(ConfigPtr config, const string& configString)
{
	bool parsingSuccessful = configResult.parse(configString);

	if (parsingSuccessful)
	{
		config->setFullscreen(configResult["fullscreen"].asBool());
		Config::AppData configAppData;
		configAppData.version = configResult["version"].asString();
		configAppData.appTypeId = configResult["appTypeId"].asInt();
		configAppData.appId = configResult["appId"].asInt();
		configAppData.logPath = configResult["logPath"].asString();
		config->setAppData(configAppData);

		Config::SocketServer configSocketServer;
		configSocketServer.ip = configResult["socketServer"]["ip"].asString();
		configSocketServer.port = configResult["socketServer"]["port"].asInt();
		configSocketServer.autoConnect = configResult["socketServer"]["autoConnect"].asBool();
		configSocketServer.delimiter = configResult["socketServer"]["delimeter"].asString();
		config->setScoketServer(configSocketServer);

		Config::YandexData yandexData;
		yandexData.apiKey = configResult["yandexTrain"]["apiKey"].asString();
		yandexData.protocol = configResult["yandexTrain"]["protocol"].asString();
		yandexData.host = configResult["yandexTrain"]["host"].asString();
		yandexData.format = configResult["yandexTrain"]["format"].asString();
		yandexData.station = configResult["yandexTrain"]["station"].asString();
		yandexData.lang = configResult["yandexTrain"]["lang"].asString();
		yandexData.date = configResult["yandexTrain"]["date"].asString();
		config->setYandexdata(yandexData);

		config->setChangeArtSeconds(configResult["changeArtSeconds"].asFloat());
		config->setArtRotateMode(configResult["artRotateMode"].asBool());

		int type = configResult["viewType"].asInt();
		config->setViewType((frames::VIEW_TYPES)type);

		int width = configResult["screen"]["width"].asInt();
		config->setScreenWidth(width);

		int widthLarge = configResult["screen"]["largeWidth"].asInt();
		config->setScreenWidthLarge(widthLarge);

		int height = configResult["screen"]["height"].asInt();		
		config->setScreenHeight(height);


		int fboNumSamples = configResult["fbo"]["numSamples"].asInt();
		config->setfboNumSamples(fboNumSamples);

		int fboNumSamplesThreeArts = configResult["fbo"]["fboNumSamplesThreeArts"].asInt();
		config->setfboNumSamplesThreeArts(fboNumSamplesThreeArts);
		
		Config::Commands configCommands;		
		for (int i = 0; i < configResult["commands"].size(); i++)
		{
			Config::Command tempCommand;
			tempCommand.metaName = configResult["commands"][i]["metaName"].asString();
			tempCommand.action = configResult["commands"][i]["action"].asString();
			configCommands.push_back(tempCommand);
		}
		config->setCommands(configCommands);
		parseConfigSuccessEvent.notify(this);
	}
	else
	{
		parseConfigErrorEvent.notify(this);
	}
}

