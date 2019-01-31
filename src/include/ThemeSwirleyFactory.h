#ifndef THEMESWIRLEY_FACTORY_H
#define THEMESWIRLEY_FACTORY_H

#include "ThemeFactory.h"
class ThemeSwirleyFactory : public ThemeFactory
{
public:
    ThemeSwirley *create(SoundAnalyzer* sndAnly) {return new ThemeSwirley(sndAnly);}
};

#endif