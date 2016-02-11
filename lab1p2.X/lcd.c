/*
 * File:   lcd.c
 * Authors:
 *
 * Created on December 31, 2014, 1:39 PM
 */

/* TODO: Make define statements for each pin used in the LCD
 */
#include <xc.h>
#include "lcd.h"
#include "timer.h"

/* The pins that you will use for the lcd control will be
 * LCD_RS      RC4
 * LCD_E       RC2
 * LCD_D4      RE7
 * LCD_D5      RE5
 * LCD_D6      RE3
 * LCD_D7      RE1
 * Use these so that we will be able to test your code for grading and to
 * help you debug your implementation!
 */

#define LCD_DATA  
#define LCD_RS  
#define LCD_E   

#define TRIS_D7 
#define TRIS_D6
#define TRIS_D5 
#define TRIS_D4 
#define TRIS_RS 
#define TRIS_E  

/* This function should take in a two-byte word and writes either the lower or upper
 * byte to the last four bits of LATE. Additionally, according to the LCD data sheet
 * It should set LCD_RS and LCD_E with the appropriate values and delays.
 * After these commands are issued, there should be a delay.
 * The command type is a simplification. From the data sheet, the RS is '1'
 * when you are simply writing a character. Otherwise, RS is '0'.
 */
void writeFourBits(unsigned char word, unsigned int commandType, unsigned int delayAfter, unsigned int lower){
    //TODO:
    // set the commandType (RS value)
    LATEbits.LATE0 = word&0x01;
    LATEbits.LATE2 = word&0x02;
    LATEbits.LATE4 = word&0x04;
    LATEbits.LATE6 = word&0x08;
    
    //enable
    //delay
    //disable
}

/* Using writeFourBits, this function should write the two bytes of a character
 * to the LCD.
 */
void writeLCD(unsigned char word, unsigned int commandType, unsigned int delayAfter){
    //TODO:
}

/* Given a character, write it to the LCD. RS should be set to the appropriate value.
 */
void printCharLCD(char c) {
    //TODO:
}
/*Initialize the LCD
 */
void initLCD(void) {
    // Setup D, RS, and E to be outputs (0).

    // Initilization sequence utilizes specific LCD commands before the general configuration
    // commands can be utilized. The first few initilition commands cannot be done using the
    // WriteLCD function. Additionally, the specific sequence and timing is very important.

    // Enable 4-bit interface

    // Function Set (specifies data width, lines, and font.

    // 4-bit mode initialization is complete. We can now configure the various LCD
    // options to control how the LCD will function.

    // TODO: Display On/Off Control
        // Turn Display (D) Off
    // TODO: Clear Display (The delay is not specified in the data sheet at this point. You really need to have the clear display delay here.
    // TODO: Entry Mode Set
        // Set Increment Display, No Shift (i.e. cursor move)
    // TODO: Display On/Off Control
        // Turn Display (D) On, Cursor (C) Off, and Blink(B) Off
}

/*
 * You can use printCharLCD here. Note that every time you write a character
 * the cursor increments its position automatically.
 * Since a string is just a character array, try to be clever with your use of pointers.
 */
void printStringLCD(const char* s) {
    //TODO:
}

/*
 * Clear the display.
 */
void clearLCD(){
}

/*
 Use the command for changing the DD RAM address to put the cursor somewhere.
 */
void moveCursorLCD(unsigned char x, unsigned char y){
}

/*
 * This function is called in lab1p2.c for testing purposes.
 * If everything is working properly, you should get this to look like the video on D2L
 * However, it is suggested that you test more than just this one function.
 */
void testLCD(){
    initLCD();
    int i = 0;
    printCharLCD('c');
    for(i = 0; i < 1000; i++) delayUs(1000);
    clearLCD();
    printStringLCD("Hello!");
    moveCursorLCD(1, 2);
    for(i = 0; i < 1000; i++) delayUs(1000);
    printStringLCD("Hello!");
    for(i = 0; i < 1000; i++) delayUs(1000);
}