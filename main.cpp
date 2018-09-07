#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main(int argc, char *argv[])
{
	//ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context
	ofGLFWWindowSettings settings;
	//settings.setGLVersion(3, 2); //we define the OpenGL version we want to use
	settings.width = 720;
	settings.height = 1280;
	//settings.decorated = false;
	ofCreateWindow(settings);

	ofApp *app = new ofApp();
	app->setCommandLineArgs(vector<string>(argv, argv + argc));
	ofRunApp(app);
}
