/******************************************************************************
 *
 *  Copyright 2014 Broadcom Corporation
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
#include <base/bind.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include "bt_target.h"

#include "bt_types.h"
#include "bt_utils.h"
#include "btm_ble_api.h"
#include "btm_int.h"
#include "btu.h"
#include "device/include/controller.h"
#include "hcimsgs.h"

using base::Bind;
using base::Callback;
using hci_cmd_cb = base::Callback<void(uint8_t* /* return_parameters */,
                                       uint16_t /* return_parameters_length*/)>;

tBTM_BLE_BATCH_SCAN_CB ble_batchscan_cb;
tBTM_BLE_ADV_TRACK_CB ble_advtrack_cb;

/* length of each batch scan command */
#define BTM_BLE_BATCH_SCAN_STORAGE_CFG_LEN 4
#define BTM_BLE_BATCH_SCAN_PARAM_CONFIG_LEN 12
#define BTM_BLE_BATCH_SCAN_ENB_DISB_LEN 2
#define BTM_BLE_BATCH_SCAN_READ_RESULTS_LEN 2

namespace {

bool can_do_batch_scan() {
  if (!controller_get_interface()->supports_ble()) return false;

  tBTM_BLE_VSC_CB cmn_ble_vsc_cb;
  BTM_BleGetVendorCapabilities(&cmn_ble_vsc_cb);

  if (cmn_ble_vsc_cb.tot_scan_results_strg == 0) return false;

  return true;
}

/* VSE callback for batch scan, filter, and tracking events */
void btm_ble_batchscan_filter_track_adv_vse_cback(uint8_t len, uint8_t* p) {
  tBTM_BLE_TRACK_ADV_DATA adv_data;

  uint8_t sub_event = 0;
  tBTM_BLE_VSC_CB cmn_ble_vsc_cb;
  if (len == 0) return;
  STREAM_TO_UINT8(sub_event, p);

  BTM_TRACE_EVENT(
      "btm_ble_batchscan_filter_track_adv_vse_cback called with event:%x",
      sub_event);
  if (HCI_VSE_SUBCODE_BLE_THRESHOLD_SUB_EVT == sub_event &&
      NULL != ble_batchscan_cb.p_thres_cback) {
    ble_batchscan_cb.p_thres_cback(ble_batchscan_cb.ref_value);
    return;
  }

  if (HCI_VSE_SUBCODE_BLE_TRACKING_SUB_EVT == sub_event &&
      NULL != ble_advtrack_cb.p_track_cback) {
    if (len < 10) return;

    memset(&adv_data, 0, sizeof(tBTM_BLE_TRACK_ADV_DATA));
    BTM_BleGetVendorCapabilities(&cmn_ble_vsc_cb);
    adv_data.client_if = (uint8_t)ble_advtrack_cb.ref_value;
    if (cmn_ble_vsc_cb.version_supported > BTM_VSC_CHIP_CAPABILITY_L_VERSION) {
      STREAM_TO_UINT8(adv_data.filt_index, p);
      STREAM_TO_UINT8(adv_data.advertiser_state, p);
      STREAM_TO_UINT8(adv_data.advertiser_info_present, p);
      STREAM_TO_BDADDR(adv_data.bd_addr, p);
      STREAM_TO_UINT8(adv_data.addr_type, p);

      /* Extract the adv info details */
      if (ADV_INFO_PRESENT == adv_data.advertiser_info_present) {
        if (len < 15) return;
        STREAM_TO_UINT8(adv_data.tx_power, p);
        STREAM_TO_UINT8(adv_data.rssi_value, p);
        STREAM_TO_UINT16(adv_data.time_stamp, p);

        STREAM_TO_UINT8(adv_data.adv_pkt_len, p);
        if (adv_data.adv_pkt_len > 0) {
          adv_data.p_adv_pkt_data =
              static_cast<uint8_t*>(osi_malloc(adv_data.adv_pkt_len));
          memcpy(adv_data.p_adv_pkt_data, p, adv_data.adv_pkt_len);
        }

        STREAM_TO_UINT8(adv_data.scan_rsp_len, p);
        if (adv_data.scan_rsp_len > 0) {
          adv_data.p_scan_rsp_data =
              static_cast<uint8_t*>(osi_malloc(adv_data.scan_rsp_len));
          memcpy(adv_data.p_scan_rsp_data, p, adv_data.scan_rsp_len);
        }
      }
    } else {
      /* Based on L-release version */
      STREAM_TO_UINT8(adv_data.filt_index, p);
      STREAM_TO_UINT8(adv_data.addr_type, p);
      STREAM_TO_BDADDR(adv_data.bd_addr, p);
      STREAM_TO_UINT8(adv_data.advertiser_state, p);
    }

    BTM_TRACE_EVENT("track_adv_vse_cback called: %d, %d, %d",
                    adv_data.filt_index, adv_data.addr_type,
                    adv_data.advertiser_state);

    // Make sure the device is known
    BTM_SecAddBleDevice(adv_data.bd_addr, NULL, BT_DEVICE_TYPE_BLE,
                        adv_data.addr_type);

    ble_advtrack_cb.p_track_cback(&adv_data);
    return;
  }
}

void feat_enable_cb(uint8_t* p, uint16_t len) {
  if (len < 2) {
    BTM_TRACE_ERROR("%s: wrong length", __func__);
    return;
  }

  uint8_t status, subcode;
  STREAM_TO_UINT8(status, p);
  STREAM_TO_UINT8(subcode, p);

  uint8_t expected_opcode = BTM_BLE_BATCH_SCAN_ENB_DISAB_CUST_FEATURE;
  if (subcode != expected_opcode) {
    BTM_TRACE_ERROR("%s: bad subcode, expected: %d got: %d", __func__,
                    expected_opcode, subcode);
    return;
  }

  if (ble_batchscan_cb.cur_state != BTM_BLE_SCAN_ENABLE_CALLED)
    BTM_TRACE_ERROR("%s: state should be ENABLE_CALLED", __func__);

  ble_batchscan_cb.cur_state = BTM_BLE_SCAN_ENABLED_STATE;
}

void storage_config_cb(Callback<void(uint8_t /* status */)> cb, uint8_t* p,
                       uint16_t len) {
  if (len < 2) {
    BTM_TRACE_ERROR("%s: wrong length", __func__);
    return;
  }

  uint8_t status, subcode;
  STREAM_TO_UINT8(status, p);
  STREAM_TO_UINT8(subcode, p);

  uint8_t expected_opcode = BTM_BLE_BATCH_SCAN_SET_STORAGE_PARAM;
  if (subcode != expected_opcode) {
    BTM_TRACE_ERROR("%s: bad subcode, expected: %d got: %d", __func__,
                    expected_opcode, subcode);
    return;
  }

  cb.Run(status);
}

void param_enable_cb(Callback<void(uint8_t /* status */)> cb, uint8_t* p,
                     uint16_t len) {
  if (len < 2) {
    BTM_TRACE_ERROR("%s: wrong length", __func__);
    return;
  }

  uint8_t status, subcode;
  STREAM_TO_UINT8(status, p);
  STREAM_TO_UINT8(subcode, p);

  uint8_t expected_opcode = BTM_BLE_BATCH_SCAN_SET_PARAMS;
  if (subcode != expected_opcode) {
    BTM_TRACE_ERROR("%s: bad subcode: 0x%02x 0x%02x", __func__, expected_opcode,
                    subcode);
    return;
  }

  cb.Run(status);
}

void disable_cb(base::Callback<void(uint8_t /* status */)> cb, uint8_t* p,
                uint16_t len) {
  if (len < 2) {
    BTM_TRACE_ERROR("%s: wrong length", __func__);
    return;
  }

  uint8_t status, subcode;
  STREAM_TO_UINT8(status, p);
  STREAM_TO_UINT8(subcode, p);

  uint8_t expected_opcode = BTM_BLE_BATCH_SCAN_SET_PARAMS;
  if (subcode != expected_opcode) {
    BTM_TRACE_ERROR("%s: bad subcode: 0x%02x 0x%02x", __func__, expected_opcode,
                    subcode);
    return;
  }

  if (ble_batchscan_cb.cur_state != BTM_BLE_SCAN_DISABLE_CALLED) {
    BTM_TRACE_ERROR("%s: state should be DISABLE_CALLED", __func__);
  }

  if (BTM_SUCCESS == status) {
    ble_batchscan_cb.cur_state = BTM_BLE_SCAN_DISABLED_STATE;
  } else {
    BTM_TRACE_ERROR("%s: Invalid state after disabled", __func__);
    ble_batchscan_cb.cur_state = BTM_BLE_SCAN_INVALID_STATE;
  }

  cb.Run(status);
}

/**
 * This function reads the reports from controller. |scan_mode| is the mode for
 * which the reports are to be read
 */
void btm_ble_read_batchscan_reports(tBTM_BLE_BATCH_SCAN_MODE scan_mode,
                                    hci_cmd_cb cb) {
  uint8_t len = BTM_BLE_BATCH_SCAN_READ_RESULTS_LEN;
  uint8_t param[len];
  memset(param, 0, len);

  uint8_t* pp = param;
  UINT8_TO_STREAM(pp, BTM_BLE_BATCH_SCAN_READ_RESULTS);
  UINT8_TO_STREAM(pp, scan_mode);

  btu_hcif_send_cmd_with_cb(FROM_HERE, HCI_BLE_BATCH_SCAN_OCF, param, len, cb);
}

/* read reports. data is accumulated in |data_all|, number of records is
 * accumulated in |num_records_all| */
void read_reports_cb(std::vector<uint8_t> data_all, uint8_t num_records_all,
                     tBTM_BLE_SCAN_REP_CBACK cb, uint8_t* p, uint16_t len) {
  if (len < 2) {
    BTM_TRACE_ERROR("%s: wrong length", __func__);
    return;
  }

  uint8_t status, subcode;
  STREAM_TO_UINT8(status, p);
  STREAM_TO_UINT8(subcode, p);

  uint8_t expected_opcode = BTM_BLE_BATCH_SCAN_READ_RESULTS;
  if (subcode != expected_opcode) {
    BTM_TRACE_ERROR("%s: bad subcode, expected: %d got: %d", __func__,
                    expected_opcode, subcode);
    return;
  }

  uint8_t report_format, num_records;
  STREAM_TO_UINT8(report_format, p);
  STREAM_TO_UINT8(num_records, p);

  BTM_TRACE_DEBUG("%s: status=%d,len=%d,rec=%d", __func__, status, len - 4,
                  num_records);

  if (num_records == 0) {
    cb.Run(status, report_format, num_records_all, data_all);
    return;
  }

  if (len > 4) {
    data_all.insert(data_all.end(), p, p + len - 4);
    num_records_all += num_records;

    /* More records could be in the buffer and needs to be pulled out */
    btm_ble_read_batchscan_reports(
        report_format, base::Bind(&read_reports_cb, std::move(data_all),
                                  num_records_all, std::move(cb)));
  }
}

/**
 * This function writes the storage configuration in controller
 *
 * Parameters       batch_scan_full_max - Max storage space (in %) allocated to
 *                                        full scanning
 *                  batch_scan_trunc_max - Max storage space (in %) allocated to
 *                                         truncated scanning
 *                  batch_scan_notify_threshold - Set up notification level
 *                                                based on total space
 *
 **/
void btm_ble_set_storage_config(uint8_t batch_scan_full_max,
                                uint8_t batch_scan_trunc_max,
                                uint8_t batch_scan_notify_threshold,
                                hci_cmd_cb cb) {
  uint8_t len = BTM_BLE_BATCH_SCAN_STORAGE_CFG_LEN;
  uint8_t param[len];
  memset(param, 0, len);

  uint8_t* pp = param;
  UINT8_TO_STREAM(pp, BTM_BLE_BATCH_SCAN_SET_STORAGE_PARAM);
  UINT8_TO_STREAM(pp, batch_scan_full_max);
  UINT8_TO_STREAM(pp, batch_scan_trunc_max);
  UINT8_TO_STREAM(pp, batch_scan_notify_threshold);

  btu_hcif_send_cmd_with_cb(FROM_HERE, HCI_BLE_BATCH_SCAN_OCF, param, len, cb);
}

/* This function writes the batch scan params in controller */
void btm_ble_set_batchscan_param(tBTM_BLE_BATCH_SCAN_MODE scan_mode,
                                 uint32_t scan_interval, uint32_t scan_window,
                                 tBLE_ADDR_TYPE addr_type,
                                 tBTM_BLE_DISCARD_RULE discard_rule,
                                 hci_cmd_cb cb) {
  // Override param and decide addr_type based on own addr type
  // TODO: Remove upper layer parameter?
  addr_type = btm_cb.ble_ctr_cb.addr_mgnt_cb.own_addr_type;

  uint8_t len = BTM_BLE_BATCH_SCAN_PARAM_CONFIG_LEN;
  uint8_t param[len];
  memset(param, 0, len);

  uint8_t* p = param;
  UINT8_TO_STREAM(p, BTM_BLE_BATCH_SCAN_SET_PARAMS);
  UINT8_TO_STREAM(p, scan_mode);
  UINT32_TO_STREAM(p, scan_window);
  UINT32_TO_STREAM(p, scan_interval);
  UINT8_TO_STREAM(p, addr_type);
  UINT8_TO_STREAM(p, discard_rule);

  btu_hcif_send_cmd_with_cb(FROM_HERE, HCI_BLE_BATCH_SCAN_OCF, param, len, cb);
}

/* This function enables the customer specific feature in controller */
void btm_ble_enable_batchscan(hci_cmd_cb cb) {
  uint8_t len = BTM_BLE_BATCH_SCAN_ENB_DISB_LEN;
  uint8_t param[len];
  memset(param, 0, len);

  uint8_t* p = param;
  UINT8_TO_STREAM(p, BTM_BLE_BATCH_SCAN_ENB_DISAB_CUST_FEATURE);
  UINT8_TO_STREAM(p, 0x01 /* enable */);

  btu_hcif_send_cmd_with_cb(FROM_HERE, HCI_BLE_BATCH_SCAN_OCF, param, len, cb);
}

}  // namespace

/*******************************************************************************
 *
 * Description      This function is called to write storage config params.
 *
 * Parameters:      batch_scan_full_max - Max storage space (in %) allocated to
 *                                        full style
 *                  batch_scan_trunc_max - Max storage space (in %) allocated to
 *                                         trunc style
 *                  batch_scan_notify_threshold - Setup notification level based
 *                                                on total space
 *                  cb - Setup callback pointer
 *                  p_thres_cback - Threshold callback pointer
 *                  ref_value - Reference value
 *
 ******************************************************************************/
void BTM_BleSetStorageConfig(uint8_t batch_scan_full_max,
                             uint8_t batch_scan_trunc_max,
                             uint8_t batch_scan_notify_threshold,
                             Callback<void(uint8_t /* status */)> cb,
                             tBTM_BLE_SCAN_THRESHOLD_CBACK* p_thres_cback,
                             tBTM_BLE_REF_VALUE ref_value) {
  if (!can_do_batch_scan()) {
    cb.Run(BTM_ERR_PROCESSING);
    return;
  }

  BTM_TRACE_EVENT("%s: %d, %d, %d, %d, %d", __func__,
                  ble_batchscan_cb.cur_state, ref_value, batch_scan_full_max,
                  batch_scan_trunc_max, batch_scan_notify_threshold);

  ble_batchscan_cb.p_thres_cback = p_thres_cback;
  ble_batchscan_cb.ref_value = ref_value;

  if (batch_scan_full_max > BTM_BLE_ADV_SCAN_FULL_MAX ||
      batch_scan_trunc_max > BTM_BLE_ADV_SCAN_TRUNC_MAX ||
      batch_scan_notify_threshold > BTM_BLE_ADV_SCAN_THR_MAX) {
    BTM_TRACE_ERROR("Illegal set storage config params");
    cb.Run(BTM_ILLEGAL_VALUE);
    return;
  }

  if (BTM_BLE_SCAN_INVALID_STATE == ble_batchscan_cb.cur_state ||
      BTM_BLE_SCAN_DISABLED_STATE == ble_batchscan_cb.cur_state ||
      BTM_BLE_SCAN_DISABLE_CALLED == ble_batchscan_cb.cur_state) {
    btm_ble_enable_batchscan(Bind(&feat_enable_cb));
    ble_batchscan_cb.cur_state = BTM_BLE_SCAN_ENABLE_CALLED;
  }

  btm_ble_set_storage_config(batch_scan_full_max, batch_scan_trunc_max,
                             batch_scan_notify_threshold,
                             Bind(&storage_config_cb, cb));
  return;
}

/* This function is called to configure and enable batch scanning */
void BTM_BleEnableBatchScan(tBTM_BLE_BATCH_SCAN_MODE scan_mode,
                            uint32_t scan_interval, uint32_t scan_window,
                            tBLE_ADDR_TYPE addr_type,
                            tBTM_BLE_DISCARD_RULE discard_rule,
                            Callback<void(uint8_t /* status */)> cb) {
  BTM_TRACE_EVENT("%s: %d, %d, %d, %d, %d, %d", __func__, scan_mode,
                  scan_interval, scan_window, addr_type, discard_rule);

  if (!can_do_batch_scan()) {
    cb.Run(BTM_ERR_PROCESSING);
    return;
  }

  BTM_TRACE_DEBUG("%s: %d, %x, %x, %d, %d", __func__, scan_mode, scan_interval,
                  scan_window, discard_rule, ble_batchscan_cb.cur_state);

  /* Only 16 bits will be used for scan interval and scan window as per
   * agreement with Google */
  /* So the standard LE range would suffice for scan interval and scan window */
  if ((BTM_BLE_ISVALID_PARAM(scan_interval, BTM_BLE_SCAN_INT_MIN,
                             BTM_BLE_SCAN_INT_MAX) ||
       BTM_BLE_ISVALID_PARAM(scan_window, BTM_BLE_SCAN_WIN_MIN,
                             BTM_BLE_SCAN_WIN_MAX)) &&
      (BTM_BLE_BATCH_SCAN_MODE_PASS == scan_mode ||
       BTM_BLE_BATCH_SCAN_MODE_ACTI == scan_mode ||
       BTM_BLE_BATCH_SCAN_MODE_PASS_ACTI == scan_mode) &&
      (BTM_BLE_DISCARD_OLD_ITEMS == discard_rule ||
       BTM_BLE_DISCARD_LOWER_RSSI_ITEMS == discard_rule)) {
  } else {
    BTM_TRACE_ERROR("%s: Illegal enable scan params", __func__);
    cb.Run(BTM_ILLEGAL_VALUE);
    return;
  }

  if (BTM_BLE_SCAN_INVALID_STATE == ble_batchscan_cb.cur_state ||
      BTM_BLE_SCAN_DISABLED_STATE == ble_batchscan_cb.cur_state ||
      BTM_BLE_SCAN_DISABLE_CALLED == ble_batchscan_cb.cur_state) {
    btm_ble_enable_batchscan(Bind(&feat_enable_cb));
    ble_batchscan_cb.cur_state = BTM_BLE_SCAN_ENABLE_CALLED;
  }

  ble_batchscan_cb.scan_mode = scan_mode;
  ble_batchscan_cb.scan_interval = scan_interval;
  ble_batchscan_cb.scan_window = scan_window;
  ble_batchscan_cb.addr_type = addr_type;
  ble_batchscan_cb.discard_rule = discard_rule;
  /* This command starts batch scanning, if enabled */
  btm_ble_set_batchscan_param(scan_mode, scan_interval, scan_window, addr_type,
                              discard_rule, Bind(&param_enable_cb, cb));
}

/* This function is called to disable batch scanning */
void BTM_BleDisableBatchScan(base::Callback<void(uint8_t /* status */)> cb) {
  BTM_TRACE_EVENT(" BTM_BleDisableBatchScan");

  if (!can_do_batch_scan()) {
    cb.Run(BTM_ERR_PROCESSING);
    return;
  }

  btm_ble_set_batchscan_param(
      BTM_BLE_BATCH_SCAN_MODE_DISABLE, ble_batchscan_cb.scan_interval,
      ble_batchscan_cb.scan_window, ble_batchscan_cb.addr_type,
      ble_batchscan_cb.discard_rule, Bind(&disable_cb, cb));
  ble_batchscan_cb.cur_state = BTM_BLE_SCAN_DISABLE_CALLED;
}

/* This function is called to start reading batch scan reports */
void BTM_BleReadScanReports(tBTM_BLE_BATCH_SCAN_MODE scan_mode,
                            tBTM_BLE_SCAN_REP_CBACK cb) {
  uint8_t read_scan_mode = 0;

  BTM_TRACE_EVENT("%s; %d", __func__, scan_mode);

  if (!can_do_batch_scan()) {
    BTM_TRACE_ERROR("Controller does not support batch scan");
    cb.Run(BTM_ERR_PROCESSING, 0, 0, {});
    return;
  }

  /*  Check if the requested scan mode has already been setup by the user */
  read_scan_mode = ble_batchscan_cb.scan_mode & BTM_BLE_BATCH_SCAN_MODE_ACTI;
  if (0 == read_scan_mode)
    read_scan_mode = ble_batchscan_cb.scan_mode & BTM_BLE_BATCH_SCAN_MODE_PASS;

  /* Check only for modes, as scan reports can be called after disabling batch
   * scan */
  if (scan_mode != BTM_BLE_BATCH_SCAN_MODE_PASS &&
      scan_mode != BTM_BLE_BATCH_SCAN_MODE_ACTI) {
    BTM_TRACE_ERROR("Illegal read scan params: %d, %d, %d", read_scan_mode,
                    scan_mode, ble_batchscan_cb.cur_state);
    cb.Run(BTM_ILLEGAL_VALUE, 0, 0, {});
    return;
  }

  btm_ble_read_batchscan_reports(
      scan_mode, base::Bind(&read_reports_cb, std::vector<uint8_t>(), 0, cb));
  return;
}

/* This function is called to setup the callback for tracking */
void BTM_BleTrackAdvertiser(tBTM_BLE_TRACK_ADV_CBACK* p_track_cback,
                            tBTM_BLE_REF_VALUE ref_value) {
  BTM_TRACE_EVENT("%s:", __func__);

  if (!can_do_batch_scan()) {
    BTM_TRACE_ERROR("Controller does not support batch scan");

    tBTM_BLE_TRACK_ADV_DATA track_adv_data;
    memset(&track_adv_data, 0, sizeof(tBTM_BLE_TRACK_ADV_DATA));
    track_adv_data.advertiser_info_present =
        NO_ADV_INFO_PRESENT; /* Indicates failure */
    track_adv_data.client_if = (uint8_t)ref_value;
    p_track_cback(&track_adv_data);
    return;
  }

  ble_advtrack_cb.p_track_cback = p_track_cback;
  ble_advtrack_cb.ref_value = ref_value;
  return;
}

/**
 * This function initialize the batch scan control block.
 **/
void btm_ble_batchscan_init(void) {
  BTM_TRACE_EVENT(" btm_ble_batchscan_init");
  memset(&ble_batchscan_cb, 0, sizeof(tBTM_BLE_BATCH_SCAN_CB));
  memset(&ble_advtrack_cb, 0, sizeof(tBTM_BLE_ADV_TRACK_CB));
  BTM_RegisterForVSEvents(btm_ble_batchscan_filter_track_adv_vse_cback, true);
}

/**
 * This function cleans the batch scan control block.
 **/
void btm_ble_batchscan_cleanup(void) {
  BTM_TRACE_EVENT("%s", __func__);

  memset(&ble_batchscan_cb, 0, sizeof(tBTM_BLE_BATCH_SCAN_CB));
  memset(&ble_advtrack_cb, 0, sizeof(tBTM_BLE_ADV_TRACK_CB));
}
