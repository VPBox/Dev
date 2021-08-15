/******************************************************************************
 *
 *  Copyright 2009-2012 Broadcom Corporation
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

/*******************************************************************************
 *
 *  Filename:      btif_dm.c
 *
 *  Description:   Contains Device Management (DM) related functionality
 *
 *
 ******************************************************************************/

#define LOG_TAG "bt_btif_dm"

#include "btif_dm.h"

#include <base/bind.h>
#include <base/logging.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include <mutex>

#include <bluetooth/uuid.h>
#include <hardware/bluetooth.h>
#include <hardware/bt_hearing_aid.h>

#include "advertise_data_parser.h"
#include "bt_common.h"
#include "bta_gatt_api.h"
#include "btif_api.h"
#include "btif_av.h"
#include "btif_bqr.h"
#include "btif_config.h"
#include "btif_dm.h"
#include "btif_hd.h"
#include "btif_hf.h"
#include "btif_hh.h"
#include "btif_sdp.h"
#include "btif_storage.h"
#include "btif_util.h"
#include "btu.h"
#include "common/metrics.h"
#include "device/include/controller.h"
#include "device/include/interop.h"
#include "internal_include/stack_config.h"
#include "osi/include/allocator.h"
#include "osi/include/log.h"
#include "osi/include/osi.h"
#include "osi/include/properties.h"
#include "stack/btm/btm_int.h"
#include "stack_config.h"

using bluetooth::Uuid;
/******************************************************************************
 *  Constants & Macros
 *****************************************************************************/

const Uuid UUID_HEARING_AID = Uuid::FromString("FDF0");

#define COD_MASK 0x07FF

#define COD_UNCLASSIFIED ((0x1F) << 8)
#define COD_HID_KEYBOARD 0x0540
#define COD_HID_POINTING 0x0580
#define COD_HID_COMBO 0x05C0
#define COD_HID_MAJOR 0x0500
#define COD_HID_MASK 0x0700
#define COD_AV_HEADSETS 0x0404
#define COD_AV_HANDSFREE 0x0408
#define COD_AV_HEADPHONES 0x0418
#define COD_AV_PORTABLE_AUDIO 0x041C
#define COD_AV_HIFI_AUDIO 0x0428

#define BTIF_DM_DEFAULT_INQ_MAX_RESULTS 0
#define BTIF_DM_DEFAULT_INQ_MAX_DURATION 10
#define BTIF_DM_MAX_SDP_ATTEMPTS_AFTER_PAIRING 2

#define NUM_TIMEOUT_RETRIES 5
#ifndef PROPERTY_PRODUCT_MODEL
#define PROPERTY_PRODUCT_MODEL "ro.product.model"
#endif
#define DEFAULT_LOCAL_NAME_MAX 31
#if (DEFAULT_LOCAL_NAME_MAX > BTM_MAX_LOC_BD_NAME_LEN)
#error "default btif local name size exceeds stack supported length"
#endif

#define ENCRYPTED_BREDR 2
#define ENCRYPTED_LE 4

typedef struct {
  bt_bond_state_t state;
  RawAddress static_bdaddr;
  RawAddress bd_addr;
  tBTM_BOND_TYPE bond_type;
  uint8_t pin_code_len;
  uint8_t is_ssp;
  uint8_t auth_req;
  uint8_t io_cap;
  uint8_t autopair_attempts;
  uint8_t timeout_retries;
  uint8_t is_local_initiated;
  uint8_t sdp_attempts;
  bool is_le_only;
  bool is_le_nc; /* LE Numeric comparison */
  btif_dm_ble_cb_t ble;
} btif_dm_pairing_cb_t;

// TODO(jpawlowski): unify ?
// btif_dm_local_key_id_t == tBTM_BLE_LOCAL_ID_KEYS == tBTA_BLE_LOCAL_ID_KEYS
typedef struct {
  Octet16 ir;
  Octet16 irk;
  Octet16 dhk;
} btif_dm_local_key_id_t;

typedef struct {
  bool is_er_rcvd;
  Octet16 er;
  bool is_id_keys_rcvd;
  btif_dm_local_key_id_t id_keys; /* ID kyes */

} btif_dm_local_key_cb_t;

typedef struct {
  RawAddress bd_addr;
  BD_NAME bd_name;
} btif_dm_remote_name_t;

/* this structure holds optional OOB data for remote device */
typedef struct {
  RawAddress bdaddr; /* peer bdaddr */
  bt_out_of_band_data_t oob_data;
} btif_dm_oob_cb_t;

typedef struct {
  RawAddress bdaddr;
  uint8_t transport; /* 0=Unknown, 1=BR/EDR, 2=LE */
} btif_dm_create_bond_cb_t;

typedef struct {
  uint8_t status;
  uint8_t ctrl_state;
  uint64_t tx_time;
  uint64_t rx_time;
  uint64_t idle_time;
  uint64_t energy_used;
} btif_activity_energy_info_cb_t;

typedef struct { unsigned int manufact_id; } skip_sdp_entry_t;

typedef enum {
  BTIF_DM_FUNC_CREATE_BOND,
  BTIF_DM_FUNC_CANCEL_BOND,
  BTIF_DM_FUNC_REMOVE_BOND,
  BTIF_DM_FUNC_BOND_STATE_CHANGED,
} bt_bond_function_t;

typedef struct {
  RawAddress bd_addr;
  bt_bond_function_t function;
  bt_bond_state_t state;
  struct timespec timestamp;
} btif_bond_event_t;

#define BTA_SERVICE_ID_TO_SERVICE_MASK(id) (1 << (id))

#define MAX_BTIF_BOND_EVENT_ENTRIES 15

static skip_sdp_entry_t sdp_blacklist[] = {{76}};  // Apple Mouse and Keyboard

/* This flag will be true if HCI_Inquiry is in progress */
static bool btif_dm_inquiry_in_progress = false;

/*******************************************************************************
 *  Static variables
 ******************************************************************************/
static char btif_default_local_name[DEFAULT_LOCAL_NAME_MAX + 1] = {'\0'};
static uid_set_t* uid_set = NULL;

/* A circular array to keep track of the most recent bond events */
static btif_bond_event_t btif_dm_bond_events[MAX_BTIF_BOND_EVENT_ENTRIES + 1];

static std::mutex bond_event_lock;

/* |btif_num_bond_events| keeps track of the total number of events and can be
   greater than |MAX_BTIF_BOND_EVENT_ENTRIES| */
static size_t btif_num_bond_events = 0;
static size_t btif_events_start_index = 0;
static size_t btif_events_end_index = 0;

/******************************************************************************
 *  Static functions
 *****************************************************************************/
static btif_dm_pairing_cb_t pairing_cb;
static btif_dm_oob_cb_t oob_cb;
static void btif_dm_generic_evt(uint16_t event, char* p_param);
static void btif_dm_cb_create_bond(const RawAddress& bd_addr,
                                   tBTA_TRANSPORT transport);
static void btif_dm_cb_hid_remote_name(tBTM_REMOTE_DEV_NAME* p_remote_name);
static void btif_update_remote_properties(const RawAddress& bd_addr,
                                          BD_NAME bd_name, DEV_CLASS dev_class,
                                          tBT_DEVICE_TYPE dev_type);
static btif_dm_local_key_cb_t ble_local_key_cb;
static void btif_dm_ble_key_notif_evt(tBTA_DM_SP_KEY_NOTIF* p_ssp_key_notif);
static void btif_dm_ble_auth_cmpl_evt(tBTA_DM_AUTH_CMPL* p_auth_cmpl);
static void btif_dm_ble_passkey_req_evt(tBTA_DM_PIN_REQ* p_pin_req);
static void btif_dm_ble_key_nc_req_evt(tBTA_DM_SP_KEY_NOTIF* p_notif_req);
static void btif_dm_ble_oob_req_evt(tBTA_DM_SP_RMT_OOB* req_oob_type);
static void btif_dm_ble_sc_oob_req_evt(tBTA_DM_SP_RMT_OOB* req_oob_type);

static void bte_scan_filt_param_cfg_evt(uint8_t action_type, uint8_t avbl_space,
                                        uint8_t ref_value, uint8_t status);

static char* btif_get_default_local_name();

static void btif_stats_add_bond_event(const RawAddress& bd_addr,
                                      bt_bond_function_t function,
                                      bt_bond_state_t state);

/******************************************************************************
 *  Externs
 *****************************************************************************/
extern bt_status_t btif_av_execute_service(bool b_enable);
extern bt_status_t btif_av_sink_execute_service(bool b_enable);
extern bt_status_t btif_hh_execute_service(bool b_enable);
extern bt_status_t btif_hf_client_execute_service(bool b_enable);
extern bt_status_t btif_sdp_execute_service(bool b_enable);
extern int btif_hh_connect(const RawAddress* bd_addr);
extern bt_status_t btif_hd_execute_service(bool b_enable);
extern bluetooth::hearing_aid::HearingAidInterface*
btif_hearing_aid_get_interface();

/******************************************************************************
 *  Functions
 *****************************************************************************/

static bool is_empty_128bit(uint8_t* data) {
  static const uint8_t zero[16] = {0};
  return !memcmp(zero, data, sizeof(zero));
}

static bool is_bonding_or_sdp() {
  return pairing_cb.state == BT_BOND_STATE_BONDING ||
         (pairing_cb.state == BT_BOND_STATE_BONDED && pairing_cb.sdp_attempts);
}

static void btif_dm_data_copy(uint16_t event, char* dst, char* src) {
  tBTA_DM_SEC* dst_dm_sec = (tBTA_DM_SEC*)dst;
  tBTA_DM_SEC* src_dm_sec = (tBTA_DM_SEC*)src;

  if (!src_dm_sec) return;

  CHECK(dst_dm_sec);
  maybe_non_aligned_memcpy(dst_dm_sec, src_dm_sec, sizeof(*src_dm_sec));

  if (event == BTA_DM_BLE_KEY_EVT) {
    dst_dm_sec->ble_key.p_key_value =
        (tBTM_LE_KEY_VALUE*)osi_malloc(sizeof(tBTM_LE_KEY_VALUE));
    CHECK(src_dm_sec->ble_key.p_key_value);
    memcpy(dst_dm_sec->ble_key.p_key_value, src_dm_sec->ble_key.p_key_value,
           sizeof(tBTM_LE_KEY_VALUE));
  }
}

static void btif_dm_data_free(uint16_t event, tBTA_DM_SEC* dm_sec) {
  if (event == BTA_DM_BLE_KEY_EVT)
    osi_free_and_reset((void**)&dm_sec->ble_key.p_key_value);
}

void btif_dm_init(uid_set_t* set) { uid_set = set; }

void btif_dm_cleanup(void) {
  if (uid_set) {
    uid_set_destroy(uid_set);
    uid_set = NULL;
  }
}

bt_status_t btif_in_execute_service_request(tBTA_SERVICE_ID service_id,
                                            bool b_enable) {
  BTIF_TRACE_DEBUG("%s service_id: %d", __func__, service_id);
  /* Check the service_ID and invoke the profile's BT state changed API */
  switch (service_id) {
    case BTA_HFP_SERVICE_ID:
    case BTA_HSP_SERVICE_ID: {
      bluetooth::headset::ExecuteService(b_enable);
    } break;
    case BTA_A2DP_SOURCE_SERVICE_ID: {
      btif_av_source_execute_service(b_enable);
    } break;
    case BTA_A2DP_SINK_SERVICE_ID: {
      btif_av_sink_execute_service(b_enable);
    } break;
    case BTA_HID_SERVICE_ID: {
      btif_hh_execute_service(b_enable);
    } break;
    case BTA_HFP_HS_SERVICE_ID: {
      btif_hf_client_execute_service(b_enable);
    } break;
    case BTA_SDP_SERVICE_ID: {
      btif_sdp_execute_service(b_enable);
    } break;
    case BTA_HIDD_SERVICE_ID: {
      btif_hd_execute_service(b_enable);
    } break;
    default:
      BTIF_TRACE_ERROR("%s: Unknown service %d being %s", __func__, service_id,
                       (b_enable) ? "enabled" : "disabled");
      return BT_STATUS_FAIL;
  }
  return BT_STATUS_SUCCESS;
}

/*******************************************************************************
 *
 * Function         check_eir_remote_name
 *
 * Description      Check if remote name is in the EIR data
 *
 * Returns          true if remote name found
 *                  Populate p_remote_name, if provided and remote name found
 *
 ******************************************************************************/
static bool check_eir_remote_name(tBTA_DM_SEARCH* p_search_data,
                                  uint8_t* p_remote_name,
                                  uint8_t* p_remote_name_len) {
  const uint8_t* p_eir_remote_name = NULL;
  uint8_t remote_name_len = 0;

  /* Check EIR for remote name and services */
  if (p_search_data->inq_res.p_eir) {
    p_eir_remote_name = AdvertiseDataParser::GetFieldByType(
        p_search_data->inq_res.p_eir, p_search_data->inq_res.eir_len,
        BTM_EIR_COMPLETE_LOCAL_NAME_TYPE, &remote_name_len);
    if (!p_eir_remote_name) {
      p_eir_remote_name = AdvertiseDataParser::GetFieldByType(
          p_search_data->inq_res.p_eir, p_search_data->inq_res.eir_len,
          BTM_EIR_SHORTENED_LOCAL_NAME_TYPE, &remote_name_len);
    }

    if (p_eir_remote_name) {
      if (remote_name_len > BD_NAME_LEN) remote_name_len = BD_NAME_LEN;

      if (p_remote_name && p_remote_name_len) {
        memcpy(p_remote_name, p_eir_remote_name, remote_name_len);
        *(p_remote_name + remote_name_len) = 0;
        *p_remote_name_len = remote_name_len;
      }

      return true;
    }
  }

  return false;
}

/*******************************************************************************
 *
 * Function         check_cached_remote_name
 *
 * Description      Check if remote name is in the NVRAM cache
 *
 * Returns          true if remote name found
 *                  Populate p_remote_name, if provided and remote name found
 *
 ******************************************************************************/
static bool check_cached_remote_name(tBTA_DM_SEARCH* p_search_data,
                                     uint8_t* p_remote_name,
                                     uint8_t* p_remote_name_len) {
  bt_bdname_t bdname;
  bt_property_t prop_name;

  /* check if we already have it in our btif_storage cache */

  BTIF_STORAGE_FILL_PROPERTY(&prop_name, BT_PROPERTY_BDNAME,
                             sizeof(bt_bdname_t), &bdname);
  if (btif_storage_get_remote_device_property(
          &p_search_data->inq_res.bd_addr, &prop_name) == BT_STATUS_SUCCESS) {
    if (p_remote_name && p_remote_name_len) {
      strcpy((char*)p_remote_name, (char*)bdname.name);
      *p_remote_name_len = strlen((char*)p_remote_name);
    }
    return true;
  }

  return false;
}

static uint32_t get_cod(const RawAddress* remote_bdaddr) {
  uint32_t remote_cod;
  bt_property_t prop_name;

  /* check if we already have it in our btif_storage cache */
  BTIF_STORAGE_FILL_PROPERTY(&prop_name, BT_PROPERTY_CLASS_OF_DEVICE,
                             sizeof(uint32_t), &remote_cod);
  if (btif_storage_get_remote_device_property(
          (RawAddress*)remote_bdaddr, &prop_name) == BT_STATUS_SUCCESS) {
    LOG_INFO(LOG_TAG, "%s remote_cod = 0x%08x", __func__, remote_cod);
    return remote_cod & COD_MASK;
  }

  return 0;
}

bool check_cod(const RawAddress* remote_bdaddr, uint32_t cod) {
  return get_cod(remote_bdaddr) == cod;
}

bool check_cod_hid(const RawAddress* remote_bdaddr) {
  return (get_cod(remote_bdaddr) & COD_HID_MASK) == COD_HID_MAJOR;
}

bool check_hid_le(const RawAddress* remote_bdaddr) {
  uint32_t remote_dev_type;
  bt_property_t prop_name;

  /* check if we already have it in our btif_storage cache */
  BTIF_STORAGE_FILL_PROPERTY(&prop_name, BT_PROPERTY_TYPE_OF_DEVICE,
                             sizeof(uint32_t), &remote_dev_type);
  if (btif_storage_get_remote_device_property(
          (RawAddress*)remote_bdaddr, &prop_name) == BT_STATUS_SUCCESS) {
    if (remote_dev_type == BT_DEVICE_DEVTYPE_BLE) {
      if (btif_config_exist(remote_bdaddr->ToString().c_str(), "HidAppId"))
        return true;
    }
  }
  return false;
}

/*****************************************************************************
 *
 * Function        check_sdp_bl
 *
 * Description     Checks if a given device is blacklisted to skip sdp
 *
 * Parameters     skip_sdp_entry
 *
 * Returns         true if the device is present in blacklist, else false
 *
 ******************************************************************************/
bool check_sdp_bl(const RawAddress* remote_bdaddr) {
  uint16_t manufacturer = 0;
  uint8_t lmp_ver = 0;
  uint16_t lmp_subver = 0;
  bt_property_t prop_name;
  bt_remote_version_t info;

  if (remote_bdaddr == NULL) return false;

  /* fetch additional info about remote device used in iop query */
  BTM_ReadRemoteVersion(*remote_bdaddr, &lmp_ver, &manufacturer, &lmp_subver);

  /* if not available yet, try fetching from config database */
  BTIF_STORAGE_FILL_PROPERTY(&prop_name, BT_PROPERTY_REMOTE_VERSION_INFO,
                             sizeof(bt_remote_version_t), &info);

  if (btif_storage_get_remote_device_property(remote_bdaddr, &prop_name) !=
      BT_STATUS_SUCCESS) {
    return false;
  }
  manufacturer = info.manufacturer;

  for (unsigned int i = 0; i < ARRAY_SIZE(sdp_blacklist); i++) {
    if (manufacturer == sdp_blacklist[i].manufact_id) return true;
  }
  return false;
}

static void bond_state_changed(bt_status_t status, const RawAddress& bd_addr,
                               bt_bond_state_t state) {
  btif_stats_add_bond_event(bd_addr, BTIF_DM_FUNC_BOND_STATE_CHANGED, state);

  if ((pairing_cb.state == state) && (state == BT_BOND_STATE_BONDING)) {
    // Cross key pairing so send callback for static address
    if (!pairing_cb.static_bdaddr.IsEmpty()) {
      auto tmp = bd_addr;
      HAL_CBACK(bt_hal_cbacks, bond_state_changed_cb, status, &tmp, state);
    }
    return;
  }

  if (pairing_cb.bond_type == BOND_TYPE_TEMPORARY) state = BT_BOND_STATE_NONE;

  BTIF_TRACE_DEBUG("%s: state=%d, prev_state=%d, sdp_attempts = %d", __func__,
                   state, pairing_cb.state, pairing_cb.sdp_attempts);

  auto tmp = bd_addr;
  HAL_CBACK(bt_hal_cbacks, bond_state_changed_cb, status, &tmp, state);

  int dev_type;
  if (!btif_get_device_type(bd_addr, &dev_type)) {
    dev_type = BT_DEVICE_TYPE_BREDR;
  }

  if (state == BT_BOND_STATE_BONDING ||
      (state == BT_BOND_STATE_BONDED && pairing_cb.sdp_attempts > 0)) {
    // Save state for the device is bonding or SDP.
    pairing_cb.state = state;
    pairing_cb.bd_addr = bd_addr;
  } else {
    pairing_cb = {};
  }
}

/* store remote version in bt config to always have access
   to it post pairing*/
static void btif_update_remote_version_property(RawAddress* p_bd) {
  bt_property_t property;
  uint8_t lmp_ver = 0;
  uint16_t lmp_subver = 0;
  uint16_t mfct_set = 0;
  tBTM_STATUS btm_status;
  bt_remote_version_t info;
  bt_status_t status;

  btm_status = BTM_ReadRemoteVersion(*p_bd, &lmp_ver, &mfct_set, &lmp_subver);

  LOG_DEBUG(LOG_TAG, "remote version info [%s]: %x, %x, %x",
            p_bd->ToString().c_str(), lmp_ver, mfct_set, lmp_subver);

  if (btm_status == BTM_SUCCESS) {
    // Always update cache to ensure we have availability whenever BTM API is
    // not populated
    info.manufacturer = mfct_set;
    info.sub_ver = lmp_subver;
    info.version = lmp_ver;
    BTIF_STORAGE_FILL_PROPERTY(&property, BT_PROPERTY_REMOTE_VERSION_INFO,
                               sizeof(bt_remote_version_t), &info);
    status = btif_storage_set_remote_device_property(p_bd, &property);
    ASSERTC(status == BT_STATUS_SUCCESS, "failed to save remote version",
            status);
  }
}

static void btif_update_remote_properties(const RawAddress& bdaddr,
                                          BD_NAME bd_name, DEV_CLASS dev_class,
                                          tBT_DEVICE_TYPE device_type) {
  int num_properties = 0;
  bt_property_t properties[3];
  bt_status_t status;
  uint32_t cod;
  bt_device_type_t dev_type;

  memset(properties, 0, sizeof(properties));

  /* remote name */
  if (strlen((const char*)bd_name)) {
    BTIF_STORAGE_FILL_PROPERTY(&properties[num_properties], BT_PROPERTY_BDNAME,
                               strlen((char*)bd_name), bd_name);
    status = btif_storage_set_remote_device_property(
        &bdaddr, &properties[num_properties]);
    ASSERTC(status == BT_STATUS_SUCCESS, "failed to save remote device name",
            status);
    num_properties++;
  }

  /* class of device */
  cod = devclass2uint(dev_class);
  BTIF_TRACE_DEBUG("%s cod is 0x%06x", __func__, cod);
  if (cod == 0) {
    /* Try to retrieve cod from storage */
    BTIF_TRACE_DEBUG("%s cod is 0, checking cod from storage", __func__);
    BTIF_STORAGE_FILL_PROPERTY(&properties[num_properties],
                               BT_PROPERTY_CLASS_OF_DEVICE, sizeof(cod), &cod);
    status = btif_storage_get_remote_device_property(
        &bdaddr, &properties[num_properties]);
    BTIF_TRACE_DEBUG("%s cod retrieved from storage is 0x%06x", __func__, cod);
    if (cod == 0) {
      BTIF_TRACE_DEBUG("%s cod is again 0, set as unclassified", __func__);
      cod = COD_UNCLASSIFIED;
    }
  }

  BTIF_STORAGE_FILL_PROPERTY(&properties[num_properties],
                             BT_PROPERTY_CLASS_OF_DEVICE, sizeof(cod), &cod);
  status = btif_storage_set_remote_device_property(&bdaddr,
                                                   &properties[num_properties]);
  ASSERTC(status == BT_STATUS_SUCCESS, "failed to save remote device class",
          status);
  num_properties++;

  /* device type */
  bt_property_t prop_name;
  uint8_t remote_dev_type;
  BTIF_STORAGE_FILL_PROPERTY(&prop_name, BT_PROPERTY_TYPE_OF_DEVICE,
                             sizeof(uint8_t), &remote_dev_type);
  if (btif_storage_get_remote_device_property(&bdaddr, &prop_name) ==
      BT_STATUS_SUCCESS)
    dev_type = (bt_device_type_t)(remote_dev_type | device_type);
  else
    dev_type = (bt_device_type_t)device_type;

  BTIF_STORAGE_FILL_PROPERTY(&properties[num_properties],
                             BT_PROPERTY_TYPE_OF_DEVICE, sizeof(dev_type),
                             &dev_type);
  status = btif_storage_set_remote_device_property(&bdaddr,
                                                   &properties[num_properties]);
  ASSERTC(status == BT_STATUS_SUCCESS, "failed to save remote device type",
          status);
  num_properties++;

  auto tmp = bdaddr;
  HAL_CBACK(bt_hal_cbacks, remote_device_properties_cb, status, &tmp,
            num_properties, properties);
}

/*******************************************************************************
 *
 * Function         btif_dm_cb_hid_remote_name
 *
 * Description      Remote name callback for HID device. Called in btif context
 *                  Special handling for HID devices
 *
 * Returns          void
 *
 ******************************************************************************/
static void btif_dm_cb_hid_remote_name(tBTM_REMOTE_DEV_NAME* p_remote_name) {
  BTIF_TRACE_DEBUG("%s: status=%d pairing_cb.state=%d", __func__,
                   p_remote_name->status, pairing_cb.state);
  if (pairing_cb.state == BT_BOND_STATE_BONDING) {
    if (p_remote_name->status == BTM_SUCCESS) {
      bond_state_changed(BT_STATUS_SUCCESS, pairing_cb.bd_addr,
                         BT_BOND_STATE_BONDED);
    } else
      bond_state_changed(BT_STATUS_FAIL, pairing_cb.bd_addr,
                         BT_BOND_STATE_NONE);
  }
}

/*******************************************************************************
 *
 * Function         btif_dm_cb_create_bond
 *
 * Description      Create bond initiated from the BTIF thread context
 *                  Special handling for HID devices
 *
 * Returns          void
 *
 ******************************************************************************/
static void btif_dm_cb_create_bond(const RawAddress& bd_addr,
                                   tBTA_TRANSPORT transport) {
  bool is_hid = check_cod(&bd_addr, COD_HID_POINTING);
  bond_state_changed(BT_STATUS_SUCCESS, bd_addr, BT_BOND_STATE_BONDING);

  int device_type;
  int addr_type;
  std::string addrstr = bd_addr.ToString();
  const char* bdstr = addrstr.c_str();
  if (transport == BT_TRANSPORT_LE) {
    if (!btif_config_get_int(bdstr, "DevType", &device_type)) {
      btif_config_set_int(bdstr, "DevType", BT_DEVICE_TYPE_BLE);
    }
    if (btif_storage_get_remote_addr_type(&bd_addr, &addr_type) !=
        BT_STATUS_SUCCESS) {
      // Try to read address type. OOB pairing might have set it earlier, but
      // didn't store it, it defaults to BLE_ADDR_PUBLIC
      uint8_t tmp_dev_type;
      uint8_t tmp_addr_type;
      BTM_ReadDevInfo(bd_addr, &tmp_dev_type, &tmp_addr_type);
      addr_type = tmp_addr_type;

      btif_storage_set_remote_addr_type(&bd_addr, addr_type);
    }
  }
  if ((btif_config_get_int(bdstr, "DevType", &device_type) &&
       (btif_storage_get_remote_addr_type(&bd_addr, &addr_type) ==
        BT_STATUS_SUCCESS) &&
       (device_type & BT_DEVICE_TYPE_BLE) == BT_DEVICE_TYPE_BLE) ||
      (transport == BT_TRANSPORT_LE)) {
    BTA_DmAddBleDevice(bd_addr, addr_type, device_type);
  }

  if (is_hid && (device_type & BT_DEVICE_TYPE_BLE) == 0) {
    bt_status_t status;
    status = (bt_status_t)btif_hh_connect(&bd_addr);
    if (status != BT_STATUS_SUCCESS)
      bond_state_changed(status, bd_addr, BT_BOND_STATE_NONE);
  } else {
    BTA_DmBondByTransport(bd_addr, transport);
  }
  /*  Track  originator of bond creation  */
  pairing_cb.is_local_initiated = true;
}

/*******************************************************************************
 *
 * Function         btif_dm_cb_remove_bond
 *
 * Description      remove bond initiated from the BTIF thread context
 *                  Special handling for HID devices
 *
 * Returns          void
 *
 ******************************************************************************/
void btif_dm_cb_remove_bond(const RawAddress* bd_addr) {
/*special handling for HID devices */
/*  VUP needs to be sent if its a HID Device. The HID HOST module will check if
there
is a valid hid connection with this bd_addr. If yes VUP will be issued.*/
#if (BTA_HH_INCLUDED == TRUE)
  if (btif_hh_virtual_unplug(bd_addr) != BT_STATUS_SUCCESS)
#endif
  {
    BTIF_TRACE_DEBUG("%s: Removing HH device", __func__);
    BTA_DmRemoveDevice(*bd_addr);
  }
}

/*******************************************************************************
 *
 * Function         btif_dm_get_connection_state
 *
 * Description      Returns whether the remote device is currently connected
 *                  and whether encryption is active for the connection
 *
 * Returns          0 if not connected; 1 if connected and > 1 if connection is
 *                  encrypted
 *
 ******************************************************************************/
uint16_t btif_dm_get_connection_state(const RawAddress* bd_addr) {
  uint16_t rc = BTA_DmGetConnectionState(*bd_addr);

  if (rc != 0) {
    uint8_t flags = 0;

    BTM_GetSecurityFlagsByTransport(*bd_addr, &flags, BT_TRANSPORT_BR_EDR);
    BTIF_TRACE_DEBUG("%s: security flags (BR/EDR)=0x%02x", __func__, flags);
    if (flags & BTM_SEC_FLAG_ENCRYPTED) rc |= ENCRYPTED_BREDR;

    BTM_GetSecurityFlagsByTransport(*bd_addr, &flags, BT_TRANSPORT_LE);
    BTIF_TRACE_DEBUG("%s: security flags (LE)=0x%02x", __func__, flags);
    if (flags & BTM_SEC_FLAG_ENCRYPTED) rc |= ENCRYPTED_LE;
  }

  return rc;
}

/*******************************************************************************
 *
 * Function         search_devices_copy_cb
 *
 * Description      Deep copy callback for search devices event
 *
 * Returns          void
 *
 ******************************************************************************/
static void search_devices_copy_cb(uint16_t event, char* p_dest, char* p_src) {
  tBTA_DM_SEARCH* p_dest_data = (tBTA_DM_SEARCH*)p_dest;
  tBTA_DM_SEARCH* p_src_data = (tBTA_DM_SEARCH*)p_src;

  if (!p_src) return;

  BTIF_TRACE_DEBUG("%s: event=%s", __func__, dump_dm_search_event(event));
  maybe_non_aligned_memcpy(p_dest_data, p_src_data, sizeof(*p_src_data));
  switch (event) {
    case BTA_DM_INQ_RES_EVT: {
      if (p_src_data->inq_res.p_eir) {
        p_dest_data->inq_res.p_eir =
            (uint8_t*)(p_dest + sizeof(tBTA_DM_SEARCH));
        memcpy(p_dest_data->inq_res.p_eir, p_src_data->inq_res.p_eir,
               p_src_data->inq_res.eir_len);
        p_dest_data->inq_res.eir_len = p_src_data->inq_res.eir_len;
      }
    } break;

    case BTA_DM_DISC_RES_EVT: {
      if (p_src_data->disc_res.raw_data_size &&
          p_src_data->disc_res.p_raw_data) {
        p_dest_data->disc_res.p_raw_data =
            (uint8_t*)(p_dest + sizeof(tBTA_DM_SEARCH));
        memcpy(p_dest_data->disc_res.p_raw_data,
               p_src_data->disc_res.p_raw_data,
               p_src_data->disc_res.raw_data_size);
      }
    } break;
  }
}

static void search_services_copy_cb(uint16_t event, char* p_dest, char* p_src) {
  tBTA_DM_SEARCH* p_dest_data = (tBTA_DM_SEARCH*)p_dest;
  tBTA_DM_SEARCH* p_src_data = (tBTA_DM_SEARCH*)p_src;

  if (!p_src) return;
  maybe_non_aligned_memcpy(p_dest_data, p_src_data, sizeof(*p_src_data));
  switch (event) {
    case BTA_DM_DISC_RES_EVT: {
      if (p_src_data->disc_res.result == BTA_SUCCESS) {
        if (p_src_data->disc_res.num_uuids > 0) {
          p_dest_data->disc_res.p_uuid_list =
              (Uuid*)(p_dest + sizeof(tBTA_DM_SEARCH));
          memcpy(p_dest_data->disc_res.p_uuid_list,
                 p_src_data->disc_res.p_uuid_list,
                 p_src_data->disc_res.num_uuids * sizeof(Uuid));
          osi_free_and_reset((void**)&p_src_data->disc_res.p_uuid_list);
        }
        osi_free_and_reset((void**)&p_src_data->disc_res.p_raw_data);
      }
    } break;
  }
}
/******************************************************************************
 *
 *  BTIF DM callback events
 *
 ****************************************************************************/

/*******************************************************************************
 *
 * Function         btif_dm_pin_req_evt
 *
 * Description      Executes pin request event in btif context
 *
 * Returns          void
 *
 ******************************************************************************/
static void btif_dm_pin_req_evt(tBTA_DM_PIN_REQ* p_pin_req) {
  bt_bdname_t bd_name;
  uint32_t cod;
  bt_pin_code_t pin_code;
  int dev_type;

  /* Remote properties update */
  if (!btif_get_device_type(p_pin_req->bd_addr, &dev_type)) {
    dev_type = BT_DEVICE_TYPE_BREDR;
  }
  btif_update_remote_properties(p_pin_req->bd_addr, p_pin_req->bd_name,
                                p_pin_req->dev_class,
                                (tBT_DEVICE_TYPE)dev_type);

  const RawAddress& bd_addr = p_pin_req->bd_addr;
  memcpy(bd_name.name, p_pin_req->bd_name, BD_NAME_LEN);

  if (pairing_cb.state == BT_BOND_STATE_BONDING &&
      bd_addr != pairing_cb.bd_addr) {
    BTIF_TRACE_WARNING("%s(): already in bonding state, reject request",
                       __FUNCTION__);
    btif_dm_pin_reply(&bd_addr, 0, 0, NULL);
    return;
  }

  bond_state_changed(BT_STATUS_SUCCESS, bd_addr, BT_BOND_STATE_BONDING);

  cod = devclass2uint(p_pin_req->dev_class);

  if (cod == 0) {
    BTIF_TRACE_DEBUG("%s cod is 0, set as unclassified", __func__);
    cod = COD_UNCLASSIFIED;
  }

  /* check for auto pair possiblity only if bond was initiated by local device
   */
  if (pairing_cb.is_local_initiated && !p_pin_req->min_16_digit) {
    if (check_cod(&bd_addr, COD_AV_HEADSETS) ||
        check_cod(&bd_addr, COD_AV_HEADPHONES) ||
        check_cod(&bd_addr, COD_AV_PORTABLE_AUDIO) ||
        check_cod(&bd_addr, COD_AV_HIFI_AUDIO) ||
        check_cod(&bd_addr, COD_HID_POINTING)) {
      /*  Check if this device can be auto paired  */
      if (!interop_match_addr(INTEROP_DISABLE_AUTO_PAIRING, &bd_addr) &&
          !interop_match_name(INTEROP_DISABLE_AUTO_PAIRING,
                              (const char*)bd_name.name) &&
          (pairing_cb.autopair_attempts == 0)) {
        BTIF_TRACE_DEBUG("%s() Attempting auto pair", __func__);
        pin_code.pin[0] = 0x30;
        pin_code.pin[1] = 0x30;
        pin_code.pin[2] = 0x30;
        pin_code.pin[3] = 0x30;

        pairing_cb.autopair_attempts++;
        BTA_DmPinReply(bd_addr, true, 4, pin_code.pin);
        return;
      }
    } else if (check_cod(&bd_addr, COD_HID_KEYBOARD) ||
               check_cod(&bd_addr, COD_HID_COMBO)) {
      if ((interop_match_addr(INTEROP_KEYBOARD_REQUIRES_FIXED_PIN, &bd_addr) ==
           true) &&
          (pairing_cb.autopair_attempts == 0)) {
        BTIF_TRACE_DEBUG("%s() Attempting auto pair", __func__);
        pin_code.pin[0] = 0x30;
        pin_code.pin[1] = 0x30;
        pin_code.pin[2] = 0x30;
        pin_code.pin[3] = 0x30;

        pairing_cb.autopair_attempts++;
        BTA_DmPinReply(bd_addr, true, 4, pin_code.pin);
        return;
      }
    }
  }
  // TODO: make cback accept const and get rid of tmp!
  auto tmp = bd_addr;
  HAL_CBACK(bt_hal_cbacks, pin_request_cb, &tmp, &bd_name, cod,
            p_pin_req->min_16_digit);
}

/*******************************************************************************
 *
 * Function         btif_dm_ssp_cfm_req_evt
 *
 * Description      Executes SSP confirm request event in btif context
 *
 * Returns          void
 *
 ******************************************************************************/
static void btif_dm_ssp_cfm_req_evt(tBTA_DM_SP_CFM_REQ* p_ssp_cfm_req) {
  bt_bdname_t bd_name;
  uint32_t cod;
  bool is_incoming = !(pairing_cb.state == BT_BOND_STATE_BONDING);
  int dev_type;

  BTIF_TRACE_DEBUG("%s", __func__);

  /* Remote properties update */
  if (!btif_get_device_type(p_ssp_cfm_req->bd_addr, &dev_type)) {
    dev_type = BT_DEVICE_TYPE_BREDR;
  }
  btif_update_remote_properties(p_ssp_cfm_req->bd_addr, p_ssp_cfm_req->bd_name,
                                p_ssp_cfm_req->dev_class,
                                (tBT_DEVICE_TYPE)dev_type);

  RawAddress bd_addr = p_ssp_cfm_req->bd_addr;
  memcpy(bd_name.name, p_ssp_cfm_req->bd_name, BD_NAME_LEN);

  if (pairing_cb.state == BT_BOND_STATE_BONDING &&
      bd_addr != pairing_cb.bd_addr) {
    BTIF_TRACE_WARNING("%s(): already in bonding state, reject request",
                       __FUNCTION__);
    btif_dm_ssp_reply(&bd_addr, BT_SSP_VARIANT_PASSKEY_CONFIRMATION, 0, 0);
    return;
  }

  /* Set the pairing_cb based on the local & remote authentication requirements
   */
  bond_state_changed(BT_STATUS_SUCCESS, bd_addr, BT_BOND_STATE_BONDING);

  BTIF_TRACE_EVENT("%s: just_works:%d, loc_auth_req=%d, rmt_auth_req=%d",
                   __func__, p_ssp_cfm_req->just_works,
                   p_ssp_cfm_req->loc_auth_req, p_ssp_cfm_req->rmt_auth_req);

  /* if just_works and bonding bit is not set treat this as temporary */
  if (p_ssp_cfm_req->just_works &&
      !(p_ssp_cfm_req->loc_auth_req & BTM_AUTH_BONDS) &&
      !(p_ssp_cfm_req->rmt_auth_req & BTM_AUTH_BONDS) &&
      !(check_cod((RawAddress*)&p_ssp_cfm_req->bd_addr, COD_HID_POINTING)))
    pairing_cb.bond_type = BOND_TYPE_TEMPORARY;
  else
    pairing_cb.bond_type = BOND_TYPE_PERSISTENT;

  btm_set_bond_type_dev(p_ssp_cfm_req->bd_addr, pairing_cb.bond_type);

  pairing_cb.is_ssp = true;

  /* If JustWorks auto-accept */
  if (p_ssp_cfm_req->just_works) {
    /* Pairing consent for JustWorks NOT needed if:
     * 1. Incoming temporary pairing is detected
     */
    if (is_incoming && pairing_cb.bond_type == BOND_TYPE_TEMPORARY) {
      BTIF_TRACE_EVENT(
          "%s: Auto-accept JustWorks pairing for temporary incoming", __func__);
      btif_dm_ssp_reply(&bd_addr, BT_SSP_VARIANT_CONSENT, true, 0);
      return;
    }
  }

  cod = devclass2uint(p_ssp_cfm_req->dev_class);

  if (cod == 0) {
    LOG_DEBUG(LOG_TAG, "%s cod is 0, set as unclassified", __func__);
    cod = COD_UNCLASSIFIED;
  }

  pairing_cb.sdp_attempts = 0;
  HAL_CBACK(bt_hal_cbacks, ssp_request_cb, &bd_addr, &bd_name, cod,
            (p_ssp_cfm_req->just_works ? BT_SSP_VARIANT_CONSENT
                                       : BT_SSP_VARIANT_PASSKEY_CONFIRMATION),
            p_ssp_cfm_req->num_val);
}

static void btif_dm_ssp_key_notif_evt(tBTA_DM_SP_KEY_NOTIF* p_ssp_key_notif) {
  bt_bdname_t bd_name;
  uint32_t cod;
  int dev_type;

  BTIF_TRACE_DEBUG("%s", __func__);

  /* Remote properties update */
  if (!btif_get_device_type(p_ssp_key_notif->bd_addr, &dev_type)) {
    dev_type = BT_DEVICE_TYPE_BREDR;
  }
  btif_update_remote_properties(
      p_ssp_key_notif->bd_addr, p_ssp_key_notif->bd_name,
      p_ssp_key_notif->dev_class, (tBT_DEVICE_TYPE)dev_type);

  RawAddress bd_addr = p_ssp_key_notif->bd_addr;
  memcpy(bd_name.name, p_ssp_key_notif->bd_name, BD_NAME_LEN);

  bond_state_changed(BT_STATUS_SUCCESS, bd_addr, BT_BOND_STATE_BONDING);
  pairing_cb.is_ssp = true;
  cod = devclass2uint(p_ssp_key_notif->dev_class);

  if (cod == 0) {
    LOG_DEBUG(LOG_TAG, "%s cod is 0, set as unclassified", __func__);
    cod = COD_UNCLASSIFIED;
  }

  HAL_CBACK(bt_hal_cbacks, ssp_request_cb, &bd_addr, &bd_name, cod,
            BT_SSP_VARIANT_PASSKEY_NOTIFICATION, p_ssp_key_notif->passkey);
}
/*******************************************************************************
 *
 * Function         btif_dm_auth_cmpl_evt
 *
 * Description      Executes authentication complete event in btif context
 *
 * Returns          void
 *
 ******************************************************************************/
static void btif_dm_auth_cmpl_evt(tBTA_DM_AUTH_CMPL* p_auth_cmpl) {
  /* Save link key, if not temporary */
  bt_status_t status = BT_STATUS_FAIL;
  bt_bond_state_t state = BT_BOND_STATE_NONE;
  bool skip_sdp = false;

  BTIF_TRACE_DEBUG("%s: bond state=%d, success=%d, key_present=%d", __func__,
                   pairing_cb.state, p_auth_cmpl->success,
                   p_auth_cmpl->key_present);

  RawAddress bd_addr = p_auth_cmpl->bd_addr;
  if ((p_auth_cmpl->success) && (p_auth_cmpl->key_present)) {
    if ((p_auth_cmpl->key_type < HCI_LKEY_TYPE_DEBUG_COMB) ||
        (p_auth_cmpl->key_type == HCI_LKEY_TYPE_AUTH_COMB) ||
        (p_auth_cmpl->key_type == HCI_LKEY_TYPE_CHANGED_COMB) ||
        (p_auth_cmpl->key_type == HCI_LKEY_TYPE_AUTH_COMB_P_256) ||
        pairing_cb.bond_type == BOND_TYPE_PERSISTENT) {
      bt_status_t ret;
      BTIF_TRACE_DEBUG("%s: Storing link key. key_type=0x%x, bond_type=%d",
                       __func__, p_auth_cmpl->key_type, pairing_cb.bond_type);
      ret = btif_storage_add_bonded_device(&bd_addr, p_auth_cmpl->key,
                                           p_auth_cmpl->key_type,
                                           pairing_cb.pin_code_len);
      ASSERTC(ret == BT_STATUS_SUCCESS, "storing link key failed", ret);
    } else {
      BTIF_TRACE_DEBUG(
          "%s: Temporary key. Not storing. key_type=0x%x, bond_type=%d",
          __func__, p_auth_cmpl->key_type, pairing_cb.bond_type);
      if (pairing_cb.bond_type == BOND_TYPE_TEMPORARY) {
        BTIF_TRACE_DEBUG("%s: sending BT_BOND_STATE_NONE for Temp pairing",
                         __func__);
        btif_storage_remove_bonded_device(&bd_addr);
        bond_state_changed(BT_STATUS_SUCCESS, bd_addr, BT_BOND_STATE_NONE);
        return;
      }
    }
  }

  if (p_auth_cmpl->success) {
    // We could have received a new link key without going through the pairing
    // flow.  If so, we don't want to perform SDP or any other operations on the
    // authenticated device. Also, make sure that the link key is not derived
    // from secure LTK, because we will need to perform SDP in case of link key
    // derivation to allow bond state change notification for the BR/EDR
    // transport so that the subsequent BR/EDR connections to the remote can use
    // the derived link key.
    if (p_auth_cmpl->bd_addr != pairing_cb.bd_addr &&
        (!pairing_cb.ble.is_penc_key_rcvd)) {
      LOG(INFO) << __func__
                << " skipping SDP since we did not initiate pairing to "
                << p_auth_cmpl->bd_addr;
      return;
    }

    btif_storage_set_remote_addr_type(&bd_addr, p_auth_cmpl->addr_type);
    btif_update_remote_properties(p_auth_cmpl->bd_addr, p_auth_cmpl->bd_name,
                                  NULL, p_auth_cmpl->dev_type);
    pairing_cb.timeout_retries = 0;
    status = BT_STATUS_SUCCESS;
    state = BT_BOND_STATE_BONDED;
    bd_addr = p_auth_cmpl->bd_addr;

    if (check_sdp_bl(&bd_addr) && check_cod_hid(&bd_addr)) {
      LOG_WARN(LOG_TAG, "%s:skip SDP", __func__);
      skip_sdp = true;
    }
    if (!pairing_cb.is_local_initiated && skip_sdp) {
      bond_state_changed(status, bd_addr, state);

      LOG_WARN(LOG_TAG, "%s: Incoming HID Connection", __func__);
      bt_property_t prop;
      RawAddress bd_addr;
      Uuid uuid = Uuid::From16Bit(UUID_SERVCLASS_HUMAN_INTERFACE);

      prop.type = BT_PROPERTY_UUIDS;
      prop.val = &uuid;
      prop.len = Uuid::kNumBytes128;

      /* Send the event to the BTIF */
      HAL_CBACK(bt_hal_cbacks, remote_device_properties_cb, BT_STATUS_SUCCESS,
                &bd_addr, 1, &prop);
    } else {
      bool is_crosskey = false;
      /* If bonded due to cross-key, save the static address too*/
      if (pairing_cb.state == BT_BOND_STATE_BONDING &&
          p_auth_cmpl->bd_addr != pairing_cb.bd_addr) {
        BTIF_TRACE_DEBUG(
            "%s: bonding initiated due to cross key, adding static address",
            __func__);
        pairing_cb.static_bdaddr = bd_addr;
        is_crosskey = true;
      }
      if (!is_crosskey ||
          !(stack_config_get_interface()->get_pts_crosskey_sdp_disable())) {
        // Ensure inquiry is stopped before attempting service discovery
        btif_dm_cancel_discovery();

        /* Trigger SDP on the device */
        pairing_cb.sdp_attempts = 1;

        if (is_crosskey) {
          // If bonding occurred due to cross-key pairing, send bonding callback
          // for static address now
          LOG_INFO(LOG_TAG,
                   "%s: send bonding state update for static address %s",
                   __func__, bd_addr.ToString().c_str());
          bond_state_changed(BT_STATUS_SUCCESS, bd_addr, BT_BOND_STATE_BONDING);
        }
        bond_state_changed(BT_STATUS_SUCCESS, bd_addr, BT_BOND_STATE_BONDED);

        btif_dm_get_remote_services(bd_addr);
      }
    }
    // Do not call bond_state_changed_cb yet. Wait until remote service
    // discovery is complete
  } else {
    // Map the HCI fail reason  to  bt status
    switch (p_auth_cmpl->fail_reason) {
      case HCI_ERR_PAGE_TIMEOUT:
      case HCI_ERR_LMP_RESPONSE_TIMEOUT:
        if (interop_match_addr(INTEROP_AUTO_RETRY_PAIRING, &bd_addr) &&
            pairing_cb.timeout_retries) {
          BTIF_TRACE_WARNING("%s() - Pairing timeout; retrying (%d) ...",
                             __func__, pairing_cb.timeout_retries);
          --pairing_cb.timeout_retries;
          btif_dm_cb_create_bond(bd_addr, BTA_TRANSPORT_UNKNOWN);
          return;
        }
        FALLTHROUGH_INTENDED; /* FALLTHROUGH */
      case HCI_ERR_CONNECTION_TOUT:
        status = BT_STATUS_RMT_DEV_DOWN;
        break;

      case HCI_ERR_PAIRING_NOT_ALLOWED:
        btif_storage_remove_bonded_device(&bd_addr);
        status = BT_STATUS_AUTH_REJECTED;
        break;

      /* map the auth failure codes, so we can retry pairing if necessary */
      case HCI_ERR_AUTH_FAILURE:
      case HCI_ERR_KEY_MISSING:
        btif_storage_remove_bonded_device(&bd_addr);
      case HCI_ERR_HOST_REJECT_SECURITY:
      case HCI_ERR_ENCRY_MODE_NOT_ACCEPTABLE:
      case HCI_ERR_UNIT_KEY_USED:
      case HCI_ERR_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED:
      case HCI_ERR_INSUFFCIENT_SECURITY:
      case HCI_ERR_PEER_USER:
      case HCI_ERR_UNSPECIFIED:
        BTIF_TRACE_DEBUG(" %s() Authentication fail reason %d", __func__,
                         p_auth_cmpl->fail_reason);
        if (pairing_cb.autopair_attempts == 1) {
          /* Create the Bond once again */
          BTIF_TRACE_WARNING("%s() auto pair failed. Reinitiate Bond",
                             __func__);
          btif_dm_cb_create_bond(bd_addr, BTA_TRANSPORT_UNKNOWN);
          return;
        } else {
          /* if autopair attempts are more than 1, or not attempted */
          status = BT_STATUS_AUTH_FAILURE;
        }
        break;

      default:
        status = BT_STATUS_FAIL;
    }
    /* Special Handling for HID Devices */
    if (check_cod(&bd_addr, COD_HID_POINTING)) {
      /* Remove Device as bonded in nvram as authentication failed */
      BTIF_TRACE_DEBUG("%s(): removing hid pointing device from nvram",
                       __func__);
      btif_storage_remove_bonded_device(&bd_addr);
    }
    bond_state_changed(status, bd_addr, state);
  }
}

/******************************************************************************
 *
 * Function         btif_dm_search_devices_evt
 *
 * Description      Executes search devices callback events in btif context
 *
 * Returns          void
 *
 *****************************************************************************/
static void btif_dm_search_devices_evt(uint16_t event, char* p_param) {
  tBTA_DM_SEARCH* p_search_data;
  BTIF_TRACE_EVENT("%s event=%s", __func__, dump_dm_search_event(event));

  switch (event) {
    case BTA_DM_DISC_RES_EVT: {
      p_search_data = (tBTA_DM_SEARCH*)p_param;
      /* Remote name update */
      if (strlen((const char*)p_search_data->disc_res.bd_name)) {
        bt_property_t properties[1];
        bt_status_t status;

        properties[0].type = BT_PROPERTY_BDNAME;
        properties[0].val = p_search_data->disc_res.bd_name;
        properties[0].len = strlen((char*)p_search_data->disc_res.bd_name);
        RawAddress& bdaddr = p_search_data->disc_res.bd_addr;

        status =
            btif_storage_set_remote_device_property(&bdaddr, &properties[0]);
        ASSERTC(status == BT_STATUS_SUCCESS,
                "failed to save remote device property", status);
        HAL_CBACK(bt_hal_cbacks, remote_device_properties_cb, status, &bdaddr,
                  1, properties);
      }
      /* TODO: Services? */
    } break;

    case BTA_DM_INQ_RES_EVT: {
      /* inquiry result */
      bt_bdname_t bdname;
      uint8_t remote_name_len;
      tBTA_SERVICE_MASK services = 0;

      p_search_data = (tBTA_DM_SEARCH*)p_param;
      RawAddress& bdaddr = p_search_data->inq_res.bd_addr;

      BTIF_TRACE_DEBUG("%s() %s device_type = 0x%x\n", __func__,
                       bdaddr.ToString().c_str(),
                       p_search_data->inq_res.device_type);
      bdname.name[0] = 0;

      if (!check_eir_remote_name(p_search_data, bdname.name, &remote_name_len))
        check_cached_remote_name(p_search_data, bdname.name, &remote_name_len);

      /* Check EIR for remote name and services */
      if (p_search_data->inq_res.p_eir) {
        BTA_GetEirService(p_search_data->inq_res.p_eir,
                          p_search_data->inq_res.eir_len, &services);
        BTIF_TRACE_DEBUG("%s()EIR BTA services = %08X", __func__,
                         (uint32_t)services);
        /* TODO:  Get the service list and check to see which uuids we got and
         * send it back to the client. */
      }

      {
        bt_property_t properties[5];
        bt_device_type_t dev_type;
        uint32_t num_properties = 0;
        bt_status_t status;
        int addr_type = 0;

        memset(properties, 0, sizeof(properties));
        /* RawAddress */
        BTIF_STORAGE_FILL_PROPERTY(&properties[num_properties],
                                   BT_PROPERTY_BDADDR, sizeof(bdaddr), &bdaddr);
        num_properties++;
        /* BD_NAME */
        /* Don't send BDNAME if it is empty */
        if (bdname.name[0]) {
          BTIF_STORAGE_FILL_PROPERTY(&properties[num_properties],
                                     BT_PROPERTY_BDNAME,
                                     strlen((char*)bdname.name), &bdname);
          num_properties++;
        }

        /* DEV_CLASS */
        uint32_t cod = devclass2uint(p_search_data->inq_res.dev_class);
        BTIF_TRACE_DEBUG("%s cod is 0x%06x", __func__, cod);
        if (cod != 0) {
          BTIF_STORAGE_FILL_PROPERTY(&properties[num_properties],
                                     BT_PROPERTY_CLASS_OF_DEVICE, sizeof(cod),
                                     &cod);
          num_properties++;
        }

        /* DEV_TYPE */
        /* FixMe: Assumption is that bluetooth.h and BTE enums match */

        /* Verify if the device is dual mode in NVRAM */
        int stored_device_type = 0;
        if (btif_get_device_type(bdaddr, &stored_device_type) &&
            ((stored_device_type != BT_DEVICE_TYPE_BREDR &&
              p_search_data->inq_res.device_type == BT_DEVICE_TYPE_BREDR) ||
             (stored_device_type != BT_DEVICE_TYPE_BLE &&
              p_search_data->inq_res.device_type == BT_DEVICE_TYPE_BLE))) {
          dev_type = (bt_device_type_t)BT_DEVICE_TYPE_DUMO;
        } else {
          dev_type = (bt_device_type_t)p_search_data->inq_res.device_type;
        }

        if (p_search_data->inq_res.device_type == BT_DEVICE_TYPE_BLE)
          addr_type = p_search_data->inq_res.ble_addr_type;
        BTIF_STORAGE_FILL_PROPERTY(&properties[num_properties],
                                   BT_PROPERTY_TYPE_OF_DEVICE, sizeof(dev_type),
                                   &dev_type);
        num_properties++;
        /* RSSI */
        BTIF_STORAGE_FILL_PROPERTY(&properties[num_properties],
                                   BT_PROPERTY_REMOTE_RSSI, sizeof(int8_t),
                                   &(p_search_data->inq_res.rssi));
        num_properties++;

        status =
            btif_storage_add_remote_device(&bdaddr, num_properties, properties);
        ASSERTC(status == BT_STATUS_SUCCESS,
                "failed to save remote device (inquiry)", status);
        status = btif_storage_set_remote_addr_type(&bdaddr, addr_type);
        ASSERTC(status == BT_STATUS_SUCCESS,
                "failed to save remote addr type (inquiry)", status);
        /* Callback to notify upper layer of device */
        HAL_CBACK(bt_hal_cbacks, device_found_cb, num_properties, properties);
      }
    } break;

    case BTA_DM_INQ_CMPL_EVT: {
      do_in_main_thread(
          FROM_HERE,
          base::Bind(&BTM_BleAdvFilterParamSetup, BTM_BLE_SCAN_COND_DELETE, 0,
                     nullptr, base::Bind(&bte_scan_filt_param_cfg_evt, 0)));
    } break;
    case BTA_DM_DISC_CMPL_EVT: {
      HAL_CBACK(bt_hal_cbacks, discovery_state_changed_cb,
                BT_DISCOVERY_STOPPED);
    } break;
    case BTA_DM_SEARCH_CANCEL_CMPL_EVT: {
      /* if inquiry is not in progress and we get a cancel event, then
       * it means we are done with inquiry, but remote_name fetches are in
       * progress
       *
       * if inquiry  is in progress, then we don't want to act on this
       * cancel_cmpl_evt
       * but instead wait for the cancel_cmpl_evt via the Busy Level
       *
       */
      if (!btif_dm_inquiry_in_progress) {
        btgatt_filt_param_setup_t adv_filt_param;
        memset(&adv_filt_param, 0, sizeof(btgatt_filt_param_setup_t));
        do_in_main_thread(
            FROM_HERE,
            base::Bind(&BTM_BleAdvFilterParamSetup, BTM_BLE_SCAN_COND_DELETE, 0,
                       nullptr, base::Bind(&bte_scan_filt_param_cfg_evt, 0)));
        HAL_CBACK(bt_hal_cbacks, discovery_state_changed_cb,
                  BT_DISCOVERY_STOPPED);
      }
    } break;
  }
}

/*******************************************************************************
 *
 * Function         btif_dm_search_services_evt
 *
 * Description      Executes search services event in btif context
 *
 * Returns          void
 *
 ******************************************************************************/
static void btif_dm_search_services_evt(uint16_t event, char* p_param) {
  tBTA_DM_SEARCH* p_data = (tBTA_DM_SEARCH*)p_param;

  BTIF_TRACE_EVENT("%s:  event = %d", __func__, event);
  switch (event) {
    case BTA_DM_DISC_RES_EVT: {
      bt_property_t prop;
      uint32_t i = 0;
      bt_status_t ret;

      RawAddress& bd_addr = p_data->disc_res.bd_addr;

      BTIF_TRACE_DEBUG("%s:(result=0x%x, services 0x%x)", __func__,
                       p_data->disc_res.result, p_data->disc_res.services);
      if (p_data->disc_res.result != BTA_SUCCESS &&
          pairing_cb.state == BT_BOND_STATE_BONDED &&
          pairing_cb.sdp_attempts < BTIF_DM_MAX_SDP_ATTEMPTS_AFTER_PAIRING) {
        if (pairing_cb.sdp_attempts) {
          BTIF_TRACE_WARNING("%s: SDP failed after bonding re-attempting",
                             __func__);
          pairing_cb.sdp_attempts++;
          btif_dm_get_remote_services(bd_addr);
        } else {
          BTIF_TRACE_WARNING("%s: SDP triggered by someone failed when bonding",
                             __func__);
        }
        return;
      }
      prop.type = BT_PROPERTY_UUIDS;
      prop.len = 0;
      if ((p_data->disc_res.result == BTA_SUCCESS) &&
          (p_data->disc_res.num_uuids > 0)) {
        prop.val = p_data->disc_res.p_uuid_list;
        prop.len = p_data->disc_res.num_uuids * Uuid::kNumBytes128;
        for (i = 0; i < p_data->disc_res.num_uuids; i++) {
          std::string temp = ((p_data->disc_res.p_uuid_list + i))->ToString();
          LOG_INFO(LOG_TAG, "%s index:%d uuid:%s", __func__, i, temp.c_str());
        }
      }

      /* onUuidChanged requires getBondedDevices to be populated.
      ** bond_state_changed needs to be sent prior to remote_device_property
      */
      if (pairing_cb.state == BT_BOND_STATE_BONDED && pairing_cb.sdp_attempts &&
          (p_data->disc_res.bd_addr == pairing_cb.bd_addr ||
           p_data->disc_res.bd_addr == pairing_cb.static_bdaddr)) {
        LOG_INFO(LOG_TAG, "%s: SDP search done for %s", __func__,
                 bd_addr.ToString().c_str());
        pairing_cb.sdp_attempts = 0;

        // Both SDP and bonding are done, clear pairing control block in case
        // it is not already cleared
        pairing_cb = {};

        // Send one empty UUID to Java to unblock pairing intent when SDP failed
        // or no UUID is discovered
        if (p_data->disc_res.result != BTA_SUCCESS ||
            p_data->disc_res.num_uuids == 0) {
          LOG_INFO(LOG_TAG,
                   "%s: SDP failed, send empty UUID to unblock bonding %s",
                   __func__, bd_addr.ToString().c_str());
          bt_property_t prop;
          Uuid uuid = {};

          prop.type = BT_PROPERTY_UUIDS;
          prop.val = &uuid;
          prop.len = Uuid::kNumBytes128;

          /* Send the event to the BTIF */
          HAL_CBACK(bt_hal_cbacks, remote_device_properties_cb,
                    BT_STATUS_SUCCESS, &bd_addr, 1, &prop);
          break;
        }
      }

      if (p_data->disc_res.num_uuids != 0) {
        /* Also write this to the NVRAM */
        ret = btif_storage_set_remote_device_property(&bd_addr, &prop);
        ASSERTC(ret == BT_STATUS_SUCCESS, "storing remote services failed",
                ret);
        /* Send the event to the BTIF */
        HAL_CBACK(bt_hal_cbacks, remote_device_properties_cb, BT_STATUS_SUCCESS,
                  &bd_addr, 1, &prop);
      }
    } break;

    case BTA_DM_DISC_CMPL_EVT:
      /* fixme */
      break;

    case BTA_DM_SEARCH_CANCEL_CMPL_EVT:
      /* no-op */
      break;

    case BTA_DM_DISC_BLE_RES_EVT: {
      BTIF_TRACE_DEBUG("%s: service %s", __func__,
                       p_data->disc_ble_res.service.ToString().c_str());
      int num_properties = 0;
      if (p_data->disc_ble_res.service.As16Bit() == UUID_SERVCLASS_LE_HID ||
          p_data->disc_ble_res.service == UUID_HEARING_AID) {
        BTIF_TRACE_DEBUG("%s: Found HOGP or HEARING AID UUID", __func__);
        bt_property_t prop[2];
        bt_status_t ret;

        const auto& arr = p_data->disc_ble_res.service.To128BitBE();

        RawAddress& bd_addr = p_data->disc_ble_res.bd_addr;
        prop[0].type = BT_PROPERTY_UUIDS;
        prop[0].val = (void*)arr.data();
        prop[0].len = Uuid::kNumBytes128;

        /* Also write this to the NVRAM */
        ret = btif_storage_set_remote_device_property(&bd_addr, &prop[0]);
        ASSERTC(ret == BT_STATUS_SUCCESS, "storing remote services failed",
                ret);
        num_properties++;

        /* Remote name update */
        if (strnlen((const char*)p_data->disc_ble_res.bd_name, BD_NAME_LEN)) {
          prop[1].type = BT_PROPERTY_BDNAME;
          prop[1].val = p_data->disc_ble_res.bd_name;
          prop[1].len =
              strnlen((char*)p_data->disc_ble_res.bd_name, BD_NAME_LEN);

          ret = btif_storage_set_remote_device_property(&bd_addr, &prop[1]);
          ASSERTC(ret == BT_STATUS_SUCCESS,
                  "failed to save remote device property", ret);
          num_properties++;
        }

        /* Send the event to the BTIF */
        HAL_CBACK(bt_hal_cbacks, remote_device_properties_cb, BT_STATUS_SUCCESS,
                  &bd_addr, num_properties, prop);
      }
    } break;

    default: { ASSERTC(0, "unhandled search services event", event); } break;
  }
}

/*******************************************************************************
 *
 * Function         btif_dm_remote_service_record_evt
 *
 * Description      Executes search service record event in btif context
 *
 * Returns          void
 *
 ******************************************************************************/
static void btif_dm_remote_service_record_evt(uint16_t event, char* p_param) {
  tBTA_DM_SEARCH* p_data = (tBTA_DM_SEARCH*)p_param;

  BTIF_TRACE_EVENT("%s:  event = %d", __func__, event);
  switch (event) {
    case BTA_DM_DISC_RES_EVT: {
      bt_service_record_t rec;
      bt_property_t prop;

      memset(&rec, 0, sizeof(bt_service_record_t));
      RawAddress& bd_addr = p_data->disc_res.bd_addr;

      BTIF_TRACE_DEBUG("%s:(result=0x%x, services 0x%x)", __func__,
                       p_data->disc_res.result, p_data->disc_res.services);
      prop.type = BT_PROPERTY_SERVICE_RECORD;
      prop.val = (void*)&rec;
      prop.len = sizeof(rec);

      /* disc_res.result is overloaded with SCN. Cannot check result */
      p_data->disc_res.services &= ~BTA_USER_SERVICE_MASK;
      /* TODO: Get the UUID as well */
      rec.channel = p_data->disc_res.result - 3;
      /* TODO: Need to get the service name using p_raw_data */
      rec.name[0] = 0;

      HAL_CBACK(bt_hal_cbacks, remote_device_properties_cb, BT_STATUS_SUCCESS,
                &bd_addr, 1, &prop);
    } break;

    default: {
      ASSERTC(0, "unhandled remote service record event", event);
    } break;
  }
}

/*******************************************************************************
 *
 * Function         btif_dm_upstreams_cback
 *
 * Description      Executes UPSTREAMS events in btif context
 *
 * Returns          void
 *
 ******************************************************************************/
static void btif_dm_upstreams_evt(uint16_t event, char* p_param) {
  tBTA_DM_SEC* p_data = (tBTA_DM_SEC*)p_param;
  tBTA_SERVICE_MASK service_mask;
  uint32_t i;
  RawAddress bd_addr;

  BTIF_TRACE_EVENT("%s: ev: %s", __func__, dump_dm_event(event));

  switch (event) {
    case BTA_DM_ENABLE_EVT: {
      BD_NAME bdname;
      bt_status_t status;
      bt_property_t prop;
      prop.type = BT_PROPERTY_BDNAME;
      prop.len = BD_NAME_LEN;
      prop.val = (void*)bdname;

      status = btif_storage_get_adapter_property(&prop);
      if (status == BT_STATUS_SUCCESS) {
        /* A name exists in the storage. Make this the device name */
        BTA_DmSetDeviceName((char*)prop.val);
      } else {
        /* Storage does not have a name yet.
         * Use the default name and write it to the chip
         */
        BTA_DmSetDeviceName(btif_get_default_local_name());
      }

      /* Enable local privacy */
      BTA_DmBleConfigLocalPrivacy(BLE_LOCAL_PRIVACY_ENABLED);

      /* for each of the enabled services in the mask, trigger the profile
       * enable */
      service_mask = btif_get_enabled_services_mask();
      for (i = 0; i <= BTA_MAX_SERVICE_ID; i++) {
        if (service_mask &
            (tBTA_SERVICE_MASK)(BTA_SERVICE_ID_TO_SERVICE_MASK(i))) {
          btif_in_execute_service_request(i, true);
        }
      }
      /* clear control blocks */
      memset(&pairing_cb, 0, sizeof(btif_dm_pairing_cb_t));
      pairing_cb.bond_type = BOND_TYPE_PERSISTENT;

      /* This function will also trigger the adapter_properties_cb
      ** and bonded_devices_info_cb
      */
      btif_storage_load_bonded_devices();
      bluetooth::bqr::EnableBtQualityReport(true);
      btif_enable_bluetooth_evt(p_data->enable.status);
    } break;

    case BTA_DM_DISABLE_EVT:
      /* for each of the enabled services in the mask, trigger the profile
       * disable */
      service_mask = btif_get_enabled_services_mask();
      for (i = 0; i <= BTA_MAX_SERVICE_ID; i++) {
        if (service_mask &
            (tBTA_SERVICE_MASK)(BTA_SERVICE_ID_TO_SERVICE_MASK(i))) {
          btif_in_execute_service_request(i, false);
        }
      }
      bluetooth::bqr::EnableBtQualityReport(false);
      btif_disable_bluetooth_evt();
      break;

    case BTA_DM_PIN_REQ_EVT:
      btif_dm_pin_req_evt(&p_data->pin_req);
      break;

    case BTA_DM_AUTH_CMPL_EVT:
      btif_dm_auth_cmpl_evt(&p_data->auth_cmpl);
      break;

    case BTA_DM_BOND_CANCEL_CMPL_EVT:
      if (is_bonding_or_sdp()) {
        bd_addr = pairing_cb.bd_addr;
        btm_set_bond_type_dev(pairing_cb.bd_addr, BOND_TYPE_UNKNOWN);
        bond_state_changed((bt_status_t)p_data->bond_cancel_cmpl.result,
                           bd_addr, BT_BOND_STATE_NONE);
      }
      break;

    case BTA_DM_SP_CFM_REQ_EVT:
      btif_dm_ssp_cfm_req_evt(&p_data->cfm_req);
      break;
    case BTA_DM_SP_KEY_NOTIF_EVT:
      btif_dm_ssp_key_notif_evt(&p_data->key_notif);
      break;

    case BTA_DM_DEV_UNPAIRED_EVT:
      bd_addr = p_data->link_down.bd_addr;
      btm_set_bond_type_dev(p_data->link_down.bd_addr, BOND_TYPE_UNKNOWN);

/*special handling for HID devices */
#if (defined(BTA_HH_INCLUDED) && (BTA_HH_INCLUDED == TRUE))
      btif_hh_remove_device(bd_addr);
#endif
#if (defined(BTA_HD_INCLUDED) && (BTA_HD_INCLUDED == TRUE))
      btif_hd_remove_device(bd_addr);
#endif
      btif_hearing_aid_get_interface()->RemoveDevice(bd_addr);
      btif_storage_remove_bonded_device(&bd_addr);
      bond_state_changed(BT_STATUS_SUCCESS, bd_addr, BT_BOND_STATE_NONE);
      break;

    case BTA_DM_BUSY_LEVEL_EVT: {
      if (p_data->busy_level.level_flags & BTM_BL_INQUIRY_PAGING_MASK) {
        if (p_data->busy_level.level_flags == BTM_BL_INQUIRY_STARTED) {
          HAL_CBACK(bt_hal_cbacks, discovery_state_changed_cb,
                    BT_DISCOVERY_STARTED);
          btif_dm_inquiry_in_progress = true;
        } else if (p_data->busy_level.level_flags == BTM_BL_INQUIRY_CANCELLED) {
          HAL_CBACK(bt_hal_cbacks, discovery_state_changed_cb,
                    BT_DISCOVERY_STOPPED);
          btif_dm_inquiry_in_progress = false;
        } else if (p_data->busy_level.level_flags == BTM_BL_INQUIRY_COMPLETE) {
          btif_dm_inquiry_in_progress = false;
        }
      }
    } break;

    case BTA_DM_LINK_UP_EVT:
      bd_addr = p_data->link_up.bd_addr;
      BTIF_TRACE_DEBUG("BTA_DM_LINK_UP_EVT. Sending BT_ACL_STATE_CONNECTED");

      btif_update_remote_version_property(&bd_addr);

      HAL_CBACK(bt_hal_cbacks, acl_state_changed_cb, BT_STATUS_SUCCESS,
                &bd_addr, BT_ACL_STATE_CONNECTED);
      break;

    case BTA_DM_LINK_DOWN_EVT:
      bd_addr = p_data->link_down.bd_addr;
      btm_set_bond_type_dev(p_data->link_down.bd_addr, BOND_TYPE_UNKNOWN);
      btif_av_acl_disconnected(bd_addr);
      BTIF_TRACE_DEBUG(
          "BTA_DM_LINK_DOWN_EVT. Sending BT_ACL_STATE_DISCONNECTED");
      HAL_CBACK(bt_hal_cbacks, acl_state_changed_cb, BT_STATUS_SUCCESS,
                &bd_addr, BT_ACL_STATE_DISCONNECTED);
      break;

    case BTA_DM_HW_ERROR_EVT:
      BTIF_TRACE_ERROR("Received H/W Error. ");
      /* Flush storage data */
      btif_config_flush();
      usleep(100000); /* 100milliseconds */
      /* Killing the process to force a restart as part of fault tolerance */
      kill(getpid(), SIGKILL);
      break;

    case BTA_DM_BLE_KEY_EVT:
      BTIF_TRACE_DEBUG("BTA_DM_BLE_KEY_EVT key_type=0x%02x ",
                       p_data->ble_key.key_type);

      /* If this pairing is by-product of local initiated GATT client Read or
      Write,
      BTA would not have sent BTA_DM_BLE_SEC_REQ_EVT event and Bond state would
      not
      have setup properly. Setup pairing_cb and notify App about Bonding state
      now*/
      if (pairing_cb.state != BT_BOND_STATE_BONDING) {
        BTIF_TRACE_DEBUG(
            "Bond state not sent to App so far.Notify the app now");
        bond_state_changed(BT_STATUS_SUCCESS, p_data->ble_key.bd_addr,
                           BT_BOND_STATE_BONDING);
      } else if (pairing_cb.bd_addr != p_data->ble_key.bd_addr) {
        BTIF_TRACE_ERROR("BD mismatch discard BLE key_type=%d ",
                         p_data->ble_key.key_type);
        break;
      }

      switch (p_data->ble_key.key_type) {
        case BTA_LE_KEY_PENC:
          BTIF_TRACE_DEBUG("Rcv BTA_LE_KEY_PENC");
          pairing_cb.ble.is_penc_key_rcvd = true;
          pairing_cb.ble.penc_key = p_data->ble_key.p_key_value->penc_key;
          break;

        case BTA_LE_KEY_PID:
          BTIF_TRACE_DEBUG("Rcv BTA_LE_KEY_PID");
          pairing_cb.ble.is_pid_key_rcvd = true;
          pairing_cb.ble.pid_key = p_data->ble_key.p_key_value->pid_key;
          break;

        case BTA_LE_KEY_PCSRK:
          BTIF_TRACE_DEBUG("Rcv BTA_LE_KEY_PCSRK");
          pairing_cb.ble.is_pcsrk_key_rcvd = true;
          pairing_cb.ble.pcsrk_key = p_data->ble_key.p_key_value->pcsrk_key;
          break;

        case BTA_LE_KEY_LENC:
          BTIF_TRACE_DEBUG("Rcv BTA_LE_KEY_LENC");
          pairing_cb.ble.is_lenc_key_rcvd = true;
          pairing_cb.ble.lenc_key = p_data->ble_key.p_key_value->lenc_key;
          break;

        case BTA_LE_KEY_LCSRK:
          BTIF_TRACE_DEBUG("Rcv BTA_LE_KEY_LCSRK");
          pairing_cb.ble.is_lcsrk_key_rcvd = true;
          pairing_cb.ble.lcsrk_key = p_data->ble_key.p_key_value->lcsrk_key;
          break;

        case BTA_LE_KEY_LID:
          BTIF_TRACE_DEBUG("Rcv BTA_LE_KEY_LID");
          pairing_cb.ble.is_lidk_key_rcvd = true;
          break;

        default:
          BTIF_TRACE_ERROR("unknown BLE key type (0x%02x)",
                           p_data->ble_key.key_type);
          break;
      }
      break;
    case BTA_DM_BLE_SEC_REQ_EVT:
      BTIF_TRACE_DEBUG("BTA_DM_BLE_SEC_REQ_EVT. ");
      btif_dm_ble_sec_req_evt(&p_data->ble_req);
      break;
    case BTA_DM_BLE_PASSKEY_NOTIF_EVT:
      BTIF_TRACE_DEBUG("BTA_DM_BLE_PASSKEY_NOTIF_EVT. ");
      btif_dm_ble_key_notif_evt(&p_data->key_notif);
      break;
    case BTA_DM_BLE_PASSKEY_REQ_EVT:
      BTIF_TRACE_DEBUG("BTA_DM_BLE_PASSKEY_REQ_EVT. ");
      btif_dm_ble_passkey_req_evt(&p_data->pin_req);
      break;
    case BTA_DM_BLE_NC_REQ_EVT:
      BTIF_TRACE_DEBUG("BTA_DM_BLE_PASSKEY_REQ_EVT. ");
      btif_dm_ble_key_nc_req_evt(&p_data->key_notif);
      break;
    case BTA_DM_BLE_OOB_REQ_EVT:
      BTIF_TRACE_DEBUG("BTA_DM_BLE_OOB_REQ_EVT. ");
      btif_dm_ble_oob_req_evt(&p_data->rmt_oob);
      break;
    case BTA_DM_BLE_SC_OOB_REQ_EVT:
      BTIF_TRACE_DEBUG("BTA_DM_BLE_SC_OOB_REQ_EVT. ");
      btif_dm_ble_sc_oob_req_evt(&p_data->rmt_oob);
      break;
    case BTA_DM_BLE_LOCAL_IR_EVT:
      BTIF_TRACE_DEBUG("BTA_DM_BLE_LOCAL_IR_EVT. ");
      ble_local_key_cb.is_id_keys_rcvd = true;
      ble_local_key_cb.id_keys.irk = p_data->ble_id_keys.irk;
      ble_local_key_cb.id_keys.ir = p_data->ble_id_keys.ir;
      ble_local_key_cb.id_keys.dhk = p_data->ble_id_keys.dhk;
      btif_storage_add_ble_local_key(ble_local_key_cb.id_keys.irk,
                                     BTIF_DM_LE_LOCAL_KEY_IRK);
      btif_storage_add_ble_local_key(ble_local_key_cb.id_keys.ir,
                                     BTIF_DM_LE_LOCAL_KEY_IR);
      btif_storage_add_ble_local_key(ble_local_key_cb.id_keys.dhk,
                                     BTIF_DM_LE_LOCAL_KEY_DHK);
      break;
    case BTA_DM_BLE_LOCAL_ER_EVT:
      BTIF_TRACE_DEBUG("BTA_DM_BLE_LOCAL_ER_EVT. ");
      ble_local_key_cb.is_er_rcvd = true;
      ble_local_key_cb.er = p_data->ble_er;
      btif_storage_add_ble_local_key(ble_local_key_cb.er,
                                     BTIF_DM_LE_LOCAL_KEY_ER);
      break;

    case BTA_DM_BLE_AUTH_CMPL_EVT:
      BTIF_TRACE_DEBUG("BTA_DM_BLE_AUTH_CMPL_EVT. ");
      btif_dm_ble_auth_cmpl_evt(&p_data->auth_cmpl);
      break;

    case BTA_DM_LE_FEATURES_READ: {
      tBTM_BLE_VSC_CB cmn_vsc_cb;
      bt_local_le_features_t local_le_features;
      char buf[512];
      bt_property_t prop;
      prop.type = BT_PROPERTY_LOCAL_LE_FEATURES;
      prop.val = (void*)buf;
      prop.len = sizeof(buf);

      /* LE features are not stored in storage. Should be retrived from stack */
      BTM_BleGetVendorCapabilities(&cmn_vsc_cb);
      local_le_features.local_privacy_enabled = BTM_BleLocalPrivacyEnabled();

      prop.len = sizeof(bt_local_le_features_t);
      if (cmn_vsc_cb.filter_support == 1)
        local_le_features.max_adv_filter_supported = cmn_vsc_cb.max_filter;
      else
        local_le_features.max_adv_filter_supported = 0;
      local_le_features.max_adv_instance = cmn_vsc_cb.adv_inst_max;
      local_le_features.max_irk_list_size = cmn_vsc_cb.max_irk_list_sz;
      local_le_features.rpa_offload_supported = cmn_vsc_cb.rpa_offloading;
      local_le_features.activity_energy_info_supported =
          cmn_vsc_cb.energy_support;
      local_le_features.scan_result_storage_size =
          cmn_vsc_cb.tot_scan_results_strg;
      local_le_features.version_supported = cmn_vsc_cb.version_supported;
      local_le_features.total_trackable_advertisers =
          cmn_vsc_cb.total_trackable_advertisers;

      local_le_features.extended_scan_support =
          cmn_vsc_cb.extended_scan_support > 0;
      local_le_features.debug_logging_supported =
          cmn_vsc_cb.debug_logging_supported > 0;

      const controller_t* controller = controller_get_interface();

      local_le_features.le_2m_phy_supported = controller->supports_ble_2m_phy();
      local_le_features.le_coded_phy_supported =
          controller->supports_ble_coded_phy();
      local_le_features.le_extended_advertising_supported =
          controller->supports_ble_extended_advertising();
      local_le_features.le_periodic_advertising_supported =
          controller->supports_ble_periodic_advertising();
      local_le_features.le_maximum_advertising_data_length =
          controller->get_ble_maxium_advertising_data_length();

      memcpy(prop.val, &local_le_features, prop.len);
      HAL_CBACK(bt_hal_cbacks, adapter_properties_cb, BT_STATUS_SUCCESS, 1,
                &prop);
      break;
    }

    case BTA_DM_ENER_INFO_READ: {
      btif_activity_energy_info_cb_t* p_ener_data =
          (btif_activity_energy_info_cb_t*)p_param;
      bt_activity_energy_info energy_info;
      energy_info.status = p_ener_data->status;
      energy_info.ctrl_state = p_ener_data->ctrl_state;
      energy_info.rx_time = p_ener_data->rx_time;
      energy_info.tx_time = p_ener_data->tx_time;
      energy_info.idle_time = p_ener_data->idle_time;
      energy_info.energy_used = p_ener_data->energy_used;

      bt_uid_traffic_t* data = uid_set_read_and_clear(uid_set);
      HAL_CBACK(bt_hal_cbacks, energy_info_cb, &energy_info, data);
      osi_free(data);
      break;
    }

    case BTA_DM_AUTHORIZE_EVT:
    case BTA_DM_SIG_STRENGTH_EVT:
    case BTA_DM_SP_RMT_OOB_EVT:
    case BTA_DM_SP_KEYPRESS_EVT:
    case BTA_DM_ROLE_CHG_EVT:

    default:
      BTIF_TRACE_WARNING("%s: unhandled event (%d)", __func__, event);
      break;
  }

  btif_dm_data_free(event, p_data);
}

/*******************************************************************************
 *
 * Function         btif_dm_generic_evt
 *
 * Description      Executes non-BTA upstream events in BTIF context
 *
 * Returns          void
 *
 ******************************************************************************/
static void btif_dm_generic_evt(uint16_t event, char* p_param) {
  BTIF_TRACE_EVENT("%s: event=%d", __func__, event);
  switch (event) {
    case BTIF_DM_CB_DISCOVERY_STARTED: {
      HAL_CBACK(bt_hal_cbacks, discovery_state_changed_cb,
                BT_DISCOVERY_STARTED);
    } break;

    case BTIF_DM_CB_CREATE_BOND: {
      pairing_cb.timeout_retries = NUM_TIMEOUT_RETRIES;
      btif_dm_create_bond_cb_t* create_bond_cb =
          (btif_dm_create_bond_cb_t*)p_param;
      btif_dm_cb_create_bond(create_bond_cb->bdaddr, create_bond_cb->transport);
    } break;

    case BTIF_DM_CB_REMOVE_BOND: {
      btif_dm_cb_remove_bond((RawAddress*)p_param);
    } break;

    case BTIF_DM_CB_HID_REMOTE_NAME: {
      btif_dm_cb_hid_remote_name((tBTM_REMOTE_DEV_NAME*)p_param);
    } break;

    case BTIF_DM_CB_BOND_STATE_BONDING: {
      bond_state_changed(BT_STATUS_SUCCESS, *((RawAddress*)p_param),
                         BT_BOND_STATE_BONDING);
    } break;
    case BTIF_DM_CB_LE_TX_TEST:
    case BTIF_DM_CB_LE_RX_TEST: {
      uint8_t status;
      STREAM_TO_UINT8(status, p_param);
      HAL_CBACK(bt_hal_cbacks, le_test_mode_cb,
                (status == 0) ? BT_STATUS_SUCCESS : BT_STATUS_FAIL, 0);
    } break;
    case BTIF_DM_CB_LE_TEST_END: {
      uint8_t status;
      uint16_t count = 0;
      STREAM_TO_UINT8(status, p_param);
      if (status == 0) STREAM_TO_UINT16(count, p_param);
      HAL_CBACK(bt_hal_cbacks, le_test_mode_cb,
                (status == 0) ? BT_STATUS_SUCCESS : BT_STATUS_FAIL, count);
    } break;
    default: {
      BTIF_TRACE_WARNING("%s : Unknown event 0x%x", __func__, event);
    } break;
  }
}

/*******************************************************************************
 *
 * Function         bte_dm_evt
 *
 * Description      Switches context from BTE to BTIF for all DM events
 *
 * Returns          void
 *
 ******************************************************************************/

void bte_dm_evt(tBTA_DM_SEC_EVT event, tBTA_DM_SEC* p_data) {
  /* switch context to btif task context (copy full union size for convenience)
   */
  bt_status_t status = btif_transfer_context(
      btif_dm_upstreams_evt, (uint16_t)event, (char*)p_data,
      sizeof(tBTA_DM_SEC), btif_dm_data_copy);

  /* catch any failed context transfers */
  ASSERTC(status == BT_STATUS_SUCCESS, "context transfer failed", status);
}

/*******************************************************************************
 *
 * Function         bte_search_devices_evt
 *
 * Description      Switches context from BTE to BTIF for DM search events
 *
 * Returns          void
 *
 ******************************************************************************/
static void bte_search_devices_evt(tBTA_DM_SEARCH_EVT event,
                                   tBTA_DM_SEARCH* p_data) {
  uint16_t param_len = 0;

  if (p_data) param_len += sizeof(tBTA_DM_SEARCH);
  /* Allocate buffer to hold the pointers (deep copy). The pointers will point
   * to the end of the tBTA_DM_SEARCH */
  switch (event) {
    case BTA_DM_INQ_RES_EVT: {
      if (p_data->inq_res.p_eir) param_len += p_data->inq_res.eir_len;
    } break;

    case BTA_DM_DISC_RES_EVT: {
      if (p_data->disc_res.raw_data_size && p_data->disc_res.p_raw_data)
        param_len += p_data->disc_res.raw_data_size;
    } break;
  }
  BTIF_TRACE_DEBUG("%s event=%s param_len=%d", __func__,
                   dump_dm_search_event(event), param_len);

  /* if remote name is available in EIR, set teh flag so that stack doesnt
   * trigger RNR */
  if (event == BTA_DM_INQ_RES_EVT)
    p_data->inq_res.remt_name_not_required =
        check_eir_remote_name(p_data, NULL, NULL);

  btif_transfer_context(
      btif_dm_search_devices_evt, (uint16_t)event, (char*)p_data, param_len,
      (param_len > sizeof(tBTA_DM_SEARCH)) ? search_devices_copy_cb : NULL);
}

/*******************************************************************************
 *
 * Function         bte_dm_search_services_evt
 *
 * Description      Switches context from BTE to BTIF for DM search services
 *                  event
 *
 * Returns          void
 *
 ******************************************************************************/
static void bte_dm_search_services_evt(tBTA_DM_SEARCH_EVT event,
                                       tBTA_DM_SEARCH* p_data) {
  uint16_t param_len = 0;
  if (p_data) param_len += sizeof(tBTA_DM_SEARCH);
  switch (event) {
    case BTA_DM_DISC_RES_EVT: {
      if ((p_data->disc_res.result == BTA_SUCCESS) &&
          (p_data->disc_res.num_uuids > 0)) {
        param_len += (p_data->disc_res.num_uuids * Uuid::kNumBytes128);
      }
    } break;
  }
  /* TODO: The only other member that needs a deep copy is the p_raw_data. But
   * not sure
   * if raw_data is needed. */
  btif_transfer_context(
      btif_dm_search_services_evt, event, (char*)p_data, param_len,
      (param_len > sizeof(tBTA_DM_SEARCH)) ? search_services_copy_cb : NULL);
}

/*******************************************************************************
 *
 * Function         bte_dm_remote_service_record_evt
 *
 * Description      Switches context from BTE to BTIF for DM search service
 *                  record event
 *
 * Returns          void
 *
 ******************************************************************************/
static void bte_dm_remote_service_record_evt(tBTA_DM_SEARCH_EVT event,
                                             tBTA_DM_SEARCH* p_data) {
  /* TODO: The only member that needs a deep copy is the p_raw_data. But not
   * sure yet if this is needed. */
  btif_transfer_context(btif_dm_remote_service_record_evt, event, (char*)p_data,
                        sizeof(tBTA_DM_SEARCH), NULL);
}

/*******************************************************************************
 *
 * Function         bta_energy_info_cb
 *
 * Description      Switches context from BTE to BTIF for DM energy info event
 *
 * Returns          void
 *
 ******************************************************************************/
static void bta_energy_info_cb(tBTA_DM_BLE_TX_TIME_MS tx_time,
                               tBTA_DM_BLE_RX_TIME_MS rx_time,
                               tBTA_DM_BLE_IDLE_TIME_MS idle_time,
                               tBTA_DM_BLE_ENERGY_USED energy_used,
                               tBTA_DM_CONTRL_STATE ctrl_state,
                               tBTA_STATUS status) {
  BTIF_TRACE_DEBUG(
      "energy_info_cb-Status:%d,state=%d,tx_t=%ld, rx_t=%ld, "
      "idle_time=%ld,used=%ld",
      status, ctrl_state, tx_time, rx_time, idle_time, energy_used);

  btif_activity_energy_info_cb_t btif_cb;
  btif_cb.status = status;
  btif_cb.ctrl_state = ctrl_state;
  btif_cb.tx_time = (uint64_t)tx_time;
  btif_cb.rx_time = (uint64_t)rx_time;
  btif_cb.idle_time = (uint64_t)idle_time;
  btif_cb.energy_used = (uint64_t)energy_used;
  btif_transfer_context(btif_dm_upstreams_evt, BTA_DM_ENER_INFO_READ,
                        (char*)&btif_cb, sizeof(btif_activity_energy_info_cb_t),
                        NULL);
}

/* Scan filter param config event */
static void bte_scan_filt_param_cfg_evt(uint8_t ref_value, uint8_t avbl_space,
                                        uint8_t action_type, uint8_t status) {
  /* This event occurs on calling BTA_DmBleCfgFilterCondition internally,
  ** and that is why there is no HAL callback
  */
  if (BTA_SUCCESS != status) {
    BTIF_TRACE_ERROR("%s, %d", __func__, status);
  } else {
    BTIF_TRACE_DEBUG("%s", __func__);
  }
}

/*****************************************************************************
 *
 *   btif api functions (no context switch)
 *
 ****************************************************************************/

/*******************************************************************************
 *
 * Function         btif_dm_start_discovery
 *
 * Description      Start device discovery/inquiry
 *
 * Returns          bt_status_t
 *
 ******************************************************************************/
bt_status_t btif_dm_start_discovery(void) {
  tBTA_DM_INQ inq_params;
  tBTA_SERVICE_MASK services = 0;

  BTIF_TRACE_EVENT("%s", __func__);

  /* Cleanup anything remaining on index 0 */
  do_in_main_thread(
      FROM_HERE,
      base::Bind(&BTM_BleAdvFilterParamSetup, BTM_BLE_SCAN_COND_DELETE, 0,
                 nullptr, base::Bind(&bte_scan_filt_param_cfg_evt, 0)));

  auto adv_filt_param = std::make_unique<btgatt_filt_param_setup_t>();
  /* Add an allow-all filter on index 0*/
  adv_filt_param->dely_mode = IMMEDIATE_DELY_MODE;
  adv_filt_param->feat_seln = ALLOW_ALL_FILTER;
  adv_filt_param->filt_logic_type = BTA_DM_BLE_PF_FILT_LOGIC_OR;
  adv_filt_param->list_logic_type = BTA_DM_BLE_PF_LIST_LOGIC_OR;
  adv_filt_param->rssi_low_thres = LOWEST_RSSI_VALUE;
  adv_filt_param->rssi_high_thres = LOWEST_RSSI_VALUE;
  do_in_main_thread(
      FROM_HERE, base::Bind(&BTM_BleAdvFilterParamSetup, BTM_BLE_SCAN_COND_ADD,
                            0, base::Passed(&adv_filt_param),
                            base::Bind(&bte_scan_filt_param_cfg_evt, 0)));

  /* TODO: Do we need to handle multiple inquiries at the same time? */

  /* Set inquiry params and call API */
  inq_params.mode = BTA_DM_GENERAL_INQUIRY | BTA_BLE_GENERAL_INQUIRY;
  inq_params.duration = BTIF_DM_DEFAULT_INQ_MAX_DURATION;

  inq_params.max_resps = BTIF_DM_DEFAULT_INQ_MAX_RESULTS;
  inq_params.report_dup = true;

  inq_params.filter_type = BTA_DM_INQ_CLR;
  /* TODO: Filter device by BDA needs to be implemented here */

  /* Will be enabled to true once inquiry busy level has been received */
  btif_dm_inquiry_in_progress = false;
  /* find nearby devices */
  BTA_DmSearch(&inq_params, services, bte_search_devices_evt);

  return BT_STATUS_SUCCESS;
}

/*******************************************************************************
 *
 * Function         btif_dm_cancel_discovery
 *
 * Description      Cancels search
 *
 * Returns          bt_status_t
 *
 ******************************************************************************/
bt_status_t btif_dm_cancel_discovery(void) {
  BTIF_TRACE_EVENT("%s", __func__);
  BTA_DmSearchCancel();
  return BT_STATUS_SUCCESS;
}

/*******************************************************************************
 *
 * Function         btif_dm_create_bond
 *
 * Description      Initiate bonding with the specified device
 *
 * Returns          bt_status_t
 *
 ******************************************************************************/
bt_status_t btif_dm_create_bond(const RawAddress* bd_addr, int transport) {
  btif_dm_create_bond_cb_t create_bond_cb;
  create_bond_cb.transport = transport;
  create_bond_cb.bdaddr = *bd_addr;

  BTIF_TRACE_EVENT("%s: bd_addr=%s, transport=%d", __func__,
                   bd_addr->ToString().c_str(), transport);
  if (pairing_cb.state != BT_BOND_STATE_NONE) return BT_STATUS_BUSY;

  btif_stats_add_bond_event(*bd_addr, BTIF_DM_FUNC_CREATE_BOND,
                            pairing_cb.state);

  btif_transfer_context(btif_dm_generic_evt, BTIF_DM_CB_CREATE_BOND,
                        (char*)&create_bond_cb,
                        sizeof(btif_dm_create_bond_cb_t), NULL);

  return BT_STATUS_SUCCESS;
}

/*******************************************************************************
 *
 * Function         btif_dm_create_bond_out_of_band
 *
 * Description      Initiate bonding with the specified device using out of band
 *                  data
 *
 * Returns          bt_status_t
 *
 ******************************************************************************/
bt_status_t btif_dm_create_bond_out_of_band(
    const RawAddress* bd_addr, int transport,
    const bt_out_of_band_data_t* oob_data) {
  oob_cb.bdaddr = *bd_addr;
  memcpy(&oob_cb.oob_data, oob_data, sizeof(bt_out_of_band_data_t));

  uint8_t empty[] = {0, 0, 0, 0, 0, 0, 0};
  // If LE Bluetooth Device Address is provided, use provided address type
  // value.
  if (memcmp(oob_data->le_bt_dev_addr, empty, 7) != 0) {
    /* byte no 7 is address type in LE Bluetooth Address OOB data */
    uint8_t address_type = oob_data->le_bt_dev_addr[6];
    if (address_type == BLE_ADDR_PUBLIC || address_type == BLE_ADDR_RANDOM) {
      // bd_addr->address is already reversed, so use it instead of
      // oob_data->le_bt_dev_addr
      BTM_SecAddBleDevice(*bd_addr, NULL, BT_DEVICE_TYPE_BLE, address_type);
    }
  }

  BTIF_TRACE_EVENT("%s: bd_addr=%s, transport=%d", __func__,
                   bd_addr->ToString().c_str(), transport);
  return btif_dm_create_bond(bd_addr, transport);
}

/*******************************************************************************
 *
 * Function         btif_dm_cancel_bond
 *
 * Description      Initiate bonding with the specified device
 *
 * Returns          bt_status_t
 *
 ******************************************************************************/

bt_status_t btif_dm_cancel_bond(const RawAddress* bd_addr) {
  BTIF_TRACE_EVENT("%s: bd_addr=%s", __func__, bd_addr->ToString().c_str());

  btif_stats_add_bond_event(*bd_addr, BTIF_DM_FUNC_CANCEL_BOND,
                            pairing_cb.state);

  /* TODO:
  **  1. Restore scan modes
  **  2. special handling for HID devices
  */
  if (is_bonding_or_sdp()) {
    if (pairing_cb.is_ssp) {
      if (pairing_cb.is_le_only) {
        BTA_DmBleSecurityGrant(*bd_addr, BTA_DM_SEC_PAIR_NOT_SPT);
      } else {
        BTA_DmConfirm(*bd_addr, false);
        BTA_DmBondCancel(*bd_addr);
        btif_storage_remove_bonded_device(bd_addr);
      }
    } else {
      if (pairing_cb.is_le_only) {
        BTA_DmBondCancel(*bd_addr);
      } else {
        BTA_DmPinReply(*bd_addr, false, 0, NULL);
      }
      /* Cancel bonding, in case it is in ACL connection setup state */
      BTA_DmBondCancel(*bd_addr);
    }
  }

  return BT_STATUS_SUCCESS;
}

/*******************************************************************************
 *
 * Function         btif_dm_hh_open_failed
 *
 * Description      informs the upper layers if the HH have failed during
 *                  bonding
 *
 * Returns          none
 *
 ******************************************************************************/

void btif_dm_hh_open_failed(RawAddress* bdaddr) {
  if (pairing_cb.state == BT_BOND_STATE_BONDING &&
      *bdaddr == pairing_cb.bd_addr) {
    bond_state_changed(BT_STATUS_FAIL, *bdaddr, BT_BOND_STATE_NONE);
  }
}

/*******************************************************************************
 *
 * Function         btif_dm_remove_bond
 *
 * Description      Removes bonding with the specified device
 *
 * Returns          bt_status_t
 *
 ******************************************************************************/

bt_status_t btif_dm_remove_bond(const RawAddress* bd_addr) {
  BTIF_TRACE_EVENT("%s: bd_addr=%s", __func__, bd_addr->ToString().c_str());

  btif_stats_add_bond_event(*bd_addr, BTIF_DM_FUNC_REMOVE_BOND,
                            pairing_cb.state);

  btif_transfer_context(btif_dm_generic_evt, BTIF_DM_CB_REMOVE_BOND,
                        (char*)bd_addr, sizeof(RawAddress), NULL);

  return BT_STATUS_SUCCESS;
}

/*******************************************************************************
 *
 * Function         btif_dm_pin_reply
 *
 * Description      BT legacy pairing - PIN code reply
 *
 * Returns          bt_status_t
 *
 ******************************************************************************/

bt_status_t btif_dm_pin_reply(const RawAddress* bd_addr, uint8_t accept,
                              uint8_t pin_len, bt_pin_code_t* pin_code) {
  BTIF_TRACE_EVENT("%s: accept=%d", __func__, accept);
  if (pin_code == NULL || pin_len > PIN_CODE_LEN) return BT_STATUS_FAIL;
  if (pairing_cb.is_le_only) {
    int i;
    uint32_t passkey = 0;
    int multi[] = {100000, 10000, 1000, 100, 10, 1};
    for (i = 0; i < 6; i++) {
      passkey += (multi[i] * (pin_code->pin[i] - '0'));
    }
    BTIF_TRACE_DEBUG("btif_dm_pin_reply: passkey: %d", passkey);
    BTA_DmBlePasskeyReply(*bd_addr, accept, passkey);

  } else {
    BTA_DmPinReply(*bd_addr, accept, pin_len, pin_code->pin);
    if (accept) pairing_cb.pin_code_len = pin_len;
  }
  return BT_STATUS_SUCCESS;
}

/*******************************************************************************
 *
 * Function         btif_dm_ssp_reply
 *
 * Description      BT SSP Reply - Just Works, Numeric Comparison & Passkey
 *                  Entry
 *
 * Returns          bt_status_t
 *
 ******************************************************************************/
bt_status_t btif_dm_ssp_reply(const RawAddress* bd_addr,
                              bt_ssp_variant_t variant, uint8_t accept,
                              UNUSED_ATTR uint32_t passkey) {
  if (variant == BT_SSP_VARIANT_PASSKEY_ENTRY) {
    /* This is not implemented in the stack.
     * For devices with display, this is not needed
    */
    BTIF_TRACE_WARNING("%s: Not implemented", __func__);
    return BT_STATUS_FAIL;
  }
  /* BT_SSP_VARIANT_CONSENT & BT_SSP_VARIANT_PASSKEY_CONFIRMATION supported */
  BTIF_TRACE_EVENT("%s: accept=%d", __func__, accept);
  if (pairing_cb.is_le_only) {
    if (pairing_cb.is_le_nc) {
      BTA_DmBleConfirmReply(*bd_addr, accept);
    } else {
      if (accept)
        BTA_DmBleSecurityGrant(*bd_addr, BTA_DM_SEC_GRANTED);
      else
        BTA_DmBleSecurityGrant(*bd_addr, BTA_DM_SEC_PAIR_NOT_SPT);
    }
  } else {
    BTA_DmConfirm(*bd_addr, accept);
  }
  return BT_STATUS_SUCCESS;
}

/*******************************************************************************
 *
 * Function         btif_dm_get_adapter_property
 *
 * Description     Queries the BTA for the adapter property
 *
 * Returns          bt_status_t
 *
 ******************************************************************************/
bt_status_t btif_dm_get_adapter_property(bt_property_t* prop) {
  BTIF_TRACE_EVENT("%s: type=0x%x", __func__, prop->type);
  switch (prop->type) {
    case BT_PROPERTY_BDNAME: {
      bt_bdname_t* bd_name = (bt_bdname_t*)prop->val;
      strncpy((char*)bd_name->name, (char*)btif_get_default_local_name(),
              sizeof(bd_name->name) - 1);
      bd_name->name[sizeof(bd_name->name) - 1] = 0;
      prop->len = strlen((char*)bd_name->name);
    } break;

    case BT_PROPERTY_ADAPTER_SCAN_MODE: {
      /* if the storage does not have it. Most likely app never set it. Default
       * is NONE */
      bt_scan_mode_t* mode = (bt_scan_mode_t*)prop->val;
      *mode = BT_SCAN_MODE_NONE;
      prop->len = sizeof(bt_scan_mode_t);
    } break;

    case BT_PROPERTY_ADAPTER_DISCOVERY_TIMEOUT: {
      uint32_t* tmt = (uint32_t*)prop->val;
      *tmt = 120; /* default to 120s, if not found in NV */
      prop->len = sizeof(uint32_t);
    } break;

    case BT_PROPERTY_CLASS_OF_DEVICE: {
      DEV_CLASS dev_class = BTA_DM_COD;
      memcpy(prop->val, dev_class, sizeof(DEV_CLASS));
      prop->len = sizeof(DEV_CLASS);
    } break;

    // While fetching IO_CAP* values for the local device, we maintain backward
    // compatibility by using the value from #define macros BTM_LOCAL_IO_CAPS,
    // BTM_LOCAL_IO_CAPS_BLE if the values have never been explicitly set.

    case BT_PROPERTY_LOCAL_IO_CAPS: {
      *(bt_io_cap_t*)prop->val = (bt_io_cap_t)BTM_LOCAL_IO_CAPS;
      prop->len = sizeof(bt_io_cap_t);
    } break;

    case BT_PROPERTY_LOCAL_IO_CAPS_BLE: {
      *(bt_io_cap_t*)prop->val = (bt_io_cap_t)BTM_LOCAL_IO_CAPS_BLE;
      prop->len = sizeof(bt_io_cap_t);
    } break;

    default:
      prop->len = 0;
      return BT_STATUS_FAIL;
  }
  return BT_STATUS_SUCCESS;
}

/*******************************************************************************
 *
 * Function         btif_dm_get_remote_services
 *
 * Description      Start SDP to get remote services
 *
 * Returns          bt_status_t
 *
 ******************************************************************************/
bt_status_t btif_dm_get_remote_services(const RawAddress& remote_addr) {
  BTIF_TRACE_EVENT("%s: bd_addr=%s", __func__, remote_addr.ToString().c_str());

  BTA_DmDiscover(remote_addr, BTA_ALL_SERVICE_MASK, bte_dm_search_services_evt,
                 true);

  return BT_STATUS_SUCCESS;
}

/*******************************************************************************
 *
 * Function         btif_dm_get_remote_services_by_transport
 *
 * Description      Start SDP to get remote services by transport
 *
 * Returns          bt_status_t
 *
 ******************************************************************************/
bt_status_t btif_dm_get_remote_services_by_transport(RawAddress* remote_addr,
                                                     const int transport) {
  BTIF_TRACE_EVENT("%s: transport=%d, remote_addr=%s", __func__, transport,
                   remote_addr->ToString().c_str());

  /* Set the mask extension */
  tBTA_SERVICE_MASK_EXT mask_ext;
  mask_ext.num_uuid = 0;
  mask_ext.p_uuid = NULL;
  mask_ext.srvc_mask = BTA_ALL_SERVICE_MASK;

  BTA_DmDiscoverByTransport(*remote_addr, &mask_ext, bte_dm_search_services_evt,
                            true, transport);

  return BT_STATUS_SUCCESS;
}

/*******************************************************************************
 *
 * Function         btif_dm_get_remote_service_record
 *
 * Description      Start SDP to get remote service record
 *
 *
 * Returns          bt_status_t
 ******************************************************************************/
bt_status_t btif_dm_get_remote_service_record(const RawAddress& remote_addr,
                                              const Uuid& uuid) {
  BTIF_TRACE_EVENT("%s: bd_addr=%s", __func__, remote_addr.ToString().c_str());
  BTA_DmDiscoverUUID(remote_addr, uuid, bte_dm_remote_service_record_evt, true);

  return BT_STATUS_SUCCESS;
}

void btif_dm_execute_service_request(uint16_t event, char* p_param) {
  bool b_enable = false;
  bt_status_t status;
  if (event == BTIF_DM_ENABLE_SERVICE) {
    b_enable = true;
  }
  status =
      btif_in_execute_service_request(*((tBTA_SERVICE_ID*)p_param), b_enable);
  if (status == BT_STATUS_SUCCESS) {
    bt_property_t property;
    Uuid local_uuids[BT_MAX_NUM_UUIDS];

    /* Now send the UUID_PROPERTY_CHANGED event to the upper layer */
    BTIF_STORAGE_FILL_PROPERTY(&property, BT_PROPERTY_UUIDS,
                               sizeof(local_uuids), local_uuids);
    btif_storage_get_adapter_property(&property);
    HAL_CBACK(bt_hal_cbacks, adapter_properties_cb, BT_STATUS_SUCCESS, 1,
              &property);
  }
  return;
}

void btif_dm_proc_io_req(UNUSED_ATTR const RawAddress& bd_addr,
                         UNUSED_ATTR tBTA_IO_CAP* p_io_cap,
                         UNUSED_ATTR tBTA_OOB_DATA* p_oob_data,
                         tBTA_AUTH_REQ* p_auth_req, bool is_orig) {
  uint8_t yes_no_bit = BTA_AUTH_SP_YES & *p_auth_req;
  /* if local initiated:
  **      1. set DD + MITM
  ** if remote initiated:
  **      1. Copy over the auth_req from peer's io_rsp
  **      2. Set the MITM if peer has it set or if peer has DisplayYesNo
  *(iPhone)
  ** as a fallback set MITM+GB if peer had MITM set
  */

  BTIF_TRACE_DEBUG("+%s: p_auth_req=%d", __func__, *p_auth_req);
  if (pairing_cb.is_local_initiated) {
    /* if initing/responding to a dedicated bonding, use dedicate bonding bit */
    *p_auth_req = BTA_AUTH_DD_BOND | BTA_AUTH_SP_YES;
  } else if (!is_orig) {
    /* peer initiated paring. They probably know what they want.
    ** Copy the mitm from peer device.
    */
    BTIF_TRACE_DEBUG("%s: setting p_auth_req to peer's: %d", __func__,
                     pairing_cb.auth_req);
    *p_auth_req = (pairing_cb.auth_req & BTA_AUTH_BONDS);

    /* copy over the MITM bit as well. In addition if the peer has DisplayYesNo,
     * force MITM */
    if ((yes_no_bit) || (pairing_cb.io_cap & BTM_IO_CAP_IO))
      *p_auth_req |= BTA_AUTH_SP_YES;
  } else if (yes_no_bit) {
    /* set the general bonding bit for stored device */
    *p_auth_req = BTA_AUTH_GEN_BOND | yes_no_bit;
  }
  BTIF_TRACE_DEBUG("-%s: p_auth_req=%d", __func__, *p_auth_req);
}

void btif_dm_proc_io_rsp(UNUSED_ATTR const RawAddress& bd_addr,
                         tBTA_IO_CAP io_cap, UNUSED_ATTR tBTA_OOB_DATA oob_data,
                         tBTA_AUTH_REQ auth_req) {
  if (auth_req & BTA_AUTH_BONDS) {
    BTIF_TRACE_DEBUG("%s auth_req:%d", __func__, auth_req);
    pairing_cb.auth_req = auth_req;
    pairing_cb.io_cap = io_cap;
  }
}

void btif_dm_set_oob_for_io_req(tBTA_OOB_DATA* p_has_oob_data) {
  if (is_empty_128bit(oob_cb.oob_data.c192)) {
    *p_has_oob_data = false;
  } else {
    *p_has_oob_data = true;
  }
  BTIF_TRACE_DEBUG("%s: *p_has_oob_data=%d", __func__, *p_has_oob_data);
}

void btif_dm_set_oob_for_le_io_req(const RawAddress& bd_addr,
                                   tBTA_OOB_DATA* p_has_oob_data,
                                   tBTA_LE_AUTH_REQ* p_auth_req) {
  if (!is_empty_128bit(oob_cb.oob_data.le_sc_c) &&
      !is_empty_128bit(oob_cb.oob_data.le_sc_r)) {
    /* We have LE SC OOB data */

    /* make sure OOB data is for this particular device */
    if (bd_addr == oob_cb.bdaddr) {
      *p_auth_req = ((*p_auth_req) | BTM_LE_AUTH_REQ_SC_ONLY);
      *p_has_oob_data = true;
    } else {
      *p_has_oob_data = false;
      BTIF_TRACE_WARNING("%s: remote address didn't match OOB data address",
                         __func__);
    }
  } else if (!is_empty_128bit(oob_cb.oob_data.sm_tk)) {
    /* We have security manager TK */

    /* make sure OOB data is for this particular device */
    if (bd_addr == oob_cb.bdaddr) {
      // When using OOB with TK, SC Secure Connections bit must be disabled.
      tBTA_LE_AUTH_REQ mask = ~BTM_LE_AUTH_REQ_SC_ONLY;
      *p_auth_req = ((*p_auth_req) & mask);

      *p_has_oob_data = true;
    } else {
      *p_has_oob_data = false;
      BTIF_TRACE_WARNING("%s: remote address didn't match OOB data address",
                         __func__);
    }
  } else {
    *p_has_oob_data = false;
  }
  BTIF_TRACE_DEBUG("%s *p_has_oob_data=%d", __func__, *p_has_oob_data);
}

#ifdef BTIF_DM_OOB_TEST
void btif_dm_load_local_oob(void) {
  char prop_oob[PROPERTY_VALUE_MAX];
  osi_property_get("service.brcm.bt.oob", prop_oob, "3");
  BTIF_TRACE_DEBUG("%s: prop_oob = %s", __func__, prop_oob);
  if (prop_oob[0] != '3') {
    if (is_empty_128bit(oob_cb.oob_data.c192)) {
      BTIF_TRACE_DEBUG("%s: read OOB, call BTA_DmLocalOob()", __func__);
      BTA_DmLocalOob();
    }
  }
}

void btif_dm_proc_loc_oob(bool valid, const Octet16& c, const Octet16& r) {
  FILE* fp;
  const char* path_a = "/data/misc/bluedroid/LOCAL/a.key";
  const char* path_b = "/data/misc/bluedroid/LOCAL/b.key";
  const char* path = NULL;
  char prop_oob[PROPERTY_VALUE_MAX];
  BTIF_TRACE_DEBUG("%s: valid=%d", __func__, valid);
  if (is_empty_128bit(oob_cb.oob_data.c192) && valid) {
    BTIF_TRACE_DEBUG("save local OOB data in memory");
    memcpy(oob_cb.oob_data.c192, c.data(), OCTET16_LEN);
    memcpy(oob_cb.oob_data.r192, r.data(), OCTET16_LEN);
    osi_property_get("service.brcm.bt.oob", prop_oob, "3");
    BTIF_TRACE_DEBUG("%s: prop_oob = %s", __func__, prop_oob);
    if (prop_oob[0] == '1')
      path = path_a;
    else if (prop_oob[0] == '2')
      path = path_b;
    if (path) {
      fp = fopen(path, "wb+");
      if (fp == NULL) {
        BTIF_TRACE_DEBUG("%s: failed to save local OOB data to %s", __func__,
                         path);
      } else {
        BTIF_TRACE_DEBUG("%s: save local OOB data into file %s", __func__,
                         path);
        fwrite(c.data(), 1, OCTET16_LEN, fp);
        fwrite(r.data(), 1, OCTET16_LEN, fp);
        fclose(fp);
      }
    }
  }
}

/*******************************************************************************
 *
 * Function         btif_dm_get_smp_config
 *
 * Description      Retrieve the SMP pairing options from the bt_stack.conf
 *                  file. To provide specific pairing options for the host
 *                  add a node with label "SmpOptions" to the config file
 *                  and assign it a comma separated list of 5 values in the
 *                  format: auth, io, ikey, rkey, ksize, oob
 *                  eg: PTS_SmpOptions=0xD,0x4,0xf,0xf,0x10
 *
 * Parameters:      tBTE_APPL_CFG*: pointer to struct defining pairing options
 *
 * Returns          true if the options were successfully read, else false
 *
 ******************************************************************************/
bool btif_dm_get_smp_config(tBTE_APPL_CFG* p_cfg) {
  const std::string* recv = stack_config_get_interface()->get_pts_smp_options();
  if (!recv) {
    BTIF_TRACE_DEBUG("%s: SMP options not found in configuration", __func__);
    return false;
  }

  char conf[64];
  char* pch;
  char* endptr;

  strncpy(conf, recv->c_str(), 64);
  conf[63] = 0;  // null terminate

  pch = strtok(conf, ",");
  if (pch != NULL)
    p_cfg->ble_auth_req = (uint8_t)strtoul(pch, &endptr, 16);
  else
    return false;

  pch = strtok(NULL, ",");
  if (pch != NULL)
    p_cfg->ble_io_cap = (uint8_t)strtoul(pch, &endptr, 16);
  else
    return false;

  pch = strtok(NULL, ",");
  if (pch != NULL)
    p_cfg->ble_init_key = (uint8_t)strtoul(pch, &endptr, 16);
  else
    return false;

  pch = strtok(NULL, ",");
  if (pch != NULL)
    p_cfg->ble_resp_key = (uint8_t)strtoul(pch, &endptr, 16);
  else
    return false;

  pch = strtok(NULL, ",");
  if (pch != NULL)
    p_cfg->ble_max_key_size = (uint8_t)strtoul(pch, &endptr, 16);
  else
    return false;

  return true;
}

bool btif_dm_proc_rmt_oob(const RawAddress& bd_addr, Octet16* p_c,
                          Octet16* p_r) {
  const char* path_a = "/data/misc/bluedroid/LOCAL/a.key";
  const char* path_b = "/data/misc/bluedroid/LOCAL/b.key";
  const char* path = NULL;
  char prop_oob[PROPERTY_VALUE_MAX];
  osi_property_get("service.brcm.bt.oob", prop_oob, "3");
  BTIF_TRACE_DEBUG("%s: prop_oob = %s", __func__, prop_oob);
  if (prop_oob[0] == '1')
    path = path_b;
  else if (prop_oob[0] == '2')
    path = path_a;
  if (!path) {
    BTIF_TRACE_DEBUG("%s: can't open path!", __func__);
    return false;
  }

  FILE* fp = fopen(path, "rb");
  if (fp == NULL) {
    BTIF_TRACE_DEBUG("%s: failed to read OOB keys from %s", __func__, path);
    return false;
  }

  BTIF_TRACE_DEBUG("%s: read OOB data from %s", __func__, path);
  fread(p_c->data(), 1, OCTET16_LEN, fp);
  fread(p_r->data(), 1, OCTET16_LEN, fp);
  fclose(fp);

  RawAddress bt_bd_addr = bd_addr;
  btif_transfer_context(btif_dm_generic_evt, BTIF_DM_CB_BOND_STATE_BONDING,
                        (char*)&bt_bd_addr, sizeof(RawAddress), NULL);
  return true;
}
#endif /*  BTIF_DM_OOB_TEST */

static void btif_dm_ble_key_notif_evt(tBTA_DM_SP_KEY_NOTIF* p_ssp_key_notif) {
  RawAddress bd_addr;
  bt_bdname_t bd_name;
  uint32_t cod;
  int dev_type;

  BTIF_TRACE_DEBUG("%s", __func__);

  /* Remote name update */
  if (!btif_get_device_type(p_ssp_key_notif->bd_addr, &dev_type)) {
    dev_type = BT_DEVICE_TYPE_BLE;
  }
  btif_dm_update_ble_remote_properties(p_ssp_key_notif->bd_addr,
                                       p_ssp_key_notif->bd_name,
                                       (tBT_DEVICE_TYPE)dev_type);
  bd_addr = p_ssp_key_notif->bd_addr;
  memcpy(bd_name.name, p_ssp_key_notif->bd_name, BD_NAME_LEN);

  bond_state_changed(BT_STATUS_SUCCESS, bd_addr, BT_BOND_STATE_BONDING);
  pairing_cb.is_ssp = false;
  cod = COD_UNCLASSIFIED;

  HAL_CBACK(bt_hal_cbacks, ssp_request_cb, &bd_addr, &bd_name, cod,
            BT_SSP_VARIANT_PASSKEY_NOTIFICATION, p_ssp_key_notif->passkey);
}

/*******************************************************************************
 *
 * Function         btif_dm_ble_auth_cmpl_evt
 *
 * Description      Executes authentication complete event in btif context
 *
 * Returns          void
 *
 ******************************************************************************/
static void btif_dm_ble_auth_cmpl_evt(tBTA_DM_AUTH_CMPL* p_auth_cmpl) {
  /* Save link key, if not temporary */
  bt_status_t status = BT_STATUS_FAIL;
  bt_bond_state_t state = BT_BOND_STATE_NONE;

  RawAddress bd_addr = p_auth_cmpl->bd_addr;

  /* Clear OOB data */
  memset(&oob_cb, 0, sizeof(oob_cb));

  if ((p_auth_cmpl->success) && (p_auth_cmpl->key_present)) {
    /* store keys */
  }
  if (p_auth_cmpl->success) {
    status = BT_STATUS_SUCCESS;
    state = BT_BOND_STATE_BONDED;
    int addr_type;
    RawAddress bdaddr = p_auth_cmpl->bd_addr;
    if (btif_storage_get_remote_addr_type(&bdaddr, &addr_type) !=
        BT_STATUS_SUCCESS)
      btif_storage_set_remote_addr_type(&bdaddr, p_auth_cmpl->addr_type);

    /* Test for temporary bonding */
    if (btm_get_bond_type_dev(p_auth_cmpl->bd_addr) == BOND_TYPE_TEMPORARY) {
      BTIF_TRACE_DEBUG("%s: sending BT_BOND_STATE_NONE for Temp pairing",
                       __func__);
      btif_storage_remove_bonded_device(&bdaddr);
      state = BT_BOND_STATE_NONE;
    } else {
      btif_dm_save_ble_bonding_keys(bdaddr);
      btif_dm_get_remote_services_by_transport(&bd_addr, GATT_TRANSPORT_LE);
    }
  } else {
    /*Map the HCI fail reason  to  bt status  */
    switch (p_auth_cmpl->fail_reason) {
      case BTA_DM_AUTH_SMP_PAIR_AUTH_FAIL:
      case BTA_DM_AUTH_SMP_CONFIRM_VALUE_FAIL:
      case BTA_DM_AUTH_SMP_UNKNOWN_ERR:
        btif_dm_remove_ble_bonding_keys();
        status = BT_STATUS_AUTH_FAILURE;
        break;

      case BTA_DM_AUTH_SMP_CONN_TOUT: {
        if (btm_sec_is_a_bonded_dev(bd_addr)) {
          LOG(INFO) << __func__ << " Bonded device addr=" << bd_addr
                    << " timed out - will not remove the keys";
          // Don't send state change to upper layers - otherwise Java think we
          // unbonded, and will disconnect HID profile.
          return;
        }

        btif_dm_remove_ble_bonding_keys();
        status = BT_STATUS_AUTH_FAILURE;
        break;
      }
      case BTA_DM_AUTH_SMP_PAIR_NOT_SUPPORT:
        status = BT_STATUS_AUTH_REJECTED;
        break;
      default:
        btif_dm_remove_ble_bonding_keys();
        status = BT_STATUS_FAIL;
        break;
    }
  }
  if (state == BT_BOND_STATE_BONDED && bd_addr != pairing_cb.static_bdaddr) {
    // Report RPA bonding state to Java in crosskey paring
    bond_state_changed(status, bd_addr, BT_BOND_STATE_BONDING);
  }
  bond_state_changed(status, bd_addr, state);
}

void btif_dm_load_ble_local_keys(void) {
  memset(&ble_local_key_cb, 0, sizeof(btif_dm_local_key_cb_t));

  if (btif_storage_get_ble_local_key(
          BTIF_DM_LE_LOCAL_KEY_ER, &ble_local_key_cb.er) == BT_STATUS_SUCCESS) {
    ble_local_key_cb.is_er_rcvd = true;
    BTIF_TRACE_DEBUG("%s BLE ER key loaded", __func__);
  }

  if ((btif_storage_get_ble_local_key(BTIF_DM_LE_LOCAL_KEY_IR,
                                      &ble_local_key_cb.id_keys.ir) ==
       BT_STATUS_SUCCESS) &&
      (btif_storage_get_ble_local_key(BTIF_DM_LE_LOCAL_KEY_IRK,
                                      &ble_local_key_cb.id_keys.irk) ==
       BT_STATUS_SUCCESS) &&
      (btif_storage_get_ble_local_key(BTIF_DM_LE_LOCAL_KEY_DHK,
                                      &ble_local_key_cb.id_keys.dhk) ==
       BT_STATUS_SUCCESS)) {
    ble_local_key_cb.is_id_keys_rcvd = true;
    BTIF_TRACE_DEBUG("%s BLE ID keys loaded", __func__);
  }
}
void btif_dm_get_ble_local_keys(tBTA_DM_BLE_LOCAL_KEY_MASK* p_key_mask,
                                Octet16* p_er,
                                tBTA_BLE_LOCAL_ID_KEYS* p_id_keys) {
  if (ble_local_key_cb.is_er_rcvd) {
    *p_er = ble_local_key_cb.er;
    *p_key_mask |= BTA_BLE_LOCAL_KEY_TYPE_ER;
  }

  if (ble_local_key_cb.is_id_keys_rcvd) {
    p_id_keys->ir = ble_local_key_cb.id_keys.ir;
    p_id_keys->irk = ble_local_key_cb.id_keys.irk;
    p_id_keys->dhk = ble_local_key_cb.id_keys.dhk;
    *p_key_mask |= BTA_BLE_LOCAL_KEY_TYPE_ID;
  }
  BTIF_TRACE_DEBUG("%s  *p_key_mask=0x%02x", __func__, *p_key_mask);
}

void btif_dm_save_ble_bonding_keys(RawAddress& bd_addr) {
  BTIF_TRACE_DEBUG("%s", __func__);

  if (pairing_cb.ble.is_penc_key_rcvd) {
    btif_storage_add_ble_bonding_key(
        &bd_addr, (uint8_t*)&pairing_cb.ble.penc_key, BTIF_DM_LE_KEY_PENC,
        sizeof(tBTM_LE_PENC_KEYS));
  }

  if (pairing_cb.ble.is_pid_key_rcvd) {
    btif_storage_add_ble_bonding_key(
        &bd_addr, (uint8_t*)&pairing_cb.ble.pid_key, BTIF_DM_LE_KEY_PID,
        sizeof(tBTM_LE_PID_KEYS));
  }

  if (pairing_cb.ble.is_pcsrk_key_rcvd) {
    btif_storage_add_ble_bonding_key(
        &bd_addr, (uint8_t*)&pairing_cb.ble.pcsrk_key, BTIF_DM_LE_KEY_PCSRK,
        sizeof(tBTM_LE_PCSRK_KEYS));
  }

  if (pairing_cb.ble.is_lenc_key_rcvd) {
    btif_storage_add_ble_bonding_key(
        &bd_addr, (uint8_t*)&pairing_cb.ble.lenc_key, BTIF_DM_LE_KEY_LENC,
        sizeof(tBTM_LE_LENC_KEYS));
  }

  if (pairing_cb.ble.is_lcsrk_key_rcvd) {
    btif_storage_add_ble_bonding_key(
        &bd_addr, (uint8_t*)&pairing_cb.ble.lcsrk_key, BTIF_DM_LE_KEY_LCSRK,
        sizeof(tBTM_LE_LCSRK_KEYS));
  }

  if (pairing_cb.ble.is_lidk_key_rcvd) {
    btif_storage_add_ble_bonding_key(&bd_addr, NULL, BTIF_DM_LE_KEY_LID, 0);
  }
}

void btif_dm_remove_ble_bonding_keys(void) {
  BTIF_TRACE_DEBUG("%s", __func__);

  RawAddress bd_addr = pairing_cb.bd_addr;
  btif_storage_remove_ble_bonding_keys(&bd_addr);
}

/*******************************************************************************
 *
 * Function         btif_dm_ble_sec_req_evt
 *
 * Description      Eprocess security request event in btif context
 *
 * Returns          void
 *
 ******************************************************************************/
void btif_dm_ble_sec_req_evt(tBTA_DM_BLE_SEC_REQ* p_ble_req) {
  bt_bdname_t bd_name;
  uint32_t cod;
  int dev_type;

  BTIF_TRACE_DEBUG("%s", __func__);

  if (pairing_cb.state == BT_BOND_STATE_BONDING) {
    BTIF_TRACE_DEBUG("%s Discard security request", __func__);
    return;
  }

  /* Remote name update */
  if (!btif_get_device_type(p_ble_req->bd_addr, &dev_type)) {
    dev_type = BT_DEVICE_TYPE_BLE;
  }
  btif_dm_update_ble_remote_properties(p_ble_req->bd_addr, p_ble_req->bd_name,
                                       (tBT_DEVICE_TYPE)dev_type);

  RawAddress bd_addr = p_ble_req->bd_addr;
  memcpy(bd_name.name, p_ble_req->bd_name, BD_NAME_LEN);

  bond_state_changed(BT_STATUS_SUCCESS, bd_addr, BT_BOND_STATE_BONDING);

  pairing_cb.bond_type = BOND_TYPE_PERSISTENT;
  pairing_cb.is_le_only = true;
  pairing_cb.is_le_nc = false;
  pairing_cb.is_ssp = true;
  btm_set_bond_type_dev(p_ble_req->bd_addr, pairing_cb.bond_type);

  cod = COD_UNCLASSIFIED;

  HAL_CBACK(bt_hal_cbacks, ssp_request_cb, &bd_addr, &bd_name, cod,
            BT_SSP_VARIANT_CONSENT, 0);
}

/*******************************************************************************
 *
 * Function         btif_dm_ble_passkey_req_evt
 *
 * Description      Executes pin request event in btif context
 *
 * Returns          void
 *
 ******************************************************************************/
static void btif_dm_ble_passkey_req_evt(tBTA_DM_PIN_REQ* p_pin_req) {
  bt_bdname_t bd_name;
  uint32_t cod;
  int dev_type;

  /* Remote name update */
  if (!btif_get_device_type(p_pin_req->bd_addr, &dev_type)) {
    dev_type = BT_DEVICE_TYPE_BLE;
  }
  btif_dm_update_ble_remote_properties(p_pin_req->bd_addr, p_pin_req->bd_name,
                                       (tBT_DEVICE_TYPE)dev_type);

  RawAddress bd_addr = p_pin_req->bd_addr;
  memcpy(bd_name.name, p_pin_req->bd_name, BD_NAME_LEN);

  bond_state_changed(BT_STATUS_SUCCESS, bd_addr, BT_BOND_STATE_BONDING);
  pairing_cb.is_le_only = true;

  cod = COD_UNCLASSIFIED;

  HAL_CBACK(bt_hal_cbacks, pin_request_cb, &bd_addr, &bd_name, cod, false);
}
static void btif_dm_ble_key_nc_req_evt(tBTA_DM_SP_KEY_NOTIF* p_notif_req) {
  /* TODO implement key notification for numeric comparison */
  BTIF_TRACE_DEBUG("%s", __func__);

  /* Remote name update */
  btif_update_remote_properties(p_notif_req->bd_addr, p_notif_req->bd_name,
                                NULL, BT_DEVICE_TYPE_BLE);

  RawAddress bd_addr = p_notif_req->bd_addr;

  bt_bdname_t bd_name;
  memcpy(bd_name.name, p_notif_req->bd_name, BD_NAME_LEN);

  bond_state_changed(BT_STATUS_SUCCESS, bd_addr, BT_BOND_STATE_BONDING);
  pairing_cb.is_ssp = false;
  pairing_cb.is_le_only = true;
  pairing_cb.is_le_nc = true;

  HAL_CBACK(bt_hal_cbacks, ssp_request_cb, &bd_addr, &bd_name, COD_UNCLASSIFIED,
            BT_SSP_VARIANT_PASSKEY_CONFIRMATION, p_notif_req->passkey);
}

static void btif_dm_ble_oob_req_evt(tBTA_DM_SP_RMT_OOB* req_oob_type) {
  BTIF_TRACE_DEBUG("%s", __func__);

  RawAddress bd_addr = req_oob_type->bd_addr;
  /* We already checked if OOB data is present in
   * btif_dm_set_oob_for_le_io_req, but check here again. If it's not present
   * do nothing, pairing will timeout.
   */
  if (is_empty_128bit(oob_cb.oob_data.sm_tk)) {
    return;
  }

  /* make sure OOB data is for this particular device */
  if (req_oob_type->bd_addr != oob_cb.bdaddr) {
    BTIF_TRACE_WARNING("%s: remote address didn't match OOB data address",
                       __func__);
    return;
  }

  /* Remote name update */
  btif_update_remote_properties(req_oob_type->bd_addr, req_oob_type->bd_name,
                                NULL, BT_DEVICE_TYPE_BLE);

  bond_state_changed(BT_STATUS_SUCCESS, bd_addr, BT_BOND_STATE_BONDING);
  pairing_cb.is_ssp = false;
  pairing_cb.is_le_only = true;
  pairing_cb.is_le_nc = false;

  BTM_BleOobDataReply(req_oob_type->bd_addr, 0, 16, oob_cb.oob_data.sm_tk);
}

static void btif_dm_ble_sc_oob_req_evt(tBTA_DM_SP_RMT_OOB* req_oob_type) {
  BTIF_TRACE_DEBUG("%s", __func__);

  RawAddress bd_addr = req_oob_type->bd_addr;

  /* We already checked if OOB data is present in
   * btif_dm_set_oob_for_le_io_req, but check here again. If it's not present
   * do nothing, pairing will timeout.
   */
  if (is_empty_128bit(oob_cb.oob_data.le_sc_c) &&
      is_empty_128bit(oob_cb.oob_data.le_sc_r)) {
    BTIF_TRACE_WARNING("%s: LE SC OOB data is empty", __func__);
    return;
  }

  /* make sure OOB data is for this particular device */
  if (req_oob_type->bd_addr != oob_cb.bdaddr) {
    BTIF_TRACE_WARNING("%s: remote address didn't match OOB data address",
                       __func__);
    return;
  }

  /* Remote name update */
  btif_update_remote_properties(req_oob_type->bd_addr, req_oob_type->bd_name,
                                NULL, BT_DEVICE_TYPE_BLE);

  bond_state_changed(BT_STATUS_SUCCESS, bd_addr, BT_BOND_STATE_BONDING);
  pairing_cb.is_ssp = false;
  pairing_cb.is_le_only =
      true;  // TODO: we can derive classic pairing from this one
  pairing_cb.is_le_nc = false;

  BTM_BleSecureConnectionOobDataReply(
      req_oob_type->bd_addr, oob_cb.oob_data.le_sc_c, oob_cb.oob_data.le_sc_r);
}

void btif_dm_update_ble_remote_properties(const RawAddress& bd_addr,
                                          BD_NAME bd_name,
                                          tBT_DEVICE_TYPE dev_type) {
  btif_update_remote_properties(bd_addr, bd_name, NULL, dev_type);
}

static void btif_dm_ble_tx_test_cback(void* p) {
  btif_transfer_context(btif_dm_generic_evt, BTIF_DM_CB_LE_TX_TEST, (char*)p, 1,
                        NULL);
}

static void btif_dm_ble_rx_test_cback(void* p) {
  btif_transfer_context(btif_dm_generic_evt, BTIF_DM_CB_LE_RX_TEST, (char*)p, 1,
                        NULL);
}

static void btif_dm_ble_test_end_cback(void* p) {
  btif_transfer_context(btif_dm_generic_evt, BTIF_DM_CB_LE_TEST_END, (char*)p,
                        3, NULL);
}
/*******************************************************************************
 *
 * Function         btif_le_test_mode
 *
 * Description     Sends a HCI BLE Test command to the Controller
 *
 * Returns          BT_STATUS_SUCCESS on success
 *
 ******************************************************************************/
bt_status_t btif_le_test_mode(uint16_t opcode, uint8_t* buf, uint8_t len) {
  switch (opcode) {
    case HCI_BLE_TRANSMITTER_TEST:
      if (len != 3) return BT_STATUS_PARM_INVALID;
      BTM_BleTransmitterTest(buf[0], buf[1], buf[2], btif_dm_ble_tx_test_cback);
      break;
    case HCI_BLE_RECEIVER_TEST:
      if (len != 1) return BT_STATUS_PARM_INVALID;
      BTM_BleReceiverTest(buf[0], btif_dm_ble_rx_test_cback);
      break;
    case HCI_BLE_TEST_END:
      BTM_BleTestEnd(btif_dm_ble_test_end_cback);
      break;
    default:
      BTIF_TRACE_ERROR("%s: Unknown LE Test Mode Command 0x%x", __func__,
                       opcode);
      return BT_STATUS_UNSUPPORTED;
  }
  return BT_STATUS_SUCCESS;
}

void btif_dm_on_disable() {
  /* cancel any pending pairing requests */
  if (is_bonding_or_sdp()) {
    BTIF_TRACE_DEBUG("%s: Cancel pending pairing request", __func__);
    btif_dm_cancel_bond(&pairing_cb.bd_addr);
  }
}

/*******************************************************************************
 *
 * Function         btif_dm_read_energy_info
 *
 * Description     Reads the energy info from controller
 *
 * Returns         void
 *
 ******************************************************************************/
void btif_dm_read_energy_info() { BTA_DmBleGetEnergyInfo(bta_energy_info_cb); }

static char* btif_get_default_local_name() {
  if (btif_default_local_name[0] == '\0') {
    int max_len = sizeof(btif_default_local_name) - 1;
    if (BTM_DEF_LOCAL_NAME[0] != '\0') {
      strncpy(btif_default_local_name, BTM_DEF_LOCAL_NAME, max_len);
    } else {
      char prop_model[PROPERTY_VALUE_MAX];
      osi_property_get(PROPERTY_PRODUCT_MODEL, prop_model, "");
      strncpy(btif_default_local_name, prop_model, max_len);
    }
    btif_default_local_name[max_len] = '\0';
  }
  return btif_default_local_name;
}

static void btif_stats_add_bond_event(const RawAddress& bd_addr,
                                      bt_bond_function_t function,
                                      bt_bond_state_t state) {
  std::unique_lock<std::mutex> lock(bond_event_lock);

  btif_bond_event_t* event = &btif_dm_bond_events[btif_events_end_index];
  event->bd_addr = bd_addr;
  event->function = function;
  event->state = state;
  clock_gettime(CLOCK_REALTIME, &event->timestamp);

  btif_num_bond_events++;
  btif_events_end_index =
      (btif_events_end_index + 1) % (MAX_BTIF_BOND_EVENT_ENTRIES + 1);
  if (btif_events_end_index == btif_events_start_index) {
    btif_events_start_index =
        (btif_events_start_index + 1) % (MAX_BTIF_BOND_EVENT_ENTRIES + 1);
  }

  int type;
  btif_get_device_type(bd_addr, &type);

  bluetooth::common::device_type_t device_type;
  switch (type) {
    case BT_DEVICE_TYPE_BREDR:
      device_type = bluetooth::common::DEVICE_TYPE_BREDR;
      break;
    case BT_DEVICE_TYPE_BLE:
      device_type = bluetooth::common::DEVICE_TYPE_LE;
      break;
    case BT_DEVICE_TYPE_DUMO:
      device_type = bluetooth::common::DEVICE_TYPE_DUMO;
      break;
    default:
      device_type = bluetooth::common::DEVICE_TYPE_UNKNOWN;
      break;
  }

  uint32_t cod = get_cod(&bd_addr);
  uint64_t ts =
      event->timestamp.tv_sec * 1000 + event->timestamp.tv_nsec / 1000000;
  bluetooth::common::BluetoothMetricsLogger::GetInstance()->LogPairEvent(
      0, ts, cod, device_type);
}

void btif_debug_bond_event_dump(int fd) {
  std::unique_lock<std::mutex> lock(bond_event_lock);
  dprintf(fd, "\nBond Events: \n");
  dprintf(fd, "  Total Number of events: %zu\n", btif_num_bond_events);
  if (btif_num_bond_events > 0)
    dprintf(fd,
            "  Time          address            Function             State\n");

  for (size_t i = btif_events_start_index; i != btif_events_end_index;
       i = (i + 1) % (MAX_BTIF_BOND_EVENT_ENTRIES + 1)) {
    btif_bond_event_t* event = &btif_dm_bond_events[i];

    char eventtime[20];
    char temptime[20];
    struct tm* tstamp = localtime(&event->timestamp.tv_sec);
    strftime(temptime, sizeof(temptime), "%H:%M:%S", tstamp);
    snprintf(eventtime, sizeof(eventtime), "%s.%03ld", temptime,
             event->timestamp.tv_nsec / 1000000);

    const char* func_name;
    switch (event->function) {
      case BTIF_DM_FUNC_CREATE_BOND:
        func_name = "btif_dm_create_bond";
        break;
      case BTIF_DM_FUNC_REMOVE_BOND:
        func_name = "btif_dm_remove_bond";
        break;
      case BTIF_DM_FUNC_BOND_STATE_CHANGED:
        func_name = "bond_state_changed ";
        break;
      default:
        func_name = "Invalid value      ";
        break;
    }

    const char* bond_state;
    switch (event->state) {
      case BT_BOND_STATE_NONE:
        bond_state = "BOND_STATE_NONE";
        break;
      case BT_BOND_STATE_BONDING:
        bond_state = "BOND_STATE_BONDING";
        break;
      case BT_BOND_STATE_BONDED:
        bond_state = "BOND_STATE_BONDED";
        break;
      default:
        bond_state = "Invalid bond state";
        break;
    }

    dprintf(fd, "  %s  %s  %s  %s\n", eventtime,
            event->bd_addr.ToString().c_str(), func_name, bond_state);
  }
}
