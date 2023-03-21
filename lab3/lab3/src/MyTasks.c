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
#include <queue.h>
#include <string.h>

#include "MyTasks.h"
#include "uartdrv.h"
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
* Purpose: Toggle the LEDs and adjust frequency when a message is sent
*
* Precondition:
*   None
*
* Postcondition:
*	LED will be toggled on or off
*	Frequency wil lbe increased or decreased
*
************************************************************************/
void TaskLED(void* pvParameters)
{
	// Number of ms to delay
	uint16_t ms = 500;
	// Set default delay to 1/2s
	TickType_t delay = ms / portTICK_PERIOD_MS;
	// Extract the LED number
	uint8_t led = (uint8_t)pvParameters;
	// Action item
	LEDAction action;
	
	while (true)
	{
		// Only if a message is waiting
		while (uxQueueMessagesWaiting(qLeds[led - 1]))
		{
			// Read the message
			xQueueReceive(qLeds[led - 1], (void*)&action, 0);
			
			if (action == decrease)
			{
				if (ms > 200)
				{
					ms -= 50;
				}
			}			
			else if (action == increase)
			{
				if (ms < 1000)
				{
					ms += 50;
				}
			}
					
			delay = ms / portTICK_PERIOD_MS;
		}
		
		// Toggle the LED
		toggleLED(led);
		
		// Wait delay
		vTaskDelay(delay);
	}
}

/**********************************************************************
* Purpose: Poll for button presses
*
* Precondition:
*   None
*
* Postcondition:
*	If button 1 is pressed, decrease the delay
*	If button 2 is pressed, increase the delay
*	If button 3 is pressed, move control to the next LED (Suspend and Resume appropriate task)
*
************************************************************************/
void TaskMainControl(void* pvParameters)
{
	// Extract the LED number
	uint8_t led = (uint8_t)pvParameters;

	if (led == 1)
	{
		vTaskSuspend(mainTasks[1]);
		vTaskSuspend(mainTasks[2]);
	}	

	// Set delay to 100ms
	const TickType_t delay = 100 / portTICK_PERIOD_MS;

	LEDAction decreaseTime = decrease;
	LEDAction increaseTime = increase;

	
	
	while (true)
	{
		if (readButton(1))
		{
			// Send a message to decrease the time
			if (xQueueSendToBack(qLeds[led - 1], (void*)&decreaseTime, 0) != pdPASS)
			{
				
			}
		}
		
		if (readButton(2))
		{
			// Send a message to increase the time
			if (xQueueSendToBack(qLeds[led - 1], (void*)&increaseTime, 0) != pdPASS)
			{

			}
		}
		
		if (readButton(3))
		{
			// Resume the next LED (wrapping at 3) and suspend this task
			switch (led)
			{
				case 1:
					if (xQueueSendToBack(qUart, (void*)"LED 2 IS NOW ACTIVE\r\n", 0) != pdPASS)
					{

					}
					vTaskResume(mainTasks[1]);
					break;
				case 2:
					if (xQueueSendToBack(qUart, (void*)"LED 3 IS NOW ACTIVE\r\n", 0) != pdPASS)
					{

					}
					vTaskResume(mainTasks[2]);
					break;
				case 3:
					if (xQueueSendToBack(qUart, (void*)"LED 1 IS NOW ACTIVE\r\n", 0) != pdPASS)
					{

					}
					vTaskResume(mainTasks[0]);
					break;
			}
			
			vTaskSuspend(NULL);
		}
		
		vTaskDelay(delay);
	}
}

/**********************************************************************
* Purpose: Block until a message is waiting
*
* Precondition:
*   None
*
* Postcondition:
*	Strings will be pulled from the Queue to send over UART
*
************************************************************************/
void TaskUART(void* pvParameters)
{
	char buffer[50];
	
	while (true)
	{
		xQueueReceive(qUart, buffer, portMAX_DELAY);
		
		UARTPutStr(EDBG_UART, &buffer, strlen(buffer));
	}
}