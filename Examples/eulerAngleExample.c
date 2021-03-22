#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "mpu9250.h"

int main()
{
    stdio_init_all();

    sleep_ms(10000);

    printf("Hello, MPU9250! Reading raw data from registers via SPI...\n");

    start_spi();

    int16_t acceleration[3], eulerAngles[2], fullAngles[2];

    while (1)
    {
        mpu9250_read_raw(acceleration);
        calculate_angles(eulerAngles, acceleration);
        convert_to_full(eulerAngles, acceleration, fullAngles);

        printf("Acc. X = %d, Y = %d, Z = %d\n", acceleration[0], acceleration[1], acceleration[2]);
        printf("Euler. Roll = %d, Pitch = %d\n", eulerAngles[0], eulerAngles[1]);
        printf("Full. Roll = %d, Pitch = %d\n", fullAngles[0], fullAngles[1]);

        sleep_ms(100);
    }

    return 0;
}