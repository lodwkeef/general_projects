// ******************************************************************************************* //
//
// File:        main.c
// Date:        2/12/2016
// Authors:     Alex Thompson
//              Ben Schifman
//              Justin Siekmann
//              Kevin Curtis              
//
// Description: 
// ******************************************************************************************* //

#include <xc.h>
#include <sys/attribs.h>
#include "lcd.h"
#include "timer.h"
#include "config.h"
#include "interrupt.h"
#include "switch.h"
#include "leds.h"


#define PRESS 1
#define RELEASE 0
#define RUN 12
#define STOP 14
#define ON 1
#define OFF 0
#define RS 1
#define RESET 0

/* Please note that the configuration file has changed from lab 0.
 * the oscillator is now of a different frequency. FALSE
 */
typedef enum stateTypeEnum {
    running, stopped, reset, debounce, press
} stateType;

volatile int currLED = STOP;
volatile int prevAct = STOP;
volatile int pressRelease = -1;
volatile int switchP = -1;
volatile stateType state = reset;

// ******************************************************************************************* //


int main(void){
    SYSTEMConfigPerformance(10000000);
    enableInterrupts();
    
    initLEDs(); //Initializes the LEDs
    initTimer1(); //Initialize Timer1
    initTimer2();
    initSWext(); //Initialize external Switch
    initSW1();
    initLCD();
    
    while(1)
    {    
        switch(state){
            case reset:
                clearLCD();
                //need to make Stopwatch Timer
                state = stopped;
                break;
            case stopped:
                moveCursorLCD(0, 4);
                printStringLCD("Stopped:");
                turnOnLED(STOP);
                prevAct = STOP;
                pressRelease = PRESS; //next change will be a press
                //LCD Paused
                break;
            case running:
                moveCursorLCD(0, 4);
                printStringLCD("Running:");
                turnOnLED(RUN);
                prevAct = RUN;
                pressRelease = PRESS;
                //LCD Updating
                break;
            case debounce:
                break;
            case press:
                if(prevAct == RUN)
                {
                    //LCD Updating as well if it previously came from running
                }               
                pressRelease = RELEASE;
                
                break;
        }
    }
    return 0;
}


void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt() {
    int dummy;
    dummy = PORTA; // Read the Port A
    dummy = PORTD;
    T1CONbits.TON = ON;
    if(IFS1bits.CNAIF == 1) switchP = RS; //External Switch was Pressed
    else if(IFS1bits.CNDIF == 1) switchP = RESET; //Internal Switch was Pressed
    IFS1bits.CNAIF = 0; //lower flags
    IFS1bits.CNDIF = 0;
}

void __ISR(_TIMER_1_VECTOR, IPL7SRS) _T1Interrupt() { //Timer 1 is for debouncing
    IFS0bits.T1IF = 0; //lower the flag
    T1CONbits.TON = OFF;
    if(pressRelease == PRESS) state = press;
    else if(pressRelease == RELEASE && switchP == RESET) state = reset;
    else if(pressRelease == RELEASE && switchP == RS && prevAct == RUN) state = stopped;
    else if(pressRelease == RELEASE && switchP == RS && prevAct == STOP) state = running;
}

