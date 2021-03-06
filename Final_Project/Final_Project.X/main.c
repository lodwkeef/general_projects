// ******************************************************************************************* //
//
// File:        main.c
// Date:        3/28/2016
// Authors:     Alex Thompson
//              Ben Schifman
//              Justin Siekmann
//              Kevin Curtis
//
// Created on March 28, 2016
//
// Description: Motor Go Vrrooom Vrooooom.
// ******************************************************************************************* //

#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "interrupt.h"
#include "adc.h"
#include "pwm.h"
#include "lcd.h"
#include "timer.h"
#include "motor.h"
#include "line.h"
#include "switch.h"

//Define statements
#define OFF 0
#define ON 1
#define M1 1
#define M2 2

#define RFULL 500
#define R90 250
#define R60 175
#define RSTOP 0

#define LFULL 500
#define L90 250
#define L60 175
#define LSTOP 0

#define delayspin90  200000
#define delayspin180 650000

#define SPIN 700

#define FORWARD 1
#define BACKWARD 0

#define PRESS 0
#define RELEASE 1
#define YES 1
#define NOPE 0

//for reference: this is defined in line.h in order to use in multiple files
//typedef enum stateTypeEnum {
//    idle, forward, left, hardLeft, superHardLeft, right, hardRight, superHardRight, spin90, spin180, test
//} stateType;

volatile stateType state = idle;
volatile int pressRelease = PRESS;
volatile int remap = NOPE;

//*************************************************************************8****************** //

int main(void){
    SYSTEMConfigPerformance(10000000);
    enableInterrupts();
    initTimer1();
    initTimer2();
    initTimer45();
    initSW1();
    initLCD();
    initPWM();
    initADC();
    
    float ADCarrayAveraged[4] = {0,0,0,0};
    float volt[4] = {0,0,0,0};
    char s[32];
    int rSpeed = 0;
    int lSpeed = 0;
    int leftDirection = FORWARD; //forward
    int rightDirection = FORWARD; //forward
    int spin90Count = 0;
    int innerLoop = NOPE;
    int spun = NOPE;
    
    stateType prevState = forward;
    
    setMotorDirection(M1, leftDirection); 
    setMotorDirection(M2, rightDirection);
    while(1){
       switch(state){
            case forward:
                if(state != prevState){
                    lSpeed = LFULL; rSpeed = RFULL;
                    leftDirection = FORWARD;
                    rightDirection = FORWARD;
                    clearLCD();
                    printStringLCD("Forward");
                    if(remap == YES){
                        setMotorDirection(M1,leftDirection);
                        setMotorDirection(M2,rightDirection);
                        remap = NOPE;
                    }
                    setMotorSpeed(lSpeed, rSpeed, leftDirection, rightDirection);
                }
                getADCbuffer(ADCarrayAveraged);
                if(state != idle){
                    state = calcNextState(ADCarrayAveraged, volt, innerLoop, spun);
                }
                moveCursorLCD(1,0);
                sprintf(s, "%.1f %.1f %.1f %.1f", volt[3], volt[2], volt[1], volt[0]);
                printStringLCD(s);
                prevState = forward;
                break;
            case left:
                if(state != prevState){
                    lSpeed = L90; rSpeed = RFULL;
                    leftDirection = FORWARD;
                    rightDirection = FORWARD;
                    clearLCD();
                    printStringLCD("Left");
                    setMotorSpeed(lSpeed, rSpeed, leftDirection, rightDirection);
                }
                getADCbuffer(ADCarrayAveraged);
                if(state != idle){
                    state = calcNextState(ADCarrayAveraged, volt, innerLoop, spun);
                }
                moveCursorLCD(1,0);
                sprintf(s, "%.1f %.1f %.1f %.1f", volt[3], volt[2], volt[1], volt[0]);
                printStringLCD(s);
                prevState = left;
                break;
            case hardLeft:
                if(state != prevState){
                    lSpeed = L60; rSpeed = RFULL;
                    leftDirection = FORWARD;
                    rightDirection = FORWARD;
                    clearLCD();
                    printStringLCD("Hard Left");
                    setMotorSpeed(lSpeed, rSpeed, leftDirection, rightDirection);
                }
                getADCbuffer(ADCarrayAveraged);
                if(state != idle){
                    state = calcNextState(ADCarrayAveraged, volt, innerLoop, spun);
                }
                moveCursorLCD(1,0);
                sprintf(s, "%.1f %.1f %.1f %.1f", volt[3], volt[2], volt[1], volt[0]);
                printStringLCD(s);
                prevState = hardLeft;
                break;
            case superHardLeft:
                if(state != prevState){
                    lSpeed = LSTOP; rSpeed = RFULL;
                    leftDirection = FORWARD;
                    rightDirection = FORWARD;
                    clearLCD();
                    printStringLCD("Super Hard Left");
                    setMotorSpeed(lSpeed, rSpeed, leftDirection, rightDirection);
                }
                getADCbuffer(ADCarrayAveraged);
                if(state != idle){
                    state = calcNextState(ADCarrayAveraged, volt, innerLoop, spun);
                }
                moveCursorLCD(1,0);
                sprintf(s, "%.1f %.1f %.1f %.1f", volt[3], volt[2], volt[1], volt[0]);
                printStringLCD(s);
                prevState = superHardLeft;
                break;
            case right:
                if(state != prevState){
                    lSpeed = LFULL; rSpeed = R90;
                    leftDirection = FORWARD;
                    rightDirection = FORWARD;
                    clearLCD();
                    printStringLCD("Right");
                    setMotorSpeed(lSpeed, rSpeed, leftDirection, rightDirection);
                }
                getADCbuffer(ADCarrayAveraged);
                if(state != idle){
                    state = calcNextState(ADCarrayAveraged, volt, innerLoop, spun);
                }
                moveCursorLCD(1,0);
                sprintf(s, "%.1f %.1f %.1f %.1f", volt[3], volt[2], volt[1], volt[0]);
                printStringLCD(s);
                prevState = right;
                break;
            case hardRight:
                if(state != prevState){
                    lSpeed = LFULL; rSpeed = R60;
                    leftDirection = FORWARD;
                    rightDirection = FORWARD;
                    clearLCD();
                    printStringLCD("Hard Right");
                    setMotorSpeed(lSpeed, rSpeed, leftDirection, rightDirection);
                }
                getADCbuffer(ADCarrayAveraged);
                if(state != idle){
                    state = calcNextState(ADCarrayAveraged, volt, innerLoop, spun);
                }
                moveCursorLCD(1,0);
                sprintf(s, "%.1f %.1f %.1f %.1f", volt[3], volt[2], volt[1], volt[0]);
                printStringLCD(s);
                prevState = hardRight;
                break;
            case superHardRight:
                if(state != prevState){
                    lSpeed = LFULL; rSpeed = RSTOP;
                    leftDirection = FORWARD;
                    rightDirection = FORWARD;
                    clearLCD();
                    printStringLCD("Super Hard Right");
                    setMotorSpeed(lSpeed, rSpeed, leftDirection, rightDirection);
                }
                getADCbuffer(ADCarrayAveraged);
                if(state != idle){
                    state = calcNextState(ADCarrayAveraged, volt, innerLoop, spun);
                }
                moveCursorLCD(1,0);
                sprintf(s, "%.1f %.1f %.1f %.1f", volt[3], volt[2], volt[1], volt[0]);
                printStringLCD(s);
                prevState = superHardRight;
                break;
            case spin90:
                moveCursorLCD(0,0);
                printStringLCD("TUUUUUUUUUURN 90");
                lSpeed = LSTOP; rSpeed = RSTOP;
                leftDirection = FORWARD;
                rightDirection = FORWARD;
                setMotorSpeed(lSpeed, rSpeed, leftDirection, rightDirection);
                lSpeed = SPIN; rSpeed = SPIN;
                //setMotorDirection(M2, rightDirection);
                setMotorSpeed(lSpeed, rSpeed, leftDirection, rightDirection);
                delayUs(delayspin90);
                //rightDirection = FORWARD;
                //setMotorDirection(M2, rightDirection);
                lSpeed = LSTOP; rSpeed = RSTOP;
                setMotorSpeed(lSpeed, rSpeed, leftDirection, rightDirection);
                getADCbuffer(ADCarrayAveraged);
                if(state != idle){
                    if(spin90Count < 3){
                        innerLoop = YES;
                        spin90Count = spin90Count + 1;
                    }
                    else{
                        innerLoop = NOPE;
                    }
                    state = calcNextState(ADCarrayAveraged, volt, innerLoop, spun);
                }
                moveCursorLCD(1,0);
                sprintf(s, "%.1f %.1f %.1f %.1f", volt[3], volt[2], volt[1], volt[0]);
                printStringLCD(s);
                prevState = spin90;
            break;
            case spin180:
                spun = YES;
                moveCursorLCD(0,0);
                printStringLCD("SPINNNNNNNAH 180");
                lSpeed = LSTOP; rSpeed = RSTOP;
                leftDirection = FORWARD;
                rightDirection = BACKWARD;
                setMotorSpeed(lSpeed, rSpeed, leftDirection, rightDirection);
                lSpeed = SPIN; rSpeed = SPIN;
                setMotorDirection(M2, rightDirection);
                setMotorSpeed(lSpeed, rSpeed, leftDirection, rightDirection);
                delayUs(delayspin180);
                rightDirection = FORWARD;
                setMotorDirection(M2, rightDirection);
                lSpeed = LSTOP; rSpeed = RSTOP;
                setMotorSpeed(lSpeed, rSpeed, leftDirection, rightDirection);
                getADCbuffer(ADCarrayAveraged);
                if(state != idle){
                    state = calcNextState(ADCarrayAveraged, volt, innerLoop, spun);
                }
                moveCursorLCD(1,0);
                sprintf(s, "%.1f %.1f %.1f %.1f", volt[3], volt[2], volt[1], volt[0]);
                printStringLCD(s);
                prevState = spin180;
            break;   
            case idle:
                if(state != prevState){
                    unmapPins();
                    clearLCD();
                    printStringLCD("Idle Mode");
                }
                innerLoop = NOPE;
                spin90Count = 0;
                prevState = idle;
            break;
            case test:
                testTimerDelay();
            break;
            case spin90Left:
                moveCursorLCD(0,0);
                printStringLCD("TUUUUUUUUURN L90");
                lSpeed = LSTOP; rSpeed = RSTOP;
                leftDirection = FORWARD;
                rightDirection = FORWARD;
                setMotorSpeed(lSpeed, rSpeed, leftDirection, rightDirection);
                lSpeed = SPIN; rSpeed = SPIN;
                //setMotorDirection(M1, leftDirection);
                setMotorSpeed(lSpeed, rSpeed, leftDirection, rightDirection);
                delayUs(delayspin90);
                //leftDirection = FORWARD;
                //setMotorDirection(M1, leftDirection);
                lSpeed = LSTOP; rSpeed = RSTOP;
                setMotorSpeed(lSpeed, rSpeed, leftDirection, rightDirection);
                getADCbuffer(ADCarrayAveraged);
                if(state != idle){
                    state = calcNextState(ADCarrayAveraged, volt, innerLoop, spun);
                }
                moveCursorLCD(1,0);
                sprintf(s, "%.1f %.1f %.1f %.1f", volt[3], volt[2], volt[1], volt[0]);
                printStringLCD(s);
                prevState = spin90;
            break;
        }
    }  
    return 0;
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt(){
    int dummy;
    dummy = PORTA; // Read the Port A
    T1CONbits.TON = ON;
    IFS1bits.CNAIF = 0; //lower flag
}

void __ISR(_TIMER_1_VECTOR, IPL7SRS) _T1Interrupt() { //Timer 1 is for debouncing
    IFS0bits.T1IF = 0; //lower the flag
    T1CONbits.TON = OFF;
    if(pressRelease == RELEASE){
        if(state != idle) state = idle; 
        else if(state == idle){ state = forward; remap = YES;}
        pressRelease = PRESS;
    }
    else{
        pressRelease = RELEASE;
    }
}