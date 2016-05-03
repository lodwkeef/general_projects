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

void initSW1(){ //external switch RB12 to +5V microcontroller
   
    TRISBbits.TRISB12 = 1; //set as input
    ANSELBbits.ANSB12 = 0; //set to digital
    
    CNCONBbits.ON = 1; //enable change notifications for Port B
    CNENBbits.CNIEB12 = 1; //Enables CN for RB12
    IEC1bits.CNBIE = 1; //Enable Interrupts
    IFS1bits.CNBIF = 0; //Set flag to lowered
    IPC8bits.CNIP = 7; //set default priority level 
    
}

