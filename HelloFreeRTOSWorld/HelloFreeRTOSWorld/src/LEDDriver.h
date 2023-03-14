/*
 * LEDDriver.h
 *
 * Created: 3/8/2023 3:57:26 PM
 *  Author: mattv
 */ 

#include <stdint.h>

#ifndef LEDDRIVER_H_
#define LEDDRIVER_H_

void initializeLEDDriver(void);
uint8_t readLED(uint8_t uiLedNum);
uint8_t setLED(uint8_t uiLedNum, uint8_t uiLedValue);
uint8_t toggleLED(uint8_t uiLedNum);

#endif /* LEDDRIVER_H_ */