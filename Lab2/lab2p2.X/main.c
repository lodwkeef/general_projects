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

//Define statements
#define OFF 0
#define ON 1
#define PRESS 1
#define RELEASE 0
#define PRESSED 1
#define ENTER 0
#define SET 1
#define MATCH 0
#define NOTMATCH -1

#define CN_G 0x00040000
#define CN_C 0x00004000
#define CN_D 0x00008000
#define CN_E 0x00010000

#define TEST 1
#define NTEST 0


typedef enum stateTypeEnum {
    enter, debounce, update, set_mode, 
    good, bad, valid, invalid
} stateType;

volatile stateType state = enter;
volatile stateType prevState = enter;
volatile int nextChange = PRESS;
volatile int row = -1;
volatile int mode = -1;
volatile int keyPresses = 0;

//*************************************************************************8****************** //

int main(void){
    SYSTEMConfigPerformance(10000000);
    enableInterrupts();
    initTimerDebounce();
    initTimerDelay();
    initLCD();
    initKeypad();
    
    int runQATests = NTEST; //test keypad software functions if TEST, don't if NTEST
    if(runQATests == TEST){
        test_clearString();
        delayUs(2000000);
        clearLCD();
        test_checkPass();
        delayUs(2000000);
        clearLCD();
        test_updatePass();
        return -1;
    }
    
    char key = NULL;
    char passStringa[5] = "";
    char passStringb[5] = "";
    char passStringc[5] = "";
    char passStringd[5] = "";
    int PassCount = 0;
    int correct = NOTMATCH;
    char tempPass[5] = "";
    char guessString[5] = ""; 
    moveCursorLCD(0,0);
       
    while(1){
        switch(state){
            case enter:
                if(mode != ENTER || prevState != enter){
                    clearLCD();
                    printStringLCD("Enter");
                    moveCursorLCD(1,0);
                    mode = ENTER;
                }
                prevState = enter;
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
                    if(mode == ENTER){
                        guessString[keyPresses] = key;//append key to guessString
                        state = enter;
                    }
                    else if(mode == SET){
                        tempPass[keyPresses] = key;//append key to passString
                        state = set_mode;
                    }
                    keyPresses = keyPresses + 1;
                    if(mode == ENTER){
                        correct = NOTMATCH;
                        correct = checkPass(guessString, passStringa, passStringb, passStringc, passStringd);
                        if(keyPresses == 4 && (correct == NOTMATCH)){
                            state = bad;
                        }
                        else if(key == '#'){
                            state = bad;
                        }
                        else if((keyPresses > 1) && (keyPresses < 4) && (key == '*') && (strcmp(guessString, "**")!=0)){
                            state = bad;
                        }
                        else if((keyPresses==2)&&(strcmp(guessString,"**")==0)){
                            state = set_mode;
                        }
                        else if((keyPresses==2) && (key!='*')&&(guessString[0]=='*')){
                            state = bad;
                        }
                        else if((keyPresses==4)&&(correct == 0)){
                            state = good;
                        }                       
                    }
                    else if((keyPresses<4)&&(mode==SET)&&(key!='#')&&(key!='*')){
                        state = set_mode;
                    }
                    else if(((key=='#')||(key=='*'))&&(mode==SET)){
                        state = invalid;
                    }
                    else if((mode==SET)&&(keyPresses==4)&&(key!='#')&&(key!='*')){
                        state = valid;
                    }
                    else if(keyPresses < 4 && mode == ENTER && key != '#') state = enter;
                }
                else{
                    printCharLCD(' ');
                    moveCursorLCD(1, keyPresses);
                    if(mode == ENTER) state = enter;
                    else if(mode == SET) state = set_mode;
                }
                key = NULL;
                break;
            case set_mode:
                if(mode != SET || prevState != set_mode){
                    clearLCD();
                    printStringLCD("Set Mode");
                    moveCursorLCD(1,0);
                    keyPresses = 0;
                    strcpy(guessString, "");
                    clearString(guessString);
                    mode = SET;
                }
                prevState = set_mode;
                break;
            case good:
                clearLCD();
                printStringLCD("Good");
                keyPresses = 0;
                clearString(guessString);
                state = enter;
                prevState = good;
                delayUs(2000000);
                break;
            case bad:
                clearLCD();
                printStringLCD("Bad");
                keyPresses = 0;
                clearString(guessString);
                state = enter;
                prevState = bad;
                delayUs(2000000);
                break;
            case valid:
                clearLCD();
                if(PassCount == 3){
                    PassCount = 0;
                }
                else PassCount = PassCount + 1;
                updatePass(tempPass, passStringa, passStringb, passStringc, passStringd, PassCount);
                printStringLCD("Valid");
                keyPresses = 0;
                clearString(tempPass);
                mode = ENTER; 
                state = enter; 
                prevState = valid;
                delayUs(2000000);  
                break;
            case invalid:
                clearLCD();
                printStringLCD("Invalid");
                keyPresses = 0;
                clearString(tempPass);
                mode = ENTER;
                state = enter;
                prevState = invalid;
                delayUs(2000000);
                break;
        }
    }
    return 0;
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt(){
    int dummy; dummy = PORTG; dummy = PORTC; dummy = PORTD; dummy = PORTE; //dummy reads
    if(state == enter || state == set_mode){
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

void __ISR(_TIMER_1_VECTOR, IPL7SRS) _debounceInterrupt(){
    IFS0bits.T1IF = 0; //lower the flag
    T1CONbits.TON = OFF;
    if(nextChange == PRESS){
        state = update;
        nextChange = RELEASE;
    }
    else if(nextChange == RELEASE && mode==ENTER){
        state = enter;
        nextChange = PRESS;
    }
    else if(nextChange == RELEASE && mode==SET){
        state = set_mode;
        nextChange = PRESS;
    }
}