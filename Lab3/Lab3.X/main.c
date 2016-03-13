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
#define PRESS 1
#define RELEASE 0
#define PRESSED 1
#define ENTER 0
#define SET 1
#define MATCH 0
#define NOTMATCH -1

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
volatile unsigned int ADCval = 0;


//*************************************************************************8****************** //

int main(void){
    SYSTEMConfigPerformance(10000000);
    enableInterrupts();
    initTimer1();
    initTimer2();
    initLCD();
    initPWM();
    initADC();
  
    T2CONbits.TON = 1;
    
    while(1){
        //testM1();
        testLCD();
//        OC1RS=998; //trying t just force a value to make motor move, not going
//        
//         //OC1RS=(1000*ADCNum)/1023;//this little while loop will just run both motors together in the same direction for testing
//         OC2RS=(1000*ADCval)/1023;
    }
    
    
    
    return 0;
}

void __ISR(_ADC_VECTOR, IPL7AUTO) _ADCInterrupt(void){
    IFS0bits.AD1IF = 0;
    ADCval = ADC1BUF0;
}