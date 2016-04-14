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

int onLine(float adcVal){
    float volt = 0;
    volt = 3.3*(adcVal)/1023;
    if(volt <= .9){
        return OFFLINE; //off line
    }
    else{
        return ONLINE;
    }
}

stateType calcNextState(float ADCArray[]){
    stateType nextState; 
    int detectorArray[4]; int i;
    int arrayState= 0b0000;
    
    for(i=0; i<4; i++){
        detectorArray[i] = onLine(ADCArray[i]);
        arrayState |= onLine(ADCArray[i])<<(3-i);
    }
    
    switch(arrayState){
        case 0b0000: nextState = end;           break;//0: //end line detected OR T joint of loop
        case 0b0001: nextState = forward;       break;//1: // left line detected, ignore for now
        case 0b0010: nextState = forward;       break;//2: //anomally detection, ignore and go forward
        case 0b0011: nextState = hardLeft;      break;//3: //drifting hard right turn hard left
        case 0b0100: nextState = forward;       break;//4: //anomally detection, ignore and go forward
        case 0b0101: nextState = forward;       break;//5: //anomally detection, ignore and go forward
        case 0b0110: nextState = forward;       break;//6: //anomally detection, ignore and go forward
        case 0b0111: nextState = superHardLeft; break;//7: //drifting superhard right, turn super hard left
        case 0b1000: nextState = forward;       break;//8: // right line detected, ignore for now
        case 0b1001: nextState = forward;       break;//9:  //centered forward
        case 0b1010: nextState = forward;       break;//10: //anomally detection, ignore and go forward
        case 0b1011: nextState = left;          break;//11: //drifting right, turn left
        case 0b1100: nextState = hardRight;     break;//12: //drifting hard left, turn hard right
        case 0b1101: nextState = right;         break;//13: //drifting left, turn right
        case 0b1110: nextState = superHardRight;break;//14: //drifting super hard left, turn super hard right
        case 0b1111: nextState = forward;       break;//15: //no lines detected, end of course
    }
    
//    
//    //AN0 is far left, AN3 is far right
//    if(detectorArray[0] == OFFLINE && detectorArray[1] == ONLINE && detectorArray[2] == ONLINE && detectorArray[3] == OFFLINE){ //forward
//        nextState = forward;
//    }
//    else if(detectorArray[0] == OFFLINE && detectorArray[1] == OFFLINE && detectorArray[2] == ONLINE && detectorArray[3] == OFFLINE){ //left
//        nextState = right;
//    }
//    else if(detectorArray[0] == OFFLINE && detectorArray[1] == OFFLINE && detectorArray[2] == ONLINE && detectorArray[3] == ONLINE){ //hardleft
//        nextState = hardRight;
//    }
//    else if(detectorArray[0] == OFFLINE && detectorArray[1] == OFFLINE && detectorArray[2] == OFFLINE && detectorArray[3] == ONLINE){ //superhardleft
//        nextState = superHardRight;
//    }
//    else if(detectorArray[0] == OFFLINE && detectorArray[1] == ONLINE && detectorArray[2] == OFFLINE && detectorArray[3] == OFFLINE){ //right
//        nextState = left;
//    }
//    else if(detectorArray[0] == ONLINE && detectorArray[1] == ONLINE && detectorArray[2] == OFFLINE && detectorArray[3] == OFFLINE){ //hardright
//        nextState = hardLeft;
//    }
//    else if(detectorArray[0] == ONLINE && detectorArray[1] == OFFLINE && detectorArray[2] == OFFLINE && detectorArray[3] == OFFLINE){ //superhardright
//        nextState = superHardLeft;
//    }
//    else if(detectorArray[0] == OFFLINE && detectorArray[1] == ONLINE && detectorArray[2] == ONLINE && detectorArray[3] == ONLINE){ //ignore inner loop...for now
//        nextState = forward;
//    }
//    else if(detectorArray[0] == ONLINE && detectorArray[1] == ONLINE && detectorArray[2] == ONLINE && detectorArray[3] == OFFLINE){ //ignore inner loop...for now
//        nextState = forward;
//    }
//    else if(detectorArray[0] == ONLINE && detectorArray[1] == ONLINE && detectorArray[2] == ONLINE && detectorArray[3] == ONLINE){ 
//        nextState = end; //probably should implement some check before spin...we'll see
//    }
//    
    return nextState;
}