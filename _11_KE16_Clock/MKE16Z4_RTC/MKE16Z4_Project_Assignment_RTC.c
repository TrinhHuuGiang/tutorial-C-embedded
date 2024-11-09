/*******************************************************************************
* Definitions
******************************************************************************/
#include "MKE16Z4.h"
#include <stdint.h>
#include "led_gb.h"
#include "rtc_2led.h"

/*******************************************************************************
* Variables
******************************************************************************/
unsigned char led_green_on = 1;
unsigned char led_green_off = 1;
unsigned char led_blue_on = 2;
unsigned char led_blue_off = 2;

/*
 * 4 value led must > 0
 * The initial status is 2 LEDs off together
 * Some test case: (1,2,1,2): (G+B) Turn on Together
 *                 (1,1,2,2): No -> G -> B -> G+B -> No
 *
 */


/*******************************************************************************
* Prototypes
******************************************************************************/

/*******************************************************************************
* Code
******************************************************************************/
int main()
{
	// check led status green and blue
	uint8_t l_g = 0,l_b = 0;

	// save time to count down
	uint8_t l_g0 = led_green_off;
	uint8_t l_g1 = led_green_on;
	uint8_t l_b0 = led_blue_off;
	uint8_t l_b1 = led_blue_on;

	// after 1 s, set flag
	uint8_t flag_1s = 0;

	//init led
	init_led_GREEN();
	init_led_BLUE();

	//init rtc
	init_RTC();
	reset_tsr_RTC(); //set time to 0s

	while (1)
	{
		//check after 1s
		flag_1s = 0;
		if(check_tsr_RTC()!=0)
		{
			reset_tsr_RTC();
			//set flag
			flag_1s = 1;
		}
		//if flag is set, count down --
		if(flag_1s)
		{
			if(l_g) l_g1--;
			else l_g0--;
			if(l_b) l_b1--;
			else l_b0--;
			//check count down then change status led green
			if(!l_g0)
			{
				//reset l_g0
				l_g0 = led_green_off;
				//on led green
				on_led_GREEN();
				//change status
				l_g = 1;
			}
			else if(!l_g1)
			{
				//reset l_g1
				l_g1 = led_green_on;
				//off led green
				off_led_GREEN();
				//change status
				l_g = 0;
			}
			//check count down then change status led blue
			if(!l_b0)
			{
				//reset l_b0
				l_b0 = led_blue_off;
				//on led blue
				on_led_BLUE();
				//change status
				l_b = 1;
			}
			else if(!l_b1)
			{
				//reset l_b1
				l_b1 = led_blue_on;
				//off led blue
				off_led_BLUE();
				//change status
				l_b = 0;
			}
		}
	}
	return 0;
}
