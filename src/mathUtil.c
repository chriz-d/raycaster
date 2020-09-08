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
    float result = tan(val);
    return result * 180 / M_PI;
}
