/*
 * apds9960_test.c
 *
 *  Created on: Oct 12, 2021
 *      Author: han16
 */

#include "apds9960_test.h"
#include "SparkFun_APDS9960.h"

#include "log.h"

extern void timerWaitms(uint32_t time_ms);
#define delay timerWaitms


void apds9960_test()
{
  uint16_t ambient_light = 0;
  uint16_t red_light = 0;
  uint16_t green_light = 0;
  uint16_t blue_light = 0;

  APDS9960_data_init();

  if (APDS9960_init())
    {
      LOG_INFO("APDS-9960 initialization complete");
    }
  else
    {
      LOG_ERROR("Something went wrong during APDS-9960 init!");
    }

  if (APDS9960_enableLightSensor(false))
    {
      LOG_INFO("Light sensor is now running");
    }
  else
    {
      LOG_ERROR("Something went wrong during light sensor init!");
    }

  // Wait for initialization and calibration to finish
  delay(500);

  while(1)
    {
      // Read the light levels (ambient, red, green, blue)
      if (  !APDS9960_readAmbientLight(&ambient_light) ||
            !APDS9960_readRedLight(&red_light) ||
            !APDS9960_readGreenLight(&green_light) ||
            !APDS9960_readBlueLight(&blue_light) )
      {
        LOG_ERROR("Error reading light values");
      }
      else
      {
        LOG_INFO("Ambient: %d", ambient_light);

        LOG_INFO(" Red: %d", red_light);

        LOG_INFO(" Green: %d",green_light);

        LOG_INFO(" Blue: %d", blue_light);

      }

      // Wait 1 second before next reading
      delay(1000);

    }

}
