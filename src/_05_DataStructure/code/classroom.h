/*
    Copyright (C) <2024> <Giang Trinh>

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
*/

#ifndef _HEADER_CLASSROOM_
#define _HEADER_CLASSROOM_
/*******************************************************************************
* Definitions
******************************************************************************/
#include <stdint.h>

typedef struct _Student
{
	char* name;// 59 byte
	uint32_t id;//4 byte
	uint8_t score;//1 byte
	struct _Student* Next;
}Student;

typedef uint8_t(*fp_class)(Student**);

/*******************************************************************************
* API
******************************************************************************/
uint8_t add_Student(Student** head);/*add new student*/
uint8_t remove_Student(Student** head);/*remove a student*/
uint8_t find_Student(Student** head);/*find a student*/
uint8_t view_Classroom(Student** head);/*view entire classroom*/
uint8_t close_Classroom(Student** head);/*delete classroom*/

#endif/*_HEADER_CLASSROOM_*/

/*
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */