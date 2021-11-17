/*
 * oscillators.h
 *
 *  Created on: Sep 6, 2021
 *      Author: Fanghao Han
 */

#ifndef SRC_OSCILLATORS_H_
#define SRC_OSCILLATORS_H_


#include "main.h"
#include "em_cmu.h"

#define CMU_CLK_BRANCH      (cmuClock_LFA)
#define CMU_CLK_TIMER       (cmuClock_LETIMER0)

void oscInit();
uint32_t oscGetCMUFreq();

#endif /* SRC_OSCILLATORS_H_ */
