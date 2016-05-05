// ******************************************************************************************* //
//
// File:        switch.c
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
#include "switch.h"
#include <xc.h>

#include "switch.h"
#include <xc.h>

void initSW1(){ //external switch RG14 to +5V microcontroller
   
    TRISGbits.TRISG14 = 1; //set as input
    
    CNCONGbits.ON = 1; //enable change notifications for Port G
    CNENGbits.CNIEG14 = 1; //Enables CN for RG14
    IEC1bits.CNGIE = 1; //Enable Interrupts
    IFS1bits.CNGIF = 0; //Set flag to lowered
    IPC8bits.CNIP = 7; //set default priority level 
    
}

