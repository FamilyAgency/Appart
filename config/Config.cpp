#include "Config/Config.h"

using namespace synapse;

Config::Config()
{
	
}

Config::~Config()
{

}

void Config::setFullscreen(bool value)
{
	isFullscreen = value;
}

bool Config::getFullscreen() const
{
	return isFullscreen;
}

void Config::setShowCursor(bool value)
{
	showCursor = value;
}

bool Config::getShowCursor() const
{
	return showCursor;
}

void Config::setAppData(const AppData& appData)
{
	this->appData = appData;
}

Config::AppData Config::getAppData() const
{
	return appData;
}

void Config::setScoketServer(const SocketServer& socketServer)
{
	this->socketServer = socketServer;
}

Config::SocketServer Config::getSocketServer() const
{
	return socketServer;
}

void Config::setCommands(const Commands& commands)
{
	this->commands = commands;
}

Config::Commands Config::getCommands() const
{
	return commands;
}

void Config::setChangeArtSeconds(float value)
{
	changeArtSeconds = value;
	createConfigRotator();
}

float Config::getChangeArtSeconds() const
{
	return changeArtSeconds;
}

void Config::setYandexdata(const YandexData& yandexData)
{
	this->yandexData = yandexData;
}

Config::YandexData Config::getYandexdata() const
{
	return yandexData;
}

void Config::createConfigRotator()
{
	configRotator.clear();

	ConfigObject configObject;

	configObject.viewType = frames::VIEW_TYPES::THREE_ARTWORKS;
	configObject.typeSeconds = 3 * changeArtSeconds;
	configObject.oneArtSeconds = changeArtSeconds;
	configRotator.push_back(configObject);

	configObject.viewType = frames::VIEW_TYPES::ONE_LARGE_ARTWORK;
	configObject.typeSeconds = 3 * changeArtSeconds;
	configObject.oneArtSeconds = changeArtSeconds;
	configRotator.push_back(configObject);
}

bool Config::getArtRotateMode() const
{
	return artRotateMode;
}

void Config::setArtRotateMode(bool value)
{
	artRotateMode = value;
}

frames::VIEW_TYPES Config::getViewType() const
{
	return viewType;
}

void  Config::setViewType(frames::VIEW_TYPES value)
{
	viewType = value;
}

void Config::setScreenWidth(int value)
{
	screenWidth = value;
}

int Config::getScreenWidth() const
{
	return screenWidth;
}

void Config::setScreenHeight(int value)
{
	screenHeight = value;
}

int Config::getScreenHeight() const
{
	return screenHeight;
}

void Config::setScreenWidthLarge(int value)
{
	screenWidthLarge = value;
}

int Config::getScreenWidthLarge() const
{
	return screenWidthLarge;
}

void Config::setfboNumSamples(int value)
{
	fboNumSamples = value;
}

int Config::getfboNumSamples() const
{
	return fboNumSamples;
}

void Config::setfboNumSamplesThreeArts(int value)
{
	fboNumSamplesThreeArts = value;
}

int Config::getfboNumSamplesThreeArts() const
{
	return fboNumSamplesThreeArts;
}