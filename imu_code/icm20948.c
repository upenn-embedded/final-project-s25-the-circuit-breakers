#include "i2c.h"
#include "icm20948.h"

void ICM20948_setBank(uint8_t bank) {
    I2C_start();
    I2C_write((ICM20948_ADDR << 1) | 0); // write
    I2C_write(BANK_SEL_REG);
    I2C_write(bank << 4); // bits 7:4 define the bank
    I2C_stop();
}

uint8_t ICM20948_readReg(uint8_t bank, uint8_t reg) {
    ICM20948_setBank(bank);
    I2C_start();
    I2C_write((ICM20948_ADDR << 1) | 0); // write
    I2C_write(reg);
    I2C_start();
    I2C_write((ICM20948_ADDR << 1) | 1); // read
    uint8_t val = I2C_read_nack();
    I2C_stop();
    return val;
}

void ICM20948_writeReg(uint8_t bank, uint8_t reg, uint8_t data) {
    ICM20948_setBank(bank);
    I2C_start();
    I2C_write((ICM20948_ADDR << 1) | 0);
    I2C_write(reg);
    I2C_write(data);
    I2C_stop();
}

void ICM20948_init(void) {
    I2C_init();
    ICM20948_writeReg(0, PWR_MGMT_1, 0x01); // Auto selects clock
    ICM20948_writeReg(0, PWR_MGMT_2, 0x00);
}
