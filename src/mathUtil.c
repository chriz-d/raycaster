#include <math.h>
#include "mathUtil.h"

#ifndef M_PI 
#define M_PI 3.1415926535 
#endif

float addAngle(float angle, float rotationVal) {
    angle += rotationVal;
    if(angle >= 360) {
        angle -= 360;
    } else if(angle < 0) {
        angle += 360;
    }
    return angle;
}

float tanDeg(double val) {
    float result = val * M_PI/ 180.0;
    return tan(result);
}

float cosDeg(double val) {
    float result = val * M_PI/ 180.0;
    return cos(result);
}

float sinDeg(double val) {
    float result = val * M_PI/ 180.0;
    return sin(result);
}