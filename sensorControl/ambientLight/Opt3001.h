/*
 * Opt3001.h
 *
 *  Created on: May 9, 2019
 *      Author: sebas
 */

#ifndef OPT3001_H_
#define OPT3001_H_

void OPT3001Init(int ui32SysClkFreq);
void OPT3001Config(void);
void OPT3001ReadManufacturerID(char id[]);
char* OPT3001Value(void);
char* itoa(int val, char *a);
void I2CTimer(int ui32SysClkFreq);


#endif /* OPT3001_H_ */
