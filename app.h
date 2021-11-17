/***************************************************************************//**
 * @file
 * @brief Application interface provided to main().
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * Date:        08-07-2021
 * Author:      Dave Sluiter
 * Description: This code was created by the Silicon Labs application wizard
 *              and started as "Bluetooth - SoC Empty".
 *              It is to be used only for ECEN 5823 "IoT Embedded Firmware".
 *              The MSLA referenced above is in effect.
 *
 ******************************************************************************/

#ifndef APP_H
#define APP_H


#include "em_common.h"
#include "app_assert.h" // got messages that sl_app_assert() is deprecated and should switch to all_assert()

#include "sl_bluetooth.h"
#include "gatt_db.h"

#include "sl_status.h" // for sl_status_print()

#include "ble_device_type.h"
#include "gpio.h"
#include "lcd.h"
#include "oscillators.h"
#include "timers.h"
#include "irq.h"
#include "scheduler.h"
#include "i2c.h"
#include "ble.h"

#include "ridar_test.h"
#include "ridar_fsm.h"

#define INCLUDE_LOG_DEBUG 1

#define DEBUG_EM0

#define LOWEST_ENERGY_MODE      SL_POWER_MANAGER_EM2

#define EM0                     SL_POWER_MANAGER_EM0
#define EM1                     SL_POWER_MANAGER_EM1
#define EM2                     SL_POWER_MANAGER_EM2
#define EM3                     SL_POWER_MANAGER_EM3

#if defined(DEBUG_EM0)
#define ENERGY_MODE             EM0
#elif defined(DEBUG_EM1)
#define ENERGY_MODE             EM1
#elif defined(DEBUG_EM2)
#define ENERGY_MODE             EM2
#elif defined(DEBUG_EM3)
#define ENERGY_MODE             EM3
#endif //defined(DEBUG_EM0)


#define LETIMER_ON_TIME_MS      175
#define LETIMER_PERIOD_MS       3000

#define PRESCALER_VALUE         2
#define ACTUAL_CLK_FREQ         (38400 / PRESCALER_VALUE)
#define ACTUAL_CLK_PERIOD       (1.0 / ACTUAL_CLK_FREQ)

#define VALUE_TO_LOAD           (LETIMER_PERIOD_MS * ACTUAL_CLK_FREQ / 1000)


// See: https://docs.silabs.com/gecko-platform/latest/service/power_manager/overview
#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)

// -----------------------------------------------------------------------------
// defines for power manager callbacks
// -----------------------------------------------------------------------------
// Return values for app_is_ok_to_sleep():
//   Return false to keep sl_power_manager_sleep() from sleeping the MCU.
//   Return true to allow system to sleep when you expect/want an IRQ to wake
//   up the MCU from the call to sl_power_manager_sleep() in the main while (1)
//   loop.
// Students: We'll need to modify this for A2 onward.
/** @note false for EM0, and to true for EM1, EM2 and EM3. */
#if defined(DEBUG_EM0)
#define APP_IS_OK_TO_SLEEP      (false)
#else
#define APP_IS_OK_TO_SLEEP      (true)
#endif // defined(DEBUG_EM0)

// Return values for app_sleep_on_isr_exit():
//   SL_POWER_MANAGER_IGNORE; // The module did not trigger an ISR and it doesn't want to contribute to the decision
//   SL_POWER_MANAGER_SLEEP;  // The module was the one that caused the system wakeup and the system SHOULD go back to sleep
//   SL_POWER_MANAGER_WAKEUP; // The module was the one that caused the system wakeup and the system MUST NOT go back to sleep
//
// Notes:
//       SL_POWER_MANAGER_IGNORE, we see calls to app_process_action() on each IRQ. This is the
//       expected "normal" behavior.
//
//       SL_POWER_MANAGER_SLEEP, the function app_process_action()
//       in the main while(1) loop will not be called! It would seem that sl_power_manager_sleep()
//       does not return in this case.
//
//       SL_POWER_MANAGER_WAKEUP, doesn't seem to allow ISRs to run. Main while loop is
//       running continuously, flooding the VCOM port with printf text with LETIMER0 IRQs
//       disabled somehow, LED0 is not flashing.

#define APP_SLEEP_ON_ISR_EXIT   (SL_POWER_MANAGER_IGNORE)
//#define APP_SLEEP_ON_ISR_EXIT   (SL_POWER_MANAGER_SLEEP)
//#define APP_SLEEP_ON_ISR_EXIT   (SL_POWER_MANAGER_WAKEUP)

#endif // defined(SL_CATALOG_POWER_MANAGER_PRESENT)


/**************************************************************************//**
 * Application Init.
 *****************************************************************************/
void app_init(void);

/**************************************************************************//**
 * Application Process Action.
 *****************************************************************************/
void app_process_action(void);

#endif // APP_H
