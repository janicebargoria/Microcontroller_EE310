//-----------------------------
// Title: Temperature Contol 
//-----------------------------
// Purpose: To Detect Temperature & control heating and cooling systems
// Dependencies: AssemblyConfig.inc
// Compiler: pic-as (v3.10)
// Author: Janice Bargoria
// OUTPUTS: PORTD.1 and PORTD.2
// INPUTS: refTemp and measTemp
// Versions:
//  	V1.0: 03/10/2026 - First version 
//-----------------------------

    
    
    
    
    #include "C:\Users\Janice Bargoria\MPLABXProjects\Lab 6A\AssemblyConfig.inc"
#include <xc.inc>

;---------------------
; Program Inputs
;---------------------
Inner_loop  equ 3 // in decimal
Outer_loop  equ 2
 
;---------------------
; Program Constants
;---------------------
REG10   equ     10h   // in HEX
REG11   equ     11h

;---------------------
; Definitions
;---------------------
#define SWITCH    PORTD,1  
#define LED0      PORTD,0
#define LED1      PORTB,0
   
;---------------------
; Main Program
;---------------------
    PSECT absdata,abs,ovrld        ; Do not change
    
    ORG          0                ;Reset vector
    GOTO        _initialization

    ORG          0020H           ; Begin assembly at 0020H
 
_initialization: 
    RCALL _setupPortD
    RCALL _setupPortB
    
_main:
    ;BTFSC	SWITCH  ; By removing these two lines the input has no impact
    ;BRA	_main
    BTG		LED0
    BTG		LED1
    CALL	loopDelay ; we can use RCALL
    BRA         _main
    
;-----The Delay Subroutine    
loopDelay: 
    MOVLW       Inner_loop
    MOVWF       REG10
    MOVLW       Outer_loop
    MOVWF       REG11
_loop1:
    DECF        REG10,1
    BNZ         _loop1
    MOVLW       Inner_loop ; Re-initialize the inner loop for when the outer loop decrements.
    MOVWF       REG10
    DECF        REG11,1 // outer loop
    BNZ        _loop1
    RETURN

 
_setupPortD:
    BANKSEL	PORTD ;
    CLRF	PORTD ;Init PORTA
    BANKSEL	LATD ;Data Latch
    CLRF	LATD ;
    BANKSEL	ANSELD ;
    CLRF	ANSELD ;digital I/O
    BANKSEL	TRISD ;
    MOVLW	0b11111110 ;Set RD[7:1] as inputs
    MOVWF	TRISD ;and set RD0 as ouput
    RETURN
 
_setupPortB:
    BANKSEL	PORTB ;
    CLRF	PORTB ;Init PORTA
    BANKSEL	LATB ;Data Latch
    CLRF	LATB ;
    BANKSEL	ANSELB ;
    CLRF	ANSELB ;digital I/O
    BANKSEL	TRISB ;
    MOVLW	0b11111110 ;Set RD[7:1] as inputs
    MOVWF	TRISB ;and set RD0 as ouput
    RETURN    
    
    END