/*
 * File:   debouncing.c
 * Author: Janice Bargoria
 *
 * Created on April 8, 2026, 10:00 PM
 */

#include <xc.h> // must have this
#include "myheader.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "pic18f47k42.h"

#define _XTAL_FREQ 4000000
#define FCY    _XTAL_FREQ/4

//define switch
#define Switch_Pin PORTBbits.RB0

void main(void)
{
//disable analog ports

ANSELB = 0x00;
ANSELD = 0x00;

//set directions

 TRISBbits.TRISB0 = 1; //RB0 Input
 TRISD = 0x00; //RD0 LED
 
 //Enable Weak pull-up on RB0
 
 WPUBbits.WPUB0 = 1;

 
 //Initialize LED off
LATD = 0x00;
 
 while(1)
     
 {
     //check if switch is pressed (Active Low)
     if (Switch_Pin == 0)
     {
         __delay_ms(10);
         
         if (Switch_Pin == 0)
         {
             LATD = 0x0F;
             
             while(Switch_Pin == 0);
             
             
             __delay_ms(10);
             
             LATD = 0x00;
         }
     }
 }
}