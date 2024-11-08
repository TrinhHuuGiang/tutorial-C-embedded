/*******************************************************************************
* Definitions
******************************************************************************/
#ifndef _HANDLE_ARRAY_
#define _HANDLE_ARRAY_
#include <stdint.h>
#define MAX_KEY (4)
typedef uint8_t(*fpar)(uint8_t*, const int);

/*******************************************************************************
* API
******************************************************************************/
uint8_t add_val(uint8_t* arr, const int arl);/*them gia tri*/
uint8_t minus_val(uint8_t* arr, const int arl);/*xoa gia tri*/
uint8_t print_val(uint8_t* arr, const int arl);/*in gia tri*/
uint8_t exit_loop(uint8_t* arr, const int arl);/*thoat vong lap*/
#endif /*_HANDLE_ARRAY_*/