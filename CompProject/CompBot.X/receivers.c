// ******************************************************************************************* //
//
// File:        receivers.c
// Date:        5/1/2016
// Authors:     Alex Thompson
//              Ben Schifman
//              Justin Siekmann
//              Kevin Curtis
//
// Created on May 1, 2016
//
// Description: Motor Go Vrrooom Vrooooom.
// ******************************************************************************************* //
#include "receivers.h"
#include <xc.h>

#define 

void initReceivers(){ 
    
    TRISEbits.TRISE4 = 1; //set tristate as input for 30kHz 
    CNCONEbits.ON = 1; //enable change notifications for Port E
    CNENEbits.CNIEE4 = 1; //Enables CN for RA7
    IEC1bits.CNAIE = 1; //Enable Interrupts
    IFS1bits.CNAIF = 0; //Set flag to lowered
    IPC8bits.CNIP = 7; //set default priority level
}

