/*
 * ble.c
 *
 *  Created on: Oct 2, 2021
 *      Author: han16
 *
 *  Reference: soc_thermometer/app.c
 */

#include "app.h"
#include "ble.h"
#include "log.h"
#include "app_assert.h"

#include <string.h>


#define ADV_INTERVAL          (400)   // advertising interval (* 0.625ms = 250ms)
#define CONN_INTERVAL         (60)    // connection interval (* 1.25ms = 75ms)
#define SLAVE_LATENCY         (4)     // slave latency (* connection interval)
#define SUPERVISION_TIMEOUT   (200)   // (1+4)*(2*75ms)/(10ms)=75

#define VL53_TIMER_HANDLE     12
#define APDS_TIMER_HANDLE     13

// BLE private data
static ble_data_struct_t ble_data;

void ble_data_init(void)
{
  memset(&ble_data, 0, sizeof(ble_data));

  ble_data.advertisingSetHandle = 0xFF;
}

uint8_t ble_evt_get_connection_handle(sl_bt_msg_t *evt)
{
  return evt->data.evt_connection_opened.connection;
}

ble_data_struct_t* getBleDataPtr()
{
  return (&ble_data);
} // getBleDataPtr()

void handle_ble_event(sl_bt_msg_t *evt)
{
  sl_status_t sc;

  uint8_t system_id[8];


  switch (SL_BT_MSG_ID(evt->header)) {
      // -------------------------------
      // This event indicates the device has started and the radio is ready.
      // Do not call any stack command before receiving this boot event!
      case sl_bt_evt_system_boot_id:
      {
        // Extract unique ID from BT Address.
        sc = sl_bt_system_get_identity_address(&ble_data.myAddress, &ble_data.address_type);
        if (sc != SL_STATUS_OK)
        {
            LOG_ERROR("sl_bt_system_get_identity_address() returned != 0 status=0x%04x", (unsigned int) sc);
        }


        // Pad and reverse unique ID to get System ID.
        system_id[0] = ble_data.myAddress.addr[5];
        system_id[1] = ble_data.myAddress.addr[4];
        system_id[2] = ble_data.myAddress.addr[3];
        system_id[3] = 0xFF;
        system_id[4] = 0xFE;
        system_id[5] = ble_data.myAddress.addr[2];
        system_id[6] = ble_data.myAddress.addr[1];
        system_id[7] = ble_data.myAddress.addr[0];

        sc = sl_bt_gatt_server_write_attribute_value(gattdb_system_id,
                                                     0,
                                                     sizeof(system_id),
                                                     system_id);
        if (sc != SL_STATUS_OK)
        {
            LOG_ERROR("sl_bt_gatt_server_write_attribute_value() returned != 0 status=0x%04x", (unsigned int) sc);
        }


        // Create an advertising set.
        sc = sl_bt_advertiser_create_set(&ble_data.advertisingSetHandle);
        if (sc != SL_STATUS_OK)
        {
            LOG_ERROR("sl_bt_advertiser_create_set() returned != 0 status=0x%04x", (unsigned int) sc);
        }


        // Set advertising interval to 250ms.
        sc = sl_bt_advertiser_set_timing(
          ble_data.advertisingSetHandle,
          400, // min. adv. interval (milliseconds * 1.6)
          400, // max. adv. interval (milliseconds * 1.6)
          0,   // adv. duration
          0);  // max. num. adv. events
        if (sc != SL_STATUS_OK)
        {
            LOG_ERROR("sl_bt_advertiser_set_timing() returned != 0 status=0x%04x", (unsigned int) sc);
        }


        // Start general advertising and enable connections.
        sc = sl_bt_advertiser_start(
          ble_data.advertisingSetHandle,
          sl_bt_advertiser_general_discoverable,
          sl_bt_advertiser_connectable_scannable);
        if (sc != SL_STATUS_OK)
        {
            LOG_ERROR("sl_bt_advertiser_start() returned != 0 status=0x%04x", (unsigned int) sc);
        }


        // Customize events can be received from now on.
        schedulerSetEventIdle();

        break;
      }

      // -------------------------------
      // This event indicates that a new connection was opened.
      case sl_bt_evt_connection_opened_id:
      {
        LOG_INFO("Connection opened.\n\r");
        sc = sl_bt_advertiser_stop(ble_data.advertisingSetHandle);
        if (sc != SL_STATUS_OK)
        {
            LOG_ERROR("sl_bt_advertiser_stop() returned != 0 status=0x%04x", (unsigned int) sc);
        }

        sl_bt_connection_set_parameters(evt->data.evt_connection_opened.connection,
                                        CONN_INTERVAL,
                                        CONN_INTERVAL,
                                        SLAVE_LATENCY,
                                        SUPERVISION_TIMEOUT,
                                        0,
                                        0);

        ble_data.app_connection = true;
        ble_data.app_connection_handle = ble_evt_get_connection_handle(evt);
        break;
      }

      // -------------------------------
      // This event indicates that a connection was closed.
      case sl_bt_evt_connection_closed_id:
      {
        LOG_INFO("Connection closed.\n\r");

        // Restart advertising after client has disconnected.
        sc = sl_bt_advertiser_start(
          ble_data.advertisingSetHandle,
          sl_bt_advertiser_general_discoverable,
          sl_bt_advertiser_connectable_scannable);
        if (sc != SL_STATUS_OK)
        {
            LOG_ERROR("sl_bt_advertiser_start() returned != 0 status=0x%04x", (unsigned int) sc);
        }

        // stop distance sensor
        if (sl_bt_system_set_soft_timer(32768, VL53_TIMER_HANDLE,1) != SL_STATUS_OK) {
            LOG_ERROR("soft timer error\r\n");
        }

        schedulerSetEventIdle();
        vl_set_flag_enable(false);
        gest_set_flag_enable(false);

        ble_data.app_connection = false;
        ble_data.app_connection_handle = ble_evt_get_connection_handle(evt);
      }
      break;

      case sl_bt_evt_connection_parameters_id:
      {
        // log parameters
        LOG_INFO( "CONNECTION INTERVAL: %d\n\r", evt->data.evt_connection_parameters.interval );
        LOG_INFO( "SLAVE LATENCY: %d\n\r", evt->data.evt_connection_parameters.latency );
        LOG_INFO( "TIMEOUT: %d\n\r", evt->data.evt_connection_parameters.timeout );
        break;
      }

      case sl_bt_evt_system_external_signal_id:
      {
        if (evt->data.evt_system_external_signal.extsignals == BT_EXT_SIG_RIDAR_READY) {
            uint16_t dist_val = vl_get_result();
            uint8_t buf[2] = {0};
            buf[1] = dist_val & 0x00FF;
            buf[0] = (dist_val & 0xFF00) >> 8;

            if (ble_data.app_connection) {

              sc = sl_bt_gatt_server_write_attribute_value(gattdb_Distance, 0, sizeof(buf), buf);
              if (sc != SL_STATUS_OK)
              {
                  LOG_ERROR("sl_bt_gatt_server_send_indication() returned != 0 status=0x%04x", (unsigned int) sc);
              }
//
//              if (sl_bt_system_set_soft_timer(32768, VL53_TIMER_HANDLE,1) != SL_STATUS_OK) {
//                  LOG_ERROR("soft timer error\r\n");
//              }
            }

            app_log("distance: %d mm\r\n", dist_val);
        }
        break;
      }

      case sl_bt_evt_system_soft_timer_id:
      {
        if (evt->data.evt_system_soft_timer.handle == VL53_TIMER_HANDLE) {
            if (!vl_get_flag_enable() && !gest_get_flag_enable()) {
                schedulerSetEventReadDistance();
                vl_set_flag_enable(true);
            }
        }
//        else if (evt->data.evt_system_soft_timer.handle == APDS_TIMER_HANDLE) {
//            if (!vl_get_flag_enable() && !gest_get_flag_enable()) {
//                schedulerSetEventReadGesture();
//                gest_set_flag_enable(true);
//            }
//        }
      }
          break;

      // -------------------------------
      // This event indicates that the value of an attribute in the local GATT
      // database was changed by a remote GATT client.
      case sl_bt_evt_gatt_server_attribute_value_id:
        // The value of characteristic was changed.
        if (gattdb_distance_en == evt->data.evt_gatt_server_characteristic_status.characteristic) {
          uint8_t data_recv;
          size_t data_recv_len;

          // Read characteristic value.
          sc = sl_bt_gatt_server_read_attribute_value(gattdb_distance_en,
                                                      0,
                                                      sizeof(data_recv),
                                                      &data_recv_len,
                                                      &data_recv);
          (void)data_recv_len;
          app_log_status_error(sc);

          if (sc != SL_STATUS_OK) {
            break;
          }

          // Toggle LED.
          if (data_recv == 0x00) {
              // stop distance sensor
              if (sl_bt_system_set_soft_timer(32768, VL53_TIMER_HANDLE,1) != SL_STATUS_OK) {
                  LOG_ERROR("soft timer error\r\n");
              }

            schedulerSetEventIdle();
            vl_set_flag_enable(false);
            gest_set_flag_enable(false);

            LOG_INFO("Sensor off.");
          } else if (data_recv == 0x01) {
              // 1s per read for distance sensor
                if (sl_bt_system_set_soft_timer(32768, VL53_TIMER_HANDLE,0) != SL_STATUS_OK) {
                    LOG_ERROR("soft timer error\r\n");
                }

                //schedulerSetEventIdle();
                schedulerSetEventReadDistance();
                vl_set_flag_enable(true);
                gest_set_flag_enable(true);

              LOG_INFO("Sensor on.");
          } else {
              LOG_ERROR("Invalid attribute value: 0x%02x", (int)data_recv);
          }
        }
        break;

      // -------------------------------
      // This event occurs when the remote device enabled or disabled the
      // notification.
      case sl_bt_evt_gatt_server_characteristic_status_id:
      {
        //if (evt->data.evt_gatt_server_characteristic_status.characteristic == )
        {
         // do some cool stuff
        }
        break;
      }

      ///////////////////////////////////////////////////////////////////////////
      // Add additional event handlers here as your application requires!      //
      ///////////////////////////////////////////////////////////////////////////
      case sl_bt_evt_gatt_server_indication_timeout_id:
      {
        // Possible event from calling sl_bt_gatt_server_send_indication() -
        // i.e. we never received a confirmation for a previously transmitted indication.
        if (evt->data.evt_gatt_server_indication_timeout.connection)
        {
            LOG_INFO("Timeout!\n\r");
        }
        break;
      }

      // -------------------------------
      // Default event handler.
      default:
        break;
  }


}


