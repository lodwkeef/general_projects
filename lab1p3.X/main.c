// ******************************************************************************************* //
//
// File:         lab1p2.c
// Date:         12-30-2014
// Authors:      Garrett Vanhoy
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

// ******************************************************************************************* //


int main(void)
{
    SYSTEMConfigPerformance(10000000);
    enableInterrupts();
    initLEDs(); //Initializes the LEDs
    initTimer2();
    initLCD();
    initTimer1(); //Initialize Timer1
    initSWext(); //Initialize SW1
    
    while(1)
    {
        
        
        //testTimer2();
        //testLCD();
    }
    
    return 0;
}