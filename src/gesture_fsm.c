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

struct gest_data_typedef {
    uint16_t proximity;
    uint16_t ambient_light;
    uint16_t red_light;
    uint16_t green_light;
    uint16_t blue_light;
};


extern void timerWaitms(uint32_t time_ms);

static uint8_t curr_state = 0;
static uint8_t next_state = 0;

bool flag_gest_en = false;
bool flag_gest_isr = false;
bool flag_gest_data_ready = false;

gest_data_t gest_data = {0};


static bool handleGesture();

#if 0
void gesture_fsm() {
    curr_state = next_state;

    switch(curr_state) {
        case gest_st_IDLE: {
            if (gest_get_flag_enable()) {
                next_state = gest_st_MeasureRequest;
            }
            break;
        }

        case gest_st_MeasureRequest: {
            // Read the light levels (ambient, red, green, blue)
            gest_data_t gest_data_buf = {0};

            if (  !APDS9960_readAmbientLight(&gest_data_buf.ambient_light) ||
                !APDS9960_readRedLight(&gest_data_buf.red_light) ||
                !APDS9960_readGreenLight(&gest_data_buf.green_light) ||
                !APDS9960_readBlueLight(&gest_data_buf.blue_light) )
            {
                LOG_ERROR("Error reading light values");
                next_state = gest_st_Error;
            }
            else
            {
                //memcpy(&gest_data, &gest_data_buf, sizeof(gest_data));
                gest_set_result(&gest_data_buf);
                gest_set_flag_enable(true);
                next_state = gest_st_IDLE;
            }
            break;
        }
    }
}
#else
void gesture_fsm() {

  curr_state = next_state;

  switch(curr_state) {
      case gest_st_IDLE: {
        if (gest_get_flag_isr()) {
            gest_set_flag_isr(false);

//            gpioLed0SetOn();
            gpioGestureLedSet(DIR_UP, 1);
            gpioGestureLedSet(DIR_DOWN, 1);
            gpioGestureLedSet(DIR_LEFT, 1);
            gpioGestureLedSet(DIR_RIGHT, 1);

            handleGesture();

            //next_state = gest_st_MeasureRequest;
        }
        else {
//            gpioLed0SetOff();
            ;
        }
        break;
      }
  }
}
#endif




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


void gest_set_result(gest_data_t * wdata) {
    if ((&gest_data == NULL)
        || (wdata == NULL)) {
        return;
    }

    memcpy(&gest_data, wdata, sizeof(gest_data));
}

gest_data_t * gest_get_result() {
    return &gest_data;
}


static bool handleGesture() {
    if (APDS9960_isGestureAvailable()) {
      int gest_st = APDS9960_readGesture();

      switch (gest_st) {
        case DIR_UP:
//          LOG_INFO("Up");
          gpioGestureLedSet(DIR_UP, 0);
          gpioGestureLedSet(DIR_DOWN, 0);
          gpioGestureLedSet(DIR_LEFT, 0);
          gpioGestureLedSet(DIR_RIGHT, 0);
          break;
        case DIR_DOWN:
//          LOG_INFO("Down");
          gpioGestureLedSet(DIR_UP, 0);
          gpioGestureLedSet(DIR_DOWN, 1);
          gpioGestureLedSet(DIR_LEFT, 0);
          gpioGestureLedSet(DIR_RIGHT, 0);
          break;
        case DIR_LEFT:
//          LOG_INFO("Left");
          gpioGestureLedSet(DIR_UP, 0);
          gpioGestureLedSet(DIR_DOWN, 0);
          gpioGestureLedSet(DIR_LEFT, 1);
          gpioGestureLedSet(DIR_RIGHT, 0);
          break;
        case DIR_RIGHT:
//          LOG_INFO("Right");
          gpioGestureLedSet(DIR_UP, 0);
          gpioGestureLedSet(DIR_DOWN, 0);
          gpioGestureLedSet(DIR_LEFT, 0);
          gpioGestureLedSet(DIR_RIGHT, 1);
          break;
        case DIR_NEAR:
//          LOG_INFO("Near");
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

      return true;
    }
    else {
        return false;
    }
}
