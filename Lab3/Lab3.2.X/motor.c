/* 
 * File:   motor.c
// Authors:     Alex Thompson
//              Ben Schifman
//              Justin Siekmann
//              Kevin Curtis
 *
 * Created on March 10, 2016
 */
#include <xc.h>
#include "motor.h"
#include "timer.h"
#define forward     1
#define reverse     0
#define M1          1
#define M2          2
#define LATD_0      LATDbits.LATD0
#define LATD_1      LATDbits.LATD1
#define LATD_2      LATDbits.LATD2
#define LATD_3      LATDbits.LATD3
#define Pin_0       RPD0Rbits.RPD0R
#define Pin_1       RPD1Rbits.RPD1R
#define Pin_2       RPD2Rbits.RPD2R
#define Pin_3       RPD3Rbits.RPD3R
#define OFF         0
#define LOW         0



void setMotorDirection(int motor, int direction){
    
    if(motor == M1){
        switch (direction) {
            case forward:
                //OC1CONbits.ON = OFF;     //Turn off OC1 while doing setup.
                Pin_2 = 0b0000;            // map 'No connect' to RD2
                Pin_0 = 0b1100;            // map OC1 to RD0
                LATD_2 = LOW;              //set D2 to common
                OC1CONbits.ON = 1;         //Turn on OC1 while doing setup.
                delayUs(1000);
                break;

            case reverse:
                //OC1CONbits.ON = OFF;     //Turn off OC1 while doing setup.
                //OC3CONbits.ON = OFF;     //Turn off OC3 while doing setup
                Pin_0 = 0b0000;            // map 'No connect' to RD0
                Pin_2 = 0b1011;            // map OC3 to RD2
                LATD_0 = LOW;              //set D0 to common
                OC3CONbits.ON = 1;         //Turn on OC1 while doing setup.
                delayUs(1000);
                break;
        }
    }
    else if(motor == M2){
        switch (direction) {
            case forward:
                //OC1CONbits.ON = OFF;     //Turn off OC1 while doing setup.
                Pin_1 = 0b0000;            // map 'No connect' to RD1
                Pin_3 = 0b1011;            // map OC4 to RD3
                LATD_1 = LOW;              //set D1 to common
                OC4CONbits.ON = 1;         //Turn on OC4 while doing setup.
                delayUs(1000);
                break;

            case reverse:
                //OC1CONbits.ON = OFF;     //Turn off OC1 while doing setup.
                //OC3CONbits.ON = OFF;     //Turn off OC3 while doing setup
                Pin_3 = 0b0000;            // map 'No connect' to RD3
                Pin_1 = 0b1011;            // map OC2 to RD1
                LATD_3 = LOW;              //set D3 to common
                OC2CONbits.ON = 1;         //Turn on OC2 while doing setup.
                delayUs(1000);
                break;
        }
    }
}

void setMotorSpeed(float ADCbuffer, int direction){
    switch (direction){
            case forward:
                if(ADCbuffer < 510){
                    setPWM1(1023 - ADCbuffer);
                    setPWM4(ADCbuffer);
                }
                
                
                
                
                
                
                
                
                
                
                
                
                break;
            case reverse:
                
                
                
                
                
                
                
                
                
                
            break;
    }
}


void unmapPins(){
    setPWM1(0);
    setPWM2(0);
    setPWM3(0);
    setPWM4(0);
    Pin_0 = 0;
    Pin_1 = 0;
    Pin_2 = 0;
    Pin_3 = 0;
}

void testM1forward(){
    int i = 0;
    setMotorDirection(M1, forward);
    for (i=200; i<1023; i++){setPWM1(i); delayUs(10000);}//rev up        
    for (i=1023; i>300; i--){setPWM1(i); delayUs(10000);}//rev down
}

void testM1reverse(){
    int i = 0;
    setMotorDirection(M1, reverse);
    for (i=200; i<1023; i++){setPWM3(i); delayUs(10000);}//rev up    
    for (i=1023; i>300; i--){setPWM3(i); delayUs(10000);}//rev down  
}

void testM2forward(){
    int i = 0;
    setMotorDirection(M2, forward);
    for (i=200; i<1023; i++){setPWM4(i); delayUs(10000);}//rev down  
    for (i=1023; i>300; i--){setPWM4(i); delayUs(10000);}//rev down      
}

void testM2reverse(){
    int i = 0;
    setMotorDirection(M2, reverse);
    for (i=200; i<1023; i++){setPWM2(i); delayUs(10000);}//rev down  
    for (i=1023; i>300; i--){setPWM2(i); delayUs(10000);}//rev down      
}