/*******************************************************************************
* Definitions
******************************************************************************/
#include "MKE16Z4.h"
#include <stdint.h>
#include "rtc_2led.h"


/*******************************************************************************
* Variables
******************************************************************************/

/*******************************************************************************
* Prototypes
******************************************************************************/


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
