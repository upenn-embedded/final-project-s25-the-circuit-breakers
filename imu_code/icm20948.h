#ifndef ICM20948_H
#define ICM20948_H

#include <stdint.h>

#define ICM20948_ADDR 0x68 // or 0x69 depending on AD0
#define WHO_AM_I_REG  0x00
#define PWR_MGMT_1    0x06
#define PWR_MGMT_2    0x07
#define USER_CTRL     0x03
#define BANK_SEL_REG  0x7F

void ICM20948_init(void);
void ICM20948_setBank(uint8_t bank);
uint8_t ICM20948_readReg(uint8_t bank, uint8_t reg);
void ICM20948_writeReg(uint8_t bank, uint8_t reg, uint8_t data);

#endif
