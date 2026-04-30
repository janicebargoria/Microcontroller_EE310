/********************************************************************
 * Title: Timer0 Interrupt-Based Software Reset (PIC18F47K42)
 *
 * Description:
 * This program demonstrates the use of Timer0 interrupt to trigger
 * a software reset using the RESET() instruction. RB0 toggles in
 * the main loop to indicate normal operation, while RB1 provides
 * a startup indication. When Timer0 overflows, the ISR executes
 * and forces a reset.
 *
 ********************************************************************/

#include <xc.h>
#include "pic18f47k42.h"
#define _XTAL_FREQ 4000000

//------------------------------------------------------------
// Timer0 Interrupt Service Routine
//------------------------------------------------------------
void __interrupt(irq(TMR0), base(8)) TMR0_ISR(void) {
    if (PIR3bits.TMR0IF) 
    {
        PIR3bits.TMR0IF = 0; // Clear interrupt flag
        RESET();             // Perform software reset
    }
}

//------------------------------------------------------------
// Timer0 Initialization
//------------------------------------------------------------
void init_timer0(void) {
    T0CON0 = 0b10000000; // TMR0 ON, 8-bit mode
    T0CON1 = 0b01000000; // Prescaler 1:2, Clock = Fosc/4

    TMR0H = 0x00;        // Clear high byte
    TMR0L = 0x00;        // Clear low byte

    PIR3bits.TMR0IF = 0;   // Clear interrupt flag
    PIE3bits.TMR0IE = 1;   // Enable Timer0 interrupt

    INTCON0bits.GIE = 1;   // Global Interrupt Enable
    INTCON0bits.IPEN = 0;  // Disable interrupt priority (simple mode)

    // Interrupt Vector Table base address
    IVTBASEU = 0x00; 
    IVTBASEH = 0x00; 
    IVTBASEL = 0x08; 
}

//------------------------------------------------------------
// Main Program
//------------------------------------------------------------
void main(void) {
    ANSELB = 0x00;

    TRISBbits.TRISB0 = 0; // Configure RB0 as output 
    TRISBbits.TRISB1 = 0; // Configure RB1 as output 

    LATBbits.LATB1 = 1;   // Turn ON RB1 at startup
    __delay_ms(500);
    LATBbits.LATB1 = 0;   // Turn OFF RB1
    
    

        if(PCON0bits.nRI == 0)
    {
        LATBbits.LATB1 = 1;
        __delay_ms(1000);
        LATBbits.LATB1 = 0;

        PCON0bits.nRI = 1;
    }
    
    
    
    init_timer0();        // Initialize Timer0

    while(1) {
        // Main loop: toggle RB0 to show program is running
        LATBbits.LATB0 = ~LATBbits.LATB0;
        __delay_ms(300);
    }
}