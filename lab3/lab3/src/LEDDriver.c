/*************************************************************
* Author:			Matt Volpe
* Filename:			LEDDriver.c
* Date Created:		3/8/23
* Modifications:	
**************************************************************/

#include <asf.h>

#include "LEDDriver.h"
#include "OITExpansionBoardDefines.h"

/**********************************************************************
* Purpose: Initialize the LEDs
*
* Precondition:
*   None
*
* Postcondition:
*	LEDs 1-4 will be set as outputs and off
*
************************************************************************/
void initializeLEDDriver(void)
{
	ioport_set_pin_dir(EXT1_LED1_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(EXT1_LED1_GPIO, EXT1_LED1_INACTIVE_LEVEL);
	
	ioport_set_pin_dir(EXT1_LED2_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(EXT1_LED2_GPIO, EXT1_LED2_INACTIVE_LEVEL);
	
	ioport_set_pin_dir(EXT1_LED3_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(EXT1_LED3_GPIO, EXT1_LED3_INACTIVE_LEVEL);
	
	ioport_set_pin_dir(EXT1_LED4_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(EXT1_LED4_GPIO, EXT1_LED4_INACTIVE_LEVEL);
}

/**********************************************************************
* Purpose: Return the current LED value
*
* Precondition:
*   uiLedNum should be between 0 and 4
*
* Postcondition:
*	The current LED pin value will be returned if a proper LED is requested
*	255 will be returned if uiLedNum is out of bounds
*
************************************************************************/
uint8_t readLED(uint8_t uiLedNum)
{
	uint8_t level = 255;
	
	switch (uiLedNum)
	{
		case 0:
			level = ioport_get_pin_level(LED_0_PIN);
			break;
		case 1:
			level = ioport_get_pin_level(EXT1_LED_1_PIN);
			break;
		case 2:
			level = ioport_get_pin_level(EXT1_LED_2_PIN);
			break;
		case 3:
			level = ioport_get_pin_level(EXT1_LED_3_PIN);
			break;
		case 4:
			level = ioport_get_pin_level(EXT1_LED_4_PIN);
			break;
	}
	
	return level;
}

/**********************************************************************
* Purpose: Set the given LED to the given value
*
* Precondition:
*   uiLedNum should be between 0 and 4
*	uiLedValue should be 0 or 1
*
* Postcondition:
*	LED will set on or off
*	The provided uiLedValue will be returned
*
************************************************************************/
uint8_t setLED(uint8_t uiLedNum, uint8_t uiLedValue)
{
	if (uiLedValue == 0 || uiLedValue == 1)
	{
		switch (uiLedNum)
		{
			case 0:
				// If uiLedValue is 0, set the value to inactive, if 1, set to inactive
				ioport_set_pin_level(LED_0_PIN, uiLedValue == 0 ? LED_0_INACTIVE : LED_0_ACTIVE);
				break;
			case 1:
				ioport_set_pin_level(EXT1_LED_1_PIN, uiLedValue == 0 ? EXT1_LED_1_INACTIVE : EXT1_LED_1_ACTIVE);
				break;
			case 2:
				ioport_set_pin_level(EXT1_LED_2_PIN, uiLedValue == 0 ? EXT1_LED_2_INACTIVE : EXT1_LED_2_ACTIVE);
				break;
			case 3:
				ioport_set_pin_level(EXT1_LED_3_PIN, uiLedValue == 0 ? EXT1_LED_3_INACTIVE : EXT1_LED_3_ACTIVE);
				break;
			case 4:
				ioport_set_pin_level(EXT1_LED_4_PIN, uiLedValue == 0 ? EXT1_LED_4_INACTIVE : EXT1_LED_4_ACTIVE);
				break;
		}
	}
	
	return uiLedValue;
}

/**********************************************************************
* Purpose: Toggle the LED given
*
* Precondition:
*   uiLedNum should be between 0 and 4
*
* Postcondition:
*	LED will be toggled on or off if a proper index is given
*	255 will be returned if an invalid index is given
*	0 will be returned if the task completed successfully
*
************************************************************************/
uint8_t toggleLED(uint8_t uiLedNum)
{
	uint8_t result = 0;
	
	switch (uiLedNum)
	{
		case 0:
			ioport_toggle_pin_level(LED_0_PIN);
			break;
		case 1:
			ioport_toggle_pin_level(EXT1_LED_1_PIN);
			break;
		case 2:
			ioport_toggle_pin_level(EXT1_LED_2_PIN);
			break;
		case 3:
			ioport_toggle_pin_level(EXT1_LED_3_PIN);
			break;
		case 4:
			ioport_toggle_pin_level(EXT1_LED_4_PIN);
			break;
		default:
			result = 255;
			break;
	}
	
	return result;
}