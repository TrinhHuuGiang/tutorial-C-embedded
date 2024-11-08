/*
    <main.c>: <Using linked list to manage classroom>.
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
#include <stdlib.h>
#include <stdint.h>
#include "classroom.h"
#define MIN_Q (0)
#define MAX_Q (4)/*0-add,1-remove,2-find,3-view,4-close*/
#define Menu_Q (5)/*5-menu*/
/*******************************************************************************
* Variables
******************************************************************************/

fp_class fpc[] =
{
	&add_Student,	
	&remove_Student,
	&find_Student,
	&view_Classroom,
	&close_Classroom
};

/*******************************************************************************
* Prototypes
******************************************************************************/
/**
 * Handles driver operations for a classroom.
 *
 * @param head pointer to the address of the first node in the linked list.
 * @param fpc_i function pointer from the fpc[] array.
 * @return -1 if an error occurs or loop query (add/remove/find/view/close); 0 if deleted classroom.
 */
static uint8_t DriverHandlerClass(Student** head, int query, fp_class fpc_i);

/**
 * Prints the menu for the classroom.
 */
static void menu_Classroom();

/**
 * Allows user to input queries to modify the classroom.
 *
 * @param head pointer to the address of the first node in the linked list.
 */
static void Console_Menu(Student** head);

/*******************************************************************************
* Code
******************************************************************************/
static uint8_t DriverHandlerClass(Student** head, int query, fp_class fpc_i)
{
	uint8_t code = (*fpc_i)(head);
	/*MAX_Q is close classroom and exit*/
	if (query == MAX_Q)
		return 0;
	/*capture bug*/
	else if (code)
		printf("[error!]\n");
	/*loop*/
	return -1;
}
static void menu_Classroom()
{
	printf("0. add  new  student to the classroom\n");
	printf("1. remove a student out off classroom\n");
	printf("2. find one student in classroom\n");
	printf("3. View all student in classroom\n");
	printf("4. close classroom\n");
}
static void Console_Menu(Student** head)
{
	int query;
	do/*input a query*/
	{
		query = -1;
		while (query < MIN_Q || query > (MAX_Q))
		{
			printf("_________________________________________________\n");
			printf("Input %d to %d [%d for help], your query: ", MIN_Q, MAX_Q, Menu_Q);
			scanf("%d", &query);
			/*If need help, open menu*/
			if (query == Menu_Q)
			{
				menu_Classroom();
			}
		}
	} while (DriverHandlerClass(head, query, fpc[query]));
	printf("_________________________________________________\n");
	printf("\n<<= = = Ended menu = = =>>\n\n");
}
int main(int argc, char* argv[])
{
	Student* first = NULL;
	printf("%10s----OPEN NEW CLASS----\n","");
	/*open menu, input user query*/
	Console_Menu(&first);
	/*ended*/
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