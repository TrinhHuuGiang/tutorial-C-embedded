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
#ifndef _HANDLE_ARRAY_
#define _HANDLE_ARRAY_
#include <stdint.h>
#define MAX_KEY (4)
typedef uint8_t(*fpar)(uint8_t*, const int);

/*******************************************************************************
* API
******************************************************************************/
uint8_t add_val(uint8_t* arr, const int arl);/*them gia tri*/
uint8_t minus_val(uint8_t* arr, const int arl);/*xoa gia tri*/
uint8_t print_val(uint8_t* arr, const int arl);/*in gia tri*/
uint8_t exit_loop(uint8_t* arr, const int arl);/*thoat vong lap*/
#endif /*_HANDLE_ARRAY_*/

/*
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

