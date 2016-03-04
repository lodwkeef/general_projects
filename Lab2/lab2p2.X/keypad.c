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
#include "lcd.h"

#define INPUT 1
#define OUTPUT 0
#define ODC_ON 1
#define LAT_OFF 1
#define LAT_ON 0
#define DIGITAL 0
#define ENABLE 1
#define ALLOFF -1

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

void setODCLatch(int col){
    switch(col){
        case 0:
            LAT_C0 = LAT_ON;
            LAT_C1 = LAT_OFF;
            LAT_C2 = LAT_OFF;
            break;
        case 1:
            LAT_C0 = LAT_OFF;
            LAT_C1 = LAT_ON;
            LAT_C2 = LAT_OFF;
            break;
        case 2:
            LAT_C0 = LAT_OFF;
            LAT_C1 = LAT_OFF;
            LAT_C2 = LAT_ON;
            break;
        case -1: //reset all
            LAT_C2 = LAT_OFF;
            LAT_C0 = LAT_OFF;
            LAT_C1 = LAT_OFF;
            break;
    }
}

/* This function will be called AFTER we have determined the row from CN. 
 * This function is to figure out WHICH column has been pressed.
 * This function should return -1 if more than one column is pressed or if
 * no key is pressed at all. Otherwise, it should return the ASCII character of
 * the key that is pressed. The ascii character c programmatically is just 'c'
 */
char scanKeypad(int row){
    char key = -1;
    char keypad[4][3] = 
    {'1','2','3',
    '4','5','6',
    '7','8','9',
    '*','0','#'};
    int c0 = -1;
    int c1 = -1;
    int c2 = -1;
    int col = -1;
    
    switch(row){
        case 0:
            setODCLatch(0);
            delayUs(10);
            c0 = PORT_R0;
            setODCLatch(1);
            delayUs(10);
            c1 = PORT_R0;
            setODCLatch(2);
            delayUs(10);
            c2 = PORT_R0;
            setODCLatch(ALLOFF);
            break;
        case 1:
            setODCLatch(0);
            delayUs(10);
            c0 = PORT_R1;
            setODCLatch(1);
            delayUs(10);
            c1 = PORT_R1;
            setODCLatch(2);
            delayUs(10);
            c2 = PORT_R1;
            setODCLatch(ALLOFF);
            break;
        case 2:
            setODCLatch(0);
            delayUs(10);
            c0 = PORT_R2;
            setODCLatch(1);
            delayUs(10);
            c1 = PORT_R2;
            setODCLatch(2);
            delayUs(10);
            c2 = PORT_R2;
            setODCLatch(ALLOFF);
            break;
        case 3:
            setODCLatch(0);
            delayUs(10);
            c0 = PORT_R3;
            setODCLatch(1);
            delayUs(10);
            c1 = PORT_R3;
            setODCLatch(2);
            delayUs(10);
            c2 = PORT_R3;
            setODCLatch(ALLOFF);
            break;
        default:
            break;
    }
    
    if((c0+c1+c2)==2){ //check if output values are good (one grounded, 2 high)
        if(c0 == 0) col = 0;
        else if(c1 == 0) col = 1;
        else if(c2 == 0) col = 2;
        key = keypad[row][col];
    }
    return key;
}
    
void clearString(char string[]){
        string[0] = '\0'; string[1] = '\0'; string[2] = '\0'; string[3] = '\0'; 
}

int checkPass(char guessString[], char passStringa[], char passStringb[], char passStringc[], char passStringd[]){
    int a = 0;
    int b = 0;
    int c = 0;
    int d = 0;
    a = strcmp(guessString, passStringa);
    b = strcmp(guessString, passStringb);
    c = strcmp(guessString, passStringc);
    d = strcmp(guessString, passStringd);
    if ((a == 0) || (b == 0) || (c == 0) || (d == 0)) return 0;
    return -1;
}

void updatePass(char tempPass[], char passStringa[], char passStringb[], char passStringc[], char passStringd[], int i){
    switch(i){
        case 0:
            strcpy(passStringa, tempPass);
            break;
        case 1:
            strcpy(passStringb, tempPass);
            break;
        case 2:
            strcpy(passStringc, tempPass);
            break;
        case 3:
            strcpy(passStringd, tempPass);
            break;
    }
}

/******QA SOFTWARE TESTS*******************************************************************************************/
void test_clearString(){
    char testString[5] = "1234";
    clearString(testString);
    if(testString[0]=='\0' && testString[1]=='\0' && testString[2]=='\0' && testString[3]=='\0'){
        printStringLCD("clearString:PASS");
    }
    else{
        printStringLCD("clearString:FAIL");
    }
}
void test_checkPass(){
    int pass = 1;
    char testPassa[5] = "1234";
    char testPassb[5] = "1111";
    char testPassc[5] = "0000";
    char testPassd[5] = "0159";
    
    if(checkPass("1112", testPassa, testPassb, testPassc, testPassd)==0){
        pass = 0;
        printStringLCD("checkPass:FAIL");
    }
    if(checkPass("0000", testPassa, testPassb, testPassc, testPassd)!=0){
        pass = 0;
        printStringLCD("checkPass:FAIL");
    }
    if(pass == 1){
        printStringLCD("checkPass:PASS");
    }
}
void test_updatePass(){
    char testPassa[5] = "1234";
    char testPassb[5] = "1111";
    char testPassc[5] = "0000";
    char testPassd[5] = "0159";
    
    updatePass("9999", testPassa, testPassb, testPassc, testPassd, 0);
    updatePass("8888", testPassa, testPassb, testPassc, testPassd, 1);
    updatePass("7777", testPassa, testPassb, testPassc, testPassd, 2);
    updatePass("6666", testPassa, testPassb, testPassc, testPassd, 3);
    
    if((strcmp("9999", testPassa)!=0) || (strcmp("8888", testPassb)!=0) || (strcmp("7777", testPassc)!=0) || (strcmp("6666", testPassd)!=0)){
        printStringLCD("updatePass:FAIL");
    }
    else{
        printStringLCD("updatePass:PASS");
    }
}
