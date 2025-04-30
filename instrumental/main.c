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
#include "piano.h"
#include "drums.h"


#define CLK PD7 //A

uint8_t prevState;

//previous variables for drum parts
volatile int prevSnare = 0;
volatile int prevBass = 0;
volatile int prevHH = 0;

//0001 is snare drum, 0002 is hi-hat, and 0003 is bass
int main(void) {
    Initialize();
    uart_init();
    piano_init();
    
    // get previous state to compare against
    prevState = (PIND & (1 << CLK)) >> CLK;
    
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
   
        piano_main_loop(prevState);
        drums_main_loop(reading0, reading1, reading2, prevSnare, prevBass, prevHH);
        
        prevHH = reading2;
        prevSnare = reading1;
        prevBass = reading0;
        
        _delay_ms(1); // Small delay to debounce
    }
    
    return 0;
}
