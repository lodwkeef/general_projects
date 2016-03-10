/* 
 * File:   motor.c
// Authors:     Alex Thompson
//              Ben Schifman
//              Justin Siekmann
//              Kevin Curtis
 *
 * Created on March 10, 2016
 */
#include "motor.h"
#include "pwm.h"

void testM1(){
    int i = 0;
    for (i=0; i++, i<1023) setPWM1(i);
}

void testM2(){
    int i = 0;
    for (i=0; i++, i<1023) setPWM3(i);   
}
