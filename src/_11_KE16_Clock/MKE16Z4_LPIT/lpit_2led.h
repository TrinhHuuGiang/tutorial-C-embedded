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
