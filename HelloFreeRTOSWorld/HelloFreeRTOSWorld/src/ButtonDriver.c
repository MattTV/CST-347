/*
 * ButtonDriver.c
 *
 * Created: 3/8/2023 4:06:05 PM
 *  Author: mattv
 */ 

#include <asf.h>

#include "ButtonDriver.h"
#include "OITExpansionBoardDefines.h"

void initializeButtonDriver(void)
{
	
}

uint8_t readButton(uint8_t uiBtnNum)
{
	uint8_t result = 255;
	
	switch (uiBtnNum)
	{
		case 1:
			result = ioport_get_pin_level(EXT1_PUSH_BUTTON_5);
			break;
		case 2:
			result = ioport_get_pin_level(EXT1_PUSH_BUTTON_6);
			break;
		case 3:
			result = ioport_get_pin_level(EXT1_PUSH_BUTTON_7);
			break;
		case 4:
			result = ioport_get_pin_level(EXT1_PUSH_BUTTON_8);
			break;
	}
	
	return result;
}