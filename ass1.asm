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
    
    #include <xc.inc>
    #include "./myConfigFile.inc"
    
;----------------
; PROGRAM INPUTS
;----------------
;The DEFINE directive is used to create macros or symbolic names for values.
;It is more flexible and can be used to define complex expressions or sequences of instructions.
;It is processed by the preprocessor before the assembly begins.

#define  measuredTempInput 	45 ; this is the input value
#define  refTempInput 		25 ; this is the input value

;---------------------
; Definitions
;---------------------
#define SWITCH    LATD,2  
#define LED0      PORTD,0
#define LED1	     PORTD,1
    
 
;---------------------
; Program Constants
;---------------------
; The EQU (Equals) directive is used to assign a constant value to a symbolic name or label.
; It is simpler and is typically used for straightforward assignments.
;It directly substitutes the defined value into the code during the assembly process.
    
REG10   equ     10h   // in HEX
REG11   equ     11h
REG01   equ     1h
