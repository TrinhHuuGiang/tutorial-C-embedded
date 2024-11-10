/*
    Copyright (C) <2024> <Giang Trinh>

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
*/

/*******************************************************************************
* Definitions
******************************************************************************/
#ifndef RTC_2LED_H_
#define RTC_2LED_H_

/*******************************************************************************
* API
******************************************************************************/
/*
 * clock source is 1k from LPO
 */
void init_RTC();
/*
 * reset TSR to 0s
 */
void reset_tsr_RTC();// need fix to uint32t
/*
 * check value TSR
 */
uint32_t check_tsr_RTC();

#endif /* RTC_2LED_H_ */

/*
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */