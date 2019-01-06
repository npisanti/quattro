#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include "ofxGui.h"
#include "np-junkrepo.h"

#define WIDTH 1920
#define HEIGHT 1080

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        // pdsp modules
        ofxPDSPEngine   engine;
        
        np::SinePerc    beep;
        np::StereoDelayFx delay;
        np::ChristoffelSeq  cseq;
        np::ReverbFx        reverb;

        np::GrainDrone      drone;
        np::SampleData      sample;

        np::CLFO            fbMod;

        ofxPanel gui;
    
        np::PSystem ps;
        np::Palette3 palette;
        
        ofParameterGroup gravityMod;
            ofParameter<int>    gMinMod;
            ofParameter<int>    gMaxMod;
            ofParameter<bool>   gModActive;
            atomic<float>       gModValue;
            bool                bGravUpdated;

        np::PeakMeter   meterL;
        np::PeakMeter   meterR;
        
        ofParameterGroup    graphics;
            ofParameter<int> rectsWidth;
            ofParameter<int> rectsHeight;

        bool bDrawGui;
};
