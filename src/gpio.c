/*
  gpio.c
 */


#include "gpio.h"
#include "SparkFun_APDS9960.h"

#if EVAL_BRD==1
static bool led0_en = 0;
#endif
static bool led_gest_en[4] = {0};
/* Direction definitions */
//enum {
//  DIR_NONE,
//  DIR_LEFT,
//  DIR_RIGHT,
//  DIR_UP,
//  DIR_DOWN,
//  DIR_NEAR,
//  DIR_FAR,
//  DIR_ALL
//};

// Set GPIO drive strengths and modes of operation
void gpioInit()
{
  // 1. LEDs
#if EVAL_BRD==1
  GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthWeakAlternateWeak);
  GPIO_PinModeSet(LED0_port, LED0_pin, gpioModePushPull, false);
#else
  GPIO_DriveStrengthSet(LED_port, gpioDriveStrengthStrongAlternateStrong);
  GPIO_PinModeSet(LED_port, LED_LEFT_PIN, gpioModePushPull, true);

  GPIO_DriveStrengthSet(LED_port, gpioDriveStrengthStrongAlternateStrong);
  GPIO_PinModeSet(LED_port, LED_UP_PIN, gpioModePushPull, true);

  GPIO_DriveStrengthSet(LED_port, gpioDriveStrengthStrongAlternateStrong);
  GPIO_PinModeSet(LED_port, LED_RIGHT_PIN, gpioModePushPull, true);

  GPIO_DriveStrengthSet(LED_port, gpioDriveStrengthStrongAlternateStrong);
  GPIO_PinModeSet(LED_port, LED_BOTTOM_PIN, gpioModePushPull, true);
#endif
  // 2. Distance Sensor VL53L0X
  GPIO_PinModeSet(VL53L0X_GPIO_PORT, VL53L0X_GPIO1_PIN, gpioModeInputPullFilter, 1);
  GPIO_ExtIntConfig(VL53L0X_GPIO_PORT, VL53L0X_GPIO1_PIN, VL53L0X_GPIO1_PIN, 0, 1, true);

  // 3. Gesture Sensor APDS9960
  GPIO_PinModeSet(APDS9960_GPIO_PORT, APDS9960_INT_PIN, gpioModeInputPullFilter, 1);
  GPIO_ExtIntConfig(APDS9960_GPIO_PORT, APDS9960_INT_PIN, APDS9960_INT_PIN, 0, 1, true);

  // Enable IRQ for even numbered GPIO pins
  //NVIC_EnableIRQ(GPIO_EVEN_IRQn);
  // Enable IRQ for odd numbered GPIO pins
  NVIC_EnableIRQ(GPIO_ODD_IRQn);
} // gpioInit()

#if EVAL_BRD==1
void gpioLed0SetOn()
{
  if (!led0_en) {
    GPIO_PinOutSet(LED0_port,LED0_pin);
    led0_en = true;
  }
}

void gpioLed0SetOff()
{
  if (led0_en) {
    GPIO_PinOutClear(LED0_port,LED0_pin);
    led0_en = false;
  }
}
#endif

void gpioGestureLedSet(uint8_t led_id, bool on)
{
  switch(led_id) {
    case DIR_UP:
      if (on) {
          // turn on
          if (!led_gest_en[0]) {
              GPIO_PinOutClear(LED_port,LED_UP_PIN);
              led_gest_en[0] = on;
          }
      }
      else {
          // turn off
          if (led_gest_en[0]) {
            GPIO_PinOutSet(LED_port,LED_UP_PIN);
            led_gest_en[0] = on;
          }
      }
      break;
    case DIR_DOWN:
      if (on) {
          if (!led_gest_en[1]) {
          GPIO_PinOutClear(LED_port,LED_BOTTOM_PIN);
          led_gest_en[1] = on;
          }
      }
      else {
          if (led_gest_en[1]) {
          GPIO_PinOutSet(LED_port,LED_BOTTOM_PIN);
          led_gest_en[1] = on;
          }
      }
      break;
    case DIR_LEFT:
      if (on) {
          if (!led_gest_en[2]) {
          GPIO_PinOutClear(LED_port,LED_LEFT_PIN);
          led_gest_en[2] = on;
          }
      }
      else {
          if (led_gest_en[2]) {
          GPIO_PinOutSet(LED_port,LED_LEFT_PIN);
          led_gest_en[2] = on;
          }
      }
      break;
    case DIR_RIGHT:
      if (on) {
          if (!led_gest_en[3]) {
          GPIO_PinOutClear(LED_port,LED_RIGHT_PIN);
          led_gest_en[3] = on;
          }
      }
      else {
          if (led_gest_en[3]) {
          GPIO_PinOutSet(LED_port,LED_RIGHT_PIN);
          led_gest_en[3] = on;
          }
      }
      break;
    default:
      break;
  }

}

void gpioI2C0SetOff()
{
  GPIO_PinOutClear(I2C0_PORT, I2C0_SCL_PIN);
  GPIO_PinOutClear(I2C0_PORT, I2C0_SDA_PIN);
}

//void gpioVL53SetOn()
//{
//  GPIO_PinOutSet(VL53L0X_GPIO_PORT, VL53L0X_XSHUT_PIN);
//}
//
//void gpioVL53SetOff()
//{
//  GPIO_PinOutClear(VL53L0X_GPIO_PORT, VL53L0X_XSHUT_PIN);
//}

bool gpioReadVL53IrqPin()
{
  return (bool)GPIO_PinInGet(VL53L0X_GPIO_PORT, VL53L0X_GPIO1_PIN);
}


