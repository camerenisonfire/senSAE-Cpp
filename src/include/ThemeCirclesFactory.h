#ifndef THEMECIRCLES_FACTORY_H
#define THEMECIRCLES_FACTORY_H

#include "ThemeFactory.h"
class ThemeCirclesFactory : public ThemeFactory
{
public:
    ThemeCircles *create(SoundAnalyzer* sndAnly) {return new ThemeCircles(sndAnly);}
};

#endif
