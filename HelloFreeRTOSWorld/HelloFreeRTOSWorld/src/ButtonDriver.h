/*************************************************************
* Author:			Matt Volpe
* Filename:			ButtonDriver.c
* Date Created:		3/8/23
* Modifications:
**************************************************************/

#include <stdint.h>

#ifndef BUTTONDRIVER_H_
#define BUTTONDRIVER_H_

void initializeButtonDriver(void);
uint8_t readButton(uint8_t uiLedNum);

#endif /* BUTTONDRIVER_H_ */