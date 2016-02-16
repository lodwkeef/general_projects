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
#include <math.h>
#include <string.h>


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
    int currTime = 0;
    float currSeconds = 0;
    char currChar;
    
    while(1)
    {    
        switch(state){
            case reset:
                clearLCD();
                T4CONbits.TON = 0; //turn off timer
                TMR4 = 0;   //reset timer values to 0
                TMR5 = 0;
                //need to make Stopwatch Timer
                state = stopped;
                break;
            case stopped:
                T4CONbits.TON = 0; //turn off timer
                moveCursorLCD(0, 4);
                printStringLCD("Stopped:");
                turnOnLED(STOP);
                prevAct = STOP;
                pressRelease = PRESS; //next change will be a press
                //LCD Paused
                break;
            case running:
                turnOnLED(RUN);
                if(T4CONbits.TON == 0) T4CONbits.TON = 1; //if timer is off, turn it on
                moveCursorLCD(0, 4);  //set cursor to the top row
                printStringLCD("Running:");
                currTime = 0;
                currTime = (float)((PR5 << 16)| PR4); //combines timer values
                currSeconds = floor(currTime/8000)/1000; //probably unnecessary to cutoff the micro seconds, but w/e
                moveCursorLCD(1, 4);  //set cursor to the bottom row
                sprintf(currChar, "%2.3f", currSeconds) //convert the time to a string
                printStringLCD(currChar); //prints the current time
                prevAct = RUN;
                pressRelease = PRESS;
                //LCD Updating
                break;
            case debounce:
                break;
            case press:
                if(prevAct == RUN)
                {
                    moveCursorLCD(0, 4);  //set cursor to the top row
                    printStringLCD("Running:");
                    currTime = 0;
                    currTime = (float)((PR5 << 16)| PR4); //combines timer values
                    currSeconds = floor(currTime/8000)/1000; //probably unnecessary to cutoff the micro seconds, but w/e
                    moveCursorLCD(1, 4);  //set cursor to the bottom row
                    sprintf(currChar, "%2.3f", currSeconds) //convert the time to a string
                    printStringLCD(currChar); //prints the current time
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
    IFS1bits.CNAIF = 0; //lower flags4
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

