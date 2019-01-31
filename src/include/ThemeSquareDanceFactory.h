#ifndef THEMESQUAREDANCE_FACTORY_H
#define THEMESQUAREDANCE_FACTORY_H

#include "ThemeFactory.h"
class ThemeSquareDanceFactory : public ThemeFactory
{
public:
    ThemeSquareDance *create(SoundAnalyzer* sndAnly) {return new ThemeSquareDance(sndAnly);}
};

#endif
