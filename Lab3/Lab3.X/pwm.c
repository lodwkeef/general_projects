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
#define LOW 0
#define HIGH 1
#define TRISD_0      TRISDbits.TRISD0 
#define TRSID_1      TRISDbits.TRISD1 
#define TRISD_2      TRISDbits.TRISD2 
#define TRISD_3      TRISDbits.TRISD3
#define LATD_0       RPD0Rbits.RPD0R
#define LATD_1       RPD0Rbits.RPD1R
#define LATD_2       RPD0Rbits.RPD2R
#define LATD_3       RPD0Rbits.RPD3R
//Need to check that turning on the ODCs for the pins will map properly to the OC
//#define ODC_D0      ODCDbits.ODCD0 
//#define ODC_D1      ODCDbits.ODCD1
//#define ODC_D2      ODCDbits.ODCD2
//#define ODC_D3      ODCDbits.ODCD3

void initPWM(){//We only use two output compare modules.
    TRISD_0 = OUTPUT;
    TRSID_1 = OUTPUT;
    TRISD_2 = OUTPUT;
    TRISD_3 = OUTPUT;
    
    //Justin, please double check that these exist. Page 66 of datasheet. I'm pretty sure that they are not available for 3 of these pins 
//    ODC_D0 = 1;
//    ODC_D1 = 1;
//    ODC_D2 = 1;
//    ODC_D3 = 1;          
            
    LATD_0 = 0b1100; // map OC1 to RD0
    OC1CON = 0x0000; // Turn off OC1 while doing setup.
    OC1R = 0x0000; // Initialize primary Compare Register
    OC1RS = 0x0000; // Initialize secondary Compare Register
    OC1CON = 0x0006; // Configure for PWM mode
    OC1CONSET = 0x8000; // Enable OC1
    
    LATD_1 = 0b1011; // map OC2 to RD1
    OC2CON = 0x0000; // Turn off OC2 while doing setup.
    OC2R = 0x0000; // Initialize primary Compare Register
    OC2RS = 0x0000; // Initialize secondary Compare Register
    OC2CON = 0x0006; // Configure for PWM mode
    OC2CONSET = 0x8000; // Enable OC2
    
    LATD_2 = 0b1011; // map OC3 to RD2
    OC3CON = 0x0000; // Turn off OC3 while doing setup.
    OC3R = 0x0000; // Initialize primary Compare Register
    OC3RS = 0x0000; // Initialize secondary Compare Register
    OC3CON = 0x0006; // Configure for PWM mode
    OC3CONSET = 0x8000; // Enable OC3
    
    LATD_3 = 0b1011; // map OC4 to RD3
    OC4CON = 0x0000; // Turn off OC4 while doing setup.
    OC4R = 0x0000; // Initialize primary Compare Register
    OC4RS = 0x0000; // Initialize secondary Compare Register
    OC4CON = 0x0006; // Configure for PWM mode
    OC4CONSET = 0x8000; // Enable OC4
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

void setMotor1Direction(int directionBit){    //directionBit 1 is forward, 0 is reverse
    if((directionBit==1)&&(RPD0Rbits.RPD0R != 0b1100)){
        OC1CON = 0x0000;            // Turn off OC1 while doing setup
        RPD2Rbits.RPD2R = 0b0000;   // map No connect to RD2
        RPD0Rbits.RPD0R = 0b1100;   // map OC1 to RD0
        LATDbits.LATD2 = LOW;       //set D2 to common
        OC1CONSET = 0x8000;         // Enable OC1
    }
    else if((directionBit==0)&&(RPD2Rbits.RPD2R != 0b1100)){
        OC1CON = 0x0000;            // Turn off OC2 while doing setup.
        RPD0Rbits.RPD0R = 0b0000;   // map No connect to RD0
        RPD2Rbits.RPD2R = 0b1100;   // map OC1 to RD2
        LATDbits.LATD0 = LOW;       //set D0 to common
        OC1CONSET = 0x8000;         // Enable OC1
    }
}

void setMotor2Direction(int directionBit){    //directionBit 1 is forward, 0 is reverse
    if((directionBit==1)&&(RPD1Rbits.RPD1R != 0b1011)){
        OC2CON = 0x0000;            // Turn off OC1 while doing setup.
        RPD3Rbits.RPD3R = 0b0000;   // map No connect to RD3
        RPD1Rbits.RPD1R = 0b1011;   // map OC2 to RD1        
        LATDbits.LATD3 = LOW;       //set D3 to common
        OC2CONSET = 0x8000;         // Enable OC2
    }
    else if((directionBit==0)&&(RPD3Rbits.RPD3R != 0b1011)){
        OC2CON = 0x0000;            // Turn off OC2 while doing setup.
        RPD1Rbits.RPD1R = 0b0000;   // map No connect to RD1
        RPD3Rbits.RPD3R = 0b1011;   // map OC2 to RD3
        LATDbits.LATD1 = LOW;       //set D1 to common        
        OC2CONSET = 0x8000;         // Enable OC2
    }
}

