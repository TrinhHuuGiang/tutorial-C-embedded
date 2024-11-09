/*
    Copyright (C) <2024> <Giang Trinh>

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
*/

#ifndef _FILE_HANDLE_H_
#define _FILE_HANDLE_H_
/*******************************************************************************
* Definitions
******************************************************************************/
#include <stdint.h>
/*******************************************************************************
* API
******************************************************************************/
FILE* file_Open(const char* file_name, const char* mode);
int file_Close(FILE* file_p);
void file_Freeline(unsigned char** line);
/*
*----Function file_Getline----
* Please input FILE* with read mode
* 
* @param file_p : pointer to file text S record
* @param cur : pointer to variable cursor in text
* @param line : pointer to a pointer dynamic allocation string
* @param size : [size include '\\0'], pointer to a variable size of *line manage
* @return 0 : if get a normal line have a S record
* @return 1 : if only read EOF  instead of a line
* @return 2 : if only read '\\n' instead of a line
* @return -1: if unknown
* @return -2: if allocate new string failed
*/
int file_Getline(FILE* file_p, long int* cur, unsigned char** line, uint32_t* size);
/*
* file_CHARtoINT convert char to int
*/
int file_CHARtoINT(unsigned char ch);
int file_2HEXtoINT(int a, int b);/*convert 2 int[HEX] to 1 int [byte] */
int file_4HEXtoINT(int a, int b, int c, int d);/*convert 4 int[HEX] to 1 int [2 byte] */
int file_6HEXtoINT(int a, int b, int c, int d, int e, int f);/*convert 6 int[HEX] to 1 int [3 byte] */
uint32_t file_NumRecord(FILE* file_p, int type_rec);/*search all record with a type you want*/
#endif /* _FILE_HANDLE_H_*/

/*
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */