// Theme.h
#ifndef THEME_H
#define THEME_H

#include "OptionList.h"
#include "ofMain.h"

class Theme
{
    std::string inputType;
    OptionList options;
    OptionList defaultOptions;

public:
    virtual void setup() = 0;
    virtual void draw() = 0;
    virtual void update() = 0;
    virtual const char* getName() = 0;
    virtual bool isReady() = 0;
    virtual void setSize(int, int) = 0;
};

#endif
