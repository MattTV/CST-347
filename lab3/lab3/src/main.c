/*************************************************************
* Author:			Matt Volpe
* Filename:			main.c
* Date Created:		1/10/23
* Modifications:
**************************************************************/

/*************************************************************
*
* Lab: Lab 3
*
* Overview:
* Using an expansion board, this program will cause three LEDs
*	to blink, pause, and resume depending on button presses.
*
* Input:
* BTN 1-3
*
* Output:
* LED 0 Heartbeat
* LED 1-3
*
************************************************************/

// System Includes
#include <asf.h>
#include <stdint.h>
#include <stdbool.h>

// FreeRTOS Includes
#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <queue.h>
#include <semphr.h>

// My Includes
#include "MyTasks.h"
#include "LEDDriver.h"
#include "ButtonDriver.h"
#include "uartdrv.h"

// Defines
#if(BOARD == SAM4E_XPLAINED_PRO)
	// Used to place heap
	#define mainHEAP_START				_estack
	#define mainRAM_LENGTH				0x00020000	// 128 KB internal SRAM
	#define mainRAM_START				0x20000000	// At 0x20000000
	#define mainEXTERNAL_SRAM_SIZE		{ 512ul * 1024ul }	// The SAM4E Xplained board has 2 banks of external SRAM, each 512 KB
#else
	#error Define memory regions here
#endif // SAM4E_EXPLAINED_PRO

// Function Prototypes
static void prvMiscInitialization(void);
static void prvInitializeHeap(void);
static void initQueues();

// Used to place heap
extern char _estack;

/**********************************************************************
* Purpose: Create the heartbeat and control tasks and start the scheduler
*
* Precondition:
*   None
*
* Postcondition:
*	None
*
************************************************************************/
int main(void)
{
	// Initialize the board
	prvMiscInitialization();
	
	// Add heartbeat to the scheduler
	xTaskCreate(
		TaskHeartbeat,			// Function called by task
		"Heartbeat",				// Task name
		configMINIMAL_STACK_SIZE,	// Task stack size
		NULL,						// Any parameters passed to task
		1,							// Task priority
		NULL						// Place to store task handle
	);
	
	xTaskCreate(
		TaskUART,
		"UART Task",
		configMINIMAL_STACK_SIZE,
		NULL,
		6,
		NULL
	);
	
	// Add the control task to the scheduler
	xTaskCreate(
		TaskMainControl,
		"TaskMainControl",
		configMINIMAL_STACK_SIZE,
		NULL,
		3,
		&mainTasks[0]
	);

	// Create corresponding LED task
	xTaskCreate(
		TaskLED,					// Function called by task
		"LED",						// Task name
		configMINIMAL_STACK_SIZE,	// Task stack size
		(void*)1,					// Any parameters passed to task
		6,							// Task priority
		NULL						// Place to store task handle
	);
	// Create corresponding LED task
	xTaskCreate(
		TaskModifiedLED,					// Function called by task
		"LED",						// Task name
		configMINIMAL_STACK_SIZE,	// Task stack size
		(void*)2,					// Any parameters passed to task
		5,							// Task priority
		NULL						// Place to store task handle
	);
	// Create corresponding LED task
	xTaskCreate(
		TaskLED,					// Function called by task
		"LED",						// Task name
		configMINIMAL_STACK_SIZE,	// Task stack size
		(void*)3,					// Any parameters passed to task
		4,							// Task priority
		NULL						// Place to store task handle
	);
	
	// Start the scheduler
	vTaskStartScheduler();

	while (true)
	{
		// Should never end up in this loop as the scheduler has taken over
	}
}	

/**********************************************************************
* Purpose: Initialize the EXT and SAM board, setting up clock, board, heap, and peripherals
*
* Precondition:
*   None
*
* Postcondition:
*	System clock initialized
*	Board initialized
*	Heap initialized
*	Peripheral clocks initialized
*
************************************************************************/
static void prvMiscInitialization(void)
{
	sysclk_init();
	board_init();
	prvInitializeHeap();
	
	pmc_enable_periph_clk(ID_PIOA);
	pmc_enable_periph_clk(ID_PIOB);
	
	initializeLEDDriver();
	initializeButtonDriver();
	initUART(EDBG_UART);
	initQueues();
}

/**********************************************************************
* Purpose: Initialize heap
*
* Precondition:
*   None
*
* Postcondition:
*	Heap initialized
*
************************************************************************/
static void prvInitializeHeap(void)
{
	uint32_t ulHeapSize;
	uint8_t *pucHeapStart;
	uint8_t *pucHeapEnd;
	
	pucHeapStart = (uint8_t*)( ( ( (uint32_t)&mainHEAP_START) + 7) & ~0x07ul);
	pucHeapEnd = (uint8_t*)(mainRAM_START + mainRAM_LENGTH);
	ulHeapSize = (uint32_t)( (uint32_t)pucHeapEnd - (uint32_t)&mainHEAP_START);
	ulHeapSize &= ~0x07ul;
	ulHeapSize -= 1024;
	
	HeapRegion_t xHeapRegions[] = {
		{ (unsigned char*)pucHeapStart, ulHeapSize },
		{ (unsigned char*)SRAM_BASE_ADDRESS, mainEXTERNAL_SRAM_SIZE },
		{ (unsigned char*)SRAM_BASE_ADDRESS_2ND, mainEXTERNAL_SRAM_SIZE },
		{ NULL, 0 }
	};
	vPortDefineHeapRegions(xHeapRegions);
}

/**********************************************************************
* Purpose: Trap if assert is called
*
* Precondition:
*   None
*
* Postcondition:
*	Infinite loop
*
************************************************************************/
void vAssertCalled(const char* pcFile, uint32_t ulLine)
{
	
	volatile uint32_t ulBlockVariable = 0UL;
	
	// Setting ulBlockVariable to a non-zero value in the debugger will allow this function to be exited
	taskDISABLE_INTERRUPTS();
	{
		while(ulBlockVariable == 0UL)
			__asm volatile("NOP");
	}
	taskENABLE_INTERRUPTS();
}

/**********************************************************************
* Purpose: Called if a malloc is attempted and there isn't enough free memory in the heap
*
* Precondition:
*   None
*
* Postcondition:
*	Infinite loop
*
************************************************************************/
void vApplicationMallocFailedHook(void)
{
	/* Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues, software
	timers, and semaphores.  The size of the FreeRTOS heap is set by the
	configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
	vAssertCalled( __FILE__, __LINE__ );
}

/**********************************************************************
* Purpose: Traps if the stack overflows
*
* Precondition:
*   None
*
* Postcondition:
*	Infinite loop
*
************************************************************************/
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char * pcTaskName)
{
	// Remove unused parameter warnings
	(void)pcTaskName;
	(void)pxTask;
	
	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	while(true);
}

/**********************************************************************
* Purpose: Initialize all Queues
*
* Precondition:
*   None
*
* Postcondition:
*	All given queues will be ready for use
*
************************************************************************/
static void initQueues()
{
	// LED Queues
	for (uint8_t i = 0; i < 3; i++)
		qLeds[i] = xQueueCreate(20, sizeof(LEDAction));
	
	// UART Queue
	qUart = xQueueCreate(20, 50);
}