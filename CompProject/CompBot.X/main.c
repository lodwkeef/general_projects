/* 
 * File:   main.c
 * Author: jsiek
 *
 * Created on April 19, 2016, 11:53 PM
 */

#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "interrupt.h"
#include "lcd.h"
#include "timer.h"
#include "switch.h"
#include "uart.h"
#include "receivers.h"

#define OFF 0
#define ON 1
#define PRESS 0
#define RELEASE 1
#define CN_E 0x00010000
#define RX57  PORTEbits.RE0
#define RX40  PORTEbits.RE2
#define RX30  PORTEbits.RE4



typedef enum stateTypeEnum {
    idle, go
} stateType;

volatile stateType state = idle;
volatile int pressRelease = PRESS;
volatile int receiver = 0;

int main() {
    SYSTEMConfigPerformance(10000000);
    enableInterrupts();
    initTimer1();
    initTimer2();
    initTimer45();
    initSW1();
    initLCD();
    initUART();
    initReceivers();
    
    //unsigned char receivedChar = '$';
    
    while(1){
        //sendCommand("ZSL 100000"); //0b01100001 0b01111010
        U1TXREG = 0x0A;
        sendCommand("ZSL 50000");
        //U1TXREG = 0x0A;
        switch(receiver){
            case 57:
                moveCursorLCD(1,9);
                printStringLCD("57kHZ");
                break;
            case 40:
                moveCursorLCD(1,0);
                printStringLCD("40kHZ");
                break;
            case 30:
                moveCursorLCD(0,9);
                printStringLCD("30kHZ");
                break;
            default:
                clearLCD();
                printStringLCD("NONE");
                break;                
        }
        
    }
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt(){
    int dummy;
    dummy = PORTA; // Read the Port A
    dummy = PORTE; //Read the Port E
    if (IFS1bits.CNEIF == 1){ T2CONbits.TON = ON;} //if the receiver modules are triggered then use the 100Us delay
    else {T1CONbits.TON = ON;}
    IFS1bits.CNAIF = 0; IFS1bits.CNEIF = 0; //lower flags
}

void __ISR(_TIMER_1_VECTOR, IPL7SRS) _T1Interrupt() { //Timer 1 is for debouncing
    IFS0bits.T1IF = 0; //lower the flag
    T1CONbits.TON = OFF;
    if(pressRelease == RELEASE){
        if(state != idle) state = idle; 
        else if(state == idle) state = go;
        pressRelease = PRESS;
    }
    else{
        pressRelease = RELEASE;
    }
}

void __ISR(_TIMER_2_VECTOR, IPL7SRS) _T2Interrupt() { //Timer 2 is for debouncing
    IFS0bits.T2IF = 0; //lower the flag
    T2CONbits.TON = OFF;
    if(RX57 == 1)
    {
        receiver = 57;
    }
    else if(RX40 == 1)
    {
        receiver = 40;
    }
    else if(RX30 == 1)
    {
        receiver = 30;
    }
}