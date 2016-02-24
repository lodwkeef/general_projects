/*
 * File:   timer.c
 * Authors:
 *
 * Created on December 30, 2014, 8:07 PM
 */

#include <xc.h>
#include "timer.h"

//Uses timer 2

void initTimer1()
{
    TMR1 = 0;
    PR1 = 49999;//5 msec timer
    T1CONbits.TCKPS = 0;//prescaler 1
    T1CONbits.TCS = 0;//internal PClck
    IEC0bits.T1IE = 1;//enable interrupts
    IFS0bits.T1IF = 0;//initialize flag to 0
    IPC1bits.T1IP = 7;//default priority level
    T1CONbits.TON = 0;//Timer is disabled
}