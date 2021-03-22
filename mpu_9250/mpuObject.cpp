#include "mpuObject.h"
extern "C" {
    #include "mpu9250.h"
}

mpu9250::mpu9250()
{
    start_spi();
}

void mpu9250::updateAngles()
{
    mpu9250_read_raw(acceleration);
    calculate_angles(eulerAngles, acceleration);
    convert_to_full(eulerAngles, acceleration, fullAngles);
}

void mpu9250::printData()
{
    printf("Acc. X = %d, Y = %d, Z = %d\n", acceleration[0], acceleration[1], acceleration[2]);
    printf("Euler. Roll = %d, Pitch = %d\n", eulerAngles[0], eulerAngles[1]);
    printf("Full. Roll = %d, Pitch = %d\n", fullAngles[0], fullAngles[1]);
}