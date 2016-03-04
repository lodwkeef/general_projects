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
int checkPass(char guessString[], char passStringa[], char passStringb[], char passStringc[], char passStringd[]);
void updatePass(char tempPass[], char passStringa[], char passStringb[], char passStringc[], char passStringd[], int i);

//********QA Software Tests***********************************************************************************************
void test_clearString();
void test_checkPass();
void test_updatePass();

#endif	/* KEYPAD_H */

