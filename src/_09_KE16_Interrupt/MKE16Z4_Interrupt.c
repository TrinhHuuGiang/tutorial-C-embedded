/*
    <MKE16Z4_Interrupt.c>
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
#include "led_green_status.h"
#include <stdint.h>

#define SW3_PIN    (2U)	/* port D */

/*
 * State machine for green led
 */
typedef enum e_G_Led_state
{
	ON_LED = 0,     //led on
	BLINK_LED,      //led blink 5Hz
	BRIGHTEN_LED    //led gradually brightens
} G_Led_state;

/*******************************************************************************
* Variables
******************************************************************************/
/*
 * function pointer is selected by state machine
 */
fp_led_green G_Led_function[] =
{
	&on_Led_green,		//ON_LED
	&blink_Led_green,	//BLINK_LED
	&raising_Led_green	//BRIGHTEN_LED
};
static G_Led_state state_now = ON_LED; //initialize start is state 0
/*******************************************************************************
* Prototypes
******************************************************************************/
/*
 * initialize button 3 and interrupt
 */
void PORTBCD_DriverIRQHandler();
void InitSW();
void InitInterrupt();
/*******************************************************************************
* Code
******************************************************************************/
void PORTBCD_DriverIRQHandler()
{
	if(PORTD->ISFR & (1<<SW3_PIN))
	{
		//change state 0->1->2->0->...
		if(state_now == BRIGHTEN_LED)
		{
			state_now = ON_LED;
		}
		else state_now++;
		//clear the interrupt flag
		PORTD->ISFR |= (1<<SW3_PIN);
	}
}
void InitInterrupt()
{
	PORTD->PCR[SW3_PIN] |= (0xA<<16); //falling
	NVIC_EnableIRQ(PORTBCD_IRQn);
}
void InitSW()
{
	//enable clock for PORT
	PCC->CLKCFG[PCC_PORTD_INDEX] |= (1<<30);
	//set SW2 is GPIO
	PORTD->PCR[SW3_PIN] = 1<<8;	//set MUX is GPIO
	//enable pull-up
	PORTD->PCR[SW3_PIN] |= (1<<1);
	PORTD->PCR[SW3_PIN] |= (1<<0);
	//set GPIOD_3 as input
	GPIOD->PDDR &=~(1<<SW3_PIN);
}

int main()
{
	//LED
	init_Systick();
	init_Led_green();
	//Button
	InitSW();
	InitInterrupt();
	while(1)
	{
		G_Led_function[state_now]();
	}
	return 0;
}

/*
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */