/*
    Copyright (C) <2024> <Giang Trinh>

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
*/

/*
 * do not modify Led GREEN, system tick when using this library
 * because this library is using them
 * */
/*******************************************************************************
* Definitions
******************************************************************************/
#ifndef CONFIG_UART0_H_
#define CONFIG_UART0_H_

/*******************************************************************************
* API
******************************************************************************/
/*
 * -------- Initialize led, UART0, system clock with:
 * + initialize led Green
 * + clock to UART
 * + clock to PORT
 *      (Not necessary because the LED Green is clocked on PortB)
 * then configuration:
 * + RX is input, TX is output
 * + OSR, SBR =>BAUD
 * set 4 interrupt (OR, NF, FE, RDRF, IDLE), not use PF, TDRE, TC
 * + set CTRL: ORIE, NEIE, FEIE, RIE
 * 			(ignore PFIE for parity
 * 			and TIE for TDRE, TCIE for TC)
 * + register interrupt for UART by IRQ of NVIC
 * then start UART
 * + start TX,RX => TE, RE on CTRL
 * last is set clock system tick , interrupt
 * + system tick clock 1s
 * */
void init_LGREEN_UART0();
void init_Systick();

/*
 * -------- String user want to send
 * in this assignment must be "Hello from MKE16Z!"
 * */
void trans_str_UART0(char* str);

/*
 * -------- FOR TRANSMITER
 * after 1 s, interrupt execute
 * check bit TC:
 * + if TC == 0     -> [ERROR 1] Print: time out because pooling
 *   transmitting   -> [ERROR 1] or other interrupts too long
 *                  -> [ERROR 1] string delay transmit
 * + if TC == 1     -> buffer empty
 *   transmit done  -> set a "flag start send string"
 *                  -> send "Hello from MKE16Z!"
 *                  -> clear flag when complete
 */
void send_str_UART0(); //send hello

/*
 * -------- FOR RECEIVER
 * interrupt OR: [ERROR 1] Print: data overrun
 * interrupt NF: [ERROR 2] Print: noise or COM error
 *                         because disconnect and not pull up or pull down
 * interrupt FE: [ERROR 3] Print: frame or
 *                         because stop bit error
 * interrupt RDRF: -> read data
 */
void _renew_buffer_UART0(); // default 50byte
void expected_str_UART0(char* str); // to compare with buffer
/*
 * compare value received with expected
 */
void compare_str_UART0();
#endif /* CONFIG_UART0_H_ */

/*
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */