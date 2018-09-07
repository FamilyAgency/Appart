#pragma once
#include "ofMain.h"

#include "Config.h"
#include "core/viewers/AbstractViewer.h"
#include "core/viewers/OneArtViewer.h"
#include "core/viewers/ThreeArtsViewer.h"
#include "core/viewers/OneLargeArtwork.h"
#include "core/viewers/MixedArtViwer.h"
#include "core/MainArtCreator.h"

#include "Config/ConfigController.h"
#include "Config/Config.h"
#include "tools/logger/Logger.h"
#include "Network/TCP/Client/TCPAppMessageClient.h"
#include "Network/TCP/Client/TCPAppMessageClient.h"

class ofApp : public ofBaseApp
{
public:
	ofApp();

	void setCommandLineArgs(const vector<string>& args);

	void update();
	void draw();

	void keyPressed(int key);
	void mousePressed(int x, int y, int button);
	
	frames::AbstractViewerPtr viewer;
	vector<frames::ArtDrawablePtr> forceUpdateArt;
	ofRectangle exitRectangle;
	string debugString, configPath = "config.json";
	synapse::ConfigPtr configPtr;
	synapse::ConfigController configController;
	synapse::TCPAppMessageClient tcpAppMessageClient;

	void showForceHandler(frames::FRAME_TYPES& art);
	
	void onConfigSuccess();
	void onConfigError();
	void loadConfig();

	void startApp();

	void onNewCommand(synapse::TCPAppMessageClient::CommandType& command);
	void onNextArtEvent();
	std::map<frames::VIEW_TYPES, frames::AbstractViewerPtr> viewsMap;

	frames::AbstractViewerPtr createViewer(frames::VIEW_TYPES type);
	
};
