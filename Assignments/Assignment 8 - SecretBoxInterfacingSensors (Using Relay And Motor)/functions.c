#include "functions.h"
#include <xc.h> // XC8 compiler header file
#include <stdint.h> // Standard integer types
#include "buzzer.h" // Header for functions
#include "pic18f47k42.h"

// 7-segment lookup table
unsigned char numTable[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

// Password digits
int passCode1 = 2;
int passCode2 = 3;

// Reset flag (used by interrupt)
volatile int resetFlag = 0;

//--------------------------------------------------
// Initialize ports and interrupt
//--------------------------------------------------
void init(void){
   
    // Buzzer pin RB2
    ANSELBbits.ANSELB2 = 0; 
    LATBbits.LATB2 = 0;
    TRISBbits.TRISB2 = 0;
     
    // RA0, RA1 as input
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;

    // RA3 LED, RA5 Motor as output
    TRISAbits.TRISA3 = 0;
    TRISAbits.TRISA5 = 0;
  
    // Digital mode
    ANSELAbits.ANSELA0 = 0;
    ANSELAbits.ANSELA1 = 0;
    ANSELAbits.ANSELA3 = 0;
    ANSELAbits.ANSELA5 = 0;

    // Outputs OFF initially
    LATAbits.LATA3 = 0;
    LATAbits.LATA5 = 0;
   
    // 7-segment pins
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD1 = 0;
    TRISDbits.TRISD2 = 0;
    TRISDbits.TRISD3 = 0;
    TRISDbits.TRISD4 = 0;
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
    
    ANSELDbits.ANSELD0 = 0;
    ANSELDbits.ANSELD1 = 0;
    ANSELDbits.ANSELD2 = 0;
    ANSELDbits.ANSELD3 = 0;
    ANSELDbits.ANSELD4 = 0;
    ANSELDbits.ANSELD5 = 0;
    ANSELDbits.ANSELD6 = 0;
    
    LATDbits.LATD0 = 0;
    LATDbits.LATD1 = 0;
    LATDbits.LATD2 = 0;
    LATDbits.LATD3 = 0;
    LATDbits.LATD4 = 0;
    LATDbits.LATD5 = 0;
    LATDbits.LATD6 = 0;
    
    // RB0 as interrupt input
    TRISBbits.TRISB0 = 1;
    ANSELBbits.ANSELB0 = 0;

    // INT0 interrupt setup
    INTCON0bits.INT0EDG = 1;   // rising edge
    PIR1bits.INT0IF = 0;       // clear flag
    PIE1bits.INT0IE = 1;       // enable INT0
    INTCON0bits.GIE = 1;       // global interrupt enable
}

//--------------------------------------------------
// Display number on 7-segment
//--------------------------------------------------
void Sevenseg_Disp(int number){
    switch (number)
    {
        case 0: LATD = numTable[0]; break;
        case 1: LATD = numTable[1]; break;
        case 2: LATD = numTable[2]; break;
        case 3: LATD = numTable[3]; break;
        case 4: LATD = numTable[4]; break;
        case 5: LATD = numTable[5]; break;
        case 6: LATD = numTable[6]; break;
        case 7: LATD = numTable[7]; break;
        case 8: LATD = numTable[8]; break;
        case 9: LATD = numTable[9]; break;
        default: LATD = numTable[0]; break;
    }
}

//--------------------------------------------------
// Interrupt Service Routine
//--------------------------------------------------
void __interrupt(__irq(IRQ_INT0), __high_priority) ISR(void)
{
    LATAbits.LATA5 = 0;   // Motor OFF
    LATBbits.LATB2 = 0;   // Buzzer OFF
    LATAbits.LATA3 = 0;   // LED OFF

    resetFlag = 1;
    PIR1bits.INT0IF = 0;
}

//--------------------------------------------------
// Input reading functions
//--------------------------------------------------
int isDigit2_pressed(void){
    return(PORTAbits.RA0);
}

int isDigit1_pressed(void){
    return(PORTAbits.RA1);
}

//--------------------------------------------------
// Output control functions
//--------------------------------------------------
void Turn_On_Motor(void){
    LATAbits.LATA5 = 1;
}

void Turn_Off_Motor(void){
    LATAbits.LATA5 = 0;
}

void Turn_On_Buzzer(void){
    LATBbits.LATB2 = 1;
}

void Turn_Off_Buzzer(void){
    LATBbits.LATB2 = 0;
}

void Turn_On_Led(void){
    LATAbits.LATA3 = 1;
}

void Turn_Off_Led(void){
    LATAbits.LATA3 = 0;
}