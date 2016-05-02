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


void initReceivers(){ 
    
    TRISEbits.TRISE0 = 1; //57.6kHz
    TRISEbits.TRISE2 = 1; //40kHz
    TRISEbits.TRISE4 = 1; //30kHz
    
    ANSELEbits.ANSE2 = 0; //set to digital
    ANSELEbits.ANSE4 = 0;

    CNCONEbits.ON = 1; //enable change notifications for Port E
    CNENEbits.CNIEE0 = 1; //Enables CN for RE0
    CNENEbits.CNIEE2 = 1; //Enables CN for RE2
    CNENEbits.CNIEE4 = 1; //Enables CN for RE4
    IEC1bits.CNAIE = 1; //Enable Interrupts
    IFS1bits.CNAIF = 0; //Set flag to lowered
    IPC8bits.CNIP = 7; //set default priority level
}

