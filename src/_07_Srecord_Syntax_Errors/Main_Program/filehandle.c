/*
    <filehandle.c>: <File data processing functions>
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
#include "filehandle.h"

/*******************************************************************************
* Code
******************************************************************************/
uint32_t file_NumRecord(FILE* file_p, int type_rec)
{
	int ch;
	uint32_t count = 0;
	/*find from start of file*/
	rewind(file_p);
	ch = fgetc(file_p);
	while (ch != EOF)
	{
		if (((unsigned char)ch == 'S') && (file_CHARtoINT((unsigned char)fgetc(file_p)) == type_rec))
		{
			count++;/*Record type found*/
		}
		if (fgetc(file_p) == EOF)
		{
			return count; /*EOF*/
		}
		else
		{
			fseek(file_p,-1,SEEK_CUR);
			ch = fgetc(file_p);
		}
	}
	return count;
}
int file_6HEXtoINT(int a, int b, int c, int d, int e, int f)
{
	a &= 0xf;
	b &= 0xf;
	c &= 0xf;
	d &= 0xf;
	e &= 0xf;
	f &= 0xf;
	a = a << (4 * 5);
	b = b << (4 * 4);
	c = c << (4 * 3);
	d = d << (4 * 2);
	e = e << (4 * 1);
	return(a | b | c | d | e | f);
}
int file_4HEXtoINT(int a, int b, int c, int d)
{
	a &= 0xf;
	b &= 0xf;
	c &= 0xf;
	d &= 0xf;
	a = a << (4 * 3);
	b = b << (4 * 2);
	c = c << (4 * 1);
	return(a | b | c | d);
}
int file_2HEXtoINT(int a, int b)
{
	a &= 0xf; /*00...0000xxxx*/
	a = a << 4;/*00..xxxx0000*/
	b &= 0xf;/*00...0000yyyy*/
	return (a | b);/*xxxx yyyy*/
}
int file_CHARtoINT(unsigned char ch)
{
	/*if be hex return 0 to 15 (0 to F)*/
	if (ch >= 48 && ch <= 57)
	{
		return ch - 48;
	}
	else if (ch >= 65 && ch <= 70)
	{
		return ch - 55;
	}
	else if (ch >= 97 && ch <= 102)
	{
		return ch - 87;
	}
	/*if not hex*/
	return -1;
}
int file_Getline(FILE* file_p, long int* cur, unsigned char** line, uint32_t* size)
{
	int ch;/*a value of fgetc()*/
	int i;/*index of *line[i]*/

	*size = 0;
	file_Freeline(line);

	fseek(file_p, *cur, SEEK_SET);
	ch = fgetc(file_p);
	while (((unsigned char)ch != '\n') && (ch != EOF))
	{
		(*size)++;
		ch = fgetc(file_p);
		if (((unsigned char)ch == '\n') || (ch == EOF))
		{
			(*size)++;/*add index for '\0'*/
		}
	}

	if ((*size) == 0 && ch == EOF)
	{
		return 1;/*current is EOF*/
	}
	else if (((*size) == 0) && ((unsigned char)ch == '\n'))
	{
		(*cur) += 2;
		return 2;/*a line only \n*/
	}
	else if ((*size) == 0)
	{
		printf("[ERROR - %s] - UNKNOWN\n", __FUNCTION__);
		return -1;
	}
	else/* size != 0 */
	{
		*line = (unsigned char*)malloc((*size) * sizeof(unsigned char));
		if ((*line) == NULL)
		{
			printf("[ERROR - %s] - ALLOCATE FAILED\n", __FUNCTION__);
			return -2;
		}
		fseek(file_p, *cur, SEEK_SET);
		i = 0;
		ch = fgetc(file_p);
		while (((unsigned char)ch != '\n') && (ch != EOF))
		{
			(*cur)++;
			(*line)[i] = ch;
			i++;
			ch = fgetc(file_p);
			if ((unsigned char)ch == '\n')
			{
				/* current cursor is \r|\n, so that cursor ++*/
				(*cur) += 2;
				(*line)[i] = '\0';
			}
		}
	}
	return 0;
}
void file_Freeline(unsigned char** line)
{
	if (*line != NULL)
	{
		free(*line);
		*line = NULL;
	}
}
int file_Close(FILE* file_p)
{
	if (fclose(file_p) == EOF)
	{
		printf("[ERROR - %s] - CLOSE FILE FAILED\n", __FUNCTION__);
		return -1;
	}
	else
		return 0;
}
FILE* file_Open(const char* file_name, const char* mode)
{
	FILE* file_p = fopen(file_name, mode);
	if (file_p == NULL)
	{
		printf("[ERROR - %s] - FILE OPENING FAILED\n", __FUNCTION__);
	}
	return file_p;
}

/*
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */