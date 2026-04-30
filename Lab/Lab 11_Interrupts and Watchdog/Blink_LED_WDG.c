/********************************************************************
 * Title: Watchdog Timer Demonstration using PIC18F47K42
 * 
 * Description:
 * This program demonstrates the operation of the Watchdog Timer (WDT)
 * on the PIC18F47K42 microcontroller. RD0 is used as a blinking LED 
 * to count loop iterations, while RD1 indicates when the program has 
 * entered a stuck condition. After 5 counts, the program stops clearing 
 * the watchdog timer, allowing it to time out (~1 second) and reset 
 * the microcontroller.
 *
 * Key Features:
 * - RD0 (LATD0): Blinking LED (loop counter)
 * - RD1 (LATD1): Watchdog indicator LED
 * - WDT configured to ~1 second using WDTCON0
 * - Demonstrates automatic MCU reset via WDT
 *
 ********************************************************************/

#include <xc.h>
#include "pic18f47k42.h"
#define _XTAL_FREQ 4000000   // System clock frequency (4 MHz)

// CONFIG2L
#pragma config WDTE = ON     // Enable Watchdog Timer (always on)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31   // Initial WDT postscaler (overridden at runtime)

// LED Definitions
#define BlinkLED LATDbits.LATD0     // RD0 used for blinking/counting
#define WdtLED LATDbits.LATD1       // RD1 used as Watchdog indicator
#define PORTD_DIR TRISD
#define OUTPUT 0

int count = 0;              // Loop counter variable
void Dummy_Stuck(void);     // Function prototype

void main()
{
    ANSELD = 0x00;          // Configure PORTD pins as digital
    TRISD = 0x00;           // Set PORTD pins as outputs
    LATD = 0x00;            // Initialize all outputs LOW (LEDs OFF)

    // Configure Watchdog Timer to ~1 second timeout
    WDTCON0 = 0b00010101;   // WDTPS = 01010 (1 second), WDT enabled

    __delay_ms(500);        // Initial delay for stabilization/visibility

    WdtLED = 0;             // Ensure RD1 LED is OFF
    count = 0;              // Reset counter
    BlinkLED = 0;           // Ensure RD0 starts LOW
    
    while(1)
    {
        BlinkLED = !BlinkLED;   // Toggle RD0 (visual count indicator)
        __delay_ms(100);        // Delay to make blinking visible

        CLRWDT();               // Clear Watchdog Timer to prevent reset
        count++;                // Increment loop count

        if(count > 5)           // After 5 counts
        {
            WdtLED = 1;         // Turn ON RD1 (indicates stuck condition)
            Dummy_Stuck();      // Enter infinite loop (stop clearing WDT)
        }
    }
}

void Dummy_Stuck(void) //Function called
{
    while(1);   // Infinite loop: WDT not cleared ? causes MCU reset
}