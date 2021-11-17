 /*
 * gesture_fsm.h
 *
 *  Created on: Nov 17, 2021
 *      Author: han16
 */

#ifndef INC_GESTURE_FSM_H_
#define INC_GESTURE_FSM_H_

/*
 * ridar_test.h
 *
 *  Created on: Oct 5, 2021
 *      Author: han16
 */

#include "app.h"

#include <stdint.h>
#include <stdbool.h>

#define LIGHT_INT_HIGH  1000 // High light level for interrupt
#define LIGHT_INT_LOW   10   // Low light level for interrupt

/**
 * Gesture Test Mode
*/
enum {
    GTM_NONE    = 0,
    GTM_COLOR   = 1,
    GTM_COLOR_INT,
    GTM_POX,
    GTM_POX_INT,
    GTM_NUM,
};

/**
 * apds9960 gesture reading state
 * */
typedef enum {
  gest_st_IDLE                = 0,
  gest_st_Boot,
  gest_st_MeasureRequest,
  gest_st_WaitForStartBit,
  gest_st_WaitForMeasure,
  gest_st_MeasureCompleted,
  gest_st_Error,
}gest_state_t;

typedef struct gest_data_typedef gest_data_t;

/**
 * @brief
 *  FLAG - Enable
 * */
void gest_set_flag_enable(bool is_enable);
bool gest_get_flag_enable();

/**
 * @brief
 *  FLAG - Chip Measurement Completed
 * @details
 *
 * */
void gest_set_flag_measure_ready(bool is_enable);
bool gest_get_flag_measure_ready();

/**
 * @brief
 *  FLAG - Chip Data Ready
 * @details
 * 
 * */
void gest_set_flag_data_ready(bool is_enable);
bool gest_get_flag_data_ready();

/**
 * @brief
 *  gesture data write & read
 * */
void gest_set_result(gest_data_t wdata);
gest_data_t * gest_get_result();


void gesture_fsm();

#endif /* INC_GESTURE_FSM_H_ */
