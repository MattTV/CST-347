/*
 * LEDDriver.c
 *
 * Created: 3/8/2023 3:56:50 PM
 *  Author: mattv
 */ 

#include <asf.h>

#include "LEDDriver.h"
#include "OITExpansionBoardDefines.h"

void initializeLEDDriver(void)
{
	
}

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

uint8_t setLED(uint8_t uiLedNum, uint8_t uiLedValue)
{
	if (uiLedValue == 0 || uiLedValue == 1)
	{
		switch (uiLedNum)
		{
			case 0:
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