#include "include/ThemeCircles.h"

ThemeCircles::ThemeCircles() {
    // defined for c++ standards
}

ThemeCircles::ThemeCircles(SoundAnalyzer* sndAnly) {
    soundAnly = sndAnly;
}

void ThemeCircles::setSize(int width, int height) {
    synthWidth = width;
    synthHeight = height;
}

const char* ThemeCircles::getName() {
    return "Circles";
}

void ThemeCircles::setup() {
    ofBackground(ofColor::black);
    //ofSetBackgroundAuto(false);
    
    radius = 100;

    xOff = ofRandom(0, .1);

    numCirclesInRow = 2;

    // allows senSAE to display in GUI
    showTheme = true;
}

void ThemeCircles::draw() {
    ofSetColor(0, 0, 0, 5);
    ofRect(0,0, synthWidth, synthHeight);

    drawGrid();


    if(multiplyLevel >= 5){
        ofPushMatrix();
        ofTranslate(synthWidth/2, synthHeight/2);
        drawCircles();
        ofPopMatrix();
    }
}

void ThemeCircles::update() {
    float level = 4*soundAnly->getSoundLevel();
    float newRad = ofMap(level, 0, 7, 25, 250, true);
    radius = radius + 0.4 * (newRad - radius);
    //ofLog(OF_LOG_NOTICE, "radius: %f", radius);

    brightness = ofMap(level, 0, 7, 150, 255, true);
    multiplyLevel = ofMap(level, 0, 7, 2, 4, true);

    xOff += ofMap(level, 0, 7, 0, .1, true);
    noiseResult = ofNoise(xOff);


    hue = ofMap(noiseResult, 0, 1, -200, 300, true);
    posXOff = ofMap(noiseResult, 0, 1, 0, synthWidth, true);
    posYOff = ofMap(noiseResult, 0, 1, -synthHeight*.25, synthHeight, true);

    rotation += 10;

    gridCountUpdate();
}

bool ThemeCircles::isReady() {
    return showTheme;
}

void ThemeCircles::drawGrid(){
    int translateX = synthWidth/(2 * numCirclesInRow);
    int translateY = synthHeight/(2 * numCirclesInRow);

    for(int i = 0; i < numCirclesInRow; i++){
        for(int j = 0; j < numCirclesInRow; j++){
            ofPushMatrix();
            ofTranslate(translateX, translateY);
            translateX += 2 * synthWidth/(2 * numCirclesInRow);
            ofSetColor(ofColor::fromHsb(hue, brightness, brightness));
            drawCircles();
            ofPopMatrix();
        }
        translateY += 2 * synthHeight/(2 * numCirclesInRow);
        translateX = synthWidth/(2 * numCirclesInRow);
    }

}

void ThemeCircles::drawCircles(){
    ofSetColor(ofColor::fromHsb(hue, brightness, brightness));
    ofSetLineWidth(.25);
    //float centerOfCircleRadius = ofMap(noiseResult, 0, 1, .1, .5, true);
    for(float multiplier = .5; multiplier <= multiplyLevel; multiplier *= 4){
        drawSingleCircle(multiplier);
    }

}

void ThemeCircles::drawSingleCircle(float multiplier){
    bool fill = false;
    float radiusMultiplier = ofMap(noiseResult, 0, 1, 1, 3, true);
    float spacing = 15 - ofMap(noiseResult, 0, 1, 1, 5, true);
    for(float ang = 0; ang <= 360; ang+=5){
        float rad = ofDegToRad(ang);
        float x = ((radius * 4 * multiplier) * cos(rad));
        float y = ((radius * 4 * multiplier) * sin(rad));
        ofPushMatrix();
        ofRotate(rotation);

        if(fill){
            ofNoFill();
            fill = false;
        }else{
            ofFill();
            fill = true;
        }

        //ofEllipse(x, y, 10, 10);
        ofRect(x, y, 10, 10);
        ofPopMatrix();
    }
}

void ThemeCircles::gridCountUpdate(){
    float updateMinimum = ofMap(noiseResult, 0, 1, 0, 50, true);    //lowers the 'gate' to try and change number of circles when the sound is louder

    if(ofRandom(100) > 100 - updateMinimum/2){

        float changeNumCircles = ofRandom(100);
        if(changeNumCircles > 100 - updateMinimum){
            numCirclesInRow += 1;
            if(numCirclesInRow > 4)
                numCirclesInRow = 4;
        }else if(changeNumCircles < updateMinimum+30){
            numCirclesInRow -= 1;
            if(numCirclesInRow < 1)
                numCirclesInRow = 1;
        }
    }
}
