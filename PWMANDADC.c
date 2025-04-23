/* 
 * File:   main.c
 * Author: zed
 *
 * Created on September 11, 2024, 10:29 AM
 */

#define F_CPU 16000000UL 
#include <avr/io.h>
#include <util/delay.h> 
#include "uart.h"
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>


// piano keys pins
#define SW PD5 //when pressed HIGH
#define DT PD6 //B
#define CLK PD7 //A
#define OCTAVE_DOWN PD3 //
#define OCTAVE_UP PD4
#define BUZZER PB1

// modify duty cycle for volume

uint8_t lastState;
uint8_t currentState;
uint8_t counter = 0; //rotary encoder reading
uint8_t octave = 1; // starts at Octave 2

// encoding for piano keys
const uint16_t note_freqs[4][8] = {
    {130, 147, 165, 175, 196, 220, 247, 261}, // Octave 1
    {261, 294, 329, 349, 392, 440, 493, 523}, // Octave 2 (default)
    // C   A   E    F    G     A    C   C
    {523, 587, 659, 698, 784, 880, 987, 1046}, // Octave 3
    {1046, 1175, 1318, 1396, 1567, 1760, 1975, 2093} // Octave 4
};

void rotary_init() {
    // Set PD5, PD6, PD7 as input
    DDRD &= ~(1 << SW);
    DDRD &= ~(1 << DT);
    DDRD &= ~(1 << CLK);

    
    // Optional: Enable pull-up resistors
    PORTD |= (1 << SW) | (1 << DT) | (1 << CLK);
}


void pwm_init() {    
    // BUZZER INITIALIZATION
    DDRB |= (1 << BUZZER);
    TCCR1A = (1 << COM1A0);
    TCCR1B = (1 << WGM12) | (1 << CS11);
}

void play_note(uint16_t freq, uint16_t duration_ms) {
    printf("playing note: %d Hz\n", freq);

    uint32_t top = (F_CPU / (2UL * 8 * freq)); // Prescaler = 8

    if (top > 65535) return; // Out of range for 16-bit timer

    OCR1A = (uint16_t)top;

    TCCR1A |= (1 << COM1A0); // Enable toggling

    for (uint16_t i = 0; i < duration_ms; i++) {
        _delay_ms(1);
    }

    TCCR1A &= ~(1 << COM1A0); // Disable buzzer
}

void button_init() {
    DDRD &= ~(1 << OCTAVE_UP);
    DDRD &= ~(1 << OCTAVE_DOWN);
    PORTD |= (1 << OCTAVE_UP); // enable pull-ups
    PORTD |= (1 << OCTAVE_DOWN);
}

//previous variables for drum parts
volatile int prevSnare = 0;
volatile int prevBass = 0;
volatile int prevHH = 0;

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

void play_track(uint16_t track) {
    bufferTransmitDF(0x03, track >> 8, track & 0xFF);
}


//0001 is snare drum, 0002 is hi-hat, and 0003 is bass
int main(void) {
    Initialize();
    uart_init();
    
    rotary_init();
    pwm_init();
    button_init();
    octave = 1;
    printf("octave %d\n", octave);
    
    // get previous state to compare against
    lastState = (PIND & (1 << CLK)) >> CLK;
    
    while (1) {
        ADMUX &= ~(1 << MUX0);
        ADMUX &= ~(1 << MUX1);
        ADMUX &= ~(1 << MUX2);
        ADMUX &= ~(1 << MUX3);
        int reading0 = ADC;
        printf("Hi-Hat: %d \n", reading0);
        
        ADMUX |= (1 << MUX0);
        ADMUX &= ~(1 << MUX1);
        ADMUX &= ~(1 << MUX2);
        ADMUX &= ~(1 << MUX3);
        int reading1 = ADC;
        printf("Snare Drum: %d \n", reading1);
        
        ADMUX &= ~(1 << MUX0);
        ADMUX |= (1 << MUX1);
        ADMUX &= ~(1 << MUX2);
        ADMUX &= ~(1 << MUX3);
        int reading2 = ADC;
        printf("Bass Drum: %d \n\n", reading2);
        
        //code for pwm
        currentState = (PIND & (1 << CLK)) >> CLK;
        
        if (currentState != lastState) {
            // Only act on the rising edge
            if (currentState == 1) {
                if ((PIND & (1 << DT)) >> DT) {
                    counter = (counter + 1) % 8; // wrap 0-7
//                    counter = (counter + 1) % 20;
                } else {
//                    counter = (counter == 0) ? 19 : counter - 1;
                    counter = (counter == 0) ? 7 : counter - 1; // wrap around 0 to 7
                }
                printf("Counter: %d\n", counter);
            }
        }
        lastState = currentState;

        // Read button
        if (!(PIND & (1 << OCTAVE_UP))) {
            printf("up pressed\n");
            if (octave < 3) {
                octave += 1;
                printf("octave: %d\n", octave);
                _delay_ms(200);  // debounce delay
            }
        }
        if (!(PIND & (1 << OCTAVE_DOWN))) {
            printf("down pressed\n");
            if (octave > 0) {
                octave -= 1;
                printf("octave: %d\n", octave);
                _delay_ms(200);
            }
        }
        
        if (!((PIND & (1 << SW)) >> SW)) {
            printf("encoder pressed, key %d, octave %d\n", counter, octave);
            play_note(note_freqs[octave][counter % 8], 300);
        }
        
        
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
            //play_track(2);
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
            //play_track(1);
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
            //play_track(3);
        }
        
        prevHH = reading2;
        prevSnare = reading1;
        prevBass = reading0;
        
        _delay_ms(1); // Small delay to debounce
    }
    
    return 0;
}
