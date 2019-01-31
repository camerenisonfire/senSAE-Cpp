//
//  SoundAnalyzer.cpp
//  sensaeSynth
//
//  Created by Spencer Rhodes on 9/23/15.
//
//

#include <stdio.h>
#include <math.h>
#include "SoundAnalyzer.h"

/**
 * Create an instance of SoundAnalyzer.
 */
SoundAnalyzer::SoundAnalyzer () {
    // Do nothing
}

/**
 * Update normalized FFT data with newest data obtained
 * from ofSoundStream in fftMagnitudes. Also updates the
 * soundLevel and handles beat slicing.
 */
void SoundAnalyzer::update() {
    dataPeak = 0;
    int peakIndex;

    // Copy values from fftMagnitudes to local fftData
    for (int i = 0; i < fftData.size(); i++) {
        fftData[i] = fftMagnitudes[i];

        // Don't allow power to exceed 10
        if (abs(fftData[i]) > 10) {
            fftData[i] = 0;
        }

        // Determine fftData peak and bin index
        if (dataPeak < fftData[i]) {
            dataPeak = fftData[i];
            peakIndex = i;
        }
    }

    // TESTING: Print the frequency with the highest relative contribution
    //ofLog(OF_LOG_NOTICE, "Peak Frequency: %f", ((float)peakIndex * (float)sampleRate / (float)bufferSize));

    // Compute the raw sound level and onset sound level (both as root-mean squares)
    float rawLevel = 0; float newOnsetRMS = 0;
    for (int i = 0; i < fftData.size(); i++) {
        if (newOnsetRMS == 0 && i > onsetMaxFreqIndex) {
            newOnsetRMS = rawLevel;
        }
        rawLevel += fftData[i] * fftData[i];
    }
    rawLevel = sqrt(rawLevel / fftData.size());
    newOnsetRMS = sqrt(newOnsetRMS / (onsetMaxFreqIndex+1));

    // Update soundLevel
    soundLevel = rawLevel;

    // Normalize fftData...
    // The reason we want to normalize the FFT data is because the
    // FFT gives us a frequency distribution of the sound, so with
    // this we're really only interested in the relative contribution
    // to each frequency, not necessarily the magnitude of the sound
    // itself. Hence the distinction between fftData and fftMagnitudes.
    if (dataPeak > 0) {
        for (int i = 0; i < fftData.size(); i++) {
            fftData[i] /= dataPeak;
        }
    }

    // Beat slicing...
    // The current implementation of beat slicing will maintain
    // an average sound energy for the lowest frequencies (up to
    // a certain freq, currently testing). A beat will be "detected"
    // whenever the instant sound energy of those frequencies exceeds
    // the average sound energy. Average sound energy will be over previous
    // 0.75 - 1.25 seconds (continuously discard previous 1/2 second of avg
    // everytime 1.25 second average duration is reached)

    // First, check if an average has been started
    if (energyAvgStart == 0) {
        // Start the average and tracking
        energyAvgStart = ofGetElapsedTimeMillis();
        timeOfLastDiscard = ofGetElapsedTimeMillis();
    }

    // Check if 0.5 seconds has passed since time of last discard
    if (ofGetElapsedTimeMillis() - timeOfLastDiscard >= 500) {
        // If so, push energy info from the last .5 secs to queue
        energyToDiscard.push(curEnergy);
        numSamplesInEnergyToDiscard.push(numSamplesInCurEnergyAvg);
        // Reset current energy tracking
        curEnergy = 0;
        numSamplesInCurEnergyAvg = 0;
    }

    // Check if 1.25 seconds has passed since start of total energy average
    if (ofGetElapsedTimeMillis() - energyAvgStart >= 1250) {
        // If so, reset duration of energy average to 0.75 seconds
        energyAvgStart = ofGetElapsedTimeMillis() - 750;
        // Then reset total energy average
        totalEnergy -= energyToDiscard.front(); energyToDiscard.pop();
        numSamplesInTotalEnergyAvg -= numSamplesInEnergyToDiscard.front(); numSamplesInEnergyToDiscard.pop();
    }

    // Now that we've checked everything, update current and total averages
    curEnergy += newOnsetRMS; totalEnergy += newOnsetRMS;
    numSamplesInCurEnergyAvg++; numSamplesInTotalEnergyAvg++;
    avgEnergy = totalEnergy / numSamplesInTotalEnergyAvg; instEnergy = newOnsetRMS;
}

/**
 * Inherited setup() method from Analyzer interface.
 * This is deprecated for SoundAnalyzer since it needs the pointer
 * to the ofBaseApp in order to set up the ofSoundStream.
 */
void SoundAnalyzer::setup() {
    // Deprecated, always need to provide ofBaseApp pointer for soundStream setup.
}

/**
 * If user doesn't provide sampling rate and audio buffer size, specifies
 * default values of 44100 Hz and buffer size of 256.
 */
void SoundAnalyzer::setup(ofBaseApp *baseApp) {
    setup(baseApp, 1, 44100, 256);
}

/**
 * Current method for setting up the SoundAnalyzer. Initializes the necessary
 * variables for performing sound analysis and starts the ofSoundStream. Eventually,
 * user will be able to specify number of input channels (default is 1).
 */
void SoundAnalyzer::setup(ofBaseApp *baseApp, int inputChannels, int sampleRate, int bufferSize) {
    // Setup SoundAnalyzer variables
    soundLevel = 0;
    this->sampleRate = sampleRate;
    this->bufferSize = bufferSize;

    // Setup FFT variables
    fftMagnitudes = NULL;
    fftWindow = NULL;
    fftPhases = NULL;
    fftBinSize = (int)(0.5 * bufferSize);
    fftData.resize(fftBinSize, 0);

    // Setup Beat slicing variables
    energyAvgStart = 0;
    timeOfLastDiscard = 0;
    avgEnergy = 0;
    instEnergy = 0;
    totalEnergy = 0;
    curEnergy = 0;
    numSamplesInTotalEnergyAvg = 0;
    numSamplesInCurEnergyAvg = 0;
    // For onset detection (beat slicing) we will only pay attention to frequencies less
    // than or equal to 600 Hz (i.e. mostly bass, some midrange; where majority of rhythm should lie)
    // Determine index of upper bound of frequency range
    int i = 0;
    while (((float)i * (float)sampleRate / (float)bufferSize) < 1200.0 && i < bufferSize) {
        i++;
    }
    onsetMaxFreqIndex = i;

    // Initialize fft object
    soundFFT = new fft(bufferSize);

    // Allocate memory for necessary FFT variables
    fftMagnitudes = (float *)malloc(fftBinSize * sizeof(float));
    memset(fftMagnitudes, 0, fftBinSize * sizeof(float));

    fftPhases = (float *)malloc(fftBinSize * sizeof(float));
    memset(fftPhases, 0, fftBinSize * sizeof(float));

    fftWindow = (float *)malloc(bufferSize * sizeof(float));
    memset(fftWindow, 0, bufferSize * sizeof(float));
    fft::genWindow(3, bufferSize, fftWindow);

    // Initialize soundstream input
    // TODO: This function can be called with 2 input channels if the user
    // has two audio inputs/sensors (one direct and one ambient). Support
    // for this needs to be added in a future version.

    //sound.setDeviceID(2);

    sound.setup(baseApp, 0, 1,sampleRate, bufferSize, 4);
}

/**
 * Sets Device ID to be used for SoundStream, then reinitializes SoundStream with default options.
 * Intended to be used only after initial setup call.
 */
void SoundAnalyzer::setDevice(int deviceId, ofBaseApp *baseApp, int inputChannels, int sampleRate, int bufferSize) {
    sound.setDeviceID(deviceId);
    sound.setup(baseApp, 0, 1,sampleRate, bufferSize, 4);
}

/**
 * Get the list of sound devices for user to choose from.
 */
vector<ofSoundDevice> SoundAnalyzer::getSoundDevices() {
    return sound.getDeviceList();
}

/**
 * The ofBaseApp redirects to this audioIn method for the ofSoundStream input.
 * This method will simply perform an FFT on the input and then call update() to
 * analyze the newest FFT data.
 */
void SoundAnalyzer::audioIn(float* input, int bufferSize, int nChannels) {

    // Perform FFT and get the power
    soundFFT->powerSpectrum(0, input, fftWindow, fftMagnitudes, fftPhases);

    // Call update method to update variables for sound analysis with newest FFT
    update();
}

/**
 * Get the current sound level, which is just an RMS of the most recent
 * unnormalized FFT data.
 */
float SoundAnalyzer::getSoundLevel() {
    return soundLevel;
}

/**
 * Get the pointer to the unnormalized FFT data. This data may change while accessing,
 * so doing a memcpy may be pertinent for lengthy calculations using this unnormalized
 * data.
 */
float * SoundAnalyzer::getRawFrequencies() {
    return fftMagnitudes;
}

/**
 * Get the vector array of normalized FFT data. Should return entire array, not just
 * a pointer.
 */
vector<float> SoundAnalyzer::getNormFrequencies() {
    return fftData;
}

/**
 * Get the current average energy of the sound.
 */
float SoundAnalyzer::getAvgEnergy() {
    return avgEnergy;
}

/**
 * Get the current highest energy of the sound.
 */
float SoundAnalyzer::getPeakEnergy() {
    return dataPeak;
}

/**
 * Current implementation of onset detection (beat slicing). Needs some tuning to get
 * right. TODO: Possibly set a timer so only one onset can be detected in a certain amount of
 * time (in order to emulate whatever appears to be the beat of the song).
 */
bool SoundAnalyzer::detectedOnset() {
    if ( 0.65 * instEnergy > avgEnergy)
        return true;
    return false;
}
