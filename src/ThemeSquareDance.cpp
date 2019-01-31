#include "include/ThemeSquareDance.h"

ThemeSquareDance::ThemeSquareDance() {
    // defined for c++ standards
}

ThemeSquareDance::ThemeSquareDance(SoundAnalyzer* sndAnly) {
    soundAnly = sndAnly;
}

void ThemeSquareDance::setSize(int width, int height) {
    synthWidth = width;
    synthHeight = height;
}

const char* ThemeSquareDance::getName() {
    return "SquareDance";
}

void ThemeSquareDance::setup() {
    ofBackground(ofColor::black);

    soundLevel = 0;

    width_spacer = 0;
    height_spacer = 0;

    hue = 0;
    rate = 0;


    for(int i = 0; i < WIDTH; i++) {
        for(int j = 0; j < HEIGHT; j++) {
            rects[i][j] = Rectangle();
        }
    }

    place_x = 0;
    place_y = 0;

    scale = 1.0;
    background = 0;
    back_dir = 1;

    //TODO load from XML

    // allows senSAE to display in GUI
    showTheme = true;
}

void ThemeSquareDance::draw() {
    for(int i = 0; i < WIDTH; i++) {
        for(int j = 0; j < HEIGHT; j++) {
            ofPushMatrix();
            ofTranslate(i*SQUARE_SIZE + width_spacer*(i+1), j*SQUARE_SIZE + height_spacer*(j+1));
            ofRotate(rects[i][j].get_angle(), 1, 1, 1);
            rects[i][j].display(scale);

            ofPopMatrix();
        }
    }
}

void ThemeSquareDance::update() {
    soundLevel = soundAnly->getSoundLevel();

    hue = ofMap(soundLevel, 0, 1, 0, 3, true);
    rate = ofMap(soundLevel, 0, 1, -1, 6, true);
    
    ofSetColor(0, 0, 0, 5);
    ofRect(0,0, synthWidth, synthHeight);

    for(int i = 0; i < WIDTH; i++) {
        for(int j = 0; j < HEIGHT; j++) {
            rects[i][j].add_angle(rate);
            float color = hue + rects[i][j].get_h();
            if(color > 255) {
                color = 0;
            } else if (color < 0) {
                color = 255;
            }
            rects[i][j].set_h(color);
        }
    }

    float bri = ofMap(soundLevel, 0, 1, 0, 1, true);

//    background += bri*back_dir;
//    if(background > 255) {
//        back_dir = -1;
//    } else if (background < 0) {
//        back_dir = 1;
//    }
//
//    ofBackground(ofColor::fromHsb(0,0,background));
}

bool ThemeSquareDance::isReady() {
    return showTheme;
}
