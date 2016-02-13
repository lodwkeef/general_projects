/*
 * File:   lcd.h
// Authors:     Alex Thompson
//              Ben Schifman
//              Justin Siekmann
//              Kevin Curtis
 *
 * Created on December 31, 2014, 1:39 PM
 */

void writeLCD(unsigned char word, unsigned int commandType, unsigned int delay);
void writeFourBits(unsigned char word, unsigned int commandType, unsigned int delayAfter, unsigned int lower);
void initLCD(void);
void clearLCD(void);
void moveCursorLCD(unsigned char row, unsigned char column);
void printCharLCD(char c);
void printStringLCD(const char* s);
void testLCD();
void testKevin();

#define LCD_DELAY 50
#define LCD_DELAY_CLEAR 1800