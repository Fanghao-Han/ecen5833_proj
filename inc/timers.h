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
void timerWaitUs(uint32_t time_us);
void timerWaitms(uint32_t time_ms);

#endif /* SRC_TIMERS_H_ */
