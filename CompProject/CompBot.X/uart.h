/* 
 * File:   uart.h
 * Author:
 *
 * Created on April 20, 2016, 12:16 AM
 */

#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif
    
void initUART();
void sendChar(char c);
void sendCommand(const char* sendString);


#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

