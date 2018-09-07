#include "ofApp.h"

using namespace frames;
using namespace synapse;

//--------------------------------------------------------------
ofApp::ofApp()
{
	cout << "construction............................" << endl;

	viewsMap.insert(pair<VIEW_TYPES, AbstractViewerPtr>(VIEW_TYPES::ONE_ARTWORK, OneArtViewerPtr(new OneArtViewer())));
	viewsMap.insert(pair<VIEW_TYPES, AbstractViewerPtr>(VIEW_TYPES::THREE_ARTWORKS, ThreeArtsViewerPtr(new ThreeArtsViewer())));
	viewsMap.insert(pair<VIEW_TYPES, AbstractViewerPtr>(VIEW_TYPES::ONE_LARGE_ARTWORK, OneLargeArtworkPtr(new OneLargeArtwork())));
	viewsMap.insert(pair<VIEW_TYPES, AbstractViewerPtr>(VIEW_TYPES::MIXED, MixedArtViwerPtr(new MixedArtViwer())));
}

AbstractViewerPtr ofApp::createViewer(VIEW_TYPES type)
{
	switch (type)
	{	
	case frames::VIEW_TYPES::ONE_ARTWORK:
		return OneArtViewerPtr(new OneArtViewer());	

	case frames::VIEW_TYPES::THREE_ARTWORKS:
		return ThreeArtsViewerPtr(new ThreeArtsViewer());

	case frames::VIEW_TYPES::ONE_LARGE_ARTWORK:
		return OneLargeArtworkPtr(new OneLargeArtwork());

	case frames::VIEW_TYPES::MIXED:
		return MixedArtViwerPtr(new MixedArtViwer());
	}
}

void ofApp::setCommandLineArgs(const vector<string>& args)
{
	cout << "args printed " << endl;
	if (args.size() > 1)
	{
		for (size_t i = 0; i < args.size(); i++)
		{
			auto data = tools().splitString(args[i], '=');
			if (data.size() > 1)
			{
				if (data[0].find("config") != -1)
				{
					configPath = data[1];
				}
			}
		}
	}

	loadConfig();
}

void ofApp::loadConfig()
{
	ofAddListener(configController.configSuccessEvent, this, &ofApp::onConfigSuccess);
	ofAddListener(configController.configErrorEvent, this, &ofApp::onConfigError);
	configController.init(configPath);
}

void ofApp::onConfigSuccess()
{
	configPtr = configController.getConfig();
	startApp();
}

void ofApp::onConfigError()
{
	// error message
}

//--------------------------------------------------------------
void ofApp::startApp()
{
	cout << "startApp............................" << endl;

	// system init
	ofSetFullscreen(configPtr->getFullscreen());
	ofEnableAlphaBlending();
	//ofSetVerticalSync(true);
	//ofSetFrameRate(60);
	configPtr->getShowCursor() ? ofShowCursor() : ofHideCursor();

	// force exit field activate by click
	const int closeSquareSide = 200;
	exitRectangle = ofRectangle(ofGetWindowWidth() - closeSquareSide, 0, ofGetWindowWidth(), closeSquareSide);

	// logger init
	logger().init(configPtr->getAppData().logPath);
	logger().log(Logger::LogType::Message, "message test");
	logger().log(Logger::LogType::Error, "error message test");	

	// art init
	auto artCreator = MainArtCreatorPtr(new MainArtCreator());
	artCreator->create(configPtr);
	forceUpdateArt = artCreator->getAllForceUpdateArt();

	for (size_t i = 0; i < forceUpdateArt.size(); i++)
	{
		ofAddListener(forceUpdateArt[i]->showMeForce, this, &ofApp::showForceHandler);
	}

	auto viewType = configPtr->getViewType();
	viewer = createViewer(viewType);
	if (viewer)
	{
		viewer->setup(artCreator, configPtr);
	}

	// tcp init
	ofAddListener(tcpAppMessageClient.newCommandEvent, this, &ofApp::onNewCommand);
	ofAddListener(tcpAppMessageClient.nextArtEvent, this, &ofApp::onNextArtEvent);
	tcpAppMessageClient.connect(configPtr);
}

void ofApp::onNewCommand(TCPAppMessageClient::CommandType& command)
{
	
}

void ofApp::onNextArtEvent()
{
	viewer->nextArt();
}

void ofApp::update()
{
	tcpAppMessageClient.update();

	if (viewer)
	{
		viewer->update();
	}

	for (size_t i = 0; i < forceUpdateArt.size(); i++)
	{
		forceUpdateArt[i]->forceUpdate();
	}	
}

void ofApp::draw()
{
	if (viewer)
	{
		viewer->draw();
	}
}

//--------------------------------------------------------------
void ofApp::showForceHandler(frames::FRAME_TYPES& type)
{
	viewer->setForceArt(type);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	viewer->nextArt();	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{	
	if (exitRectangle.inside(x, y))
	{
		ofExit();
	}
}