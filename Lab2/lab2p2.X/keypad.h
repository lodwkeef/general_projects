/* 
 * File:   keypad.h
// Authors:     Alex Thompson
//              Ben Schifman
//              Justin Siekmann
//              Kevin Curtis
 *
 * Created on February 25, 2016
 */

#ifndef KEYPAD_H
#define	KEYPAD_H


void initKeypad(void);
void setODCLatch(int col);
char scanKeypad(int row);
void clearString(char string[]);
void updatePass(char tempPass[], char passStringa[], char passStringb[], char passStringc[], char passStringd[], int i);

#endif	/* KEYPAD_H */

