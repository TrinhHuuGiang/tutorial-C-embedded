/*
    <macro_bit.c>: <Simulate how to set, clear, convert bit of register>.
    Copyright (C) <2024> <Giang Trinh>

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
*/

/*******************************************************************************
* Definitions
******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#define n 3/*start bit from LBS(right) to MSB(left)*/
#define REG_BIT_SET32(add,n) ((*((uint32_t*)(add)))|=(1U<<(n)))
         /* 'set a single bit'
         ép kiểu add thành con trò uint 32bit
         sau đó lấy giá trị thanh ghi add trỏ đến
         or giá trị đó với 0b1 dịch trí n vị trí
         để set bit n độ rộng 1 lên 1
         *add = (*add)|(1u<<n)
         */
#define REG_BIT_CLEAR32(add,n) ((*((uint32_t*)(add)))&=(~(1U<<(n))))
         /* 'clear a single bit'
         ép kiểu add thành con trò uint 32bit
         sau đó lấy giá trị thanh ghi add trỏ đến
         and giá trị đó với đảo của (0b1 dịch trí n vị trí)
         để set bit n độ rộng 1 thành 0
         *add = (*add)&(~(1u<<n))
         */
#define SWAP_BYTE32(add) (\
    (*((uint32_t*)(add))) = \
    (((*((uint32_t*)(add))) & 0xFF000000) >> 24) |\
    (((*((uint32_t*)(add))) & 0x00FF0000) >> 8) |\
    (((*((uint32_t*)(add))) & 0x0000FF00) << 8) |\
    (((*((uint32_t*)(add))) & 0x000000FF) << 24)\
    )
         /* 'convert 4byte from bigendian to little endian'
         Mỗi 2 số hexa tương ứng với 1 byte
         *add = abcd theo bigendian thì byte cao nhất (MSB) là a sẽ lưu ở địa chỉ thấp nhất
         Chuyển lần lượt từng byte giá trị cao xuống thấp của *add thành giá trị thấp lên cao
         *add (after) = dcba
         *add & với 0xFF000000 và dịch trái 3 byte lấy ra được giá trị byte thứ 4
         tương tự byte 3 dịch xuống vị trí 2, byte 2 tiến lên vị trí byte 3
         byte 1 tiến lên vị trí byte 4
         */
#define SWAP_BYTE16(add) (\
    (*((uint16_t*)(add))) = \
    (((*((uint16_t*)(add))) & 0xFF00) >> 8) |\
    (((*((uint16_t*)(add))) & 0x00FF) << 8)\
    )/* 'convert 2byte from bigendian to little endian'*/
#define BIT1 ((uint8_t)1)
#define BIT0 ((uint8_t)0)
#define BYTE_TO_BINARY(byte)  (\
    printf(" %u%u%u%u%u%u%u%u ",\
    ((byte) & 0x80 ? BIT1 : BIT0),\
    ((byte) & 0x40 ? BIT1 : BIT0),\
    ((byte) & 0x20 ? BIT1 : BIT0),\
    ((byte) & 0x10 ? BIT1 : BIT0),\
    ((byte) & 0x08 ? BIT1 : BIT0),\
    ((byte) & 0x04 ? BIT1 : BIT0),\
    ((byte) & 0x02 ? BIT1 : BIT0),\
    ((byte) & 0x01 ? BIT1 : BIT0))\
    )/*in ra byte dạng 8bit*/
#define INT32_TO_BINARY(num) {\
    printf("BINARY32 = [");\
    BYTE_TO_BINARY((num&0xFF000000)>>24);\
    BYTE_TO_BINARY((num&0x00FF0000)>>16);\
    BYTE_TO_BINARY((num&0x0000FF00)>>8);\
    BYTE_TO_BINARY((num&0x000000FF));\
    printf("]");\
    }/*in ra byte dạng 32bit*/
#define INT16_TO_BINARY(num) {\
    printf("BINARY16 = [");\
    BYTE_TO_BINARY((num&0xFF00)>>8);\
    BYTE_TO_BINARY((num&0x00FF));\
    printf("]");\
    }/*in ra byte dạng 16bit*/

/*******************************************************************************
* Code
******************************************************************************/
int main(int argc, char* argv[])
{
    uint16_t a = 0x5172;
    uint32_t b = 0x1A3B5C7D;
    /*16bit*/
    printf("REG_HEX16 = 0x%x\n",a);
    printf("----convert to 16bit-----\n");
    INT16_TO_BINARY(a);
    printf("\n----Set bit n=%d----------\n",n);
    REG_BIT_SET32(&a,n);
    INT16_TO_BINARY(a);
    printf("\n----Clear bit n=%d--------\n", n);
    REG_BIT_CLEAR32(&a, n);
    INT16_TO_BINARY(a);
    printf("\n----Swap byte n=%d--------\n", n);
    SWAP_BYTE16(&a);
    INT16_TO_BINARY(a);
    /*32bit*/
    printf("\n\n\nREG_HEX32 = 0x%x\n", b);
    printf("----convert to 32bit-----\n");
    INT32_TO_BINARY(b);
    printf("\n----Set bit n=%d----------\n", n);
    REG_BIT_SET32(&b, n);
    INT32_TO_BINARY(b);
    printf("\n----Clear bit n=%d--------\n", n);
    REG_BIT_CLEAR32(&b, n);
    INT32_TO_BINARY(b);
    printf("\n----Swap byte n=%d--------\n", n);
    SWAP_BYTE32(&b);
    INT32_TO_BINARY(b);
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
