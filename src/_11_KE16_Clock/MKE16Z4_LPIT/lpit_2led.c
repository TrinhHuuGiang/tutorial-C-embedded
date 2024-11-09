/*******************************************************************************
* Definitions
******************************************************************************/
#include "MKE16Z4.h"
#include <stdint.h>
#include "lpit_2led.h"
#include "led_gb.h"


/*******************************************************************************
* Variables
******************************************************************************/
static uint32_t count_time1 = 0; // reset while = 20 <=> 1s
static uint32_t count_time235 = 0; // reset while = 47 <=> 2.35
/*******************************************************************************
* Prototypes
******************************************************************************/
static void init_SIRC_8M(); // init SIRC 8M
void LPIT0_IRQHandler();
/*******************************************************************************
* Code
******************************************************************************/
void LPIT0_IRQHandler()
{
	count_time1++;
	count_time235++;
	// if choose lpit0_ch0_second -> count_time1 =20 <=> 1s
	if(count_time1 == 20)
	{
		toggle_led_GREEN();
		count_time1 = 0;
	}
	// if choose lpit0_ch0_second -> count_time1 =20 <=> 2.35s
	if(count_time235 == 47)
	{
		toggle_led_BLUE();
		count_time235 = 0;
	}
	// Clear interrupt flag
	LPIT0->MSR |= (1<<0);
}
void reset_value_LPIT0_channel0(double lpit0_ch0_second)
{
	//disable LPIT0 channel_0
	LPIT0->SETTEN &=~ (1<<0);

	//set value overflow to timer
	LPIT0->CHANNEL[0].TVAL =  lpit0_ch0_second/ (1.0 / 8000000) - 1;


	//start LPIT0 channel_0
	LPIT0->SETTEN |= (1<<0);
}
void init_LPIT0_chanel0()
{
	//init led
	init_led_GREEN();
	init_led_BLUE();
	//init clock
	init_SIRC_8M();

	//enable clock gate, select SIRC is clk src
	PCC->CLKCFG[PCC_LPIT0_INDEX] = (1<<30) | (0b10<<24);
	//reset all LPIT register
	LPIT0->MCR |= (1<<1);
	//disable reset LPIT
	LPIT0->MCR &= ~(1<<1);

	//enable LPIT0
	LPIT0->MCR |= (1<<0);

	//allow interrupt LPIT0 channel_0
	LPIT0->MIER = (1<<0);
	//register interrupt handler
	NVIC_EnableIRQ(LPIT0_IRQn);
}
static void init_SIRC_8M()
{
	//turn off SIRC
	SCG->SIRCCSR &= ~(1<<0);
	//select frequency to 8mhz
	SCG->SIRCCFG |= (1<<0);
	//select div 1
	SCG->SIRCDIV |= (1<<8);
	//turn on SIRC again
	SCG->SIRCCSR |= (1<<0);
}
