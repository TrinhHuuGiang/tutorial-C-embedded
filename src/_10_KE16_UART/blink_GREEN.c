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
#include "MKE16Z4.h"
#include <stdint.h>
#include "blink_GREEN.h"

#define LED_GREEN (4U)

/*******************************************************************************
* Variables
******************************************************************************/

/*******************************************************************************
* Prototypes
******************************************************************************/

/*******************************************************************************
* Code
******************************************************************************/
void toggle_Led_green()
{
	FGPIOB->PTOR |= (1<<LED_GREEN);
}
void init_Led_green()
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