/*
 * File:   switch.c
// Authors:     Alex Thompson
//              Ben Schifman
//              Justin Siekmann
//              Kevin Curtis
 * Created on September 19, 2015, 10:46 AM
 */
#include "switch.h"
#include <xc.h>

void initSWext(){ //external switch RA7
    //TODO: Initialize the appropriate pins to work with the external switch using a
    //change notification interrupt.    
    TRISAbits.TRISA7 = INPUT;
    CNPUAbits.CNPUA7 = 1; //enable pull up on switch
    
    CNCONAbits.ON = 1; //enable change notifications for Port A
    CNENAbits.CNIEA7 = 1; //Enables CN for RA7
    IEC1bits.CNAIE = 1; //Enable Interrupts
    IFS1bits.CNAIF = 0; //Set flag to lowered
    IPC8bits.CNIP = 7; //set default priority level
}

void initSW1(){
    TRISDbits.TRISD6 = INPUT;
    CNPUDbits.CNPUD6 = 1; //enable pull up on switch
    
    CNCONDbits.ON = 1; //enable change notifications for Port A
    CNENDbits.CNIED6 = 1; //Enables CN for RA7
    IEC1bits.CNDIE = 1; //Enable Interrupts
    IFS1bits.CNDIF = 0; //Set flag to lowered
    IPC8bits.CNIP = 7; //set default priority level
}