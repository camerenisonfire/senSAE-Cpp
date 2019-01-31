#include "ThemeSwirley.h"
#include "ofMain.h"

ThemeSwirley::ThemeSwirley() {
    // Do we need to do anything here?
}

ThemeSwirley::ThemeSwirley(SoundAnalyzer* sndAnly) {
    soundAnly = sndAnly;
}

void ThemeSwirley::setSize(int width, int height) {
    synthWidth = width;
    synthHeight = height;
}

const char* ThemeSwirley::getName() {
    return "Swirley";
}

void ThemeSwirley::setup() {
    countX.setup("countX", 35, 0, 200);
    stepX.setup("stepX", 58, 0, 200);
    twistX.setup("twistX", -7.65, -45, 45);
    countY.setup("countY", 25, 0, 50);
    stepY.setup("stepY", 0, 0, 200);
    twistY.setup("twistY", 13.8, -30, 30);
    pinchY.setup("pinchY", 0, 0, 1);
    Scale.setup("Scale", 0.770408, 0.0, 1);
    Background.setup("Background", 0, 0, 255);
    shiftY.setup("shiftY", -234.694, -1000.0, 1000.0);
    rotate.setup("rotate", -134.082, -180.0, 180.0);
    size.setup("size", ofVec2f(1.04956,0), ofVec2f(0,0), ofVec2f(20,20));
    filled.setup("filled", false);
    type.setup("type", false);
    
    for (int i = 0; i < 200; i++) {
        rColor[i] = -1;
        bColor[i] = -1;
        gColor[i] = -1;
    }
    ofSeedRandom(ofGetUnixTime());

    wholeRotate = 0;
    twistXchange = -7.65;
    twistXChanger = 0.0001;
    prevLineLength = 15;
    onsetDecayRate = 200; // # of milliseconds after an onset to respond to it
    timeOfLastOnset = 0;
    ofBackground(Background);

    //TODO Allow user to load settings from XML file

    // Set show GUI to true at the end so senSAE knows that
    // the theme is ready to be displayed
    showTheme = true;
}

void ThemeSwirley::update() {
    ofSetColor(0, 0, 0, 5);
    ofRect(0,0, synthWidth, synthHeight);

}

void ThemeSwirley::draw() {

    //---------------
    ofPushMatrix();
    ofTranslate(synthWidth / 2, synthHeight / 2);

    float Scl = pow(Scale, 4.0f);
    ofScale(Scl, Scl);
    float accelFactor = soundAnly->getSoundLevel()/soundAnly->getAvgEnergy();
    if (accelFactor > 1) {
        wholeRotate += accelFactor;
    } else {
        wholeRotate++;
    }
    if (wholeRotate > 180) {
        wholeRotate = -180;
    }
    ofRotate(wholeRotate);

    matrixPattern();

    ofPopMatrix();
    //--------------
}

bool ThemeSwirley::isReady() {
    return showTheme;
}

void ThemeSwirley::matrixPattern() {
    currentSoundAnalysis = *soundAnly;
    unsigned long timeSinceLastOnset = ofGetElapsedTimeMillis() - timeOfLastOnset;

    if (currentSoundAnalysis.detectedOnset() && timeSinceLastOnset > onsetDecayRate) {
        timeOfLastOnset = ofGetElapsedTimeMillis();
        timeSinceLastOnset = ofGetElapsedTimeMillis() - timeOfLastOnset;
    }
    
    int thisRcolor = -1, thisBcolor = -1;
    if (timeSinceLastOnset < onsetDecayRate) {
        thisRcolor = ofMap(-(float)timeSinceLastOnset, -(float)onsetDecayRate, 0, 125, 255);
        thisBcolor = ofMap((float)timeSinceLastOnset, 0, (float)onsetDecayRate, 0, 255);
        int randomX = round(ofRandom(countX-1));
        rColor[randomX] = thisRcolor; rColor[randomX+1] = thisRcolor;
        bColor[randomX] = thisBcolor; bColor[randomX+1] = thisBcolor;
    }

    lineLength = ofMap(currentSoundAnalysis.getSoundLevel(), 0, currentSoundAnalysis.getPeakEnergy(), 10, 62.5);
    if (lineLength > prevLineLength) {
        lineLength = prevLineLength + 0.5;
    } else {
        lineLength = prevLineLength - 1.5;
        if (lineLength < 10) lineLength = 10;
    }
    prevLineLength = lineLength;

    for(int y = 0; y<= countY; y++){
        //        if (normData[countFreqBin] < 0.3) {
        //            countFreqBin += 3;
        //            continue;
        //        } else {
        //            countFreqBin += 3;
        //        }
        ofPushMatrix();
        //---------------
        if(countY > 0){
            float sc1 = ofMap(y, -countY, countY, 1-pinchY, 1);
            ofScale(sc1, sc1);
        }
        ofTranslate(0, y * stepY);
        ofRotate(y * twistY);
        
        spiralPattern();
        //--------------
        ofPopMatrix();
    }
}

void ThemeSwirley::spiralPattern() {

    //ofSetLineWidth(ofMap(currentSoundAnalysis.getSoundLevel(), 0, currentSoundAnalysis.getPeakEnergy() / 2, 1, 8));
    ofSetLineWidth(3);

    twistXchange = twistXchange + twistXChanger;
    twistXChanger += twistXChanger;
    // Keep twisting between -38 and 30 for this theme/prototype
    if (twistXchange < -35) {
        twistXChanger = 0.0001;
    } else if (twistXchange > 27) {
        twistXChanger = -0.0001;
    }

    // Make sure twistXChanger does not exceed 1.2
    if (twistXChanger > 0.008) {
        twistXChanger = 0.008;
    } else if (twistXChanger < -0.008) {
        twistXChanger = -0.008;
    }

    if (filled) ofFill(); else ofNoFill();
    for(int i = 0; i <= countX; i++){
        ofPushMatrix();
        ofTranslate(i * stepX, 0);
        ofRotate(i * twistXchange);

        ofTranslate(0, shiftY);
        ofRotate(rotate);
        ofScale(size->x, size->y);
        if (rColor[i] >= 0 || bColor[i] >= 0) {
            ofSetColor(rColor[i], 0, bColor[i], 255);
            rColor[i] -= 0.01;
            bColor[i] += 0.01;
            if (rColor[i] < 125 || bColor[i] > 255) {
                rColor[i] = -1;
                bColor[i] = -1;
            }
        } else {
            ofSetColor(125, 0, 255, 255);
        }
        ofLine(-lineLength, -lineLength, lineLength, lineLength);

        ofPopMatrix();
    }

}
