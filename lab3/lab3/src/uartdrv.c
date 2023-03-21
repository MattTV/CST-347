/*************************************************************
* Author:			Troy Scevers & Jon Kelley
* Filename:			uartdrv.c
* Date Created:		3/15/23
* Modifications:
**************************************************************/

#include "uartdrv.h"

/**********************************************************************
* Purpose: Initialize the UART system
*
* Precondition:
*   None
*
* Postcondition:
*	UART will be ready to use
*
************************************************************************/
uint8_t initUART(Uart * p_Uart)
{
	uint32_t cd = 0;
	uint8_t retVal = 0;

	// configure UART pins
	ioport_set_port_mode(IOPORT_PIOA, PIO_PA9A_URXD0 | PIO_PA10A_UTXD0, IOPORT_MODE_MUX_A);
	ioport_disable_port(IOPORT_PIOA, PIO_PA9A_URXD0 | PIO_PA10A_UTXD0);
	sysclk_enable_peripheral_clock(ID_UART0);

	// Configure UART Control Registers
	// Reset and Disable RX and TX
	p_Uart->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;

	// Check and configure baudrate
	// Asynchronous, no oversampling
	cd = (sysclk_get_peripheral_hz() / UART_SERIAL_BAUDRATE) / UART_MCK_DIV;
	if(cd < UART_MCK_DIV_MIN_FACTOR || cd > UART_MCK_DIV_MAX_FACTOR)
	{
		retVal = 1;
	}
	
	if(retVal != 1)
	{
		// Set The Actual BAUD to Control Register
		p_Uart->UART_BRGR = cd;

		// Configure Mode
		p_Uart->UART_MR = UART_SERIAL_PARITY;
		
		// Disable PDC Channel
		p_Uart->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;
		
		// Enable RX and TX
		p_Uart->UART_CR = UART_CR_RXEN | UART_CR_TXEN;
	}
	
	return retVal;
}

/**********************************************************************
* Purpose: Send one character
*
* Precondition:
*   pUart should point to a valid Uart struct
*
* Postcondition:
*	The given character will be transmitted
*
************************************************************************/
void UARTPutC(Uart * pUart, char data)
{
	// Wait for Transmitter to be ready
	while((pUart->UART_SR & UART_SR_TXRDY) == 0);
	
	// Put the data in the register
	pUart->UART_THR = data;
}

/**********************************************************************
* Purpose: Send an entire string
*
* Precondition:
*   pUart should point to a valid Uart struct
*	data should point to a valid cstring
*	length should accurately reflect the length of the string
*
* Postcondition:
*	The given string will transmitted
*
************************************************************************/
void UARTPutStr(Uart * pUart, char * data, uint8_t len)
{
	for (uint8_t i = 0; i < len; i++)
		UARTPutC(pUart, data[i]);
}