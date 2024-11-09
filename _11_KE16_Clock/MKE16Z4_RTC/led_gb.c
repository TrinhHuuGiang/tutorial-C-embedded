/*******************************************************************************
* Definitions
******************************************************************************/
#include "MKE16Z4.h"
#include <stdint.h>
#include "led_gb.h"
#define LED_GREEN     (4U)
#define LED_BLUE      (1U)

/*******************************************************************************
* Variables
******************************************************************************/

/*******************************************************************************
* Prototypes
******************************************************************************/


/*******************************************************************************
* Code
******************************************************************************/
void on_led_GREEN()
{
	FGPIOB->PCOR |= (1<<LED_GREEN);
}
void on_led_BLUE()
{
	FGPIOD->PCOR |= (1<<LED_BLUE);
}
void off_led_GREEN()
{
	FGPIOB->PSOR |= (1<<LED_GREEN);
}
void off_led_BLUE()
{
	FGPIOD->PSOR |= (1<<LED_BLUE);
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
