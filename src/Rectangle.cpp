#include "include/Rectangle.h"

#define MAX_ANGLE 180
#define MAX_LIFETIME 50
#define STATE_ALIVE 1
#define STATE_DEAD 0

Rectangle::Rectangle() {
    x = 0.0;
    y = 0.0;
    h = 0.0;
    s = 200.0;
    b = 255.0;
    angle = 0;
    angle_dir = 1;
}
Rectangle::Rectangle(float n_x,float n_y,float n_h,float n_s,float n_b) {
    x = n_x;
    y = n_y;
    h = n_h;
    s = n_s;
    b = n_b;
    angle = 0;
    angle_dir = 1;
}
void Rectangle::display(float scale) {
    ofSetColor(ofColor::fromHsb(h,s,b));
    ofRect(0,0,scale*SQUARE_SIZE,scale*SQUARE_SIZE);
}
void Rectangle::update() {

}
float Rectangle::get_x() {
    return x;
}

float Rectangle::get_y() {
    return y;
}

float Rectangle::get_h() {
    return h;
}

float Rectangle::get_s() {
    return s;
}

float Rectangle::get_b() {
    return b;
}
int Rectangle::get_angle() {
    return angle;
}
void Rectangle::set_x(float new_x) {
    x = new_x;
}

void Rectangle::set_y(float new_y) {
    y = new_y;
}

void Rectangle::set_h(float new_h) {
    h = new_h;
}

void Rectangle::set_s(float new_s) {
    s = new_s;
}

void Rectangle::set_b(float new_b) {
    b = new_b;
}
void Rectangle::add_angle(int angle_rate) {
    angle += angle_dir*angle_rate;
    if(angle >= 360) {
        angle_dir = -1;
    }
    if(angle <= 0) {
        angle_dir = 1;
    }
}
