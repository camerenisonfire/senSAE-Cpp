//
//  SoundAnalyzer.h
//  sensaeSynth
//
//  Created by Spencer Rhodes on 9/23/15.
//
//
#ifndef SOUND_ANALYZER_H
#define SOUND_ANALYZER_H
#include <queue>
#include "Analyzer.h"
#include "ofSoundStream.h"
#include "ofSoundPlayer.h"
#include "fft.h"

class SoundAnalyzer : public Analyzer
{
public:
    // Inherited Analyzer functions
    SoundAnalyzer ();
    void update();
    void setup();
    // Overloaded functions
    void setup(ofBaseApp *baseApp);
    void setup(ofBaseApp *baseApp, int inputChannels, int sampleRate, int bufferSize);
    void setDevice(int deviceId, ofBaseApp *baseApp, int inputChannels, int sampleRate, int bufferSize);

    // Sound Analyzer only functions
    void audioIn(float* input, int bufferSize, int nChannels);
    float getSoundLevel();
    float * getRawFrequencies();
    vector<float> getNormFrequencies();
    float getAvgEnergy();
    float getPeakEnergy();
    bool detectedOnset();
    vector<ofSoundDevice> getSoundDevices();

    //variables defined here
private:
    float soundLevel;
    int bufferSize;
    int sampleRate;
    ofSoundStream sound;

    // FFT variables
    fft * soundFFT;
    float * fftMagnitudes;
    float * fftWindow;
    float * fftPhases;
    int fftBinSize;
    vector<float> fftData;
    float dataPeak;

    // Beat slicing variables
    unsigned long energyAvgStart; // Track the beginning of energy average (as # of millis since start of app)
    unsigned long timeOfLastDiscard; // Track the time the last "discard" was pushed to discard queue
    float avgEnergy; // The average energy over the previous 0.75-1.25 seconds
    float instEnergy; // Instantaneous energy (energy from most recent update)
    float totalEnergy; // Track the numerator of mean of energy
    std::queue<float> energyToDiscard; // Keep a queue of 0.5 second energy sums to discard
    std::queue<float> numSamplesInEnergyToDiscard; // Keep another queue with corresponding number of
                                                   // samples in 0.5 second energy sum
    float curEnergy; // Track the sum of newest 0.5 second energy
    float numSamplesInTotalEnergyAvg; // Track the total number of records in current avgEnergy
    float numSamplesInCurEnergyAvg; // Track the number of records in sum of new energy
    int onsetMaxFreqIndex;

};

#endif
