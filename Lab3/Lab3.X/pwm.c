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
#define OUTPUT 0
#define INPUT 1
#define TRISD_0      TRISDbits.TRISD0 
#define TRSID_1      TRISDbits.TRISD1 
#define TRISD_2      TRISDbits.TRISD2 
#define TRISD_3      TRISDbits.TRISD3 

void initPWM(){//We only use two output compare modules.
    TRISD_0 = OUTPUT;
    TRSID_1 = OUTPUT;
    TRISD_2 = OUTPUT;
    TRISD_3 = OUTPUT;
            
            
    RPD0Rbits.RPD0R = 0b1100; // map OC1 to RD0
    OC1CON = 0x0000; // Turn off OC1 while doing setup.
    OC1R = 0x0000; // Initialize primary Compare Register
    OC1RS = 0x0000; // Initialize secondary Compare Register
    OC1CON = 0x0006; // Configure for PWM mode
    OC1CONSET = 0x8000; // Enable OC1
    
    RPD1Rbits.RPD1R = 0b1011; // map OC2 to RD1
    OC2CON = 0x0000; // Turn off OC2 while doing setup.
    OC2R = 0x0000; // Initialize primary Compare Register
    OC2RS = 0x0000; // Initialize secondary Compare Register
    OC2CON = 0x0006; // Configure for PWM mode
    OC2CONSET = 0x8000; // Enable OC2
    
//    RPD2Rbits.RPD2R = 0b1011; // map OC3 to RD2
//    OC3CON = 0x0000; // Turn off OC3 while doing setup.
//    OC3R = 0x0000; // Initialize primary Compare Register
//    OC3RS = 0x0000; // Initialize secondary Compare Register
//    OC3CON = 0x0006; // Configure for PWM mode
//    OC3CONSET = 0x8000; // Enable OC3
//    
//    RPD3Rbits.RPD3R = 0b1011; // map OC4 to RD3
//    OC4CON = 0x0000; // Turn off OC4 while doing setup.
//    OC4R = 0x0000; // Initialize primary Compare Register
//    OC4RS = 0x0000; // Initialize secondary Compare Register
//    OC4CON = 0x0006; // Configure for PWM mode
//    OC4CONSET = 0x8000; // Enable OC4
}

void setPWM1(int duty){
    OC1RS = duty;
}

void setPWM2(int duty){
    OC2RS = duty;
}

//void setPWM3(int duty){
//    OC3RS = duty;
//}
//
//void setPWM4(int duty){
//    OC4RS = duty;
//}
