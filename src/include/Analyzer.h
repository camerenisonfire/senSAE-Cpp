// analyzer.h
#ifndef ANALYZER_H
#define ANALYZER_H

class Analyzer
{
public:
    //functions defined here
    virtual void update() = 0;
    virtual void setup() = 0;
    // Each analyzer will have getters
    // for all of its analysis data
    
    //variables defined here
};

#endif
