/**
 * gpio.h
 */

#ifndef SRC_GPIO_H_
#define SRC_GPIO_H_

#include <stdbool.h>
#include "em_gpio.h"
#include <string.h>


// Student Edit: Define these, 0's are placeholder values.
// See the radio board user guide at https://www.silabs.com/documents/login/user-guides/ug279-brd4104a-user-guide.pdf
// and GPIO documentation at https://siliconlabs.github.io/Gecko_SDK_Doc/efm32g/html/group__GPIO.html
// to determine the correct values for these.

/* LED Peripheral */
#define LED0_port         gpioPortF
#define LED0_pin          4
#define LED1_port         gpioPortF
#define LED1_pin          5

/* buttons */
#define BTN_port          gpioPortF
#define BTN0_PIN          6
#define BTN1_PIN          7

/* I2C0 Peripheral */
#define I2C0_PORT         gpioPortC
#define I2C0_SCL_PIN      10
#define I2C0_SDA_PIN      11

/* Distance Sensor VL53L0x */
#define VL53L0X_GPIO_PORT   gpioPortD
#define VL53L0X_XSHUT_PIN   11        // Power control, EXP9
#define VL53L0X_GPIO1_PIN   12        // Interrupt, EXP11

/* Color & Gesture Sensor Apds9960 */
#define APDS9960_GPIO_PORT  gpioPortD
#define APDS9960_INT_PIN    10        // Interrupt, EXP7

// Function prototypes
void gpioInit();

/* GPIO Write */
void gpioLed0SetOn();
void gpioLed0SetOff();
void gpioLed1SetOn();
void gpioLed1SetOff();
void gpioVL53SetOn();
void gpioVL53SetOff();

/*GPIO Read*/
bool gpioReadBTN(uint8_t id);

#endif /* SRC_GPIO_H_ */
