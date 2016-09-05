/* 
 * File:   main.c
 * Author: jsiek <---the jew
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

#define GO 1
#define IDLE 0
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
#define TOTALWAYPOINTS 20
#define DETECTOROFFSETSTEPS 13200
#define STEPSPERREV 51200



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
volatile int towerDetected = 0;
volatile int toggle = 0;
volatile int MISSION = IDLE;

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
    float tower1Steps = 48578;
    float tower2Steps = 20808;
    float tower3Steps = 35778;
    float tempScalar = 0;
    int tempTowerStepPos = 0;
    int towerDiffNum = 0;
    int currentWaypoint = 0;
    float newXPos = 0;
    float newYPos = 0;
    float newHeading = 270;
    float currXPos = 36;
    float currYPos = 108;
    float currHeading = 270;
    float newPosArray[3]={0,0,270};
    unsigned char posArray[64];
    char s[64] = {};
    strcpy(posArray, "");
    float wayPoints[TOTALWAYPOINTS][2] = {
    {36,72},    //1st waypoint
    {72,72},    //2
    {72,36},    //3
    {84,36},    //4
    {90,42},    //5
    {96,36},    //6
    {102,42},    //7
    {108,36},    //8
    {114,42},    //9
    {120,36},    //10
    {126,42},    //11
    {132,36},    //12
    {140,40},    //13
    {144,48},    //14
    {140,56},    //15
    {132,60},    //16
    {124,64},    //17
    {120,72},    //18
    {124,80},    //19
    {36,108}    //20
    };
    float relaWayX = 0;
    float relaWayY = 0;
    float relaWayAng = 0;
    float *relaWayAngPoint=&relaWayAng;
    float newWayDis = 0;
    float currWayDis = 0;
    float disMultiplier = 1;
    float headingFromWaypoint = 0;
    
            
        sendChar(0x0A);
        sendChar(0x0A);
        sendChar(0x0A);
        sendChar(0x0A);
        sendCommand("ZSL 200000");
        sendCommand("LVM 155000");
        
        
    while(1){
        
        
        
        
        
        if(towerDetected==1){//find the closest old tower to this new tower position and mark it as such
            querryPos(posArray);
            tempTowerStepPos= atoi(posArray);
            
            switch(towerIdentification((tower1Steps*360/51200), (tower2Steps*360/51200), (tower3Steps*360/51200), tempTowerStepPos)){
                case 1:
                    tower1Steps=tempTowerStepPos;       //if tower 1 is the closest tower to detected tower, give it the detected tower position
                    break;
                case 2:
                    tower2Steps=tempTowerStepPos;       //if tower 2 is the closest tower to detected tower, give it the detected tower position
                    break;
                case 3:
                    tower3Steps=tempTowerStepPos;       //if tower 3 is the closest tower to detected tower, give it the detected tower position
                    break;
                default: //something has gone terribly wrong
                    break;
            }
            /*
            towerDiffNum = towerIdentification((tower1Steps*360/51200), (tower2Steps*360/51200), (tower3Steps*360/51200), tempTowerStepPos);
            if(towerDiffNum == 1){ tower1Steps=tempTowerStepPos; }//if tower 1 is the closest tower to detected tower, give it the detected tower position
            if(towerDiffNum == 2){ tower2Steps=tempTowerStepPos; }//if tower 2 is the closest tower to detected tower, give it the detected tower position
            if(towerDiffNum == 3){ tower3Steps=tempTowerStepPos; }//if tower 3 is the closest tower to detected tower, give it the detected tower position
            */    
            
            strcpy(posArray, "");
            towerDiffNum = 0;
            towerDetected = 0;
        }
          
          
                
        if(updatePos==1){
            //moveCursorLCD(0,0);       //this was just test code to see if it got here
            //newXPos = posXreturn(tower1Steps, tower2Steps, tower3Steps);
            //newYPos = posYreturn(tower1Steps, tower2Steps, tower3Steps);
            //newHeading = heading(tower1Steps, tower2Steps, tower3Steps)+DETECTOROFFSETSTEPS;
            getLocAndHeading(tower1Steps, tower2Steps, tower3Steps, newPosArray);
            newXPos = newPosArray[0];
            newYPos = newPosArray[1];
            newHeading =newPosArray[2];
            //if(newHeading>STEPSPERREV){ newHeading-STEPSPERREV; }
            
            newWayDis = (sqrt((newXPos-currXPos)*(newXPos-currXPos)+(newYPos-currYPos)*(newYPos-currYPos)));
            
            if((newWayDis<20)&&(newHeading>0)){ //if new distance is less than 20 inches from old distance, use new
                currXPos = newXPos; 
                currYPos = newYPos; 
                currHeading = newHeading;
                newWayDis = (sqrt(((wayPoints[currentWaypoint][0])-currXPos)*((wayPoints[currentWaypoint][0])-currXPos)+((wayPoints[currentWaypoint][1])-currYPos)*((wayPoints[currentWaypoint][1])-currYPos)));
                if((newWayDis<3)&&(currentWaypoint!=TOTALWAYPOINTS-1)){    //if distance from waypoint is less than 3 inches and not the last waypoint
                    currentWaypoint++;
                    newWayDis = (sqrt(((wayPoints[currentWaypoint][0])-currXPos)*((wayPoints[currentWaypoint][0])-currXPos)+((wayPoints[currentWaypoint][1])-currYPos)*((wayPoints[currentWaypoint][1])-currYPos)));
                }
                currWayDis = newWayDis;
                
                relaWayX = (wayPoints[currentWaypoint][0])-currXPos;
                relaWayY = (wayPoints[currentWaypoint][1])-currYPos;                
                relaWayX = relaWayX/(sqrt(relaWayX*relaWayX+relaWayY*relaWayY)); //normalize the X direction to the waypoint to the unit circle               
                relaWayY = relaWayY/(sqrt(relaWayX*relaWayX+relaWayY*relaWayY)); //normalize the Y direction to the waypoint to the unit circle 
                getWaypointHeading(relaWayX,relaWayY,relaWayAngPoint);
                headingFromWaypoint = angleToTarget(relaWayAng, currHeading);   //find the angle to the target
                
                if(headingFromWaypoint>=0){//if the current heading is to the left of waypoint heading
                    tempScalar = (((fabsf(headingFromWaypoint)/360)-.15)*1.2);       //set right motor speed based off angle
                    turnRightScaled( tempScalar,  newWayDis,  currWayDis,  disMultiplier );  //set the speed of the motors in a rightward turn
                    /*
                    tempScalar = tempScalar*tempScalar*tempScalar*tempScalar*tempScalar*tempScalar;  //scale angle exponentially
                    if(tempScalar>=1){tempScalar = 1;}        //if angle is small enough just set angle multiplier to 1
                    if(newWayDis<MAXSPEEDDISTANCE){disMultiplier = newWayDis/MAXSPEEDDISTANCE; }  //set distance multiplier
                    tempVar = tempScalar*MAXSPEED*disMultiplier;     //set right motor speed based off angle, distance and maxspeed
                    strcpy(s, "");
                    sprintf(s,"RSL %d", (int)(tempVar));
                    sendCommand(s);
                    delayUs(10000);
                    
                    tempScalar = tempScalar + .5;
                    if(tempScalar>=1){tempScalar = 1;}        //if angle is small enough just set angle multiplier to 1
                    tempVar = tempScalar*disMultiplier*MAXSPEED*(-1);       //set left motor speed based on max speed and distance multiplier
                    strcpy(s, "");
                    sprintf(s,"LSL %d", (int)(tempVar));
                    sendCommand(s);
                    delayUs(10000);
                      */
                }
                else if(headingFromWaypoint<0){ //if the current heading is to the right of waypoint heading
                    tempScalar = (((fabsf(headingFromWaypoint)/360)-.15)*1.2);       //set right motor speed based off angle
                    turnLeftScaled( tempScalar,  newWayDis,  currWayDis,  disMultiplier ); //set the speed of the motors in a leftward turn
                    /*
                    tempScalar = tempScalar*tempScalar*tempScalar*tempScalar*tempScalar*tempScalar;  //scale angle exponentially
                    if(tempScalar>=1){tempScalar = 1;}        //if angle is small enough just set angle multiplier to 1
                    if(newWayDis<MAXSPEEDDISTANCE){disMultiplier = newWayDis/MAXSPEEDDISTANCE; }  //set distance multiplier
                    tempVar = tempScalar*MAXSPEED*disMultiplier;     //set right motor speed based off angle, distance and maxspeed
                    strcpy(s, "");
                    sprintf(s,"RSL %d", (int)(tempVar));
                    sendCommand(s);
                    delayUs(10000);
                    
                    tempScalar = tempScalar + .5;
                    if(tempScalar>=1){tempScalar = 1;}        //if angle is small enough just set angle multiplier to 1
                    tempVar = tempScalar*disMultiplier*MAXSPEED*(-1);       //set left motor speed based on max speed and distance multiplier
                    strcpy(s, "");
                    sprintf(s,"LSL %d", (int)(tempVar));
                    sendCommand(s);
                    delayUs(10000);
                    */
                }
                /*
                else if ((currHeading-relaWayAng)<0){  //if the current heading is to the right of the waypoint direction
                    tempScalar = ((((relaWayAng-currHeading)/360)-.15)*1.2);       //set right motor speed based off angle
                    tempScalar = tempScalar*tempScalar*tempScalar*tempScalar*tempScalar*tempScalar;  //scale angle exponentially
                    if(tempScalar>=1){tempScalar = 1;}        //if angle is small enough just set angle multiplier to 1
                    if(newWayDis<MAXSPEEDDISTANCE){disMultiplier = newWayDis/MAXSPEEDDISTANCE; }  //set distance multiplier
                    tempVar = tempScalar*MAXSPEED*disMultiplier*(-1);     //set LEFT motor speed based off angle, distance and maxspeed
                    strcpy(s, "");
                    sprintf(s,"LSL %d", (int)(tempVar));
                    sendCommand(s);
                    delayUs(10000);
                    
                    tempScalar = tempScalar + .5;
                    if(tempScalar>=1){tempScalar = 1;}        //if angle is small enough just set angle multiplier to 1
                    tempVar = tempScalar*disMultiplier*MAXSPEED;       //set right motor speed based on max speed and distance multiplier
                    strcpy(s, "");
                    sprintf(s,"RSL %d", (int)(tempVar));
                    sendCommand(s);
                    delayUs(10000);
                } 
                else if((currHeading-relaWayAng)>=0){    //if the current heading is to the right of the waypoint direction
                    tempScalar = ((((360+relaWayAng-currHeading)/360)-.15)*1.2);       //set right motor speed based off angle
                    tempScalar = tempScalar*tempScalar*tempScalar*tempScalar*tempScalar*tempScalar;  //scale angle exponentially
                    if(tempScalar>=1){tempScalar = 1;}        //if angle is small enough just set angle multiplier to 1
                    if(newWayDis<MAXSPEEDDISTANCE){disMultiplier = newWayDis/MAXSPEEDDISTANCE; }  //set distance multiplier
                    tempVar = tempScalar*MAXSPEED*disMultiplier*(-1);     //set LEFT motor speed based off angle, distance and maxspeed
                    strcpy(s, "");
                    sprintf(s,"LSL %d", (int)(tempVar));
                    sendCommand(s);
                    delayUs(10000);
                    
                    tempScalar = tempScalar + .5;
                    if(tempScalar>=1){tempScalar = 1;}        //if angle is small enough just set angle multiplier to 1
                    tempVar = tempScalar*disMultiplier*MAXSPEED;       //set right motor speed based on max speed and distance multiplier
                    strcpy(s, "");
                    sprintf(s,"RSL %d", (int)(tempVar));
                    sendCommand(s);
                    delayUs(10000);
                }
                */
            }
            else{
                sendCommand("RSL 0");
                sendCommand("LSL 0");
            }
            
            //print out XY position
            clearLCD();
            moveCursorLCD(0,0);
            sprintf(s,"X%.1f , Y%.1f", currXPos, currYPos);
            printStringLCD(s);
            moveCursorLCD(1,0);
            sprintf(s,"Angle: %.1f", currHeading);
            printStringLCD(s);
            
            updatePos=0;
            
        }
                 
    }
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt(){
    int dummy;
    dummy = PORTA; // Read the Port A
    dummy = PORTB; //Read the Port B
    dummy = PORTE; //Read the Port E
    dummy = PORTG; //Read the Port G
    if (IFS1bits.CNGIF == 1) {T1CONbits.TON = ON; toggle = 1;} //Mission Switch
    if (IFS1bits.CNBIF == 1){updatePos = 1;} //limit switch
    if (IFS1bits.CNEIF == 1){ T2CONbits.TON = ON;} //if the receiver modules are triggered then use the 100Us delayZ
    //else {T1CONbits.TON = ON;}
    IFS1bits.CNAIF = 0; IFS1bits.CNBIF = 0; IFS1bits.CNEIF =0; IFS1bits.CNGIF = 0; //lower flags
}

void __ISR(_TIMER_1_VECTOR, IPL7SRS) _T1Interrupt() { //Timer 1 is for debouncing
    IFS0bits.T1IF = 0; //lower the flag
    T1CONbits.TON = OFF;
    if(toggle == 1)
    {
        MISSION = MISSION ^= 1; //alternate the STATUS pin
        toggle = 0;
    }
}

void __ISR(_TIMER_2_VECTOR, IPL7SRS) _T2Interrupt() { //Timer 2 is for debouncing
    IFS0bits.T2IF = 0; //lower the flag
    T2CONbits.TON = OFF;
    towerDetected = 1;
}