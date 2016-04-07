// ******************************************************************************************* //
//
// File:        adc.c
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

#include <xc.h>
#include "adc.h"

void initADC(){ //updated for Final Project
    TRISBbits.TRISB0 = 1; //set pin to input 
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 1;
    TRISBbits.TRISB3 = 1;
    
    ANSELBbits.ANSB0 = 1; // RB0 set to analog
    ANSELBbits.ANSB1 = 1;
    ANSELBbits.ANSB2 = 1;
    ANSELBbits.ANSB3 = 1;
    
    AD1CON1bits.FORM = 0; // 16 unsigned integer
    AD1CON1bits.SSRC = 7; // Auto-convert mode
    AD1CON1bits.ASAM = 1; // Auto-sampling
    AD1CON2bits.VCFG = 0; // Use board refernece voltages
    AD1CON2bits.CSCNA = 1; //UPDATED: scan different inputs (was 0)
    AD1CON2bits.SMPI = 15; // 16 burritos
    AD1CON2bits.ALTS = 0; // Only Mux A
    AD1CON3bits.ADRC = 0; // Use PBCLK
    AD1CON3bits.SAMC = 2; // 2 Tad per sample
    AD1CON3bits.ADCS = 0xFF; // 512 times the PBCLK
    AD1CHSbits.CH0NA = 0; // Use Vref- as negative reference
    AD1CHSbits.CH0NA = 0; //UPDATED: Scan is enabled, these are ignored
    AD1CSSLbits.CSSL0 = 0x0000000F; //UPDATED: Scanning AN0-AN3
    IFS0bits.AD1IF = 0;
    IEC0bits.AD1IE = 1;
    IPC5bits.AD1IP = 7;
    AD1CON1bits.ADON = 1; // turn on the ADC
}

void getADCbuffer(float *ADCarrayAveraged){ //Updated to average ADC values then store pins
    float ADCbufferArray[16];
    AD1CON1bits.ADON = 1;
    IFS0bits.AD1IF = 0;
    while(AD1CON1bits.DONE == 0);
    AD1CON1bits.ADON = 0;
    AD1CON1bits.SAMP = 0;    
    
    ADCbufferArray[0] = (float) ADC1BUF0; //AN0
    ADCbufferArray[1] = (float) ADC1BUF1; //AN1
    ADCbufferArray[2] = (float) ADC1BUF2; //AN2
    ADCbufferArray[3] = (float) ADC1BUF3; //AN3
    ADCbufferArray[4] = (float) ADC1BUF4; //AN0
    ADCbufferArray[5] = (float) ADC1BUF5; //AN1
    ADCbufferArray[6] = (float) ADC1BUF6; //AN2
    ADCbufferArray[7] = (float) ADC1BUF7; //AN3
    ADCbufferArray[8] = (float) ADC1BUF8; //AN0
    ADCbufferArray[9] = (float) ADC1BUF9; //AN1
    ADCbufferArray[10] = (float) ADC1BUFA; //AN2
    ADCbufferArray[11] = (float) ADC1BUFB; //AN3
    ADCbufferArray[12] = (float) ADC1BUFC; //AN0
    ADCbufferArray[13] = (float) ADC1BUFD; //AN1
    ADCbufferArray[14] = (float) ADC1BUFE; //AN2
    ADCbufferArray[15] = (float) ADC1BUFF; //AN3
    
    ADCarrayAveraged[0] = (ADCbufferArray[0] + ADCbufferArray[4] + ADCbufferArray[8] + ADCbufferArray[12])/4; //Averaged AN0
    ADCarrayAveraged[1] = (ADCbufferArray[1] + ADCbufferArray[5] + ADCbufferArray[9] + ADCbufferArray[13])/4; //Averaged AN1
    ADCarrayAveraged[2] = (ADCbufferArray[2] + ADCbufferArray[6] + ADCbufferArray[10] + ADCbufferArray[14])/4; //Averaged AN2
    ADCarrayAveraged[3] = (ADCbufferArray[3] + ADCbufferArray[7] + ADCbufferArray[11] + ADCbufferArray[15])/4; //Averaged AN3
}

void printVoltage(float ADCbuffer){
    //clearLCD();
    moveCursorLCD(0,0);
    float dispVolt;
    char s[9] = {};
    dispVolt = (3.3*ADCbuffer)/1023;
    sprintf(s,"Voltage: %.2fV", dispVolt);
    printStringLCD(s);
    moveCursorLCD(0,8);
}

float UpdateLCDVoltage(float dispVoltInput, float ADCbufferInput){
    float tempDispVolt = 0;
    delayUs(1000);
    if((dispVoltInput < ADCbufferInput) && ((dispVoltInput + 1) <= ADCbufferInput)){//to reduce excessive LCD updates
        printVoltage(ADCbufferInput);
        tempDispVolt = ADCbufferInput;
    }
    else if((dispVoltInput > ADCbufferInput) && ((dispVoltInput - 1) >= ADCbufferInput)){
        printVoltage(ADCbufferInput);
        tempDispVolt = ADCbufferInput;
    }
    return tempDispVolt;
}