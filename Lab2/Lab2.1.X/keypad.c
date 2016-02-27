/* 
 * File: keypad.c
// Authors:     Alex Thompson
//              Ben Schifman
//              Justin Siekmann
//              Kevin Curtis
 *
 * Created on February 25, 2016
 */

#include <xc.h>
#include "keypad.h"
#include "timer.h"

#define INPUT 1
#define OUTPUT 0
#define ODC_ON 1
#define LAT_OFF 0
#define LAT_ON 1//Writing a 1 will ground the ODC
#define DIGITAL 0
#define ENABLE 1

#define ANSEL_E2    ANSELEbits.ANSE2
#define ANSEL_E4    ANSELEbits.ANSE4
#define ANSEL_E6    ANSELEbits.ANSE6

#define TRIS_R0     TRISGbits.TRISG14
#define TRIS_R1     TRISCbits.TRISC1
#define TRIS_R2     TRISDbits.TRISD10
#define TRIS_R3     TRISEbits.TRISE6
#define TRIS_C0     TRISEbits.TRISE0
#define TRIS_C1     TRISEbits.TRISE2
#define TRIS_C2     TRISEbits.TRISE4

#define CNPU_R0     CNPUGbits.CNPUG14
#define CNPU_R1     CNPUCbits.CNPUC1
#define CNPU_R2     CNPUDbits.CNPUD10
#define CNPU_R3     CNPUEbits.CNPUE6

#define ODC_C0      ODCEbits.ODCE0
#define ODC_C1      ODCEbits.ODCE2
#define ODC_C2      ODCEbits.ODCE4

#define LAT_C0      LATEbits.LATE0
#define LAT_C1      LATEbits.LATE2
#define LAT_C2      LATEbits.LATE4

#define PORT_R0     PORTGbits.RG14
#define PORT_R1     PORTCbits.RC1
#define PORT_R2     PORTDbits.RD10
#define PORT_R3     PORTEbits.RE6

#define CNEN_R0     CNENGbits.CNIEG14
#define CNEN_R1     CNENCbits.CNIEC1
#define CNEN_R2     CNENDbits.CNIED10
#define CNEN_R3     CNENEbits.CNIEE6
/* Initialize the columns as ODC outputs and the rows as inputs with pull-up
 * resistors. Don't forget about other considerations...
 */
void initKeypad(void){
    ANSEL_E2 = DIGITAL; ANSEL_E4 = DIGITAL; ANSEL_E6 = DIGITAL;
    
    TRIS_R0 = INPUT; TRIS_R1 = INPUT;TRIS_R2 = INPUT; TRIS_R3 = INPUT;
    //CNPU_R0 = ENABLE; CNPU_R1 = ENABLE; CNPU_R2 = ENABLE; CNPU_R3 = ENABLE; //internal pullups
    
    TRIS_C0 = OUTPUT; TRIS_C1 = OUTPUT; TRIS_C2 = OUTPUT;    
    ODC_C0 = ODC_ON; ODC_C1 = ODC_ON; ODC_C2 = ODC_ON;    
    LAT_C0 = LAT_OFF; LAT_C1 = LAT_OFF; LAT_C2 = LAT_OFF;
    
    CNCONGbits.ON = ENABLE; CNCONCbits.ON = ENABLE; CNCONDbits.ON = ENABLE; CNCONEbits.ON = ENABLE;
    CNEN_R0 = ENABLE; CNEN_R1 = ENABLE; CNEN_R2 = ENABLE; CNEN_R3 = ENABLE;
    IEC1 = 0x0005C000; //enable interrupts for G,C,D,E
    IFS1 = 0x00000000; //flags down
    IPC8bits.CNIP = 7;    
}

/* This function will be called AFTER we have determined the row from CN. 
 * This function is to figure out WHICH column has been pressed.
 * This function should return -1 if more than one column is pressed or if
 * no key is pressed at all. Otherwise, it should return the ASCII character of
 * the key that is pressed. The ascii character c programmatically is just 'c'
 */
char scanKeypad(int row){
    //char key = 'c';
    char keypad[4][3] = 
    {'1','2','3',
    '4','5','6',
    '7','8','9',
    '*','0','#'};
    int c0 = 1;
    int c1 = 1;
    int c2 = 1;
    int col = -1;
    
    switch(row){//IF THE SWITCH IS PRESSED THE COLUMN WILL BE ZERO(0)
        case 0:
            LAT_C0 = LAT_ON;
            c0 = PORT_R0;
            LAT_C0 = LAT_OFF;
            LAT_C1 = LAT_ON;
            c1 = PORT_R0;
            LAT_C1 = LAT_OFF;
            LAT_C2 = LAT_ON;
            c2 = PORT_R0;
            LAT_C2 = LAT_OFF;
            break;
        case 1:
            LAT_C0 = LAT_ON;
            c0 = PORT_R1;
            LAT_C0 = LAT_OFF;
            LAT_C1 = LAT_ON;
            c1 = PORT_R1;
            LAT_C1 = LAT_OFF;
            LAT_C2 = LAT_ON;
            c2 = PORT_R1;
            LAT_C2 = LAT_OFF;
            break;
        case 2:
            LAT_C0 = LAT_ON;
            c0 = PORT_R2;
            LAT_C0 = LAT_OFF;
            LAT_C1 = LAT_ON;
            c1 = PORT_R2;
            LAT_C1 = LAT_OFF;
            LAT_C2 = LAT_ON;
            c2 = PORT_R2;
            LAT_C2 = LAT_OFF;
            break;
        case 3:
            LAT_C0 = LAT_ON;
            c0 = PORT_R3;
            LAT_C0 = LAT_OFF;
            LAT_C1 = LAT_ON;
            c1 = PORT_R3;
            LAT_C1 = LAT_OFF;
            LAT_C2 = LAT_ON;
            c2 = PORT_R3;
            LAT_C2 = LAT_OFF;
            break;
        default:
            break;
    }
    
    if((c0+c1+c2)==2){ //check if output values are good
        if(c0 == 0) col = 0;
        else if(c1 == 0) col = 1;
        else if(c2 == 0) col = 2;
        key = keypad[row][col];//WHAT IF WE GET A -1 IN THE COLUMN??????????
    }
        
    return key;
}
