#include "pico/stdlib.h"

#ifndef mpuObject_h
#define mpuObject_h

class mpu9250
{
    public: 
    int16_t acceleration[3], eulerAngles[2], fullAngles[2];

    mpu9250();
    void updateAngles();
    void printData();
};

#endif