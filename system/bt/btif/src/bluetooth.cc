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
 *  Filename:      bluetooth.c
 *
 *  Description:   Bluetooth HAL implementation
 *
 ******************************************************************************/

#define LOG_TAG "bt_btif"

#include <base/logging.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <hardware/bluetooth.h>
#include <hardware/bluetooth_headset_interface.h>
#include <hardware/bt_av.h>
#include <hardware/bt_gatt.h>
#include <hardware/bt_hd.h>
#include <hardware/bt_hearing_aid.h>
#include <hardware/bt_hf_client.h>
#include <hardware/bt_hh.h>
#include <hardware/bt_mce.h>
#include <hardware/bt_pan.h>
#include <hardware/bt_rc.h>
#include <hardware/bt_sdp.h>
#include <hardware/bt_sock.h>

#include "bt_utils.h"
#include "bta/include/bta_hearing_aid_api.h"
#include "bta/include/bta_hf_client_api.h"
#include "btif/avrcp/avrcp_service.h"
#include "btif_a2dp.h"
#include "btif_api.h"
#include "btif_av.h"
#include "btif_bqr.h"
#include "btif_config.h"
#include "btif_debug.h"
#include "btif_debug_btsnoop.h"
#include "btif_debug_conn.h"
#include "btif_hf.h"
#include "btif_storage.h"
#include "btsnoop.h"
#include "btsnoop_mem.h"
#include "common/address_obfuscator.h"
#include "common/metrics.h"
#include "device/include/interop.h"
#include "osi/include/alarm.h"
#include "osi/include/allocation_tracker.h"
#include "osi/include/log.h"
#include "osi/include/osi.h"
#include "osi/include/wakelock.h"
#include "stack/gatt/connection_manager.h"
#include "stack_manager.h"

using bluetooth::hearing_aid::HearingAidInterface;

/*******************************************************************************
 *  Static variables
 ******************************************************************************/

bt_callbacks_t* bt_hal_cbacks = NULL;
bool restricted_mode = false;
bool niap_mode = false;

/*******************************************************************************
 *  Externs
 ******************************************************************************/

/* list all extended interfaces here */

/* handsfree profile - client */
extern const bthf_client_interface_t* btif_hf_client_get_interface();
/* advanced audio profile */
extern const btav_source_interface_t* btif_av_get_src_interface();
extern const btav_sink_interface_t* btif_av_get_sink_interface();
/*rfc l2cap*/
extern const btsock_interface_t* btif_sock_get_interface();
/* hid host profile */
extern const bthh_interface_t* btif_hh_get_interface();
/* hid device profile */
extern const bthd_interface_t* btif_hd_get_interface();
/*pan*/
extern const btpan_interface_t* btif_pan_get_interface();
/*map client*/
extern const btmce_interface_t* btif_mce_get_interface();
/* gatt */
extern const btgatt_interface_t* btif_gatt_get_interface();
/* avrc target */
extern const btrc_interface_t* btif_rc_get_interface();
/* avrc controller */
extern const btrc_ctrl_interface_t* btif_rc_ctrl_get_interface();
/*SDP search client*/
extern const btsdp_interface_t* btif_sdp_get_interface();
/*Hearing Aid client*/
extern HearingAidInterface* btif_hearing_aid_get_interface();

/*******************************************************************************
 *  Functions
 ******************************************************************************/

static bool interface_ready(void) { return bt_hal_cbacks != NULL; }

static bool is_profile(const char* p1, const char* p2) {
  CHECK(p1);
  CHECK(p2);
  return strlen(p1) == strlen(p2) && strncmp(p1, p2, strlen(p2)) == 0;
}

/*****************************************************************************
 *
 *   BLUETOOTH HAL INTERFACE FUNCTIONS
 *
 ****************************************************************************/

static int init(bt_callbacks_t* callbacks, bool start_restricted,
                bool is_niap_mode) {
  LOG_INFO(LOG_TAG, "%s: start restricted = %d ; niap = %d", __func__,
           start_restricted, is_niap_mode);

  if (interface_ready()) return BT_STATUS_DONE;

#ifdef BLUEDROID_DEBUG
  allocation_tracker_init();
#endif

  bt_hal_cbacks = callbacks;
  restricted_mode = start_restricted;
  niap_mode = is_niap_mode;
  stack_manager_get_interface()->init_stack();
  btif_debug_init();
  return BT_STATUS_SUCCESS;
}

static int enable() {
  if (!interface_ready()) return BT_STATUS_NOT_READY;

  stack_manager_get_interface()->start_up_stack_async();
  return BT_STATUS_SUCCESS;
}

static int disable(void) {
  if (!interface_ready()) return BT_STATUS_NOT_READY;

  stack_manager_get_interface()->shut_down_stack_async();
  return BT_STATUS_SUCCESS;
}

static void cleanup(void) { stack_manager_get_interface()->clean_up_stack(); }

bool is_restricted_mode() { return restricted_mode; }
bool is_niap_mode() { return niap_mode; }

static int get_adapter_properties(void) {
  /* sanity check */
  if (!interface_ready()) return BT_STATUS_NOT_READY;

  return btif_get_adapter_properties();
}

static int get_adapter_property(bt_property_type_t type) {
  /* sanity check */
  if (!interface_ready()) return BT_STATUS_NOT_READY;

  return btif_get_adapter_property(type);
}

static int set_adapter_property(const bt_property_t* property) {
  /* sanity check */
  if (!interface_ready()) return BT_STATUS_NOT_READY;

  return btif_set_adapter_property(property);
}

int get_remote_device_properties(RawAddress* remote_addr) {
  /* sanity check */
  if (!interface_ready()) return BT_STATUS_NOT_READY;

  return btif_get_remote_device_properties(remote_addr);
}

int get_remote_device_property(RawAddress* remote_addr,
                               bt_property_type_t type) {
  /* sanity check */
  if (!interface_ready()) return BT_STATUS_NOT_READY;

  return btif_get_remote_device_property(remote_addr, type);
}

int set_remote_device_property(RawAddress* remote_addr,
                               const bt_property_t* property) {
  /* sanity check */
  if (!interface_ready()) return BT_STATUS_NOT_READY;

  return btif_set_remote_device_property(remote_addr, property);
}

int get_remote_service_record(const RawAddress& remote_addr,
                              const bluetooth::Uuid& uuid) {
  /* sanity check */
  if (!interface_ready()) return BT_STATUS_NOT_READY;

  return btif_get_remote_service_record(remote_addr, uuid);
}

int get_remote_services(RawAddress* remote_addr) {
  /* sanity check */
  if (!interface_ready()) return BT_STATUS_NOT_READY;

  return btif_dm_get_remote_services(*remote_addr);
}

static int start_discovery(void) {
  /* sanity check */
  if (!interface_ready()) return BT_STATUS_NOT_READY;

  return btif_dm_start_discovery();
}

static int cancel_discovery(void) {
  /* sanity check */
  if (!interface_ready()) return BT_STATUS_NOT_READY;

  return btif_dm_cancel_discovery();
}

static int create_bond(const RawAddress* bd_addr, int transport) {
  /* sanity check */
  if (!interface_ready()) return BT_STATUS_NOT_READY;

  return btif_dm_create_bond(bd_addr, transport);
}

static int create_bond_out_of_band(const RawAddress* bd_addr, int transport,
                                   const bt_out_of_band_data_t* oob_data) {
  /* sanity check */
  if (!interface_ready()) return BT_STATUS_NOT_READY;

  return btif_dm_create_bond_out_of_band(bd_addr, transport, oob_data);
}

static int cancel_bond(const RawAddress* bd_addr) {
  /* sanity check */
  if (!interface_ready()) return BT_STATUS_NOT_READY;

  return btif_dm_cancel_bond(bd_addr);
}

static int remove_bond(const RawAddress* bd_addr) {
  if (is_restricted_mode() && !btif_storage_is_restricted_device(bd_addr))
    return BT_STATUS_SUCCESS;

  /* sanity check */
  if (!interface_ready()) return BT_STATUS_NOT_READY;

  return btif_dm_remove_bond(bd_addr);
}

static int get_connection_state(const RawAddress* bd_addr) {
  /* sanity check */
  if (!interface_ready()) return 0;

  return btif_dm_get_connection_state(bd_addr);
}

static int pin_reply(const RawAddress* bd_addr, uint8_t accept, uint8_t pin_len,
                     bt_pin_code_t* pin_code) {
  /* sanity check */
  if (!interface_ready()) return BT_STATUS_NOT_READY;

  return btif_dm_pin_reply(bd_addr, accept, pin_len, pin_code);
}

static int ssp_reply(const RawAddress* bd_addr, bt_ssp_variant_t variant,
                     uint8_t accept, uint32_t passkey) {
  /* sanity check */
  if (!interface_ready()) return BT_STATUS_NOT_READY;

  return btif_dm_ssp_reply(bd_addr, variant, accept, passkey);
}

static int read_energy_info() {
  if (!interface_ready()) return BT_STATUS_NOT_READY;
  btif_dm_read_energy_info();
  return BT_STATUS_SUCCESS;
}

static void dump(int fd, const char** arguments) {
  btif_debug_conn_dump(fd);
  btif_debug_bond_event_dump(fd);
  btif_debug_a2dp_dump(fd);
  btif_debug_av_dump(fd);
  bta_debug_av_dump(fd);
  stack_debug_avdtp_api_dump(fd);
  bluetooth::avrcp::AvrcpService::DebugDump(fd);
  btif_debug_config_dump(fd);
  BTA_HfClientDumpStatistics(fd);
  wakelock_debug_dump(fd);
  osi_allocator_debug_dump(fd);
  alarm_debug_dump(fd);
  HearingAid::DebugDump(fd);
  connection_manager::dump(fd);
  bluetooth::bqr::DebugDump(fd);
#if (BTSNOOP_MEM == TRUE)
  btif_debug_btsnoop_dump(fd);
#endif
}

static void dumpMetrics(std::string* output) {
  bluetooth::common::BluetoothMetricsLogger::GetInstance()->WriteString(output);
}

static const void* get_profile_interface(const char* profile_id) {
  LOG_INFO(LOG_TAG, "%s: id = %s", __func__, profile_id);

  /* sanity check */
  if (!interface_ready()) return NULL;

  /* check for supported profile interfaces */
  if (is_profile(profile_id, BT_PROFILE_HANDSFREE_ID))
    return bluetooth::headset::GetInterface();

  if (is_profile(profile_id, BT_PROFILE_HANDSFREE_CLIENT_ID))
    return btif_hf_client_get_interface();

  if (is_profile(profile_id, BT_PROFILE_SOCKETS_ID))
    return btif_sock_get_interface();

  if (is_profile(profile_id, BT_PROFILE_PAN_ID))
    return btif_pan_get_interface();

  if (is_profile(profile_id, BT_PROFILE_ADVANCED_AUDIO_ID))
    return btif_av_get_src_interface();

  if (is_profile(profile_id, BT_PROFILE_ADVANCED_AUDIO_SINK_ID))
    return btif_av_get_sink_interface();

  if (is_profile(profile_id, BT_PROFILE_HIDHOST_ID))
    return btif_hh_get_interface();

  if (is_profile(profile_id, BT_PROFILE_HIDDEV_ID))
    return btif_hd_get_interface();

  if (is_profile(profile_id, BT_PROFILE_SDP_CLIENT_ID))
    return btif_sdp_get_interface();

  if (is_profile(profile_id, BT_PROFILE_GATT_ID))
    return btif_gatt_get_interface();

  if (is_profile(profile_id, BT_PROFILE_AV_RC_ID))
    return btif_rc_get_interface();

  if (is_profile(profile_id, BT_PROFILE_AV_RC_CTRL_ID))
    return btif_rc_ctrl_get_interface();

  if (is_profile(profile_id, BT_PROFILE_HEARING_AID_ID))
    return btif_hearing_aid_get_interface();
  return NULL;
}

int dut_mode_configure(uint8_t enable) {
  LOG_INFO(LOG_TAG, "%s", __func__);

  /* sanity check */
  if (!interface_ready()) return BT_STATUS_NOT_READY;

  return btif_dut_mode_configure(enable);
}

int dut_mode_send(uint16_t opcode, uint8_t* buf, uint8_t len) {
  LOG_INFO(LOG_TAG, "%s", __func__);

  /* sanity check */
  if (!interface_ready()) return BT_STATUS_NOT_READY;

  return btif_dut_mode_send(opcode, buf, len);
}

int le_test_mode(uint16_t opcode, uint8_t* buf, uint8_t len) {
  LOG_INFO(LOG_TAG, "%s", __func__);

  /* sanity check */
  if (!interface_ready()) return BT_STATUS_NOT_READY;

  return btif_le_test_mode(opcode, buf, len);
}

static bt_os_callouts_t* wakelock_os_callouts_saved = nullptr;

static int acquire_wake_lock_cb(const char* lock_name) {
  return do_in_jni_thread(
      FROM_HERE, base::Bind(base::IgnoreResult(
                                wakelock_os_callouts_saved->acquire_wake_lock),
                            lock_name));
}

static int release_wake_lock_cb(const char* lock_name) {
  return do_in_jni_thread(
      FROM_HERE, base::Bind(base::IgnoreResult(
                                wakelock_os_callouts_saved->release_wake_lock),
                            lock_name));
}

static bt_os_callouts_t wakelock_os_callouts_jni = {
    sizeof(wakelock_os_callouts_jni),
    nullptr /* not used */,
    acquire_wake_lock_cb,
    release_wake_lock_cb,
};

static int set_os_callouts(bt_os_callouts_t* callouts) {
  wakelock_os_callouts_saved = callouts;
  wakelock_set_os_callouts(&wakelock_os_callouts_jni);
  return BT_STATUS_SUCCESS;
}

static int config_clear(void) {
  LOG_INFO(LOG_TAG, "%s", __func__);
  return btif_config_clear() ? BT_STATUS_SUCCESS : BT_STATUS_FAIL;
}

static bluetooth::avrcp::ServiceInterface* get_avrcp_service(void) {
  return bluetooth::avrcp::AvrcpService::GetServiceInterface();
}

static std::string obfuscate_address(const RawAddress& address) {
  return bluetooth::common::AddressObfuscator::GetInstance()->Obfuscate(
      address);
}

EXPORT_SYMBOL bt_interface_t bluetoothInterface = {
    sizeof(bluetoothInterface),
    init,
    enable,
    disable,
    cleanup,
    get_adapter_properties,
    get_adapter_property,
    set_adapter_property,
    get_remote_device_properties,
    get_remote_device_property,
    set_remote_device_property,
    get_remote_service_record,
    get_remote_services,
    start_discovery,
    cancel_discovery,
    create_bond,
    create_bond_out_of_band,
    remove_bond,
    cancel_bond,
    get_connection_state,
    pin_reply,
    ssp_reply,
    get_profile_interface,
    dut_mode_configure,
    dut_mode_send,
    le_test_mode,
    set_os_callouts,
    read_energy_info,
    dump,
    dumpMetrics,
    config_clear,
    interop_database_clear,
    interop_database_add,
    get_avrcp_service,
    obfuscate_address,
};
