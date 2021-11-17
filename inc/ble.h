/*
 * ble.h
 *
 *  Created on: Oct 2, 2021
 *      Author: han16
 */

#ifndef SRC_BLE_H_
#define SRC_BLE_H_

#include "gatt_db.h"
#include "sl_bt_api.h"

#include "app.h"

#include <stdint.h>
#include <stddef.h>

#define UINT8_TO_BITSTREAM(p, n) { *(p)++ = (uint8_t)(n); }
#define UINT32_TO_BITSTREAM(p, n) { *(p)++ = (uint8_t)(n); *(p)++ = (uint8_t)((n) >> 8); \
  *(p)++ = (uint8_t)((n) >> 16); *(p)++ = (uint8_t)((n) >> 24); }
#define UINT32_TO_FLOAT(m, e) (((uint32_t)(m) & 0x00FFFFFFU) | (uint32_t)((int32_t)(e) << 24))

enum bl_external_signal_t
{
  BT_EXT_SIG_IDLE = 0,
  BT_EXT_SIG_RIDAR_READY,
  BT_EXT_SIG_GESTURE_READY,
  NUM_OF_BT_EXT_SIG,
};


// BLE Data Structure, save all of our private BT data in here.
// Modern C (circa 2021 does it this way)
// typedef ble_data_struct_t is referred to as an anonymous struct definition
typedef struct {
  // values that are common to servers and clients
  bd_addr myAddress;
  uint8_t address_type;

  // values unique for server
  // The advertising set handle allocated from Bluetooth stack.
  uint8_t advertisingSetHandle;
  uint8_t app_connection;
  uint8_t app_connection_handle;

  // values unique for client

} ble_data_struct_t;

ble_data_struct_t* getBleDataPtr(void);

void handle_ble_event(sl_bt_msg_t *evt);

#endif /* SRC_BLE_H_ */
