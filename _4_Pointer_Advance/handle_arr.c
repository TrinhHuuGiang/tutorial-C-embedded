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
#include "handle_arr.h"
#include "stdlib.h"
static struct ArrPointer
{
	uint8_t val;
	struct ArrPointer* next;
};
typedef struct ArrPointer List;

/*******************************************************************************
* Variables
******************************************************************************/
static List* First;/*Khong malloc o global*/

/*******************************************************************************
* Code
******************************************************************************/
uint8_t add_val(uint8_t* arr, const int arl)
{
	int loc, val;
	uint8_t _val;
	/*- Nhap vao vi tri ? Nhap vao gia tri ?*/
	printf("Nhap vao vi tri [0; %d] va gia tri [0; 100] : ", arl);
	scanf("%d %d", &loc, &val);
	/*- Kiem tra vi tri co ngoai 0 - 19 khong*/
	if (loc < 0 || loc >= arl)
	{
		printf("[ERROR]");
		printf("->Vi tri [%d] nam ngoai [0; %d]\n", loc, arl);
	}
	/*(Co the bo qua) Kiem tra co gia tri luu chua*/
	else 
		if (arr[loc] != 0xFF)
		{
			printf("[ERROR]");
			printf("->Vi tri [%d] da ton tai gia tri\n", loc);
		}
	/*- Kiem tra gia tri co ngoai 0- 100 khong*/
		else
		{
			_val = (uint8_t)val;
			if(val < 0 || val > 100)
			{
				printf("[ERROR]");
				printf("->Gia tri [%d] nam ngoai [0,100]\n", val);
			}
	/*(Co the bo qua) Kiem tra co gia tri do trong mang khong*/
			else
			{
				uint8_t temp = 0;
				for (int i = 0; i < arl; i++)
				{
					if (_val == arr[i])
					{
						printf("[ERROR]");
						printf("->Gia tri [%u] da ton tai o vi tri [%d]\n", _val, i);
						temp++;
					}
				}
				if (temp != 0)
					return 1;
	/*(Co the bo qua) Them gia tri vao*/
				else
				{
					arr[loc] = _val;
					printf("[SUCCESS]");
					printf("->Ghi thanh cong [%u] vao vi tri [%d]\n", _val, loc);
				}
			}
		}

	/*return 1 loop*/
	return 1;
}
uint8_t minus_val(uint8_t* arr, const int arl)
{
	int val;
	uint8_t _val;
	/*- Nhap vao gia tri muon xoa?*/
	printf("Nhap vao gia tri can xoa [0; 100] : ", arl);
	scanf("%d", &val);
	_val = (uint8_t)val;
	/*- Kiem tra xem gia tri co nam trong 0 - 100*/
	if (val < 0 || val>100)
	{
		printf("[ERROR]");
		printf("->Gia tri [%d] nam ngoai [0,100]\n", val);
	}
	/*- Kiem tra gia tri co ton tai thi xoa no*/
	else
	{
		uint8_t temp = 0;
		for (int i = 0; i < arl; i++)
		{
			if (_val == arr[i])
			{
				arr[i] = 0xFF;
				printf("[SUCCESS]->Xoa thanh cong gia tri [%u]\n",_val);
				temp++;
			}
		}
	/*+ Neu khong co gia tri do thi canh bao*/
		if (temp == 0)
		{
			printf("[ERROR]->Khong ton tai gia tri [%u]\n", _val);
		}
	}

	/*return 1 loop*/
	return 1;
}
uint8_t print_val(uint8_t* arr, const int arl)
{
	First = (List*)malloc(sizeof(List));
	List* Temp;
	List* Tempback;
	First->val = arr[0];
	First->next = NULL;
	/*-----Before sorting*/
	printf("\nBefore sorting:");
	for (uint8_t i = 0;i<arl;i++)
	{
		if(i%10==0) printf("\n");
		printf("[%u] ", arr[i]);
	}
	/*-----Sorting*/
	/*so sanh arr[i] voi cac phan tu trong danh sach*/
	/*noi them vao vi tri hop ly*/
	for (uint8_t i = 1; i < arl; i++)
	{
		List* t = (List*)malloc(sizeof(List));
		uint8_t check = 1;
		Temp = First;
		Tempback = First;
		/*Neu phan tu dau > arr[i]*/
		if (Temp->val >= arr[i])
		{
			t->val = arr[i];
			t->next = First;
			First = t;
		}
		/*neu la phan tu sau*/
		else
		{
			Temp = Temp->next;/*dich vi tri con tro gia tri len 1*/
			while (check)
			{
				if (Temp->next != NULL)
				{
					if (Temp->val >= arr[i])
					{
						t->val = arr[i];
						t->next = Temp;
						Tempback->next = t;
						/*continue*/
						check = 0;
					}
					Temp = Temp->next;
					Tempback = Tempback->next;
				}
				else
				{
					if (Temp->val >= arr[i])
					{
						t->val = arr[i];
						t->next = Temp;
						Tempback->next = t;
					}
					else
					{
						t->val = arr[i];
						t->next = NULL;
						Temp->next = t;
					}
					/*continue*/
					check = 0;
				}
			}
		}
	}
	printf("\nCreated linked list\n");
	/*-----After sorting*/
	Temp = First;
	printf("\nAfter sorting:");
	for (uint8_t i = 0; i < arl; i++)
	{
		if (i % 10 == 0) printf("\n");
		printf("[%u] ", Temp->val);
		Temp = Temp->next;
	}

	/*-----free*/
	printf("\nFree List:\n");
	while (First->next != NULL)
	{
		Temp = First;
		Tempback = First;
		while (Temp->next != NULL)
		{
			Temp = Temp->next;
		}/*Chay toi cuoi list*/
		while (Tempback->next != Temp)
		{
			Tempback = Tempback->next;
		}/*Chay toi sat temp*/
		free(Temp);/*xoa phan tu cuoi*/
		Tempback->next = NULL;
		printf("[free] ");
	}
	free(First);
	printf("[free]\n");

	/*-----return 1 loop*/
	return 1;
}
uint8_t exit_loop(uint8_t* arr, const int arl)
{
	/*return 0 exit loop*/
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


