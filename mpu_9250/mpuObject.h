#include "pico/stdlib.h"

#ifndef mpuObject_h
#define mpuObject_h

class mpu9250
{
    public: 
    int16_t acceleration[3], gyro[3], gyroCal[3], eulerAngles[2], fullAngles[2];
    absolute_time_t timeOfLastCheck;

    mpu9250(int loop);
    void updateAngles();
    void printData();
};

#endif