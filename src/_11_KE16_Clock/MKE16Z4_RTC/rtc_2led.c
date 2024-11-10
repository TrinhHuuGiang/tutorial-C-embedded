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
#include "rtc_2led.h"

/*******************************************************************************
* Code
******************************************************************************/
/*
 * check value TSR
 */
uint32_t check_tsr_RTC()
{
	//read only tsr while rtc running
	return RTC->TSR;
}
/*
 * reset TSR to 0s
 */
void reset_tsr_RTC()
{
	// disable rtc
	RTC->SR &=~ (1<<4);

	// reset tsr
	RTC->TSR = 0;

	// enable rtc
	RTC->SR |= (1<<4);
}
void init_RTC()
{
	PCC->CLKCFG[PCC_RTC_INDEX] = (1<<30);

	// Reset the RTC by set RTC_CR[SWR] bit, and wait
	// for the TIF flag cleared by writing the TSR
	while (RTC->SR & RTC_SR_TIF_MASK)
	{
		RTC->CR |= RTC_CR_SWR_MASK;
		RTC->CR &= ~RTC_CR_SWR_MASK;
		RTC->TSR = 1;
	}

	// Setup the update mode and supervisor access mode
	// enable 1 kHz oscillator timer
	// disable 32 768khz
	RTC->CR = RTC_CR_CPE(0) | RTC_CR_LPOS(1) | RTC_CR_CPS(1) |
	RTC_CR_UM(0) | RTC_CR_SUP(0)| RTC_CR_OSCE(0);

	// disable all the interrupts first
	RTC->IER = 0;

	// stop timer first
	RTC->SR &= ~RTC_SR_TCE_MASK;
}

/*
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */