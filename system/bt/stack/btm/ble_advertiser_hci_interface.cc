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

#include "ble_advertiser_hci_interface.h"
#include "btm_api.h"
#include "btm_ble_api.h"
#include "btm_int_types.h"
#include "device/include/controller.h"
#include "hcidefs.h"
#include "log/log.h"

#include <queue>
#include <utility>

#include <base/bind.h>
#include <base/callback.h>
#include <base/location.h>
#include <base/logging.h>

#define BTM_BLE_MULTI_ADV_SET_RANDOM_ADDR_LEN 8
#define BTM_BLE_MULTI_ADV_ENB_LEN 3
#define BTM_BLE_MULTI_ADV_SET_PARAM_LEN 24
#define BTM_BLE_AD_DATA_LEN 31
#define BTM_BLE_MULTI_ADV_WRITE_DATA_LEN (BTM_BLE_AD_DATA_LEN + 3)

#define HCIC_PARAM_SIZE_WRITE_ADV_ENABLE 1
#define HCIC_PARAM_SIZE_WRITE_RANDOM_ADDR_CMD 6
#define HCIC_PARAM_SIZE_BLE_WRITE_ADV_PARAMS 15
#define HCIC_PARAM_SIZE_BLE_WRITE_SCAN_RSP 31
#define HCIC_PARAM_SIZE_BLE_WRITE_ADV_DATA 31

using status_cb = BleAdvertiserHciInterface::status_cb;

using hci_cmd_cb = base::OnceCallback<void(
    uint8_t* /* return_parameters */, uint16_t /* return_parameters_length*/)>;
extern void btu_hcif_send_cmd_with_cb(const base::Location& posted_from,
                                      uint16_t opcode, uint8_t* params,
                                      uint8_t params_len, hci_cmd_cb cb);

namespace {
BleAdvertiserHciInterface* instance = nullptr;

void btm_ble_multi_adv_vsc_cmpl_cback(uint8_t expected_opcode,
                                      status_cb command_complete,
                                      uint8_t* param, uint16_t param_len) {
  uint8_t status, subcode;

  // All multi-adv commands respond with status and inst_id.
  LOG_ASSERT(param_len == 2) << "Received bad response length to multi-adv VSC";

  STREAM_TO_UINT8(status, param);
  STREAM_TO_UINT8(subcode, param);

  VLOG(1) << "subcode = " << +subcode << ", status: " << +status;

  if (expected_opcode != subcode) {
    LOG(ERROR) << "unexpected VSC cmpl, expect: " << +subcode
               << " get: " << +expected_opcode;
    return;
  }

  command_complete.Run(status);
}

void parameters_response_parser(BleAdvertiserHciInterface::parameters_cb cb,
                                uint8_t* ret_params, uint16_t ret_params_len) {
  uint8_t status;
  int8_t tx_power;

  uint8_t* pp = ret_params;
  STREAM_TO_UINT8(status, pp);
  STREAM_TO_INT8(tx_power, pp);

  cb.Run(status, tx_power);
}

void known_tx_pwr(BleAdvertiserHciInterface::parameters_cb cb, int8_t tx_power,
                  uint8_t status) {
  cb.Run(status, tx_power);
}

class BleAdvertiserVscHciInterfaceImpl : public BleAdvertiserHciInterface {
  void SendAdvCmd(const base::Location& posted_from, uint8_t param_len,
                  uint8_t* param_buf, status_cb command_complete) {
    btu_hcif_send_cmd_with_cb(posted_from, HCI_BLE_MULTI_ADV_OCF, param_buf,
                              param_len,
                              base::Bind(&btm_ble_multi_adv_vsc_cmpl_cback,
                                         param_buf[0], command_complete));
  }

  void ReadInstanceCount(
      base::Callback<void(uint8_t /* inst_cnt*/)> cb) override {
    cb.Run(BTM_BleMaxMultiAdvInstanceCount());
  }

  void SetAdvertisingEventObserver(
      AdvertisingEventObserver* observer) override {
    this->advertising_event_observer = observer;
  }

  void SetParameters(uint8_t handle, uint16_t properties, uint32_t adv_int_min,
                     uint32_t adv_int_max, uint8_t channel_map,
                     uint8_t own_address_type, const RawAddress& own_address,
                     uint8_t peer_address_type, const RawAddress& peer_address,
                     uint8_t filter_policy, int8_t tx_power,
                     uint8_t primary_phy, uint8_t secondary_max_skip,
                     uint8_t secondary_phy, uint8_t advertising_sid,
                     uint8_t scan_request_notify_enable,
                     parameters_cb command_complete) override {
    VLOG(1) << __func__;
    uint8_t param[BTM_BLE_MULTI_ADV_SET_PARAM_LEN];
    memset(param, 0, BTM_BLE_MULTI_ADV_SET_PARAM_LEN);

    uint8_t* pp = param;
    UINT8_TO_STREAM(pp, BTM_BLE_MULTI_ADV_SET_PARAM);
    UINT16_TO_STREAM(pp, adv_int_min);
    UINT16_TO_STREAM(pp, adv_int_max);

    if (properties == 0x0013) {
      UINT8_TO_STREAM(pp, 0x00);  // ADV_IND
    } else if (properties == 0x0012) {
      UINT8_TO_STREAM(pp, 0x02);  // ADV_SCAN_IND
    } else if (properties == 0x0010) {
      UINT8_TO_STREAM(pp, 0x03);  // ADV_NONCONN_IND
    } else {
      LOG(ERROR) << "Unsupported advertisement type selected:" << std::hex
                 << properties;
      command_complete.Run(HCI_ERR_ILLEGAL_PARAMETER_FMT, 0);
      return;
    }

    UINT8_TO_STREAM(pp, own_address_type);
    BDADDR_TO_STREAM(pp, own_address);
    UINT8_TO_STREAM(pp, peer_address_type);
    BDADDR_TO_STREAM(pp, peer_address);
    UINT8_TO_STREAM(pp, channel_map);
    UINT8_TO_STREAM(pp, filter_policy);
    UINT8_TO_STREAM(pp, handle);
    INT8_TO_STREAM(pp, tx_power);

    SendAdvCmd(
        FROM_HERE, BTM_BLE_MULTI_ADV_SET_PARAM_LEN, param,
        base::Bind(&known_tx_pwr, std::move(command_complete), tx_power));
  }

  void SetAdvertisingData(uint8_t handle, uint8_t operation,
                          uint8_t fragment_preference, uint8_t data_length,
                          uint8_t* data, status_cb command_complete) override {
    VLOG(1) << __func__;
    uint8_t param[BTM_BLE_MULTI_ADV_WRITE_DATA_LEN];
    memset(param, 0, BTM_BLE_MULTI_ADV_WRITE_DATA_LEN);

    if (data_length > BTM_BLE_AD_DATA_LEN) {
      android_errorWriteLog(0x534e4554, "121145627");
      LOG(ERROR) << __func__
                 << ": data_length=" << static_cast<int>(data_length)
                 << ", is longer than size limit " << BTM_BLE_AD_DATA_LEN;
      data_length = BTM_BLE_AD_DATA_LEN;
    }

    uint8_t* pp = param;
    UINT8_TO_STREAM(pp, BTM_BLE_MULTI_ADV_WRITE_ADV_DATA);
    UINT8_TO_STREAM(pp, data_length);
    ARRAY_TO_STREAM(pp, data, data_length);
    param[BTM_BLE_MULTI_ADV_WRITE_DATA_LEN - 1] = handle;

    SendAdvCmd(FROM_HERE, (uint8_t)BTM_BLE_MULTI_ADV_WRITE_DATA_LEN, param,
               command_complete);
  }

  void SetScanResponseData(uint8_t handle, uint8_t operation,
                           uint8_t fragment_preference,
                           uint8_t scan_response_data_length,
                           uint8_t* scan_response_data,
                           status_cb command_complete) override {
    VLOG(1) << __func__;
    uint8_t param[BTM_BLE_MULTI_ADV_WRITE_DATA_LEN];
    memset(param, 0, BTM_BLE_MULTI_ADV_WRITE_DATA_LEN);

    if (scan_response_data_length > BTM_BLE_AD_DATA_LEN) {
      android_errorWriteLog(0x534e4554, "121145627");
      LOG(ERROR) << __func__ << ": scan_response_data_length="
                 << static_cast<int>(scan_response_data_length)
                 << ", is longer than size limit " << BTM_BLE_AD_DATA_LEN;
      scan_response_data_length = BTM_BLE_AD_DATA_LEN;
    }

    uint8_t* pp = param;
    UINT8_TO_STREAM(pp, BTM_BLE_MULTI_ADV_WRITE_SCAN_RSP_DATA);
    UINT8_TO_STREAM(pp, scan_response_data_length);
    ARRAY_TO_STREAM(pp, scan_response_data, scan_response_data_length);
    param[BTM_BLE_MULTI_ADV_WRITE_DATA_LEN - 1] = handle;

    SendAdvCmd(FROM_HERE, (uint8_t)BTM_BLE_MULTI_ADV_WRITE_DATA_LEN, param,
               command_complete);
  }

  void SetRandomAddress(uint8_t handle, const RawAddress& random_address,
                        status_cb command_complete) override {
    VLOG(1) << __func__;
    uint8_t param[BTM_BLE_MULTI_ADV_SET_RANDOM_ADDR_LEN];
    memset(param, 0, BTM_BLE_MULTI_ADV_SET_RANDOM_ADDR_LEN);

    uint8_t* pp = param;
    UINT8_TO_STREAM(pp, BTM_BLE_MULTI_ADV_SET_RANDOM_ADDR);
    BDADDR_TO_STREAM(pp, random_address);
    UINT8_TO_STREAM(pp, handle);

    SendAdvCmd(FROM_HERE, (uint8_t)BTM_BLE_MULTI_ADV_SET_RANDOM_ADDR_LEN, param,
               command_complete);
  }

  void Enable(uint8_t enable, std::vector<SetEnableData> sets,
              status_cb command_complete) override {
    VLOG(1) << __func__;

    if (sets.size() != 1) {
      LOG(ERROR) << "Trying to enable multiple sets in VSC implemenetation!";
      command_complete.Run(HCI_ERR_ILLEGAL_PARAMETER_FMT);
      return;
    }
    SetEnableData& set = sets[0];

    if (set.max_extended_advertising_events) {
      command_complete.Run(HCI_ERR_ILLEGAL_PARAMETER_FMT);
      return;
    }

    uint8_t param[BTM_BLE_MULTI_ADV_ENB_LEN];
    memset(param, 0, BTM_BLE_MULTI_ADV_ENB_LEN);

    uint8_t* pp = param;
    UINT8_TO_STREAM(pp, BTM_BLE_MULTI_ADV_ENB);
    UINT8_TO_STREAM(pp, enable);
    UINT8_TO_STREAM(pp, set.handle);

    SendAdvCmd(FROM_HERE, (uint8_t)BTM_BLE_MULTI_ADV_ENB_LEN, param,
               command_complete);
  }

  void SetPeriodicAdvertisingParameters(uint8_t, uint16_t, uint16_t, uint16_t,
                                        status_cb command_complete) override {
    LOG(INFO) << __func__ << " VSC can't do periodic advertising";
    command_complete.Run(HCI_ERR_ILLEGAL_COMMAND);
  }

  void SetPeriodicAdvertisingData(uint8_t, uint8_t, uint8_t, uint8_t*,
                                  status_cb command_complete) override {
    LOG(INFO) << __func__ << " VSC can't do periodic advertising";
    command_complete.Run(HCI_ERR_ILLEGAL_COMMAND);
  }

  void SetPeriodicAdvertisingEnable(uint8_t, uint8_t,
                                    status_cb command_complete) override {
    LOG(INFO) << __func__ << " VSC can't do periodic advertising";
    command_complete.Run(HCI_ERR_ILLEGAL_COMMAND);
  }

  bool QuirkAdvertiserZeroHandle() override {
    // Android BT HCI Requirements version 0.96 and below specify that handle 0
    // is equal to standard HCI interface, and should be accessed using non-VSC
    // commands.
    LOG(INFO) << "QuirkAdvertiserZeroHandle in use";
    return true;
  }

  void RemoveAdvertisingSet(uint8_t handle,
                            status_cb command_complete) override {
    // VSC Advertising don't have remove method.
    command_complete.Run(0);
  }

 public:
  static void VendorSpecificEventCback(uint8_t length, uint8_t* p) {
    VLOG(1) << __func__;

    LOG_ASSERT(p);
    uint8_t sub_event, adv_inst, change_reason;
    uint16_t conn_handle;

    STREAM_TO_UINT8(sub_event, p);
    length--;

    if (sub_event != HCI_VSE_SUBCODE_BLE_MULTI_ADV_ST_CHG || length != 4) {
      return;
    }

    STREAM_TO_UINT8(adv_inst, p);
    STREAM_TO_UINT8(change_reason, p);
    STREAM_TO_UINT16(conn_handle, p);

    AdvertisingEventObserver* observer =
        ((BleAdvertiserVscHciInterfaceImpl*)BleAdvertiserHciInterface::Get())
            ->advertising_event_observer;
    if (observer)
      observer->OnAdvertisingSetTerminated(change_reason, adv_inst, conn_handle,
                                           0x00);
  }

 private:
  AdvertisingEventObserver* advertising_event_observer = nullptr;
};

void adv_cmd_cmpl_cback(status_cb cb, uint8_t* return_parameters,
                        uint16_t return_parameters_length) {
  uint8_t status = *return_parameters;
  cb.Run(status);
}

class BleAdvertiserLegacyHciInterfaceImpl : public BleAdvertiserHciInterface {
  void SendAdvCmd(const base::Location& posted_from, uint16_t opcode,
                  uint8_t* param_buf, uint8_t param_buf_len,
                  status_cb command_complete) {
    btu_hcif_send_cmd_with_cb(
        posted_from, opcode, param_buf, param_buf_len,
        base::Bind(&adv_cmd_cmpl_cback, command_complete));
  }

  void ReadInstanceCount(
      base::Callback<void(uint8_t /* inst_cnt*/)> cb) override {
    cb.Run(1);
  }

  void SetAdvertisingEventObserver(
      AdvertisingEventObserver* observer) override {
    this->advertising_event_observer = observer;
  }

  void SetParameters(uint8_t handle, uint16_t properties, uint32_t adv_int_min,
                     uint32_t adv_int_max, uint8_t channel_map,
                     uint8_t own_address_type,
                     const RawAddress& /* own_address */,
                     uint8_t peer_address_type, const RawAddress& peer_address,
                     uint8_t filter_policy, int8_t tx_power,
                     uint8_t primary_phy, uint8_t secondary_max_skip,
                     uint8_t secondary_phy, uint8_t advertising_sid,
                     uint8_t scan_request_notify_enable,
                     parameters_cb command_complete) override {
    VLOG(1) << __func__;

    uint8_t param[HCIC_PARAM_SIZE_BLE_WRITE_ADV_PARAMS];

    uint8_t* pp = param;
    UINT16_TO_STREAM(pp, adv_int_min);
    UINT16_TO_STREAM(pp, adv_int_max);

    if (properties == 0x0013) {
      UINT8_TO_STREAM(pp, 0x00);  // ADV_IND
    } else if (properties == 0x0012) {
      UINT8_TO_STREAM(pp, 0x02);  // ADV_SCAN_IND
    } else if (properties == 0x0010) {
      UINT8_TO_STREAM(pp, 0x03);  // ADV_NONCONN_IND
    } else {
      LOG(ERROR) << "Unsupported advertisement type selected:" << std::hex
                 << properties;
      command_complete.Run(HCI_ERR_ILLEGAL_PARAMETER_FMT, 0);
      return;
    }

    UINT8_TO_STREAM(pp, own_address_type);
    UINT8_TO_STREAM(pp, peer_address_type);
    BDADDR_TO_STREAM(pp, peer_address);
    UINT8_TO_STREAM(pp, channel_map);
    UINT8_TO_STREAM(pp, filter_policy);

    SendAdvCmd(
        FROM_HERE, HCI_BLE_WRITE_ADV_PARAMS, param,
        HCIC_PARAM_SIZE_BLE_WRITE_ADV_PARAMS,
        base::Bind(&known_tx_pwr, std::move(command_complete), (int8_t)0));
  }

  void SetAdvertisingData(uint8_t handle, uint8_t operation,
                          uint8_t fragment_preference, uint8_t data_length,
                          uint8_t* data, status_cb command_complete) override {
    VLOG(1) << __func__;

    uint8_t param[HCIC_PARAM_SIZE_BLE_WRITE_ADV_DATA + 1];

    if (data_length > HCIC_PARAM_SIZE_BLE_WRITE_ADV_DATA) {
      android_errorWriteLog(0x534e4554, "121145627");
      LOG(ERROR) << __func__
                 << ": data_length=" << static_cast<int>(data_length)
                 << ", is longer than size limit "
                 << HCIC_PARAM_SIZE_BLE_WRITE_ADV_DATA;
      data_length = HCIC_PARAM_SIZE_BLE_WRITE_ADV_DATA;
    }

    uint8_t* pp = param;
    memset(pp, 0, HCIC_PARAM_SIZE_BLE_WRITE_ADV_DATA + 1);
    UINT8_TO_STREAM(pp, data_length);
    ARRAY_TO_STREAM(pp, data, data_length);

    SendAdvCmd(FROM_HERE, HCI_BLE_WRITE_ADV_DATA, param,
               HCIC_PARAM_SIZE_BLE_WRITE_ADV_DATA + 1, command_complete);
  }

  void SetScanResponseData(uint8_t handle, uint8_t operation,
                           uint8_t fragment_preference,
                           uint8_t scan_response_data_length,
                           uint8_t* scan_response_data,
                           status_cb command_complete) override {
    VLOG(1) << __func__;
    uint8_t param[HCIC_PARAM_SIZE_BLE_WRITE_ADV_DATA + 1];

    if (scan_response_data_length > HCIC_PARAM_SIZE_BLE_WRITE_ADV_DATA) {
      android_errorWriteLog(0x534e4554, "121145627");
      LOG(ERROR) << __func__ << ": scan_response_data_length="
                 << static_cast<int>(scan_response_data_length)
                 << ", is longer than size limit "
                 << HCIC_PARAM_SIZE_BLE_WRITE_ADV_DATA;
      scan_response_data_length = HCIC_PARAM_SIZE_BLE_WRITE_ADV_DATA;
    }

    uint8_t* pp = param;
    memset(pp, 0, HCIC_PARAM_SIZE_BLE_WRITE_ADV_DATA + 1);
    UINT8_TO_STREAM(pp, scan_response_data_length);
    ARRAY_TO_STREAM(pp, scan_response_data, scan_response_data_length);

    SendAdvCmd(FROM_HERE, HCI_BLE_WRITE_SCAN_RSP_DATA, param,
               HCIC_PARAM_SIZE_BLE_WRITE_ADV_DATA + 1, command_complete);
  }

  void SetRandomAddress(uint8_t handle, const RawAddress& random_address,
                        status_cb command_complete) override {
    VLOG(1) << __func__;

    uint8_t param[HCIC_PARAM_SIZE_WRITE_RANDOM_ADDR_CMD];

    uint8_t* pp = param;
    BDADDR_TO_STREAM(pp, random_address);

    SendAdvCmd(FROM_HERE, HCI_BLE_WRITE_RANDOM_ADDR, param,
               HCIC_PARAM_SIZE_WRITE_RANDOM_ADDR_CMD, command_complete);
  }

  void Enable(uint8_t enable, std::vector<SetEnableData> sets,
              status_cb command_complete) override {
    VLOG(1) << __func__;

    if (sets.size() != 1) {
      LOG(ERROR) << "Trying to enable multiple sets in legacy implemenetation!";
      command_complete.Run(HCI_ERR_ILLEGAL_PARAMETER_FMT);
      return;
    }

    SetEnableData& set = sets[0];
    if (set.max_extended_advertising_events) {
      command_complete.Run(HCI_ERR_ILLEGAL_PARAMETER_FMT);
      return;
    }

    uint8_t param[HCIC_PARAM_SIZE_WRITE_ADV_ENABLE];

    uint8_t* pp = param;
    UINT8_TO_STREAM(pp, enable);

    SendAdvCmd(FROM_HERE, HCI_BLE_WRITE_ADV_ENABLE, param,
               HCIC_PARAM_SIZE_WRITE_ADV_ENABLE, command_complete);
  }

  void SetPeriodicAdvertisingParameters(uint8_t, uint16_t, uint16_t, uint16_t,
                                        status_cb command_complete) override {
    LOG(INFO) << __func__ << "Legacy can't do periodic advertising";
    command_complete.Run(HCI_ERR_ILLEGAL_COMMAND);
  }

  void SetPeriodicAdvertisingData(uint8_t, uint8_t, uint8_t, uint8_t*,
                                  status_cb command_complete) override {
    LOG(INFO) << __func__ << "Legacy can't do periodic advertising";
    command_complete.Run(HCI_ERR_ILLEGAL_COMMAND);
  }

  void SetPeriodicAdvertisingEnable(uint8_t, uint8_t,
                                    status_cb command_complete) override {
    LOG(INFO) << __func__ << "Legacy can't do periodic advertising";
    command_complete.Run(HCI_ERR_ILLEGAL_COMMAND);
  }

  void RemoveAdvertisingSet(uint8_t handle,
                            status_cb command_complete) override {
    // Legacy Advertising don't have remove method.
    command_complete.Run(0);
  }

 public:
  void OnAdvertisingSetTerminated(uint8_t status, uint16_t connection_handle) {
    VLOG(1) << __func__;

    AdvertisingEventObserver* observer = this->advertising_event_observer;
    if (observer)
      observer->OnAdvertisingSetTerminated(status, 0 /*advertising_handle*/,
                                           connection_handle, 0);
  }

 private:
  AdvertisingEventObserver* advertising_event_observer = nullptr;
};

class BleAdvertiserHciExtendedImpl : public BleAdvertiserHciInterface {
  void SendAdvCmd(const base::Location& posted_from, uint16_t opcode,
                  uint8_t* param_buf, uint8_t param_buf_len,
                  status_cb command_complete) {
    btu_hcif_send_cmd_with_cb(
        posted_from, opcode, param_buf, param_buf_len,
        base::Bind(&adv_cmd_cmpl_cback, command_complete));
  }

  void ReadInstanceCount(
      base::Callback<void(uint8_t /* inst_cnt*/)> cb) override {
    cb.Run(controller_get_interface()
               ->get_ble_number_of_supported_advertising_sets());
  }

  void SetAdvertisingEventObserver(
      AdvertisingEventObserver* observer) override {
    this->advertising_event_observer = observer;
  }

  void SetParameters(uint8_t handle, uint16_t properties, uint32_t adv_int_min,
                     uint32_t adv_int_max, uint8_t channel_map,
                     uint8_t own_address_type,
                     const RawAddress& /* own_address */,
                     uint8_t peer_address_type, const RawAddress& peer_address,
                     uint8_t filter_policy, int8_t tx_power,
                     uint8_t primary_phy, uint8_t secondary_max_skip,
                     uint8_t secondary_phy, uint8_t advertising_sid,
                     uint8_t scan_request_notify_enable,
                     parameters_cb command_complete) override {
    VLOG(1) << __func__;
    const uint16_t HCI_LE_SET_EXT_ADVERTISING_PARAM_LEN = 25;
    uint8_t param[HCI_LE_SET_EXT_ADVERTISING_PARAM_LEN];
    memset(param, 0, HCI_LE_SET_EXT_ADVERTISING_PARAM_LEN);

    uint8_t* pp = param;
    UINT8_TO_STREAM(pp, handle);
    UINT16_TO_STREAM(pp, properties);
    UINT24_TO_STREAM(pp, adv_int_min);
    UINT24_TO_STREAM(pp, adv_int_max);
    UINT8_TO_STREAM(pp, channel_map);
    UINT8_TO_STREAM(pp, own_address_type);
    UINT8_TO_STREAM(pp, peer_address_type);
    BDADDR_TO_STREAM(pp, peer_address);
    UINT8_TO_STREAM(pp, filter_policy);
    INT8_TO_STREAM(pp, tx_power);
    UINT8_TO_STREAM(pp, primary_phy);
    UINT8_TO_STREAM(pp, secondary_max_skip);
    UINT8_TO_STREAM(pp, secondary_phy);
    UINT8_TO_STREAM(pp, advertising_sid);
    UINT8_TO_STREAM(pp, scan_request_notify_enable);

    btu_hcif_send_cmd_with_cb(
        FROM_HERE, HCI_LE_SET_EXT_ADVERTISING_PARAM, param,
        HCI_LE_SET_EXT_ADVERTISING_PARAM_LEN,
        base::Bind(parameters_response_parser, std::move(command_complete)));
  }

  void SetAdvertisingData(uint8_t handle, uint8_t operation,
                          uint8_t fragment_preference, uint8_t data_length,
                          uint8_t* data, status_cb command_complete) override {
    VLOG(1) << __func__;

    const uint16_t cmd_length = 4 + data_length;
    uint8_t param[cmd_length];
    memset(param, 0, cmd_length);

    uint8_t* pp = param;
    UINT8_TO_STREAM(pp, handle);
    UINT8_TO_STREAM(pp, operation);
    UINT8_TO_STREAM(pp, fragment_preference);
    UINT8_TO_STREAM(pp, data_length);
    ARRAY_TO_STREAM(pp, data, data_length);

    SendAdvCmd(FROM_HERE, HCI_LE_SET_EXT_ADVERTISING_DATA, param, cmd_length,
               command_complete);
  }

  void SetScanResponseData(uint8_t handle, uint8_t operation,
                           uint8_t fragment_preference,
                           uint8_t scan_response_data_length,
                           uint8_t* scan_response_data,
                           status_cb command_complete) override {
    VLOG(1) << __func__;

    const uint16_t cmd_length = 4 + scan_response_data_length;
    uint8_t param[cmd_length];
    memset(param, 0, cmd_length);

    uint8_t* pp = param;
    UINT8_TO_STREAM(pp, handle);
    UINT8_TO_STREAM(pp, operation);
    UINT8_TO_STREAM(pp, fragment_preference);
    UINT8_TO_STREAM(pp, scan_response_data_length);
    ARRAY_TO_STREAM(pp, scan_response_data, scan_response_data_length);

    SendAdvCmd(FROM_HERE, HCI_LE_SET_EXT_ADVERTISING_SCAN_RESP, param,
               cmd_length, command_complete);
  }

  void SetRandomAddress(uint8_t handle, const RawAddress& random_address,
                        status_cb command_complete) override {
    VLOG(1) << __func__;
    const int LE_SET_ADVERTISING_SET_RANDOM_ADDRESS_LEN = 7;

    uint8_t param[LE_SET_ADVERTISING_SET_RANDOM_ADDRESS_LEN];
    memset(param, 0, LE_SET_ADVERTISING_SET_RANDOM_ADDRESS_LEN);

    uint8_t* pp = param;
    UINT8_TO_STREAM(pp, handle);
    BDADDR_TO_STREAM(pp, random_address);

    SendAdvCmd(FROM_HERE, HCI_LE_SET_EXT_ADVERTISING_RANDOM_ADDRESS, param,
               LE_SET_ADVERTISING_SET_RANDOM_ADDRESS_LEN, command_complete);
  }

  void Enable(uint8_t enable, std::vector<SetEnableData> sets,
              status_cb command_complete) override {
    VLOG(1) << __func__;

    /* cmd_length = header_size + num_of_of_advertiser * size_per_advertiser */
    const uint16_t cmd_length = 2 + sets.size() * 4;
    uint8_t param[cmd_length];
    memset(param, 0, cmd_length);

    uint8_t* pp = param;
    UINT8_TO_STREAM(pp, enable);

    UINT8_TO_STREAM(pp, sets.size());
    for (const SetEnableData& set : sets) {
      UINT8_TO_STREAM(pp, set.handle);
      UINT16_TO_STREAM(pp, set.duration);
      UINT8_TO_STREAM(pp, set.max_extended_advertising_events);
    }

    SendAdvCmd(FROM_HERE, HCI_LE_SET_EXT_ADVERTISING_ENABLE, param, cmd_length,
               command_complete);
  }

  void SetPeriodicAdvertisingParameters(uint8_t handle,
                                        uint16_t periodic_adv_int_min,
                                        uint16_t periodic_adv_int_max,
                                        uint16_t periodic_properties,
                                        status_cb command_complete) override {
    VLOG(1) << __func__;
    const uint16_t HCI_LE_SET_PRIODIC_ADVERTISING_PARAM_LEN = 7;
    uint8_t param[HCI_LE_SET_PRIODIC_ADVERTISING_PARAM_LEN];
    memset(param, 0, HCI_LE_SET_PRIODIC_ADVERTISING_PARAM_LEN);

    uint8_t* pp = param;
    UINT8_TO_STREAM(pp, handle);
    UINT16_TO_STREAM(pp, periodic_adv_int_min);
    UINT16_TO_STREAM(pp, periodic_adv_int_max);
    UINT16_TO_STREAM(pp, periodic_properties);

    SendAdvCmd(FROM_HERE, HCI_LE_SET_PERIODIC_ADVERTISING_PARAM, param,
               HCI_LE_SET_PRIODIC_ADVERTISING_PARAM_LEN, command_complete);
  }

  void SetPeriodicAdvertisingData(uint8_t handle, uint8_t operation,
                                  uint8_t adv_data_length, uint8_t* adv_data,
                                  status_cb command_complete) override {
    VLOG(1) << __func__;
    const uint16_t HCI_LE_SET_PRIODIC_ADVERTISING_DATA_LEN =
        3 + adv_data_length;
    uint8_t param[HCI_LE_SET_PRIODIC_ADVERTISING_DATA_LEN];
    memset(param, 0, HCI_LE_SET_PRIODIC_ADVERTISING_DATA_LEN);
    uint8_t* pp = param;
    UINT8_TO_STREAM(pp, handle);
    UINT8_TO_STREAM(pp, operation);
    UINT8_TO_STREAM(pp, adv_data_length);
    ARRAY_TO_STREAM(pp, adv_data, adv_data_length);
    SendAdvCmd(FROM_HERE, HCI_LE_SET_PERIODIC_ADVERTISING_DATA, param,
               HCI_LE_SET_PRIODIC_ADVERTISING_DATA_LEN, command_complete);
  }

  void SetPeriodicAdvertisingEnable(uint8_t enable, uint8_t handle,
                                    status_cb command_complete) override {
    VLOG(1) << __func__;
    const uint16_t HCI_LE_ENABLE_PRIODIC_ADVERTISEMENT_LEN = 2;
    uint8_t param[HCI_LE_ENABLE_PRIODIC_ADVERTISEMENT_LEN];
    memset(param, 0, HCI_LE_ENABLE_PRIODIC_ADVERTISEMENT_LEN);
    uint8_t* pp = param;
    UINT8_TO_STREAM(pp, enable);
    UINT8_TO_STREAM(pp, handle);
    SendAdvCmd(FROM_HERE, HCI_LE_SET_PERIODIC_ADVERTISING_ENABLE, param,
               HCI_LE_ENABLE_PRIODIC_ADVERTISEMENT_LEN, command_complete);
  }

  void RemoveAdvertisingSet(uint8_t handle,
                            status_cb command_complete) override {
    VLOG(1) << __func__;

    const uint16_t cmd_length = 1;
    uint8_t param[cmd_length];
    memset(param, 0, cmd_length);

    uint8_t* pp = param;
    UINT8_TO_STREAM(pp, handle);

    SendAdvCmd(FROM_HERE, HCI_LE_REMOVE_ADVERTISING_SET, param, cmd_length,
               command_complete);
  }

 public:
  void OnAdvertisingSetTerminated(uint8_t length, uint8_t* p) {
    VLOG(1) << __func__;
    LOG_ASSERT(p);
    uint8_t status, advertising_handle, num_completed_extended_adv_events;
    uint16_t conn_handle;

    STREAM_TO_UINT8(status, p);
    STREAM_TO_UINT8(advertising_handle, p);
    STREAM_TO_UINT16(conn_handle, p);
    STREAM_TO_UINT8(num_completed_extended_adv_events, p);

    conn_handle = conn_handle & 0x0FFF;  // only 12 bits meaningful

    AdvertisingEventObserver* observer = this->advertising_event_observer;
    if (observer)
      observer->OnAdvertisingSetTerminated(status, advertising_handle,
                                           conn_handle,
                                           num_completed_extended_adv_events);
  }

 private:
  AdvertisingEventObserver* advertising_event_observer = nullptr;
};

}  // namespace

void btm_le_on_advertising_set_terminated(uint8_t* p, uint16_t length) {
  if (BleAdvertiserHciInterface::Get()) {
    ((BleAdvertiserHciExtendedImpl*)BleAdvertiserHciInterface::Get())
        ->OnAdvertisingSetTerminated(length, p);
  }
}

bool legacy_advertising_in_use = false;
void btm_ble_advertiser_notify_terminated_legacy(uint8_t status,
                                                 uint16_t connection_handle) {
  if (BleAdvertiserHciInterface::Get() && legacy_advertising_in_use) {
    ((BleAdvertiserLegacyHciInterfaceImpl*)BleAdvertiserHciInterface::Get())
        ->OnAdvertisingSetTerminated(status, connection_handle);
  }
}

void BleAdvertiserHciInterface::Initialize() {
  VLOG(1) << __func__;
  LOG_ASSERT(instance == nullptr) << "Was already initialized.";

  if (controller_get_interface()->supports_ble_extended_advertising()) {
    LOG(INFO) << "Extended advertising will be in use";
    instance = new BleAdvertiserHciExtendedImpl();
  } else if (BTM_BleMaxMultiAdvInstanceCount()) {
    LOG(INFO) << "VSC advertising will be in use";
    instance = new BleAdvertiserVscHciInterfaceImpl();
    BTM_RegisterForVSEvents(
        BleAdvertiserVscHciInterfaceImpl::VendorSpecificEventCback, true);
  } else {
    LOG(INFO) << "Legacy advertising will be in use";
    instance = new BleAdvertiserLegacyHciInterfaceImpl();
    legacy_advertising_in_use = true;
  }
}

BleAdvertiserHciInterface* BleAdvertiserHciInterface::Get() { return instance; }

void BleAdvertiserHciInterface::CleanUp() {
  VLOG(1) << __func__;

  if (BTM_BleMaxMultiAdvInstanceCount()) {
    BTM_RegisterForVSEvents(
        BleAdvertiserVscHciInterfaceImpl::VendorSpecificEventCback, false);
  }

  delete instance;
  instance = nullptr;
}
