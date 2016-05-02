/* 
 * File:   main.c
 * Author: jsiek
 *
 * Created on April 19, 2016, 11:53 PM
 */

#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "interrupt.h"
#include "lcd.h"
#include "timer.h"
#include "switch.h"
#include "uart.h"
#include "receivers.h"
#include "smotors.h"
#include <math.h>

#define OFF 0
#define ON 1
#define PRESS 0
#define RELEASE 1
#define CN_E 0x00010000
#define RX57  PORTEbits.RE0
#define RX40  PORTEbits.RE2
#define RX30  PORTEbits.RE4
#define MAXSPEED 150000
#define MAXSPEEDDISTANCE 6



typedef enum stateTypeEnum {
    idle, go
} stateType;

volatile stateType state = idle;
volatile int pressRelease = PRESS;
volatile int receiver = 0;
volatile int fiftySevenK = 0;
volatile int fourtyK = 0;
volatile int thirtyK = 0;
volatile int updatePos = 0;

int main() {
    SYSTEMConfigPerformance(10000000);
    enableInterrupts();
    initTimer1();
    initTimer2();
    initTimer45();
    initSW1();
    initLCD();
    initUART();
    initReceivers();
    float tower1Steps = 12000;
    float tower2Steps = 25600;
    float tower3Steps = 46080;
    int tempVar = 0;
    int tempScalar = 0;
    int tempTowerStepPos = 0;
    int towerDetected = 0;
    int towerDetectDiff = 0;
    int towerDiffNum = 0;
    int currentWaypoint = 0;
    float newXPos = 0;
    float newYPos = 0;
    float newHeading = 270;
    float currXPos = 24;
    float currYPos = 90;
    float currHeading = 270;
    unsigned char posArray[64];
    char s[64] = {};
    strcpy(posArray, "");
    float wayPoints[2][2] = {
    {24,80},
    {24,60}
    };
    float relaWayX = 0;
    float relaWayY = 0;
    float relaWayAng = 0;
    float newWayDis = 0;
    float currWayDis = 0;
    float disMultiplier = 1;
    
            
        sendChar(0x0A);
        sendChar(0x0A);
        sendChar(0x0A);
        sendChar(0x0A);
        sendCommand("ZSL 0");
        
    while(1){
        
        if(towerDetected==1){//find the closest old tower to this new tower position and mark it as such
            //find the closest old tower to this new tower position and mark it as such
            querryPos(posArray);
            tempTowerStepPos= atoi(posArray);
            if(abs(tempTowerStepPos-tower1Steps)<180){towerDetectDiff=abs(tempTowerStepPos-tower1Steps); towerDiffNum = 1;} //sorry this is completely unreadable, this finds the closest old tower value to recent tower detect
            else if(abs(360+tempTowerStepPos-tower1Steps)<180){towerDetectDiff=abs(360+tempTowerStepPos-tower1Steps); towerDiffNum = 1;}
            if((abs(tempTowerStepPos-tower1Steps)<180)&&(towerDetectDiff>abs(tempTowerStepPos-tower1Steps))){towerDetectDiff=abs(tempTowerStepPos-tower1Steps); towerDiffNum = 2;}
            else if((abs(360+tempTowerStepPos-tower1Steps)<180)&&(towerDetectDiff>abs(360+tempTowerStepPos-tower1Steps))){towerDetectDiff=abs(360+tempTowerStepPos-tower1Steps); towerDiffNum = 2;}
            if((abs(tempTowerStepPos-tower1Steps)<180)&&(towerDetectDiff>abs(tempTowerStepPos-tower1Steps))){towerDetectDiff=abs(tempTowerStepPos-tower1Steps); towerDiffNum = 3;}
            else if((abs(360+tempTowerStepPos-tower1Steps)<180)&&(towerDetectDiff>abs(360+tempTowerStepPos-tower1Steps))){towerDetectDiff=abs(360+tempTowerStepPos-tower1Steps); towerDiffNum = 3;}
            
            if(towerDiffNum == 1){tower1Steps=tempTowerStepPos;}//if tower 1 is the closest tower to detected tower, give it the detected tower position
            if(towerDiffNum == 2){tower2Steps=tempTowerStepPos;}//if tower 1 is the closest tower to detected tower, give it the detected tower position
            if(towerDiffNum == 3){tower3Steps=tempTowerStepPos;}//if tower 1 is the closest tower to detected tower, give it the detected tower position
                
            towerDiffNum = 0;
        }
        
        
        if(updatePos==1){
            moveCursorLCD(0,0);
            printStringLCD("FUCKING FUCK STICKS");
            newXPos = posXreturn(tower1Steps, tower2Steps, tower3Steps);
            newYPos = posYreturn(tower1Steps, tower2Steps, tower3Steps);
            newHeading = heading(tower1Steps, tower2Steps, tower3Steps);
            
            newWayDis = (sqrt((newXPos-currXPos)*(newXPos-currXPos)+(newYPos-currYPos)*(newYPos-currYPos)));
            
            if((newWayDis<20)&&(newHeading>0)){ //if new distance is less than 20 inches from old distance, use new
                currXPos = newXPos; 
                currYPos = newYPos; 
                currHeading = newHeading;
                newWayDis = (sqrt(((wayPoints[currentWaypoint][0])-currXPos)*((wayPoints[currentWaypoint][0])-currXPos)+((wayPoints[currentWaypoint][1])-currYPos)*((wayPoints[currentWaypoint][1])-currYPos)));
                if((newWayDis<3)&&(currentWaypoint!=1)){    //if distance from waypoint is less than 3 inches and not the last waypoint
                    currentWaypoint++;
                    newWayDis = (sqrt(((wayPoints[currentWaypoint][0])-currXPos)*((wayPoints[currentWaypoint][0])-currXPos)+((wayPoints[currentWaypoint][1])-currYPos)*((wayPoints[currentWaypoint][1])-currYPos)));
                }
                currWayDis = newWayDis;
                
                relaWayX = (wayPoints[currentWaypoint][0])-currXPos;
                relaWayY = (wayPoints[currentWaypoint][1])-currYPos;                
                relaWayX = relaWayX/(sqrt(relaWayX*relaWayX+relaWayY*relaWayY)); //normalize the X direction to the waypoint to the unit circle               
                relaWayY = relaWayY/(sqrt(relaWayX*relaWayX+relaWayY*relaWayY)); //normalize the Y direction to the waypoint to the unit circle 
                relaWayAng = getWaypointHeading(relaWayX,relaWayY);
                
                if((fabsf(currHeading-relaWayAng)<180)&&((currHeading-relaWayAng)>=0)){//if the current heading is to the left of waypoint heading
                    tempScalar = ((((currHeading-relaWayAng)/360)-.15)*1.2);       //set right motor speed based off angle
                    tempScalar = tempVar*tempVar*tempVar*tempVar*tempVar*tempVar;  //scale angle exponentially
                    if(tempScalar>=1){tempScalar = 1;}        //if angle is small enough just set angle multiplier to 1
                    if(newWayDis<MAXSPEEDDISTANCE){disMultiplier = newWayDis/MAXSPEEDDISTANCE; }  //set distance multiplier
                    tempVar = tempScalar*MAXSPEED*disMultiplier;     //set right motor speed based off angle, distance and maxspeed
                    sprintf(s,"RSL %d", (int)(tempVar));
                    sendCommand(s);
                    
                    tempScalar = tempScalar + .5;
                    if(tempScalar>=1){tempScalar = 1;}        //if angle is small enough just set angle multiplier to 1
                    tempVar = tempScalar*disMultiplier*MAXSPEED*(-1);       //set left motor speed based on max speed and distance multiplier
                    sprintf(s,"LSL %d", (int)(tempVar));
                    sendCommand(s);
                }
                else if((fabsf(currHeading-relaWayAng)>180)&&((currHeading-relaWayAng)<0)){ //if the current heading is to the left of waypoint heading
                    tempScalar = ((((360+currHeading-relaWayAng)/360)-.15)*1.2);       //set right motor speed based off angle
                    tempScalar = tempVar*tempVar*tempVar*tempVar*tempVar*tempVar;  //scale angle exponentially
                    if(tempScalar>=1){tempScalar = 1;}        //if angle is small enough just set angle multiplier to 1
                    if(newWayDis<MAXSPEEDDISTANCE){disMultiplier = newWayDis/MAXSPEEDDISTANCE; }  //set distance multiplier
                    tempVar = tempScalar*MAXSPEED*disMultiplier;     //set right motor speed based off angle, distance and maxspeed
                    sprintf(s,"RSL %d", (int)(tempVar));
                    sendCommand(s);
                    
                    tempScalar = tempScalar + .5;
                    if(tempScalar>=1){tempScalar = 1;}        //if angle is small enough just set angle multiplier to 1
                    tempVar = tempScalar*disMultiplier*MAXSPEED*(-1);       //set left motor speed based on max speed and distance multiplier
                    sprintf(s,"LSL %d", (int)(tempVar));
                    sendCommand(s);
                }
                else if ((currHeading-relaWayAng)<0){  //if the current heading is to the right of the waypoint direction
                    tempScalar = ((((relaWayAng-currHeading)/360)-.15)*1.2);       //set right motor speed based off angle
                    tempScalar = tempVar*tempVar*tempVar*tempVar*tempVar*tempVar;  //scale angle exponentially
                    if(tempScalar>=1){tempScalar = 1;}        //if angle is small enough just set angle multiplier to 1
                    if(newWayDis<MAXSPEEDDISTANCE){disMultiplier = newWayDis/MAXSPEEDDISTANCE; }  //set distance multiplier
                    tempVar = tempScalar*MAXSPEED*disMultiplier*(-1);     //set LEFT motor speed based off angle, distance and maxspeed
                    sprintf(s,"LSL %d", (int)(tempVar));
                    sendCommand(s);
                    
                    tempScalar = tempScalar + .5;
                    if(tempScalar>=1){tempScalar = 1;}        //if angle is small enough just set angle multiplier to 1
                    tempVar = tempScalar*disMultiplier*MAXSPEED;       //set right motor speed based on max speed and distance multiplier
                    sprintf(s,"RSL %d", (int)(tempVar));
                    sendCommand(s);
                } 
                else if((currHeading-relaWayAng)>=0){    //if the current heading is to the right of the waypoint direction
                    tempScalar = ((((360+relaWayAng-currHeading)/360)-.15)*1.2);       //set right motor speed based off angle
                    tempScalar = tempVar*tempVar*tempVar*tempVar*tempVar*tempVar;  //scale angle exponentially
                    if(tempScalar>=1){tempScalar = 1;}        //if angle is small enough just set angle multiplier to 1
                    if(newWayDis<MAXSPEEDDISTANCE){disMultiplier = newWayDis/MAXSPEEDDISTANCE; }  //set distance multiplier
                    tempVar = tempScalar*MAXSPEED*disMultiplier*(-1);     //set LEFT motor speed based off angle, distance and maxspeed
                    sprintf(s,"LSL %d", (int)(tempVar));
                    sendCommand(s);
                    
                    tempScalar = tempScalar + .5;
                    if(tempScalar>=1){tempScalar = 1;}        //if angle is small enough just set angle multiplier to 1
                    tempVar = tempScalar*disMultiplier*MAXSPEED;       //set right motor speed based on max speed and distance multiplier
                    sprintf(s,"RSL %d", (int)(tempVar));
                    sendCommand(s);
                }
            }
            else{
                sendCommand("RSL 0");
                sendCommand("LSL 0");
            }
            
            //print out XY position
            moveCursorLCD(0,9);
            sprintf(s,"X%.1f , Y%.1f", currXPos, currYPos);
            printStringLCD(s);
            updatePos=0;
            
        }
        //delayUs(2000);

        if(RX30==0){
            moveCursorLCD(0,9);
            printStringLCD("30kHZ");
        }
        else{
            moveCursorLCD(0,9);
            printStringLCD("     ");
        }
        
        if(RX40==0){
            moveCursorLCD(1,0);
            printStringLCD("40kHZ");
        }
        else{
            moveCursorLCD(1,0);
            printStringLCD("     ");
        }
        
        if(RX57==0){
            moveCursorLCD(1,9);
            printStringLCD("57kHZ");
        }
        else{
            moveCursorLCD(1,9);
            printStringLCD("     ");
        }
        
//        switch(receiver){
//            case 57:
//                moveCursorLCD(1,9);
//                printStringLCD("57kHZ");
//                break;
//            case 40:
//                moveCursorLCD(1,0);
//                printStringLCD("40kHZ");
//                break;
//            case 30:
//                moveCursorLCD(0,9);
//                printStringLCD("30kHZ");
//                break;
//            default:
//                //clearLCD();
//                //printStringLCD("NONE");
//                break;                
//        }
        
        if((receiver == 30)||(receiver == 40)||(receiver == 57)||(receiver == 0)){
            delayUs(20000);
            querryPos(posArray);
            moveCursorLCD(0,0);
            printStringLCD(posArray);
            receiver=1;
        }
    }
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt(){
    int dummy;
    dummy = PORTA; // Read the Port A
    dummy = PORTE; //Read the Port E
    if (IFS1bits.CNBIF == 0){updatePos = 1;} 
    if (IFS1bits.CNEIF == 1){ T2CONbits.TON = ON;} //if the receiver modules are triggered then use the 100Us delay
    else {T1CONbits.TON = ON;}
    IFS1bits.CNAIF = 0; IFS1bits.CNEIF = 0; //lower flags
}

void __ISR(_TIMER_1_VECTOR, IPL7SRS) _T1Interrupt() { //Timer 1 is for debouncing
    IFS0bits.T1IF = 0; //lower the flag
    T1CONbits.TON = OFF;
    if(pressRelease == RELEASE){
        if(state != idle) state = idle; 
        else if(state == idle) state = go;
        pressRelease = PRESS;
    }
    else{
        pressRelease = RELEASE;
    }
}

void __ISR(_TIMER_2_VECTOR, IPL7SRS) _T2Interrupt() { //Timer 2 is for debouncing
    IFS0bits.T2IF = 0; //lower the flag
    T2CONbits.TON = OFF;
    if(RX57 == 0)
    {
        receiver = 57;
        fiftySevenK=1;
    }
    if(RX40 == 0)
    {
        receiver = 40;
        fourtyK = 1;
    }
    else{fourtyK=0;}
    if(RX30 == 0)
    {
        receiver = 30;
        thirtyK = 1;
    }
    else{thirtyK=0;}
}