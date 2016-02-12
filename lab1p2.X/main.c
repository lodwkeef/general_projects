// ******************************************************************************************* //
//
// File:         lab1p2.c
// Date:         12-30-2014
// Authors:      Garrett Vanhoy
//
// Description: 
// ******************************************************************************************* //

#include <xc.h>
#include <sys/attribs.h>
#include "lcd.h"
#include "timer.h"
#include "config.h"
#include "interrupt.h"


// ******************************************************************************************* //

//hey look how fast this commits Justin

int main(void)
{
    SYSTEMConfigPerformance(10000000);
    initTimer2();
    while(1)
    {
        testTimer2();
        //testLCD();
    }
    
    return 0;
}