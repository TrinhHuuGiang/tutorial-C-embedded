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
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "classroom.h"
#define DEBUG (0)
#define NAME_LENGTH (59)
/*******************************************************************************
* Variables
******************************************************************************/

/*******************************************************************************
* Prototypes
******************************************************************************/
/**
 * Create new Student.
 *
 * name + id + score = 59 + 3 + 1 = 64 byte
 * @return pointer NULL if failed; pointer type Student if true
 */
static Student* create_Student();

/**
 * Free a Student.
 *
 * free name
 * free memory Student
 * @return pointer NULL
 */
static void free_Student(Student* stu);
/*******************************************************************************
* Code
******************************************************************************/
static Student* create_Student()
{
	Student* newSt = (Student*)malloc(sizeof(Student));
	if (newSt == NULL) return NULL;
	else
	{
		newSt->name = (char*)malloc(NAME_LENGTH*sizeof(char));
		if (newSt->name == NULL)
		{
			free(newSt);
			return NULL;
		}
		else return newSt;
	}
}
static void free_Student(Student* stu)
{
	if (stu == NULL)
	{
		printf("This is a NULL pointer\n");
		return;
	}
	else
	{
		free(stu->name);
		free(stu);
		printf("Free allocate\n");
	}
}
uint8_t add_Student(Student** head)
{
	Student* newSt;
	Student* last = * head;
	/*Create new node*/
	newSt = create_Student();
	if (newSt == NULL)
	{
		printf("Can't allocate new Node\n");
		return -1;
	}
	/*Input information*/
	printf("----ADD NEW STUDENT----\n");
	printf("Name: ");
	scanf("%*c%[^\n]%*c", newSt->name);
	printf("id: ");
	scanf("%u", &(newSt->id));
	printf("score: ");
	scanf("%hhu", &(newSt->score));
	newSt->Next = NULL;
#if DEBUG
	printf("1 %s [%s] [%u] [%hhu]\n",__FUNCTION__, newSt->name, newSt->id, newSt->score);
#endif
	/*add_to_linked list*/
	/*1. list is empty*/
	if (*head == NULL)
	{
		*head = newSt;
#if DEBUG
		printf("2 %s [%s] [%u] [%hhu]\n", __FUNCTION__, (*head)->name, (*head)->id, (*head)->score);
#endif
		return 0;
	}
	/*2. add new student into the last node*/
	while (last->Next != NULL)
	{
		last = last->Next;
	}
	last->Next = newSt;
#if DEBUG
	printf("hello\n");
	last = *head;
	while (last->Next != NULL)
	{
		printf("3 %s [%s] [%u] [%hhu]\n", __FUNCTION__, last->name, last->id, last->score);
		last = last->Next;
	}
	printf("3 %s [%s] [%u] [%hhu]\n", __FUNCTION__, last->name, last->id, last->score);
#endif
	return 0;
}
uint8_t remove_Student(Student** head)
{
	uint32_t id;
	Student* last = *head;
	Student* pre = *head;
	/*check empty classroom*/
	if (*head == NULL)
	{
		printf("Empty classroom\n");
		return -1;
	}
	/*input info*/
	printf("----REMOVE STUDENT----\n");
	printf("Input student id: ");
	scanf("%u", &id);
	/*check if exist */
	/*1. if that is first node*/
	if ((*head)->id == id)
	{
		(*head) = (*head)->Next;
		free_Student(last);
		printf("Removed id [%u]\n", id);
		return 0;
	}
	/*2. if that is not first node*/
	/*2.1 check where is id*/
	while ((last->Next != NULL)&&(last->id!=id))
		last = last->Next;
	/*2.2 if student at the end*/
	if ((last->id == id)&&(last->Next==NULL))
	{
		while (pre->Next != last)
			pre = pre->Next;
		pre->Next = NULL;
		free_Student(last);
	}
	/*2.3 if student not found*/
	else if (last->Next == NULL)
	{
		printf("not exist student id [%u]\n", id);
		return -1;
	}
	/*2.4 if student in the middle*/
	else
	{
		while (pre->Next != last)
			pre = pre->Next;
		pre->Next = last->Next;
		free_Student(last);
	}
	printf("Removed id [%u]\n", id);
	return 0;
}
uint8_t find_Student(Student** head)
{
	uint32_t id;
	Student* last = *head;
	/*check empty classroom*/
	if (*head == NULL)
	{
		printf("Empty classroom\n");
		return -1;
	}
	/*input info*/
	printf("----FIND STUDENT----\n");
	printf("Input student id: ");
	scanf("%u", &id);
	/*find*/
	while (last->Next != NULL && last->id != id)
	{
		last = last->Next;
	}
	if (last->id != id)
	{
		printf("Student not found\n");
		return -1;
	}
	printf("%15s<%4s>%16s<%2s>%5s<%5s>\n", "", "NAME", "", "ID", "", "SCORE");
	printf("[%30s] [%10u] [%3hhu]\n", last->name, last->id, last->score);
	return 0;
}
uint8_t view_Classroom(Student** head)
{
	int count = 0;
	Student* last = *head;
	printf("----CLASS ROOM----\n");
	/*If class empty*/
	if (*head == NULL)
	{
		printf("Empty classroom\n");
		return -1;
	}
	else
	{
		printf("%25s<%4s>%16s<%2s>%5s<%5s>\n","", "NAME","","ID","","SCORE");
		while (last->Next != NULL)
		{
			printf("STT: %4d [%30s] [%10u] [%3hhu]\n", ++count, last->name, last->id, last->score);
			last = last->Next;
		}
		printf("STT: %4d [%30s] [%10u] [%3hhu]\n", ++count, last->name, last->id, last->score);
	}
	return 0;
}
uint8_t close_Classroom(Student** head)
{
	Student* temp = *head;
	Student* next;
	while (temp != NULL)
	{
		next = temp->Next;
		free(temp);
		temp = next;
	}
	/*set first node = null*/
	*head = NULL;
#if DEBUG
	view_Classroom(head);
#endif
	printf("Class deleted!!!\n");
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