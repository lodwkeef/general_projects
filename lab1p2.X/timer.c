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
    IPC3bits.T3IP = 7;
    IFS0bits.T3IF = 0;
    //IEC0bits.T3IE = 1;
    T2CONbits.TON = 0;
}

void delayUs(unsigned int delay){
    //TODO: Create a delay for "delay" micro seconds using timer 2
    int prVal = 10*delay - 1;
    PR2 = (prVal & 0xFFFF); //least sig bits
    PR3 = (prVal >> 16); //most sig bits
    T2CONbits.TON = 1;
    while(IFS0bits.T3IF == 0){}
    T2CONbits.TON = 0;
    IFS0bits.T3IF = 0;
}