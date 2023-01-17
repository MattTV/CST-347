/*************************************************************
* Author:			Matt Volpe
* Filename:			MyTasks.c
* Date Created:		1/10/23
* Modifications:
**************************************************************/

#include <asf.h>
#include <stdbool.h>

#include <FreeRTOS.h>
#include <task.h>

#include "MyTasks.h"

/**********************************************************************
* Purpose: Toggle the LED every 500 ms
*
* Precondition:
*   None
*
* Postcondition:
*	LED will be toggled on or off
*
************************************************************************/
void MyButtonTask(void* pvParameters)
{
	(void)pvParameters;
	
	while(true)
	{
		const TickType_t delay = 500 / portTICK_PERIOD_MS;	// Delay 500 ms

		ioport_set_pin_level(LED_0_PIN, LED_0_ACTIVE);		// Turn LED on

		vTaskDelay(delay);									// Wait 500 ms

		ioport_set_pin_level(LED_0_PIN, LED_0_INACTIVE);	// Turn LED off
		
		vTaskDelay(delay);									// Wait 500 ms
	}
}