/*
 * ---------------------
 * Title: Simple LED Blinking 
 * ---------------------
 * Program Details:
 *  The purpose of this program is to simply blink an LED 
 * Inputs: myDelay 
 * Outputs: RD0 (output)
 * Setup: C- Simulator
 * Date: Feb 24, 2023
 * File Dependencies / Libraries: It is required to include the 
 * Configuration Header File 
 * Compiler: xc8, 3.7
 * Author: Janice Bargoria
 * Versions:
 *      V1.0: Original
 * Useful links:  
 *      Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18(L)F26-27-45-46-47-55-56-57K42-Data-Sheet-40001919G.pdf 
 *      PIC18F Instruction Sets: https://onlinelibrary.wiley.com/doi/pdf/10.1002/9781119448457.app4 
 *      List of Instrcutions: http://143.110.227.210/faridfarahmand/sonoma/courses/es310/resources/20140217124422790.pdf 
 */

#include <xc.h> // must have this
#include "myheader.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define _XTAL_FREQ 4000000
#define FCY (_XTAL_FREQ/4)

#define LEDPORT LATD           // LEDs on RD0-RD3
#define SWITCH PORTBbits.RB0   // Switch on RB0

void main(void) {
    unsigned char value;

    // --- LED setup ---
    ANSELD = 0x00;   // PORTB digital
    TRISD = 0x00;    // PORTB outputs
    LATD = 0x00;     // LEDs off initially

    // --- Switch setup ---
    ANSELBbits.ANSELB0 = 0; // RD0 digital
    TRISBbits.TRISB0 = 1;   // RD0 input
    WPUBbits.WPUB0 = 1;     // weak pull-up enabled
    LATB = 0x00;

    // --- Seed RNG ---
    srand(1); // or use timer/analog for better randomness

    while(1) {
        if(SWITCH == 0) {       // button pressed
            __delay_ms(50);     // debounce
            if(SWITCH == 0) {   // confirm press
                value = rand() % 16;        // random 0-15
                LEDPORT = (LEDPORT & 0xF0) | (value & 0x0F); // show on RB0-RB3
            }
            while(SWITCH == 0);  // wait until release
        }
    }
}
