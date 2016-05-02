/* 
 * File:   i2c.h
 * Author: jsiek
 *
 * Created on May 1, 2016, 11:16 PM
 */

#ifndef I2C_H
#define	I2C_H

#ifdef	__cplusplus
extern "C" {
#endif

void initI2C2();
char i2cRead(char i2cAddress, char regAddress);
char i2cWrite(char i2cAddress, char reg_address, char data);
char* i2cMultiByteRead(char i2cAddress, char regAddress, int numAddress);

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

