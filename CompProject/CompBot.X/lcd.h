/*
 * File:   lcd.h
// Authors:     Alex Thompson
//              Ben Schifman
//              Justin Siekmann
//              Kevin Curtis
 *
 * Created on March 28, 2016
 */


#ifndef LCD_H
#define	LCD_H


void writeLCD(unsigned char word, unsigned int commandType, unsigned int delay);
void writeFourBits(unsigned char word, unsigned int commandType, unsigned int delayAfter, unsigned int lower);
void initLCD(void);
void clearLCD(void);
void moveCursorLCD(unsigned char row, unsigned char column);
void printCharLCD(char c);
void printStringLCD(const char* s);
void testLCD();

#endif	/* LCD_H */