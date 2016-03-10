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

void initTimer1()
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

void initTimer2(){
    TMR2 = 0;
    PR2 = 1000;
    T2CONbits.T32 = 0;
    T2CONbits.TCKPS = 0b100; //16 prescalar value 
    T2CONbits.TCS = 0;//internal PClck
    IEC0bits.T2IE = 1;//enable interrupts
    IFS0bits.T2IF = 0;//initialize flag to 0
    T2CONbits.TON = 0;
    
    TRISGbits.TRISG15 = 0; //for testing
}

/*          //actually we don't need timer 3, unless you want to use it to debounce -kdc
void initTimer3(){      //also I didn't finish setting this up....so good luck if you want it
    TMR3 = 0;
    PR3 = 1000;
    T3CONbits.T32 = 0;
    T3CONbits.TCKPS = 0b100; //16 prescalar value 
    T3CONbits.TCS = 0;//internal PClck
    IFS0bits.T3IF = 0;
    T3CONbits.TON = 0;
    
    TRISGbits.TRISG15 = 0; //for testing
}
*/

void delayUs(unsigned int delay){      
    //delay for "delay" micro seconds using timer 2
    int prVal = (8*delay - 1); //delay was 120-125% longer per clock than it should have been   -kdc
    PR1 = prVal; //least sig bits
    T1CONbits.TON = 1;
    while(IFS0bits.T1IF == 0){}
    T1CONbits.TON = 0;
    IFS0bits.T1IF = 0;
}

/*
void delayUs(unsigned int delay){       //*******************we need timer 2 and 3 for the PWM, delay has to be changed to TMR1***********
    //delay for "delay" micro seconds using timer 2
    int prVal = (8*delay - 1); //delay was 120-125% longer per clock than it should have been
    PR2 = prVal; //least sig bits
    PR3 = (prVal >> 16); //most sig bits
    T2CONbits.TON = 1;
    while(IFS0bits.T3IF == 0){}
    T2CONbits.TON = 0;
    IFS0bits.T3IF = 0;
}
*/
void testTimerDelay(){
    LATGSET = 0x8000;
    delayUs(300);
    LATGINV = 0x8000;
    delayUs(300);
}