/*
 * scheduler.c
 *
 *  Created on: Sep 15, 2021
 *      Author: han16
 */

#include "scheduler.h"
#include "log.h"

#include "em_core.h"

#include <string.h>


/****************************** Declaration ********************************/

uint8_t curr_event = 0;


/****************************** Definition *********************************/

void schedulerApp()
{
  switch (getEvent()) {
    case evtReadDistance:
      ridar_fsm();
      break;
    case evtReadGesture:
      gesture_fsm();
      break;
    default:
      break;
  }
}


uint8_t getEvent()
{
  return (curr_event);
} // getEvent()


void schedulerSetEventIdle()
{
  CORE_DECLARE_IRQ_STATE;
  // enter critical section
  CORE_ENTER_CRITICAL();
  // set the event in your data structure, this is a read-modify-write
  curr_event = evtNoEvent; // 0
  // exit critical section
  CORE_EXIT_CRITICAL();
} // schedulerSetEventIdle()

void schedulerSetEventReadDistance()
{
  CORE_DECLARE_IRQ_STATE;
  // enter critical section
  CORE_ENTER_CRITICAL();
  // set the event in your data structure, this is a read-modify-write
  curr_event = evtReadDistance;
  // exit critical section
  CORE_EXIT_CRITICAL();
} // schedulerSetEventReadTemp()

void schedulerSetEventReadGesture()
{
  CORE_DECLARE_IRQ_STATE;
  // enter critical section
  CORE_ENTER_CRITICAL();
  // set the event in your data structure, this is a read-modify-write
  curr_event = evtReadGesture;
  // exit critical section
  CORE_EXIT_CRITICAL();
} // schedulerSetEventReadHumd()


