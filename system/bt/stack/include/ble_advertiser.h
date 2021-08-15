/******************************************************************************
 *
 *  Copyright 2016 The Android Open Source Project
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#ifndef BLE_ADVERTISER_H
#define BLE_ADVERTISER_H

#include <base/bind.h>
#include <base/memory/weak_ptr.h>
#include <vector>
#include "btm_ble_api.h"

#define BTM_BLE_MULTI_ADV_SUCCESS 0
#define BTM_BLE_MULTI_ADV_FAILURE 1
#define ADVERTISE_FAILED_TOO_MANY_ADVERTISERS 0x02

using MultiAdvCb = base::Callback<void(uint8_t /* status */)>;
using ParametersCb =
    base::Callback<void(uint8_t /* status */, int8_t /* tx_power */)>;

// methods we must have defined
void btm_ble_update_dmt_flag_bits(uint8_t* flag_value,
                                  const uint16_t connect_mode,
                                  const uint16_t disc_mode);
void btm_acl_update_conn_addr(uint16_t conn_handle, const RawAddress& address);

// methods we expose to c code:
void btm_ble_multi_adv_cleanup(void);
void btm_ble_multi_adv_init();

typedef struct {
  uint16_t advertising_event_properties;
  uint32_t adv_int_min;
  uint32_t adv_int_max;
  tBTM_BLE_ADV_CHNL_MAP channel_map;
  tBTM_BLE_AFP adv_filter_policy;
  int8_t tx_power;
  uint8_t primary_advertising_phy;
  uint8_t secondary_advertising_phy;
  uint8_t scan_request_notification_enable;
} tBTM_BLE_ADV_PARAMS;

typedef struct {
  uint8_t enable;
  uint16_t min_interval;
  uint16_t max_interval;
  uint16_t periodic_advertising_properties;
} tBLE_PERIODIC_ADV_PARAMS;

class BleAdvertiserHciInterface;

class BleAdvertisingManager {
 public:
  virtual ~BleAdvertisingManager() = default;

  static const uint16_t advertising_prop_legacy_connectable = 0x0011;
  static const uint16_t advertising_prop_legacy_non_connectable = 0x0010;

  static void Initialize(BleAdvertiserHciInterface* interface);
  static void CleanUp();
  static bool IsInitialized();
  static base::WeakPtr<BleAdvertisingManager> Get();

  /* Register an advertising instance, status will be returned in |cb|
   * callback, with assigned id, if operation succeeds. Instance is freed when
   * advertising is disabled by calling |BTM_BleDisableAdvInstance|, or when any
   * of the operations fails.
   * The instance will have data set to |advertise_data|, scan response set to
   * |scan_response_data|, and will be enabled.
   */
  virtual void StartAdvertising(uint8_t advertiser_id, MultiAdvCb cb,
                                tBTM_BLE_ADV_PARAMS* params,
                                std::vector<uint8_t> advertise_data,
                                std::vector<uint8_t> scan_response_data,
                                int duration, MultiAdvCb timeout_cb) = 0;

  /* Register an advertising instance, status will be returned in |cb|
   * callback, with assigned id, if operation succeeds. Instance is freed when
   * advertising is disabled by calling |BTM_BleDisableAdvInstance|, or when any
   * of the operations fails.
   * The instance will have data set to |advertise_data|, scan response set to
   * |scan_response_data|, periodic data set to |periodic_data| and will be
   * enabled.
   */
  virtual void StartAdvertisingSet(
      base::Callback<void(uint8_t /* inst_id */, int8_t /* tx_power */,
                          uint8_t /* status */)>
          cb,
      tBTM_BLE_ADV_PARAMS* params, std::vector<uint8_t> advertise_data,
      std::vector<uint8_t> scan_response_data,
      tBLE_PERIODIC_ADV_PARAMS* periodic_params,
      std::vector<uint8_t> periodic_data, uint16_t duration,
      uint8_t maxExtAdvEvents,
      base::Callback<void(uint8_t /* inst_id */, uint8_t /* status */)>
          timeout_cb) = 0;

  /* Register an advertising instance, status will be returned in |cb|
   * callback, with assigned id, if operation succeeds. Instance is freed when
   * advertising is disabled by calling |BTM_BleDisableAdvInstance|, or when any
   * of the operations fails. */
  virtual void RegisterAdvertiser(
      base::Callback<void(uint8_t /* inst_id */, uint8_t /* status */)>) = 0;

  /* This function enables/disables an advertising instance. Operation status is
   * returned in |cb| */
  virtual void Enable(uint8_t inst_id, bool enable, MultiAdvCb cb,
                      uint16_t duration, uint8_t maxExtAdvEvents,
                      MultiAdvCb timeout_cb) = 0;

  /* This function update a Multi-ADV instance with the specififed adv
   * parameters. */
  virtual void SetParameters(uint8_t inst_id, tBTM_BLE_ADV_PARAMS* p_params,
                             ParametersCb cb) = 0;

  /* This function configure a Multi-ADV instance with the specified adv data or
   * scan response data.*/
  virtual void SetData(uint8_t inst_id, bool is_scan_rsp,
                       std::vector<uint8_t> data, MultiAdvCb cb) = 0;

  /* This function configure instance with the specified periodic parameters */
  virtual void SetPeriodicAdvertisingParameters(
      uint8_t inst_id, tBLE_PERIODIC_ADV_PARAMS* params, MultiAdvCb cb) = 0;

  /* This function configure instance with the specified periodic data */
  virtual void SetPeriodicAdvertisingData(uint8_t inst_id,
                                          std::vector<uint8_t> data,
                                          MultiAdvCb cb) = 0;

  /* This function enables/disables periodic advertising on selected instance */
  virtual void SetPeriodicAdvertisingEnable(uint8_t inst_id, uint8_t enable,
                                            MultiAdvCb cb) = 0;

  /*  This function disable a Multi-ADV instance */
  virtual void Unregister(uint8_t inst_id) = 0;

  /* When resolving list is used, we need to suspend and resume all advertising
   * instances for the time of operation. Suspend() saves current state,
   * Resume() resumes the advertising.
   */
  virtual void Suspend() = 0;
  virtual void Resume() = 0;

  /* This method is a member of BleAdvertiserHciInterface, and is exposed here
   * just for tests. It should never be called from upper layers*/
  virtual void OnAdvertisingSetTerminated(
      uint8_t status, uint8_t advertising_handle, uint16_t connection_handle,
      uint8_t num_completed_extended_adv_events) = 0;

  using GetAddressCallback =
      base::Callback<void(uint8_t /* address_type*/, RawAddress /*address*/)>;
  virtual void GetOwnAddress(uint8_t inst_id, GetAddressCallback cb) = 0;
};

#endif  // BLE_ADVERTISER_H
