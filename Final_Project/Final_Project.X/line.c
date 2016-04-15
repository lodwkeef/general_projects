/* 
 * File:   line.c
 * Authors: Alex Thompson
 *          Ben Schifman
 *          Justin Siekmann
 *          Kevin Curtis
 *
 * Created on April 12, 2016, 4:32 PM
 */

#include <xc.h>
#include "line.h"
#include "adc.h"

#define OFFLINE 1
#define ONLINE 0

int onLine(float adcVal, int diode){
    float volt = 0;
    volt = 3.3*(adcVal)/1023;
    switch(diode){
        case 0:
            if(volt <= 1.1){
                return OFFLINE; //off line
            }
            else{
                return ONLINE;
            }
            break;
        case 1:
            if(volt <= 1.1){
                return OFFLINE; //off line
            }
            else{
                return ONLINE;
            }
            break;
        case 2:
            if(volt <= .9){
                return OFFLINE; //off line
            }
            else{
                return ONLINE;
            }
            break;
        case 3:
            if(volt <= 1.1){
                return OFFLINE; //off line
            }
            else{
                return ONLINE;
            }
            break;
    }
}

stateType calcNextState(float ADCArray[], float volt[], int innerLoop, int spun){
    stateType nextState; 
    int detectorArray[4]; int i;
    int arrayState= 0b0000;
    
    volt[0] = 3.3*(ADCArray[0])/1023;
    volt[1] = 3.3*(ADCArray[1])/1023;
    volt[2] = 3.3*(ADCArray[2])/1023;
    volt[3] = 3.3*(ADCArray[3])/1023;
    
    arrayState |= onLine(ADCArray[0], 0)<<(3);
    arrayState |= onLine(ADCArray[1], 1)<<(2);
    arrayState |= onLine(ADCArray[2], 2)<<(1);
    arrayState |= onLine(ADCArray[3], 3)<<(0);
    
    
    switch(arrayState){
        case 0b0000:
            if(innerLoop == 0){
                nextState = spin180; 
            }
            else{
                nextState = spin90;
            }
        break;//0: //end line detected OR T joint of loop
        case 0b0001: 
            if(spun = 1){
                nextState = forward;
            }
            else{
                nextState = spin90Left;
            }
            
        break;//1: // left line detected, ignore and go forward
        case 0b0010: nextState = forward;       break;//2: //anomally detection, ignore and go forward
        case 0b0011: nextState = hardLeft;      break;//3: //drifting hard right turn hard left
        case 0b0100: nextState = forward;       break;//4: //anomally detection, ignore and go forward
        case 0b0101: nextState = forward;       break;//5: //anomally detection, ignore and go forward
        case 0b0110: nextState = forward;       break;//6: //anomally detection, ignore and go forward
        case 0b0111: nextState = superHardLeft; break;//7: //drifting superhard right, turn super hard left
        case 0b1000: nextState = spin90;        break;//8: // right line detected, ignore for now
        case 0b1001: nextState = forward;       break;//9:  //centered forward
        case 0b1010: nextState = forward;       break;//10: //anomally detection, ignore and go forward
        case 0b1011: nextState = left;          break;//11: //drifting right, turn left
        case 0b1100: nextState = hardRight;     break;//12: //drifting hard left, turn hard right
        case 0b1101: nextState = right;         break;//13: //drifting left, turn right
        case 0b1110: nextState = superHardRight;break;//14: //drifting super hard left, turn super hard right
        case 0b1111: nextState = forward;       break;//15: //no lines detected, end of course
    }
    
    return nextState;
}