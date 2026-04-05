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
    #include "C:\Users\Janice Bargoria\MPLABXProjects\Lab 6A\AssemblyConfig.inc"

PSECT absdata,abs,ovrld

;--------------------------------
; Reset Vector
;--------------------------------
ORG 0x00
GOTO START
;--------------------------------
; Input test values
;--------------------------------
#define measuredTempInput 15     ; current temperature
#define refTempInput      5    ; desired temperature
;--------------------------------
; Register assignments (R8)
;--------------------------------
refTemp      EQU 0x20
measuredTemp EQU 0x21
contReg      EQU 0x22
; Decimal storage (R9, R10)
refHund EQU 0x60
refTen  EQU 0x61
refOne  EQU 0x62

measHund EQU 0x70
measTen  EQU 0x71
measOne  EQU 0x72

;--------------------------------
; Main program starts at 0x20 (R7)
;--------------------------------
ORG 0x20

START:
;--------------------------------
; Configure PORTD
;--------------------------------
CLRF ANSELD        ; disable analog mode
CLRF TRISD         ; set PORTD as output
CLRF PORTD         ; clear all output bits
CLRF contReg       ; initialize control register
;--------------------------------
; Load temperature values
;--------------------------------
MOVLW measuredTempInput
MOVWF measuredTemp

MOVLW refTempInput
MOVWF refTemp
;--------------------------------
; Convert refTemp ? decimal digits
;--------------------------------
CLRF refHund       ; hundreds = 0
CLRF refTen        ; tens = 0
MOVF refTemp,W
MOVWF refOne       ; ones = value
    
REF_LOOP:
    MOVLW 10
    SUBWF refOne,W ; subtract 10

    BTFSS STATUS,0 ; stop if < 10
    GOTO REF_DONE

    MOVWF refOne   ; store remainder
    INCF refTen,F  ; increment tens
    GOTO REF_LOOP

REF_DONE:
;--------------------------------
; Convert measuredTemp ? digits
;--------------------------------
CLRF measHund
CLRF measTen

MOVF measuredTemp,W
MOVWF measOne

MEAS_LOOP:
    MOVLW 10
    SUBWF measOne,W
   
    BTFSS STATUS,0
    GOTO MEAS_DONE

    MOVWF measOne
    INCF measTen,F
    GOTO MEAS_LOOP

MEAS_DONE:
;--------------------------------
; Compare temperatures
;--------------------------------
MOVF refTemp,W
SUBWF measuredTemp,W   ; measuredTemp - refTemp
BTFSC STATUS,2         ; if equal
GOTO LED_OFF
BTFSS STATUS,0         ; if measuredTemp < refTemp
GOTO LED_HOT
GOTO LED_COOL          ; otherwise measuredTemp > refTemp
    
;--------------------------------
; Cooling system (R1)
;--------------------------------
LED_COOL:
CLRF PORTD             ; clear previous outputs
MOVLW 2
MOVWF contReg
BSF PORTD,2            ; turn ON cooling (RD2)
BCF PORTD,1            ; ensure heating OFF

GOTO START
;--------------------------------
; Heating system (R2)
;--------------------------------
LED_HOT:
CLRF PORTD
MOVLW 1
MOVWF contReg
BSF PORTD,1            ; turn ON heating (RD1)
BCF PORTD,2            ; ensure cooling OFF

GOTO START
;--------------------------------
; Equal temperature (R3)
;--------------------------------
LED_OFF:
CLRF PORTD             ; turn OFF all outputs
CLRF contReg
GOTO START

END