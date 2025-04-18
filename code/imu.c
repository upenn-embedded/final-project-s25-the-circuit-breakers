#include "imu.h"
#include "i2c.h"

void ICM20948_init(uint8_t addr) {
    // Wake device up
    I2C_writeRegister(addr << 1, 0x01, PWR_MGMT_1);
}

uint8_t ICM20948_whoami(uint8_t addr) {
    uint8_t data;
    I2C_readRegister(addr << 1, &data, WHO_AM_I_REG);
    return data;
}

void ICM20948_readAccel(int16_t* ax, int16_t* ay, int16_t* az) {
    uint8_t data[6];
    I2C_readCompleteStream(data, ICM20948_ADDR << 1, ACCEL_XOUT_H, 6);
    *ax = (int16_t)((data[0] << 8) | data[1]);
    *ay = (int16_t)((data[2] << 8) | data[3]);
    *az = (int16_t)((data[4] << 8) | data[5]);
}

void ICM20948_readGyro(int16_t* gx, int16_t* gy, int16_t* gz) {
    uint8_t data[6];
    I2C_readCompleteStream(data, ICM20948_ADDR << 1, GYRO_XOUT_H, 6);
    *gx = (int16_t)((data[0] << 8) | data[1]);
    *gy = (int16_t)((data[2] << 8) | data[3]);
    *gz = (int16_t)((data[4] << 8) | data[5]);
}
