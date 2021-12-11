/*
 * gesture_fsm.c
 *
 *  Created on: Nov 17, 2021
 *      Author: han16
 */

#define GESTURE_TEST_MODE  GTM_GEST

#include "gesture_fsm.h"
#include "SparkFun_APDS9960.h"
#include "apds9960_i2c.h"

#define INCLUDE_LOG_DEBUG 1
#include "log.h"

extern void timerWaitms(uint32_t time_ms);

static uint8_t curr_state = 0;
static uint8_t next_state = 0;

bool flag_gest_en = false;
bool flag_gest_isr = false;
bool flag_gest_data_ready = false;

static bool handleGesture();

void gesture_fsm() {

  curr_state = next_state;

  switch(curr_state) {
      case gest_st_IDLE: {
        if (gest_get_flag_isr()) {
            gest_set_flag_isr(false);
#if EVAL_BRD==1
            gpioLed0SetOn();
#else
            gpioLed0SetOn();
#endif
            handleGesture();

            schedulerSetEventIdle();
            //next_state = gest_st_MeasureRequest;
        }
        else {
#if EVAL_BRD==1
            gpioLed0SetOff();
#else
            gpioLed0SetOff();;
#endif
        }
        break;
      }
  }
}




void gest_set_flag_enable(bool is_enable) {
    flag_gest_en = is_enable;
}
bool gest_get_flag_enable() {
    return flag_gest_en;
}


void gest_set_flag_isr(bool is_enable) {
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();

  flag_gest_isr = is_enable;

  CORE_EXIT_CRITICAL();

}
bool gest_get_flag_isr() {
    return flag_gest_isr;
}


void gest_set_flag_data_ready(bool is_enable) {
    flag_gest_data_ready = is_enable;
}
bool gest_get_flag_data_ready() {
    return flag_gest_data_ready;
}


static bool handleGesture() {
    if (APDS9960_isGestureAvailable()) {
      int gest_st = APDS9960_readGesture();
#if EVAL_BRD==1
      switch (gest_st) {
        case DIR_UP:
          LOG_INFO("Up");
          break;
        case DIR_DOWN:
          LOG_INFO("Down");
          break;
        case DIR_LEFT:
          LOG_INFO("Left");
          break;
        case DIR_RIGHT:
          LOG_INFO("Right");
          break;
        case DIR_NEAR:
          LOG_INFO("Near");
          break;
        case DIR_FAR:
          LOG_INFO("Far");
          break;
        default:
          LOG_INFO("None");
          break;
      }
#else
      switch (gest_st) {
        case DIR_UP:
          gpioGestureLedSet(DIR_UP, 1);
          gpioGestureLedSet(DIR_DOWN, 0);
          gpioGestureLedSet(DIR_LEFT, 0);
          gpioGestureLedSet(DIR_RIGHT, 0);
          break;
        case DIR_DOWN:
          gpioGestureLedSet(DIR_UP, 0);
          gpioGestureLedSet(DIR_DOWN, 1);
          gpioGestureLedSet(DIR_LEFT, 0);
          gpioGestureLedSet(DIR_RIGHT, 0);
          break;
        case DIR_LEFT:
          gpioGestureLedSet(DIR_UP, 0);
          gpioGestureLedSet(DIR_DOWN, 0);
          gpioGestureLedSet(DIR_LEFT, 1);
          gpioGestureLedSet(DIR_RIGHT, 0);
          break;
        case DIR_RIGHT:
          gpioGestureLedSet(DIR_UP, 0);
          gpioGestureLedSet(DIR_DOWN, 0);
          gpioGestureLedSet(DIR_LEFT, 0);
          gpioGestureLedSet(DIR_RIGHT, 1);
          break;
        case DIR_NEAR:
          // Stop Sign
          gpioGestureLedSet(DIR_UP, 0);
          gpioGestureLedSet(DIR_DOWN, 0);
          gpioGestureLedSet(DIR_LEFT, 0);
          gpioGestureLedSet(DIR_RIGHT, 0);
          break;
        case DIR_FAR:
//          LOG_INFO("Far");
          break;
        default:
//          LOG_INFO("None");
          break;
      }
#endif
      return true;
    }
    else {
        return false;
    }
}
