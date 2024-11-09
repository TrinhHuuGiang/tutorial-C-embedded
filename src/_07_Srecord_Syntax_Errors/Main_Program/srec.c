/*
    <srec.c>: <Srecord error check functions>
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
#include "srec.h"
#include "filehandle.h"

/*******************************************************************************
* Code
******************************************************************************/
ErrCode sr_Check_CheckSumValue(Srec* record, char* line, uint32_t size, FILE* file_p)
{
	uint32_t sum, actual_sum;
	/*Save "check sum" from string*/
	record->checkSum = (uint32_t*)malloc(3 * sizeof(uint32_t));
	if ((record->checkSum) == NULL)
	{
		printf("[ERROR - %s] - Allocate Failed\n", __FUNCTION__);
		return S_MALLOC_FAILED;
	}
	(record->checkSum)[0] = file_CHARtoINT(line[size - 2]);
	(record->checkSum)[1] = file_CHARtoINT(line[size - 1]);
	(record->checkSum)[2] = END_OF_ARR_HEX;
	actual_sum = file_2HEXtoINT(record->checkSum[0], record->checkSum[1]);
	/*Compare actual "sum" with "check sum"*/
	/*sum = 1byte count + 2/3/4byte address + (dataLen/2) byte*/
	/*sum = 1 byte count*/
	sum = file_2HEXtoINT(record->byteCount[0], record->byteCount[1]);
	/*sum += 2/3/4byte address*/
	for (int i = 0; (record->address)[i] != END_OF_ARR_HEX; i++)
	{
		sum += file_2HEXtoINT((record->address)[i], (record->address)[i + 1]);
		i++;
	}
	/*sum += 2/3/4byte datalen/2*/
	/*data may be empty*/
	if (record->dataLen != 0)
	{
		for (int i = 0; (record->data)[i] != END_OF_ARR_HEX; i++)
		{
			sum += file_2HEXtoINT((record->data)[i], (record->data)[i + 1]);
			i++;
		}
	}
	/*one complement*/
	sum = ~sum;
	/*Get LSB*/
	sum &= 0xFF;
	/*compare*/
	if (sum != actual_sum)
	{
		printf("[ERROR - %s] - Actual checksum:    [0x%X]\n", __FUNCTION__, actual_sum);
		printf("[ERROR - %s] - Calculate checksum: [0x%X]\n", __FUNCTION__, sum);
		return S_CHECSUM_INVALID;
	}
	return S_OK;
}
ErrCode sr_Check_DataSize(Srec* record, char* line, uint32_t size, FILE* file_p)
{
	int count = size - 2 - 2;/*ignore S+type+byte count*/
	int i;
	if (((record->type) == S0) || ((record->type) == S1) || ((record->type) == S5) || ((record->type) == S9))
	{
		count -= (4+2);
	}
	else if (((record->type) == S2) || ((record->type) == S6) || ((record->type) == S8))
	{
		count -= (6+2);
	}
	else /*else if S3 or S5*/
	{
		count -= (8+2);
	}
	/*if
	* count == -2 -> end of s rec -> stop
	* count == 0 -> data = 0 byte, check sum = 1 byte -> S0 end not null
	* count > 0 -> data > 0 byte, check sum = 1 byte ->S0 end not null, S1/S2/S3 data > 32 byte, S5/S6/S7/S8/S9 isn't 0
	*/
	if (count == -2)
	{
		printf("[ERROR - %s] - Missing Checksum\n", __FUNCTION__);
		return S_MISSING_CHECKSUM;
	}
	else if (count == 0)
	{
		if ((record->type) == S0)
		{
			printf("[ERROR - %s] - S0 data is empty\n", __FUNCTION__);
			return S_DATA_ENDNOTNULL;
		}
	}
	else if (count > 0)
	{
		/*save data into data*/
		/*convert to hex and save into data*/
		record->data = (unsigned char*)malloc((count+1) * sizeof(unsigned char));
		if ((record->data) == NULL)/*if allocate failed*/
		{
			record->dataLen = 0;
			printf("[ERROR - %s] - Allocate Failed\n", __FUNCTION__);
			return S_MALLOC_FAILED;
		}
		record->dataLen = count;
		i = 0;
		if (((record->type) == S0) || ((record->type) == S1) || ((record->type) == S5) || ((record->type) == S9))
		{
			for ( i ; i < count; i++)
			{
				(record->data)[i] = file_CHARtoINT(line[8 + i]);/*2 char type, 2 char byte count, 4 char address*/
			}
		}
		else if (((record->type) == S2) || ((record->type) == S6) || ((record->type) == S8))
		{
			for ( i ; i < count; i++)
			{
				(record->data)[i] = file_CHARtoINT(line[10 + i]);/*2 char type, 2 char byte count, 6 char address*/
			}
		}
		else /*else if S3 or S5*/
		{
			for ( i ; i < count; i++)
			{
				(record->data)[i] = file_CHARtoINT(line[12 + i]);/*2 char type, 2 char byte count, 8 char address*/
			}
		}
		/*add end of arr*/
		(record->data)[i] = END_OF_ARR_HEX;
		/*check size s5-s9*/
		if (((record->type) == S5) || ((record->type) == S6) || ((record->type) == S7) || ((record->type) == S8) || ((record->type) == S9))
		{
			printf("[ERROR - %s] - Data is [%d] byte\n", __FUNCTION__,count/2);
			printf("[ERROR - %s] - Data from S5 to S9 must be empty\n", __FUNCTION__);
			return S_DATA_BIGGER0;
		}
		/*check size s1-s3*/
		else if (((record->type) == S1) || ((record->type) == S2) || ((record->type) == S3))
		{
			if ((count/2) > 32)
			{
				printf("[ERROR - %s] - Data from S1 to S3 is [%d] byte - must be less than 32 byte\n", __FUNCTION__, count / 2);
				printf("[ERROR - %s] - Data from S1 to S3 must be less than 32 byte\n", __FUNCTION__);
				return S_DATA_BIGGER32;
			}
		}
		/*re-check S0 end is null*/
		else if ((record->type) == S0)
		{
			if (((record->data)[count - 1] != 0) || ((record->data)[count - 2] != 0))
			{
				printf("[ERROR - %s] - S0 data end [0x%X%X] not null\n", __FUNCTION__, (record->data)[count - 2], (record->data)[count - 1]);
				return S_S0_ADDRESS_NOTNULL;
			}
		}
	}
	return S_OK;
}
ErrCode sr_Check_S0Address(Srec* record, char* line, uint32_t size, FILE* file_p)
{
	if (record->type == S0)
	{
		for (int i = 0; (record->address)[i] != END_OF_ARR_HEX; i++)
		{
			if ((record->address)[i] != 0)
			{
				printf("[ERROR - %s] - S0 address is not 0x0000\n", __FUNCTION__);
				return S_S0_ADDRESS_NOTNULL;
			}
		}
	}
	return S_OK;
}
ErrCode sr_Check_NumberRecord(Srec* record, char* line, uint32_t size, FILE* file_p)
{
	uint32_t num_sr = 0;
	int count_S5_6;
	/*count number S1/S2/S3 in file txt*/
	if (((record)->type == S5) || ((record)->type == S6))
	{
		num_sr = file_NumRecord(file_p, 1) + file_NumRecord(file_p, 2) + file_NumRecord(file_p, 3);
	}
	if ((record->type) == S5)
	{
		count_S5_6 = file_4HEXtoINT((record->address)[0], (record->address)[1], (record->address)[2], (record->address)[3]);
		/*convert 4 hex to int*/
		if (count_S5_6 != num_sr)
		{
			printf("[ERROR - %s] - Type [S5 - Calculate] : Number of [S1+S2+S3] = [0x%X]\n", __FUNCTION__, num_sr);
			printf("[ERROR - %s] - Type [S5 - Actual   ] : Count  of [   S5   ] = [0x%X]\n", __FUNCTION__, count_S5_6);
			return S_NUM_SR_MISMATCH;
		}
	}
	else if ((record->type) == S6)
	{
		/*convert 6 hex to int*/
		count_S5_6 = file_6HEXtoINT((record->address)[0], (record->address)[1], (record->address)[2],
			(record->address)[3], (record->address)[4], (record->address)[5]);
		if (count_S5_6 != num_sr)
		{
			printf("[ERROR - %s] - Type [S6 - Calculate] : Number of [S1+S2+S3] = [0x%X]\n", __FUNCTION__, num_sr);
			printf("[ERROR - %s] - Type [S6 - Actual   ] : Count  of [   S6   ] = [0x%X]\n", __FUNCTION__, count_S5_6);
			return S_NUM_SR_MISMATCH;
		}
	}
	return S_OK;
}
ErrCode sr_Check_AddressSize(Srec* record, char* line, uint32_t size, FILE* file_p)
{
	int count = size - 2 - 2;
	int i;
	/*count = size - (s + type) - byte count*/
	if (count == 0)
	{
		printf("[ERROR - %s] - Address is empty\n", __FUNCTION__);
		printf("[ERROR - %s] - End of S record\n", __FUNCTION__);
		return S_ADDRSIZE_INVALID;
	}
	/*save address/ count into address (size malloc = 8+1(32 bit + 1 for End of arr)*/
	record->address = (uint32_t*)malloc(9 * sizeof(uint32_t));
	if ((record->address) == NULL)
	{
		printf("[ERROR - %s] - Allocate Failed\n", __FUNCTION__);
		return S_MALLOC_FAILED;
	}
	/*check cases size and add value*/
	if (((record->type) == S0) || ((record->type) == S1) || ((record->type) == S5) || ((record->type) == S9))
	{
		for (i = 4; (i < 8) && (line[i] != '\0'); i++)
		{
			(record->address)[i - 4] = file_CHARtoINT(line[i]);
			if ((line[i + 1] == '\0') && ((i + 1) < 8))
			{
				(record->address)[i - 3] = END_OF_ARR_HEX;
				printf("[ERROR - %s] - Address size [%d] < 2 byte with S0,1,5,9\n", __FUNCTION__,
					count/2);
				return S_ADDRSIZE_INVALID;
			}
		}
	}
	else if (((record->type) == S2) || ((record->type) == S6) || ((record->type) == S8))
	{
		for (i = 4; (i < 10) && (line[i] != '\0'); i++)
		{
			(record->address)[i - 4] = file_CHARtoINT(line[i]);
			if ((line[i + 1] == '\0') && ((i + 1) < 10))
			{
				(record->address)[i - 3] = END_OF_ARR_HEX;
				printf("[ERROR - %s] - Address size [%d] < 3 byte with S2,6,8\n", __FUNCTION__,
					count / 2);
				return S_ADDRSIZE_INVALID;
			}
		}
	}
	else /*else if S3 or S5*/
	{
		for (i = 4; (i < 12) && (line[i] != '\0'); i++)
		{
			(record->address)[i - 4] = file_CHARtoINT(line[i]);
			if ((line[i + 1] == '\0') && ((i + 1) < 12))
			{
				(record->address)[i - 3] = END_OF_ARR_HEX;
				printf("[ERROR - %s] - Address size [%d] < 4 byte with S3,5\n", __FUNCTION__,
					count / 2);
				return S_ADDRSIZE_INVALID;
			}
		}
	}
	/*add End of arr hex*/
	(record->address)[i - 4] = END_OF_ARR_HEX;
	return S_OK;
}
ErrCode sr_Check_ByteCountMatching(Srec* record, char* line, uint32_t size, FILE* file_p)
{
	/*count (byte) = (size - 2 char type - 2 char byte count)/2*/
	int count = (size - 2 - 2) / 2;
	int byte_count = file_2HEXtoINT(record->byteCount[0], record->byteCount[1]);
	if (count != byte_count)
	{
		printf("[ERROR - %s] - Byte count [0x%X] mismatch with number of bytes remaining [0x%X]\n", __FUNCTION__,byte_count, count);
		return S_BYTECOUNT_MISMATCH;
	}
	return S_OK;
}
ErrCode sr_Check_ByteCountMin(Srec* record, char* line, uint32_t size, FILE* file_p)
{
	int byte_count;
	int line2, line3;
	/*if byte count empty = end of s record*/
	if ((size - 2) < 2)
	{
		printf("[ERROR - %s] - Byte count empty\n", __FUNCTION__);
		printf("[ERROR - %s] - End of S record\n", __FUNCTION__);
		return S_BYTECOUNT_EMPTY;
	}

	/*if anything ok, save value line[2], line[3] into byte count*/
	record->byteCount = (uint32_t*)malloc(3 * sizeof(uint32_t));
	if ((record->byteCount) == NULL)
	{
		printf("[ERROR - %s] - Allocate Failed\n", __FUNCTION__);
		return S_MALLOC_FAILED;
	}
	else
	{
		line2 = file_CHARtoINT(line[2]);
		line3 = file_CHARtoINT(line[3]);
		record->byteCount[0] = line2;
		record->byteCount[1] = line3;
		record->byteCount[2] = END_OF_ARR_HEX;
	}

	/*check cases*/
	byte_count = file_2HEXtoINT(line2, line3);
	if (((record->type) == S0)||((record->type)==S1)|| ((record->type) == S5)|| ((record->type) == S9))
	{
		if (byte_count < 3)
		{
			printf("[ERROR - %s] - BYTE COUNT [0x%X%X] = int [%d] < 3 byte with S0,1,5,9\n", __FUNCTION__,
				line2, line3, byte_count);
			return S_BYTECOUNT_INVALID;
		}
	}
	else if(((record->type) == S2) || ((record->type) == S6) || ((record->type) == S8))
	{
		if (byte_count < 4)
		{
			printf("[ERROR - %s] - BYTE COUNT [0x%X%X] = int [%d] < 4 byte with S2,6,8\n", __FUNCTION__,
				line2, line3, byte_count);
			return S_BYTECOUNT_INVALID;
		}
	}
	else /*else if S3 or S5*/
	{
		if (byte_count < 5)
		{
			printf("[ERROR - %s] - BYTE COUNT [0x%X%X] = int [%d] < 5 byte with S3,5\n", __FUNCTION__,
				line2, line3, byte_count);
			return S_BYTECOUNT_INVALID;
		}
	}
	return S_OK;
}
ErrCode sr_Check_S4Type(Srec* record, char* line, uint32_t size, FILE* file_p)
{
	if (file_CHARtoINT(line[1]) == 4)
	{
		printf("[ERROR - %s] - S4 type reserved\n", __FUNCTION__);
		/*save rest into data*/
		record->data = (unsigned char*)malloc((size - 1) * sizeof(unsigned char));
		if ((record->data) == NULL)/*if allocate failed*/
		{
			record->dataLen = 0;
			printf("[ERROR - %s] - Allocate Failed\n", __FUNCTION__);
			return S_MALLOC_FAILED;
		}
		else/*save into data*/
		{
			record->dataLen = size - 2;
			for (int i = 2; i < size; i++)
			{
				(record->data)[i - 2] = file_CHARtoINT(line[i]);
			}
			(record->data)[size - 2] = END_OF_ARR_HEX;
		}
		return S_4_RESERVED;
	}
	return S_OK;
}
ErrCode sr_Check_SizeMin(Srec* record, char* line, uint32_t size, FILE* file_p)
{
	if (size < 10)
	{
		printf("[ERROR - %s] - Number character is [%u] < 10\n", __FUNCTION__, size);
		return S_SIZEMIN_INVALID;
	}
	return S_OK;
}
ErrCode sr_Check_EvenSize(Srec* record, char* line, uint32_t size, FILE* file_p)
{
	if ((size % 2) != 0)
	{
		printf("[ERROR - %s] - Size of record [%u] HEX - not even\n", __FUNCTION__, size);
		/*convert to hex and save into data*/
		record->data = (unsigned char*)malloc((size - 1) * sizeof(unsigned char));
		if ((record->data) == NULL)/*if allocate failed*/
		{
			record->dataLen = 0;
			printf("[ERROR - %s] - Allocate Failed\n", __FUNCTION__);
			return S_MALLOC_FAILED;
		}
		else/*save into data*/
		{
			record->dataLen = size - 2;
			for (int i = 2; i < size; i++)
			{
				(record->data)[i-2] = file_CHARtoINT(line[i]);
			}
			(record->data)[size - 2] = END_OF_ARR_HEX;
		}
		return S_SIZEEVEN_INVALID;
	}
	/*don't save and return*/
	return S_OK;
}
ErrCode sr_Check_Type(Srec* record, char* line, uint32_t size, FILE* file_p)
{
	if (size <= 0)
	{
		printf("[ERROR - %s] - Empty input string\n", __FUNCTION__);
		return S_TYPE_INVALID;
	}
	else if (line[0]!='S')
	{
		printf("[ERROR - %s] - Record start [%c] isn't [S]\n", __FUNCTION__, line[0]);
		record->type = NaS;
		return S_TYPE_INVALID;
	}
	else if (size == 1)
	{
		printf("[ERROR - %s] - Input string [S] only\n", __FUNCTION__);
		return S_TYPE_INVALID;
	}
	else if (size > 1)
	{
		int line1 = file_CHARtoINT(line[1]);
		if (line1 < 0 || line1 > 9)
		{
			record->type = NaS;
			printf("[ERROR - %s] - [%c%c] is not a S-record\n", __FUNCTION__, line[0], line[1]);
			return S_TYPE_INVALID;
		}
		else
		{
			record->type = line1;
		}
	}
	if (size == 2)
	{
		printf("[ERROR - %s] - Input string [%c%c] only\n", __FUNCTION__, line[0], line[1]);
		return S_TYPE_INVALID;
	}
	/*size > 2*/
	else
	{
		/*check hexadecimal*/
		for (int i = 2; i < size; i++)
		{
			if (file_CHARtoINT(line[i]) == -1)
			{
				printf("[ERROR - %s] - character [%c] at position [%d] is not HEX\n", __FUNCTION__, line[i], i);
				return S_TYPE_INVALID;
			}
		}
	}
	/*if any rest is HEX -> check ok*/
	return S_OK;
}
Srec* sr_Init()
{
	Srec* temp = (Srec*)malloc(sizeof(Srec));
	if (temp != NULL)
	{
		temp->type = NaS;
		temp->byteCount = NULL;
		temp->address = NULL;
		temp->dataLen = 0;
		temp->data = NULL;
		temp->checkSum = NULL;
	}
	else
	{
		printf("[ERROR - %s] - ALLOCATE FAILED\n",__FUNCTION__);
	}
	return temp;
}
void sr_Free(Srec** p_SR)
{
	/*free member*/
	if (((*p_SR)->byteCount) != NULL)
	{
		free((*p_SR)->byteCount);
		(*p_SR)->byteCount = NULL;
	}
	if (((*p_SR)->address) != NULL)
	{
		free((*p_SR)->address);
		(*p_SR)->address = NULL;
	}
	if (((*p_SR)->data) != NULL)
	{
		free((*p_SR)->data);
		((*p_SR)->data) = NULL;
	}
	if (((*p_SR)->checkSum) != NULL)
	{
		free((*p_SR)->checkSum);
		((*p_SR)->checkSum) = NULL;
	}
	/*free struct*/
	if ((*p_SR) != NULL)
	{
		free(*p_SR);
		*p_SR = NULL;
	}
}

/*
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */