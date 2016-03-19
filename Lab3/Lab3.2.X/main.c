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

typedef enum stateTypeEnum {
    forward, idle, backward,
} stateType;

float dispVolt = 0;
int direction = 1;//forward
volatile stateType state = forward;
volatile stateType prevstate = forward;
volatile int change = 0;
float ADCbuffer = 0;
//*************************************************************************8****************** //

int main(void){
    SYSTEMConfigPerformance(10000000);
  //enableInterrupts();
    initTimer1();
    initTimer2();
    initTimer45();
    initLCD();
    initPWM();
    initADC();
        
    //initialize motors to forward position
    unmapPins();
    setMotorDirection(M1, 1); 
    setMotorDirection(M2, 1);
        while(1){      //Lab3 Part1
            //dispVolt = UpdateLCDVoltage(dispVolt);
           switch(state){
                case forward:
                    prevstate = forward; 
                    ADCbuffer = getADCbuffer();
                    setMotorSpeed(ADCbuffer, direction);
                    break;

                case backward:
                    prevstate = backward;
                    ADCbuffer = getADCbuffer();
                    setMotorSpeed(ADCbuffer, direction);
                    break;           
               
                case idle:
                    unmapPins();
                    break;
        }
        
          testM1forward();
          delayUs(100);
          testM1reverse();
          testM2forward();
          delayUs(100);
          testM2reverse();          
    }
    

    
    
    return 0;
}


void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt() {
    int dummy;
    dummy = PORTA; // Read the Port A
    if(change){
        if((state == forward) || (state == backward)) state = idle; 
        else if((state == idle) && (prevstate == forward)){ state = backward; direction = 1;}
        else if((state == idle) && (prevstate == backward)){ state = forward; direction = 0;}
    }
    change ^= 1;
    IFS1bits.CNAIF = 0; //lower flag
}

//void __ISR(_ADC_VECTOR, IPL7SRS) _ADCInterrupt(void){
//    int k = 0;
//    IFS0bits.AD1IF = 0;
//    //while(AD1CON1bits.SSRC == 0);
//    k = AD1CON1bits.SAMP;
//    //while(AD1CON1bits.SAMP == 0);
//    while(AD1CON1bits.DONE == 0);
//    k = AD1CON1bits.SAMP;
//    ADCNum = ADC1BUF0;  //output the current ADC value to a volatile variable
//    AD1CON1bits.ON = 0;
//}