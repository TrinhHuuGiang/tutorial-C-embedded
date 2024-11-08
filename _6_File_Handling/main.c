/*
    <main.c>: <File Handling>.
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
#include <stdlib.h>
#include <string.h>
#define LINE_LEN (256)
/*******************************************************************************
* Variables
******************************************************************************/

/*******************************************************************************
* Prototypes
******************************************************************************/
/*
* ----check_arg_main----
* @param argc : check number param argc input to console
* @return -1 : if param < 3
* @return  0 : if param >= 3
*/
static int check_arg_main(int argc);
/*
* ----open_file----
* @param f_ptr2 : pointer to FILE pointer; it = NULL if can't open file
* @param f_name, f_mode : name and mode open file
* @return -1 : if can't open
* @return  0 : if success
*/
static int open_file(FILE** f_ptr2, const char* f_name, const char* f_mode);
static int close_file(FILE* f_ptr, const char* f_name);
static char* alloc_line();
/*
* ----show_file----
* @param f_ptr : pointer to FILE
* @return -1 : if can't allocate memory to store line
* @return  0 : if success show file
*/
static int show_file(FILE* f_ptr);
/*
* ----string_to_line----
* add a string as new line into file
* @param f_ptr : pointer to FILE
* @return -1 : if can't allocate memory to store line
* @return  0 : if success show file
*/
static int string_to_file(FILE* f_ptr);
/*
* ----search_string----
* search a string if it exist in file
* @param f_ptr : pointer to FILE
* @return -1 : if can't allocate memory to store line
* @return -2 : if user input nothing (only enter)
* @return  0 : if success show file
*/
static int search_string(FILE* f_ptr);
static void menu(FILE* f_ptr);
/*******************************************************************************
* Code
******************************************************************************/
static void menu(FILE* f_ptr)
{
	int temp = 0;
	while (temp != 4)
	{
		int code = 0;
		printf("______________________MENU______________________\n");
		printf("1. Display file|2. Add new line|3. Search string\n->Input your query : ");
		scanf("%d", &temp);
		getchar();
		if (temp == 1)
		{
			code = show_file(f_ptr);
		}
		else if (temp == 2)
		{
			code = string_to_file(f_ptr);
		}
		else if (temp == 3)
		{
			code = search_string(f_ptr);
		}
		if (code)
		{
			printf("Error: %d\n", code);
		}
	}
}
static int search_string(FILE* f_ptr)
{
	long hor = 0, ver = 0;/*coordinate*/
	uint32_t x = 0;/*index of line malloc*/
	int ch;/*keep character from fgetc*/
	long cur = 0;/*check current cursor*/
	long fix = 0;/*modify cur*/
	/*allocate*/
	char* line = alloc_line();
	if (line == NULL)
	{
		printf("Error: Can't allocate memory dynamically\n");
		return -1;
	}
	line[0] = '\0';
	/*input string to find*/
	printf("Input a string to search\n-> ");
	if (!scanf("%[^\n]%*c", line))
	{
		getchar();
		printf("Input nothing?\n");
		return -2;
	}
	/*
	* Searching
	* set cursor to start of file
	* search string[0], then search string[1],...
	* if string[0] == position value, compare next with string [1],...
	* skip the last string[0] position
	* if string[last] == '\0' -> found
	* position value == EOF -> not found
	*/
	rewind(f_ptr);
	ch = fgetc(f_ptr);
	for (cur; (line[x] != '\0') && (ch != EOF); cur++)
	{
		rewind(f_ptr);
		fseek(f_ptr, cur, SEEK_SET);
		ch = fgetc(f_ptr);
		if ((char)ch == '\n')
		{
			fix--;
		}
		x = 0;
		while ((line[x] == (char)ch) && (line[x] != '\0') && (ch != EOF))
		{
			x++;
			ch = fgetc(f_ptr);
		}
	}
	cur = cur - 1 + fix/2;
	if (line[x] == '\0')
	{
		printf("Found\n");
		/*coordinates*/
		rewind(f_ptr);
		for (int i = 0; i != cur; i++)
		{
			ch = fgetc(f_ptr);
			if ((char)ch == '\n')
			{
				hor++;
				ver = 0;
			}
			else
			{
				ver++;
			}
		}
		printf("Coordinates(hor, ver) = (%ld,%ld)\n",hor,ver);
	}
	else
	{
		printf("Not found!\n");
	}
	/*free alloc*/
	free(line);
	return 0;
}
static int string_to_file(FILE* f_ptr)
{
	int n;
	/*allocate*/
	char* line = alloc_line();
	if (line == NULL)
	{
		printf("Error: Can't allocate memory dynamically\n");
		return -1;
	}
	/*set cursor at the end of file*/
	fseek(f_ptr, 0, SEEK_END);
	/*
	* input line
	* +if only enter '\n', scanf return 0 and '\n' exist in stdin
	* getchar() to give '\n'
	* +if input a string so scanf return 1 argument
	* so input string into file
	*/
	printf("Input a string at the end of file\n-> ");
	n = scanf("%[^\n]%*c",line);
	/*add string*/
	if (n != 0)
	{
		fprintf(f_ptr, "\n%s", line);
	}
	else
	{
		getchar();
		fprintf(f_ptr, "\n");
	}
	/*free alloc*/
	free(line);
	return 0;
}
static int show_file(FILE* f_ptr)
{
	int n;
	/*allocate*/
	char* line = alloc_line();
	if (line == NULL)
	{
		printf("Error: Can't allocate memory dynamically\n");
		return -1;
	}
	/*set cursor at start of file*/
	rewind(f_ptr);
	/*
	* get file
	* +if line of text only have '' then fscanf return 0
	* so that cursor not change -> set cursor next 2 to ignore \r\n
	*/
	n = fscanf(f_ptr, "%[^\n]%*c", line);
	while (n != EOF)
	{
		if (n != 0)
		{
			printf("%s\n", line);
		}
		else
		{
			fseek(f_ptr, 2, SEEK_CUR);
			printf("\n");
		}
		n = fscanf(f_ptr, "%[^\n]%*c", line);
	}
	/*free alloc*/
	free(line);
	return 0;
}
static char* alloc_line()
{
	char* temp = (char*)malloc(LINE_LEN*sizeof(char));
	return temp;
}
static int close_file(FILE* f_ptr, const char* f_name)
{
	if (fclose(f_ptr))
	{
		printf("Error: Can't close file [%s]\n", f_name);
		return -1;
	}
	else
	{
		printf("File [%s] closed\n",f_name);
		return 0;
	}
}
static int open_file(FILE** f_ptr2, const char* f_name, const char* f_mode)
{
	*f_ptr2 = fopen(f_name,f_mode);
	if (*f_ptr2 == NULL)
	{
		printf("Error: File [%s] does not exist\n", f_name);
		return -1;
	}
	else
	{
		printf("File name [%s] - Mode [%s]\n", f_name, f_mode);
		return 0;
	}
}
static int check_arg_main(int argc)
{
	if (argc < 3)
	{
		printf("Error: <main.exe> <filePath> <mode>\n");
		return -1;
	}
	else
	{
		return 0;
	}
}
int main(int argc, char* argv[])
{
	/*local val*/
	FILE* file_p = NULL;
	/*check argc*/
	if (check_arg_main(argc))
	{
		return -1;
	}
	/*
	* Open new stream to file name "Hello World.txt"
	* with open mode r (read) or another mode
	* if file not found, open file with mode a+
	* if file not found, return errcode -2
	*/
	if (open_file(&file_p, argv[1], argv[2]))
	{
		if (open_file(&file_p, argv[1], "a+"))
		{
			return -2;
		}
	}
	/*
	* Close stream, 0 if success
	* if can't close return -3
	* Open stream with mode [r+]
	* if can't open return -4
	*/
	if (close_file(file_p, argv[1]))
	{
		return -3;
	}
	if (open_file(&file_p, argv[1], "r+"))
	{
		return -4;
	}
	/*
	* after open to read and write [r+]
	* open menu
	*/
	menu(file_p);
	/*exit*/
	close_file(file_p,argv[1]);
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