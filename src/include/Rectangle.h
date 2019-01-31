#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "ofMain.h"

#define SQUARE_SIZE 80

class Rectangle {
    float x, y, h, s, b;
    int angle, angle_dir;

    public:
        Rectangle();
        Rectangle(float,float,float,float,float);
        void display(float);
        void update();
        float get_x();
        float get_y();
        float get_h();
        float get_s();
        float get_b();
        int get_angle();
        void set_x(float);
        void set_y(float);
        void set_h(float);
        void set_s(float);
        void set_b(float);
        void add_angle(int);
};
#endif
