/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   piano.h
 * Author: mosha
 * Comments: Header file for piano.c
 * Revision history: 
 */

#ifndef PIANO_H
#define	PIANO_H

#include <xc.h>
#include <stdint.h> // for uint8_t, uint16_t

// Define function prototypes for the piano system

/**
 * Initializes the rotary encoder, PWM for buzzer, and octave buttons.
 */
void piano_init(void);

/**
 * Plays a note at a given frequency and duration in milliseconds.
 * 
 * @param freq The frequency of the note in Hz.
 * @param duration_ms Duration in milliseconds.
 */
void play_note(uint16_t freq, uint16_t duration_ms);

/**
 * The main loop function for the piano system. Reads the encoder state,
 * handles octave changes, and plays notes based on input.
 * 
 * @param lastState The previous state of the rotary encoder CLK pin.
 */
void piano_main_loop(uint8_t lastState);

#endif	/* PIANO_H */
