/*
 * File:   timer.c
// Authors:     Alex Thompson
//              Ben Schifman
//              Justin Siekmann
//              Kevin Curtis
 * Created on March 7, 2016
 */
#include <xc.h>
#include <xc.h>
#include "timer.h"

void initTimerDebounce()
{
    TMR1 = 0;
    PR1 = 49999;
    T1CONbits.TCKPS = 0;//prescaler 1
    T1CONbits.TCS = 0;//internal PClck
    IEC0bits.T1IE = 1;//enable interrupts
    IFS0bits.T1IF = 0;//initialize flag to 0
    IPC1bits.T1IP = 7;//default priority level
    T1CONbits.TON = 0;//Timer is disabled
}

void initTimerDelay(){
    TMR2 = 0;
    TMR3 = 0;
    T2CONbits.T32 = 1;
    T2CONbits.TCKPS = 0; //1 prescalar value 
    IFS0bits.T3IF = 0;
    T2CONbits.TON = 0;
    
    TRISGbits.TRISG15 = 0; //for testing
}

void delayUs(unsigned int delay){
    //delay for "delay" micro seconds using timer 2
    int prVal = (8*delay - 1); //delay was 120-125% longer per clock than it should have been
    PR2 = prVal; //least sig bits
    PR3 = (prVal >> 16); //most sig bits
    T2CONbits.TON = 1;
    while(IFS0bits.T3IF == 0){}
    T2CONbits.TON = 0;
    IFS0bits.T3IF = 0;
}

void testTimerDelay(){
    LATGSET = 0x8000;
    delayUs(300);
    LATGINV = 0x8000;
    delayUs(300);
}