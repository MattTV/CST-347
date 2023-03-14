/*************************************************************
* Author:			Matt Volpe
* Filename:			MyTasks.c
* Date Created:		1/10/23
* Modifications:	3/8/23	Removed MyButtonTask, added TaskHeartbeat and TaskSystemControl
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
*   pvParameters must e set to a valid LED number
*
* Postcondition:
*	LED will be toggled on or off
*
************************************************************************/
void TaskLED(void* pvParameters)
{
	// Set delay to 1s
	const TickType_t delay = 500 / portTICK_PERIOD_MS;
	
	while (true)
	{
		// Toggle the on-board LED
		toggleLED((uint8_t)pvParameters);
		
		// Wait delay
		vTaskDelay(delay);
	}
}

void TaskSystemControl(void* pvParameters)
{
	//EXT_SW1 first press start EXT_LED1, next press start EXT_LED2, third press start EXT_LED3, next press nothing
	// use xTaskCreate() for this and save the handles, giving parameter to LED one
	
	uint8_t currentLed = 0;
	static bool frozen = false;
	TaskHandle_t leds[] = { NULL, NULL, NULL };
	
	while (true)
	{
		if (readButton(1) == 1)
		{
			if (currentLed >= 0 && currentLed <= 2)
			{
				xTaskCreate(
					TaskLED,					// Function called by task
					"LED Control",				// Task name
					configMINIMAL_STACK_SIZE,	// Task stack size
					(void*)(currentLed + 1),	// Any parameters passed to task
					1,							// Task priority
					&leds[currentLed]			// Place to store task handle
				);
				
				++currentLed;
			}
		}
	
		if (readButton(2) == 1)
		{
			if (currentLed >= 1 && currentLed <= 3)
			{
				--currentLed;
				
				vTaskDelete(leds[currentLed]);
				
				leds[currentLed] = NULL;
				
				setLED(currentLed, 0);
			}
		}
	
		if (readButton(3) == 1)
		{
			if (frozen == true)
			{
				for (uint8_t i = 0; i < 3; i++)
				{
					if (leds[i] != NULL)
						vTaskResume(leds[i]);
				}
				
				frozen = false;
			}
			else if (frozen == false)
			{
				for (uint8_t i = 0; i < 3; i++)
				{
					if (leds[i] != NULL)
						vTaskSuspend(leds[i]);
				}
				
				frozen = true;
			}
		}
		
		const TickType_t delay = 100 / portTICK_PERIOD_MS;
			
		// Wait delay
		vTaskDelay(delay);
	}
	
	
	// Add system control to the scheduler
	xTaskCreate(
	TaskSystemControl,			// Function called by task
	"System Control",			// Task name
	configMINIMAL_STACK_SIZE,	// Task stack size
	NULL,						// Any parameters passed to task
	1,							// Task priority
	NULL						// Place to store task handle
	);
}