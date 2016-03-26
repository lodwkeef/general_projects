// ******************************************************************************************* //
//
// File:        main.c
// Date:        3/7/2016
// Authors:     Alex Thompson
//              Ben Schifman
//              Justin Siekmann
//              Kevin Curtis
//
// Created on March 7, 2016
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

//Define statements
#define OFF 0
#define ON 1
#define M1 1
#define M2 2

#define PRESS 0
#define RELEASE 1

typedef enum stateTypeEnum {
    forward, idle, backward,
} stateType;

volatile float dispVolt = 0;
volatile int direction = 1;//forward
volatile stateType state = idle;
volatile stateType prevstate = backward;
volatile int pressRelease = 0;
volatile int remap = 0;
volatile float ADCbuffer = 0;

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
    
    setMotorDirection(M1, 1); 
    setMotorDirection(M2, 1);
    while(1){      //Lab3 Part1
       switch(state){
            case forward:
                prevstate = forward; 
                ADCbuffer = getADCbuffer();
                if((dispVolt < ADCbuffer) && ((dispVolt + 1) <= ADCbuffer)){//to reduce excessive LCD updates
                    printVoltage(ADCbuffer);
                    dispVolt = ADCbuffer;
                }
                else if((dispVolt > ADCbuffer) && ((dispVolt - 1) >= ADCbuffer)){
                    printVoltage(ADCbuffer);
                    dispVolt = ADCbuffer;
                }
                moveCursorLCD(1,0);
                printStringLCD("forward ");
                if(remap == 1){
                    setMotorDirection(M1,1);
                    setMotorDirection(M2,1);
                    delayUs(1000);
                    remap = 0;
                }
                setMotorSpeed(ADCbuffer, direction);
                break;

            case backward:
                prevstate = backward;
                ADCbuffer = getADCbuffer();
                if((dispVolt < ADCbuffer) && ((dispVolt + 1) <= ADCbuffer)){//to reduce excessive LCD updates
                    printVoltage(ADCbuffer);
                    dispVolt = ADCbuffer;
                }
                else if((dispVolt > ADCbuffer) && ((dispVolt - 1) >= ADCbuffer)){
                    printVoltage(ADCbuffer);
                    dispVolt = ADCbuffer;
                }
                moveCursorLCD(1,0);
                printStringLCD("backward");
                if(remap == 1){
                    setMotorDirection(M1,0);
                    setMotorDirection(M2,0);
                    delayUs(1000);
                    remap = 0;
                }
                setMotorSpeed(ADCbuffer, direction);
                break;           

            case idle:
                unmapPins();
                ADCbuffer = getADCbuffer();
                if((dispVolt < ADCbuffer) && ((dispVolt + 1) <= ADCbuffer)){//to reduce excessive LCD updates
                    printVoltage(ADCbuffer);
                    dispVolt = ADCbuffer;
                }
                else if((dispVolt > ADCbuffer) && ((dispVolt - 1) >= ADCbuffer)){
                    printVoltage(ADCbuffer);
                    dispVolt = ADCbuffer;
                }
                moveCursorLCD(1,0);
                printStringLCD("Idle    ");
                delayUs(1000);
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
        if((state == forward) || (state == backward)) state = idle; 
        else if((state == idle) && (prevstate == forward)){ state = backward; direction = 0; remap = 1;}
        else if((state == idle) && (prevstate == backward)){ state = forward; direction = 1; remap = 1;}
        pressRelease = PRESS;
    }
    else{
        pressRelease = RELEASE;
    }
}
