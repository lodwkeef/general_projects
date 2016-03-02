/*
 * File:   lcd.c
// Authors:     Alex Thompson
//              Ben Schifman
//              Justin Siekmann
//              Kevin Curtis
 * Created on February 25, 2016
 */

/* define statements for each pin used in the LCD
 */
#include <xc.h>
#include "lcd.h"
#include "timer.h"
#include <math.h>
#include <string.h>
#include <ctype.h>

/* pins that you will be used for the lcd control
 * LCD_RS      RC4
 * LCD_E       RC2
 * LCD_D4      RE7
 * LCD_D5      RE5
 * LCD_D6      RE3
 * LCD_D7      RE1
 * Use these so that we will be able to test your code for grading and to
 * help you debug your implementation!
 */

#define LCD_DATA    LATE      
#define LCD_RS      LATCbits.LATC4
#define LCD_E       LATCbits.LATC2

#define TRIS_D7     TRISEbits.TRISE1
#define TRIS_D6     TRISEbits.TRISE3
#define TRIS_D5     TRISEbits.TRISE5
#define TRIS_D4     TRISEbits.TRISE7
#define TRIS_RS     TRISCbits.TRISC4
#define TRIS_E      TRISCbits.TRISC2

#define OUTPUT 0
#define LOWER 1
#define UPPER 0

#define LCD_DELAY 50
#define LCD_DELAY_CLEAR 2000


/* This function should take in a 8-bit word and writes either the lower or upper
 * 4 bits to the last(appropriate) four bits of LATE. Additionally, according to the LCD data sheet
 * It should set LCD_RS and LCD_E with the appropriate values and delays.
 * After these commands are issued, there should be a delay.
 * The command type is a simplification. From the data sheet, the RS is '1'
 * when you are simply writing a character. Otherwise, RS is '0'.
 */
void writeFourBits(unsigned char word, unsigned int commandType, unsigned int delayAfter, unsigned int lower) {
    //    * LCD_RS      RC4
    //    * LCD_E       RC2
    //    * LCD_D4      RE7
    //    * LCD_D5      RE5
    //    * LCD_D6      RE3
    //    * LCD_D7      RE1
    LCD_E = 0;
    LCD_RS = commandType; //set the commandType (RS value)

    LATE = 0; //Reset LATE bits to 0 in order for bit masking functionality to work correctly
    if (lower == LOWER) {
        LCD_DATA |= (word & 0x08) >> 2; //RE1
        LCD_DATA |= (word & 0x04) << 1; //RE3
        LCD_DATA |= (word & 0x02) << 4; //RE5
        LCD_DATA |= (word & 0x01) << 7; //RE7       
    } else if (lower == UPPER) {
        LCD_DATA |= (word & 0x80) >> 6; //RE1
        LCD_DATA |= (word & 0x40) >> 3; //RE3
        LCD_DATA |= (word & 0x20); //RE5
        LCD_DATA |= (word & 0x10) << 3; //RE7
    }

    LCD_E = 1; //enable
    delayUs(delayAfter); //delay
    LCD_E = 0; //disable

    delayUs(delayAfter);
}

/* Using writeFourBits, this function should write the one byte of a character
 * to the LCD.
 */
void writeLCD(unsigned char word, unsigned int commandType, unsigned int delayAfter) {
    //TODO:
    writeFourBits(word, commandType, delayAfter, UPPER);
    writeFourBits(word, commandType, delayAfter, LOWER);
}

/* Given a character, write it to the LCD. RS should be set to the appropriate value.
 */
void printCharLCD(char c) {
    writeLCD(c, 1, LCD_DELAY);
}

/*Initialize the LCD
 */
void initLCD(void) {
    // Setup D, RS, and E to be outputs (0).
    ANSELEbits.ANSE7 = 0;
    ANSELEbits.ANSE5 = 0; //Digital
    
    TRIS_D7 = OUTPUT;
    TRIS_D6 = OUTPUT;
    TRIS_D5 = OUTPUT;
    TRIS_D4 = OUTPUT;
    TRIS_RS = OUTPUT;
    TRIS_E = OUTPUT;

    LCD_E = 0;

    // Initialization sequence utilizes specific LCD commands before the general configuration
    // commands can be utilized. The first few initialization commands cannot be done using the
    // WriteLCD function. Additionally, the specific sequence and timing is very important.

    // Enable 4-bit interface
    //wait 15+ ms after VDD reaches 4.5V
    delayUs(15000);
    writeFourBits(0b00000011, 0, LCD_DELAY, LOWER);
    delayUs(4500);
    writeFourBits(0b00000011, 0, LCD_DELAY, LOWER);
    delayUs(100);

    writeLCD(0b00110010, 0, LCD_DELAY);

    // Function Set (specifies data width, lines, and font.
    writeLCD(0b00101000, 0, LCD_DELAY);

    // 4-bit mode initialization is complete. We can now configure the various LCD
    // options to control how the LCD will function.

    // TODO: Display On/Off Control
    writeLCD(0b00001000, 0, LCD_DELAY); // Turn Display (D) Off
    writeLCD(0b00000001, 0, LCD_DELAY_CLEAR); // TODO: Clear Display (The delay is not specified in the data sheet at this point. You really need to have the clear display delay here.
    // TODO: Entry Mode Set
    writeLCD(0b00000110, 0, LCD_DELAY); // Set Increment Display, No Shift (i.e. cursor move)
    // TODO: Display On/Off Control
    writeLCD(0b00001110, 0, LCD_DELAY); // Turn Display (D) On, Cursor (C) Off, and Blink(B) Off
}

/*
 * You can use printCharLCD here. Note that every time you write a character
 * the cursor increments its position automatically.
 * Since a string is just a character array, try to be clever with your use of pointers.
 */
void printStringLCD(const char* s) {
    const char* tempChar = NULL;
    for (tempChar = s; *tempChar; tempChar++) {
        printCharLCD(*tempChar);
    }
}

/*
 * Clear the display.
 */
void clearLCD() {
    writeLCD(0b00000001, 0, LCD_DELAY_CLEAR);
}

/*
 Use the command for changing the DD RAM address to put the cursor somewhere.
 */
void moveCursorLCD(unsigned char row, unsigned char column) { //row and column start at 0
    unsigned char cAddress = '\0';
    switch (row) {
        case 0:
            cAddress = (0x80) | (column & 0x0F);
            break;
        case 1:
            cAddress = (0xC0) | (column & 0x0F);
            break;
    }
    writeLCD(cAddress, 0, LCD_DELAY);
}

/*
 * This function is called in lab1p2.c for testing purposes.
 * If everything is working properly, you should get this to look like the video on D2L
 * However, it is suggested that you test more than just this one function.
 */
void testLCD() {
    int i = 0;
    printCharLCD('c');
    for (i = 0; i < 1000; i++) delayUs(1000);
    clearLCD();
    printStringLCD("Hello!");
    moveCursorLCD(1, 2);
    for (i = 0; i < 1000; i++) delayUs(1000);
    printStringLCD("Hello!");
    for (i = 0; i < 1000; i++) delayUs(1000);
}