/*
 * scheduler.h
 *
 *  Created on: Sep 15, 2021
 *      Author: han16
 */

#ifndef SRC_SCHEDULER_H_
#define SRC_SCHEDULER_H_

#include "app.h"
#include "ridar_fsm.h"

#include <stdint.h>


typedef enum {
  evtNoEvent            = 0,
  evtReadDistance       = 1,
  evtReadGesture        = 2,
  evtNum
}evt_t;


void schedulerApp();

/**
 * @brief
 *  scheduler routine to set a scheduler event
 */
void schedulerSetEventIdle(void);
void schedulerSetEventReadDistance(void);
void schedulerSetEventReadGesture(void);

/**
 * @brief
 *  Get current event
 * @details
 *  scheduler routine to return 1 event at lower state to main()code
 */
uint8_t getEvent(void);



#endif /* SRC_SCHEDULER_H_ */
