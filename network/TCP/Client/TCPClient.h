#pragma once
#include "ofMain.h"
#include "Config/Config.h"
#include "ofxNetwork\src\ofxNetwork.h"

namespace synapse
{
	class TCPClient : public ofThread
	{
	public:
		TCPClient();
		~TCPClient();

		void update();
		void connect(ConfigPtr socketServer);

		ofEvent<string> newMessageEvent;

	protected:
		ofxTCPClient tcp;
		ConfigPtr config;

	private:
		string ip;
		int port;
		string delimiter;
		bool autoConnect;
		float reconnectMills;

		string message;
		bool newMessageIsCome;
		bool firstRun = true;
		

		void newMessage(const string& message);
		void tryToConnect();

		void threadedFunction();
	};
}