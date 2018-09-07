#pragma once
#include "ofMain.h"
#include "Config/Config.h"
#include "Config/ConfigLoader.h"
#include "Config/ConfigParser.h"
namespace synapse
{
	class ConfigController
	{
	public:
		ConfigController();
		~ConfigController();

		void init(const string& configPath);

		ofEvent<void> configSuccessEvent;
		ofEvent<void> configErrorEvent;

		ConfigPtr getConfig();

	private:
		string configPath;
		ConfigPtr config;

		ConfigLoader configLoader;
		void onConfigLoadSuccess(string& rawConfig);
		void onConfigLoadError();

		ConfigParser configParser;
		void onConfigParseSuccess();
		void onConfigParseError();

	protected:
	};
}