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

volatile unsigned int ADCNum = 0;


//*************************************************************************8****************** //

int main(void){
    SYSTEMConfigPerformance(10000000);
    //enableInterrupts();
    initTimer1();
    initTimer2();
    initLCD();
    initPWM();
    initADC();

    char s[9] = {};
    float currVolt =0;
    
    while(1){       //testing to see if anything actually works
//         OC1RS=750; //trying to just force a value to make motor move
//         //OC1RS=(1000*ADCNum)/1023;//this little while loop will just run both motors together in the same direction for testing
//         OC2RS=(1000*ADCNum)/1023;

          testM1();
    }
    
//    while(1){      //Lab3 Part1
//        clearLCD();
//        moveCursorLCD(0, 0);
//        printStringLCD("Voltage");
//        currVolt = (5*ADCNum)/1023;
//        sprintf(s,"Voltage:%04f", currVolt);
//        printStringLCD(s);
//        delayUs(1000);
//    }
    
    
    return 0;
}

void __ISR(_ADC_VECTOR, IPL7AUTO) _ADCInterrupt(void){//interrupts for ADC trigger constantly, need to fix; disabled interrupts -BAS
    IFS0bits.AD1IF = 0;
    while(AD1CON1bits.SSRC == 0);
    ADCNum = ADC1BUF0;  //output the current ADC value to a volatile variable
}