#ifndef THEMECIRCLES_H
#define THEMECIRCLES_H

#include "Theme.h"
#include "SoundAnalyzer.h"

class ThemeCircles : public Theme
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
    ThemeCircles();
    ThemeCircles(SoundAnalyzer*);

    float soundLevel;
    float radius;
    float red, blue, green;
    float brightness;
    float rotation;
    int multiplyLevel;
    float hue;
    float xOff;
    float noiseResult;
    float posXOff;
    float posYOff;
    int numCirclesInRow;
    int refreshGridCount;
    bool showTheme = false;

    //inherited from Theme.h
    void setup();
    void draw();
    void update();
    bool isReady();
    const char* getName();
    void setSize(int, int);

    void drawCircles();
    void drawSingleCircle(float multiplier);
    void gridCountUpdate();
    void drawGrid();

};

#endif
