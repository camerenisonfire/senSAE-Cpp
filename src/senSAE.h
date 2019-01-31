#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxImGui.h"
#include "SoundAnalyzer.h"
#include "ThemeList.h"
#include "MasterThemeList.h"

// This variable defines the time to perform half of a full transition.
// That is, the old theme will fully fade out in this time, and then the next
// theme will fade in fully in this time
#define TRANSITION_DURATION_MS_L 2000L // ms
#define TRANSITION_DURATION_MS_F 2000.0f // (ms = milliseconds)
#define DEFAULT_TIMING 30000L // in milliseconds
#ifndef MAX_THEMES
#define MAX_THEMES 100
#endif

class senSAE : public ofBaseApp{

public:
    typedef std::map<const char*, ThemeFactory *> ThemeFactories;

    // General class functions
    void setup();
    void update();
    void draw();

    // GUI functions
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void audioIn(float *input, int bufferSize, int nChannels);

    // Other functions
    void addThemeToList(const char* themeToAdd);

private:
    // Make a function for setting up factories (FOR NOW... this is a test)
    ThemeFactories factories;
    void setupFactories();

    // General class variables
    std::string errorMessage = "";

    // GUI variables
    ofxImGui desktopGui;
    ofFbo fbo;
    bool firstTime;
    bool showSynth;
    bool showGui;
    bool* p_showGui;
    float randomHues[200]; // Highly doubt user will ever have 200 themes in their theme list...
    ofTrueTypeFont verdanaFont;
    
    // Theme list / senSAE variables
    bool runningSenSAE;
    int wasRunning;
    ofColor_<float> sensaeRunColor;
    unsigned long timeStarted;
    unsigned long timeThemeStarted;
    int hourEnd[MAX_THEMES - 1];
    int minuteEnd[MAX_THEMES - 1];
    int secondEnd[MAX_THEMES - 1];
    bool isTransitioning;
    unsigned long timeTransitionStarted;
    bool timingsSaved = false;
    unsigned long whenTimingsSaved;
    
    // senSAE generative logo variables
    ofImage logoWireframe;
    double logoXstart;
    double logoYstart;
    // 7 rectangle for each bar, all moving randomly
    static const int numBars = 7;
    ofRectangle redBar;
    ofRectangle orangeBar;
    ofRectangle yellowBar;
    ofRectangle greenBar;
    ofRectangle lightBlueBar;
    ofRectangle royalBlueBar;
    ofRectangle purpleBar;
    // Have a variable that keeps track of the bar's width "acceleration"
    double barAccel[7];
    bool logoBeingShown;
    unsigned long startedShowingLogo;

    // Running themes (TESTING this)
    ThemeList themes;
    int runningTheme; // Index to running theme
    int selectedTheme;
    bool * themesInList;
    int windowWidth;
    int windowHeight;

    // Sound variables
    SoundAnalyzer sndAnly;
    ofxSlider<float> trackSound;
    ofParameter<float> sndTracker;
		
};
