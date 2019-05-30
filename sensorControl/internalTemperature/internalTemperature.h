/*
 * internalTemperature.h
 *
 *  Created on: May 28, 2019
 *      Author: sebas
 */

#ifndef SENSORCONTROL_INTERNALTEMPERATURE_INTERNALTEMPERATURE_H_
#define SENSORCONTROL_INTERNALTEMPERATURE_INTERNALTEMPERATURE_H_

void temperatureInit();
char* getTemperature();
void tempTimer(int ui32SysClkFreq);



#endif /* SENSORCONTROL_INTERNALTEMPERATURE_INTERNALTEMPERATURE_H_ */
