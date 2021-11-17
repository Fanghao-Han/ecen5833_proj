/*
 * irq.h
 *
 *  Created on: Sep 6, 2021
 *      Author: Fanghao Han
 */

#ifndef SRC_IRQ_H_
#define SRC_IRQ_H_

#include <stdint.h>

void LETIMER0_IRQHandler();

void letimerWaitInit(void);
uint32_t letimerMilliseconds(void);

#endif /* SRC_IRQ_H_ */
