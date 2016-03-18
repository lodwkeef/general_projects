/* 
 * File:   adc.c
// Authors:     Alex Thompson
//              Ben Schifman
//              Justin Siekmann
//              Kevin Curtis
 *
 * Created on March 7, 2016
 */

#include <xc.h>
#include "adc.h"

void initADC(){
    //the tristate for the analog pin. What is up with that doc
    TRISBbits.TRISB0 = 1; //set pin to input 
    ANSELBbits.ANSB0 = 1; // RB0 set to analog
    AD1CON1bits.FORM = 0; // 16 unsigned integer
    AD1CON1bits.SSRC = 7; // Auto-convert mode
    AD1CON1bits.ASAM = 1; // Auto-sampling
    AD1CON2bits.VCFG = 0; // Use board refernece voltages
    AD1CON2bits.CSCNA = 0; // Disable scanning
    AD1CON2bits.SMPI = 15; // 15 burritos
    AD1CON2bits.ALTS = 0; // Only Mux A
    AD1CON3bits.ADRC = 0; // Use PBCLK
    AD1CON3bits.SAMC = 2; // 2 Tad per sample
    AD1CON3bits.ADCS = 0xFF; // 512 times the PBCLK
    AD1CHSbits.CH0NA = 0; // Use Vref- as negative reference
    AD1CHSbits.CH0SA = 0; // Scan AN0 at least
    AD1CSSLbits.CSSL0 = 1; //ADC pin select bits, may not change anything -BAS
    IFS0bits.AD1IF = 0;
    IEC0bits.AD1IE = 1;
    IPC5bits.AD1IP = 7;
    AD1CON1bits.ADON = 1; // turn on the ADC
}

float getADCbuffer(){
    float ADCbuffer = 0;
    AD1CON1bits.ADON = 1;
    IFS0bits.AD1IF = 0;
    while(AD1CON1bits.DONE == 0); 
    AD1CON1bits.SAMP = 0;
    AD1CON1bits.ADON = 0;
    ADCbuffer = (float) ADC1BUF0;
    return ADCbuffer;
}

void printVoltage(float ADCbuffer){
    //clearLCD();
    moveCursorLCD(0,0);
    float dispVolt;
    char s[9] = {};
    dispVolt = (3.3*ADCbuffer)/1023;
    sprintf(s,"Voltage: %.2fV", dispVolt);
    printStringLCD(s);
    moveCursorLCD(0,0);
}