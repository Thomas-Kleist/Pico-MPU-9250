#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include <math.h>
#include "mpu9250.h"

/* Example code to talk to a MPU9250 MEMS accelerometer and gyroscope.
   
   NOTE: Ensure the device is capable of being driven at 3.3v NOT 5v. The Pico
   GPIO (and therefor SPI) cannot be used at 5v.

   You will need to use a level shifter on the I2C lines if you want to run the
   board at 5v.

   Connections on Raspberry Pi Pico board and a generic MPU9250 board, other
   boards may vary.

   GPIO 4 (pin 6) MISO/spi0_rx-> ADO on MPU9250 board
   GPIO 5 (pin 7) Chip select -> NCS on MPU9250 board
   GPIO 6 (pin 9) SCK/spi0_sclk -> SCL on MPU9250 board
   GPIO 7 (pin 10) MOSI/spi0_tx -> SDA on MPU9250 board
   3.3v (pin 36) -> VCC on MPU9250 board
   GND (pin 38)  -> GND on MPU9250 board

   Note: SPI devices can have a number of different naming schemes for pins. See
   the Wikipedia page at https://en.wikipedia.org/wiki/Serial_Peripheral_Interface
   for variations.
   The particular device used here uses the same pins for I2C and SPI, hence the
   using of I2C names
*/

#define PIN_MISO 4
#define PIN_CS 5
#define PIN_SCK 6
#define PIN_MOSI 7

#define SPI_PORT spi0
#define READ_BIT 0x80

void cs_select()
{
    asm volatile("nop \n nop \n nop");
    gpio_put(PIN_CS, 0); // Active low
    asm volatile("nop \n nop \n nop");
}

void cs_deselect()
{
    asm volatile("nop \n nop \n nop");
    gpio_put(PIN_CS, 1);
    asm volatile("nop \n nop \n nop");
}

void mpu9250_reset()
{
    // Two byte reset. First byte register, second byte data
    // There are a load more options to set up the device in different ways that could be added here
    uint8_t buf[] = {0x6B, 0x00};
    cs_select();
    spi_write_blocking(SPI_PORT, buf, 2);
    cs_deselect();
}

void read_registers(uint8_t reg, uint8_t *buf, uint16_t len)
{
    // For this particular device, we send the device the register we want to read
    // first, then subsequently read from the device. The register is auto incrementing
    // so we don't need to keep sending the register we want, just the first.

    reg |= READ_BIT;
    cs_select();
    spi_write_blocking(SPI_PORT, &reg, 1);
    sleep_ms(10);
    spi_read_blocking(SPI_PORT, 0, buf, len);
    cs_deselect();
    sleep_ms(10);
}

void mpu9250_read_raw_accel(int16_t accel[3]) { //Used to get the raw acceleration values from the mpu
    uint8_t buffer[6];

    // Start reading acceleration registers from register 0x3B for 6 bytes
    read_registers(0x3B, buffer, 6);

    for (int i = 0; i < 3; i++) {
        accel[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    }
}

void mpu9250_read_raw_gyro(int16_t gyro[3]) {  //Used to get the raw gyro values from the mpu
    uint8_t buffer[6];
    
    read_registers(0x43, buffer, 6);

    for (int i = 0; i < 3; i++) {
        gyro[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);;
    }
}

void calibrate_gyro(int16_t gyroCal[3], int loop)  //Used to calibrate the gyro. The gyro must be still while calibration happens
{
    int16_t temp[3];
    for (int i = 0; i < loop; i++)
    {
        mpu9250_read_raw_gyro(temp);
        gyroCal[0] += temp[0];
        gyroCal[1] += temp[1];
        gyroCal[2] += temp[2];
    }
    gyroCal[0] /= loop;
    gyroCal[1] /= loop;
    gyroCal[2] /= loop;
}

void calculate_angles(int16_t eulerAngles[2], int16_t accel[3], int16_t gyro[3], uint64_t usSinceLastReading) //Calculates angles based on the accelerometer and gyroscope. Requires usSinceLastReading to use the gyro.
{
    long hertz = 1000000/usSinceLastReading;
    
    if (hertz < 200)
    {
        calculate_angles_from_accel(eulerAngles, accel);
        return;
    }

    long temp = 1.l/(hertz * 65.5l);  

    eulerAngles[0] += gyro[0] * temp;
    eulerAngles[1] += gyro[1] * temp;

    eulerAngles[0] += eulerAngles[1] * sin(gyro[2] * temp * 0.1f);
    eulerAngles[1] -= eulerAngles[0] * sin(gyro[2] * temp * 0.1f);

    int16_t accelEuler[2];
    calculate_angles_from_accel(accelEuler, accel);

    eulerAngles[0] = eulerAngles[0] * 0.9996 + accelEuler[0] * 0.0004;
    eulerAngles[1] = eulerAngles[1] * 0.9996 + accelEuler[1] * 0.0004;
}

void calculate_angles_from_accel(int16_t eulerAngles[2], int16_t accel[3]) //Uses just the direction gravity is pulling to calculate angles.
{
    float accTotalVector = sqrt((accel[0] * accel[0]) + (accel[1] * accel[1]) + (accel[2] * accel[2]));

    float anglePitchAcc = asin(accel[1] / accTotalVector) * 57.296;
    float angleRollAcc = asin(accel[0] / accTotalVector) * -57.296;

    eulerAngles[0] = anglePitchAcc;
    eulerAngles[1] = angleRollAcc;
}

void convert_to_full(int16_t eulerAngles[2], int16_t accel[3], int16_t fullAngles[2]) //Converts from -90/90 to 360 using the direction gravity is pulling
{
    if (accel[1] > 0 && accel[2] > 0) fullAngles[0] = eulerAngles[0];
    if (accel[1] > 0 && accel[2] < 0) fullAngles[0] = 180 - eulerAngles[0];
    if (accel[1] < 0 && accel[2] < 0) fullAngles[0] = 180 - eulerAngles[0];
    if (accel[1] < 0 && accel[2] > 0) fullAngles[0] = 360 + eulerAngles[0];

    if (accel[0] < 0 && accel[2] > 0) fullAngles[1] = eulerAngles[1];
    if (accel[0] < 0 && accel[2] < 0) fullAngles[1] = 180 - eulerAngles[1];
    if (accel[0] > 0 && accel[2] < 0) fullAngles[1] = 180 - eulerAngles[1];
    if (accel[0] > 0 && accel[2] > 0) fullAngles[1] = 360 + eulerAngles[1];
}

void start_spi() //Starts the mpu and resets it
{
    spi_init(SPI_PORT, 500 * 1000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);

    mpu9250_reset();

        // See if SPI is working - interrograte the device for its I2C ID number, should be 0x71
    uint8_t id;
    read_registers(0x75, &id, 1);
    printf("I2C address is 0x%x\n", id);
}