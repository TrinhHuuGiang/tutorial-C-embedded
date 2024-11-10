/*
    Copyright (C) <2024> <Giang Trinh>

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
*/

/*
 * don't use led blue + green, SIRC
 * because they are used
 * */
/*******************************************************************************
* Definitions
******************************************************************************/
#ifndef LPIT_2LED_H_
#define LPIT_2LED_H_

/*******************************************************************************
* API
******************************************************************************/
/*
 * init LPIT0 using channel 0
 * + LPIT using SIRC is clock 8M
 * + add interrupt
 * */
void init_LPIT0_chanel0();
/*
 * set value second (double)
 * then run LPIT0 Channel0
 * */
void reset_value_LPIT0_channel0(double lpit0_ch0_cycle);

#endif /* LPIT_2LED_H_ */

/*
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */