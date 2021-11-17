/*
 * apds9960_i2c.c
 *
 *  Created on: Nov 15, 2021
 *      Author: han16
 */

#include "apds9960_i2c.h"
#include "i2c.h"

I2C_TransferSeq_TypeDef apds_i2c_seq = { 0 };
uint8_t apds_i2c_seq_data_buf[64] = { 0 };


/*******************************************************************************
* Raw I2C Reads and Writes
******************************************************************************/

uint8_t APDS_Write_nByte(uint8_t REG_Address,uint16_t len, uint8_t *buf)
{
  // 1. Device addr + register addr + data
  I2C_TransferReturn_TypeDef ret;
  uint16_t i = 0;

  memset(&apds_i2c_seq_data_buf, 0, sizeof(apds_i2c_seq_data_buf));

  apds_i2c_seq_data_buf[0] = REG_Address;

  for(i=1; i<(len+1); i++)
  {
      apds_i2c_seq_data_buf[i] = *buf++;
  }

  ret = i2c0Write_poll(APDS9960_I2C_ADDR, apds_i2c_seq_data_buf, len + 1);

  if (ret != i2cTransferDone)
  {
      return 1;
  }

  return 0;
}

uint8_t APDS_Read_nByte(uint8_t REG_Address,uint16_t len,uint8_t *buf)
{
  uint8_t write_data[1] = {0};

  // 1. Device addr + register addr + data
  I2C_TransferReturn_TypeDef ret;

  ret = APDS_Write_nByte(REG_Address, 0, write_data);
  if (ret != i2cTransferDone)
  {
      return 1;
  }

  memset(&apds_i2c_seq_data_buf, 0, sizeof(apds_i2c_seq_data_buf));

  ret = i2c0Read_poll(APDS9960_I2C_ADDR, apds_i2c_seq_data_buf, len);

  if (ret != i2cTransferDone)
  {
      return 1;
  }

  memcpy(buf, apds_i2c_seq_data_buf, len);

  return 0;
}


/**
* @brief Writes a single byte to the I2C device and specified register
*
* @param[in] reg the register in the I2C device to write to
* @param[in] val the 1-byte value to write to the I2C device
* @return True if successful write operation. False otherwise.
*/
bool wireWriteDataByte(uint8_t reg, uint8_t val)
{
  uint8_t buf = val;
  if (APDS_Write_nByte(reg, 1, &buf) != i2cTransferDone)
    return false;

  return true;
}

/**
* @brief Writes a block (array) of bytes to the I2C device and register
*
* @param[in] reg the register in the I2C device to write to
* @param[in] val pointer to the beginning of the data byte array
* @param[in] len the length (in bytes) of the data to write
* @return True if successful write operation. False otherwise.
*/
bool wireWriteDataBlock(uint8_t reg,
                        uint8_t *val,
                        unsigned int len)
{
  if (APDS_Write_nByte(reg, len, val) != i2cTransferDone)
    return false;

  return true;
}

/**
* @brief Reads a single byte from the I2C device and specified register
*
* @param[in] reg the register to read from
* @param[out] the value returned from the register
* @return True if successful read operation. False otherwise.
*/
bool wireReadDataByte(uint8_t reg, uint8_t *val)
{
  if (APDS_Read_nByte(reg, 1, val) != i2cTransferDone)
    return 0;

  return true;
}

/**
* @brief Reads a block (array) of bytes from the I2C device and register
*
* @param[in] reg the register to read from
* @param[out] val pointer to the beginning of the data
* @param[in] len number of bytes to read
* @return Number of bytes read. -1 on read error.
*/
int wireReadDataBlock(uint8_t reg,
                      uint8_t *val,
                      unsigned int len)
{
  if (APDS_Read_nByte(reg, len, val) != i2cTransferDone)
  {
    return -1;
  }

  return len;
}
