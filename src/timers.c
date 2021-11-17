/*
 * timers.c
 *
 *  Created on: Sep 6, 2021
 *      Author: Fanghao Han
 *
 *  LETIMER0 functions
 */

#include <timers.h>
#include "oscillators.h"

static uint32_t clkFreq;
static uint32_t usPerTick;
static uint32_t comp0_val;
static uint32_t calCOMP(uint32_t ms);

/**
 * @brief   Initialize LETIMER0
 *
 * @details
 *  Timer needs to be enabled outside this function.
 */
void initLETIMER0(void)
{
  /* 1. Set configurations for LETIMER 0 */
  const LETIMER_Init_TypeDef letimerInit =   {
      .enable = false,   /* Do not start timer when initialization completes. */
      .debugRun = false,
      .comp0Top = true,  /* Load COMP0 into CNT on underflow. */
      .bufTop = false,
      .out0Pol = 0,
      .out1Pol = 0,
      .ufoa0 = letimerUFOANone,
      .ufoa1 = letimerUFOANone,
      .repMode = letimerRepeatFree,
      .topValue = 0
    };

  LETIMER_Init(LETIMER0, &letimerInit);

  /* 2. Clear Interrupt Flag */
  LETIMER_IntClear(LETIMER0, LETIMER_IFC_COMP0);
  //LETIMER_IntClear(LETIMER0, LETIMER_IFC_COMP1);

  /* 3. Calculate & Set COMP0 & COM1 Load values */
  comp0_val = calCOMP(LETIMER_PERIOD_MS);
  LETIMER_CompareSet(LETIMER0, 0, (uint16_t)comp0_val);
  //LETIMER_CompareSet(LETIMER0, 1, calCOMP(LETIMER_ON_TIME_MS));

  clkFreq = CMU_ClockFreqGet(CMU_CLK_TIMER);
  usPerTick = 1000000 / clkFreq;

  /* 4. Enable Interrupt */
  LETIMER_IntEnable( LETIMER0, LETIMER_IEN_COMP0 );
  //LETIMER_IntEnable( LETIMER0, LETIMER_IEN_COMP1 );
}


/**
* @brief Calculate Load value for COMP register
*
* @param[in] ms
*   Target time in milli seconds.
*
* @retval
*   COMP value.
*/
static uint32_t calCOMP(uint32_t ms)
{
  uint32_t ticks = 0;
  // Read timer's clock frequency, which has taken PRESCALER into account.
  uint32_t clkfreq = CMU_ClockFreqGet(CMU_CLK_TIMER);

  /* Time cannot be longer than 7 sec */
  if(ms <= 7000)
  {
      ticks = (clkfreq * ms) / 1000;
  }
  else
  {
      ticks = clkfreq * 7;
  }

  return ticks;
}

/**
 * @brief
 *  A delay function implemented by timer.
 * @param time_us
 * @details
 *  1 tick is (1/clock_freq) sec.
 *  For example,
 *    PRESCALER=1 -> 1 tick = (1/38400) s ≈ 26 us
 *    PRESCALER=2 -> 1 tick = (2/38400) s ≈ 52 us
 *    PRESCALER=4 -> 1 tick = (4/38400) s ≈ 104 us
 */
void timerWaitUs(uint32_t time_us)
{
  uint32_t remainTime_us = 0;

  //uint32_t clkFreq = 0;
  //uint32_t usPerTick = 0;

  //unsigned long totalTicks = 0;
  uint32_t startTick = 0;
  uint32_t endTick = 0;
  uint32_t remainTicks = 0;

  int32_t iter = 0;

  /* 1. Calculate time per tick (us) */
  //clkFreq = CMU_ClockFreqGet(CMU_CLK_TIMER);
  //usPerTick = 1000000 / clkFreq;

  /* 2. Calculate 3-step's wait ticks */
  startTick = LETIMER_CounterGet(LETIMER0);


  if(time_us <= (startTick * usPerTick))
  {
      iter = -1;

      if(time_us < usPerTick)
      {
          /* wait time is too short */
          endTick = startTick - 1;
      }
      else
      {
          /* wait time is shorter than current Timer sequence */
          endTick = startTick - (time_us / usPerTick);
      }
  }
  else
  {
      /* wait time is longer than current Timer sequence */
      remainTime_us = time_us - startTick * usPerTick;
      iter = remainTime_us / (LETIMER_PERIOD_MS * 1000) + 1;

      remainTime_us = remainTime_us % (LETIMER_PERIOD_MS * 1000);
      remainTicks = remainTime_us / usPerTick;
      endTick = LETIMER_CompareGet(LETIMER0, 0) - remainTicks;
  }


  /* Start waiting...*/
  if(iter == -1)
  {
      while((uint16_t)endTick < (uint16_t)LETIMER_CounterGet(LETIMER0))
      {
          ;
      }
  }
  else
  {
      /* step-1. Count-down current CNT value. */
      /* step-2. Start counting UF bit set times -> period time */

      while(iter--)
      {
          while(LETIMER_CounterGet(LETIMER0) != 0)
          {
              ;
          }
      }

      /* step-3. Calculate and count remaining ticks that required */
      while((uint16_t)endTick < (uint16_t)LETIMER_CounterGet(LETIMER0))
      {
          ;
      }
  }
}

void timerWaitms(uint32_t time_ms)
{
  timerWaitUs(time_ms * 10000);
}

