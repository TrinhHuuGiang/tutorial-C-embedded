/*
 * do not modify system tick when using this library
 * using function init_Systick before using library
 * */
/*******************************************************************************
* Definitions
******************************************************************************/
#ifndef LED_GREEN_STATUS_H_
#define LED_GREEN_STATUS_H_

#include <stdint.h>
#define BLINK_CYCLE (0.2)  //5 Hz
#define DUTY_CYCLE  (0.01) //100 Hz
#define RAISE_CYCLE (4.0)  //second
#define DUTY_STEP   (DUTY_CYCLE/RAISE_CYCLE) //0.25%

typedef void (*fp_led_green)();
/*******************************************************************************
* API
******************************************************************************/
/*
 * initialize system tick
 * - set interrupt
 */
void init_Systick();
/*
 * initialize led GREEN
 * */
void init_Led_green();
/*
 * turn on led GREEN
 * */
void on_Led_green();
/*
 * turn off led GREEN
 * */
void off_Led_green();
/*
 * blink led with frequency 5Hz
 */
void blink_Led_green();
/*
 *  led GREEN gradually brightens
 *  default:
 *  - duty: brightness from 0% to 100%, step 1%
 *  - frequency: 100hz
 * */
void raising_Led_green();

#endif /* LED_GREEN_STATUS_H_ */
