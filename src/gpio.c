/*
  gpio.c
 */


#include "gpio.h"

// Set GPIO drive strengths and modes of operation
void gpioInit()
{
  // 1. LEDs
  GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthStrongAlternateStrong);
  GPIO_PinModeSet(LED0_port, LED0_pin, gpioModePushPull, false);

  GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthStrongAlternateStrong);
  GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, false);

  // 2. buttons, filter
  GPIO_PinModeSet(BTN_port, BTN0_PIN, gpioModeInputPullFilter, 1);
  GPIO_PinModeSet(BTN_port, BTN1_PIN, gpioModeInputPullFilter, 1);

  // Enable rising & falling-edge interrupts for PB pins
  GPIO_ExtIntConfig(BTN_port, BTN0_PIN, BTN0_PIN, 1, 1, true);
  GPIO_ExtIntConfig(BTN_port, BTN1_PIN, BTN1_PIN, 1, 1, true);

  // 3. Distance Sensor VL53L0X
  GPIO_DriveStrengthSet(VL53L0X_GPIO_PORT, gpioDriveStrengthStrongAlternateStrong);
  GPIO_PinModeSet(VL53L0X_GPIO_PORT, VL53L0X_XSHUT_PIN, gpioModePushPull, false);

  GPIO_PinModeSet(VL53L0X_GPIO_PORT, VL53L0X_GPIO1_PIN, gpioModeInputPullFilter, 1);
//  GPIO_ExtIntConfig(VL53L0X_GPIO_PORT, VL53L0X_GPIO1_PIN, VL53L0X_GPIO1_PIN / 4,
//                    1, 1, true);
  GPIO_ExtIntConfig(VL53L0X_GPIO_PORT, VL53L0X_GPIO1_PIN, VL53L0X_GPIO1_PIN, 0, 1, true);

  // Enable IRQ for even numbered GPIO pins
  NVIC_EnableIRQ(GPIO_EVEN_IRQn);
  // Enable IRQ for odd numbered GPIO pins
  NVIC_EnableIRQ(GPIO_ODD_IRQn);
} // gpioInit()


void gpioLed0SetOn()
{
  GPIO_PinOutSet(LED0_port,LED0_pin);
}


void gpioLed0SetOff()
{
  GPIO_PinOutClear(LED0_port,LED0_pin);
}


void gpioLed1SetOn()
{
  GPIO_PinOutSet(LED1_port,LED1_pin);
}


void gpioLed1SetOff()
{
  GPIO_PinOutClear(LED1_port,LED1_pin);
}

bool gpioReadBTN(uint8_t id)
{
  switch(id)
  {
    case 0:
      return (bool)GPIO_PinInGet(BTN_port, BTN0_PIN);

    case 1:
      return (bool)GPIO_PinInGet(BTN_port, BTN1_PIN);

    default:
      return 0;
  }
}

void gpioI2C0SetOff()
{
  GPIO_PinOutClear(I2C0_PORT, I2C0_SCL_PIN);
  GPIO_PinOutClear(I2C0_PORT, I2C0_SDA_PIN);
}

void gpioVL53SetOn()
{
  GPIO_PinOutSet(VL53L0X_GPIO_PORT, VL53L0X_XSHUT_PIN);
}

void gpioVL53SetOff()
{
  GPIO_PinOutClear(VL53L0X_GPIO_PORT, VL53L0X_XSHUT_PIN);
}

bool gpioReadVL53IrqPin()
{
  return (bool)GPIO_PinInGet(VL53L0X_GPIO_PORT, VL53L0X_GPIO1_PIN);
}


