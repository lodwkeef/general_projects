/* 
 * File:   motor.c
// Authors:     Alex Thompson
//              Ben Schifman
//              Justin Siekmann
//              Kevin Curtis
 *
 * Created on March 10, 2016
 */
#include <xc.h>
#include "motor.h"
#include "timer.h"

#define LATD_0      LATDbits.LATD0
#define LATD_1      LATDbits.LATD1
#define LATD_2      LATDbits.LATD2
#define LATD_3      LATDbits.LATD3
#define Pin_0       RPD0Rbits.RPD0R
#define Pin_1       RPD1Rbits.RPD1R
#define Pin_2       RPD2Rbits.RPD2R
#define Pin_3       RPD3Rbits.RPD3R
#define OFF         0
#define LOW         0

void testM1(){
    int i = 0;
    OC1CONbits.ON = OFF;           //Turn off OC1 while doing setup.
    Pin_2 = 0b0000;            // map 'No connect' to RD2
    Pin_0 = 0b1100;            // map OC1 to RD0
    LATD_2 = LOW;              //set D2 to common
    OC1CONbits.ON = 1;         //Turn on OC1 while doing setup.
    delayUs(1000);
    for (i=200; i<1023; i++){//VRROOOOM UP
        setPWM1(i); 
        delayUs(10000);
    }
    for (i=1023; i>0; i--){//VROOOM DOWN
        setPWM1(i);
        delayUs(20000);
    }
}

void testM2(){
    int i = 0;
    for (i=0; i++; i<1023) setPWM2(i); delayUs(100000);
}
