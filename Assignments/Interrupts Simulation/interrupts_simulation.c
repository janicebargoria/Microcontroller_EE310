/*
 * Interrupt Assignment - PIC18F47K42
 *
 * RB0 = INT0 input (external interrupt)
 * RD0 = blinks during interrupt (ISR)
 * RD1 = blinks in main loop
 *
 * Behavior:
 * - RD1 blinks continuously
 * - When RB0 gets a rising edge ? interrupt occurs
 * - RD0 blinks quickly inside ISR
 * - RD1 stops while ISR is running
 * - After ISR, RD1 continues blinking
 */

#include <xc.h>
#include "interrupts.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ 4000000   // Needed for delay (not used in simulation)

// ---------------- CONFIG BITS ----------------
#pragma config FEXTOSC = LP
#pragma config RSTOSC = EXTOSC
#pragma config CLKOUTEN = OFF
#pragma config PR1WAY = ON
#pragma config CSWEN = ON
#pragma config FCMEN = ON

#pragma config MCLRE = EXTMCLR
#pragma config PWRTS = PWRT_OFF
#pragma config MVECEN = ON      // Enable multi-vector interrupts
#pragma config IVT1WAY = ON
#pragma config LPBOREN = OFF
#pragma config BOREN = SBORDIS

#pragma config BORV = VBOR_2P45
#pragma config ZCD = OFF
#pragma config PPS1WAY = ON
#pragma config STVREN = ON
#pragma config DEBUG = OFF
#pragma config XINST = OFF

#pragma config WDTCPS = WDTCPS_31
#pragma config WDTE = OFF
#pragma config WDTCWS = WDTCWS_7
#pragma config WDTCCS = SC

#pragma config BBSIZE = BBSIZE_512
#pragma config BBEN = OFF
#pragma config SAFEN = OFF
#pragma config WRTAPP = OFF

#pragma config WRTB = OFF
#pragma config WRTC = OFF
#pragma config WRTD = OFF
#pragma config WRTSAF = OFF
#pragma config LVP = ON

#pragma config CP = OFF

// Function prototype
void INTERRUPT_Initialize(void);

// ---------------- INTERRUPT SERVICE ROUTINE ----------------
void __interrupt(irq(IRQ_INT0), base(0x4008)) INT0_ISR(void)
{
    uint8_t i;

    // Check if INT0 interrupt flag is set
    if (PIR1bits.INT0IF == 1)
    {
        // Toggle RD0 several times (visible in simulation)
        for (i = 0; i < 16; i++)
        {
            LATDbits.LATD0 = !LATDbits.LATD0;
        }

        // Clear interrupt flag (VERY IMPORTANT)
        PIR1bits.INT0IF = 0;

        // Ensure RD0 ends OFF
        LATDbits.LATD0 = 0;
    }
}

// ---------------- INTERRUPT INITIALIZATION ----------------
void INTERRUPT_Initialize(void)
{
    // Enable interrupt priority system
    INTCON0bits.IPEN = 1;

    // Enable global interrupts
    INTCON0bits.GIEH = 1;
    INTCON0bits.GIEL = 1;

    // Set INT0 to trigger on rising edge (0 ? 1)
    INTCON0bits.INT0EDG = 1;

    // Set INT0 as high priority interrupt
    IPR1bits.INT0IP = 1;

    // Clear interrupt flag before enabling
    PIR1bits.INT0IF = 0;

    // Enable INT0 interrupt
    PIE1bits.INT0IE = 1;

    // Set Interrupt Vector Table base address (0x4008)
    IVTBASEU = 0x00;
    IVTBASEH = 0x40;
    IVTBASEL = 0x08;
}

// ---------------- MAIN FUNCTION ----------------
void main(void)
{
    // Disable analog on ports (make digital)
    ANSELB = 0x00;
    ANSELD = 0x00;

    // RB0 = input (INT0 pin)
    TRISBbits.TRISB0 = 1;

    // RD0 and RD1 = outputs
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD1 = 0;

    // Initialize outputs to LOW
    LATDbits.LATD0 = 0;
    LATDbits.LATD1 = 0;

    // Enable weak pull-up on RB0 (helps stabilize input)
    WPUBbits.WPUB0 = 1;

    // Initialize interrupts
    INTERRUPT_Initialize();

    // -------- MAIN LOOP --------
    while (1)
    {
        // RD1 keeps toggling continuously
        // This will STOP temporarily when ISR runs
        LATDbits.LATD1 = !LATDbits.LATD1;
    }
}