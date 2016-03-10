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


//*************************************************************************8****************** //

int main(void){
    SYSTEMConfigPerformance(10000000);
    enableInterrupts();
    initTimerDebounce();
    initTimerDelay();
    initLCD();
    initPWM();
    initADC();
    
    while(1){
        //testM1();
    }
    
    
    
    return 0;
}

