// ******************************************************************************************* //
//
// File:        timer.c
// Date:        3/28/2016
// Authors:     Alex Thompson
//              Ben Schifman
//              Justin Siekmann
//              Kevin Curtis
//
// Created on March 28, 2016
//
// Description: Motor Go Vrrooom Vrooooom.
// ******************************************************************************************* //
#include <xc.h>
#include "timer.h"

void initTimer1(){//debounce timer
    TMR1 = 0;
    PR1 = 49999;
    T1CONbits.TCKPS = 0;//prescaler 1
    T1CONbits.TCS = 0;//internal PClck
    IEC0bits.T1IE = 1;//enable interrupts
    IFS0bits.T1IF = 0;//initialize flag to 0
    IPC1bits.T1IP = 7;//default priority level
    T1CONbits.TON = 0;//Timer is disabled
}

//void initTimer2(){//timer for PWM
//    TMR2 = 0;
//    PR2 = 1023;
//    T2CONbits.TCKPS = 0;
//    T2CONbits.TON = 0;
//    T2CONbits.TON = 1;
//}

void initTimer45(){//timer for delayUs
    TMR4 = 0; 
    TMR5 = 0; 
    T4CONbits.T32 = 1; 
    T4CONbits.TCKPS = 0; //1 prescalar value  
    IFS0bits.T5IF = 0; 
    T4CONbits.TON = 0; 
       
    TRISAbits.TRISA6 = 0; //for testing
}

void delayUs(unsigned int delay){//using timers 4 and 5
    //TODO: Create a delay for "delay" micro seconds using timer 4/5
    int prVal = (10*delay - 1); 
    PR4 = prVal; //least sig bits
    PR5 = (prVal >> 16); //most sig bits
    T4CONbits.TON = 1;
    while(IFS0bits.T5IF == 0){}
    T4CONbits.TON = 0;
    IFS0bits.T5IF = 0;
}

void testTimerDelay(){
    LATAbits.LATA6 = 1;
    delayUs(1000);
    LATAbits.LATA6 = 0;
    delayUs(1000);
}

