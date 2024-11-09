/*
    <MKE16Z4_GPIO.c>:
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

#define MICRO_CYCLE (10000U)// 100Hz
#define LED_GREEN (4U)

/*******************************************************************************
* Variables
******************************************************************************/

/*******************************************************************************
* Prototypes
******************************************************************************/
void init_LED();
void delay_micro_second(uint16_t mic);
void delay_second(uint16_t second);
    /*Mode1
     *
     * 2s: increase brightness up to maximum
     * 2s: decrease brightness to 0%
     * total cycle time: 4s
     * blink led at frequency: 100Hz
     * => total blink: 400 time (200 rising, 200 falling)
     * */
void mode_Led_1();
    /*Mode2
     *
     * Toggle the LED each 1s
     *
     * */
void mode_Led_2();
/*******************************************************************************
* Code
******************************************************************************/
void mode_Led_2()
{
	/*toggle led*/
	FGPIOB->PTOR |= (1<<LED_GREEN);
	/*delay 1s*/
	delay_second(1);
}
void mode_Led_1()
{
	uint16_t temp;
	/*rising*/
	for(int i = 0; i < 200; i++)
	{
		/*f:100HZ == 10 000 us*/
		/*step 100%/200 = 0.5%*/
		temp = (MICRO_CYCLE*0.005*i);
		/*on led*/
		FGPIOB->PCOR |= (1<<LED_GREEN);
		delay_micro_second(temp);
		/*off led*/
		FGPIOB->PSOR |= (1<<LED_GREEN);
		delay_micro_second(MICRO_CYCLE-temp);
	}
	/*falling*/
	for(int i = 200; i > 0; i--)
	{
		temp = (MICRO_CYCLE*0.005*i);
		/*on led*/
		FGPIOB->PCOR |= (1<<LED_GREEN);
		delay_micro_second(temp);
		/*off led*/
		FGPIOB->PSOR |= (1<<LED_GREEN);
		delay_micro_second(MICRO_CYCLE-temp);
	}
}

void delay_second(uint16_t second)
{
	while(second)
	{
		second--;
		/*
		 * f = 48Mhz
		 * 1s == 48M cycle
		 * 1 'for' cycle == 4 -> 5 machine cycle
		 * => i == 10 000 000-> fix 5 000 000
		 * */
		for(uint32_t i = 0; i < 5000000; i++)
		{
			__asm("NOP");
		}
	}
}
void delay_micro_second(uint16_t mic)
{
	while(mic)
	{
		mic--;
		/*
		 * f = 48Mhz
		 * 1us == 48 cycle
		 * 1 'for' cycle == 4 -> 5 machine cycle
		 * => i == 10, fix = 2 for smooth
		 * */
		for(uint8_t i = 0; i < 2; i++)
		{
			__asm("NOP");
		}
	}
}
void init_LED()
{
	/*Enable clock PORTB*/
	PCC->CLKCFG[PCC_PORTB_INDEX] = (1<<30);
	/*select MUX is ALT1(GPIO) on PORTB_PCR4 (LED GREEN connected PIN 4)*/
	PORTB->PCR[LED_GREEN] = (1<<8);
	/*select GPIO direction output (FGPIO4_PDDR)*/
	FGPIOB->PDDR |= (1<<LED_GREEN);
	/*(Beginning) set output logic level 1 - off led (FGPIO4_PCOR)*/
	FGPIOB->PSOR |= (1<<LED_GREEN);
}

int main()
{
	init_LED();
	while(1)
	{
		/*8s = Mode 1 replay 2 time*/
		for(int i = 0; i < 2 ; i++)
		{
			mode_Led_1();
		}
		/*8s = Mode 2 replay 8 time*/
		for(int i = 0; i < 8 ; i++)
		{
			mode_Led_2();
		}
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