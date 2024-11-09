/*
    <main.c>:
	Handle Srecord file errors and print out the results to the file and terminal.
	The printing process until last line is encountered.
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
#define S_RECORD_COMPARE_DEFAULT "S rec compare.txt"
#define S_REC_REPORT "Test_Report.txt"
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
static uint32_t g_passed = 0;/*save number s record passed*/
static uint32_t g_failed = 0;/*save number s record failed*/
/*******************************************************************************
* Prototypes
******************************************************************************/
static void help();
/*check param user input*/
static int check_file_invalid(int argc, char** argv);
/*try to open default txt or user path*/
static int open_to_read(FILE** file_p, char* file_name);
/*Call back to driver to handle a S record, return error code*/
static ErrCode DriverHandlerRecord(Srec* record, char* line, uint32_t size, FILE* file_p, fp_SR fps_i);
/*S record must be null
* return 1 if not error, not end of file -> continue
* return 0 if end of file -> end loop
* return -1 if get line failed -> error-> return
* return -2 if S record have error -> end loop
*/
static int check_line_Record(Srec* record, FILE* file_p, long int* cur);
/*
* [using for Bonus Program]
* -print log to Report
* -print log to console
* -free and renew 2 record
*/
static int log2_RecordToText(Srec** record1_p, Srec** record2_p, FILE* w_file_p, uint32_t num_order);
static void print_total(FILE* w_file_p);
/*******************************************************************************
* Code
******************************************************************************/
static void print_total(FILE* w_file_p)
{
	fseek(w_file_p, 0, SEEK_END);
	printf("\n");/*console*/
	fprintf(w_file_p, "\n");/*report*/
	/*print passed*/
	printf("number passed: %u\n",g_passed);/*console*/
	fprintf(w_file_p, "number passed: %u\n", g_passed);/*report*/
	/*print failed*/
	printf("number failed: %u\n", g_failed);/*console*/
	fprintf(w_file_p, "number failed: %u\n", g_failed);/*report*/
	/*print total*/
	printf("total        : %u\n", g_passed+g_failed);/*console*/
	fprintf(w_file_p, "total        : %u\n", g_passed + g_failed);/*report*/
}
static int log2_RecordToText(Srec** record1_p, Srec** record2_p, FILE* w_file_p, uint32_t num_order)
{
	int pass_fail = 0;/*=0 is passed, !=0 is failed*/
	if (w_file_p == NULL)
	{
		printf("[ERROR - %s] - Write record to [%s] failed\n", __FUNCTION__, S_REC_REPORT);
		return -1;
	}
	/*--------------------------print record 1 - actual--------------------------*/
	printf("%u. Actual: |", num_order);/*console*/
	fprintf(w_file_p, "%u. Actual: |", num_order);/*report*/
	if ((*record1_p)->type == NaS)
	{
		printf("NaS|0xEmpty|0xEmpty|Empty|0xEmpty|");/*console*/
		fprintf(w_file_p,"NaS|0xEmpty|0xEmpty|Empty|0xEmpty|");/*report*/
	}
	else
	{
		/*[PRINT TYPE]*/
		printf("S%d|", (*record1_p)->type);/*console*/
		fprintf(w_file_p, "S%d|", (*record1_p)->type);/*report*/
		/*[PRINT BYTE COUNT]*/
		if ((*record1_p)->byteCount == NULL)/*print empty byte count*/
		{
			printf("0xEmpty|");/*console*/
			fprintf(w_file_p, "0xEmpty|");/*report*/
		}
		else/*print byte count*/
		{
			printf("0x");/*console*/
			fprintf(w_file_p, "0x");/*report*/
			for (int i = 0; ((*record1_p)->byteCount)[i] != END_OF_ARR_HEX; i++)
			{
				printf("%X", ((*record1_p)->byteCount)[i]);/*console*/
				fprintf(w_file_p, "%X", ((*record1_p)->byteCount)[i]);/*report*/
			}
			printf("|");/*console*/
			fprintf(w_file_p, "|");/*report*/
		}
		/*[PRINT ADD/COUNT]*/
		if ((*record1_p)->address == NULL)/*print empty address*/
		{
			printf("0xEmpty|");/*console*/
			fprintf(w_file_p, "0xEmpty|");/*report*/
		}
		else/*print address*/
		{
			printf("0x");/*console*/
			fprintf(w_file_p, "0x");/*report*/
			for (int i = 0; ((*record1_p)->address)[i] != END_OF_ARR_HEX; i++)
			{
				printf("%X", ((*record1_p)->address)[i]);/*console*/
				fprintf(w_file_p, "%X", ((*record1_p)->address)[i]);/*report*/
			}
			printf("|");/*console*/
			fprintf(w_file_p, "|");/*report*/
		}
		/*[PRINT DATA]*/
		if ((*record1_p)->data == NULL)/*print empty data*/
		{
			printf("Empty|");/*console*/
			fprintf(w_file_p, "Empty|");/*report*/
		}
		else/*print data*/
		{
			for (int i = 0; ((*record1_p)->data)[i] != END_OF_ARR_HEX; i++)
			{
				printf("%X", ((*record1_p)->data)[i]);/*console*/
				fprintf(w_file_p, "%X", ((*record1_p)->data)[i]);/*report*/
			}
			printf("|");/*console*/
			fprintf(w_file_p, "|");/*report*/
		}
		/*[PRINT CHECK SUM]*/
		if ((*record1_p)->checkSum == NULL)/*print empty check sum*/
		{
			printf("0xEmpty|");/*console*/
			fprintf(w_file_p, "0xEmpty|");/*report*/
		}
		else/*print check sum*/
		{
			printf("0x");/*console*/
			fprintf(w_file_p, "0x");/*report*/
			for (int i = 0; ((*record1_p)->checkSum)[i] != END_OF_ARR_HEX; i++)
			{
				printf("%X", ((*record1_p)->checkSum)[i]);/*console*/
				fprintf(w_file_p, "%X", ((*record1_p)->checkSum)[i]);/*report*/
			}
			printf("|");/*console*/
			fprintf(w_file_p, "|");/*report*/
		}
	}
	/*--------------------------add \\n--------------------------*/
	printf("\n");/*console*/
	fprintf(w_file_p, "\n");/*report*/
	/*--------------------------print record 2 - expected--------------------------*/
	printf("%u. Expect: |", num_order);/*console*/
	fprintf(w_file_p, "%u. Expect: |", num_order);/*report*/
	if ((*record2_p)->type == NaS)
	{
		printf("NaS|0xEmpty|0xEmpty|Empty|0xEmpty|");/*console*/
		fprintf(w_file_p, "NaS|0xEmpty|0xEmpty|Empty|0xEmpty|");/*report*/
	}
	else
	{
		/*[PRINT TYPE]*/
		printf("S%d|", (*record2_p)->type);/*console*/
		fprintf(w_file_p, "S%d|", (*record2_p)->type);/*report*/
		/*[PRINT BYTE COUNT]*/
		if ((*record2_p)->byteCount == NULL)/*print empty byte count*/
		{
			printf("0xEmpty|");/*console*/
			fprintf(w_file_p, "0xEmpty|");/*report*/
		}
		else/*print byte count*/
		{
			printf("0x");/*console*/
			fprintf(w_file_p, "0x");/*report*/
			for (int i = 0; ((*record2_p)->byteCount)[i] != END_OF_ARR_HEX; i++)
			{
				printf("%X", ((*record2_p)->byteCount)[i]);/*console*/
				fprintf(w_file_p, "%X", ((*record2_p)->byteCount)[i]);/*report*/
			}
			printf("|");/*console*/
			fprintf(w_file_p, "|");/*report*/
		}
		/*[PRINT ADD/COUNT]*/
		if ((*record2_p)->address == NULL)/*print empty address*/
		{
			printf("0xEmpty|");/*console*/
			fprintf(w_file_p, "0xEmpty|");/*report*/
		}
		else/*print address*/
		{
			printf("0x");/*console*/
			fprintf(w_file_p, "0x");/*report*/
			for (int i = 0; ((*record2_p)->address)[i] != END_OF_ARR_HEX; i++)
			{
				printf("%X", ((*record2_p)->address)[i]);/*console*/
				fprintf(w_file_p, "%X", ((*record2_p)->address)[i]);/*report*/
			}
			printf("|");/*console*/
			fprintf(w_file_p, "|");/*report*/
		}
		/*[PRINT DATA]*/
		if ((*record2_p)->data == NULL)/*print empty data*/
		{
			printf("Empty|");/*console*/
			fprintf(w_file_p, "Empty|");/*report*/
		}
		else/*print data*/
		{
			for (int i = 0; ((*record2_p)->data)[i] != END_OF_ARR_HEX; i++)
			{
				printf("%X", ((*record2_p)->data)[i]);/*console*/
				fprintf(w_file_p, "%X", ((*record2_p)->data)[i]);/*report*/
			}
			printf("|");/*console*/
			fprintf(w_file_p, "|");/*report*/
		}
		/*[PRINT CHECK SUM]*/
		if ((*record2_p)->checkSum == NULL)/*print empty check sum*/
		{
			printf("0xEmpty|");/*console*/
			fprintf(w_file_p, "0xEmpty|");/*report*/
		}
		else/*print check sum*/
		{
			printf("0x");/*console*/
			fprintf(w_file_p, "0x");/*report*/
			for (int i = 0; ((*record2_p)->checkSum)[i] != END_OF_ARR_HEX; i++)
			{
				printf("%X", ((*record2_p)->checkSum)[i]);/*console*/
				fprintf(w_file_p, "%X", ((*record2_p)->checkSum)[i]);/*report*/
			}
			printf("|");/*console*/
			fprintf(w_file_p, "|");/*report*/
		}
	}
	/*--------------------------add \\n--------------------------*/
	printf("\n");/*console*/
	fprintf(w_file_p, "\n");/*report*/
	/*--------------------------Compare \\n--------------------------*/
	/*compare type*/
	if (((*record1_p)->type) != ((*record2_p)->type))
	{
		pass_fail++;
		if(((*record1_p)->type)==NaS)
		{
			fprintf(w_file_p, "[ERROR - Type] - Actual [NaS] - Expect: [S%d]\n", (*record2_p)->type);/*report*/
		}
		else if (((*record2_p)->type) == NaS)
		{
			fprintf(w_file_p, "[ERROR - Type] - Actual [S%d] - Expect: [NaS]\n", (*record1_p)->type);/*report*/
		}
		else
		{
			fprintf(w_file_p, "[ERROR - Type] - Actual [S%d] - Expect: [S%d]\n", (*record1_p)->type, (*record2_p)->type);/*report*/
		}
	}
	/*compare byte count
	* 1 null x2
	* 2 not null
	* both null - ignore
	* both not null -> only ignore if rec 1 != rec 2
	*/
	if ((((*record1_p)->byteCount) == NULL) && (((*record2_p)->byteCount) != NULL))
	{
		pass_fail++;
		fprintf(w_file_p, "[ERROR - Byte count] - Actual [Empty] - Expect: [Not Empty]\n");/*report*/
	}
	else if ((((*record1_p)->byteCount) != NULL) && (((*record2_p)->byteCount) == NULL))
	{
		pass_fail++;
		fprintf(w_file_p, "[ERROR - Byte count] - Actual [Not Empty] - Expect: [Empty]\n");/*report*/
	}
	else if ((((*record1_p)->byteCount) != NULL) && (((*record2_p)->byteCount) != NULL))
	{
		int i;
		int j;
		/*compare size*/
		for(i = 0; ((*record1_p)->byteCount)[i]!=END_OF_ARR_HEX;i++)
		{ }
		for (j = 0; ((*record2_p)->byteCount)[j] != END_OF_ARR_HEX; j++)
		{ }
		if (i != j)
		{
			pass_fail++;
			fprintf(w_file_p, "[ERROR - Byte count size] - Actual [%d byte] - Expect: [%d byte]\n", i/2, j/2);/*report*/
		}
		/*compare value*/
		else
		{
			for (i = 0; ((*record1_p)->byteCount)[i] != END_OF_ARR_HEX; i++)
			{
				if (((*record1_p)->byteCount)[i] != ((*record2_p)->byteCount)[i])
				{
					pass_fail++;
					fprintf(w_file_p, "[ERROR - Byte count location %d] - Actual [0x%X] - Expect: [0x%X]\n",
						i, ((*record1_p)->byteCount)[i], ((*record2_p)->byteCount)[i]);/*report*/
				}
			}
		}
	}
	/*compare address*/
	if ((((*record1_p)->address) == NULL) && (((*record2_p)->address) != NULL))
	{
		pass_fail++;
		fprintf(w_file_p, "[ERROR - Address/Count] - Actual [Empty] - Expect: [Not Empty]\n");/*report*/
	}
	else if ((((*record1_p)->address) != NULL) && (((*record2_p)->address) == NULL))
	{
		pass_fail++;
		fprintf(w_file_p, "[ERROR - Address/Count] - Actual [Not Empty] - Expect: [Empty]\n");/*report*/
	}
	else if ((((*record1_p)->address) != NULL) && (((*record2_p)->address) != NULL))
	{
		int i;
		int j;
		/*compare size*/
		for (i = 0; ((*record1_p)->address)[i] != END_OF_ARR_HEX; i++)
		{
		}
		for (j = 0; ((*record2_p)->address)[j] != END_OF_ARR_HEX; j++)
		{
		}
		if (i != j)
		{
			pass_fail++;
			fprintf(w_file_p, "[ERROR - Address/Count size] - Actual [%d byte] - Expect: [%d byte]\n", i / 2, j / 2);/*report*/
		}
		/*compare value*/
		else
		{
			for (i = 0; ((*record1_p)->address)[i] != END_OF_ARR_HEX; i++)
			{
				if (((*record1_p)->address)[i] != ((*record2_p)->address)[i])
				{
					pass_fail++;
					fprintf(w_file_p, "[ERROR - Address/Count location %d] - Actual [0x%X] - Expect: [0x%X]\n",
						i, ((*record1_p)->address)[i], ((*record2_p)->address)[i]);/*report*/
				}
			}
		}
	}
	/*compare data*/
	if ((((*record1_p)->data) == NULL) && (((*record2_p)->data) != NULL))
	{
		pass_fail++;
		fprintf(w_file_p, "[ERROR - Data] - Actual [Empty] - Expect: [Not Empty]\n");/*report*/
	}
	else if ((((*record1_p)->data) != NULL) && (((*record2_p)->data) == NULL))
	{
		pass_fail++;
		fprintf(w_file_p, "[ERROR - Data] - Actual [Not Empty] - Expect: [Empty]\n");/*report*/
	}
	else if ((((*record1_p)->data) != NULL) && (((*record2_p)->data) != NULL))
	{
		int i;
		int j;
		/*compare size*/
		for (i = 0; ((*record1_p)->data)[i] != END_OF_ARR_HEX; i++)
		{
		}
		for (j = 0; ((*record2_p)->data)[j] != END_OF_ARR_HEX; j++)
		{
		}
		if (i != j)
		{
			pass_fail++;
			fprintf(w_file_p, "[ERROR - Data size] - Actual [%d byte] - Expect: [%d byte]\n", i / 2, j / 2);/*report*/
		}
		/*compare value*/
		else
		{
			for (i = 0; ((*record1_p)->data)[i] != END_OF_ARR_HEX; i++)
			{
				if (((*record1_p)->data)[i] != ((*record2_p)->data)[i])
				{
					pass_fail++;
					fprintf(w_file_p, "[ERROR - Data location %d] - Actual [0x%X] - Expect: [0x%X]\n",
						i, ((*record1_p)->data)[i], ((*record2_p)->data)[i]);/*report*/
				}
			}
		}
	}
	/*compare checksum*/
	if ((((*record1_p)->checkSum) == NULL) && (((*record2_p)->checkSum) != NULL))
	{
		pass_fail++;
		fprintf(w_file_p, "[ERROR - Check Sum] - Actual [Empty] - Expect: [Not Empty]\n");/*report*/
	}
	else if ((((*record1_p)->checkSum) != NULL) && (((*record2_p)->checkSum) == NULL))
	{
		pass_fail++;
		fprintf(w_file_p, "[ERROR - Check Sum] - Actual [Not Empty] - Expect: [Empty]\n");/*report*/
	}
	else if ((((*record1_p)->checkSum) != NULL) && (((*record2_p)->checkSum) != NULL))
	{
		int i;
		int j;
		/*compare size*/
		for (i = 0; ((*record1_p)->checkSum)[i] != END_OF_ARR_HEX; i++)
		{
		}
		for (j = 0; ((*record2_p)->checkSum)[j] != END_OF_ARR_HEX; j++)
		{
		}
		if (i != j)
		{
			pass_fail++;
			fprintf(w_file_p, "[ERROR - Check Sum size] - Actual [%d byte] - Expect: [%d byte]\n", i / 2, j / 2);/*report*/
		}
		/*compare value*/
		else
		{
			for (i = 0; ((*record1_p)->checkSum)[i] != END_OF_ARR_HEX; i++)
			{
				if (((*record1_p)->checkSum)[i] != ((*record2_p)->checkSum)[i])
				{
					pass_fail++;
					fprintf(w_file_p, "[ERROR - Check Sum location %d] - Actual [0x%X] - Expect: [0x%X]\n",
						i, ((*record1_p)->checkSum)[i], ((*record2_p)->checkSum)[i]);/*report*/
				}
			}
		}
	}
	/*--------------------------Passed| Failed \\n--------------------------*/

	if (pass_fail == 0)/*pass*/
	{
		g_passed++;
		fprintf(w_file_p, "-> passed\n\n");
	}
	else/*failed*/
	{
		g_failed++;
		fprintf(w_file_p, "-> failed\n\n");
	}

	/*--------------------------Renew \\n--------------------------*/
	/*Free and Renew*/
	sr_Free(record1_p);
	sr_Free(record2_p);
	(*record1_p) = sr_Init();
	(*record2_p) = sr_Init();
	if ((*record1_p) == NULL)
	{
		return -2;
	}
	if ((*record2_p) == NULL)
	{
		return -2;
	}
	/*renew success*/
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
	printf("Read file path: %s\n", file_name);
	return 0;
}
static int check_file_invalid(int argc, char** argv)
{
	printf("Number parameter: [%d]\n", argc - 1);
	if (argc == 1)
	{
		printf("Process default: - Actual   Record [%s]\n", S_RECORD_DEFAULT);
		printf("Process default: - Expected Record [%s]\n", S_RECORD_COMPARE_DEFAULT);
		return 0;
	}
	else if (argc == 2)
	{
		if (strcmp(argv[1], MENU_HELP))
		{
			printf("[ERROR - %s] - Number of file input is less than 2\n", __FUNCTION__);
			printf("[ERROR - %s] - Type 'Program.exe  1' for help\n", __FUNCTION__);
			return -1;/*exit return error*/
		}
		help();
		return 1;/*exit return 0*/
	}
	else if (argc == 3)
	{
		printf("Path input: - Actual   Record [%s]\n", argv[1]);
		printf("Path input: - Expected Record [%s]\n", argv[2]);
		return 2;
	}
	else if (argc > 3)
	{
		printf("[ERROR - %s] - Number of file input is bigger than 2\n", __FUNCTION__);
		printf("[ERROR - %s] - Type 'Program.exe  1' for help\n", __FUNCTION__);
		return -2;
	}
}
static void help()
{
	printf("[HELP]\n");
	printf("1. Syntax:\n");
	printf(" - Check S Record file with expected S Record: Program.exe  Actual_S_REC.txt  Expected_S_REC.txt\n");
	printf(" - Help_____________________________________ : Program.exe  1\n");
	printf(" - run default S-record file________________ : Program.exe\n");
	printf("2. Exception error: \n");
	printf(" - Number of file input is greater than 2\n");
	printf(" - Number of file input is less than 2\n");
	printf(" - File name not found\n");
	printf("____________________________________________________________________\n");
}
int main(int argc, char* argv[])
{
	/*
	* [Local Variables]
	* - 2 ret to get return code
	* - 2 S record to save data | 1 for actual, 2 for expected
	* - 2 [r mode] File pointer and cursor
	* - add new [W+ mode] pointer write output.txt
	* - 1 numerical order to print s rec
	*/
	int ret1;
	Srec* record1 = sr_Init();
	FILE* file_p1 = NULL;
	long int cur1 = 0;

	int ret2;
	Srec* record2 = sr_Init();
	FILE* file_p2 = NULL;
	long int cur2 = 0;

	FILE* w_file_p = NULL;
	uint32_t num_order;
	/*
	* [open stream file]
	* - check number argc
	* - Try open file with read mode
	*/
	ret1 = check_file_invalid(argc,argv);
	if (ret1 < 0)
	{
		return -1;/*input error*/
	}
	else if(ret1 == 0)/*default record*/
	{
		if (open_to_read(&file_p1, S_RECORD_DEFAULT))
		{
			return -2;/*open failed*/
		}
		if (open_to_read(&file_p2, S_RECORD_COMPARE_DEFAULT))
		{
			return -2;
		}
	}
	else if (ret1 == 1)/*help*/
	{
		return 0;
	}
	else if (ret1 == 2)
	{
		if (open_to_read(&file_p1, argv[1]))
		{
			return -3;/*open failed*/
		}
		if (open_to_read(&file_p2, argv[2]))
		{
			return -3;
		}
	}
	/*[Open TestReport.txt to save data]
	* open with 'w+' mode
	* clear, read, write.
	*/
	w_file_p = file_Open(S_REC_REPORT, "w+");
	if (w_file_p == NULL)
	{
		return -4;
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
	ret1 = 1;
	ret2 = 1;
	while((ret1 != 0)&&(ret2 != 0))
	{
		ret1 = check_line_Record(record1, file_p1, &cur1);
		ret2 = check_line_Record(record2, file_p2, &cur2);
		/*if 1 of 2 record can't initialize*/
		if((ret1 == -1) || (ret2 == -1))
		{
			/*get string S rec failed*/
			return -5;
		}
		/*else if 1 of 2 file is EOF -> end, not read EOF*/
		else if ((ret1 == 0) || (ret2 == 0))
		{
			if (ret1 == 0)
			{
				printf("[%s - Actual] Read EOF\n", __FUNCTION__);
			}
			else
			{
				printf("[%s - Actual] Not read EOF\n", __FUNCTION__);
			}
			if (ret2 == 0)
			{
				printf("[%s - Expected] Read EOF\n", __FUNCTION__);
			}
			else
			{
				printf("[%s - Expected] Read EOF\n", __FUNCTION__);
			}
		}
		/*else save data s record to report
		* no matter right or wrong
		* - line 1 : s record 1
		* - line 2 : s record 2
		* - add optional line, if error
		* - add an empty line
		* 
		* print to console and renew 2 Record
		*/
		else
		{
			if (log2_RecordToText(&record1, &record2, w_file_p, num_order))
			{
				return -6;/*log failed*/
			}
		}
		num_order++;
		printf("____________________________________________________________________\n");
	}
	/*[write a function to statistics]}*/
	print_total(w_file_p);
	printf("[END] SAVED REPORT: %s\n", S_REC_REPORT);
	printf("[END] CHECK DONE\n");

	/*free alloc struct S record*/
	free(record1);/*free because of member is null*/
	free(record2);/*free because of member is null*/
	/*free file pointer*/
	if (file_Close(w_file_p))
	{
		return -9;
	}
	if (file_Close(file_p1))
	{
		return -10;
	}
	if (file_Close(file_p2))
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