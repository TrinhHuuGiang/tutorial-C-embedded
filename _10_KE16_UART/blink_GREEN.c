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
