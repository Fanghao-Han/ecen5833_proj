/*
 * irq.c
 *
 *  Created on: Sep 6, 2021
 *      Author: Fanghao Han
 *
 *  Interrupt (IRQ) service routines (ISRs)
 */
#include "irq.h"
#include "app.h"
#include "em_core.h"
#include "em_letimer.h"

/**
* @brief
*   LETIMER0 IRQ
* @details
*   Measure temp when triggered by IFC_COMP0.
*/
void LETIMER0_IRQHandler()
{
    uint32_t flags = 0;
    CORE_DECLARE_IRQ_STATE;

    CORE_ENTER_CRITICAL();

    flags = LETIMER_IntGet( LETIMER0 );
    if( flags & LETIMER_IF_COMP1 )
    {
        LETIMER_IntClear( LETIMER0, LETIMER_IFC_COMP1 );
    }
    else if( flags & LETIMER_IF_COMP0 )
    {
        LETIMER_IntClear( LETIMER0, LETIMER_IFC_COMP0 );
    }

    CORE_EXIT_CRITICAL();
}

//void I2C0_IRQHandler()
//{
//  CORE_DECLARE_IRQ_STATE;
//
//  CORE_ENTER_CRITICAL();
//
//  I2C_TransferReturn_TypeDef ret = I2C_Transfer( I2C0 );
//
//  if( ret == i2cTransferDone )
//  {
//      schedulerSetEventI2cDone();
//  }
//  else if( ret != i2cTransferInProgress )
//  {
//      schedulerSetEventI2cErr();
//  }
//
//  CORE_EXIT_CRITICAL();
//}


void GPIO_EVEN_IRQHandler(void)
{
  uint32_t gpio_if = GPIO->IF;

  // Clear all even pin interrupt flags
  GPIO_IntClear(0x5555);

  //button_on_change_handler(0);
  if (gpio_if & (1U<<VL53L0X_GPIO1_PIN)) {
      // exp11
      vl_set_flag_measure_ready(true);
  }
  else if (gpio_if & (1U<<APDS9960_INT_PIN)) {
      // exp7
      gest_set_flag_data_ready(true);
  }
}

/**************************************************************************//**
 * @brief GPIO Odd IRQ for pushbuttons on odd-numbered pins
 *****************************************************************************/
void GPIO_ODD_IRQHandler(void)
{
  // Clear all odd pin interrupt flags
  GPIO_IntClear(0xAAAA);

}
