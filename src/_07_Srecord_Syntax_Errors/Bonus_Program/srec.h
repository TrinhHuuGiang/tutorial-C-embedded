/*
    Copyright (C) <2024> <Giang Trinh>

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
*/

#ifndef _SREC_H_
#define _SREC_H_

/*******************************************************************************
* Definitions
******************************************************************************/
#include <stdint.h>
#define END_OF_ARR_HEX (16)
/*Function return error code*/
typedef enum e_ErrCode
{
	/*No problem*/
	S_OK = 0,
	/*[CHECK TYPE]
	* CHECK TYPE _ Not be a S record
	* error:
	* - start is not S + 0,9
	* - the rest exist one is not a hexadecimal
	* action:
	* - save "Type" is NaS (Not a S record)
	* - stop
	*/
	S_TYPE_INVALID = -1,
	/*[CHECK TYPE]
	* CHECK IF NUM CHAR IS ODD
	* error:
	* - number char is odd
	* action:
	* - save rest in data
	* - stop
	*/
	S_SIZEEVEN_INVALID = -2,
	/*[CHECK TYPE]
	* CHECK SIZE MINIMUM OF S RECORD
	* at least have S + type + 1byte count + 2byte ADDR + 1byte SUM
	* - Size min = 2 u_char + 2 u_char + 4 u_char + 2 u_char = 10 u_char
	* error:
	* - Size min <10;
	* action:
	* - continue
	*/
	S_SIZEMIN_INVALID = -3,
	/*[CHECK TYPE]
	* - CHECK IF TYPE IS S4
	* action:
	* stop and save the rest in data
	*/
	S_4_RESERVED = -4,
	/*[CHECK BYTE COUNT]
	* CHECK BYTE COUNT
	* Error:
	* - nothing is read -> empty
	* - mismatch if <03 byte with S0, S1,S5,S9 (2 - ADDR, 1 - CHECK SUM) -> invalid
	* - mismatch if <04 byte with S2,S6,S8 (3 - ADDR, 1 - CHECK SUM) -> invalid
	* - mismatch if <05 byte with S3,S7 (4 - ADDR, 1 - CHECK SUM) -> invalid
	* action:
	* - stop if nothing is read
	* - else continue if can save a value to "byte count"
	*/
	S_BYTECOUNT_EMPTY = -5,
	S_BYTECOUNT_INVALID = -6,
	/*
	* [CHECK BYTE COUNT]
	* compare byte count with the rest number bytes
	* error:
	* - check the rest of byte if not matching -> mismatch
	* action:
	* - continue
	*/
	S_BYTECOUNT_MISMATCH = -7,
	/*[CHECK ADDRESS/COUNT]
	* CHECK BYTE ADDRESS/COUNT SIZE
	* size: S0,S1,S5,S9 (16bit)/ S2,S6,S8 (24bit)/ S3, S7 (32bit)
	* error:
	* - ADDRESS/COUNT < size -> invalid
	* action:
	* stop if nothing is read or number read < size
	*/
	S_ADDRSIZE_INVALID = -8,
	/*[CHECK ADDRESS/COUNT]
	* CHECK COUNT NUMBER S REC TYPE [S1/S2/S3]
	* only for [S5, S6] to check number
	* error:
	* - sum of line s_rec != count at address (S5/S6) -> mismatch
	* action:
	* - continue
	*/
	S_NUM_SR_MISMATCH = -9,
	/*
	* [CHECK ADDRESS/COUNT]
	* CHECK ADDRESS S0 is 0x0000
	* only for S0
	* error:
	* - address != 0x0000 -> not null
	* action:
	* - continue
	*/
	S_S0_ADDRESS_NOTNULL = -10,
	/*
	* [CHECK DATA]
	* CHECK NUMBER BYTE OF DATA
	* number byte data can >=0
	* n byte = after 'address' to 'end of line' - 1byte check sum
	* so:
	* n = -1 missing checksum
	* n = 0  data empty
	* n > 0  exist data
	* error:
	* - S1/S2/S3 if data > 32byte -> bigger32
	* - S0 end of data is not a null (size data unlimited) -> end not null
	* - S5/S6/S7/S8/S9 data isn't 0 -> bigger0
	* action:
	* - end if checksum is missing
	* - else save data if exist into data, continue
	* - else continue if data empty
	*/
	S_MISSING_CHECKSUM = -11,
	S_DATA_BIGGER32 = -12,
	S_DATA_ENDNOTNULL = -13,
	S_DATA_BIGGER0 = -14,
	/*
	* [CHECK SUM]
	* error:
	* - checksum empty -> missing
	* - checksum wrong -> invalid
	* action:
	* stop to print S rec error
	*/
	S_CHECSUM_INVALID = -15,
	/*
	* [MALLOC]
	* - a Malloc failed
	* - stop
	*/
	S_MALLOC_FAILED = -16
} ErrCode;
/*10 Type of S record*/
typedef enum e_SrecType
{
	NaS = -1, S0 = 0, S1, S2, S3, S4, S5, S6, S7, S8, S9
} SrecType;
/*S record structure*/
typedef struct t_S_rec
{
	SrecType type;
	uint32_t* byteCount;
	uint32_t* address;
	uint32_t dataLen;
	unsigned char* data;
	uint32_t* checkSum;
} Srec;
/*function pointer*/
typedef ErrCode(*fp_SR)(Srec*, char*, uint32_t, FILE*);

/*******************************************************************************
* API
******************************************************************************/
/*Initialize and free a S record struct*/
Srec* sr_Init();
void sr_Free(Srec** p_SR);
/*
* ----Function sr_Check_Type----
* To get value for a struct S record
* [Always execute first from sr_Check_Type -> sr_Check_CheckSumValue]
*/
ErrCode sr_Check_Type(Srec* record, char* line, uint32_t size, FILE* file_p);
ErrCode sr_Check_EvenSize(Srec* record, char* line, uint32_t size, FILE* file_p);
ErrCode sr_Check_SizeMin(Srec* record, char* line, uint32_t size, FILE* file_p);
ErrCode sr_Check_S4Type(Srec* record, char* line, uint32_t size, FILE* file_p);
ErrCode sr_Check_ByteCountMin(Srec* record, char* line, uint32_t size, FILE* file_p);
ErrCode sr_Check_ByteCountMatching(Srec* record, char* line, uint32_t size, FILE* file_p);
ErrCode sr_Check_AddressSize(Srec* record, char* line, uint32_t size, FILE* file_p);
ErrCode sr_Check_NumberRecord(Srec* record, char* line, uint32_t size, FILE* file_p);
ErrCode sr_Check_S0Address(Srec* record, char* line, uint32_t size, FILE* file_p);
ErrCode sr_Check_DataSize(Srec* record, char* line, uint32_t size, FILE* file_p);
ErrCode sr_Check_CheckSumValue(Srec* record, char* line, uint32_t size, FILE* file_p);

#endif /* _SREC_H_ */

/*
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */