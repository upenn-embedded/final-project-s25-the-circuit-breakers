#define F_CPU 16000000UL

#include <avr/io.h>
#include "i2c.h"
#include <util/twi.h>

#define F_SCL 400000UL // 400kHz
#define Prescaler 1
#define TWBR_val ((((F_CPU / F_SCL) / Prescaler) - 16) / 2)

void I2C_init(void) {
    TWSR0 = 0x00; // Prescaler = 1
    TWBR0 = (uint8_t)TWBR_val;
}

void I2C_start(void) {
    TWCR0 = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
    while (!(TWCR0 & (1 << TWINT)));
}

void I2C_stop(void) {
    TWCR0 = (1 << TWSTO) | (1 << TWEN) | (1 << TWINT);
    while (TWCR0 & (1 << TWSTO));
}

uint8_t I2C_write(uint8_t data) {
    TWDR0 = data;
    TWCR0 = (1 << TWEN) | (1 << TWINT);
    while (!(TWCR0 & (1 << TWINT)));
    return (TWSR0 & 0xF8);
}

uint8_t I2C_read_ack(void) {
    TWCR0 = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
    while (!(TWCR0 & (1 << TWINT)));
    return TWDR0;
}

uint8_t I2C_read_nack(void) {
    TWCR0 = (1 << TWINT) | (1 << TWEN);  // no TWEA
    while (!(TWCR0 & (1 << TWINT)));
    TWCR0 = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); 
    return TWDR0;
}
