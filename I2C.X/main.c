/* 
 * File:   main.c
 * Author: jsiek
 *
 * Created on May 1, 2016, 10:42 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include "HMC5883L.h" 
#include "config.h"
#include "i2c.h"
#include "lcd.h"
#include "timer.h"
#include "interrupt.h"    
                                   
void main() 
{   
    SYSTEMConfigPerformance(10000000);
    enableInterrupts();
    initTimer1();
    initTimer45();
    initLCD(); 
    initI2C2();
    HMC5883L_init();
    
    float h = 0.0;
    char s[8];
    
    while(1)                                    
    {                            
      h = HMC5883L_heading(); 
      sprintf(s,"%f", h);
      clearLCD();
      printStringLCD(s);
      testLCD();
    }                          
}