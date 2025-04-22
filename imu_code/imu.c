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

void dfPlayerPlay(uint16_t track) {
    int sendBuffer[10] = {0x7E, 0xFF, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEF};
    sendBuffer[3] = 0x03;
    sendBuffer[5] = track >> 8;
    sendBuffer[6] = track & 0xFF;
    uint16_t checksum = 0xFFFF - (0xFF + 0x06 + 0x03 + 0x00 + sendBuffer[5] + sendBuffer[6]) + 1;
    sendBuffer[7] = (checksum >> 8);
    sendBuffer[8] = (checksum & 0xFF);
    for (int i = 0; i < 10; i++) { //sends over USART1
        while (!(UCSR1A & (1 << UDRE1)));
        UDR1 = sendBuffer[i];
    }
}

void dfPlayerVolume(uint16_t volume) {
    int sendBuffer[10] = {0x7E, 0xFF, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEF};
    sendBuffer[3] = 0x06;
    sendBuffer[5] = volume >> 8;
    sendBuffer[6] = volume & 0xFF;
    uint16_t checksum = 0xFFFF - (0xFF + 0x06 + 0x06 + 0x00 + sendBuffer[5] + sendBuffer[6]) + 1;
    sendBuffer[7] = (checksum >> 8);
    sendBuffer[8] = (checksum & 0xFF);
    for (int i = 0; i < 10; i++) { //sends over USART1
        while (!(UCSR1A & (1 << UDRE1)));
        UDR1 = sendBuffer[i];
    }
}
