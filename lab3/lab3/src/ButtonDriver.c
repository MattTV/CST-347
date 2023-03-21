/*************************************************************
* Author:			Matt Volpe
* Filename:			ButtonDriver.c
* Date Created:		3/8/23
* Modifications:
**************************************************************/

#include <asf.h>
#include <FreeRTOS.h>
#include <task.h>

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
*	The pin level of the requested button will be returned
*
************************************************************************/
uint8_t readButton(uint8_t uiBtnNum)
{
	uint8_t i = 0;
	bool letgo = false;
	uint8_t result = 255;
	uint8_t results[] = { 255, 254 };
	ioport_pin_t btn;
	// Set delay to 10ms
	TickType_t delay = 10 / portTICK_PERIOD_MS;

	// Select the btn
	switch (uiBtnNum)
	{
		case 1:
			btn = EXT1_PUSH_BUTTON_1;
			break;
		case 2:
			btn = EXT1_PUSH_BUTTON_2;
			break;
		case 3:
			btn = EXT1_PUSH_BUTTON_3;
			break;
		case 4:
			btn = EXT1_PUSH_BUTTON_4;
			break;
	}
	
	for (i = 0; i < 2; i++)
	{
		results[i] = ioport_get_pin_level(btn);
		
		vTaskDelay(delay);
	}

	// Capture the result
	// The buttons are active low, so invert to make it easier for the consumer
	// Only return 1 if the both results are the same and equal to 0
	result = ((results[0] == results[1]) && (results[0] == 0)) ? 1 : 0;

	// Wait until the button has returned to its normal state
	if (result)
	{
		// Block until the button state is permanently off
		while (!letgo)
		{
			// Repeat the debounce process, looking for the opposite result
			for (i = 0; i < 2; i++)
			{
				results[i] = ioport_get_pin_level(btn);
		
				vTaskDelay(delay);
			}

			if ((results[0] == results[1]) && (results[0] == 1))
			{
				letgo = true;
			}
		}
	}
	
	return result;
}