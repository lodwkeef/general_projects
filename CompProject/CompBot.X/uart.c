// ******************************************************************************************* //
//
// File:        uart.c
// Date:        4/20/2016
// Authors:     Alex Thompson
//              Ben Schifman
//              Justin Siekmann
//              Kevin Curtis
//
//
//
// Description: Motor Go Vrrooom Vrooooom.
// ******************************************************************************************* //
#include <xc.h>
#include "uart.h"

void initUART(){
    //WARNING COPY PASTED CODE FROM VANHOY'S GITHUB
    //will go over later
    //changed pins
    RPD11Rbits.RPD11R = 0b0011;    //Maps U1TX to pin 15  ,J10
    U1RXRbits.U1RXR =   0b0111;    //Mapping U1RX to pin 17, J10
    
    U1BRG = 64;             //Set the baud rate (MAY NEED TO BE UPDATED)
  
    U1MODEbits.UEN = 0; //based on Vanhoy's
    U1MODEbits.BRGH = 0; //standard speed mode 16x baud clock enabled
    U1MODEbits.PDSEL = 0;   //Parity select (depends on device) (8-N-1)
    U1MODEbits.STSEL = 0;   //One stop bit
    
    U1STAbits.URXEN = 1;    //Enable the receive pin (TRIS not needed)
    U1STAbits.UTXEN = 1;    //Enable the transmit pin (TRIS not needed)
    U1STAbits.URXISEL = 0; //Enable interrupts for RX data buffer
    U1MODEbits.ON = 1;      //Enable the UART
}

void sendChar(char c){
    U1TXREG = c;
    delayUs(1000);
}

void sendCommand(const char* sendString){
    const char* tempChar = NULL;
    for (tempChar = sendString; *tempChar; tempChar++) {
        sendChar(*tempChar);
    }
}
