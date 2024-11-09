/*******************************************************************************
* Definitions
******************************************************************************/
#include "MKE16Z4.h"
#include <stdint.h>
#include "blink_GREEN.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "config_UART0.h"

#define SYSTICK_CLOCK_MASK  (1<<2) //core clock
#define SYSTICK_INTR_MASK   (1<<1) //interrupt on
#define BAUD_UART0     (9600)
#define RX_PIN         (0U)
#define TX_PIN         (1U)
//CTRL REG
#define CTRL_ORIE   (27) //[ERROR] Overrun Interrupt Enable
#define CTRL_NEIE   (26) //[ERROR] Noise Error Interrupt Enable
#define CTRL_FEIE   (25) //[ERROR] Framing Error Interrupt Enable
#define CTRL_ILIE   (20) //IDLE Interrupt Enable, after 1 char time
#define CTRL_RIE    (21) //Receiver Interrupt Enable
//STAT REG
#define STAT_TDRE   (23) //Transmit Data Register Empty Flag
#define STAT_TC     (22) //Transmission Complete Flag
#define STAT_OR     (19) //[ERROR] Receiver Overrun Flag
#define STAT_NF     (18) //[ERROR] Noise Flag
#define STAT_FE     (17) //[ERROR] Framing Error
#define STAT_IDLE   (20) //IDLE Line Flag
#define STAT_RDRF   (21) //Receive Data Register Full Flag

#define BUFFER_LEN  (256) // 256byte for buffer

/*******************************************************************************
* Variables
******************************************************************************/
static char* s_str_Hello = NULL;
static char* s_expected_str = NULL;
static char* buffer_received = NULL;
static uint8_t buffer_len = 0;
static uint8_t s_systic_count = 0; // 1 count = 16M <=> 1/3 second
static uint8_t s_send_flag = 0; // 0 is send, 1 is prepare
static uint8_t s_compare = 0;// 0 is not compare, 1 is prepare
/*******************************************************************************
* Prototypes
******************************************************************************/
/*
 * SysTick_Handler used to handle interrupts after 1s
 */
void SysTick_Handler(void);
/*
 * system timer 1 s, with core frequency = 48MHZ
 */
static void _1_s_Systic();
/*
 * Handler for interrupt: OR,NF,FE,RDRF
 */
void LPUART0_IRQHandler();
/*******************************************************************************
* Code
******************************************************************************/
void compare_str_UART0()
{
	if(s_compare)
	{
		//compare = 0 if 2 string equal
		if(!strcmp(buffer_received, s_expected_str))
		{
			toggle_Led_green();
		}
		//else equal length but not equal value
		_renew_buffer_UART0();

		//compare complete
		s_compare = 0;
	}
}
void _renew_buffer_UART0()
{
	if(buffer_received!=NULL)
	{
		free(buffer_received);
		buffer_received = NULL;
	}
	while(buffer_received==NULL)
	{
		//try allocate buffer
		buffer_received = (char*)malloc(BUFFER_LEN*sizeof(char));
	}
	//reset buffer length
	buffer_len = 0;
	//set value \0 for string compare
	buffer_received[0] = '\0';
}
void expected_str_UART0(char* str)
{
	//input not null
	if(str == NULL)
	{
		return;
	}
	//free old value
	if(s_expected_str != NULL)
	{
		free(s_expected_str);
		s_expected_str = NULL;
	}
	//allocate new memory
	while(s_expected_str == NULL)
	{
		s_expected_str = (char*)malloc((strlen(str)+1)*sizeof(char));
	}
	//else save value
	strcpy(s_expected_str, str);
}
void LPUART0_IRQHandler()
{
	//get value for buffer
	if(LPUART0->STAT & (1<<STAT_RDRF))
	{
		//concatenate
		buffer_received[buffer_len] = (char)(LPUART0->DATA);
		buffer_len++;
		//add null
		buffer_received[buffer_len] = '\0';
	}
	//compare value when idle
	else if(LPUART0->STAT & (1<<STAT_IDLE))
	{
		s_compare = 1; //start compare
		LPUART0->STAT |= (1<<STAT_IDLE);//w1c
	}
	//if error -> renew buffer
	else if(LPUART0->STAT & (1<<STAT_OR))
	{
		_renew_buffer_UART0();
		printf("[ERROR_RECEIVE] - Data overrun");
		LPUART0->STAT |= (1<<STAT_OR);//w1c
	}
	else if(LPUART0->STAT & (1<<STAT_NF))
	{
		_renew_buffer_UART0();
		printf("[ERROR_RECEIVE] - COM port errors");
		LPUART0->STAT |= (1<<STAT_NF);//w1c
	}
	else if(LPUART0->STAT & (1<<STAT_FE))
	{
		_renew_buffer_UART0();
		printf("[ERROR_RECEIVE] - UART configuration errors");
		LPUART0->STAT |= (1<<STAT_FE);//w1c
	}
}
void send_str_UART0()
{
	char* str = s_str_Hello; // using another pointer
	if(s_send_flag)
	{
		//reset flag
		s_send_flag = 0;
		//check null
		if(str == NULL)
		{
			return;
		}
		//else send string
		while(*str != '\0')
		{
			while(!(LPUART0->STAT & (1<<STAT_TDRE)))
			{
				// while until the buffer is free
			}
			LPUART0->DATA = *str;

			while(LPUART0->STAT & (1<<STAT_TC))
			{
				// while until the transmit done
			}
			str++;
		}
	}
	//else do nothing - wait until 1s
}

void trans_str_UART0(char* str)
{
	//input not null
	if(str == NULL)
	{
		return;
	}
	//free old value
	if(s_str_Hello != NULL)
	{
		free(s_str_Hello);
		s_str_Hello = NULL;
	}
	//allocate new memory
	while(s_str_Hello == NULL)
	{
		s_str_Hello = (char*)malloc((strlen(str)+1)*sizeof(char));
	}
	//then save value
	strcpy(s_str_Hello, str);
}
void SysTick_Handler(void)
{
	//if complete 1s
	if(!s_systic_count)
	{
		//check TC
		if(LPUART0->STAT & (1<<STAT_TC)) // TC == 1
		{
			s_send_flag = 1;// call let send string
		}
		else // tx busy
		{
			printf("[ERROR_TRANS] - time out error");
		}
		//restart 1s
		_1_s_Systic();
		//not disable system tick
		//not use end loop SysTick->CTRL &=~ (1<<0);
	}
	else
	{
		s_systic_count--;
	}
}
static void _1_s_Systic()
{
	// count = 3
	// 3 * 16M = 48M -> 1s
	//set flag second
	s_systic_count = 3;
	//set RELOAD value -> tick count
	SysTick->LOAD = 16000000-1;
	//reset value counting
	SysTick->VAL = 0;
	//enable system tick
	SysTick->CTRL |= (1<<0);
}
void init_Systick()
{
	//select clock
	SysTick->CTRL |= SYSTICK_CLOCK_MASK; // core clock 48mhz
	SysTick->CTRL |= SYSTICK_INTR_MASK; // interrupt turn on
	//start timer
	_1_s_Systic();
}
void init_LGREEN_UART0()
{
	//set baud rate
	uint8_t OSR = 15;
	uint16_t SBR = 0;

	init_Led_green();

	/*using RISC for UART*/
	//turn off SIRC
	SCG->SIRCCSR &= ~(1<<0);
	//select frequency to 8mhz
	SCG->SIRCCFG |= (1<<0);
	//select divide 1
	SCG->SIRCDIV |= (1<<8);
	//turn on SIRC again
	SCG->SIRCCSR = (1<<0);

	//enable clock for UART module
	//select clock source as the FIRC before set enable clock
	PCC->CLKCFG[PCC_LPUART0_INDEX] = (0x2<<24);
	PCC->CLKCFG[PCC_LPUART0_INDEX] |= (1<<30);
	//PCC->CLKCFG[PCC_PORTB_INDEX] |= (1<<30);

	/*configuration UART0*/
	//computing SBR. baud = 9600
	SBR = 8000000 / (double)BAUD_UART0 / (double)(OSR+1);

	//select interrupt UART
	LPUART0->CTRL |=
			(1<<CTRL_ORIE)|
			(1<<CTRL_NEIE)|
			(1<<CTRL_FEIE)|
			(1<<CTRL_RIE)|
			(1<<CTRL_ILIE);
	//enable interrupt NVIC for UART
	NVIC_EnableIRQ(LPUART0_IRQn);


	//configure Tx/ Rx as the pin out (atl2)
	PORTB->PCR[RX_PIN] = 0x02 << 8;
	PORTB->PCR[TX_PIN] = 0x02 << 8;

	//numBit = 8, stopBit = 1, parity = none
	LPUART0->BAUD = (OSR << 24) | (SBR<<0);

	//enable Tx/Rx
	LPUART0->CTRL |= (1<<19) | (1<<18);
}
