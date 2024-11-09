/*
    <main.c>:
	Handle Srecord file errors and print out the results to the file and terminal.
	The printing process will stop when an error is encountered.
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
#include <string.h>
#include "srec.h"
#include "filehandle.h"
#define MENU_HELP "1"
#define S_RECORD_DEFAULT "S rec.txt"
#define S_REC_OUTPUT "Output.txt"
#define S_MAX_FUNCTION (11)
/*******************************************************************************
* Variables
******************************************************************************/
fp_SR fp_S[] =
{
	&sr_Check_Type,
	&sr_Check_EvenSize,
	&sr_Check_SizeMin,
	&sr_Check_S4Type,
	&sr_Check_ByteCountMin,
	&sr_Check_ByteCountMatching,
	&sr_Check_AddressSize,
	&sr_Check_NumberRecord,
	&sr_Check_S0Address,
	&sr_Check_DataSize,
	&sr_Check_CheckSumValue
};
/*******************************************************************************
* Prototypes
******************************************************************************/
static void help();
/*check param user input*/
static int check_file_invalid(int argc);
/*try to open default txt or user path*/
static int open_to_read(FILE** file_p, char* file_name);
/*Call back to driver to handle a S record, return error code*/
static ErrCode DriverHandlerRecord(Srec* record, char* line, uint32_t size, FILE* file_p, fp_SR fps_i);
/*clear output file*/
static int clear_Output();
/*S record must be null
* return 1 if not error, not end of file -> continue
* return 0 if end of file -> end loop
* return -1 if get line failed -> error-> return
* return -2 if S record type error -> end loop
*/
static int check_line_Record(Srec* record, FILE* file_p, long int* cur);
/*After check, save record ok to text*/
static int log_RecordToText(Srec* record, FILE* wfile_p, uint32_t num_order);
/*After save, log error, free and re-init struct Record*/
static int print_and_RenewRecord(Srec** record_p, uint32_t num_order);

/*******************************************************************************
* Code
******************************************************************************/
static int print_and_RenewRecord(Srec** record_p, uint32_t num_order)
{
	printf("%u |", num_order);
	/*check type*/
	printf("S%d|", (*record_p)->type);
	/*check byte count*/
	if ((*record_p)->byteCount == NULL)
	{
		printf("empty |");
	}
	else
	{
		printf("0x");
		for (int i = 0; ((*record_p)->byteCount)[i] != END_OF_ARR_HEX; i++)
		{
			printf("%X", ((*record_p)->byteCount)[i]);
		}
		printf("|");
	}
	/*check address*/
	if ((*record_p)->address == NULL)
	{
		printf("empty |");
	}
	else
	{
		printf("0x");
		for (int i = 0; ((*record_p)->address)[i] != END_OF_ARR_HEX; i++)
		{
			printf("%X", ((*record_p)->address)[i]);
		}
		printf("|");
	}
	/*check data*/
	if ((*record_p)->dataLen == 0)
	{
		printf("empty |");
	}
	else
	{
		for (int i = 0; i < (*record_p)->dataLen; i++)
		{
			printf("%X", ((*record_p)->data)[i]);
			if ((i % 2) != 0)
			{
				printf(" ");
			}
		}
		printf("|");
	}
	/*check sum*/
	if ((*record_p)->checkSum == NULL)
	{
		printf("empty |");
	}
	else
	{
		printf("0x");
		for (int i = 0; ((*record_p)->checkSum)[i] != END_OF_ARR_HEX; i++)
		{
			printf("%X", ((*record_p)->checkSum)[i]);
		}
		printf("|");
	}
	/*add new line*/
	printf("\n");
	/*Free and Renew*/
	sr_Free(record_p);
	(*record_p) = sr_Init();
	if ((*record_p) == NULL)
	{
		return -1;
	}
	/*renew success*/
	return 0;
}
static int log_RecordToText(Srec* record, FILE* wfile_p, uint32_t num_order)
{
	if (wfile_p == NULL)
	{
		printf("[ERROR - %s] - Write record to [%s] failed\n", __FUNCTION__,S_REC_OUTPUT);
		return -1;
	}
	/*write newline + num order*/
	fprintf(wfile_p, "%u 0x",num_order);
	/*write address*/
	for (int i = 0; (record->address)[i] != END_OF_ARR_HEX; i++)
	{
		fprintf(wfile_p, "%X", (record->address)[i]);
	}
	/*write data*/
	if ((record->dataLen) != 0)
	{
		for (int i = 0; (record->data)[i] != END_OF_ARR_HEX; i++)
		{
			if ((i % 2) == 0)
			{
				fprintf(wfile_p, " ");
			}
			fprintf(wfile_p, "%X", (record->data)[i]);
		}
	}
	fprintf(wfile_p, "\n");
	return 0;
}
static int clear_Output()
{
	FILE* p = file_Open(S_REC_OUTPUT, "w");
	if (p == NULL)
	{
		return -1;
	}
	if (file_Close(p))
	{
		return -2;
	}
	return 0;
}
static int check_line_Record(Srec* record, FILE* file_p, long int* cur)
{
	int ret;
	char* line = NULL;
	uint32_t size = 0;/*size of line will include '\0'*/
	ErrCode code;/*return error*/
	int temp = 0;/*temp = 1 if a few errors were skipped*/
	ret = file_Getline(file_p, cur, &line, &size);
	if (ret < 0)
	{
		return -1;
	}
	else if (ret == 2)
	{
		return -1;
	}
	else if (ret == 1)
	{
		return 0;
	}
	/*else ret = 0*/
	size--;/*ignore '\0'*/
	/*handling drivers*/
	for (int i = 0; i < S_MAX_FUNCTION; i++)
	{
		code = DriverHandlerRecord(record,line,size,file_p,fp_S[i]);
		if (code == S_TYPE_INVALID ||
			code == S_SIZEEVEN_INVALID ||
			code == S_4_RESERVED ||
			code == S_BYTECOUNT_EMPTY ||
			code == S_ADDRSIZE_INVALID ||
			code == S_MISSING_CHECKSUM ||
			code == S_MALLOC_FAILED)
		{
			file_Freeline(&line);
			return -2;
		}
		else if (code != S_OK)
		{
			temp = 1;
		}
	}
	/*free line*/
	file_Freeline(&line);
	if (temp != 0) /*A few errors were skipped*/
	{
		return -2;
	}
	/*no error*/
	return 1;
}
static ErrCode DriverHandlerRecord(Srec* record, char* line, uint32_t size, FILE* file_p, fp_SR fps_i)
{
	return (*fps_i)(record, line, size, file_p);
}
static int open_to_read(FILE** file_p, char* file_name)
{
	*file_p = file_Open(file_name, "r");
	if ((*file_p) == NULL)
	{
		printf("[ERROR - %s] - Lost file\n", __FUNCTION__);
		printf("[ERROR - %s] - Path: %s\n", __FUNCTION__, file_name);
		return -1;
	}
	printf("File path: %s\n", file_name);
	return 0;
}
static int check_file_invalid(int argc)
{
	if (argc == 1)
	{
		printf("Number parameter: %d\n", argc - 1);
		printf("Running default S-record file [S rec.txt]\n");
		return 0;
	}
	else if (argc == 2)
	{
		printf("Number parameter: %d\n", argc - 1);
		return 1;
	}
	else if (argc > 2)
	{
		printf("[ERROR - %s] - Number of input parameters in the program [%d] is greater than 1\n", __FUNCTION__, argc-1);
		printf("[ERROR - %s] - Input syntax for help: Program.exe 1\n", __FUNCTION__);
		return -1;
	}
	return 0;
}
static void help()
{
	printf("[HELP]\n");
	printf("1. Syntax:\n");
	printf(" - Check S-record file with input path__ : Program.exe Path\n");
	printf(" - run default S-record file [S rec.txt] : Program.exe\n");
	printf(" - Help_________________________________ : Program.exe 1\n");
	printf("2. Exception error: \n");
	printf(" - Number of input parameters in the program is greater than 1\n");
	printf(" - File name not found\n");
	printf("3. Function:\n");
	printf(" - Check if the line is a S-record\n");
	printf(" - Check character is not HEX value\n");
	printf(" - Check number of character is odd\n");
	printf(" - Check size min of S-record\n");
	printf(" - Check type is S4\n");
	printf(" - Check byte count empty\n");
	printf(" - Check byte count invalid\n");
	printf(" - Check byte count mismatch\n");
	printf(" - Check address size invalid\n");
	printf(" - Check type S5/S6 count number S1/S2/S3 mismatch\n");
	printf(" - Check type S0 address not 0x0000\n");
	printf(" - Check type S0 data end not null\n");
	printf(" - Check data type S1/S2/S3 bigger than 32 byte\n");
	printf(" - Check data type S5 to S9 bigger than 0 byte\n");
	printf(" - Check check sum empty\n");
	printf(" - Check check sum wrong\n");
	printf("____________________________________________________________________\n");
}
int main(int argc, char* argv[])
{
	/*
	* [Local Variables]
	* - 1 ret to get return code
	* - 1 S record to save data
	* - 1 [r mode] File pointer and cursor
	* - add new [a mode] pointer write output.txt
	* - 1 numerical order to print s rec
	*/
	int ret;
	Srec* record = sr_Init();
	FILE* file_p = NULL;
	long int cur = 0;
	FILE* wfile_p = NULL;
	uint32_t num_order;
	/*
	* [open stream file]
	* - check number argc
	* - Try open file with read mode
	*/
	ret = check_file_invalid(argc);
	if(ret == 0)/*default record*/
	{
		if (open_to_read(&file_p, S_RECORD_DEFAULT))
		{
			return -1;
		}
	}
	else if (ret == 1)/*help or path*/
	{
		if (strcmp(argv[1], MENU_HELP))
		{
			if (open_to_read(&file_p, argv[1]))
			{
				return -2;
			}
		}
		else
		{
			help();
			return 0;
		}
	}
	else if (ret == -1)
	{
		return -3;
	}
	/*[Clear Output.txt to save data]
	* open with 'w' mode then close
	*/
	if(clear_Output())
	{
		return -4;
	}
	/*[Open Output.txt [a mode] to save data]*/
	wfile_p = file_Open(S_REC_OUTPUT, "a");
	if (wfile_p == NULL)
	{
		return -5;
	}
	/*
	* [PROCESS]
	* [Read file text to check all S record]
	* open file success
	* - read each S record to a dynamic array [line] to process
	* - result is will save into the pointer struct [record]
	*/
	printf("____________________________________________________________________\n");
	printf("    S Record == |S Type|Byte Count|Address/count|Data|Check Sum|\n");
	num_order = 0;
	ret = 1;
	while(ret == 1)
	{
		ret = check_line_Record(record, file_p, &cur);
		if(ret == -1)
		{
			/*get string S rec failed*/
			return -6;
		}
		else if (ret == 1)
		{
			if(log_RecordToText(record, wfile_p, num_order))
			{
				/*log failed*/
				return -7;
			}
		}
		/*print line Error*/
		else if (ret == -2)
		{
			printf("\n[ERROR - %s] - Error Line [%u]\n", __FUNCTION__, num_order);
		}
		/*print Record value*/
		if (ret != 0 )
		{
			if (print_and_RenewRecord(&record, num_order))
			{
				/*renew failed*/
				return -8;
			}
		}
		/*else if ret = 1 -> still checking ->loop*/
		/*if ret -2 -> print error -> end loop*/
		/*if ret 0 -> end loop*/
		num_order++;
	}
	printf("____________________________________________________________________\n");
	printf("[END] SAVED RECORD: %s\n", S_REC_OUTPUT);
	printf("[END] CHECK DONE\n");

	/*free alloc struct S record*/
	free(record);/*free because of member is null*/
	/*free file pointer*/
	if (file_Close(wfile_p))
	{
		return -9;
	}
	if (file_Close(file_p))
	{
		return -10;
	}
	/*end*/
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