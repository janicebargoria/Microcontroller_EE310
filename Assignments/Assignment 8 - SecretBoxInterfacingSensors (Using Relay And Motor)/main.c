#include "functions.h"   // Include all function prototypes and global variables
#include <xc.h> // XC8 compiler header file
#include <stdint.h> // Standard integer types
#include "buzzer.h" // Header for functions
#include "pic18f47k42.h"

//===============================================================
// MAIN PROGRAM
//===============================================================
void main(void)
{
    int digit1 = 0;      // Stores first digit entered
    int digit2 = 0;      // Stores second digit entered
    int TimeStamp = 0;   // Timer counter (counts in 100ms steps)
    int DigitFlag = 0;   // Indicates which digit to display (0 = digit1, 1 = digit2)

    init();              // Initialize ports and interrupt

    while(1)             // Infinite loop
    {
        //=======================================================
        // RESET CONDITION (Triggered by interrupt)
        //=======================================================
        if(resetFlag)
        {
            digit1 = 0;      // Reset first digit
            digit2 = 0;      // Reset second digit
            TimeStamp = 0;   // Reset timer
            DigitFlag = 0;   // Reset display flag

            Turn_Off_Motor();    // Ensure motor is OFF
            Turn_Off_Buzzer();   // Ensure buzzer is OFF
            Turn_Off_Led();      // Ensure LED is OFF

            Sevenseg_Disp(0);    // Display 0 on 7-segment

            resetFlag = 0;       // Clear reset flag
        }

        //=======================================================
        // BUTTON 1 PRESSED (RA1 ? first digit)
        //=======================================================
        if(isDigit1_pressed())
        {
            digit1++;            // Increment first digit
            DigitFlag = 0;       // Display digit1
            TimeStamp = 0;       // Reset inactivity timer
            while(isDigit1_pressed());   // Wait for button release
        }
        
        //=======================================================
        // BUTTON 2 PRESSED (RA0 ? second digit)
        //=======================================================
        if(isDigit2_pressed())
        {
            digit2++;            // Increment second digit
            DigitFlag = 1;       // Display digit2
            TimeStamp = 0;       // Reset inactivity timer
            while(isDigit2_pressed());   // Wait for button release
        }

        //=======================================================
        // DISPLAY CURRENT DIGIT
        //=======================================================
        if(DigitFlag == 0){
            Sevenseg_Disp(digit1);   // Show digit1
        }
        else if(DigitFlag == 1){
            Sevenseg_Disp(digit2);   // Show digit2
        }
        
        //=======================================================
        // PASSWORD CHECK AFTER ~5 SECONDS (50 × 100ms)
        //=======================================================
        if(TimeStamp == 50){

            //-----------------------------
            // CORRECT PASSWORD
            //-----------------------------
            if((passCode1 == digit1) && (passCode2 == digit2))
            {
                Turn_On_Motor();     // Turn ON motor
                Turn_On_Led();       // Turn ON LED

                // Keep ON for ~5 seconds or until reset
                for(int i = 0; i < 50; i++)
                {
                    if(resetFlag) break;   // Exit if reset occurs
                    __delay_ms(100);
                }

                Turn_Off_Motor();    // Turn OFF motor
                Turn_Off_Led();      // Turn OFF LED
            }
            else
            {
                //-----------------------------
                // WRONG PASSWORD
                //-----------------------------
                Turn_On_Buzzer();    // Turn ON buzzer
                Turn_On_Led();       // Turn ON LED

                // Keep ON for ~5 seconds or until reset
                for(int i = 0; i < 50; i++)
                {
                    if(resetFlag) break;   // Exit if reset occurs
                    __delay_ms(100);
                }

                Turn_Off_Buzzer();   // Turn OFF buzzer
                Turn_Off_Led();      // Turn OFF LED
            }

            //===================================================
            // RESET VALUES AFTER CHECK
            //===================================================
            digit1 = 0;         // Clear first digit
            digit2 = 0;         // Clear second digit
            TimeStamp = 0;      // Reset timer
            Sevenseg_Disp(0);   // Reset display to 0
        }

        __delay_ms(100);   // 100 ms delay (time base)
        TimeStamp++;       // Increment timer
    }
}