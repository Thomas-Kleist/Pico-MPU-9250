#include "mpuObject.h"
#include <stdio.h>
#include "pico/stdlib.h"

mpu9250 mpu;

int main()
{
    stdio_init_all();

    sleep_ms(10000);

    printf("Hello, MPU9250! Reading raw data from registers via SPI...\n");

    while (1)
    {
        mpu.updateAngles();
        mpu.printData();

        sleep_ms(100);
    }
}