#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "buzzer.h" // Header for functions

//===============================================================
// Title: Password-Based Motor Control (7-Segment)
// Assignment: PIC18 Safebox System
//
// Author: Janice Bargoria
// Date: 04/20/2026
//
// Description:
// Simple password system using two push buttons and a 7-segment display.
// - RA1: increment digit1
// - RA0: increment digit2
// - Displays selected digit on 7-segment
// - After ~5 seconds, checks password
//     - Correct -> Motor + LED ON
//     - Wrong   -> Buzzer + LED ON
// - RB0 interrupt resets system instantly
//===============================================================

// Global variables
extern unsigned char numTable[10];
extern int passCode1;
extern int passCode2;
extern volatile int resetFlag;

// Function prototypes
void init(void);
void Sevenseg_Disp(int number);

int isDigit2_pressed(void);
int isDigit1_pressed(void);

void Turn_On_Motor(void);
void Turn_Off_Motor(void);
void Turn_On_Buzzer(void);
void Turn_Off_Buzzer(void);
void Turn_On_Led(void);
void Turn_Off_Led(void);

void __interrupt(__irq(IRQ_INT0), __high_priority) ISR(void);

#endif