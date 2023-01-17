/*************************************************************
* Author:			Matt Volpe
* Filename:			MyTasks.c
* Date Created:		1/10/23
* Modifications:
**************************************************************/

#include <asf.h>
#include <stdbool.h>

#include "MyTasks.h"

/**********************************************************************
* Purpose: If the button is pressed, light the LED. Else, turn it off.
*
* Precondition:
*   None
*
* Postcondition:
*	None
*
************************************************************************/
void MyButtonTask(void* pvParameters)
{
	while(true)
	{
		// Is the button pressed?
		if (ioport_get_pin_level(BUTTON_0_PIN) == BUTTON_0_ACTIVE)
		{
			ioport_set_pin_level(LED_0_PIN, LED_0_ACTIVE);
		}
		else
		{
			ioport_set_pin_level(LED_0_PIN, LED_0_INACTIVE);
		}
	}
}