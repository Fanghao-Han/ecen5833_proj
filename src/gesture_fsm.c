/*
 * gesture_fsm.c
 *
 *  Created on: Nov 17, 2021
 *      Author: han16
 */

#define GESTURE_TEST_MODE  GTM_COLOR
#include "gesture_fsm.h"




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
bool flag_gest_measure_ready = false;
bool flag_gest_data_ready = false;

gest_data_t gest_data = {0};


#if (GESTURE_TEST_MODE == GTM_COLOR)
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
#elif (GESTURE_TEST_MODE == GTM_COLOR_INT)
void gesture_fsm() {
    ;
}
#endif




void gest_set_flag_enable(bool is_enable) {
    flag_gest_en = is_enable;
}
bool gest_get_flag_enable() {
    return flag_gest_en;
}


void gest_set_flag_measure_ready(bool is_enable) {
    flag_gest_measure_ready = is_enable;
}
bool gest_get_flag_measure_ready() {
    return flag_gest_measure_ready;
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


