//
//  SoundAnalyzerTest.h
//  sensaeSynth
//
//  Created by Spencer Rhodes on 10/2/15.
//
//

#ifndef __sensaeSynth__SoundAnalyzerTest__
#define __sensaeSynth__SoundAnalyzerTest__
#endif /* defined(__sensaeSynth__SoundAnalyzerTest__) */

#include <stdio.h>
#include <gtest/gtest.h>
#include "SoundAnalyzer.h"

class SoundAnalyzerTest : public ::testing::Test {
protected:
    SoundAnalyzer analyzer;
    
    virtual void SetUp() {
        analyzer.setup();
    }
    
    
};