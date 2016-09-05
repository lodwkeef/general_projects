/* 
 * File:   receivers.h
// Authors:     Alex Thompson
//              Ben Schifman
//              Justin Siekmann
//              Kevin Curtis
 *
 * Created on May 1, 2016
 */

#ifndef TRIANGULATION_H
#define	TRIANGULATION_H


float posXreturn(float steps1, float steps2, float steps3);
float posYreturn(float steps1, float steps2, float steps3);
float heading(float steps1, float steps2, float steps3);
void getWaypointHeading(float normalizedX, float normalizedY, float *relNormAngle);
void getLocAndHeading(float steps1, float steps2, float steps3, float dataArray[]);
int towerIdentification (float tower1, float tower2, float tower3, float unknownTower);
float angleToTarget (float target, float source);


#endif	/* TRIANGULATION_H */