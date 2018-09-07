#pragma once
#include "ofMain.h"
#include "../core/Types.h"

namespace synapse
{
	typedef ofPtr<class Config> ConfigPtr;

	class Config
	{
	public:
		struct AppData
		{
			string version;
			int appTypeId;
			int appId;
			string logPath;
		};	

		struct SocketServer
		{
			string ip;
			int port;
			bool autoConnect;
			string delimiter;
			float reconnectMills = 3000;
		};

		struct Command
		{
			string metaName;
			string action;			
		};

		typedef vector<Command> Commands;

		struct YandexData
		{
			string apiKey;
			string protocol;
			string host;
			string format;
			string station;
			string lang;
			string date;
		};

		Config();
		~Config();

		void setAppData(const AppData& appData);
		void setScoketServer(const SocketServer& socketServer);
		void setCommands(const Commands& commands);

		AppData getAppData() const;
		SocketServer getSocketServer() const;
		Commands getCommands() const;

		void setFullscreen(bool value);
		bool getFullscreen() const;

		void setShowCursor(bool value);
		bool getShowCursor() const;

		bool getArtRotateMode() const;
		void setArtRotateMode(bool value);

		struct ConfigObject
		{
			frames::VIEW_TYPES viewType;
			float typeSeconds;
			float oneArtSeconds;
		};
		std::vector<ConfigObject> configRotator;				

		void setYandexdata(const YandexData& yandexData);
		YandexData getYandexdata() const;

		void setChangeArtSeconds(float value);
		float getChangeArtSeconds() const;

		void setScreenWidth(int value);
		int getScreenWidth() const;

		void setScreenHeight(int value);
		int getScreenHeight() const;
		
		void setScreenWidthLarge(int value);
		int getScreenWidthLarge() const;

		frames::VIEW_TYPES getViewType() const;
		void setViewType(frames::VIEW_TYPES value);

		void setfboNumSamples(int value);
		int getfboNumSamples() const;

		void setfboNumSamplesThreeArts(int value);
		int getfboNumSamplesThreeArts() const;

	private:
		AppData appData;
		SocketServer socketServer;
		YandexData yandexData;
		Commands commands;
		bool isFullscreen = false;
		bool showCursor = false;	
		float changeArtSeconds = 1 * 60.0f;

		void createConfigRotator();
		bool artRotateMode = true;
		frames::VIEW_TYPES viewType = frames::VIEW_TYPES::ONE_ARTWORK;

		int screenWidth = 1080 / 2;// 1080;//720
		int screenHeight = 1920 / 2;// 1920;// 1280
		int screenWidthLarge = screenWidth * 3;

		int fboNumSamples = 2;
		int fboNumSamplesThreeArts = 2;
	};
}