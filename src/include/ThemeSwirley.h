//
//  ThemeSwirley.h
//  sensaeSynth
//
//  Created by Spencer Rhodes on 10/28/15.
//
//
#ifndef THEMESWIRLEY_H
#define THEMESWIRLEY_H

#include "Theme.h"
#include "SoundAnalyzer.h"
#include "ofxGui.h"

#define WIDTH 16
#define HEIGHT 9

class ThemeSwirley : public Theme
{
    std::string inputType;
    OptionList options;
    OptionList defaultOptions;
    
private:
    SoundAnalyzer * soundAnly;
    SoundAnalyzer currentSoundAnalysis;
    int synthWidth;
    int synthHeight;
    
    // Theme-specific functions
    void matrixPattern();
    void spiralPattern();
    
public:
    ThemeSwirley();
    ThemeSwirley(SoundAnalyzer*);
    
    int wholeRotate;
    unsigned long onsetDecayRate;
    unsigned long timeOfLastOnset;
    float twistXChanger;
    float twistXchange;
    float prevLineLength;
    float lineLength;
    bool reverseTwistXchange;
    int rColor[200];
    int gColor[200];
    int bColor[200];
    ofxFloatSlider Scale;
    ofxFloatSlider Rotate;
    ofxFloatSlider Background;
    ofxToggle filled, type;
    ofxFloatSlider shiftY, rotate;
    ofxVec2Slider size;
    ofxColorSlider color;
    ofxIntSlider countX;
    ofxFloatSlider stepX;
    ofxFloatSlider twistX;
    ofxIntSlider countY;
    ofxFloatSlider stepY, twistY, pinchY;
    bool showTheme = false;
    
    // Inherited functions from Theme
    void setup();
    void draw();
    void update();
    bool isReady();
    const char* getName();
    void setSize(int, int);
};

#endif