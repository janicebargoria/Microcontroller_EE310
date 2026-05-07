/* Description:
This program displays accelerometer movement on a 16x2 LCD using a PIC18
 microcontroller. The X-axis output of the accelerometer is connected to
 RA0/ANA0 and read using the ADC module. The ADC value is converted to voltage
 and then used to calculate the X-axis acceleration.

 The program compares the acceleration value to set movement states such as
 "Flat", "Tilt Left", "Tilt Right", and "Shake". These states are displayed
 on the LCD along with the calculated X-axis acceleration.

 A push button connected to RC2 is configured as an interrupt-on-change input.
 When the button interrupt occurs, an LED connected to RD3 blinks ten times.
----------------------------------------------------------------------
 Inputs:
 RA0/ANA0  - Accelerometer X-axis analog output
 RC2       - Push button interrupt input

 Outputs:
 PORTB     - LCD data lines
 RD0       - LCD RS control pin
 RD1       - LCD EN control pin
 RD3       - LED output
----------------------------------------------------------------------
 Purpose:
 To read accelerometer movement, detect tilt or shake motion, display the
 movement state on an LCD, and use an interrupt button to blink an LED.*

  * Author: Janice Bargoria
  * Date: 05/04/2026
*/
#pragma config FEXTOSC = LP
#pragma config RSTOSC = EXTOSC
#pragma config CLKOUTEN = OFF
#pragma config PR1WAY = ON
#pragma config CSWEN = ON
#pragma config FCMEN = ON
#pragma config MCLRE = EXTMCLR
#pragma config PWRTS = PWRT_OFF
#pragma config MVECEN = ON
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
//#pragma config WDTE = ON
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

#include <xc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define _XTAL_FREQ 4000000
#define FCY _XTAL_FREQ/4

#define RS LATD0
#define EN LATD1
#define ldata LATB

#define LCD_Port TRISB
#define LCD_Control TRISD

#define Vref 3.3

void LCD_Init();
void LCD_Command(char);
void LCD_Char(char x);
void LCD_String(const char *);
void LCD_String_xy(char, char, const char *);
void MSdelay(unsigned int);
void INTERRUPT_Initialize();
void ADC_Init();
void LCD_Clear();

int light_loop = 0;

int digital;
float voltage;
float accel_x;
float prev_voltage = 0;

char data[16];
char state[17];

void main(void)
{
    IOCCFbits.IOCCF2 = 0;
    PIR0bits.IOCIF = 0;

    INTERRUPT_Initialize();

    IOCCFbits.IOCCF2 = 0;
    PIR0bits.IOCIF = 0;

    ADC_Init();
    LCD_Init();

    IOCCFbits.IOCCF2 = 0;

    while(1)
    {
        ADCON0bits.GO = 1;

        while(ADCON0bits.GO)
        {
            // Wait until ADC conversion is done
        }

        digital = (ADRESH * 256) | ADRESL;

        voltage = digital * ((float)Vref / (float)4096);

        accel_x = (voltage - 1.06) / 0.33;

if(fabs(voltage - prev_voltage) > 0.10)
{
    strcpy(state, "Shake");
}
else if(accel_x < -0.1)
{
    strcpy(state, "Tilt Left");
}
else if(accel_x > 0.3)
{
    strcpy(state, "Tilt Right");
}
else
{
    strcpy(state, "Flat");
}

prev_voltage = voltage;

        sprintf(data, "%.2f X-dir", accel_x);

        LCD_Command(0x01);
        LCD_String_xy(1, 0, "State:");
        LCD_String_xy(1, 7, state);
        LCD_String_xy(2, 0, data);

        __delay_ms(300);
    }
}

void __interrupt(irq(default), base(0x6008)) ISR(void)
{
    if(IOCCFbits.IOCCF2 == 1)
    {
        while(light_loop < 10)
        {
            PORTDbits.RD3 = 0;
            __delay_ms(500);

            PORTDbits.RD3 = 1;
            __delay_ms(500);

            light_loop++;
        }

        light_loop = 0;
        PORTDbits.RD3 = 0;

        IOCCFbits.IOCCF2 = 0;
        PIR0bits.IOCIF = 0;
    }
}

void INTERRUPT_Initialize(void)
{
    INTCON0bits.IPEN = 1;
    INTCON0bits.GIEH = 1;
    INTCON0bits.GIEL = 1;
    INTCON0bits.GIE = 1;

    TRISCbits.TRISC2 = 1;
    ANSELCbits.ANSELC2 = 0;

    TRISDbits.TRISD3 = 0;
    ANSELDbits.ANSELD3 = 0;

    IOCCPbits.IOCCP2 = 1;

    IPR0bits.IOCIP = 1;
    PIE0bits.IOCIE = 1;

    PIR0bits.IOCIF = 0;
    IOCCFbits.IOCCF2 = 0;

    IVTBASEU = 0x00;
    IVTBASEH = 0x60;
    IVTBASEL = 0x08;
}

void ADC_Init(void)
{
    ADCON0bits.FM = 1;
    ADCON0bits.CS = 1;

    TRISAbits.TRISA0 = 1;
    ANSELAbits.ANSELA0 = 1;

    ADPCH = 0x00;

    ADCLK = 0x00;

    ADRESH = 0;
    ADRESL = 0;

    ADPREL = 0;
    ADPREH = 0;

    ADACQL = 0;
    ADACQH = 0;

    ADREF = 0x00;

    ADCON0bits.ON = 1;
}

void LCD_Init()
{
    MSdelay(15);

    LCD_Port = 0x00;
    LCD_Control = 0x00;

    LATC = 0b00000000;
    ANSELC = 0b00000000;
    PORTC = 0b00000000;
    TRISC = 0b00000100;

    LATA = 0b00000000;
    ANSELA = 0b00000001;
    PORTA = 0b00000000;
    TRISA = 0b00000001;

    LCD_Command(0x01);
    LCD_Command(0x38);
    LCD_Command(0x0c);
    LCD_Command(0x06);
}

void LCD_Clear()
{
    LCD_Command(0x01);
}

void LCD_Command(char cmd)
{
    ldata = cmd;
    RS = 0;
    EN = 1;
    NOP();
    EN = 0;
    MSdelay(3);
}

void LCD_Char(char dat)
{
    ldata = dat;
    RS = 1;
    EN = 1;
    NOP();
    EN = 0;
    MSdelay(1);
}

void LCD_String(const char *msg)
{
    while((*msg) != 0)
    {
        LCD_Char(*msg);
        msg++;
    }
}

void LCD_String_xy(char row, char pos, const char *msg)
{
    char location = 0;

    if(row <= 1)
    {
        location = (0x80) | ((pos) & 0x0f);
        LCD_Command(location);
    }
    else
    {
        location = (0xC0) | ((pos) & 0x0f);
        LCD_Command(location);
    }

    LCD_String(msg);
}

void MSdelay(unsigned int val)
{
    unsigned int i, j;

    for(i = 0; i < val; i++)
    {
        for(j = 0; j < 165; j++);
    }
}