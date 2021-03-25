#include "mpuObject.h"
extern "C" {
    #include "mpu9250.h"
}

mpu9250::mpu9250(int loop) //Starts the mpu and calibrates the gyro
{
    start_spi();
    calibrate_gyro(gyroCal, loop);
    mpu9250_read_raw_accel(acceleration);
    calculate_angles_from_accel(eulerAngles, acceleration);
    timeOfLastCheck = get_absolute_time();
}

void mpu9250::updateAngles() //Calculates the angles based on the sensor readings
{
    mpu9250_read_raw_accel(acceleration);
    mpu9250_read_raw_gyro(gyro);
    gyro[0] -= gyroCal[0];
    gyro[1] -= gyroCal[1];
    gyro[2] -= gyroCal[2];
    calculate_angles(eulerAngles, acceleration, gyro, absolute_time_diff_us(timeOfLastCheck, get_absolute_time()));
    timeOfLastCheck = get_absolute_time();

    convert_to_full(eulerAngles, acceleration, fullAngles);
}

void mpu9250::printData() //Prints out the sensor readings and calculated values
{
    printf("Acc. X = %d, Y = %d, Z = %d\n", acceleration[0], acceleration[1], acceleration[2]);
    printf("Gyro. X = %d, Y = %d, Z = %d\n", gyro[0] - gyroCal[0], gyro[1] - gyroCal[1], gyro[2] - gyroCal[2]);
    printf("Euler. Roll = %d, Pitch = %d\n", eulerAngles[0], eulerAngles[1]);
    printf("Full. Roll = %d, Pitch = %d\n", fullAngles[0], fullAngles[1]);
}