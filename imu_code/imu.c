#include "i2c.h"
#include "icm20948.h"
#include "imu.h"

#include <stdio.h>
#include "uart.h"
#include <avr/io.h>
#include <stdarg.h>
#include <string.h>

uint8_t setup_imu() {
    imu_timer_init();
    _delay_ms(3000);
    ICM20948_init();
    _delay_ms(10);
   
    return ICM20948_readReg(0, WHO_AM_I_REG);
}

void imu_timer_init() {
    TCCR1A = 0;
    TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10); // CTC, prescaler 64
    OCR1A = 2500 - 1;  // (16e6 / 64) * 0.01s = 2500
    TIMSK1 |= (1 << OCIE1A);  // Enable interrupt
    TCNT1 = 0;
}

int16_t read_imu(uint8_t regH, uint8_t regL) {
    uint8_t hi = ICM20948_readReg(0, regH);
    uint8_t lo = ICM20948_readReg(0, regL);
//    printf("hi: %d, lo: %d", hi, lo);
    return (int16_t)((hi << 8) | lo);
}

void read_accel_gyro(IMU_Data* data) {
    data->ax = read_imu(0x2D, 0x2E); // 16384.0 * 9.81 to convert to m / s^2
    data->ay = read_imu(0x2F, 0x30);
    data->az = read_imu(0x31, 0x32);

    data->gx = read_imu(0x33, 0x34);
    data->gy = read_imu(0x35, 0x36);
    data->gz = read_imu(0x37, 0x38);
}

void calibrate_imu(IMU_Data* offset) {
    float iters = 100.0;
    IMU_Data data;
    
    for (int i = 0; i < iters; i++) {
        read_accel_gyro(&data);
        offset->ax += data.ax;
        offset->ay += data.ay;
        offset->az += data.az;
        offset->gx += data.gx;
        offset->gy += data.gy;
        offset->gz += data.gz;
        _delay_ms(10);
    }
    
    offset->ax /= iters;
    offset->ay /= iters;
    offset->az /= iters;
    offset->gx /= iters;
    offset->gy /= iters;
    offset->gz /= iters;
}

void imu_uart_init() {
    /*Set baud rate */
    UBRR1H = (unsigned char)(UART_BAUD_PRESCALER>>8);
    UBRR1L = (unsigned char)UART_BAUD_PRESCALER;
    //Enable receiver and transmitter
    UCSR1B = (1<<RXEN1)|(1<<TXEN1);
    /* Set frame format: 2 stop bits, 8 data bits */
    UCSR1C = (1<<UCSZ11) | (1<<UCSZ10); // 8 data bits
    UCSR1C |= (1<<USBS1); // 2 stop bits
    
//    __init_stdout(uart_send);
//    __init_stdin(uart_receive);
}

void bufferTransmitDF(uint8_t command, uint8_t p1, uint8_t p2) {
    // 0001-0009 in order of increasing speed
    int sendBuffer[10] = {0x7E, 0xFF, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEF};
    sendBuffer[3] = command;
    sendBuffer[5] = p1;
    sendBuffer[6] = p2;
    uint16_t checksum = 0xFFFF - (0xFF + 0x06 + command + 0x00 + p1 + p2) + 1;
    sendBuffer[7] = (checksum >> 8);
    sendBuffer[8] = (checksum & 0xFF);
    for (int i = 0; i < 10; i++) {
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = sendBuffer[i];
    }
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
