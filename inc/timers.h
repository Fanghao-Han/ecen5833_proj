/*
 * timers.h
 *
 *  Created on: Sep 6, 2021
 *      Author: Fanghao Han
 */

#ifndef SRC_TIMERS_H_
#define SRC_TIMERS_H_

#include "em_letimer.h"
#include "main.h"

void initLETIMER0(void);
void timerWaitUs(uint32_t);
void timerWaitms(uint32_t);

#endif /* SRC_TIMERS_H_ */
