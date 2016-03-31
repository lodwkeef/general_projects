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



typedef enum stateTypeEnum {
    state1
} stateType;

volatile float ADCbuffer = 0;
volatile float dispVolt = 0;

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
    //float dispVolt =0;
    
    while(1){      //Lab3 Part1
        ADCbuffer = getADCbuffer();
        if((dispVolt < ADCbuffer) && ((dispVolt + 1) <= ADCbuffer)){//to reduce excessive LCD updates
            printVoltage(ADCbuffer);
            dispVolt = ADCbuffer;
        }
        else if((dispVolt > ADCbuffer) && ((dispVolt - 1) >= ADCbuffer)){
            printVoltage(ADCbuffer);
            dispVolt = ADCbuffer;
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