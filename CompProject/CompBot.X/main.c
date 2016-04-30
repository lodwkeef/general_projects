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

#define OFF 0
#define ON 1
#define PRESS 0
#define RELEASE 1


typedef enum stateTypeEnum {
    idle, go
} stateType;

volatile stateType state = idle;
volatile int pressRelease = PRESS;

int main() {
    SYSTEMConfigPerformance(10000000);
    enableInterrupts();
    initTimer1();
    initTimer45();
    initSW1();
    initLCD();
    initUART();
    
    //unsigned char receivedChar = '$';
    
    //while(1){
        //sendCommand("ZSL 100000"); //0b01100001 0b01111010
        U1TXREG = 0x0A;
        sendCommand("ZMA 15000");
        U1TXREG = 0x0A;
        delayUs(1000000);
        sendCommand("ZMA 30000");
        U1TXREG = 0x0A;
        delayUs(1000000);
        sendCommand("ZMA 45000");
        U1TXREG = 0x0A;
        delayUs(1000000);
        sendCommand("ZMA 60000");
        U1TXREG = 0x0A;
        delayUs(1000000);
        sendCommand("ZMA 45000");
        U1TXREG = 0x0A;
        delayUs(1000000);
        sendCommand("ZMA 30000");
        U1TXREG = 0x0A;
        delayUs(1000000);
        sendCommand("ZMA 15000");
        U1TXREG = 0x0A;
        delayUs(1000000);
        sendCommand("ZMA 0");
        U1TXREG = 0x0A;
        delayUs(1000000);
        //testTimerDelay();
        
    //}
    return 1;
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt(){
    int dummy;
    dummy = PORTA; // Read the Port A
    T1CONbits.TON = ON;
    IFS1bits.CNAIF = 0; //lower flag
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