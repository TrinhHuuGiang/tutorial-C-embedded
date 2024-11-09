/*
    Copyright (C) <2024> <Giang Trinh>

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
*/

/*******************************************************************************
* Definitions
******************************************************************************/
#include <led_gb.h>
#include "MKE16Z4.h"
#include <stdint.h>
#define LED_GREEN     (4U)
#define LED_BLUE      (1U)

/*******************************************************************************
* Code
******************************************************************************/
void toggle_led_BLUE()
{
	FGPIOD->PTOR |= (1<<LED_BLUE);
}
void toggle_led_GREEN()
{
	FGPIOB->PTOR |= (1<<LED_GREEN);
}
void init_led_BLUE()
{
	/*Enable clock PORTD*/
	PCC->CLKCFG[PCC_PORTD_INDEX] = (1<<30);
	/*select MUX is ALT1(GPIO) on PORTD_PCR1 (LED BLUE connected PIN 1)*/
	PORTD->PCR[LED_BLUE] = (1<<8);
	/*select GPIO direction output (FGPIO1_PDDR)*/
	FGPIOD->PDDR |= (1<<LED_BLUE);
	/*(Beginning) set output logic level 1 - off led (FGPIO1_PSOR)*/
	FGPIOD->PSOR |= (1<<LED_BLUE);
}
void init_led_GREEN()
{
	/*Enable clock PORTB*/
	PCC->CLKCFG[PCC_PORTB_INDEX] = (1<<30);
	/*select MUX is ALT1(GPIO) on PORTB_PCR4 (LED GREEN connected PIN 4)*/
	PORTB->PCR[LED_GREEN] = (1<<8);
	/*select GPIO direction output (FGPIO4_PDDR)*/
	FGPIOB->PDDR |= (1<<LED_GREEN);
	/*(Beginning) set output logic level 1 - off led (FGPIO4_PSOR)*/
	FGPIOB->PSOR |= (1<<LED_GREEN);
}

/*
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */