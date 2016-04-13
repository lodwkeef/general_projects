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

//Define statements
#define OFF 0
#define ON 1
#define M1 1
#define M2 2

#define RFULL 1023
#define R90 921
#define R60 614
#define RSTOP 0

#define LFULL 1023
#define L90 921
#define L60 614
#define LSTOP 0

#define FORWARD 1
#define BACKWARD 0

#define PRESS 0
#define RELEASE 1
#define YES 1
#define NOPE 0

typedef enum stateTypeEnum {
    idle, forward, left, hardLeft, superHardLeft, right, hardRight, superHardRight,
} stateType;

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
    
    float ADCarrayAveraged[4];
    int rSpeed = 0;
    int lSpeed = 0;
    int direction = FORWARD; //forward
    
    stateType nextState = idle;
    
    setMotorDirection(M1, direction); 
    setMotorDirection(M2, direction);
    //why no delay
    while(1){
       switch(state){
            case forward:
                lSpeed = LFULL; rSpeed = RFULL;
                direction = FORWARD;
                moveCursorLCD(0,0);
                printStringLCD("Forward");
                if(remap == YES){
                    setMotorDirection(M1,direction);
                    setMotorDirection(M2,direction);
                    delayUs(1000); //why the delay?
                    remap = NOPE;
                }
                setMotorSpeed(rSpeed, lSpeed, direction);
                getADCbuffer(&ADCarrayAveraged);
                nextState = calcNextState(ADCarrayAveraged);
                break;
            case left:
                lSpeed = LFULL; rSpeed = R90;
                direction = FORWARD;
                moveCursorLCD(0,0);
                printStringLCD("Left");
                setMotorSpeed(rSpeed, lSpeed, direction);
                getADCbuffer(&ADCarrayAveraged);
                nextState = calcNextState(ADCarrayAveraged);
                break;
            case hardLeft:
                lSpeed = LFULL; rSpeed = R60;
                direction = FORWARD;
                moveCursorLCD(0,0);
                printStringLCD("Hard Left");
                setMotorSpeed(rSpeed, lSpeed, direction);
                getADCbuffer(&ADCarrayAveraged);
                nextState = calcNextState(ADCarrayAveraged);
                break;
            case superHardLeft:
                lSpeed = LFULL; rSpeed = RSTOP;
                direction = FORWARD;
                moveCursorLCD(0,0);
                printStringLCD("Super Hard Left");
                setMotorSpeed(rSpeed, lSpeed, direction);
                getADCbuffer(&ADCarrayAveraged);
                nextState = calcNextState(ADCarrayAveraged);
                break;
            case right:
                lSpeed = L90; rSpeed = RFULL;
                direction = FORWARD;
                moveCursorLCD(0,0);
                printStringLCD("Right");
                setMotorSpeed(rSpeed, lSpeed, direction);
                getADCbuffer(&ADCarrayAveraged);
                nextState = calcNextState(ADCarrayAveraged);
                break;
            case hardRight:
                lSpeed = L60; rSpeed = RFULL;
                direction = FORWARD;
                moveCursorLCD(0,0);
                printStringLCD("Hard Right");
                setMotorSpeed(rSpeed, lSpeed, direction);
                getADCbuffer(&ADCarrayAveraged);
                nextState = calcNextState(ADCarrayAveraged);
                break;
            case superHardRight:
                lSpeed = LSTOP; rSpeed = RFULL;
                direction = FORWARD;
                moveCursorLCD(0,0);
                printStringLCD("Super Hard Right");
                setMotorSpeed(rSpeed, lSpeed, direction);
                getADCbuffer(&ADCarrayAveraged);
                nextState = calcNextState(ADCarrayAveraged);
                break;
            case idle:
                unmapPins();
                clearLCD();
                printStringLCD("Idle Mode");
                delayUs(1000); //why the delay?
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