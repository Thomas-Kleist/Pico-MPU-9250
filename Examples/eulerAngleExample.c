#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "mpu9250.h"

int main()
{
    stdio_init_all();

    sleep_ms(10000);

    printf("Hello, MPU9250! Reading raw data from registers via SPI...\n");

    start_spi();  //Starts the mpu

    int16_t acceleration[3], gyro[3], gyroCal[3], eulerAngles[2], fullAngles[2]; //Declares the variables required for calculations
    absolute_time_t timeOfLastCheck;

    calibrate_gyro(gyroCal, 100);  //Calibrates the gyro

    mpu9250_read_raw_accel(acceleration);  //Sets the absolute angle using the direction of gravity
    calculate_angles_from_accel(eulerAngles, acceleration);
    timeOfLastCheck = get_absolute_time();

    while (1)
    {
        mpu9250_read_raw_accel(acceleration);  //Reads the accel and gyro 
        mpu9250_read_raw_gyro(gyro);
        gyro[0] -= gyroCal[0];  //Applies the calibration
        gyro[1] -= gyroCal[1];
        gyro[2] -= gyroCal[2];
        calculate_angles(eulerAngles, acceleration, gyro, absolute_time_diff_us(timeOfLastCheck, get_absolute_time()));  //Calculates the angles
        timeOfLastCheck = get_absolute_time();

        convert_to_full(eulerAngles, acceleration, fullAngles);

        printf("Acc. X = %d, Y = %d, Z = %d\n", acceleration[0], acceleration[1], acceleration[2]);  //Prints the angles
        printf("Gyro. X = %d, Y = %d, Z = %d\n", gyro[0] - gyroCal[0], gyro[1] - gyroCal[1], gyro[2] - gyroCal[2]);
        printf("Euler. Roll = %d, Pitch = %d\n", eulerAngles[0], eulerAngles[1]);
        printf("Full. Roll = %d, Pitch = %d\n", fullAngles[0], fullAngles[1]);
    }
}