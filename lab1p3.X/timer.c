/*
 * File:   timer.c
// Authors:     Alex Thompson
//              Ben Schifman
//              Justin Siekmann
//              Kevin Curtis
 * Created on December 30, 2014, 8:07 PM
 */

#include <xc.h>
#include "timer.h"

void initTimer1()
{
    TMR1 = 0;
    PR1 = 49999;//1 second timer
    T1CONbits.TCKPS = 0;//prescaler 1
    T1CONbits.TCS = 0;//internal PClck
    IEC0bits.T1IE = 1;//enable interrupts
    IFS0bits.T1IF = 0;//initialize flag to 0
    IPC1bits.T1IP = 7;//default priority level
    T1CONbits.TON = 0;//Timer is disabled
}

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

void initTimerWatch(){
    T4CONbits.T32 = 1;
    TMR4 = 0;
    TMR5 = 0;
    PR4 = 0xFFFF; //least sig bits
    PR5 = 0xFFFF; //most sig bits
    T4CONbits.TON = 0;
    T4CONbits.TCKPS = 7; //256 prescaler
    IFS0bits.T5IF = 0;
    IPC5bits.T5IP = 7;
}


void testTimer2(){
    LATGSET = 0x8000;
    delayUs(300);
    LATGINV = 0x8000;
    delayUs(300);
}