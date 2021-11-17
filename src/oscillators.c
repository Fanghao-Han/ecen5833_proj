/*
 * oscillators.c
 *
 *  Created on: Sep 6, 2021
 *      Author: Fanghao Han
 *
 *  CMU configuration functions
 */

#include "oscillators.h"


/**
* @brief
*   Initialize Oscillator for different energy modes
* @details
*   PRESCALER_VALUE is 2 by default.
*/
void oscInit()
{
  CMU_Osc_TypeDef cmu_osc_type = 0;
  CMU_Select_TypeDef cmu_osc_ref = 0;

  if ((ENERGY_MODE == EM0) || (ENERGY_MODE == EM1) || (ENERGY_MODE == EM2))
  {
    cmu_osc_type = cmuOsc_LFXO;
    cmu_osc_ref = cmuSelect_LFXO;
  }
  else if(ENERGY_MODE == EM3)
  {
    cmu_osc_type = cmuOsc_ULFRCO;
    cmu_osc_ref = cmuSelect_ULFRCO;
  }
  else
  {
    return;
  }

  CMU_ClockEnable(cmuClock_HFLE, true);

  /* enable an oscillator */
  CMU_OscillatorEnable(cmu_osc_type, true, true);
  /* select a clock source */
  CMU_ClockSelectSet(CMU_CLK_BRANCH, cmu_osc_ref);
  /* Enable a clock branch */
  CMU_ClockEnable(CMU_CLK_BRANCH, true);
  /* set the clock branch prescaler */
  CMU_ClockDivSet(CMU_CLK_TIMER, PRESCALER_VALUE);

  CMU_ClockEnable(CMU_CLK_TIMER, true);

  return;
}
