#pragma once
#include "ofMain.h"
#include "TCPClient.h"
#include "Config/Config.h"
#include "ofxNetwork\src\ofxNetwork.h"
#include "ofxJSON.h"

namespace synapse
{
	class TCPAppMessageClient : public synapse::TCPClient
	{
	public:
		TCPAppMessageClient();
		virtual ~TCPAppMessageClient();
		enum class CommandType
		{
			ClientAuth,
			KeepAliveToggle,
			NextArt,
			RandomArt,
			GetInfo
		};
		ofEvent<CommandType> newCommandEvent;
		ofEvent<void> nextArtEvent;
		ofEvent<void> randomArtEvent;

	private:
		map<string, CommandType> commandMap;
		void onNewMessage(string& message);

		void sendAuthMessage();
		void sendAppInfo();
	};
}

