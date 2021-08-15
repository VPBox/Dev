/*
 * Copyright (C) 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_INCLUDE_BT_GATT_CLIENT_H
#define ANDROID_INCLUDE_BT_GATT_CLIENT_H

#include <stdint.h>
#include <vector>
#include "bt_common_types.h"
#include "bt_gatt_types.h"

#include <bluetooth/uuid.h>

__BEGIN_DECLS

/**
 * Buffer sizes for maximum attribute length and maximum read/write
 * operation buffer size.
 */
#define BTGATT_MAX_ATTR_LEN 600

/** Buffer type for unformatted reads/writes */
typedef struct {
  uint8_t value[BTGATT_MAX_ATTR_LEN];
  uint16_t len;
} btgatt_unformatted_value_t;

/** Parameters for GATT read operations */
typedef struct {
  uint16_t handle;
  btgatt_unformatted_value_t value;
  uint16_t value_type;
  uint8_t status;
} btgatt_read_params_t;

/** Parameters for GATT write operations */
typedef struct {
  btgatt_srvc_id_t srvc_id;
  btgatt_gatt_id_t char_id;
  btgatt_gatt_id_t descr_id;
  uint8_t status;
} btgatt_write_params_t;

/** Attribute change notification parameters */
typedef struct {
  uint8_t value[BTGATT_MAX_ATTR_LEN];
  RawAddress bda;
  uint16_t handle;
  uint16_t len;
  uint8_t is_notify;
} btgatt_notify_params_t;

typedef struct {
  RawAddress* bda1;
  bluetooth::Uuid* uuid1;
  uint16_t u1;
  uint16_t u2;
  uint16_t u3;
  uint16_t u4;
  uint16_t u5;
} btgatt_test_params_t;

/* BT GATT client error codes */
typedef enum {
  BT_GATTC_COMMAND_SUCCESS = 0, /* 0  Command succeeded                 */
  BT_GATTC_COMMAND_STARTED,     /* 1  Command started OK.               */
  BT_GATTC_COMMAND_BUSY,        /* 2  Device busy with another command  */
  BT_GATTC_COMMAND_STORED,      /* 3 request is stored in control block */
  BT_GATTC_NO_RESOURCES,        /* 4  No resources to issue command     */
  BT_GATTC_MODE_UNSUPPORTED,    /* 5  Request for 1 or more unsupported modes */
  BT_GATTC_ILLEGAL_VALUE,       /* 6  Illegal command /parameter value  */
  BT_GATTC_INCORRECT_STATE,     /* 7  Device in wrong state for request  */
  BT_GATTC_UNKNOWN_ADDR,        /* 8  Unknown remote BD address         */
  BT_GATTC_DEVICE_TIMEOUT,      /* 9  Device timeout                    */
  BT_GATTC_INVALID_CONTROLLER_OUTPUT, /* 10  An incorrect value was received
                                         from HCI */
  BT_GATTC_SECURITY_ERROR, /* 11 Authorization or security failure or not
                              authorized  */
  BT_GATTC_DELAYED_ENCRYPTION_CHECK, /*12 Delayed encryption check */
  BT_GATTC_ERR_PROCESSING            /* 12 Generic error                     */
} btgattc_error_t;

/** BT-GATT Client callback structure. */

/** Callback invoked in response to register_client */
typedef void (*register_client_callback)(int status, int client_if,
                                         const bluetooth::Uuid& app_uuid);

/** GATT open callback invoked in response to open */
typedef void (*connect_callback)(int conn_id, int status, int client_if,
                                 const RawAddress& bda);

/** Callback invoked in response to close */
typedef void (*disconnect_callback)(int conn_id, int status, int client_if,
                                    const RawAddress& bda);

/**
 * Invoked in response to search_service when the GATT service search
 * has been completed.
 */
typedef void (*search_complete_callback)(int conn_id, int status);

/** Callback invoked in response to [de]register_for_notification */
typedef void (*register_for_notification_callback)(int conn_id, int registered,
                                                   int status, uint16_t handle);

/**
 * Remote device notification callback, invoked when a remote device sends
 * a notification or indication that a client has registered for.
 */
typedef void (*notify_callback)(int conn_id,
                                const btgatt_notify_params_t& p_data);

/** Reports result of a GATT read operation */
typedef void (*read_characteristic_callback)(int conn_id, int status,
                                             btgatt_read_params_t* p_data);

/** GATT write characteristic operation callback */
typedef void (*write_characteristic_callback)(int conn_id, int status,
                                              uint16_t handle);

/** GATT execute prepared write callback */
typedef void (*execute_write_callback)(int conn_id, int status);

/** Callback invoked in response to read_descriptor */
typedef void (*read_descriptor_callback)(int conn_id, int status,
                                         const btgatt_read_params_t& p_data);

/** Callback invoked in response to write_descriptor */
typedef void (*write_descriptor_callback)(int conn_id, int status,
                                          uint16_t handle);

/** Callback triggered in response to read_remote_rssi */
typedef void (*read_remote_rssi_callback)(int client_if, const RawAddress& bda,
                                          int rssi, int status);

/** Callback invoked when the MTU for a given connection changes */
typedef void (*configure_mtu_callback)(int conn_id, int status, int mtu);

/**
 * Callback notifying an application that a remote device connection is
 * currently congested and cannot receive any more data. An application should
 * avoid sending more data until a further callback is received indicating the
 * congestion status has been cleared.
 */
typedef void (*congestion_callback)(int conn_id, bool congested);

/** GATT get database callback */
typedef void (*get_gatt_db_callback)(int conn_id, const btgatt_db_element_t* db,
                                     int count);

/** GATT services between start_handle and end_handle were removed */
typedef void (*services_removed_callback)(int conn_id, uint16_t start_handle,
                                          uint16_t end_handle);

/** GATT services were added */
typedef void (*services_added_callback)(int conn_id,
                                        const btgatt_db_element_t& added,
                                        int added_count);

/** Callback invoked when the PHY for a given connection changes */
typedef void (*phy_updated_callback)(int conn_id, uint8_t tx_phy,
                                     uint8_t rx_phy, uint8_t status);

/** Callback invoked when the connection parameters for a given connection
 * changes */
typedef void (*conn_updated_callback)(int conn_id, uint16_t interval,
                                      uint16_t latency, uint16_t timeout,
                                      uint8_t status);

typedef struct {
  register_client_callback register_client_cb;
  connect_callback open_cb;
  disconnect_callback close_cb;
  search_complete_callback search_complete_cb;
  register_for_notification_callback register_for_notification_cb;
  notify_callback notify_cb;
  read_characteristic_callback read_characteristic_cb;
  write_characteristic_callback write_characteristic_cb;
  read_descriptor_callback read_descriptor_cb;
  write_descriptor_callback write_descriptor_cb;
  execute_write_callback execute_write_cb;
  read_remote_rssi_callback read_remote_rssi_cb;
  configure_mtu_callback configure_mtu_cb;
  congestion_callback congestion_cb;
  get_gatt_db_callback get_gatt_db_cb;
  services_removed_callback services_removed_cb;
  services_added_callback services_added_cb;
  phy_updated_callback phy_updated_cb;
  conn_updated_callback conn_updated_cb;
} btgatt_client_callbacks_t;

/** Represents the standard BT-GATT client interface. */

typedef struct {
  /** Registers a GATT client application with the stack */
  bt_status_t (*register_client)(const bluetooth::Uuid& uuid);

  /** Unregister a client application from the stack */
  bt_status_t (*unregister_client)(int client_if);

  /** Create a connection to a remote LE or dual-mode device */
  bt_status_t (*connect)(int client_if, const RawAddress& bd_addr,
                         bool is_direct, int transport, bool opportunistic,
                         int initiating_phys);

  /** Disconnect a remote device or cancel a pending connection */
  bt_status_t (*disconnect)(int client_if, const RawAddress& bd_addr,
                            int conn_id);

  /** Clear the attribute cache for a given device */
  bt_status_t (*refresh)(int client_if, const RawAddress& bd_addr);

  /**
   * Enumerate all GATT services on a connected device.
   * Optionally, the results can be filtered for a given UUID.
   */
  bt_status_t (*search_service)(int conn_id,
                                const bluetooth::Uuid* filter_uuid);

  /**
   * Sead "Find service by UUID" request. Used only for PTS tests.
   */
  void (*btif_gattc_discover_service_by_uuid)(int conn_id,
                                              const bluetooth::Uuid& uuid);

  /** Read a characteristic on a remote device */
  bt_status_t (*read_characteristic)(int conn_id, uint16_t handle,
                                     int auth_req);

  /** Read a characteristic on a remote device */
  bt_status_t (*read_using_characteristic_uuid)(int conn_id,
                                                const bluetooth::Uuid& uuid,
                                                uint16_t s_handle,
                                                uint16_t e_handle,
                                                int auth_req);

  /** Write a remote characteristic */
  bt_status_t (*write_characteristic)(int conn_id, uint16_t handle,
                                      int write_type, int auth_req,
                                      std::vector<uint8_t> value);

  /** Read the descriptor for a given characteristic */
  bt_status_t (*read_descriptor)(int conn_id, uint16_t handle, int auth_req);

  /** Write a remote descriptor for a given characteristic */
  bt_status_t (*write_descriptor)(int conn_id, uint16_t handle, int auth_req,
                                  std::vector<uint8_t> value);

  /** Execute a prepared write operation */
  bt_status_t (*execute_write)(int conn_id, int execute);

  /**
   * Register to receive notifications or indications for a given
   * characteristic
   */
  bt_status_t (*register_for_notification)(int client_if,
                                           const RawAddress& bd_addr,
                                           uint16_t handle);

  /** Deregister a previous request for notifications/indications */
  bt_status_t (*deregister_for_notification)(int client_if,
                                             const RawAddress& bd_addr,
                                             uint16_t handle);

  /** Request RSSI for a given remote device */
  bt_status_t (*read_remote_rssi)(int client_if, const RawAddress& bd_addr);

  /** Determine the type of the remote device (LE, BR/EDR, Dual-mode) */
  int (*get_device_type)(const RawAddress& bd_addr);

  /** Configure the MTU for a given connection */
  bt_status_t (*configure_mtu)(int conn_id, int mtu);

  /** Request a connection parameter update */
  bt_status_t (*conn_parameter_update)(const RawAddress& bd_addr,
                                       int min_interval, int max_interval,
                                       int latency, int timeout,
                                       uint16_t min_ce_len,
                                       uint16_t max_ce_len);

  bt_status_t (*set_preferred_phy)(const RawAddress& bd_addr, uint8_t tx_phy,
                                   uint8_t rx_phy, uint16_t phy_options);

  bt_status_t (*read_phy)(
      const RawAddress& bd_addr,
      base::Callback<void(uint8_t tx_phy, uint8_t rx_phy, uint8_t status)> cb);

  /** Test mode interface */
  bt_status_t (*test_command)(int command, const btgatt_test_params_t& params);

  /** Get gatt db content */
  bt_status_t (*get_gatt_db)(int conn_id);

} btgatt_client_interface_t;

__END_DECLS

#endif /* ANDROID_INCLUDE_BT_GATT_CLIENT_H */
