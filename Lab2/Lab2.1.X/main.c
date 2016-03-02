// ******************************************************************************************* //
//
// File:        main.c
// Date:        2/12/2016
// Authors:     Alex Thompson
//              Ben Schifman
//              Justin Siekmann
//              Kevin Curtis
//
// Created on February 25, 2016
//
// Description: Interfaces with a keypad to display the pressed numbers
// ******************************************************************************************* //

#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "interrupt.h"
#include "keypad.h"
#include "lcd.h"
#include "timer.h"

#define _XTAL_FREQ 10000000

//Define statements
#define OFF 0
#define ON 1
#define PRESS 1
#define RELEASE 0
#define PRESSED 1

#define CN_G 0x00040000
#define CN_C 0x00004000
#define CN_D 0x00008000
#define CN_E 0x00010000


typedef enum stateTypeEnum {
    wait, debounce, update
} stateType;

volatile stateType state = wait;
volatile int nextChange = PRESS;
volatile int row = -1;

//*************************************************************************8****************** //

int main(void){
    SYSTEMConfigPerformance(10000000);
    enableInterrupts();
    initTimerDebounce();
    initTimerDelay();
    initLCD();
    initKeypad();
    
    char key = NULL;
    int cursorRow = 0;
    int cursorCol = 0;
    moveCursorLCD(0,0);
    
    
    while(1){
        switch(state){
            case wait:
                break;
            case debounce:
                break;
            case update:
                if(row != -1){
                    key = scanKeypad(row);
                }
                else{
                    break;
                }
                if(key != -1){
                    printCharLCD(key);
                    if(cursorCol == 15){
                        if(cursorRow == 0){
                            moveCursorLCD(1,0);
                            cursorRow = 1;
                        }
                        else if(cursorRow == 1){
                            moveCursorLCD(0,0);
                            cursorRow = 0;
                        }
                        cursorCol = 0;
                    }
                    else{
                        cursorCol = cursorCol + 1;
                    }
                }
                else{
                    printCharLCD(' ');
                    moveCursorLCD(cursorRow, cursorCol);
                }
                state = wait;
                key = NULL;
                break;
        }
    }
    return 0;
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt() {
    int dummy; dummy = PORTG; dummy = PORTC; dummy = PORTD; dummy = PORTE; //dummy reads
    if(state == wait){
        switch(IFS1){
            case CN_G:
                row = 0;
                break;
            case CN_C:
                row = 1;
                break;
            case CN_D:
                row = 2;
                break;
            case CN_E:
                row = 3;
                break;
            default: //more than two rows were pressed
                row = -1;
                break;
        }
        state = debounce;
        T1CONbits.TON = ON;
    }
    IFS1bits.CNGIF = 0; IFS1bits.CNCIF = 0; IFS1bits.CNDIF = 0; IFS1bits.CNEIF = 0; //lower flags
}

void __ISR(_TIMER_1_VECTOR, IPL7SRS) _debounceInterrupt() { //Timer 1 is for debouncing
    IFS0bits.T1IF = 0; //lower the flag
    T1CONbits.TON = OFF;
    if(nextChange == PRESS){
        state = update;
        nextChange = RELEASE;
    }
    else if(nextChange == RELEASE){
        state = wait;
        nextChange = PRESS;
    }
}