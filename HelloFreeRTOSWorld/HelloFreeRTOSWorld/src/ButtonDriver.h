/*
 * ButtonDriver.h
 *
 * Created: 3/8/2023 4:05:51 PM
 *  Author: mattv
 */ 

#include <stdint.h>

#ifndef BUTTONDRIVER_H_
#define BUTTONDRIVER_H_

void initializeButtonDriver(void);
uint8_t readButton(uint8_t uiLedNum);

#endif /* BUTTONDRIVER_H_ */