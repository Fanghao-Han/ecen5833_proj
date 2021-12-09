/***************************************************************************//**
 * @file
 * @brief Core application logic.
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


#include "app.h"

#define INCLUDE_LOG_DEBUG 1
#include "log.h"

/*****************************************************************************
 * Application Power Manager callbacks
 *****************************************************************************/
#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)



bool app_is_ok_to_sleep(void)
{

  return APP_IS_OK_TO_SLEEP;

} // app_is_ok_to_sleep()



sl_power_manager_on_isr_exit_t app_sleep_on_isr_exit(void)
{

  return APP_SLEEP_ON_ISR_EXIT;

} // app_sleep_on_isr_exit()



#endif // defined(SL_CATALOG_POWER_MANAGER_PRESENT)




/**************************************************************************//**
 * Application Init.
 *****************************************************************************/
SL_WEAK void app_init(void)
{
  // Put your application 1-time init code here
  // This is called once during start-up.
  // Don't call any Bluetooth API functions until after the boot event.


  // Student Edit: Add a call to gpioInit() here
  gpioInit();
  // enable peripheral(s) to generate IRQs
  oscInit();
  initLETIMER0();
  // Init event scheduler
  schedulerSetEventIdle();

  // Add power requirement
  if(ENERGY_MODE == EM1)
  {
      sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
  }

  if(ENERGY_MODE == EM2)
  {
      sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM2);
  }


  // Start timer
  LETIMER_Enable(LETIMER0, true);

  // Initialize sensors
  if (vl53l0x_init()) {
      LOG_INFO("vl53l0x init success");
  }
  else {
      LOG_ERROR("Error: vl53l0x init");
  }
  timerWaitms(500);

  if (APDS9960_init()) {
      LOG_INFO("APDS-9960 initialization complete");
  }
  else {
      LOG_ERROR("Error: APDS-9960 init");
  }

  if (APDS9960_enableGestureSensor(true)) {
      gpioGestureLedSet(DIR_UP, 0);
      gpioGestureLedSet(DIR_DOWN, 0);
      gpioGestureLedSet(DIR_LEFT, 0);
      gpioGestureLedSet(DIR_RIGHT, 0);
      LOG_INFO("Gesture sensor is now running");
  }
  else {
      LOG_ERROR("Error: gesture sensor init");
  }

  // enable NVIC interrupts
  NVIC_ClearPendingIRQ(LETIMER0_IRQn);
  NVIC_EnableIRQ(LETIMER0_IRQn);
}




/**************************************************************************//**
 * Application Process Action.
 *****************************************************************************/
SL_WEAK void app_process_action(void)
{
  // scheduler application entry
  schedulerApp();
  //gesture_fsm();
  //gpioLedLeftToggle();
  //timerWaitms(500);

}

/**************************************************************************//**
 * Bluetooth stack event handler.
 * This overrides the dummy weak implementation.
 *
 * @param[in] evt Event coming from the Bluetooth stack.
 *
 * The code here will process events from the Bluetooth stack. This is the only
 * opportunity we will get to act on an event.
 *****************************************************************************/
void sl_bt_on_event(sl_bt_msg_t *evt)
{
  if (evt->header) {
      ;
  }

  handle_ble_event(evt); // put this code in ble.c/.h

} // sl_bt_on_event()

