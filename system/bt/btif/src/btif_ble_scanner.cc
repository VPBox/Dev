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

#define LOG_TAG "bt_btif_scanner"

#include <base/bind.h>
#include <base/threading/thread.h>
#include <errno.h>
#include <hardware/bluetooth.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unordered_set>
#include "device/include/controller.h"

#include "btif_common.h"
#include "btif_util.h"

#include <hardware/bt_gatt.h>

#include "advertise_data_parser.h"
#include "bta_api.h"
#include "bta_gatt_api.h"
#include "btif_config.h"
#include "btif_dm.h"
#include "btif_gatt.h"
#include "btif_gatt_util.h"
#include "btif_storage.h"
#include "osi/include/log.h"
#include "stack/include/btu.h"
#include "vendor_api.h"

using base::Bind;
using base::Owned;
using std::vector;
using RegisterCallback = BleScannerInterface::RegisterCallback;

extern const btgatt_callbacks_t* bt_gatt_callbacks;

#define SCAN_CBACK_IN_JNI(P_CBACK, ...)                              \
  do {                                                               \
    if (bt_gatt_callbacks && bt_gatt_callbacks->scanner->P_CBACK) {  \
      BTIF_TRACE_API("HAL bt_gatt_callbacks->client->%s", #P_CBACK); \
      do_in_jni_thread(                                              \
          Bind(bt_gatt_callbacks->scanner->P_CBACK, __VA_ARGS__));   \
    } else {                                                         \
      ASSERTC(0, "Callback is NULL", 0);                             \
    }                                                                \
  } while (0)

namespace {

// all access to this variable should be done on the jni thread
std::set<RawAddress> remote_bdaddr_cache;
std::queue<RawAddress> remote_bdaddr_cache_ordered;
const size_t remote_bdaddr_cache_max_size = 1024;

void btif_address_cache_add(const RawAddress& p_bda, uint8_t addr_type) {
  // Remove the oldest entries
  while (remote_bdaddr_cache.size() >= remote_bdaddr_cache_max_size) {
    const RawAddress& raw_address = remote_bdaddr_cache_ordered.front();
    remote_bdaddr_cache.erase(raw_address);
    remote_bdaddr_cache_ordered.pop();
  }
  remote_bdaddr_cache.insert(p_bda);
  remote_bdaddr_cache_ordered.push(p_bda);
}

bool btif_address_cache_find(const RawAddress& p_bda) {
  return (remote_bdaddr_cache.find(p_bda) != remote_bdaddr_cache.end());
}

void btif_address_cache_init(void) {
  remote_bdaddr_cache.clear();
  remote_bdaddr_cache_ordered = {};
}

void bta_batch_scan_threshold_cb(tBTM_BLE_REF_VALUE ref_value) {
  SCAN_CBACK_IN_JNI(batchscan_threshold_cb, ref_value);
}

void bta_batch_scan_reports_cb(int client_id, tBTA_STATUS status,
                               uint8_t report_format, uint8_t num_records,
                               std::vector<uint8_t> data) {
  SCAN_CBACK_IN_JNI(batchscan_reports_cb, client_id, status, report_format,
                    num_records, std::move(data));
}

void bta_scan_results_cb_impl(RawAddress bd_addr, tBT_DEVICE_TYPE device_type,
                              int8_t rssi, uint8_t addr_type,
                              uint16_t ble_evt_type, uint8_t ble_primary_phy,
                              uint8_t ble_secondary_phy,
                              uint8_t ble_advertising_sid, int8_t ble_tx_power,
                              uint16_t ble_periodic_adv_int,
                              vector<uint8_t> value) {
  uint8_t remote_name_len;
  bt_device_type_t dev_type;
  bt_property_t properties;

  const uint8_t* p_eir_remote_name = AdvertiseDataParser::GetFieldByType(
      value, BTM_EIR_COMPLETE_LOCAL_NAME_TYPE, &remote_name_len);

  if (p_eir_remote_name == NULL) {
    p_eir_remote_name = AdvertiseDataParser::GetFieldByType(
        value, BT_EIR_SHORTENED_LOCAL_NAME_TYPE, &remote_name_len);
  }

  if ((addr_type != BLE_ADDR_RANDOM) || (p_eir_remote_name)) {
    if (!btif_address_cache_find(bd_addr)) {
      btif_address_cache_add(bd_addr, addr_type);

      if (p_eir_remote_name) {
        if (remote_name_len > BD_NAME_LEN + 1 ||
            (remote_name_len == BD_NAME_LEN + 1 &&
             p_eir_remote_name[BD_NAME_LEN] != '\0')) {
          LOG_INFO(LOG_TAG,
                   "%s dropping invalid packet - device name too long: %d",
                   __func__, remote_name_len);
          return;
        }

        bt_bdname_t bdname;
        memcpy(bdname.name, p_eir_remote_name, remote_name_len);
        if (remote_name_len < BD_NAME_LEN + 1)
          bdname.name[remote_name_len] = '\0';

        LOG_VERBOSE(LOG_TAG, "%s BLE device name=%s len=%d dev_type=%d",
                    __func__, bdname.name, remote_name_len, device_type);
        btif_dm_update_ble_remote_properties(bd_addr, bdname.name, device_type);
      }
    }
  }

  dev_type = (bt_device_type_t)device_type;
  BTIF_STORAGE_FILL_PROPERTY(&properties, BT_PROPERTY_TYPE_OF_DEVICE,
                             sizeof(dev_type), &dev_type);
  btif_storage_set_remote_device_property(&(bd_addr), &properties);

  btif_storage_set_remote_addr_type(&bd_addr, addr_type);
  HAL_CBACK(bt_gatt_callbacks, scanner->scan_result_cb, ble_evt_type, addr_type,
            &bd_addr, ble_primary_phy, ble_secondary_phy, ble_advertising_sid,
            ble_tx_power, rssi, ble_periodic_adv_int, std::move(value));
}

void bta_scan_results_cb(tBTA_DM_SEARCH_EVT event, tBTA_DM_SEARCH* p_data) {
  uint8_t len;

  if (event == BTA_DM_INQ_CMPL_EVT) {
    BTIF_TRACE_DEBUG("%s  BLE observe complete. Num Resp %d", __func__,
                     p_data->inq_cmpl.num_resps);
    return;
  }

  if (event != BTA_DM_INQ_RES_EVT) {
    BTIF_TRACE_WARNING("%s : Unknown event 0x%x", __func__, event);
    return;
  }

  vector<uint8_t> value;
  if (p_data->inq_res.p_eir) {
    value.insert(value.begin(), p_data->inq_res.p_eir,
                 p_data->inq_res.p_eir + p_data->inq_res.eir_len);

    if (AdvertiseDataParser::GetFieldByType(
            value, BTM_EIR_COMPLETE_LOCAL_NAME_TYPE, &len)) {
      p_data->inq_res.remt_name_not_required = true;
    }
  }

  tBTA_DM_INQ_RES* r = &p_data->inq_res;
  do_in_jni_thread(Bind(bta_scan_results_cb_impl, r->bd_addr, r->device_type,
                        r->rssi, r->ble_addr_type, r->ble_evt_type,
                        r->ble_primary_phy, r->ble_secondary_phy,
                        r->ble_advertising_sid, r->ble_tx_power,
                        r->ble_periodic_adv_int, std::move(value)));
}

void bta_track_adv_event_cb(tBTM_BLE_TRACK_ADV_DATA* p_track_adv_data) {
  btgatt_track_adv_info_t* btif_scan_track_cb = new btgatt_track_adv_info_t;

  BTIF_TRACE_DEBUG("%s", __func__);
  btif_gatt_move_track_adv_data(btif_scan_track_cb,
                                (btgatt_track_adv_info_t*)p_track_adv_data);

  SCAN_CBACK_IN_JNI(track_adv_event_cb, Owned(btif_scan_track_cb));
}

void bta_cback(tBTA_GATTC_EVT, tBTA_GATTC*) {}

class BleScannerInterfaceImpl : public BleScannerInterface {
  ~BleScannerInterfaceImpl(){};

  void RegisterScanner(RegisterCallback cb) override {
    do_in_main_thread(FROM_HERE,
                      Bind(
                          [](RegisterCallback cb) {
                            BTA_GATTC_AppRegister(
                                bta_cback,
                                jni_thread_wrapper(FROM_HERE, std::move(cb)));
                          },
                          std::move(cb)));
  }

  void Unregister(int scanner_id) override {
    do_in_main_thread(FROM_HERE, Bind(&BTA_GATTC_AppDeregister, scanner_id));
  }

  void Scan(bool start) override {
    do_in_jni_thread(Bind(
        [](bool start) {
          if (!start) {
            do_in_main_thread(FROM_HERE,
                              Bind(&BTA_DmBleObserve, false, 0, nullptr));
            return;
          }

          btif_address_cache_init();
          do_in_main_thread(
              FROM_HERE, Bind(&BTA_DmBleObserve, true, 0, bta_scan_results_cb));
        },
        start));
  }

  void ScanFilterParamSetup(
      uint8_t client_if, uint8_t action, uint8_t filt_index,
      std::unique_ptr<btgatt_filt_param_setup_t> filt_param,
      FilterParamSetupCallback cb) override {
    BTIF_TRACE_DEBUG("%s", __func__);

    if (filt_param && filt_param->dely_mode == 1) {
      do_in_main_thread(
          FROM_HERE, base::Bind(BTM_BleTrackAdvertiser, bta_track_adv_event_cb,
                                client_if));
    }

    do_in_main_thread(
        FROM_HERE, base::Bind(&BTM_BleAdvFilterParamSetup, action, filt_index,
                              base::Passed(&filt_param),
                              jni_thread_wrapper(FROM_HERE, std::move(cb))));
  }

  void ScanFilterAdd(int filter_index, std::vector<ApcfCommand> filters,
                     FilterConfigCallback cb) override {
    BTIF_TRACE_DEBUG("%s: %d", __func__, filter_index);

    do_in_main_thread(
        FROM_HERE,
        base::Bind(
            &BTM_LE_PF_set, filter_index, std::move(filters),
            jni_thread_wrapper(
                FROM_HERE,
                Bind(std::move(cb),
                     0 /*TODO: this used to be filter type, unused ?*/))));
  }

  void ScanFilterClear(int filter_index, FilterConfigCallback cb) override {
    BTIF_TRACE_DEBUG("%s: filter_index: %d", __func__, filter_index);
    do_in_main_thread(
        FROM_HERE, base::Bind(&BTM_LE_PF_clear, filter_index,
                              jni_thread_wrapper(
                                  FROM_HERE, Bind(cb, BTM_BLE_PF_TYPE_ALL))));
  }

  void ScanFilterEnable(bool enable, EnableCallback cb) override {
    BTIF_TRACE_DEBUG("%s: enable: %d", __func__, enable);

    uint8_t action = enable ? 1 : 0;
    do_in_main_thread(FROM_HERE,
                      base::Bind(&BTM_BleEnableDisableFilterFeature, action,
                                 jni_thread_wrapper(FROM_HERE, std::move(cb))));
  }

  void SetScanParameters(int scan_interval, int scan_window,
                         Callback cb) override {
    do_in_main_thread(
        FROM_HERE, base::Bind(&BTM_BleSetScanParams, scan_interval, scan_window,
                              BTM_BLE_SCAN_MODE_ACTI,
                              jni_thread_wrapper(FROM_HERE, std::move(cb))));
  }

  void BatchscanConfigStorage(int client_if, int batch_scan_full_max,
                              int batch_scan_trunc_max,
                              int batch_scan_notify_threshold,
                              Callback cb) override {
    do_in_main_thread(
        FROM_HERE,
        base::Bind(&BTM_BleSetStorageConfig, (uint8_t)batch_scan_full_max,
                   (uint8_t)batch_scan_trunc_max,
                   (uint8_t)batch_scan_notify_threshold,
                   jni_thread_wrapper(FROM_HERE, cb),
                   bta_batch_scan_threshold_cb, (tBTM_BLE_REF_VALUE)client_if));
  }

  void BatchscanEnable(int scan_mode, int scan_interval, int scan_window,
                       int addr_type, int discard_rule, Callback cb) override {
    do_in_main_thread(
        FROM_HERE, base::Bind(&BTM_BleEnableBatchScan, scan_mode, scan_interval,
                              scan_window, discard_rule, addr_type,
                              jni_thread_wrapper(FROM_HERE, cb)));
  }

  void BatchscanDisable(Callback cb) override {
    do_in_main_thread(FROM_HERE, base::Bind(&BTM_BleDisableBatchScan,
                                            jni_thread_wrapper(FROM_HERE, cb)));
  }

  void BatchscanReadReports(int client_if, int scan_mode) override {
    do_in_main_thread(FROM_HERE,
                      base::Bind(&BTM_BleReadScanReports, (uint8_t)scan_mode,
                                 Bind(bta_batch_scan_reports_cb, client_if)));
  }

  void StartSync(uint8_t sid, RawAddress address, uint16_t skip,
                 uint16_t timeout, StartSyncCb start_cb, SyncReportCb report_cb,
                 SyncLostCb lost_cb) override {}

  void StopSync(uint16_t handle) override {}
};

BleScannerInterface* btLeScannerInstance = nullptr;

}  // namespace

BleScannerInterface* get_ble_scanner_instance() {
  if (btLeScannerInstance == nullptr)
    btLeScannerInstance = new BleScannerInterfaceImpl();

  return btLeScannerInstance;
}
