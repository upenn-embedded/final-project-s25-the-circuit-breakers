/* 
 * File:   drums.c
 * Author: mosha
 *
 * Created on April 24, 2025, 3:15 PM
 */

#define F_CPU 16000000UL 
#include <avr/io.h>
#include <util/delay.h> 
#include "uart.h"
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

/*
 * 
 */
void Initialize() {
    // Setup for ADC (10bit = 0-1023)
    // Clear power reduction bit for ADC
    PRR0 &= ~(1 << PRADC);

    // Select Vref = AVcc
    ADMUX |= (1 << REFS0);
    ADMUX &= ~(1 << REFS1);

    // Set the ADC clock div by 128
    // 16M/128=125kHz
    ADCSRA |= (1 << ADPS0);
    ADCSRA |= (1 << ADPS1);
    ADCSRA |= (1 << ADPS2);

    // Select Channel ADC0 (pin C0)
    ADMUX &= ~(1 << MUX0);
    ADMUX &= ~(1 << MUX1);
    ADMUX &= ~(1 << MUX2);
    ADMUX &= ~(1 << MUX3);

    ADCSRA |= (1 << ADATE); // Autotriggering of ADC

    // Free running mode ADTS[2:0] = 000
    ADCSRB &= ~(1 << ADTS0);
    ADCSRB &= ~(1 << ADTS1);
    ADCSRB &= ~(1 << ADTS2);

    // Disable digital input buffer on ADC pin
    DIDR0 |= (1 << ADC0D);

    // Enable ADC
    ADCSRA |= (1 << ADEN);

    // Start conversion
    ADCSRA |= (1 << ADSC);
}

void bufferTransmitDF(uint8_t command, uint8_t p1, uint8_t p2) {
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

void drums_main_loop(int reading0, int reading1, int reading2, int prevSnare, int prevBass, int prevHH) {
    if (reading2 > 100 && prevHH <= 100) {
        if (reading2 < 300) {
            bufferTransmitDF(0x06, 0x00, 0x14);
        }
        else if (reading2 < 600) {
            bufferTransmitDF(0x06, 0x00, 0x19);
        }
        else {
            bufferTransmitDF(0x06, 0x00, 0x1E);
        }
        _delay_ms(60);
        bufferTransmitDF(0x03, 0x00, 0x02);
    }
    if (reading1 > 100 && prevSnare <= 100) {
        if (reading1 < 300) {
            bufferTransmitDF(0x06, 0x00, 0x14);
        }
        else if (reading1 < 600) {
            bufferTransmitDF(0x06, 0x00, 0x19);
        }
        else {
            bufferTransmitDF(0x06, 0x00, 0x1E);
        }
        _delay_ms(60);
        bufferTransmitDF(0x03, 0x00, 0x01);
    }
    if (reading0 > 100 && prevBass <= 100) {
        if (reading0 < 300) {
            bufferTransmitDF(0x06, 0x00, 0x14);
        }
        else if (reading0 < 600) {
            bufferTransmitDF(0x06, 0x00, 0x19);
        }
        else {
            bufferTransmitDF(0x06, 0x00, 0x1E);
        }
        _delay_ms(60);
        bufferTransmitDF(0x03, 0x00, 0x03);
    }
}

