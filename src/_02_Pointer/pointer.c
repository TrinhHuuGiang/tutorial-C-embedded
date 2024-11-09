/*
    <pointer.c>: <Allocate 10 bytes heap for a pointer>.
    Copyright (C) <2024> <Giang Trinh>

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
*/

/*******************************************************************************
* Definitions
******************************************************************************/
#include <stdint.h>/*uint8_t*/
#include <stdbool.h>/*bool*/
#include <stdlib.h>/*malloc*/
#include <stdio.h>/*printf*/

/*******************************************************************************
* Prototypes
******************************************************************************/
bool allocate10Bytes(uint8_t *outPtr);/*dynamically allocate 10 bytes*/
static int check_Mem(uint8_t* outPtr);/*test case check 10 byte*/

/*******************************************************************************
* Code
******************************************************************************/
int main(int argc, char* argv[])
{
	uint8_t * Ptr = NULL;
	printf("initialize pointer (NULL): [%p]\n", Ptr);
	/*Allocate*/
	/*
	Giải thích:
	- con trỏ khai báo vùng nhớ động 10 byte là Ptr
	- truyền địa chỉ của con trỏ (&Ptr) vào hàm allocate10Bytes 
	để thay đổi địa chỉ con trỏ đến mảng động được cấp phát
	Lý do bởi vì khi truyền con trỏ cho hàm chỉ có giá trị của ô nhớ
	được quản lý bởi Ptr là bị thay đổi còn địa chỉ Ptr trỏ đến không đổi.
	- ép kiểu sang con trỏ để không bị lỗi biên dịch bằng gcc
	*/
	if (!allocate10Bytes((uint8_t*)& Ptr))
		printf("Error allocate\n");
	else
		printf("Success allocate\n");
	/*
	Test_case:
	- Nếu khởi tạo lỗi thì in ra địa chỉ con trỏ == NULL
	- Nếu khởi tạo thành công thì:
	+ in ra địa chỉ con trỏ
	+ in ra mảng {0} được calloc
	+ thay đổi từng phần tử thành i*20+20, i=0,..,9;
	+ in ra mảng giá trị sau đó
	*/
	if (check_Mem(Ptr))
		printf("pointer set NULL\n");
	else free(Ptr);
	/*end*/
	return 0;
}
bool allocate10Bytes(uint8_t *outPtr)
{
	/*tạo 1 con trỏ cấp 2 để quản lý địa chỉ của con trỏ cần cấp phát động*/
	uint8_t** _outPtr = outPtr;
	/*
	return true if success, false if not
	sử dụng calloc để thay đổi đối tượng mảng động thành 0
	*/
	*_outPtr = (uint8_t*)calloc(10,sizeof(uint8_t));/*cấp phát 10 byte unsigned char*/
	printf("calloc at: [%p]\n", *outPtr);/*in ra địa chỉ con trỏ đang trỏ tới*/
	printf("calloc at: [%p]\n", *_outPtr);/*in ra địa chỉ con trỏ đang trỏ tới*/
	if (*outPtr == NULL) return false;
	return true;
}
static int check_Mem(uint8_t* outPtr)
{
	int i;
	printf("\nTest case\n");
	/*-----Nếu khởi tạo lỗi từ trước thì trả về -1*/
	if (outPtr == NULL)
	{
		printf("Pointer value: [%p]\n", outPtr);/*In tra giá trị trỏ đến NULL*/
		return -1;
	}
	/*-----Nếu khởi tạo thành công thì hiển thị*/
	/*startbyte (outPtr),stopbyte*/
	printf("start memory (pointer): [%p]\n",&outPtr[0]);
	printf("end memory            : [%p]\n",&outPtr[9]);
	/*hien thi gia tri ban dau cua 10byte*/
	printf("10 byte value (before):\n    ");
	for (i = 0; i < 10; i++)
	{
		printf(" [%u]", outPtr[i]);
	}
	printf("\n");
	/*cho chạy vòng lặp khởi tạo giá trị cho mỗi byte tăng lên 20*/
	for (i = 0; i < 10; i++)
	{
		outPtr[i] = (uint8_t)(i*20 + 20); /*value 20 to 200*/
	}
	/*Hiển thị*/
	printf("10 byte value  (after):\n    ");
	for (i = 0; i < 10; i++)
	{
		printf(" [%u]", outPtr[i]);
	}
	printf("\n");
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
