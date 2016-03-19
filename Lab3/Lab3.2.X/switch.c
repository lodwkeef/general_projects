/* 
 * File:   switch.c
// Authors:     Alex Thompson
//              Ben Schifman
//              Justin Siekmann
//              Kevin Curtis
 *
 * Created on March 18, 2016
 */
#include "switch.h"
#include <xc.h>

#include "switch.h"
#include <xc.h>

void initSW1(){ //external switch RA7
    //TODO: Initialize the appropriate pins to work with the external switch using a
    //change notification interrupt.    
    TRISAbits.TRISA7 = 1;
    CNPUAbits.CNPUA7 = 1; //enable pull up on switch
    
    CNCONAbits.ON = 1; //enable change notifications for Port A
    CNENAbits.CNIEA7 = 1; //Enables CN for RA7
    IEC1bits.CNAIE = 1; //Enable Interrupts
    IFS1bits.CNAIF = 0; //Set flag to lowered
    IPC8bits.CNIP = 7; //set default priority level
}
