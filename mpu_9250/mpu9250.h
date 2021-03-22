#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

#ifndef mpu9250_h
#define mpu9250_h

void mpu9250_reset();
void read_registers(uint8_t reg, uint8_t *buf, uint16_t len);
void mpu9250_read_raw(int16_t accel[3]);
void calculate_angles(int16_t eulerAngles[2], int16_t accel[3]);
void start_spi();
void convert_to_full(int16_t eulerAngles[2], int16_t accel[3], int16_t fullAngles[2]);

#endif