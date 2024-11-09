/*******************************************************************************
* Definitions
******************************************************************************/
#include "MKE16Z4.h"
#include "led_green_status.h"
#include <stdint.h>

#define LED_GREEN           (4U)
#define SYSTICK_CLOCK_MASK  (1<<2) //core clock
#define SYSTICK_INTR_MASK   (1<<1) //interrupt on
#define SYSTICK_FREQ        (48000000.0) //48M
#define SYSTICK_CYCLE       (1.0/SYSTICK_FREQ)
#define SYSTICK_IN_INTERVAL (14400000.0) //==0.3s

/*******************************************************************************
* Variables
******************************************************************************/
static uint8_t systic_flag = 0; // 1: is counting, 0 is stopped
static uint32_t s_repeat_count = 0; // number repeat timer
static uint32_t s_remainder = 0; // time remainder
/*******************************************************************************
* Prototypes
******************************************************************************/
/*
 * set value for repeat count and remainder
 * bright_time is time led on
 */
static void set_static_Count(double bright_time);
/*
 * SysTick_Handler used to handle interrupts
 */
void SysTick_Handler(void);
/*
 * set system tick then turn on
 * type > 0 for s_repeat_count
 * type = 0 for s_remainder
 * */
static void set_Systick();

/*******************************************************************************
* Code
******************************************************************************/
void raising_Led_green()
{
	double percent = 0;
	while(percent < 1)
	{
		//on led
		set_static_Count(DUTY_CYCLE*percent);
		on_Led_green();
		set_Systic();
		while(systic_flag)
		{
			set_Systic();
		}
		//then off led
		set_static_Count(DUTY_CYCLE*(1.0-percent));
		off_Led_green();
		set_Systic();
		while(systic_flag)
		{
			set_Systic();
		}
		//increase percent
		percent+=DUTY_STEP;
	}
}
void blink_Led_green()
{
	//on led 0.1s
	set_static_Count(BLINK_CYCLE/2.0);
	on_Led_green();
	set_Systic();
	while(systic_flag)
	{
		set_Systic();
	}
	//then off led 0.1s
	set_static_Count(BLINK_CYCLE/2.0);
	off_Led_green();
	set_Systic();
	while(systic_flag)
	{
		set_Systic();
	}
}
static void set_Systick()
{
	uint32_t temp = 0;
	//check using s_repeat_count or s_remainder
	if(s_repeat_count)
	{
		temp = SYSTICK_IN_INTERVAL;
		s_repeat_count--;
	}
	else
	{
		temp = s_remainder;
		s_remainder = 0;
	}
	//set RELOAD value -> tick count
	SysTick->LOAD = temp;
	//reset value counting
	SysTick->VAL = 0;
	//enable system tick
	SysTick->CTRL |= (1<<0);
	//set flag
	systic_flag = 1;
}
void SysTick_Handler(void)
{
	if(s_repeat_count) s_repeat_count--; // -- to 0
	else if (s_remainder) s_remainder--; // -- to 0
	//set flag
	systic_flag = 0;
	//disable system tick
	SysTick->CTRL &=~ (1<<0);
}
static void set_static_Count(double bright_time)
{
	s_repeat_count = bright_time/(SYSTICK_IN_INTERVAL*SYSTICK_CYCLE);
	s_remainder = (bright_time - s_repeat_count*SYSTICK_IN_INTERVAL)/SYSTICK_CYCLE;
}
void off_Led_green()
{
	FGPIOB->PSOR |= (1<<LED_GREEN);
}
void on_Led_green()
{
	FGPIOB->PCOR |= (1<<LED_GREEN);
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
void init_Systick()
{
	//select clock
	SysTick->CTRL |= SYSTICK_CLOCK_MASK; // core clock 48mhz
	SysTick->CTRL |= SYSTICK_INTR_MASK; // interrupt turn on
}
