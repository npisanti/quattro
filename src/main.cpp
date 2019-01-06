#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    
#ifdef __ARM_ARCH

	ofSetupOpenGL(WIDTH, HEIGHT, OF_FULLSCREEN);			// <-------- setup the GL context
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());
    
#else
	ofGLFWWindowSettings settings;
	settings.width = WIDTH;
	settings.height = HEIGHT;
	settings.setPosition(ofVec2f(0,0));
	settings.resizable = false;
    settings.decorated = false;
    settings.multiMonitorFullScreen = true;
	shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);    

	shared_ptr<ofApp> mainApp(new ofApp);

	ofRunApp(mainWindow, mainApp);
	ofRunMainLoop();
#endif

}
