/* 
 * File:   pwm.c
// Authors:     Alex Thompson
//              Ben Schifman
//              Justin Siekmann
//              Kevin Curtis
 *
 * Created on March 7, 2016
 */


#include <xc.h>
#include "pwm.h"

#define OUTPUT 0
#define INPUT 1
#define LOW 0
#define HIGH 1
#define OFF 0
#define TRISD_0      TRISDbits.TRISD0 
#define TRSID_1      TRISDbits.TRISD1 
#define TRISD_2      TRISDbits.TRISD2 
#define TRISD_3      TRISDbits.TRISD3
#define LATD_0       LATDbits.LATD0
#define LATD_1       LATDbits.LATD1
#define LATD_2       LATDbits.LATD2
#define LATD_3       LATDbits.LATD3
#define Pin_0        RPD0Rbits.RPD0R
#define Pin_1        RPD1Rbits.RPD1R
#define Pin_2        RPD2Rbits.RPD2R
#define Pin_3        RPD3Rbits.RPD3R


void initPWM(){//We only use two output compare modules.
    TRISD_0 = OUTPUT;
    TRSID_1 = OUTPUT;
    TRISD_2 = OUTPUT;
    TRISD_3 = OUTPUT; //At this line the second motor turn on, we don't want that -BAS  
    LATD_0 = LOW;
    LATD_1 = LOW;
    LATD_2 = LOW;
    LATD_3 = LOW; //this motor was running, set it off -BAS
    
            
    Pin_0 = 0b1100; // map OC1 to RD0    
    OC1CON = 0x0000; // Turn off OC1 while doing setup.  Also clears all states
    OC1R = 0x0000; // Initialize primary Compare Register
    OC1RS = 0x0000; // Initialize secondary Compare Register
    OC1CONbits.OCM = 0b110; //Configure for PWM mode
    OC1CONbits.OCTSEL = 0;  //choose the timer, '0' is timer2, '1' is timer3
    OC1CONbits.ON = 1;      //enable the output compare module
    //OC1CONSET = 0x8000; // Enable OC1 //dont think this works -KDC
    
    Pin_1 = 0b1011; // map OC2 to RD1
    OC2CON = 0x0000; // Turn off OC2 while doing setup.  Also clears all states
    OC2R = 0x0000; // Initialize primary Compare Register
    OC2RS = 0x0000; // Initialize secondary Compare Register
    OC2CONbits.OCM = 0b110; //Configure for PWM mode
    OC2CONbits.OCTSEL = 0;  //choose the timer, '0' is timer2, '1' is timer3
    OC2CONbits.ON = 1;
    //OC2CONSET = 0x8000; // Enable OC2 //dont think this works -KDC
   
    Pin_2 = 0b1011; // map OC3 to RD2
    OC3CON = 0x0000; // Turn off OC3 while doing setup.  Also clears all states
    OC3R = 0x0000; // Initialize primary Compare Register
    OC3RS = 0x0000; // Initialize secondary Compare Register
    OC3CONbits.OCM = 0b110; //Configure for PWM mode
    OC3CONbits.OCTSEL = 0;  //choose the timer, '0' is timer2, '1' is timer3
    OC3CONbits.ON = 1;
    
    Pin_3 = 0b1011; // map OC4 to RD3
    OC4CON = 0x0000; // Turn off OC4 while doing setup.  Also clears all states
    OC4R = 0x0000; // Initialize primary Compare Register
    OC4RS = 0x0000; // Initialize secondary Compare Register
    OC4CONbits.OCM = 0b110; //Configure for PWM mode
    OC4CONbits.OCTSEL = 0;  //choose the timer, '0' is timer2, '1' is timer3
    OC4CONbits.ON = 1;
}


void setPWM1(int duty){
    OC1RS = duty;
}

void setPWM2(int duty){
    OC2RS = duty;
}


void setPWM3(int duty){
    OC3RS = duty;
}

void setPWM4(int duty){
    OC4RS = duty;
}

