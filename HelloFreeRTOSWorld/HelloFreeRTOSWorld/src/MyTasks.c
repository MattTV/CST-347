/*************************************************************
* Author:			Matt Volpe
* Filename:			MyTasks.c
* Date Created:		1/10/23
* Modifications:	3/8/23	Removed MyButtonTask, added TaskHeartbeat, TaskLED, and TaskSystemControl
**************************************************************/

#include <asf.h>
#include <stdbool.h>

#include <FreeRTOS.h>
#include <task.h>

#include "MyTasks.h"
#include "LEDDriver.h"
#include "ButtonDriver.h"

/**********************************************************************
* Purpose: Toggle the LED every second
*
* Precondition:
*   None
*
* Postcondition:
*	LED will be toggled on or off
*
************************************************************************/
void TaskHeartbeat(void* pvParameters)
{
	// Disable warning for unused parameter
	(void)pvParameters;
	
	// Set delay to 1s
	const TickType_t delay = 1000 / portTICK_PERIOD_MS;
		
	while (true)
	{
		
		toggleLED(0);		// Toggle the on-board LED 
		
		vTaskDelay(delay);	// Wait delay
	}
}

/**********************************************************************
* Purpose: Toggle the LED every half second
*
* Precondition:
*   pvParameters must be set to a valid LED number
*
* Postcondition:
*	LED will be toggled on or off
*
************************************************************************/
void TaskLED(void* pvParameters)
{
	// Set delay to 1s
	const TickType_t delay = 500 / portTICK_PERIOD_MS;
	uint8_t led = (uint8_t)pvParameters;
	while (true)
	{
		// Toggle the on-board LED
		toggleLED(led);
		
		// Wait delay
		vTaskDelay(delay);
	}
}

/**********************************************************************
* Purpose: Poll the buttons and start, suspend, or delete TaskLED instances
*
* Precondition:
*   None
*
* Postcondition:
*	None
*
************************************************************************/
void TaskSystemControl(void* pvParameters)
{
	uint8_t currentLed = 0;
	static bool frozen = false;
	TaskHandle_t leds[] = { NULL, NULL, NULL };
	
	while (true)
	{
		// If the first button is pressed
		if (readButton(1) == 1)
		{
			// ...and there are up to 2 LEDs lit
			if (currentLed >= 0 && currentLed <= 2)
			{
				// Start blinking the next LED and save the handle
				xTaskCreate(
					TaskLED,					// Function called by task
					"LED Control",				// Task name
					configMINIMAL_STACK_SIZE,	// Task stack size
					(void*)(currentLed + 1),	// Any parameters passed to task
					1,							// Task priority
					&leds[currentLed]			// Place to store task handle
				);
				
				// Move the LED tracker forward
				++currentLed;
			}
		}
	
		// If the second button is pressed
		if (readButton(2) == 1)
		{
			// ...and there is at least 1 LED blinking
			if (currentLed >= 1 && currentLed <= 3)
			{
				
				// Move the LED tracker backwards
				--currentLed;
				
				// Stop that LED from blinking
				vTaskDelete(leds[currentLed]);
				
				// NULL out the handle
				leds[currentLed] = NULL;
				
				// Ensure the LED is off
				setLED(currentLed + 1, 0);
			}
		}
	
		// If the third button is pressed
		if (readButton(3) == 1)
		{
			// ...and the LEDs are currently frozen
			if (frozen == true)
			{
				// Resume each TaskLED
				for (uint8_t i = 0; i < 3; i++)
				{
					if (leds[i] != NULL)
						vTaskResume(leds[i]);
				}
				
				// Clear the flag for next time
				frozen = false;
			}
			// ...or if the LEDs are not frozen
			else if (frozen == false)
			{
				// Freeze each TaskLED
				for (uint8_t i = 0; i < 3; i++)
				{
					if (leds[i] != NULL)
						vTaskSuspend(leds[i]);
				}
				
				// Set the flag for next time
				frozen = true;
			}
		}
		
		const TickType_t delay = 100 / portTICK_PERIOD_MS;
			
		// Wait delay
		vTaskDelay(delay);
	}
}