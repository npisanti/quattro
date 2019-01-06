
// app_cseq
// Nicola Pisanti, GPLv3 License, 2016

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    
    bGravUpdated = true;    
#ifdef __ARM_ARCH
    bDrawGui = false;
#else
    bDrawGui = true;
#endif

    // ---------------------SEQUENCES------------------------
    engine.score.init( 2, 1, 86.0f); // sections, sequences, tempo
    
    // section[0] reserved for masterplan
    engine.score.sections[0].resizeCells(1);
    engine.score.sections[0].sequence(0).setLength( 8.0); // 8 bars
    engine.score.sections[0].sequence(0).code = [&] () noexcept { // masterplan sequence
        cseq.remake = true;
        delay.lDelayTimeControl.getOFParameterInt() = pdspDice(1, 9);
        delay.rDelayTimeControl.getOFParameterInt() = pdspDice(1, 9);
        drone.positionControl.getOFParameterFloat() = 0.2f + pdspURan() * 0.6f;
        float fb = pdspURan() * 0.5f;
        drone.resonatorsFBControl.getOFParameterFloat() = fb;
        
        if( gModActive ){
            gModValue = ofMap(fb, 0.0f, 0.5f, gMinMod, gMaxMod );
            bGravUpdated = false;
        }
        
    };
    engine.score.sections[0].label(0, "masterplan" );    
    engine.score.sections[0].launchCell(0);
    
    engine.score.sections[1].setCell( 0, &cseq, pdsp::Behavior::Loop );
    engine.score.sections[1].label(0, "cseq" );  
    engine.score.sections[1].launchCell(0);
    
    // ---------------------PATCHING------------------------
    engine.score.sections[1].out_trig(0) >> beep.in("trig");
    beep >> engine.audio_out(0);
    beep >> engine.audio_out(1);   
    beep >> delay.in_0();
    beep >> delay.in_1();
    delay.out_0() >> engine.audio_out(0);
    delay.out_1() >> engine.audio_out(1);  

    beep            >> meterL;
    delay.out_0()   >> meterL;
    beep            >> meterR;
    delay.out_1()   >> meterR;
    meterL.activate( engine );
    meterR.activate( engine );
    
    beep >> reverb.in();
    delay.out_0() >> reverb.in();
    delay.out_1() >> reverb.in();
    reverb.out_0() >> engine.audio_out(0);
    reverb.out_1() >> engine.audio_out(1);  
    
    drone.setup( 420, 80 , false );
    drone.addSample( sample );
    drone.smoothing( 100.0f );

    drone.out_L() >> engine.audio_out(0);
    drone.out_R() >> engine.audio_out(1);    
    
    fbMod >> drone.in_fb();


    // -------------------GRAPHIC SETUP--------------
#ifdef __ARM_ARCH
    np::linux_no_cursor_fix();
#endif
    
    ofBackground(0);
    ofSetFrameRate(24.0);
    
    ps.setup( WIDTH, HEIGHT );
    
    engine.graphics.setup( 450, 600,  {   0,    50 }, 
                                      {   0,     1 }  );
    engine.graphics.setPosition( 10, 10 );
    engine.graphics.setColor (ofColor( 255, 95, 95 ) );

    // ------------- GUI ---------------
    gui.setup("panel", "settings.xml", ofGetWidth()-220, 20 );
    
    gui.add( cseq.ui );
    gui.add( beep.ui );
    gui.add( delay.ui );
    gui.add( reverb.ui );
    
    gui.add( drone.ui );
    gui.add( sample.ui );
    gui.add( fbMod.ui );
    
    gui.add( palette.ui );
    gui.add( ps.ui );

    gravityMod.setName("modulate gravity");
    gravityMod.add( gModActive.set("activate", false));    
    gravityMod.add( gMinMod.set("min mod", 150, 20, 300));
    gravityMod.add( gMaxMod.set("max mod", 150, 20, 300));
    gModValue = 150;
    gui.add( gravityMod );
    
    meterL.ui.setName("meter L");
    gui.add( meterL.ui );
    meterR.ui.setName("meter R");
    gui.add( meterR.ui );
    
    graphics.setName( "central graphics");
    graphics.add(rectsWidth.set("width", 200, 20, WIDTH/2) );
    graphics.add(rectsHeight.set("height", 100, 20, HEIGHT/2) );
    gui.add( graphics );
    
    gui.minimizeAll();
    gui.loadFromFile("settings.xml");

    // ------------SETUPS AND START AUDIO-------------
    engine.listDevices();
    engine.setDeviceID(0); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    engine.setup( 44100, 512, 3); 

}

//--------------------------------------------------------------
void ofApp::update(){
    
    if( gModActive  && !bGravUpdated ){
        ps.gravAcc = gModValue;
        bGravUpdated = true;
    }
    ps.update( palette.colors[0] );
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    ps.draw( 0, 0, bDrawGui );
    
    ofFill();
    int w = rectsWidth / 2;
    ofSetColor( palette.colors[1], ofMap(meterL.meter_output(), 0.0f, 1.0f, 0, 255, true) );    
    ofDrawRectangle( WIDTH/2 - w, (HEIGHT-rectsHeight)/2, w, rectsHeight );
    ofSetColor( palette.colors[1], ofMap(meterR.meter_output(), 0.0f, 1.0f, 0, 255, true) );        
    ofDrawRectangle( WIDTH/2,     (HEIGHT-rectsHeight)/2, w, rectsHeight );
        
    if( bDrawGui ){
        engine.graphics.draw();
        ofSetColor( 255, 95, 95 );
        drone.draw( 10, 140 );
        gui.draw();                
    }

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){        
        case 'g':
            bDrawGui = bDrawGui ? false : true;
        break;

        case 'c':
            for(np::PSystem::Attractor a : ps.attractors){
                a.x = ofGetWidth() * 0.5f;
                a.y = ofGetHeight() * 0.5f;
            }
        break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
