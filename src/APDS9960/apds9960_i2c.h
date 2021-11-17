/*
 * apds9960_i2c.h
 *
 *  Created on: Nov 15, 2021
 *      Author: han16
 */

#ifndef SRC_APDS9960_APDS9960_I2C_H_
#define SRC_APDS9960_APDS9960_I2C_H_

#include "SparkFun_APDS9960.h"


//bool wireWriteByte(uint8_t val);
bool wireWriteDataByte(uint8_t reg, uint8_t val);
bool wireWriteDataBlock(uint8_t reg, uint8_t *val, unsigned int len);
bool wireReadDataByte(uint8_t reg, uint8_t *val);
int wireReadDataBlock(uint8_t reg, uint8_t *val, unsigned int len);

#endif /* SRC_APDS9960_APDS9960_I2C_H_ */
