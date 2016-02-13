/*
 * File:   leds.c
 * Author: 
 *
 * Created on December 27, 2014, 1:31 PM
 */

#include <xc.h>
#include "leds.h"
#define RUN 12
#define STOP 14
#define ON 1
#define OFF 0

void initLEDs() {
    //Tristates are outputs for LEDs
    TRISGbits.TRISG14 = OUTPUT;
    TRISGbits.TRISG12 = OUTPUT;
    ODCGbits.ODCG12 = ON; //Turn on ODC for RUN
    ODCGbits.ODCG14 = ON; //Turn on ODC for STOP

    //initialize RUN to OFF & STOP to ON
    LATGbits.LATG14 = 0; //STOP LED
    LATGbits.LATG12 = 1; //RUN LED
}

void turnOnLED(int led) {
    if (led == RUN) {//Only RUN will turn on
        LATGbits.LATG14 = 0; //STOP LED
        LATGbits.LATG12 = 1; //RUN LED
    }
    else if (led == STOP) {//Only STOP will turn on
        LATGbits.LATG14 = 1; //STOP LED
        LATGbits.LATG12 = 0; //RUN LED
    }
}