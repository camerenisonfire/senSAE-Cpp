#ifndef THEMEFACTORY_H
#define THEMEFACTORY_H
#include "Theme.h"
#include "SoundAnalyzer.h"

class ThemeFactory
{
public:
    virtual Theme *create(SoundAnalyzer*) = 0;
};

#endif