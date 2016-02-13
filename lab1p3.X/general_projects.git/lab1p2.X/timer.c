/*
 * File:   timer.c
 * Authors:
 *
 * Created on December 30, 2014, 8:07 PM
 */

#include <xc.h>
#include "timer.h"

void initTimer2(){
    TMR2 = 0;
    TMR3 = 0;
    T2CONbits.T32 = 1;
    T2CONbits.TCKPS = 0; //1 prescalar value 
    IFS0bits.T3IF = 0;
    T2CONbits.TON = 0;
    
    TRISGbits.TRISG15 = 0; //for testing
}

void delayUs(unsigned int delay){
    //TODO: Create a delay for "delay" micro seconds using timer 2
    int prVal = (8*delay - 1); //delay was 120-125% longer per clock than it should have been
    PR2 = prVal; //least sig bits
    PR3 = (prVal >> 16); //most sig bits
    T2CONbits.TON = 1;
    while(IFS0bits.T3IF == 0){}
    T2CONbits.TON = 0;
    IFS0bits.T3IF = 0;
}

void testTimer2(){
    LATGSET = 0x8000;
    delayUs(300);
    LATGINV = 0x8000;
    delayUs(300);
}