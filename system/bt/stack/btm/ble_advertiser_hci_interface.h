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

#ifndef BLE_ADVERTISER_HCI_INTERFACE_H
#define BLE_ADVERTISER_HCI_INTERFACE_H

#include <vector>
#include "stack/include/bt_types.h"

#include <base/callback.h>

/* This class is an abstraction of HCI commands used for managing
 * advertisements. Please see VSC HCI SPEC at
 * https://static.googleusercontent.com/media/source.android.com/en//devices/Android-6.0-Bluetooth-HCI-Reqs.pdf
 * and Bluetooth 5.0 "Advertising Extension" feature for more details  */
class BleAdvertiserHciInterface {
 public:
  using status_cb = base::Callback<void(uint8_t /* status */)>;
  using parameters_cb =
      base::Callback<void(uint8_t /* status */, int8_t /* tx_power */)>;

  static void Initialize();
  static BleAdvertiserHciInterface* Get();
  static void CleanUp();

  virtual ~BleAdvertiserHciInterface() = default;

  class AdvertisingEventObserver {
   public:
    virtual ~AdvertisingEventObserver() = default;
    virtual void OnAdvertisingSetTerminated(
        uint8_t status, uint8_t advertising_handle, uint16_t connection_handle,
        uint8_t num_completed_extended_adv_events) = 0;
  };

  virtual void SetAdvertisingEventObserver(
      AdvertisingEventObserver* observer) = 0;
  virtual void ReadInstanceCount(
      base::Callback<void(uint8_t /* inst_cnt*/)> cb) = 0;
  virtual void SetParameters(
      uint8_t handle, uint16_t properties, uint32_t adv_int_min,
      uint32_t adv_int_max, uint8_t channel_map, uint8_t own_address_type,
      const RawAddress& own_address, uint8_t peer_address_type,
      const RawAddress& peer_address, uint8_t filter_policy, int8_t tx_power,
      uint8_t primary_phy, uint8_t secondary_max_skip, uint8_t secondary_phy,
      uint8_t advertising_sid, uint8_t scan_request_notify_enable,
      parameters_cb command_complete) = 0;
  virtual void SetAdvertisingData(uint8_t handle, uint8_t operation,
                                  uint8_t fragment_preference,
                                  uint8_t data_length, uint8_t* data,
                                  status_cb command_complete) = 0;
  virtual void SetScanResponseData(uint8_t handle, uint8_t operation,
                                   uint8_t fragment_preference,
                                   uint8_t scan_response_data_length,
                                   uint8_t* scan_response_data,
                                   status_cb command_complete) = 0;
  virtual void SetRandomAddress(uint8_t handle,
                                const RawAddress& random_address,
                                status_cb command_complete) = 0;

  struct SetEnableData {
    uint8_t handle;
    uint16_t duration;
    uint8_t max_extended_advertising_events;
  };
  virtual void Enable(uint8_t enable, std::vector<SetEnableData> sets,
                      status_cb command_complete) = 0;

  void Enable(uint8_t enable, uint8_t handle, uint16_t duration,
              uint8_t max_extended_advertising_events,
              status_cb command_complete) {
    std::vector<SetEnableData> enableData;
    enableData.emplace_back(SetEnableData{
        .handle = handle,
        .duration = duration,
        .max_extended_advertising_events = max_extended_advertising_events});
    Enable(enable, enableData, command_complete);
  };
  virtual void SetPeriodicAdvertisingParameters(uint8_t handle,
                                                uint16_t periodic_adv_int_min,
                                                uint16_t periodic_adv_int_max,
                                                uint16_t periodic_properties,
                                                status_cb command_complete) = 0;
  virtual void SetPeriodicAdvertisingData(uint8_t handle, uint8_t operation,
                                          uint8_t adv_data_length,
                                          uint8_t* adv_data,
                                          status_cb command_complete) = 0;
  virtual void SetPeriodicAdvertisingEnable(uint8_t enable, uint8_t handle,
                                            status_cb command_complete) = 0;
  virtual void RemoveAdvertisingSet(uint8_t handle,
                                    status_cb command_complete) = 0;

  // Some implementation don't behave well when handle value 0 is used.
  virtual bool QuirkAdvertiserZeroHandle() { return 0; }
};

#endif  // BLE_ADVERTISER_HCI_INTERFACE_H
