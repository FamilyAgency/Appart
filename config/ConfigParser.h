#pragma once
#include "ofMain.h"
#include "Config/Config.h"
#include "ofxJSON.h"

namespace synapse
{
	class ConfigParser
	{
	public:
		ConfigParser();
		~ConfigParser();

		void parse(ConfigPtr config, const string& configString);

		ofEvent<void> parseConfigSuccessEvent;
		ofEvent<void> parseConfigErrorEvent;

	private:
		ofxJSONElement configResult;	

	protected:
	};
}
