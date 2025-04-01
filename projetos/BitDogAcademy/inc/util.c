#include "util.h"

float angle_difference(float angle1, float angle2) {
    float diff = fmodf(fabsf(angle1 - angle2), 2*M_PI);
    return (diff > M_PI) ? (2*M_PI - diff) : diff;
}

int clamp(int x, int lower, int upper) {
    if(x < lower) return lower;
    if(x > upper) return upper;
    return x;
}

float clampf(float x, float lower, float upper) {
    if(x < lower) return lower;
    if(x > upper) return upper;
    return x;
}
