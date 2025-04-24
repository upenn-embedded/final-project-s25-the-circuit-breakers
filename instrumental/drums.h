/* 
 * File:   drums.h
 * Author: mosha
 *
 * Created on April 24, 2025, 3:16 PM
 */

#ifndef DRUMS_H
#define	DRUMS_H

void Initialize(void);

void bufferTransmitDF(uint8_t command, uint8_t p1, uint8_t p2);

void drums_main_loop(int reading0, int reading1, int reading2, int prevSnare, int prevBass, int prevHH);



#endif	/* DRUMS_H */
