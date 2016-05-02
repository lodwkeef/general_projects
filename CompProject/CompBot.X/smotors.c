// ******************************************************************************************* //
//
// File:        smotors.c
// Date:        5/1/2016
// Authors:     Alex Thompson
//              Ben Schifman
//              Justin Siekmann
//              Kevin Curtis
//
// Created on May 1, 2016
//
// Description: Motor Go Click Clack.
// ******************************************************************************************* //

#include <xc.h>
#include "smotors.h"

#define TRUE 1
#define FALSE 0


void querryPos(int positionArray[]){
    sendCommand("ZPR P");
    delayUs(2000);
    int complete = FALSE;
    int receivedChar;
    int i=0;
    
    while((complete == FALSE)){
        while(U1STAbits.URXDA ==1 ){
            // U1RXREG stores the last character received by the UART. Read this
            // value into a local variable before processing.
            receivedChar = U1RXREG;
            if((receivedChar==0x0A)||(receivedChar==0x0D)){
                complete = TRUE;
            }
            positionArray[i] = receivedChar;
            i++;
        }
    }
}