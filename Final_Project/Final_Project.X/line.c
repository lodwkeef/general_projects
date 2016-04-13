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
    if(volt <= 2.5){
        return OFFLINE; //off line
    }
    else{
        return ONLINE;
    }
}

stateType calcNextState(float ADCArray[]){
    stateType nextState; 
    int detectorArray[4]; int i;
    
    for(i=0; i<4; i++){
        detectorArray[i] = onLine(ADCArray[i]);
    }
    
    //AN0 is far left, AN3 is far right
    if(detectorArray[0] == OFFLINE && detectorArray[1] == ONLINE && detectorArray[2] == ONLINE && detectorArray[3] == OFFLINE){ //forward
        nextState = forward;
    }
    else if(detectorArray[0] == OFFLINE && detectorArray[1] == OFFLINE && detectorArray[2] == ONLINE && detectorArray[3] == OFFLINE){ //left
        nextState = left;
    }
    else if(detectorArray[0] == OFFLINE && detectorArray[1] == OFFLINE && detectorArray[2] == ONLINE && detectorArray[3] == ONLINE){ //hardleft
        nextState = hardLeft;
    }
    else if(detectorArray[0] == OFFLINE && detectorArray[1] == OFFLINE && detectorArray[2] == OFFLINE && detectorArray[3] == ONLINE){ //superhardleft
        nextState = superHardLeft;
    }
    else if(detectorArray[0] == OFFLINE && detectorArray[1] == ONLINE && detectorArray[2] == OFFLINE && detectorArray[3] == OFFLINE){ //right
        nextState = right;
    }
    else if(detectorArray[0] == ONLINE && detectorArray[1] == ONLINE && detectorArray[2] == OFFLINE && detectorArray[3] == OFFLINE){ //hardright
        nextState = hardRight;
    }
    else if(detectorArray[0] == ONLINE && detectorArray[1] == OFFLINE && detectorArray[2] == OFFLINE && detectorArray[3] == OFFLINE){ //superhardright
        nextState = superHardRight;
    }
    else if(detectorArray[0] == OFFLINE && detectorArray[1] == ONLINE && detectorArray[2] == ONLINE && detectorArray[3] == ONLINE){ //ignore inner loop...for now
        nextState = forward;
    }
    else if(detectorArray[0] == ONLINE && detectorArray[1] == ONLINE && detectorArray[2] == ONLINE && detectorArray[3] == OFFLINE){ //ignore inner loop...for now
        nextState = forward;
    }
    else if(detectorArray[0] == ONLINE && detectorArray[1] == ONLINE && detectorArray[2] == ONLINE && detectorArray[3] == ONLINE){ 
        nextState = end; //probably should implement some check before spin...we'll see
    }
    
    return nextState;
}