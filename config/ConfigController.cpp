#include "Config/ConfigController.h"
using namespace synapse;

ConfigController::ConfigController()
{
	config = ConfigPtr(new Config());
}

ConfigController::~ConfigController()
{

}

void ConfigController::init(const string& path)
{
	configPath = path;
	ofAddListener(configLoader.loadSuccessEvent, this, &ConfigController::onConfigLoadSuccess);
	ofAddListener(configLoader.loadErrorEvent, this, &ConfigController::onConfigLoadError);
	configLoader.load(configPath);
}

void ConfigController::onConfigLoadSuccess(string& rawConfig)
{
	ofAddListener(configParser.parseConfigSuccessEvent, this, &ConfigController::onConfigParseSuccess);
	configParser.parse(config, rawConfig);
}

void ConfigController::onConfigParseSuccess()
{
	//parsed!
	configSuccessEvent.notify(this);
}

ConfigPtr ConfigController::getConfig()
{
	return config;
}

void ConfigController::onConfigLoadError()
{
	configErrorEvent.notify(this);
}

void ConfigController::onConfigParseError()
{
	configErrorEvent.notify(this);
}