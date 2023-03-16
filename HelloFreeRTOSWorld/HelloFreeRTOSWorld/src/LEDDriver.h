/*************************************************************
* Author:			Matt Volpe
* Filename:			LEDDriver.h
* Date Created:		3/8/23
* Modifications:
**************************************************************/

#include <stdint.h>

#ifndef LEDDRIVER_H_
#define LEDDRIVER_H_

void initializeLEDDriver(void);
uint8_t readLED(uint8_t uiLedNum);
uint8_t setLED(uint8_t uiLedNum, uint8_t uiLedValue);
uint8_t toggleLED(uint8_t uiLedNum);

#endif /* LEDDRIVER_H_ */