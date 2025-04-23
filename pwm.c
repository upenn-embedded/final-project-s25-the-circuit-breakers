/*
 * File:   main.c
 * Author: nandiniswami
 *
 * Created on April 4, 2025, 2:59 PM
 */
#define F_CPU 16000000UL
#include "uart.h"
#include <xc.h>
#include <util/delay.h>

#define SW PD5 //when pressed HIGH
#define DT PD6 //B
#define CLK PD7 //A
#define OCTAVE_DOWN PD3
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

void main(void) {
    uart_init();
    rotary_init();
    pwm_init();
    button_init();
    octave = 1;
    printf("octave %d\n", octave);
    
    // get previous state to compare against
    lastState = (PIND & (1 << CLK)) >> CLK;
    
    while(1) {
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


        _delay_ms(1); // Small delay to debounce
    }
}
