#include "TCPClient.h"

using namespace synapse;

TCPClient::TCPClient(): autoConnect(false), newMessageIsCome(false)
{
	
}

void TCPClient::connect(ConfigPtr config)
{
	lock();	
	
	this->config = config;
	auto socketServer = config->getSocketServer();	

	ip = socketServer.ip;
	port = socketServer.port;
	autoConnect = socketServer.autoConnect;
	delimiter = socketServer.delimiter;
	reconnectMills = socketServer.reconnectMills;
	tcp.setMessageDelimiter(delimiter);	

	unlock();

	startThread();	
}

void TCPClient::update()
{
	if (newMessageIsCome)
	{		
		newMessage(message);
		newMessageIsCome = false;
	}
}

void TCPClient::newMessage(const string& message)
{
	cout << "New TCP message: " << message << endl;
	newMessageEvent.notify(this, const_cast<string&>(message));	
}

void TCPClient::tryToConnect()
{
	if (!tcp.isConnected())
	{
		tcp.setup(ip, port, false);		
	}	
}

void TCPClient::threadedFunction()
{
	lock();
	while (isThreadRunning())
	{
		if (!tcp.isConnected() && autoConnect)
		{			
			tryToConnect();
			sleep(reconnectMills);
		}
		cout<<"Is connected " << tcp.isConnected()<<endl;
		if (tcp.isConnected())
		{
			message = tcp.receive();
			if (message.length() > 0)
			{			
				newMessageIsCome = true;							
			}
		}
		sleep(100);
	}
	unlock();
	ofLogVerbose("TCPClient") << "thread finished";
}

TCPClient::~TCPClient()
{
	tcp.close();
	waitForThread(true);
}

