#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

#ifndef mpu9250_h
#define mpu9250_h

void mpu9250_reset();
void read_registers(uint8_t reg, uint8_t *buf, uint16_t len);
void mpu9250_read_raw_accel(int16_t accel[3]);
void mpu9250_read_raw_gyro(int16_t gyro[3]);
void calculate_angles_from_accel(int16_t eulerAngles[2], int16_t accel[3]);
void calculate_angles(int16_t eulerAngles[2], int16_t accel[3], int16_t gyro[3], uint64_t usSinceLastReading);
void calibrate_gyro(int16_t gyroCal[3], int loop);
void start_spi();
void convert_to_full(int16_t eulerAngles[2], int16_t accel[3], int16_t fullAngles[2]);

#endif