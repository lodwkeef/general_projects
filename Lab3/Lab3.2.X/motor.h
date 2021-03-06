/* 
 * File:   motor.h
// Authors:     Alex Thompson
//              Ben Schifman
//              Justin Siekmann
//              Kevin Curtis
 *
 * Created on March 10, 2016
 */

#ifndef MOTOR_H
#define	MOTOR_H

void testM1forward();
void testM1reverse();
void testM2forward();
void testM2reverse();
void setMotorDirection(int motor, int direction);
void setMotorSpeed(float ADCbuffer, int direction);
void unmapPins();

#endif	/* MOTOR_H */

