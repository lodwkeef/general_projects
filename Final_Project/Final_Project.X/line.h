/* 
 * File:   line.h
 * Authors: Alex Thompson
 *          Ben Schifman
 *          Justin Siekmann
 *          Kevin Curtis
 *
 * Created on April 12, 2016, 4:32 PM
 */


#ifndef LINE_H
#define	LINE_H

typedef enum stateTypeEnum {
    idle, forward, left, hardLeft, superHardLeft, right, hardRight, superHardRight, end, test
} stateType;

int onLine(float adcVal, int diode);
stateType calcNextState(float detectorArray[], float volt[]);


#endif	/* LINE_H */