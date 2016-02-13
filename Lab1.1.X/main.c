// ******************************************************************************************* //
//
// File:         lab1p1.c
// Date:         
// Authors:      
//
// Description: Part 1 for lab 1
// ******************************************************************************************* //

#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "leds.h"
#include "interrupt.h"
#include "switch.h"
#include "timer.h"

#define PRESS 1
#define RELEASE 0
#define RUN 12
#define STOP 14
#define ON 1
#define OFF 0

/* Please note that the configuration file has changed from lab 0.
 * the oscillator is now of a different frequency.
 */
typedef enum stateTypeEnum {
    debounce, press,
    STOP_LED, RUN_LED
} stateType;

volatile int currLED = RUN;//CHECK TO SEE IF STOP (defined in leds) IS AN INT
volatile int next = PRESS;
volatile stateType state = RUN_LED;

int main(void) {
    SYSTEMConfigPerformance(10000000);
    enableInterrupts();
    initLEDs(); //Initializes the LEDs
    initTimer1(); //Initialize Timer1
    initSW2(); //Initialize SW1


    while(1){
        //TODO: Using a finite-state machine, define the behavior of the LEDs
        //Debounce the switch
        switch (state) {
            
            case RUN_LED:
                turnOnLED(RUN);
                currLED = RUN;
                T1CONbits.TON = 0;//Timer is disabled
                TMR1 = 0;
                next = PRESS;
                break;
                
            case STOP_LED:
                turnOnLED(STOP); //CHECK TO SEE IF I AM REUSING VARIABLES IMPROPERLY
                currLED = STOP;
                T1CONbits.TON = OFF;//Timer is disabled
                TMR1 = 0; 
                next = PRESS;
                break;
                
            case debounce:
                T1CONbits.TON = ON;//Timer is enabled              
                break;
                
            case press:
                T1CONbits.TON = OFF;//Timer is disabled
                TMR1 = 0;
                next = RELEASE;
                break;
        }
    }

    return 0;
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt() {
    int dummy;
    dummy = PORTA; // Read the Port A
    IFS1bits.CNAIF = 0; //Set flag to lowered

    TMR1 = 0; //reset timer 1


    if (state == STOP_LED || state == RUN_LED || state == press) state = debounce;
    
}

void __ISR(_TIMER_1_VECTOR, IPL7SRS) _T1Interrupt() {
    IFS0bits.T1IF = 0; //lower the flag 
    TMR1 = 0;
    if(next == PRESS) state = press;
    else if(next != PRESS && currLED == STOP) state = RUN_LED;
    else if(next != PRESS && currLED == RUN) state = STOP_LED;
}


