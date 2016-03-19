/* 
 * File:   adc.h
// Authors:     Alex Thompson
//              Ben Schifman
//              Justin Siekmann
//              Kevin Curtis
 *
 * Created on March 7, 2016
 */

#ifndef ADC_H
#define	ADC_H

void initADC();
float getADCbuffer();
void printVoltage(float ADCbuffer);
float UpdateLCDVoltage(float dispVolt);

#endif	/* ADC_H */

