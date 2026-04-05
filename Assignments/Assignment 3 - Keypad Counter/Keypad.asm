
;===============================================================
; Title: Keypad-Controlled 7-Segment Counter
// Assignment: Assignment 6 - GPIO & 7-Segment Interface

// Author: Janice Bargoria
// Date: 03/25/2026

// Description:
; This program implements a keypad-controlled counter using a 
; PIC18 microcontroller, a 3x4 matrix keypad, and a common 
; cathode 7-segment display.

// The system performs the following functions:
;   - Key '1': Increment the counter
;   - Key '2': Decrement the counter
;   - Key '3': Reset the counter to 0
    
// Implementation Details:
;   - Uses PORTB for keypad interfacing (polling method)
;   - Uses PORTD for 7-segment display output
;   - Uses a lookup table stored in program memory to convert
;     counter values into 7-segment display patterns
    
; Version History:
;   v1.0 - Initial implementation
    
// Counter Behavior:
;   - Counts from 0 to 9 (decimal), Increment wraps: 9 ? 0,  Decrement wraps: 0 ? 9, If no key is pressed, the display holds the last value
     
// Inputs:
;   - 3x4 Keypad connected to PORTB
//  Outputs:
;   - 7-Segment Display connected to PORTD
;
; Hardware Setup:
;   - PIC18F46K42 (Curiosity Nano Board)
;   - Common Cathode 7-Segment Display
;   - External power supply (battery)
;
; Development Environment:
;   - MPLAB X IDE
;   - Compiler: pic-as (v3.x)
;   - Operating System: Windows
;
; Compilation:
;   - Built using MPLAB X IDE (Build Project)
;   - Toolchain: XC8 / pic-as assembler
;
; Notes:
;   - Keypad scanning is implemented using column driving and
;     row reading (polling method)
;   - Delay is implemented using nested loops
;   - Table pointer is used to access display patterns
;
; Version History:
;   v1.0 - Initial implementation
;   v1.1 - Added wrap-around logic (0?9)
;   v1.2 - Adjusted 7-segment mapping to match hardware wiring
;
;===============================================================


#include <xc.inc>
#include "./AssemblyConfig.inc"
;========================================================
; Program: Keypad Controlled 7-Segment Counter
; Device : PIC18
;
; Description:
;   This program uses a 3x4 keypad and a common-cathode
;   7-segment display.
;
;   Key functions:
;     Key 1 -> count up
;     Key 2 -> count down
;     Key 3 -> reset counter to 0
;
; Counter range:
;   0 to 9 only
;
; Wraparound behavior:
;   Increment: 9 -> 0
;   Decrement: 0 -> 9
;
; Notes:
;   - PORTB is used for keypad interfacing
;   - PORTD is used for 7-segment output
;   - A lookup table is used to display digits
;   - Delay is implemented using a subroutine
;========================================================

;-------------------------
; RAM Variables
;-------------------------
COUNTER     EQU 0x20     ; Stores current displayed value
BUTTON      EQU 0x21      ; Stores detected keypad button
DLY1        EQU 0x22      ; Outer delay counter
DLY2        EQU 0x23      ; Inner delay counter

;========================================================
; Reset Vector
;========================================================
    PSECT absdata,abs,ovrld
    ORG     0x0000
    GOTO    _init

    ORG     0x0020

;========================================================
; Initialization
;========================================================
_init:
    CLRF    COUNTER            ; Start from 0
    CLRF    BUTTON             ; No button pressed
    RCALL   _setup_ports       ; Configure ports
    RCALL   _display_digit     ; Show initial 0
;========================================================
; Main Loop
;========================================================
_main:
    RCALL   _scan_keypad       ; Check keypad
    ; If key 1 pressed -> count up
    MOVF    BUTTON, W
    XORLW   0x01
    BTFSC   STATUS, 2
    GOTO    _count_up
    
    ; If key 2 pressed -> count down
    MOVF    BUTTON, W
    XORLW   0x02
    BTFSC   STATUS, 2
    GOTO    _count_down
    
    ; If key 3 pressed -> reset
    MOVF    BUTTON, W
    XORLW   0x03
    BTFSC   STATUS, 2
    GOTO    _reset_count

    ; If no valid key pressed, keep displaying current value
    RCALL   _display_digit
    GOTO    _main

;========================================================
; Port Setup
;========================================================
_setup_ports:
    ;------------------------------------
    ; PORTB setup for keypad
    ; RB0, RB1, RB2 = outputs (columns)
    ; RB3-RB7       = inputs  (rows)
    ;------------------------------------
    BANKSEL PORTB
    CLRF    PORTB

    BANKSEL LATB
    CLRF    LATB

    BANKSEL ANSELB
    CLRF    ANSELB         ; Digital mode

    BANKSEL TRISB
    MOVLW   0b11111000     ; RB0-RB2 outputs, rest inputs
    MOVWF   TRISB

    ;------------------------------------
    ; PORTD setup for 7-segment display
    ;------------------------------------
    BANKSEL PORTD
    CLRF    PORTD

    BANKSEL LATD
    CLRF    LATD

    BANKSEL ANSELD
    CLRF    ANSELD         ; Digital mode

    BANKSEL TRISD
    CLRF    TRISD          ; All PORTD pins outputs

    RETURN

;========================================================
; Count Up Routine
;========================================================
_count_up:
_up_loop:
    RCALL   _display_digit ; Show current digit
    RCALL   _delay         ; Visible delay

    INCF    COUNTER, F     ; COUNTER = COUNTER + 1

    ; If COUNTER becomes 10, wrap back to 0
    MOVF    COUNTER, W
    XORLW   0x0A           ; Compare COUNTER with 10
    BTFSC   STATUS, 2
    CLRF    COUNTER        ; Reset to 0

    ; Keep counting only while key 1 is still pressed
    RCALL   _scan_keypad
    MOVF    BUTTON, W
    XORLW   0x01
    BTFSC   STATUS, 2
    GOTO    _up_loop

    GOTO    _main

;========================================================
; Count Down Routine
;========================================================
_count_down:
_down_loop:
    RCALL   _display_digit ; Show current digit
    RCALL   _delay         ; Visible delay

    ; If COUNTER = 0, wrap to 9
    MOVF    COUNTER, F
    BTFSC   STATUS, 2
    GOTO    _wrap_9

    ; Otherwise decrement normally
    DECF    COUNTER, F
    GOTO    _down_check

_wrap_9:
    MOVLW   0x09           ; Wrap to 9
    MOVWF   COUNTER

_down_check:
    ; Keep counting only while key 2 is still pressed
    RCALL   _scan_keypad
    MOVF    BUTTON, W
    XORLW   0x02
    BTFSC   STATUS, 2
    GOTO    _down_loop

    GOTO    _main

;========================================================
; Reset Routine
;========================================================
_reset_count:
    CLRF    COUNTER        ; Reset to 0
    RCALL   _display_digit ; Show 0 immediately

_wait_release:
    ; Wait until key 3 is released
    RCALL   _scan_keypad
    MOVF    BUTTON, W
    XORLW   0x03
    BTFSC   STATUS, 2
    GOTO    _wait_release

    GOTO    _main
;========================================================
; Display Routine
;
; Uses table read to fetch the 7-segment bit pattern
; corresponding to COUNTER.
;========================================================
_display_digit:
    ; Load table start address = 0x0200
    MOVLW   0x00
    MOVWF   TBLPTRL
    MOVLW   0x02
    MOVWF   TBLPTRH
    MOVLW   0x00
    MOVWF   TBLPTRU

    ; Add COUNTER offset to table pointer
    MOVF    COUNTER, W
    ADDWF   TBLPTRL, F
    CLRF    WREG
    ADDWFC  TBLPTRH, F
    ADDWFC  TBLPTRU, F

    ; Read table byte
    TBLRD*
    MOVF    TABLAT, W

    ; Output to 7-segment
    MOVWF   LATD
    RETURN

;========================================================
; Delay Routine
;========================================================
_delay:
    MOVLW   0xFF
    MOVWF   DLY1

_d1:
    MOVLW   0xFF
    MOVWF   DLY2

_d2:
    DECFSZ  DLY2, F
    GOTO    _d2
    DECFSZ  DLY1, F
    GOTO    _d1
    RETURN

;========================================================
; Keypad Scan Routine
;
; Only checks keys:
;   1, 2, 3
;
; BUTTON results:
;   0 = no valid key
;   1 = key 1
;   2 = key 2
;   3 = key 3
;========================================================
_scan_keypad:
    CLRF    BUTTON

    ; Set all columns low first
    BANKSEL LATB
    BCF     LATB,0
    BCF     LATB,1
    BCF     LATB,2
    NOP
    NOP

    ;-------------------------
    ; Check key 1
    ;-------------------------
    BSF     LATB,0
    NOP
    NOP
    BANKSEL PORTB
    BTFSC   PORTB,3
    GOTO    _k1
    BANKSEL LATB
    BCF     LATB,0

    ;-------------------------
    ; Check key 2
    ;-------------------------
    BSF     LATB,1
    NOP
    NOP
    BANKSEL PORTB
    BTFSC   PORTB,3
    GOTO    _k2
    BANKSEL LATB
    BCF     LATB,1

    ;-------------------------
    ; Check key 3
    ;-------------------------
    BSF     LATB,2
    NOP
    NOP
    BANKSEL PORTB
    BTFSC   PORTB,3
    GOTO    _k3
    BANKSEL LATB
    BCF     LATB,2

    RETURN

;========================================================
; Key Detection Labels
;========================================================
_k1:
    BANKSEL LATB
    BCF     LATB,0
    BCF     LATB,1
    BCF     LATB,2
    MOVLW   0x01
    MOVWF   BUTTON
    RETURN

_k2:
    BANKSEL LATB
    BCF     LATB,0
    BCF     LATB,1
    BCF     LATB,2
    MOVLW   0x02
    MOVWF   BUTTON
    RETURN

_k3:
    BANKSEL LATB
    BCF     LATB,0
    BCF     LATB,1
    BCF     LATB,2
    MOVLW   0x03
    MOVWF   BUTTON
    RETURN

;========================================================
; 7-Segment Lookup Table ; These patterns match the wiring ; Digits included: 0 to 9
;========================================================
    ORG     0x0200
_table:
    DB  0b0111111   ; 0
    DB  0b0001100   ; 1
    DB  0b1011011   ; 2
    DB  0b1011110   ; 3
    DB  0b1101100   ; 4
    DB  0b1110110   ; 5
    DB  0b1110111   ; 6
    DB  0b0011100   ; 7
    DB  0b1111111   ; 8
    DB  0b1111110   ; 9
    
    END

  
  
