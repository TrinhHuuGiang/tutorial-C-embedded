/*
    <main.c>: <How to use function pointer, Callback Function | Function Pointer Handler>.
    Copyright (C) <2024> <Giang Trinh>

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
*/

/*******************************************************************************
* Definitions
******************************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include "handle_arr.h"
#define MAX_ARR (20)
/*******************************************************************************
* Variables
******************************************************************************/
fpar fp[] =
{
	&add_val,
	&minus_val,
	&print_val,
	&exit_loop
};

/*******************************************************************************
* Prototypes
******************************************************************************/
uint8_t DriverHandlerArray(uint8_t* arr,const int arl,fpar fpi);
/*******************************************************************************
* Code
******************************************************************************/
uint8_t DriverHandlerArray(uint8_t* arr,const int arl,fpar fpi)
{
	return (*fpi)(arr, arl);
}
int main(int argc, char* argv[])
{
	/*-----Array 20 bytes uint8_t assignment4[20] = {0xFF, 0xFF,...,0xFF}*/
	uint8_t assignment4[MAX_ARR];
	for (int i = 0; i < MAX_ARR; i++) assignment4[i] = 0xFF;
	/*-----Local var*/
	uint8_t* Ptr = assignment4;
	/*con tro ham doi so vi tri, gia tri*/
	int key;
	/*-----Nguoi dung nhap ( 1 - 4 )*/
	do
	{
		printf("\nNhap vao lua chon (1-4): ");
		scanf("%d", &key);
		while (key < 1 || key > MAX_KEY)
		{
			printf("\nNhap lai lua chon (1-4): ");
			scanf("%d", &key);
		}
	} while (DriverHandlerArray(Ptr,MAX_ARR,fp[key-1]));
	/*exit*/
	printf("\n\n<<<--Ket thuc chuong trinh-->>>\n\n");
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

