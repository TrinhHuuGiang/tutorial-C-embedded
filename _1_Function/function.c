/*
    <function.c>: <Check 2 numbers input are prime>.
    Copyright (C) <2024> <Giang Trinh>

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
*/

/*******************************************************************************
* Definitions
******************************************************************************/
#include <stdio.h>  /*printf*/
#include <stdlib.h> /*atoi*/
#include <ctype.h>  /*isdigit*/
#include <string.h> /*strcmp*/

/*******************************************************************************
* Prototypes
******************************************************************************/
static int check_number(char* argv); /*check an object is an integer*/
static int check_prime(int num); /*check an integer is an prime*/
static void help(); /*help() function*/
static int check_input(int argc, char* argv[]); /*check number objects input*/
/*******************************************************************************
* Code
******************************************************************************/
int main(int argc,char* argv[])
{
	int code = check_input(argc, argv);
	if (code)
	{
		printf("Error code: %d\n", code);
		printf("Type 'help' for more information\n");
	}
	/*End*/
	return 0;
}
static int check_number(char* argv)
{
	char* p_argv = argv;
	/*return 0 is number, return 1 is string*/
	if ((*p_argv) == '-') p_argv++;  /*check minus*/
	if ((*p_argv) == '\0') return 0; /*check only '-'*/
	while (*p_argv)
	{
		if (!isdigit(*p_argv))
		{
			return 1;
		}
		p_argv++;
	}
	return 0;
}
static int check_prime(int num)
{
	int count;
	/*return 0 is prime, return 1 is not*/
	if (num < 2) return 1;
	else if (num == 2) return 0;
	else if (num % 2 == 0) return 1;
	else
		for (count = 3; count < (num - 1); count += 2)
		{
			if (num % count == 0) return 1;
		}
	return 0;
}
static void help()
{
	printf("Enter 2 numbers to check if they are prime.\n");
	printf("Error codes:\n");
	printf("(-1) enter nothing\n");
	printf("(-2) enter 1 character\n");
	printf("(-3) enter only 1 number\n");
	printf("(-4) enter more than 2 numbers\n");
	printf("( 1) unknown\n");
}
static int check_input(int argc, char* argv[])
{
	/*(-1) if check input nothing*/
	if (argc == 1)
	{
		printf("Inputs nothing\n");
		return -1;
	}
	/*(0) if call 'help'
	(-2)  if check 1 charactor
	(-3)  if check only 1 number*/
	else if (argc == 2)
	{
		if (strcmp(argv[1], "help") == 0)
		{
			help();
			return 0;
		}
		if (check_number(argv[1]))
		{
			printf("Inputs a character\n");
			return -2;
		}
		printf("Inputs only one number\n");
		return -3;
	}
	/*(-2) if check input 2 object, at least 1 character
	  (0)  if input 2 integer*/
	else if (argc == 3)
	{
		int i;
		int temp = 0;
		for (i = 1; i < 3; i++)
		{
			if (check_number(argv[i]))
			{
				temp++;
			}
		}
		if (temp)
		{
			printf("Inputs a character\n");
			return -2;
		}
		/*input 2 integer*/
		i = atoi(argv[1]);
		if (!check_prime(i))
		{
			printf("%d is a prime number\n", i);
			temp++;
		}
		i = atoi(argv[2]);
		if (!check_prime(i))
		{
			printf("%d is a prime number\n", i);
			temp++;
		}
		if (!temp)
		{
			printf("There are no prime numbers\n");
		}
		return 0;
	}
	/*(-4) if check input more than 2 object*/
	else if (argc > 3)
	{
		printf("Inputs more than 2 numbers\n");
		return -4;
	}
	/*(1) unknown error (ex: argc < 1)*/
	else return 1;
}


/*
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */