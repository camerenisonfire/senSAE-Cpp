//
//  main.cpp
//  UnitTest
//
//  Created by Spencer Rhodes on 10/2/15.
//  Copyright (c) 2015 sensae. All rights reserved.
//

#include <iostream>
#include "gtest/gtest.h"
#include "SoundAnalyzerTest.h"


TEST_F(SoundAnalyzerTest, UpdateTest) {
    // Create soundPlayer for testing preset sounds
    ofSoundPlayer sound;
    sound.loadSound("freqtest_1000Hz.wav");
    sound.setLoop(false);
    SoundAnalyzerTest::SetUp();
    ASSERT_EQ(1, analyzer.getAnalyzerID());
    
    float * spectrum;
    sound.play();
    while (sound.getIsPlaying()) {
        analyzer.update();
        spectrum = analyzer.getFrequencies();
        for (int i=0; i<128; i++) {
            std::cout << "\nIndex " << i << " = " << spectrum[i];
        }
    }

    ASSERT_NE(2, analyzer.getAnalyzerID());
    
}

int main(int argc, char * argv[]) {
    
    // Testing initialization
    ::testing::InitGoogleTest(&argc, argv);
    
    // Run tests
    return RUN_ALL_TESTS();
}
