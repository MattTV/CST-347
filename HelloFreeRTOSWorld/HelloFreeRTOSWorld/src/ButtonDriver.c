/*************************************************************
* Author:			Matt Volpe
* Filename:			ButtonDriver.c
* Date Created:		3/8/23
* Modifications:
**************************************************************/

#include <asf.h>

#include "ButtonDriver.h"
#include "OITExpansionBoardDefines.h"

/**********************************************************************
* Purpose: Initialize the buttons
*
* Precondition:
*   None
*
* Postcondition:
*	LEDs 1-4 will be set as inputs
*
************************************************************************/
void initializeButtonDriver(void)
{
	ioport_set_pin_dir(EXT1_PUSH_BUTTON_1, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(EXT1_PUSH_BUTTON_1, EXT1_PUSH_BUTTON_1_FLAGS);
	ioport_set_pin_sense_mode(EXT1_PUSH_BUTTON_1, EXT1_PUSH_BUTTON_1_SENSE);
	
	ioport_set_pin_dir(EXT1_PUSH_BUTTON_2, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(EXT1_PUSH_BUTTON_2, EXT1_PUSH_BUTTON_2_FLAGS);
	ioport_set_pin_sense_mode(EXT1_PUSH_BUTTON_2, EXT1_PUSH_BUTTON_2_SENSE);
	
	ioport_set_pin_dir(EXT1_PUSH_BUTTON_3, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(EXT1_PUSH_BUTTON_3, EXT1_PUSH_BUTTON_3_FLAGS);
	ioport_set_pin_sense_mode(EXT1_PUSH_BUTTON_3, EXT1_PUSH_BUTTON_3_SENSE);
	
	ioport_set_pin_dir(EXT1_PUSH_BUTTON_4, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(EXT1_PUSH_BUTTON_4, EXT1_PUSH_BUTTON_4_FLAGS);
	ioport_set_pin_sense_mode(EXT1_PUSH_BUTTON_4, EXT1_PUSH_BUTTON_4_SENSE);
}

/**********************************************************************
* Purpose: Return the specified button's value
*
* Precondition:
*	uiBtnNum should be between 0 and 4
*
* Postcondition:
*	255 will be returned if uiBtnNum is out of bounds
*	The pin level of the requested button will be returned
*
************************************************************************/
uint8_t readButton(uint8_t uiBtnNum)
{
	uint8_t result = 255;
	
	switch (uiBtnNum)
	{
		case 1:
			result = ioport_get_pin_level(EXT1_PUSH_BUTTON_1);
			break;
		case 2:
			result = ioport_get_pin_level(EXT1_PUSH_BUTTON_2);
			break;
		case 3:
			result = ioport_get_pin_level(EXT1_PUSH_BUTTON_3);
			break;
		case 4:
			result = ioport_get_pin_level(EXT1_PUSH_BUTTON_4);
			break;
	}
	
	// The buttons are active low, so invert to make it easier for the consumer
	return !result;
}