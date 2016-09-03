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


void querryPos(char positionArray[]){
    sendCommand("ZPR P");
    delayUs(2000);
    int complete = FALSE;
    char receivedChar;
    strcpy(positionArray, "");
    
    while((complete == FALSE)){
        while(U1STAbits.URXDA ==1 ){
            // U1RXREG stores the last character received by the UART. Read this
            // value into a local variable before processing.
            receivedChar = U1RXREG;
            if(((receivedChar==0x0A)||(receivedChar==0x0D))||(U1STAbits.URXDA ==0)){
                complete = TRUE;
            }
            else if((receivedChar!='\\')&&(receivedChar!='r')){
                strcat(positionArray, &receivedChar);
            }
        }
    }
}

void right90(){
    sendCommand("RMR -65500");
    sendCommand("LSL 0");
    delayUs(2000000);
}

void left90(){
    sendCommand("LMR 65500");
    sendCommand("RSL 0");
    delayUs(2000000);
}

void right45(){
    sendCommand("RMR -31200");
    sendCommand("LSL 0");
    delayUs(2000000);
}

void left45(){
    sendCommand("LMR 31900");
    sendCommand("RSL 0");
    delayUs(2000000);
}

void forward1ft(){
    sendCommand("RMR -76800");
    sendCommand("LMR 76800");
    delayUs(2000000);
}

void forward2ft(){
    sendCommand("RMR -153600");
    sendCommand("LMR 153600");
    delayUs(4000000);
}

void forward3ft(){
    sendCommand("RMR -230400");
    sendCommand("LMR 230400");
    delayUs(6000000);
}

void forward4ft(){
    sendCommand("RMR -307200");
    sendCommand("LMR 307200");
    delayUs(8000000);
}

void forward707(){
    sendCommand("RMR -54300");
    sendCommand("LMR 54300");
    delayUs(2000000);
}

void leftcircle(){
    sendCommand("RVM 75000");
    delayUs(1000000);
    sendCommand("RMR -150000");
    sendCommand("LMR 300000");
    delayUs(8500000);
    sendCommand("RVM 150000");
    delayUs(100000);
    
}


void rightcircle(){
    sendCommand("LVM 75000");
    delayUs(1000000);
    sendCommand("RMR -300000");
    sendCommand("LMR 150000");
    delayUs(8500000);
    sendCommand("LVM 155000");
    delayUs(100000);
    
}