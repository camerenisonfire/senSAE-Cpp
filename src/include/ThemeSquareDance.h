//
//  SoundAnalyzer.h
//  sensaeSynth
//
//  Created by Spencer Rhodes on 9/23/15.
//
//
#ifndef THEMESQUAREDANCE_H
#define THEMESQUAREDANCE_H

#include "Theme.h"
#include "SoundAnalyzer.h"
#include "Rectangle.h"

#define WIDTH 16
#define HEIGHT 9

class ThemeSquareDance : public Theme
{
    std::string inputType;
    OptionList options;
    OptionList defaultOptions;

private:
    SoundAnalyzer * soundAnly;
    SoundAnalyzer currentSoundAnalysis;
    int synthWidth;
    int synthHeight;

public:
    ThemeSquareDance();
    ThemeSquareDance(SoundAnalyzer*);

    float soundLevel;
    float hue, scale, rate, background;
    int back_dir, angle_dir;
    Rectangle rects[WIDTH][HEIGHT];
    int width_spacer, height_spacer;
    int place_x, place_y;
    bool showTheme = false;

    //inherited from Theme.h
    void setup();
    void draw();
    void update();
    bool isReady();
    const char* getName();
    void setSize(int, int);

    //SquareDance specific functions
    void wrap_x();
    void wrap_y();

};

#endif
