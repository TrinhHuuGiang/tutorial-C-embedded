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
#include "config_UART0.h"

/*******************************************************************************
* Code
******************************************************************************/
int main()
{
	init_LGREEN_UART0();
	init_Systick();
	trans_str_UART0("Hello from MKE16Z!");
	_renew_buffer_UART0();
	expected_str_UART0("Hello from PC!");

	while(1)
	{
		send_str_UART0();
		compare_str_UART0();
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