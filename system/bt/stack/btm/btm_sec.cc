/******************************************************************************
 *
 *  Copyright 1999-2012 Broadcom Corporation
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

/******************************************************************************
 *
 *  This file contains functions for the Bluetooth Security Manager
 *
 ******************************************************************************/

#define LOG_TAG "bt_btm_sec"

#include <frameworks/base/core/proto/android/bluetooth/enums.pb.h>
#include <frameworks/base/core/proto/android/bluetooth/hci/enums.pb.h>
#include <log/log.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "common/metrics.h"
#include "common/time_util.h"
#include "device/include/controller.h"
#include "osi/include/log.h"
#include "osi/include/osi.h"

#include "bt_types.h"
#include "bt_utils.h"
#include "btif_storage.h"
#include "btm_int.h"
#include "btu.h"
#include "hcimsgs.h"
#include "l2c_int.h"

#include "gatt_int.h"

#define BTM_SEC_MAX_COLLISION_DELAY (5000)

#ifdef APPL_AUTH_WRITE_EXCEPTION
bool(APPL_AUTH_WRITE_EXCEPTION)(const RawAddress& bd_addr);
#endif

extern void btm_ble_advertiser_notify_terminated_legacy(
    uint8_t status, uint16_t connection_handle);
extern void bta_dm_remove_device(const RawAddress& bd_addr);

/*******************************************************************************
 *             L O C A L    F U N C T I O N     P R O T O T Y P E S            *
 ******************************************************************************/
tBTM_SEC_SERV_REC* btm_sec_find_first_serv(bool is_originator, uint16_t psm);
static tBTM_SEC_SERV_REC* btm_sec_find_next_serv(tBTM_SEC_SERV_REC* p_cur);
static tBTM_SEC_SERV_REC* btm_sec_find_mx_serv(uint8_t is_originator,
                                               uint16_t psm,
                                               uint32_t mx_proto_id,
                                               uint32_t mx_chan_id);

static bool btm_sec_start_get_name(tBTM_SEC_DEV_REC* p_dev_rec);
static void btm_sec_start_authentication(tBTM_SEC_DEV_REC* p_dev_rec);
static void btm_sec_start_encryption(tBTM_SEC_DEV_REC* p_dev_rec);
static void btm_sec_collision_timeout(void* data);
static void btm_restore_mode(void);
static void btm_sec_pairing_timeout(void* data);
static tBTM_STATUS btm_sec_dd_create_conn(tBTM_SEC_DEV_REC* p_dev_rec);
static void btm_sec_change_pairing_state(tBTM_PAIRING_STATE new_state);

static const char* btm_pair_state_descr(tBTM_PAIRING_STATE state);

static void btm_sec_check_pending_reqs(void);
static bool btm_sec_queue_mx_request(const RawAddress& bd_addr, uint16_t psm,
                                     bool is_orig, uint32_t mx_proto_id,
                                     uint32_t mx_chan_id,
                                     tBTM_SEC_CALLBACK* p_callback,
                                     void* p_ref_data);
static void btm_sec_bond_cancel_complete(void);
static void btm_send_link_key_notif(tBTM_SEC_DEV_REC* p_dev_rec);
static bool btm_sec_check_prefetch_pin(tBTM_SEC_DEV_REC* p_dev_rec);

static uint8_t btm_sec_start_authorization(tBTM_SEC_DEV_REC* p_dev_rec);
bool btm_sec_are_all_trusted(uint32_t p_mask[]);

static tBTM_STATUS btm_sec_send_hci_disconnect(tBTM_SEC_DEV_REC* p_dev_rec,
                                               uint8_t reason,
                                               uint16_t conn_handle);
uint8_t btm_sec_start_role_switch(tBTM_SEC_DEV_REC* p_dev_rec);
tBTM_SEC_DEV_REC* btm_sec_find_dev_by_sec_state(uint8_t state);

static bool btm_sec_set_security_level(CONNECTION_TYPE conn_type,
                                       const char* p_name, uint8_t service_id,
                                       uint16_t sec_level, uint16_t psm,
                                       uint32_t mx_proto_id,
                                       uint32_t mx_chan_id);

static bool btm_dev_authenticated(tBTM_SEC_DEV_REC* p_dev_rec);
static bool btm_dev_encrypted(tBTM_SEC_DEV_REC* p_dev_rec);
static bool btm_dev_authorized(tBTM_SEC_DEV_REC* p_dev_rec);
static bool btm_serv_trusted(tBTM_SEC_DEV_REC* p_dev_rec,
                             tBTM_SEC_SERV_REC* p_serv_rec);
static bool btm_sec_is_serv_level0(uint16_t psm);
static uint16_t btm_sec_set_serv_level4_flags(uint16_t cur_security,
                                              bool is_originator);

static bool btm_sec_queue_encrypt_request(const RawAddress& bd_addr,
                                          tBT_TRANSPORT transport,
                                          tBTM_SEC_CALLBACK* p_callback,
                                          void* p_ref_data,
                                          tBTM_BLE_SEC_ACT sec_act);
static void btm_sec_check_pending_enc_req(tBTM_SEC_DEV_REC* p_dev_rec,
                                          tBT_TRANSPORT transport,
                                          uint8_t encr_enable);

static bool btm_sec_use_smp_br_chnl(tBTM_SEC_DEV_REC* p_dev_rec);
static bool btm_sec_is_master(tBTM_SEC_DEV_REC* p_dev_rec);

/* true - authenticated link key is possible */
static const bool btm_sec_io_map[BTM_IO_CAP_MAX][BTM_IO_CAP_MAX] = {
    /*   OUT,    IO,     IN,     NONE */
    /* OUT  */ {false, false, true, false},
    /* IO   */ {false, true, true, false},
    /* IN   */ {true, true, true, false},
    /* NONE */ {false, false, false, false}};
/*  BTM_IO_CAP_OUT      0   DisplayOnly */
/*  BTM_IO_CAP_IO       1   DisplayYesNo */
/*  BTM_IO_CAP_IN       2   KeyboardOnly */
/*  BTM_IO_CAP_NONE     3   NoInputNoOutput */

/*******************************************************************************
 *
 * Function         btm_dev_authenticated
 *
 * Description      check device is authenticated
 *
 * Returns          bool    true or false
 *
 ******************************************************************************/
static bool btm_dev_authenticated(tBTM_SEC_DEV_REC* p_dev_rec) {
  if (p_dev_rec->sec_flags & BTM_SEC_AUTHENTICATED) {
    return (true);
  }
  return (false);
}

/*******************************************************************************
 *
 * Function         btm_dev_encrypted
 *
 * Description      check device is encrypted
 *
 * Returns          bool    true or false
 *
 ******************************************************************************/
static bool btm_dev_encrypted(tBTM_SEC_DEV_REC* p_dev_rec) {
  if (p_dev_rec->sec_flags & BTM_SEC_ENCRYPTED) {
    return (true);
  }
  return (false);
}

/*******************************************************************************
 *
 * Function         btm_dev_authorized
 *
 * Description      check device is authorized
 *
 * Returns          bool    true or false
 *
 ******************************************************************************/
static bool btm_dev_authorized(tBTM_SEC_DEV_REC* p_dev_rec) {
  if (p_dev_rec->sec_flags & BTM_SEC_AUTHORIZED) {
    return (true);
  }
  return (false);
}

/*******************************************************************************
 *
 * Function         btm_dev_16_digit_authenticated
 *
 * Description      check device is authenticated by using 16 digit pin or MITM
 *
 * Returns          bool    true or false
 *
 ******************************************************************************/
static bool btm_dev_16_digit_authenticated(tBTM_SEC_DEV_REC* p_dev_rec) {
  // BTM_SEC_16_DIGIT_PIN_AUTHED is set if MITM or 16 digit pin is used
  if (p_dev_rec->sec_flags & BTM_SEC_16_DIGIT_PIN_AUTHED) {
    return (true);
  }
  return (false);
}

/*******************************************************************************
 *
 * Function         btm_serv_trusted
 *
 * Description      check service is trusted
 *
 * Returns          bool    true or false
 *
 ******************************************************************************/
static bool btm_serv_trusted(tBTM_SEC_DEV_REC* p_dev_rec,
                             tBTM_SEC_SERV_REC* p_serv_rec) {
  if (BTM_SEC_IS_SERVICE_TRUSTED(p_dev_rec->trusted_mask,
                                 p_serv_rec->service_id)) {
    return (true);
  }
  return (false);
}

/*******************************************************************************
 *
 * Function         BTM_SecRegister
 *
 * Description      Application manager calls this function to register for
 *                  security services.  There can be one and only one
 *                  application saving link keys.  BTM allows only first
 *                  registration.
 *
 * Returns          true if registered OK, else false
 *
 ******************************************************************************/
bool BTM_SecRegister(const tBTM_APPL_INFO* p_cb_info) {

  BTM_TRACE_EVENT("%s application registered", __func__);

  LOG_INFO(LOG_TAG, "%s p_cb_info->p_le_callback == 0x%p", __func__,
           p_cb_info->p_le_callback);
  if (p_cb_info->p_le_callback) {
    BTM_TRACE_EVENT("%s SMP_Register( btm_proc_smp_cback )", __func__);
    SMP_Register(btm_proc_smp_cback);
    Octet16 zero{0};
    /* if no IR is loaded, need to regenerate all the keys */
    if (btm_cb.devcb.id_keys.ir == zero) {
      btm_ble_reset_id();
    }
  } else {
    LOG_WARN(LOG_TAG, "%s p_cb_info->p_le_callback == NULL", __func__);
  }

  btm_cb.api = *p_cb_info;
  LOG_INFO(LOG_TAG, "%s btm_cb.api.p_le_callback = 0x%p ", __func__,
           btm_cb.api.p_le_callback);
  BTM_TRACE_EVENT("%s application registered", __func__);
  return (true);
}

/*******************************************************************************
 *
 * Function         BTM_SecRegisterLinkKeyNotificationCallback
 *
 * Description      Application manager calls this function to register for
 *                  link key notification.  When there is nobody registered
 *                  we should avoid changing link key
 *
 * Returns          true if registered OK, else false
 *
 ******************************************************************************/
bool BTM_SecRegisterLinkKeyNotificationCallback(
    tBTM_LINK_KEY_CALLBACK* p_callback) {
  btm_cb.api.p_link_key_callback = p_callback;
  return true;
}

/*******************************************************************************
 *
 * Function         BTM_SecAddRmtNameNotifyCallback
 *
 * Description      Any profile can register to be notified when name of the
 *                  remote device is resolved.
 *
 * Returns          true if registered OK, else false
 *
 ******************************************************************************/
bool BTM_SecAddRmtNameNotifyCallback(tBTM_RMT_NAME_CALLBACK* p_callback) {
  int i;

  for (i = 0; i < BTM_SEC_MAX_RMT_NAME_CALLBACKS; i++) {
    if (btm_cb.p_rmt_name_callback[i] == NULL) {
      btm_cb.p_rmt_name_callback[i] = p_callback;
      return (true);
    }
  }

  return (false);
}

/*******************************************************************************
 *
 * Function         BTM_SecDeleteRmtNameNotifyCallback
 *
 * Description      Any profile can deregister notification when a new Link Key
 *                  is generated per connection.
 *
 * Returns          true if OK, else false
 *
 ******************************************************************************/
bool BTM_SecDeleteRmtNameNotifyCallback(tBTM_RMT_NAME_CALLBACK* p_callback) {
  int i;

  for (i = 0; i < BTM_SEC_MAX_RMT_NAME_CALLBACKS; i++) {
    if (btm_cb.p_rmt_name_callback[i] == p_callback) {
      btm_cb.p_rmt_name_callback[i] = NULL;
      return (true);
    }
  }

  return (false);
}

/*******************************************************************************
 *
 * Function         BTM_GetSecurityFlags
 *
 * Description      Get security flags for the device
 *
 * Returns          bool    true or false is device found
 *
 ******************************************************************************/
bool BTM_GetSecurityFlags(const RawAddress& bd_addr, uint8_t* p_sec_flags) {
  tBTM_SEC_DEV_REC* p_dev_rec;

  p_dev_rec = btm_find_dev(bd_addr);
  if (p_dev_rec != NULL) {
    *p_sec_flags = (uint8_t)p_dev_rec->sec_flags;
    return (true);
  }
  BTM_TRACE_ERROR("BTM_GetSecurityFlags false");
  return (false);
}

/*******************************************************************************
 *
 * Function         BTM_GetSecurityFlagsByTransport
 *
 * Description      Get security flags for the device on a particular transport
 *
 * Returns          bool    true or false is device found
 *
 ******************************************************************************/
bool BTM_GetSecurityFlagsByTransport(const RawAddress& bd_addr,
                                     uint8_t* p_sec_flags,
                                     tBT_TRANSPORT transport) {
  tBTM_SEC_DEV_REC* p_dev_rec;

  p_dev_rec = btm_find_dev(bd_addr);
  if (p_dev_rec != NULL) {
    if (transport == BT_TRANSPORT_BR_EDR)
      *p_sec_flags = (uint8_t)p_dev_rec->sec_flags;
    else
      *p_sec_flags = (uint8_t)(p_dev_rec->sec_flags >> 8);

    return (true);
  }
  BTM_TRACE_ERROR("BTM_GetSecurityFlags false");
  return (false);
}

/*******************************************************************************
 *
 * Function         BTM_SetPinType
 *
 * Description      Set PIN type for the device.
 *
 * Returns          void
 *
 ******************************************************************************/
void BTM_SetPinType(uint8_t pin_type, PIN_CODE pin_code, uint8_t pin_code_len) {
  BTM_TRACE_API(
      "BTM_SetPinType: pin type %d [variable-0, fixed-1], code %s, length %d",
      pin_type, (char*)pin_code, pin_code_len);

  /* If device is not up security mode will be set as a part of startup */
  if ((btm_cb.cfg.pin_type != pin_type) &&
      controller_get_interface()->get_is_ready()) {
    btsnd_hcic_write_pin_type(pin_type);
  }

  btm_cb.cfg.pin_type = pin_type;
  btm_cb.cfg.pin_code_len = pin_code_len;
  memcpy(btm_cb.cfg.pin_code, pin_code, pin_code_len);
}

/*******************************************************************************
 *
 * Function         BTM_SetPairableMode
 *
 * Description      Enable or disable pairing
 *
 * Parameters       allow_pairing - (true or false) whether or not the device
 *                      allows pairing.
 *                  connect_only_paired - (true or false) whether or not to
 *                      only allow paired devices to connect.
 *
 * Returns          void
 *
 ******************************************************************************/
void BTM_SetPairableMode(bool allow_pairing, bool connect_only_paired) {
  BTM_TRACE_API(
      "BTM_SetPairableMode()  allow_pairing: %u   connect_only_paired: %u",
      allow_pairing, connect_only_paired);

  btm_cb.pairing_disabled = !allow_pairing;
  btm_cb.connect_only_paired = connect_only_paired;
}

/*******************************************************************************
 *
 * Function         BTM_SetSecureConnectionsOnly
 *
 * Description      Enable or disable default treatment for Mode 4 Level 0
 *                  services
 *
 * Parameter        secure_connections_only_mode -
 *                  true means that the device should treat Mode 4 Level 0
 *                       services as services of other levels.
 *                  false means that the device should provide default
 *                        treatment for Mode 4 Level 0 services.
 *
 * Returns          void
 *
 ******************************************************************************/
void BTM_SetSecureConnectionsOnly(bool secure_connections_only_mode) {
  BTM_TRACE_API("%s: Mode : %u", __func__, secure_connections_only_mode);

  btm_cb.devcb.secure_connections_only = secure_connections_only_mode;
  btm_cb.security_mode = BTM_SEC_MODE_SC;
}
#define BTM_NO_AVAIL_SEC_SERVICES ((uint16_t)0xffff)

/*******************************************************************************
 *
 * Function         BTM_SetSecurityLevel
 *
 * Description      Register service security level with Security Manager
 *
 * Parameters:      is_originator - true if originating the connection
 *                  p_name      - Name of the service relevant only if
 *                                authorization will show this name to user.
 *                                Ignored if BTM_SEC_SERVICE_NAME_LEN is 0.
 *                  service_id  - service ID for the service passed to
 *                                authorization callback
 *                  sec_level   - bit mask of the security features
 *                  psm         - L2CAP PSM
 *                  mx_proto_id - protocol ID of multiplexing proto below
 *                  mx_chan_id  - channel ID of multiplexing proto below
 *
 * Returns          true if registered OK, else false
 *
 ******************************************************************************/
bool BTM_SetSecurityLevel(bool is_originator, const char* p_name,
                          uint8_t service_id, uint16_t sec_level, uint16_t psm,
                          uint32_t mx_proto_id, uint32_t mx_chan_id) {
  return (btm_sec_set_security_level(is_originator, p_name, service_id,
                                     sec_level, psm, mx_proto_id, mx_chan_id));
}

/*******************************************************************************
 *
 * Function         btm_sec_set_security_level
 *
 * Description      Register service security level with Security Manager
 *
 * Parameters:      conn_type   - true if originating the connection
 *                  p_name      - Name of the service relevant only if
 *                                authorization will show this name to user.
 *                                Ignored if BTM_SEC_SERVICE_NAME_LEN is 0.
 *                  service_id  - service ID for the service passed to
 *                                authorization callback
 *                  sec_level   - bit mask of the security features
 *                  psm         - L2CAP PSM
 *                  mx_proto_id - protocol ID of multiplexing proto below
 *                  mx_chan_id  - channel ID of multiplexing proto below
 *
 * Returns          true if registered OK, else false
 *
 ******************************************************************************/
static bool btm_sec_set_security_level(CONNECTION_TYPE conn_type,
                                       const char* p_name, uint8_t service_id,
                                       uint16_t sec_level, uint16_t psm,
                                       uint32_t mx_proto_id,
                                       uint32_t mx_chan_id) {
  tBTM_SEC_SERV_REC* p_srec;
  uint16_t index;
  uint16_t first_unused_record = BTM_NO_AVAIL_SEC_SERVICES;
  bool record_allocated = false;
  bool is_originator;
  is_originator = conn_type;

  BTM_TRACE_API("%s : sec: 0x%x", __func__, sec_level);

  /* See if the record can be reused (same service name, psm, mx_proto_id,
     service_id, and mx_chan_id), or obtain the next unused record */

  p_srec = &btm_cb.sec_serv_rec[0];

  for (index = 0; index < BTM_SEC_MAX_SERVICE_RECORDS; index++, p_srec++) {
    /* Check if there is already a record for this service */
    if (p_srec->security_flags & BTM_SEC_IN_USE) {
#if BTM_SEC_SERVICE_NAME_LEN > 0
      if (p_srec->psm == psm && p_srec->mx_proto_id == mx_proto_id &&
          service_id == p_srec->service_id && p_name &&
          (!strncmp(p_name, (char*)p_srec->orig_service_name,
                    /* strlcpy replaces end char with termination char*/
                    BTM_SEC_SERVICE_NAME_LEN - 1) ||
           !strncmp(p_name, (char*)p_srec->term_service_name,
                    /* strlcpy replaces end char with termination char*/
                    BTM_SEC_SERVICE_NAME_LEN - 1)))
#else
      if (p_srec->psm == psm && p_srec->mx_proto_id == mx_proto_id &&
          service_id == p_srec->service_id)
#endif
      {
        record_allocated = true;
        break;
      }
    }
    /* Mark the first available service record */
    else if (!record_allocated) {
      memset(p_srec, 0, sizeof(tBTM_SEC_SERV_REC));
      record_allocated = true;
      first_unused_record = index;
    }
  }

  if (!record_allocated) {
    BTM_TRACE_WARNING("BTM_SEC_REG: Out of Service Records (%d)",
                      BTM_SEC_MAX_SERVICE_RECORDS);
    return (record_allocated);
  }

  /* Process the request if service record is valid */
  /* If a duplicate service wasn't found, use the first available */
  if (index >= BTM_SEC_MAX_SERVICE_RECORDS) {
    index = first_unused_record;
    p_srec = &btm_cb.sec_serv_rec[index];
  }

  p_srec->psm = psm;
  p_srec->service_id = service_id;
  p_srec->mx_proto_id = mx_proto_id;

  if (is_originator) {
    p_srec->orig_mx_chan_id = mx_chan_id;
#if BTM_SEC_SERVICE_NAME_LEN > 0
    strlcpy((char*)p_srec->orig_service_name, p_name,
            BTM_SEC_SERVICE_NAME_LEN + 1);
#endif
/* clear out the old setting, just in case it exists */
    {
      p_srec->security_flags &= ~(
          BTM_SEC_OUT_AUTHORIZE | BTM_SEC_OUT_ENCRYPT |
          BTM_SEC_OUT_AUTHENTICATE | BTM_SEC_OUT_MITM | BTM_SEC_FORCE_MASTER |
          BTM_SEC_ATTEMPT_MASTER | BTM_SEC_FORCE_SLAVE | BTM_SEC_ATTEMPT_SLAVE);
    }

    /* Parameter validation.  Originator should not set requirements for
     * incoming connections */
    sec_level &=
        ~(BTM_SEC_IN_AUTHORIZE | BTM_SEC_IN_ENCRYPT | BTM_SEC_IN_AUTHENTICATE |
          BTM_SEC_IN_MITM | BTM_SEC_IN_MIN_16_DIGIT_PIN);

    if (btm_cb.security_mode == BTM_SEC_MODE_SP ||
        btm_cb.security_mode == BTM_SEC_MODE_SP_DEBUG ||
        btm_cb.security_mode == BTM_SEC_MODE_SC) {
      if (sec_level & BTM_SEC_OUT_AUTHENTICATE) sec_level |= BTM_SEC_OUT_MITM;
    }

    /* Make sure the authenticate bit is set, when encrypt bit is set */
    if (sec_level & BTM_SEC_OUT_ENCRYPT) sec_level |= BTM_SEC_OUT_AUTHENTICATE;

/* outgoing connections usually set the security level right before
 * the connection is initiated.
 * set it to be the outgoing service */
      btm_cb.p_out_serv = p_srec;
  } else {
    p_srec->term_mx_chan_id = mx_chan_id;
#if BTM_SEC_SERVICE_NAME_LEN > 0
    strlcpy((char*)p_srec->term_service_name, p_name,
            BTM_SEC_SERVICE_NAME_LEN + 1);
#endif
/* clear out the old setting, just in case it exists */
    {
      p_srec->security_flags &=
          ~(BTM_SEC_IN_AUTHORIZE | BTM_SEC_IN_ENCRYPT |
            BTM_SEC_IN_AUTHENTICATE | BTM_SEC_IN_MITM | BTM_SEC_FORCE_MASTER |
            BTM_SEC_ATTEMPT_MASTER | BTM_SEC_FORCE_SLAVE |
            BTM_SEC_ATTEMPT_SLAVE | BTM_SEC_IN_MIN_16_DIGIT_PIN);
    }

    /* Parameter validation.  Acceptor should not set requirements for outgoing
     * connections */
    sec_level &= ~(BTM_SEC_OUT_AUTHORIZE | BTM_SEC_OUT_ENCRYPT |
                   BTM_SEC_OUT_AUTHENTICATE | BTM_SEC_OUT_MITM);

    if (btm_cb.security_mode == BTM_SEC_MODE_SP ||
        btm_cb.security_mode == BTM_SEC_MODE_SP_DEBUG ||
        btm_cb.security_mode == BTM_SEC_MODE_SC) {
      if (sec_level & BTM_SEC_IN_AUTHENTICATE) sec_level |= BTM_SEC_IN_MITM;
    }

    /* Make sure the authenticate bit is set, when encrypt bit is set */
    if (sec_level & BTM_SEC_IN_ENCRYPT) sec_level |= BTM_SEC_IN_AUTHENTICATE;
  }

  p_srec->security_flags |= (uint16_t)(sec_level | BTM_SEC_IN_USE);

  BTM_TRACE_API(
      "BTM_SEC_REG[%d]: id %d, is_orig %d, psm 0x%04x, proto_id %d, chan_id %d",
      index, service_id, is_originator, psm, mx_proto_id, mx_chan_id);

#if BTM_SEC_SERVICE_NAME_LEN > 0
  BTM_TRACE_API(
      "               : sec: 0x%x, service name [%s] (up to %d chars saved)",
      p_srec->security_flags, p_name, BTM_SEC_SERVICE_NAME_LEN);
#endif

  return (record_allocated);
}

/*******************************************************************************
 *
 * Function         BTM_SecClrService
 *
 * Description      Removes specified service record(s) from the security
 *                  database. All service records with the specified name are
 *                  removed. Typically used only by devices with limited RAM so
 *                  that it can reuse an old security service record.
 *
 *                  Note: Unpredictable results may occur if a service is
 *                      cleared that is still in use by an application/profile.
 *
 * Parameters       Service ID - Id of the service to remove. '0' removes all
 *                          service records (except SDP).
 *
 * Returns          Number of records that were freed.
 *
 ******************************************************************************/
uint8_t BTM_SecClrService(uint8_t service_id) {
  tBTM_SEC_SERV_REC* p_srec = &btm_cb.sec_serv_rec[0];
  uint8_t num_freed = 0;
  int i;

  for (i = 0; i < BTM_SEC_MAX_SERVICE_RECORDS; i++, p_srec++) {
    /* Delete services with specified name (if in use and not SDP) */
    if ((p_srec->security_flags & BTM_SEC_IN_USE) &&
        (p_srec->psm != BT_PSM_SDP) &&
        (!service_id || (service_id == p_srec->service_id))) {
      BTM_TRACE_API("BTM_SEC_CLR[%d]: id %d", i, service_id);
      p_srec->security_flags = 0;
      num_freed++;
    }
  }

  return (num_freed);
}

/*******************************************************************************
 *
 * Function         btm_sec_clr_service_by_psm
 *
 * Description      Removes specified service record from the security database.
 *                  All service records with the specified psm are removed.
 *                  Typically used by L2CAP to free up the service record used
 *                  by dynamic PSM clients when the channel is closed.
 *                  The given psm must be a virtual psm.
 *
 * Parameters       Service ID - Id of the service to remove. '0' removes all
 *                          service records (except SDP).
 *
 * Returns          Number of records that were freed.
 *
 ******************************************************************************/
uint8_t btm_sec_clr_service_by_psm(uint16_t psm) {
  tBTM_SEC_SERV_REC* p_srec = &btm_cb.sec_serv_rec[0];
  uint8_t num_freed = 0;
  int i;

  for (i = 0; i < BTM_SEC_MAX_SERVICE_RECORDS; i++, p_srec++) {
    /* Delete services with specified name (if in use and not SDP) */
    if ((p_srec->security_flags & BTM_SEC_IN_USE) && (p_srec->psm == psm)) {
      BTM_TRACE_API("BTM_SEC_CLR[%d]: id %d ", i, p_srec->service_id);
      p_srec->security_flags = 0;
      num_freed++;
    }
  }
  BTM_TRACE_API("btm_sec_clr_service_by_psm psm:0x%x num_freed:%d", psm,
                num_freed);

  return (num_freed);
}

/*******************************************************************************
 *
 * Function         btm_sec_clr_temp_auth_service
 *
 * Description      Removes specified device record's temporary authorization
 *                  flag from the security database.
 *
 * Parameters       Device address to be cleared
 *
 * Returns          void.
 *
 ******************************************************************************/
void btm_sec_clr_temp_auth_service(const RawAddress& bda) {
  tBTM_SEC_DEV_REC* p_dev_rec;

  p_dev_rec = btm_find_dev(bda);
  if (p_dev_rec == NULL) {
    BTM_TRACE_WARNING("btm_sec_clr_temp_auth_service() - no dev CB");
    return;
  }

  /* Reset the temporary authorized flag so that next time (untrusted) service
   * is accessed autorization will take place */
  if (p_dev_rec->last_author_service_id != BTM_SEC_NO_LAST_SERVICE_ID &&
      p_dev_rec->p_cur_service) {
    VLOG(1) << __func__ << " clearing device: " << bda;

    p_dev_rec->last_author_service_id = BTM_SEC_NO_LAST_SERVICE_ID;
  }
}

/*******************************************************************************
 *
 * Function         BTM_PINCodeReply
 *
 * Description      This function is called after Security Manager submitted
 *                  PIN code request to the UI.
 *
 * Parameters:      bd_addr      - Address of the device for which PIN was
 *                                 requested
 *                  res          - result of the operation BTM_SUCCESS
 *                                 if success
 *                  pin_len      - length in bytes of the PIN Code
 *                  p_pin        - pointer to array with the PIN Code
 *                  trusted_mask - bitwise OR of trusted services
 *                                 (array of uint32_t)
 *
 ******************************************************************************/
void BTM_PINCodeReply(const RawAddress& bd_addr, uint8_t res, uint8_t pin_len,
                      uint8_t* p_pin, uint32_t trusted_mask[]) {
  tBTM_SEC_DEV_REC* p_dev_rec;

  BTM_TRACE_API(
      "BTM_PINCodeReply(): PairState: %s   PairFlags: 0x%02x  PinLen:%d  "
      "Result:%d",
      btm_pair_state_descr(btm_cb.pairing_state), btm_cb.pairing_flags, pin_len,
      res);

  /* If timeout already expired or has been canceled, ignore the reply */
  if (btm_cb.pairing_state != BTM_PAIR_STATE_WAIT_LOCAL_PIN) {
    BTM_TRACE_WARNING("BTM_PINCodeReply() - Wrong State: %d",
                      btm_cb.pairing_state);
    return;
  }

  if (bd_addr != btm_cb.pairing_bda) {
    BTM_TRACE_ERROR("BTM_PINCodeReply() - Wrong BD Addr");
    return;
  }

  p_dev_rec = btm_find_dev(bd_addr);
  if (p_dev_rec == NULL) {
    BTM_TRACE_ERROR("BTM_PINCodeReply() - no dev CB");
    return;
  }

  if ((pin_len > PIN_CODE_LEN) || (pin_len == 0) || (p_pin == NULL))
    res = BTM_ILLEGAL_VALUE;

  if (res != BTM_SUCCESS) {
    /* if peer started dd OR we started dd and pre-fetch pin was not used send
     * negative reply */
    if ((btm_cb.pairing_flags & BTM_PAIR_FLAGS_PEER_STARTED_DD) ||
        ((btm_cb.pairing_flags & BTM_PAIR_FLAGS_WE_STARTED_DD) &&
         (btm_cb.pairing_flags & BTM_PAIR_FLAGS_DISC_WHEN_DONE))) {
      /* use BTM_PAIR_STATE_WAIT_AUTH_COMPLETE to report authentication failed
       * event */
      btm_sec_change_pairing_state(BTM_PAIR_STATE_WAIT_AUTH_COMPLETE);
      btm_cb.acl_disc_reason = HCI_ERR_HOST_REJECT_SECURITY;

      btsnd_hcic_pin_code_neg_reply(bd_addr);
    } else {
      p_dev_rec->security_required = BTM_SEC_NONE;
      btm_sec_change_pairing_state(BTM_PAIR_STATE_IDLE);
    }
    return;
  }
  if (trusted_mask)
    BTM_SEC_COPY_TRUSTED_DEVICE(trusted_mask, p_dev_rec->trusted_mask);
  p_dev_rec->sec_flags |= BTM_SEC_LINK_KEY_AUTHED;
  p_dev_rec->pin_code_length = pin_len;
  if (pin_len >= 16) {
    p_dev_rec->sec_flags |= BTM_SEC_16_DIGIT_PIN_AUTHED;
  }

  if ((btm_cb.pairing_flags & BTM_PAIR_FLAGS_WE_STARTED_DD) &&
      (p_dev_rec->hci_handle == BTM_SEC_INVALID_HANDLE) &&
      (!btm_cb.security_mode_changed)) {
    /* This is start of the dedicated bonding if local device is 2.0 */
    btm_cb.pin_code_len = pin_len;
    memcpy(btm_cb.pin_code, p_pin, pin_len);

    btm_cb.security_mode_changed = true;
#ifdef APPL_AUTH_WRITE_EXCEPTION
    if (!(APPL_AUTH_WRITE_EXCEPTION)(p_dev_rec->bd_addr))
#endif
      btsnd_hcic_write_auth_enable(true);

    btm_cb.acl_disc_reason = 0xff;

    /* if we rejected incoming connection request, we have to wait
     * HCI_Connection_Complete event */
    /*  before originating  */
    if (btm_cb.pairing_flags & BTM_PAIR_FLAGS_REJECTED_CONNECT) {
      BTM_TRACE_WARNING(
          "BTM_PINCodeReply(): waiting HCI_Connection_Complete after rejected "
          "incoming connection");
      /* we change state little bit early so btm_sec_connected() will originate
       * connection */
      /*   when existing ACL link is down completely */
      btm_sec_change_pairing_state(BTM_PAIR_STATE_WAIT_PIN_REQ);
    }
    /* if we already accepted incoming connection from pairing device */
    else if (p_dev_rec->sm4 & BTM_SM4_CONN_PEND) {
      BTM_TRACE_WARNING(
          "BTM_PINCodeReply(): link is connecting so wait pin code request "
          "from peer");
      btm_sec_change_pairing_state(BTM_PAIR_STATE_WAIT_PIN_REQ);
    } else if (btm_sec_dd_create_conn(p_dev_rec) != BTM_CMD_STARTED) {
      btm_sec_change_pairing_state(BTM_PAIR_STATE_IDLE);
      p_dev_rec->sec_flags &= ~BTM_SEC_LINK_KEY_AUTHED;

      if (btm_cb.api.p_auth_complete_callback)
        (*btm_cb.api.p_auth_complete_callback)(
            p_dev_rec->bd_addr, p_dev_rec->dev_class, p_dev_rec->sec_bd_name,
            HCI_ERR_AUTH_FAILURE);
    }
    return;
  }

  btm_sec_change_pairing_state(BTM_PAIR_STATE_WAIT_AUTH_COMPLETE);
  btm_cb.acl_disc_reason = HCI_SUCCESS;

  btsnd_hcic_pin_code_req_reply(bd_addr, pin_len, p_pin);
}

/*******************************************************************************
 *
 * Function         btm_sec_bond_by_transport
 *
 * Description      this is the bond function that will start either SSP or SMP.
 *
 * Parameters:      bd_addr      - Address of the device to bond
 *                  pin_len      - length in bytes of the PIN Code
 *                  p_pin        - pointer to array with the PIN Code
 *                  trusted_mask - bitwise OR of trusted services
 *                                 (array of uint32_t)
 *
 *  Note: After 2.1 parameters are not used and preserved here not to change API
 ******************************************************************************/
tBTM_STATUS btm_sec_bond_by_transport(const RawAddress& bd_addr,
                                      tBT_TRANSPORT transport, uint8_t pin_len,
                                      uint8_t* p_pin, uint32_t trusted_mask[]) {
  tBTM_SEC_DEV_REC* p_dev_rec;
  tBTM_STATUS status;
  uint8_t* p_features;
  uint8_t ii;
  tACL_CONN* p = btm_bda_to_acl(bd_addr, transport);
  VLOG(1) << __func__ << " BDA: " << bd_addr;

  BTM_TRACE_DEBUG("%s: Transport used %d, bd_addr=%s", __func__, transport,
                  bd_addr.ToString().c_str());

  /* Other security process is in progress */
  if (btm_cb.pairing_state != BTM_PAIR_STATE_IDLE) {
    BTM_TRACE_ERROR("BTM_SecBond: already busy in state: %s",
                    btm_pair_state_descr(btm_cb.pairing_state));
    return (BTM_WRONG_MODE);
  }

  p_dev_rec = btm_find_or_alloc_dev(bd_addr);
  if (p_dev_rec == NULL) {
    return (BTM_NO_RESOURCES);
  }

  if (!controller_get_interface()->get_is_ready()) {
    BTM_TRACE_ERROR("%s controller module is not ready", __func__);
    return (BTM_NO_RESOURCES);
  }

  BTM_TRACE_DEBUG("before update sec_flags=0x%x", p_dev_rec->sec_flags);

  /* Finished if connection is active and already paired */
  if (((p_dev_rec->hci_handle != BTM_SEC_INVALID_HANDLE) &&
       transport == BT_TRANSPORT_BR_EDR &&
       (p_dev_rec->sec_flags & BTM_SEC_AUTHENTICATED)) ||
      ((p_dev_rec->ble_hci_handle != BTM_SEC_INVALID_HANDLE) &&
       transport == BT_TRANSPORT_LE &&
       (p_dev_rec->sec_flags & BTM_SEC_LE_AUTHENTICATED))) {
    BTM_TRACE_WARNING("BTM_SecBond -> Already Paired");
    return (BTM_SUCCESS);
  }

  /* Tell controller to get rid of the link key if it has one stored */
  if ((BTM_DeleteStoredLinkKey(&bd_addr, NULL)) != BTM_SUCCESS)
    return (BTM_NO_RESOURCES);

  /* Save the PIN code if we got a valid one */
  if (p_pin && (pin_len <= PIN_CODE_LEN) && (pin_len != 0)) {
    btm_cb.pin_code_len = pin_len;
    p_dev_rec->pin_code_length = pin_len;
    memcpy(btm_cb.pin_code, p_pin, PIN_CODE_LEN);
  }

  btm_cb.pairing_bda = bd_addr;

  btm_cb.pairing_flags = BTM_PAIR_FLAGS_WE_STARTED_DD;

  p_dev_rec->security_required = BTM_SEC_OUT_AUTHENTICATE;
  p_dev_rec->is_originator = true;
  if (trusted_mask)
    BTM_SEC_COPY_TRUSTED_DEVICE(trusted_mask, p_dev_rec->trusted_mask);

  if (transport == BT_TRANSPORT_LE) {
    btm_ble_init_pseudo_addr(p_dev_rec, bd_addr);
    p_dev_rec->sec_flags &= ~BTM_SEC_LE_MASK;

    if (SMP_Pair(bd_addr) == SMP_STARTED) {
      btm_cb.pairing_flags |= BTM_PAIR_FLAGS_LE_ACTIVE;
      p_dev_rec->sec_state = BTM_SEC_STATE_AUTHENTICATING;
      btm_sec_change_pairing_state(BTM_PAIR_STATE_WAIT_AUTH_COMPLETE);
      return BTM_CMD_STARTED;
    }

    btm_cb.pairing_flags = 0;
    return (BTM_NO_RESOURCES);
  }

  p_dev_rec->sec_flags &=
      ~(BTM_SEC_LINK_KEY_KNOWN | BTM_SEC_AUTHENTICATED | BTM_SEC_ENCRYPTED |
        BTM_SEC_ROLE_SWITCHED | BTM_SEC_LINK_KEY_AUTHED);

  BTM_TRACE_DEBUG("after update sec_flags=0x%x", p_dev_rec->sec_flags);
  if (!controller_get_interface()->supports_simple_pairing()) {
    /* The special case when we authenticate keyboard.  Set pin type to fixed */
    /* It would be probably better to do it from the application, but it is */
    /* complicated */
    if (((p_dev_rec->dev_class[1] & BTM_COD_MAJOR_CLASS_MASK) ==
         BTM_COD_MAJOR_PERIPHERAL) &&
        (p_dev_rec->dev_class[2] & BTM_COD_MINOR_KEYBOARD) &&
        (btm_cb.cfg.pin_type != HCI_PIN_TYPE_FIXED)) {
      btm_cb.pin_type_changed = true;
      btsnd_hcic_write_pin_type(HCI_PIN_TYPE_FIXED);
    }
  }

  for (ii = 0; ii <= HCI_EXT_FEATURES_PAGE_MAX; ii++) {
    p_features = p_dev_rec->feature_pages[ii];
    BTM_TRACE_EVENT("  remote_features page[%1d] = %02x-%02x-%02x-%02x", ii,
                    p_features[0], p_features[1], p_features[2], p_features[3]);
    BTM_TRACE_EVENT("                              %02x-%02x-%02x-%02x",
                    p_features[4], p_features[5], p_features[6], p_features[7]);
  }

  BTM_TRACE_EVENT("BTM_SecBond: Remote sm4: 0x%x  HCI Handle: 0x%04x",
                  p_dev_rec->sm4, p_dev_rec->hci_handle);

#if (BTM_SEC_FORCE_RNR_FOR_DBOND == TRUE)
  p_dev_rec->sec_flags &= ~BTM_SEC_NAME_KNOWN;
#endif

  /* If connection already exists... */
  if (p && p->hci_handle != BTM_SEC_INVALID_HANDLE) {
    btm_sec_start_authentication(p_dev_rec);

    btm_sec_change_pairing_state(BTM_PAIR_STATE_WAIT_PIN_REQ);

    /* Mark lcb as bonding */
    l2cu_update_lcb_4_bonding(bd_addr, true);
    return (BTM_CMD_STARTED);
  }

  BTM_TRACE_DEBUG("sec mode: %d sm4:x%x", btm_cb.security_mode, p_dev_rec->sm4);
  if (!controller_get_interface()->supports_simple_pairing() ||
      (p_dev_rec->sm4 == BTM_SM4_KNOWN)) {
    if (btm_sec_check_prefetch_pin(p_dev_rec)) return (BTM_CMD_STARTED);
  }
  if ((btm_cb.security_mode == BTM_SEC_MODE_SP ||
       btm_cb.security_mode == BTM_SEC_MODE_SP_DEBUG ||
       btm_cb.security_mode == BTM_SEC_MODE_SC) &&
      BTM_SEC_IS_SM4_UNKNOWN(p_dev_rec->sm4)) {
    /* local is 2.1 and peer is unknown */
    if ((p_dev_rec->sm4 & BTM_SM4_CONN_PEND) == 0) {
      /* we are not accepting connection request from peer
       * -> RNR (to learn if peer is 2.1)
       * RNR when no ACL causes HCI_RMT_HOST_SUP_FEAT_NOTIFY_EVT */
      btm_sec_change_pairing_state(BTM_PAIR_STATE_GET_REM_NAME);
      status = BTM_ReadRemoteDeviceName(bd_addr, NULL, BT_TRANSPORT_BR_EDR);
    } else {
      /* We are accepting connection request from peer */
      btm_sec_change_pairing_state(BTM_PAIR_STATE_WAIT_PIN_REQ);
      status = BTM_CMD_STARTED;
    }
    BTM_TRACE_DEBUG("State:%s sm4: 0x%x sec_state:%d",
                    btm_pair_state_descr(btm_cb.pairing_state), p_dev_rec->sm4,
                    p_dev_rec->sec_state);
  } else {
    /* both local and peer are 2.1  */
    status = btm_sec_dd_create_conn(p_dev_rec);
  }

  if (status != BTM_CMD_STARTED) {
    BTM_TRACE_ERROR(
        "%s BTM_ReadRemoteDeviceName or btm_sec_dd_create_conn error: 0x%x",
        __func__, (int)status);
    btm_sec_change_pairing_state(BTM_PAIR_STATE_IDLE);
  }

  return status;
}

/*******************************************************************************
 *
 * Function         BTM_SecBondByTransport
 *
 * Description      This function is called to perform bonding with peer device.
 *                  If the connection is already up, but not secure, pairing
 *                  is attempted.  If already paired BTM_SUCCESS is returned.
 *
 * Parameters:      bd_addr      - Address of the device to bond
 *                  transport    - doing SSP over BR/EDR or SMP over LE
 *                  pin_len      - length in bytes of the PIN Code
 *                  p_pin        - pointer to array with the PIN Code
 *                  trusted_mask - bitwise OR of trusted services
 *                                 (array of uint32_t)
 *
 *  Note: After 2.1 parameters are not used and preserved here not to change API
 ******************************************************************************/
tBTM_STATUS BTM_SecBondByTransport(const RawAddress& bd_addr,
                                   tBT_TRANSPORT transport, uint8_t pin_len,
                                   uint8_t* p_pin, uint32_t trusted_mask[]) {
  tBT_DEVICE_TYPE dev_type;
  tBLE_ADDR_TYPE addr_type;

  BTM_ReadDevInfo(bd_addr, &dev_type, &addr_type);
  /* LE device, do SMP pairing */
  if ((transport == BT_TRANSPORT_LE && (dev_type & BT_DEVICE_TYPE_BLE) == 0) ||
      (transport == BT_TRANSPORT_BR_EDR &&
       (dev_type & BT_DEVICE_TYPE_BREDR) == 0)) {
    return BTM_ILLEGAL_ACTION;
  }
  return btm_sec_bond_by_transport(bd_addr, transport, pin_len, p_pin,
                                   trusted_mask);
}

/*******************************************************************************
 *
 * Function         BTM_SecBond
 *
 * Description      This function is called to perform bonding with peer device.
 *                  If the connection is already up, but not secure, pairing
 *                  is attempted.  If already paired BTM_SUCCESS is returned.
 *
 * Parameters:      bd_addr      - Address of the device to bond
 *                  pin_len      - length in bytes of the PIN Code
 *                  p_pin        - pointer to array with the PIN Code
 *                  trusted_mask - bitwise OR of trusted services
 *                                 (array of uint32_t)
 *
 *  Note: After 2.1 parameters are not used and preserved here not to change API
 ******************************************************************************/
tBTM_STATUS BTM_SecBond(const RawAddress& bd_addr, uint8_t pin_len,
                        uint8_t* p_pin, uint32_t trusted_mask[]) {
  tBT_TRANSPORT transport = BT_TRANSPORT_BR_EDR;
  if (BTM_UseLeLink(bd_addr)) transport = BT_TRANSPORT_LE;
  return btm_sec_bond_by_transport(bd_addr, transport, pin_len, p_pin,
                                   trusted_mask);
}
/*******************************************************************************
 *
 * Function         BTM_SecBondCancel
 *
 * Description      This function is called to cancel ongoing bonding process
 *                  with peer device.
 *
 * Parameters:      bd_addr      - Address of the peer device
 *                  transport    - false for BR/EDR link; true for LE link
 *
 ******************************************************************************/
tBTM_STATUS BTM_SecBondCancel(const RawAddress& bd_addr) {
  tBTM_SEC_DEV_REC* p_dev_rec;

  BTM_TRACE_API("BTM_SecBondCancel()  State: %s flags:0x%x",
                btm_pair_state_descr(btm_cb.pairing_state),
                btm_cb.pairing_flags);
  p_dev_rec = btm_find_dev(bd_addr);
  if (!p_dev_rec || btm_cb.pairing_bda != bd_addr) {
    return BTM_UNKNOWN_ADDR;
  }

  if (btm_cb.pairing_flags & BTM_PAIR_FLAGS_LE_ACTIVE) {
    if (p_dev_rec->sec_state == BTM_SEC_STATE_AUTHENTICATING) {
      BTM_TRACE_DEBUG("Cancel LE pairing");
      if (SMP_PairCancel(bd_addr)) {
        return BTM_CMD_STARTED;
      }
    }
    return BTM_WRONG_MODE;
  }

  BTM_TRACE_DEBUG("hci_handle:0x%x sec_state:%d", p_dev_rec->hci_handle,
                  p_dev_rec->sec_state);
  if (BTM_PAIR_STATE_WAIT_LOCAL_PIN == btm_cb.pairing_state &&
      BTM_PAIR_FLAGS_WE_STARTED_DD & btm_cb.pairing_flags) {
    /* pre-fetching pin for dedicated bonding */
    btm_sec_bond_cancel_complete();
    return BTM_SUCCESS;
  }

  /* If this BDA is in a bonding procedure */
  if ((btm_cb.pairing_state != BTM_PAIR_STATE_IDLE) &&
      (btm_cb.pairing_flags & BTM_PAIR_FLAGS_WE_STARTED_DD)) {
    /* If the HCI link is up */
    if (p_dev_rec->hci_handle != BTM_SEC_INVALID_HANDLE) {
      /* If some other thread disconnecting, we do not send second command */
      if ((p_dev_rec->sec_state == BTM_SEC_STATE_DISCONNECTING) ||
          (p_dev_rec->sec_state == BTM_SEC_STATE_DISCONNECTING_BOTH))
        return (BTM_CMD_STARTED);

      /* If the HCI link was set up by Bonding process */
      if (btm_cb.pairing_flags & BTM_PAIR_FLAGS_DISC_WHEN_DONE)
        return btm_sec_send_hci_disconnect(p_dev_rec, HCI_ERR_PEER_USER,
                                           p_dev_rec->hci_handle);
      else
        l2cu_update_lcb_4_bonding(bd_addr, false);

      return BTM_NOT_AUTHORIZED;
    } else /*HCI link is not up */
    {
      /* If the HCI link creation was started by Bonding process */
      if (btm_cb.pairing_flags & BTM_PAIR_FLAGS_DISC_WHEN_DONE) {
        btsnd_hcic_create_conn_cancel(bd_addr);
        return BTM_CMD_STARTED;
      }
      if (btm_cb.pairing_state == BTM_PAIR_STATE_GET_REM_NAME) {
        BTM_CancelRemoteDeviceName();
        btm_cb.pairing_flags |= BTM_PAIR_FLAGS_WE_CANCEL_DD;
        return BTM_CMD_STARTED;
      }
      return BTM_NOT_AUTHORIZED;
    }
  }

  return BTM_WRONG_MODE;
}

/*******************************************************************************
 *
 * Function         BTM_SecGetDeviceLinkKey
 *
 * Description      This function is called to obtain link key for the device
 *                  it returns BTM_SUCCESS if link key is available, or
 *                  BTM_UNKNOWN_ADDR if Security Manager does not know about
 *                  the device or device record does not contain link key info
 *
 * Parameters:      bd_addr      - Address of the device
 *                  link_key     - Link Key is copied into this pointer
 *
 ******************************************************************************/
tBTM_STATUS BTM_SecGetDeviceLinkKey(const RawAddress& bd_addr,
                                    LinkKey* link_key) {
  tBTM_SEC_DEV_REC* p_dev_rec;
  p_dev_rec = btm_find_dev(bd_addr);
  if ((p_dev_rec != NULL) && (p_dev_rec->sec_flags & BTM_SEC_LINK_KEY_KNOWN)) {
    *link_key = p_dev_rec->link_key;
    return (BTM_SUCCESS);
  }
  return (BTM_UNKNOWN_ADDR);
}

/*******************************************************************************
 *
 * Function         BTM_SecGetDeviceLinkKeyType
 *
 * Description      This function is called to obtain link key type for the
 *                  device.
 *                  it returns BTM_SUCCESS if link key is available, or
 *                  BTM_UNKNOWN_ADDR if Security Manager does not know about
 *                  the device or device record does not contain link key info
 *
 * Returns          BTM_LKEY_TYPE_IGNORE if link key is unknown, link type
 *                  otherwise.
 *
 ******************************************************************************/
tBTM_LINK_KEY_TYPE BTM_SecGetDeviceLinkKeyType(const RawAddress& bd_addr) {
  tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev(bd_addr);

  if ((p_dev_rec != NULL) && (p_dev_rec->sec_flags & BTM_SEC_LINK_KEY_KNOWN)) {
    return p_dev_rec->link_key_type;
  }
  return BTM_LKEY_TYPE_IGNORE;
}

/*******************************************************************************
 *
 * Function         BTM_SetEncryption
 *
 * Description      This function is called to ensure that connection is
 *                  encrypted.  Should be called only on an open connection.
 *                  Typically only needed for connections that first want to
 *                  bring up unencrypted links, then later encrypt them.
 *
 * Parameters:      bd_addr       - Address of the peer device
 *                  transport     - Link transport
 *                  p_callback    - Pointer to callback function called if
 *                                  this function returns PENDING after required
 *                                  procedures are completed.  Can be set to
 *                                  NULL if status is not desired.
 *                  p_ref_data    - pointer to any data the caller wishes to
 *                                  receive in the callback function upon
 *                                  completion. can be set to NULL if not used.
 *                  sec_act       - LE security action, unused for BR/EDR
 *
 * Returns          BTM_SUCCESS   - already encrypted
 *                  BTM_PENDING   - command will be returned in the callback
 *                  BTM_WRONG_MODE- connection not up.
 *                  BTM_BUSY      - security procedures are currently active
 *                  BTM_MODE_UNSUPPORTED - if security manager not linked in.
 *
 ******************************************************************************/
tBTM_STATUS BTM_SetEncryption(const RawAddress& bd_addr,
                              tBT_TRANSPORT transport,
                              tBTM_SEC_CBACK* p_callback, void* p_ref_data,
                              tBTM_BLE_SEC_ACT sec_act) {
  tBTM_STATUS rc = 0;

  tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev(bd_addr);
  if (!p_dev_rec ||
      (transport == BT_TRANSPORT_BR_EDR &&
       p_dev_rec->hci_handle == BTM_SEC_INVALID_HANDLE) ||
      (transport == BT_TRANSPORT_LE &&
       p_dev_rec->ble_hci_handle == BTM_SEC_INVALID_HANDLE)) {
    /* Connection should be up and runnning */
    BTM_TRACE_WARNING("Security Manager: BTM_SetEncryption not connected");

    if (p_callback)
      (*p_callback)(&bd_addr, transport, p_ref_data, BTM_WRONG_MODE);

    return (BTM_WRONG_MODE);
  }

  if (transport == BT_TRANSPORT_BR_EDR &&
      (p_dev_rec->sec_flags & BTM_SEC_ENCRYPTED)) {
    BTM_TRACE_EVENT("Security Manager: BTM_SetEncryption already encrypted");

    if (*p_callback)
      (*p_callback)(&bd_addr, transport, p_ref_data, BTM_SUCCESS);

    return (BTM_SUCCESS);
  }

  /* enqueue security request if security is active */
  if (p_dev_rec->p_callback || (p_dev_rec->sec_state != BTM_SEC_STATE_IDLE)) {
    BTM_TRACE_WARNING(
        "Security Manager: BTM_SetEncryption busy, enqueue request");

    if (btm_sec_queue_encrypt_request(bd_addr, transport, p_callback,
                                      p_ref_data, sec_act)) {
      return BTM_CMD_STARTED;
    } else {
      if (p_callback)
        (*p_callback)(&bd_addr, transport, p_ref_data, BTM_NO_RESOURCES);
      return BTM_NO_RESOURCES;
    }
  }

  p_dev_rec->p_callback = p_callback;
  p_dev_rec->p_ref_data = p_ref_data;
  p_dev_rec->security_required |=
      (BTM_SEC_IN_AUTHENTICATE | BTM_SEC_IN_ENCRYPT);
  p_dev_rec->is_originator = false;

  BTM_TRACE_API(
      "Security Manager: BTM_SetEncryption Handle:%d State:%d Flags:0x%x "
      "Required:0x%x, p_dev_rec=%p, p_callback=%p",
      p_dev_rec->hci_handle, p_dev_rec->sec_state, p_dev_rec->sec_flags,
      p_dev_rec->security_required, p_dev_rec, p_callback);

  if (transport == BT_TRANSPORT_LE) {
    tACL_CONN* p = btm_bda_to_acl(bd_addr, transport);
    if (p) {
      rc = btm_ble_set_encryption(bd_addr, sec_act, p->link_role);
    } else {
      rc = BTM_WRONG_MODE;
      BTM_TRACE_WARNING("%s: cannot call btm_ble_set_encryption, p is NULL",
                        __func__);
    }
  } else {
    rc = btm_sec_execute_procedure(p_dev_rec);
  }

  if (rc != BTM_CMD_STARTED && rc != BTM_BUSY) {
    if (p_callback) {
      BTM_TRACE_DEBUG(
          "%s: clearing p_callback=%p, p_dev_rec=%p, transport=%d, "
          "bd_addr=%s",
          __func__, p_callback, p_dev_rec, transport,
          bd_addr.ToString().c_str());
      p_dev_rec->p_callback = NULL;
      (*p_callback)(&bd_addr, transport, p_dev_rec->p_ref_data, rc);
    }
  }

  return (rc);
}

/*******************************************************************************
 * disconnect the ACL link, if it's not done yet.
 ******************************************************************************/
static tBTM_STATUS btm_sec_send_hci_disconnect(tBTM_SEC_DEV_REC* p_dev_rec,
                                               uint8_t reason,
                                               uint16_t conn_handle) {
  uint8_t old_state = p_dev_rec->sec_state;
  tBTM_STATUS status = BTM_CMD_STARTED;

  BTM_TRACE_EVENT("btm_sec_send_hci_disconnect:  handle:0x%x, reason=0x%x",
                  conn_handle, reason);

  /* send HCI_Disconnect on a transport only once */
  switch (old_state) {
    case BTM_SEC_STATE_DISCONNECTING:
      if (conn_handle == p_dev_rec->hci_handle) return status;

      p_dev_rec->sec_state = BTM_SEC_STATE_DISCONNECTING_BOTH;
      break;

    case BTM_SEC_STATE_DISCONNECTING_BLE:
      if (conn_handle == p_dev_rec->ble_hci_handle) return status;

      p_dev_rec->sec_state = BTM_SEC_STATE_DISCONNECTING_BOTH;
      break;

    case BTM_SEC_STATE_DISCONNECTING_BOTH:
      return status;

    default:
      p_dev_rec->sec_state = (conn_handle == p_dev_rec->hci_handle)
                                 ? BTM_SEC_STATE_DISCONNECTING
                                 : BTM_SEC_STATE_DISCONNECTING_BLE;

      break;
  }

  /* If a role switch is in progress, delay the HCI Disconnect to avoid
   * controller problem */
  if (p_dev_rec->rs_disc_pending == BTM_SEC_RS_PENDING &&
      p_dev_rec->hci_handle == conn_handle) {
    BTM_TRACE_DEBUG(
        "RS in progress - Set DISC Pending flag in btm_sec_send_hci_disconnect "
        "to delay disconnect");
    p_dev_rec->rs_disc_pending = BTM_SEC_DISC_PENDING;
    status = BTM_SUCCESS;
  }
  /* Tear down the HCI link */
  else {
    btsnd_hcic_disconnect(conn_handle, reason);
  }

  return status;
}

/*******************************************************************************
 *
 * Function         BTM_ConfirmReqReply
 *
 * Description      This function is called to confirm the numeric value for
 *                  Simple Pairing in response to BTM_SP_CFM_REQ_EVT
 *
 * Parameters:      res           - result of the operation BTM_SUCCESS if
 *                                  success
 *                  bd_addr       - Address of the peer device
 *
 ******************************************************************************/
void BTM_ConfirmReqReply(tBTM_STATUS res, const RawAddress& bd_addr) {
  tBTM_SEC_DEV_REC* p_dev_rec;

  BTM_TRACE_EVENT("BTM_ConfirmReqReply() State: %s  Res: %u",
                  btm_pair_state_descr(btm_cb.pairing_state), res);

  /* If timeout already expired or has been canceled, ignore the reply */
  if ((btm_cb.pairing_state != BTM_PAIR_STATE_WAIT_NUMERIC_CONFIRM) ||
      (btm_cb.pairing_bda != bd_addr))
    return;

  btm_sec_change_pairing_state(BTM_PAIR_STATE_WAIT_AUTH_COMPLETE);

  if ((res == BTM_SUCCESS) || (res == BTM_SUCCESS_NO_SECURITY)) {
    btm_cb.acl_disc_reason = HCI_SUCCESS;

    if (res == BTM_SUCCESS) {
      p_dev_rec = btm_find_dev(bd_addr);
      if (p_dev_rec != NULL) {
        p_dev_rec->sec_flags |= BTM_SEC_LINK_KEY_AUTHED;
        p_dev_rec->sec_flags |= BTM_SEC_16_DIGIT_PIN_AUTHED;
      }
    }

    btsnd_hcic_user_conf_reply(bd_addr, true);
  } else {
    /* Report authentication failed event from state
     * BTM_PAIR_STATE_WAIT_AUTH_COMPLETE */
    btm_cb.acl_disc_reason = HCI_ERR_HOST_REJECT_SECURITY;
    btsnd_hcic_user_conf_reply(bd_addr, false);
  }
}

/*******************************************************************************
 *
 * Function         BTM_PasskeyReqReply
 *
 * Description      This function is called to provide the passkey for
 *                  Simple Pairing in response to BTM_SP_KEY_REQ_EVT
 *
 * Parameters:      res     - result of the operation BTM_SUCCESS if success
 *                  bd_addr - Address of the peer device
 *                  passkey - numeric value in the range of
 *                  BTM_MIN_PASSKEY_VAL(0) -
 *                  BTM_MAX_PASSKEY_VAL(999999(0xF423F)).
 *
 ******************************************************************************/
void BTM_PasskeyReqReply(tBTM_STATUS res, const RawAddress& bd_addr,
                         uint32_t passkey) {
  BTM_TRACE_API("BTM_PasskeyReqReply: State: %s  res:%d",
                btm_pair_state_descr(btm_cb.pairing_state), res);

  if ((btm_cb.pairing_state == BTM_PAIR_STATE_IDLE) ||
      (btm_cb.pairing_bda != bd_addr)) {
    return;
  }

  /* If timeout already expired or has been canceled, ignore the reply */
  if ((btm_cb.pairing_state == BTM_PAIR_STATE_WAIT_AUTH_COMPLETE) &&
      (res != BTM_SUCCESS)) {
    tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev(bd_addr);
    if (p_dev_rec != NULL) {
      btm_cb.acl_disc_reason = HCI_ERR_HOST_REJECT_SECURITY;

      if (p_dev_rec->hci_handle != BTM_SEC_INVALID_HANDLE)
        btm_sec_send_hci_disconnect(p_dev_rec, HCI_ERR_AUTH_FAILURE,
                                    p_dev_rec->hci_handle);
      else
        BTM_SecBondCancel(bd_addr);

      p_dev_rec->sec_flags &=
          ~(BTM_SEC_LINK_KEY_AUTHED | BTM_SEC_LINK_KEY_KNOWN);

      btm_sec_change_pairing_state(BTM_PAIR_STATE_IDLE);
      return;
    }
  } else if (btm_cb.pairing_state != BTM_PAIR_STATE_KEY_ENTRY)
    return;

  if (passkey > BTM_MAX_PASSKEY_VAL) res = BTM_ILLEGAL_VALUE;

  btm_sec_change_pairing_state(BTM_PAIR_STATE_WAIT_AUTH_COMPLETE);

  if (res != BTM_SUCCESS) {
    /* use BTM_PAIR_STATE_WAIT_AUTH_COMPLETE to report authentication failed
     * event */
    btm_cb.acl_disc_reason = HCI_ERR_HOST_REJECT_SECURITY;
    btsnd_hcic_user_passkey_neg_reply(bd_addr);
  } else {
    btm_cb.acl_disc_reason = HCI_SUCCESS;
    btsnd_hcic_user_passkey_reply(bd_addr, passkey);
  }
}

/*******************************************************************************
 *
 * Function         BTM_SendKeypressNotif
 *
 * Description      This function is used during the passkey entry model
 *                  by a device with KeyboardOnly IO capabilities
 *                  (very likely to be a HID Device).
 *                  It is called by a HID Device to inform the remote device
 *                  when a key has been entered or erased.
 *
 * Parameters:      bd_addr - Address of the peer device
 *                  type - notification type
 *
 ******************************************************************************/
void BTM_SendKeypressNotif(const RawAddress& bd_addr, tBTM_SP_KEY_TYPE type) {
  /* This API only make sense between PASSKEY_REQ and SP complete */
  if (btm_cb.pairing_state == BTM_PAIR_STATE_KEY_ENTRY)
    btsnd_hcic_send_keypress_notif(bd_addr, type);
}

/*******************************************************************************
 *
 * Function         BTM_IoCapRsp
 *
 * Description      This function is called in response to BTM_SP_IO_REQ_EVT
 *                  When the event data io_req.oob_data is set to
 *                  BTM_OOB_UNKNOWN by the tBTM_SP_CALLBACK implementation,
 *                  this function is called to provide the actual response
 *
 * Parameters:      bd_addr - Address of the peer device
 *                  io_cap  - The IO capability of local device.
 *                  oob     - BTM_OOB_NONE or BTM_OOB_PRESENT.
 *                  auth_req- MITM protection required or not.
 *
 ******************************************************************************/
void BTM_IoCapRsp(const RawAddress& bd_addr, tBTM_IO_CAP io_cap,
                  tBTM_OOB_DATA oob, tBTM_AUTH_REQ auth_req) {
  BTM_TRACE_EVENT("BTM_IoCapRsp: state: %s  oob: %d io_cap: %d",
                  btm_pair_state_descr(btm_cb.pairing_state), oob, io_cap);

  if ((btm_cb.pairing_state != BTM_PAIR_STATE_WAIT_LOCAL_IOCAPS) ||
      (btm_cb.pairing_bda != bd_addr))
    return;

  if (oob < BTM_OOB_UNKNOWN && io_cap < BTM_IO_CAP_MAX) {
    btm_cb.devcb.loc_auth_req = auth_req;
    btm_cb.devcb.loc_io_caps = io_cap;

    if (btm_cb.pairing_flags & BTM_PAIR_FLAGS_WE_STARTED_DD)
      auth_req = (BTM_AUTH_DD_BOND | (auth_req & BTM_AUTH_YN_BIT));

    btsnd_hcic_io_cap_req_reply(bd_addr, io_cap, oob, auth_req);
  }
}

/*******************************************************************************
 *
 * Function         BTM_ReadLocalOobData
 *
 * Description      This function is called to read the local OOB data from
 *                  LM
 *
 ******************************************************************************/
void BTM_ReadLocalOobData(void) { btsnd_hcic_read_local_oob_data(); }

/*******************************************************************************
 *
 * Function         BTM_RemoteOobDataReply
 *
 * Description      This function is called to provide the remote OOB data for
 *                  Simple Pairing in response to BTM_SP_RMT_OOB_EVT
 *
 * Parameters:      bd_addr     - Address of the peer device
 *                  c           - simple pairing Hash C.
 *                  r           - simple pairing Randomizer  C.
 *
 ******************************************************************************/
void BTM_RemoteOobDataReply(tBTM_STATUS res, const RawAddress& bd_addr,
                            const Octet16& c, const Octet16& r) {
  BTM_TRACE_EVENT("%s() - State: %s res: %d", __func__,
                  btm_pair_state_descr(btm_cb.pairing_state), res);

  /* If timeout already expired or has been canceled, ignore the reply */
  if (btm_cb.pairing_state != BTM_PAIR_STATE_WAIT_LOCAL_OOB_RSP) return;

  btm_sec_change_pairing_state(BTM_PAIR_STATE_WAIT_AUTH_COMPLETE);

  if (res != BTM_SUCCESS) {
    /* use BTM_PAIR_STATE_WAIT_AUTH_COMPLETE to report authentication failed
     * event */
    btm_cb.acl_disc_reason = HCI_ERR_HOST_REJECT_SECURITY;
    btsnd_hcic_rem_oob_neg_reply(bd_addr);
  } else {
    btm_cb.acl_disc_reason = HCI_SUCCESS;
    btsnd_hcic_rem_oob_reply(bd_addr, c, r);
  }
}

/*******************************************************************************
 *
 * Function         BTM_BuildOobData
 *
 * Description      This function is called to build the OOB data payload to
 *                  be sent over OOB (non-Bluetooth) link
 *
 * Parameters:      p_data  - the location for OOB data
 *                  max_len - p_data size.
 *                  c       - simple pairing Hash C.
 *                  r       - simple pairing Randomizer  C.
 *                  name_len- 0, local device name would not be included.
 *                            otherwise, the local device name is included for
 *                            up to this specified length
 *
 * Returns          Number of bytes in p_data.
 *
 ******************************************************************************/
uint16_t BTM_BuildOobData(uint8_t* p_data, uint16_t max_len, const Octet16& c,
                          const Octet16& r, uint8_t name_len) {
  uint8_t* p = p_data;
  uint16_t len = 0;
  uint16_t name_size;
  uint8_t name_type = BTM_EIR_SHORTENED_LOCAL_NAME_TYPE;

  if (p_data && max_len >= BTM_OOB_MANDATORY_SIZE) {
    /* add mandatory part */
    UINT16_TO_STREAM(p, len);
    BDADDR_TO_STREAM(p, *controller_get_interface()->get_address());

    len = BTM_OOB_MANDATORY_SIZE;
    max_len -= len;

    /* now optional part */

    /* add Hash C */
    uint16_t delta = BTM_OOB_HASH_C_SIZE + 2;
    if (max_len >= delta) {
      *p++ = BTM_OOB_HASH_C_SIZE + 1;
      *p++ = BTM_EIR_OOB_SSP_HASH_C_TYPE;
      ARRAY_TO_STREAM(p, c.data(), BTM_OOB_HASH_C_SIZE);
      len += delta;
      max_len -= delta;
    }

    /* add Rand R */
    delta = BTM_OOB_RAND_R_SIZE + 2;
    if (max_len >= delta) {
      *p++ = BTM_OOB_RAND_R_SIZE + 1;
      *p++ = BTM_EIR_OOB_SSP_RAND_R_TYPE;
      ARRAY_TO_STREAM(p, r.data(), BTM_OOB_RAND_R_SIZE);
      len += delta;
      max_len -= delta;
    }

    /* add class of device */
    delta = BTM_OOB_COD_SIZE + 2;
    if (max_len >= delta) {
      *p++ = BTM_OOB_COD_SIZE + 1;
      *p++ = BTM_EIR_OOB_COD_TYPE;
      DEVCLASS_TO_STREAM(p, btm_cb.devcb.dev_class);
      len += delta;
      max_len -= delta;
    }
    name_size = name_len;
    if (name_size > strlen(btm_cb.cfg.bd_name)) {
      name_type = BTM_EIR_COMPLETE_LOCAL_NAME_TYPE;
      name_size = (uint16_t)strlen(btm_cb.cfg.bd_name);
    }
    delta = name_size + 2;
    if (max_len >= delta) {
      *p++ = name_size + 1;
      *p++ = name_type;
      ARRAY_TO_STREAM(p, btm_cb.cfg.bd_name, name_size);
      len += delta;
      max_len -= delta;
    }
    /* update len */
    p = p_data;
    UINT16_TO_STREAM(p, len);
  }
  return len;
}

/*******************************************************************************
 *
 * Function         BTM_BothEndsSupportSecureConnections
 *
 * Description      This function is called to check if both the local device
 *                  and the peer device specified by bd_addr support BR/EDR
 *                  Secure Connections.
 *
 * Parameters:      bd_addr - address of the peer
 *
 * Returns          true if BR/EDR Secure Connections are supported by both
 *                  local and the remote device, else false.
 *
 ******************************************************************************/
bool BTM_BothEndsSupportSecureConnections(const RawAddress& bd_addr) {
  return ((controller_get_interface()->supports_secure_connections()) &&
          (BTM_PeerSupportsSecureConnections(bd_addr)));
}

/*******************************************************************************
 *
 * Function         BTM_PeerSupportsSecureConnections
 *
 * Description      This function is called to check if the peer supports
 *                  BR/EDR Secure Connections.
 *
 * Parameters:      bd_addr - address of the peer
 *
 * Returns          true if BR/EDR Secure Connections are supported by the peer,
 *                  else false.
 *
 ******************************************************************************/
bool BTM_PeerSupportsSecureConnections(const RawAddress& bd_addr) {
  tBTM_SEC_DEV_REC* p_dev_rec;

  p_dev_rec = btm_find_dev(bd_addr);
  if (p_dev_rec == NULL) {
    LOG(WARNING) << __func__ << ": unknown BDA: " << bd_addr;
    return false;
  }

  return (p_dev_rec->remote_supports_secure_connections);
}

/*******************************************************************************
 *
 * Function         BTM_ReadOobData
 *
 * Description      This function is called to parse the OOB data payload
 *                  received over OOB (non-Bluetooth) link
 *
 * Parameters:      p_data  - the location for OOB data
 *                  eir_tag - The associated EIR tag to read the data.
 *                  *p_len(output) - the length of the data with the given tag.
 *
 * Returns          the beginning of the data with the given tag.
 *                  NULL, if the tag is not found.
 *
 ******************************************************************************/
uint8_t* BTM_ReadOobData(uint8_t* p_data, uint8_t eir_tag, uint8_t* p_len) {
  uint8_t* p = p_data;
  uint16_t max_len;
  uint8_t len, type;
  uint8_t* p_ret = NULL;
  uint8_t ret_len = 0;

  if (p_data) {
    STREAM_TO_UINT16(max_len, p);
    if (max_len >= BTM_OOB_MANDATORY_SIZE) {
      if (BTM_EIR_OOB_BD_ADDR_TYPE == eir_tag) {
        p_ret = p; /* the location for bd_addr */
        ret_len = BTM_OOB_BD_ADDR_SIZE;
      } else {
        p += BD_ADDR_LEN;
        max_len -= BTM_OOB_MANDATORY_SIZE;
        /* now the optional data in EIR format */
        while (max_len > 0) {
          len = *p++; /* tag data len + 1 */
          type = *p++;
          if (eir_tag == type) {
            p_ret = p;
            ret_len = len - 1;
            break;
          }
          /* the data size of this tag is len + 1 (tag data len + 2) */
          if (max_len > len) {
            max_len -= len;
            max_len--;
            len--;
            p += len;
          } else
            max_len = 0;
        }
      }
    }
  }

  if (p_len) *p_len = ret_len;

  return p_ret;
}

/*******************************************************************************
 *
 * Function         BTM_SetOutService
 *
 * Description      This function is called to set the service for
 *                  outgoing connections.
 *
 *                  If the profile/application calls BTM_SetSecurityLevel
 *                  before initiating a connection, this function does not
 *                  need to be called.
 *
 * Returns          void
 *
 ******************************************************************************/
void BTM_SetOutService(const RawAddress& bd_addr, uint8_t service_id,
                       uint32_t mx_chan_id) {
  tBTM_SEC_DEV_REC* p_dev_rec;
  tBTM_SEC_SERV_REC* p_serv_rec = &btm_cb.sec_serv_rec[0];

  btm_cb.p_out_serv = p_serv_rec;
  p_dev_rec = btm_find_dev(bd_addr);

  for (int i = 0; i < BTM_SEC_MAX_SERVICE_RECORDS; i++, p_serv_rec++) {
    if ((p_serv_rec->security_flags & BTM_SEC_IN_USE) &&
        (p_serv_rec->service_id == service_id) &&
        (p_serv_rec->orig_mx_chan_id == mx_chan_id)) {
      BTM_TRACE_API(
          "BTM_SetOutService p_out_serv id %d, psm 0x%04x, proto_id %d, "
          "chan_id %d",
          p_serv_rec->service_id, p_serv_rec->psm, p_serv_rec->mx_proto_id,
          p_serv_rec->orig_mx_chan_id);
      btm_cb.p_out_serv = p_serv_rec;
      if (p_dev_rec) p_dev_rec->p_cur_service = p_serv_rec;
      break;
    }
  }
}

/************************************************************************
 *              I N T E R N A L     F U N C T I O N S
 ************************************************************************/
/*******************************************************************************
 *
 * Function         btm_sec_is_upgrade_possible
 *
 * Description      This function returns true if the existing link key
 *                  can be upgraded or if the link key does not exist.
 *
 * Returns          bool
 *
 ******************************************************************************/
static bool btm_sec_is_upgrade_possible(tBTM_SEC_DEV_REC* p_dev_rec,
                                        bool is_originator) {
  uint16_t mtm_check = is_originator ? BTM_SEC_OUT_MITM : BTM_SEC_IN_MITM;
  bool is_possible = true;

  if (p_dev_rec->sec_flags & BTM_SEC_LINK_KEY_KNOWN) {
    is_possible = false;
    if (p_dev_rec->p_cur_service) {
      BTM_TRACE_DEBUG(
          "%s() id: %d, link_key_typet: %d, rmt_io_caps: %d, chk flags: 0x%x, "
          "flags: 0x%x",
          __func__, p_dev_rec->p_cur_service->service_id,
          p_dev_rec->link_key_type, p_dev_rec->rmt_io_caps, mtm_check,
          p_dev_rec->p_cur_service->security_flags);
    } else {
      BTM_TRACE_DEBUG(
          "%s() link_key_typet: %d, rmt_io_caps: %d, chk flags: 0x%x", __func__,
          p_dev_rec->link_key_type, p_dev_rec->rmt_io_caps, mtm_check);
    }
    /* Already have a link key to the connected peer. Is the link key secure
     *enough?
     ** Is a link key upgrade even possible?
     */
    if ((p_dev_rec->security_required & mtm_check) /* needs MITM */
        && ((p_dev_rec->link_key_type == BTM_LKEY_TYPE_UNAUTH_COMB) ||
            (p_dev_rec->link_key_type == BTM_LKEY_TYPE_UNAUTH_COMB_P_256))
        /* has unauthenticated
        link key */
        && (p_dev_rec->rmt_io_caps < BTM_IO_CAP_MAX) /* a valid peer IO cap */
        && (btm_sec_io_map[p_dev_rec->rmt_io_caps][btm_cb.devcb.loc_io_caps]))
    /* authenticated
    link key is possible */
    {
      /* upgrade is possible: check if the application wants the upgrade.
       * If the application is configured to use a global MITM flag,
       * it probably would not want to upgrade the link key based on the
       * security level database */
      is_possible = true;
    }
  }
  BTM_TRACE_DEBUG("%s() is_possible: %d sec_flags: 0x%x", __func__, is_possible,
                  p_dev_rec->sec_flags);
  return is_possible;
}

/*******************************************************************************
 *
 * Function         btm_sec_check_upgrade
 *
 * Description      This function is called to check if the existing link key
 *                  needs to be upgraded.
 *
 * Returns          void
 *
 ******************************************************************************/
static void btm_sec_check_upgrade(tBTM_SEC_DEV_REC* p_dev_rec,
                                  bool is_originator) {
  BTM_TRACE_DEBUG("%s()", __func__);

  /* Only check if link key already exists */
  if (!(p_dev_rec->sec_flags & BTM_SEC_LINK_KEY_KNOWN)) return;

  if (btm_sec_is_upgrade_possible(p_dev_rec, is_originator)) {
    BTM_TRACE_DEBUG("need upgrade!! sec_flags:0x%x", p_dev_rec->sec_flags);
    /* upgrade is possible: check if the application wants the upgrade.
     * If the application is configured to use a global MITM flag,
     * it probably would not want to upgrade the link key based on the security
     * level database */
    tBTM_SP_UPGRADE evt_data;
    evt_data.bd_addr = p_dev_rec->bd_addr;
    evt_data.upgrade = true;
    if (btm_cb.api.p_sp_callback)
      (*btm_cb.api.p_sp_callback)(BTM_SP_UPGRADE_EVT,
                                  (tBTM_SP_EVT_DATA*)&evt_data);

    BTM_TRACE_DEBUG("evt_data.upgrade:0x%x", evt_data.upgrade);
    if (evt_data.upgrade) {
      /* if the application confirms the upgrade, set the upgrade bit */
      p_dev_rec->sm4 |= BTM_SM4_UPGRADE;

      /* Clear the link key known to go through authentication/pairing again */
      p_dev_rec->sec_flags &=
          ~(BTM_SEC_LINK_KEY_KNOWN | BTM_SEC_LINK_KEY_AUTHED);
      p_dev_rec->sec_flags &= ~BTM_SEC_AUTHENTICATED;
      BTM_TRACE_DEBUG("sec_flags:0x%x", p_dev_rec->sec_flags);
    }
  }
}

/*******************************************************************************
 *
 * Function         btm_sec_l2cap_access_req
 *
 * Description      This function is called by the L2CAP to grant permission to
 *                  establish L2CAP connection to or from the peer device.
 *
 * Parameters:      bd_addr       - Address of the peer device
 *                  psm           - L2CAP PSM
 *                  is_originator - true if protocol above L2CAP originates
 *                                  connection
 *                  p_callback    - Pointer to callback function called if
 *                                  this function returns PENDING after required
 *                                  procedures are complete. MUST NOT BE NULL.
 *
 * Returns          tBTM_STATUS
 *
 ******************************************************************************/
tBTM_STATUS btm_sec_l2cap_access_req(const RawAddress& bd_addr, uint16_t psm,
                                     uint16_t handle, CONNECTION_TYPE conn_type,
                                     tBTM_SEC_CALLBACK* p_callback,
                                     void* p_ref_data) {
  tBTM_SEC_DEV_REC* p_dev_rec;
  tBTM_SEC_SERV_REC* p_serv_rec;
  uint16_t security_required;
  uint16_t old_security_required;
  bool old_is_originator;
  tBTM_STATUS rc = BTM_SUCCESS;
  bool chk_acp_auth_done = false;
  bool is_originator;
  tBT_TRANSPORT transport =
      BT_TRANSPORT_BR_EDR; /* should check PSM range in LE connection oriented
                              L2CAP connection */

  is_originator = conn_type;

  BTM_TRACE_DEBUG("%s() is_originator:%d, 0x%x, psm=0x%04x", __func__,
                  is_originator, p_ref_data, psm);

  /* Find or get oldest record */
  p_dev_rec = btm_find_or_alloc_dev(bd_addr);

  p_dev_rec->hci_handle = handle;

  /* Find the service record for the PSM */
  p_serv_rec = btm_sec_find_first_serv(conn_type, psm);

  /* If there is no application registered with this PSM do not allow connection
   */
  if (!p_serv_rec) {
    BTM_TRACE_WARNING("%s() PSM: %d no application registerd", __func__, psm);
    (*p_callback)(&bd_addr, transport, p_ref_data, BTM_MODE_UNSUPPORTED);
    return (BTM_MODE_UNSUPPORTED);
  }

  /* Services level0 by default have no security */
  if ((btm_sec_is_serv_level0(psm)) &&
      (!btm_cb.devcb.secure_connections_only)) {
    (*p_callback)(&bd_addr, transport, p_ref_data, BTM_SUCCESS_NO_SECURITY);
    return (BTM_SUCCESS);
  }
  if (btm_cb.security_mode == BTM_SEC_MODE_SC) {
    security_required = btm_sec_set_serv_level4_flags(
        p_serv_rec->security_flags, is_originator);
  } else {
    security_required = p_serv_rec->security_flags;
  }

  BTM_TRACE_DEBUG(
      "%s: security_required 0x%04x, is_originator 0x%02x, psm  0x%04x",
      __func__, security_required, is_originator, psm);

  if ((!is_originator) && (security_required & BTM_SEC_MODE4_LEVEL4)) {
    bool local_supports_sc =
        controller_get_interface()->supports_secure_connections();
    /* acceptor receives L2CAP Channel Connect Request for Secure Connections
     * Only service */
    if (!(local_supports_sc) ||
        !(p_dev_rec->remote_supports_secure_connections)) {
      BTM_TRACE_DEBUG("%s: SC only service, local_support_for_sc %d",
                      "rmt_support_for_sc : %d -> fail pairing", __func__,
                      local_supports_sc,
                      p_dev_rec->remote_supports_secure_connections);
      if (p_callback)
        (*p_callback)(&bd_addr, transport, (void*)p_ref_data,
                      BTM_MODE4_LEVEL4_NOT_SUPPORTED);

      return (BTM_MODE4_LEVEL4_NOT_SUPPORTED);
    }
  }

  /* there are some devices (moto KRZR) which connects to several services at
   * the same time */
  /* we will process one after another */
  if ((p_dev_rec->p_callback) ||
      (btm_cb.pairing_state != BTM_PAIR_STATE_IDLE)) {
    BTM_TRACE_EVENT("%s() - busy - PSM:%d delayed  state: %s mode:%d, sm4:0x%x",
                    __func__, psm, btm_pair_state_descr(btm_cb.pairing_state),
                    btm_cb.security_mode, p_dev_rec->sm4);
    BTM_TRACE_EVENT("security_flags:x%x, sec_flags:x%x", security_required,
                    p_dev_rec->sec_flags);
    rc = BTM_CMD_STARTED;
    if ((btm_cb.security_mode == BTM_SEC_MODE_UNDEFINED ||
         btm_cb.security_mode == BTM_SEC_MODE_NONE ||
         btm_cb.security_mode == BTM_SEC_MODE_SERVICE ||
         btm_cb.security_mode == BTM_SEC_MODE_LINK) ||
        (BTM_SM4_KNOWN == p_dev_rec->sm4) ||
        (BTM_SEC_IS_SM4(p_dev_rec->sm4) &&
         (!btm_sec_is_upgrade_possible(p_dev_rec, is_originator)))) {
      /* legacy mode - local is legacy or local is lisbon/peer is legacy
       * or SM4 with no possibility of link key upgrade */
      if (is_originator) {
        if (((security_required & BTM_SEC_OUT_FLAGS) == 0) ||
            ((((security_required & BTM_SEC_OUT_FLAGS) ==
               BTM_SEC_OUT_AUTHENTICATE) &&
              btm_dev_authenticated(p_dev_rec))) ||
            ((((security_required & BTM_SEC_OUT_FLAGS) ==
               (BTM_SEC_OUT_AUTHENTICATE | BTM_SEC_OUT_ENCRYPT)) &&
              btm_dev_encrypted(p_dev_rec))) ||
            ((((security_required & BTM_SEC_OUT_FLAGS) == BTM_SEC_OUT_FLAGS) &&
              btm_dev_authorized(p_dev_rec) && btm_dev_encrypted(p_dev_rec)))) {
          rc = BTM_SUCCESS;
        }
      } else {
        if (((security_required & BTM_SEC_IN_FLAGS) == 0) ||
            (((security_required & BTM_SEC_IN_FLAGS) ==
              BTM_SEC_IN_AUTHENTICATE) &&
             btm_dev_authenticated(p_dev_rec)) ||
            (((security_required & BTM_SEC_IN_FLAGS) ==
              (BTM_SEC_IN_AUTHENTICATE | BTM_SEC_IN_ENCRYPT)) &&
             btm_dev_encrypted(p_dev_rec)) ||
            (((security_required & BTM_SEC_IN_FLAGS) == BTM_SEC_IN_AUTHORIZE) &&
             (btm_dev_authorized(p_dev_rec) ||
              btm_serv_trusted(p_dev_rec, p_serv_rec))) ||
            (((security_required & BTM_SEC_IN_FLAGS) ==
              (BTM_SEC_IN_AUTHENTICATE | BTM_SEC_IN_AUTHORIZE)) &&
             ((btm_dev_authorized(p_dev_rec) ||
               btm_serv_trusted(p_dev_rec, p_serv_rec)) &&
              btm_dev_authenticated(p_dev_rec))) ||
            (((security_required & BTM_SEC_IN_FLAGS) ==
              (BTM_SEC_IN_ENCRYPT | BTM_SEC_IN_AUTHORIZE)) &&
             ((btm_dev_authorized(p_dev_rec) ||
               btm_serv_trusted(p_dev_rec, p_serv_rec)) &&
              btm_dev_encrypted(p_dev_rec))) ||
            (((security_required & BTM_SEC_IN_FLAGS) == BTM_SEC_IN_FLAGS) &&
             btm_dev_encrypted(p_dev_rec) &&
             (btm_dev_authorized(p_dev_rec) ||
              btm_serv_trusted(p_dev_rec, p_serv_rec)))) {
          // Check for 16 digits (or MITM)
          if (((security_required & BTM_SEC_IN_MIN_16_DIGIT_PIN) == 0) ||
              (((security_required & BTM_SEC_IN_MIN_16_DIGIT_PIN) ==
                BTM_SEC_IN_MIN_16_DIGIT_PIN) &&
               btm_dev_16_digit_authenticated(p_dev_rec))) {
            rc = BTM_SUCCESS;
          }
        }
      }

      if ((rc == BTM_SUCCESS) && (security_required & BTM_SEC_MODE4_LEVEL4) &&
          (p_dev_rec->link_key_type != BTM_LKEY_TYPE_AUTH_COMB_P_256)) {
        rc = BTM_CMD_STARTED;
      }

      if (rc == BTM_SUCCESS) {
        if (p_callback)
          (*p_callback)(&bd_addr, transport, (void*)p_ref_data, BTM_SUCCESS);
        return (BTM_SUCCESS);
      }
    }

    btm_cb.sec_req_pending = true;
    return (BTM_CMD_STARTED);
  }

  /* Save pointer to service record */
  p_dev_rec->p_cur_service = p_serv_rec;

  /* Modify security_required in btm_sec_l2cap_access_req for Lisbon */
  if (btm_cb.security_mode == BTM_SEC_MODE_SP ||
      btm_cb.security_mode == BTM_SEC_MODE_SP_DEBUG ||
      btm_cb.security_mode == BTM_SEC_MODE_SC) {
    if (BTM_SEC_IS_SM4(p_dev_rec->sm4)) {
      if (is_originator) {
        /* SM4 to SM4 -> always authenticate & encrypt */
        security_required |= (BTM_SEC_OUT_AUTHENTICATE | BTM_SEC_OUT_ENCRYPT);
      } else /* acceptor */
      {
        /* SM4 to SM4: the acceptor needs to make sure the authentication is
         * already done */
        chk_acp_auth_done = true;
        /* SM4 to SM4 -> always authenticate & encrypt */
        security_required |= (BTM_SEC_IN_AUTHENTICATE | BTM_SEC_IN_ENCRYPT);
      }
    } else if (!(BTM_SM4_KNOWN & p_dev_rec->sm4)) {
      /* the remote features are not known yet */
      BTM_TRACE_DEBUG("%s: (%s) remote features unknown!!sec_flags:0x%02x",
                      __func__, (is_originator) ? "initiator" : "acceptor",
                      p_dev_rec->sec_flags);

      p_dev_rec->sm4 |= BTM_SM4_REQ_PEND;
      return (BTM_CMD_STARTED);
    }
  }

  BTM_TRACE_DEBUG(
      "%s()  sm4:0x%x, sec_flags:0x%x, security_required:0x%x chk:%d", __func__,
      p_dev_rec->sm4, p_dev_rec->sec_flags, security_required,
      chk_acp_auth_done);

  old_security_required = p_dev_rec->security_required;
  old_is_originator = p_dev_rec->is_originator;
  p_dev_rec->security_required = security_required;
  p_dev_rec->p_ref_data = p_ref_data;
  p_dev_rec->is_originator = is_originator;

/* If there are multiple service records used through the same PSM */
/* leave security decision for the multiplexor on the top */
  if ((btm_sec_find_next_serv(p_serv_rec)) != NULL) {
    BTM_TRACE_DEBUG("no next_serv sm4:0x%x, chk:%d", p_dev_rec->sm4,
                    chk_acp_auth_done);
    if (!BTM_SEC_IS_SM4(p_dev_rec->sm4)) {
      BTM_TRACE_EVENT(
          "Security Manager: l2cap_access_req PSM:%d postponed for multiplexer",
          psm);
      /* pre-Lisbon: restore the old settings */
      p_dev_rec->security_required = old_security_required;
      p_dev_rec->is_originator = old_is_originator;

      (*p_callback)(&bd_addr, transport, p_ref_data, BTM_SUCCESS);

      return (BTM_SUCCESS);
    }
  }

  /* if the originator is using dynamic PSM in legacy mode, do not start any
   * security process now
   * The layer above L2CAP needs to carry out the security requirement after
   * L2CAP connect
   * response is received */
  if (is_originator &&
      ((btm_cb.security_mode == BTM_SEC_MODE_UNDEFINED ||
        btm_cb.security_mode == BTM_SEC_MODE_NONE ||
        btm_cb.security_mode == BTM_SEC_MODE_SERVICE ||
        btm_cb.security_mode == BTM_SEC_MODE_LINK) ||
       !BTM_SEC_IS_SM4(p_dev_rec->sm4)) &&
      (psm >= 0x1001)) {
    BTM_TRACE_EVENT(
        "dynamic PSM:0x%x in legacy mode - postponed for upper layer", psm);
    /* restore the old settings */
    p_dev_rec->security_required = old_security_required;
    p_dev_rec->is_originator = old_is_originator;

    (*p_callback)(&bd_addr, transport, p_ref_data, BTM_SUCCESS);

    return (BTM_SUCCESS);
  }

  if (chk_acp_auth_done) {
    BTM_TRACE_DEBUG(
        "(SM4 to SM4) btm_sec_l2cap_access_req rspd. authenticated: x%x, enc: "
        "x%x",
        (p_dev_rec->sec_flags & BTM_SEC_AUTHENTICATED),
        (p_dev_rec->sec_flags & BTM_SEC_ENCRYPTED));
    /* SM4, but we do not know for sure which level of security we need.
     * as long as we have a link key, it's OK */
    if ((0 == (p_dev_rec->sec_flags & BTM_SEC_AUTHENTICATED)) ||
        (0 == (p_dev_rec->sec_flags & BTM_SEC_ENCRYPTED))) {
      rc = BTM_DELAY_CHECK;
      /*
      2046 may report HCI_Encryption_Change and L2C Connection Request out of
      sequence
      because of data path issues. Delay this disconnect a little bit
      */
      LOG_INFO(
          LOG_TAG,
          "%s peer should have initiated security process by now (SM4 to SM4)",
          __func__);
      p_dev_rec->p_callback = p_callback;
      p_dev_rec->sec_state = BTM_SEC_STATE_DELAY_FOR_ENC;
      (*p_callback)(&bd_addr, transport, p_ref_data, rc);

      return BTM_SUCCESS;
    }
  }

  p_dev_rec->p_callback = p_callback;

  if (p_dev_rec->last_author_service_id == BTM_SEC_NO_LAST_SERVICE_ID ||
      p_dev_rec->last_author_service_id !=
          p_dev_rec->p_cur_service->service_id) {
    /* Although authentication and encryption are per connection
    ** authorization is per access request.  For example when serial connection
    ** is up and authorized and client requests to read file (access to other
    ** scn), we need to request user's permission again.
    */
    p_dev_rec->sec_flags &= ~BTM_SEC_AUTHORIZED;
  }

  if (BTM_SEC_IS_SM4(p_dev_rec->sm4)) {
    if ((p_dev_rec->security_required & BTM_SEC_MODE4_LEVEL4) &&
        (p_dev_rec->link_key_type != BTM_LKEY_TYPE_AUTH_COMB_P_256)) {
      /* BTM_LKEY_TYPE_AUTH_COMB_P_256 is the only acceptable key in this case
       */
      if ((p_dev_rec->sec_flags & BTM_SEC_LINK_KEY_KNOWN) != 0) {
        p_dev_rec->sm4 |= BTM_SM4_UPGRADE;
      }
      p_dev_rec->sec_flags &=
          ~(BTM_SEC_LINK_KEY_KNOWN | BTM_SEC_LINK_KEY_AUTHED |
            BTM_SEC_AUTHENTICATED);
      BTM_TRACE_DEBUG("%s: sec_flags:0x%x", __func__, p_dev_rec->sec_flags);
    } else {
      /* If we already have a link key to the connected peer, is it secure
       * enough? */
      btm_sec_check_upgrade(p_dev_rec, is_originator);
    }
  }

  BTM_TRACE_EVENT(
      "%s() PSM:%d Handle:%d State:%d Flags: 0x%x Required: 0x%x Service ID:%d",
      __func__, psm, handle, p_dev_rec->sec_state, p_dev_rec->sec_flags,
      p_dev_rec->security_required, p_dev_rec->p_cur_service->service_id);

  rc = btm_sec_execute_procedure(p_dev_rec);
  if (rc != BTM_CMD_STARTED) {
    BTM_TRACE_DEBUG("%s: p_dev_rec=%p, clearing callback. old p_callback=%p",
                    __func__, p_dev_rec, p_dev_rec->p_callback);
    p_dev_rec->p_callback = NULL;
    (*p_callback)(&bd_addr, transport, p_dev_rec->p_ref_data, (uint8_t)rc);
  }

  return (rc);
}

/*******************************************************************************
 *
 * Function         btm_sec_mx_access_request
 *
 * Description      This function is called by all Multiplexing Protocols during
 *                  establishing connection to or from peer device to grant
 *                  permission to establish application connection.
 *
 * Parameters:      bd_addr       - Address of the peer device
 *                  psm           - L2CAP PSM
 *                  is_originator - true if protocol above L2CAP originates
 *                                  connection
 *                  mx_proto_id   - protocol ID of the multiplexer
 *                  mx_chan_id    - multiplexer channel to reach application
 *                  p_callback    - Pointer to callback function called if
 *                                  this function returns PENDING after required
 *                                  procedures are completed
 *                  p_ref_data    - Pointer to any reference data needed by the
 *                                  the callback function.
 *
 * Returns          BTM_CMD_STARTED
 *
 ******************************************************************************/
tBTM_STATUS btm_sec_mx_access_request(const RawAddress& bd_addr, uint16_t psm,
                                      bool is_originator, uint32_t mx_proto_id,
                                      uint32_t mx_chan_id,
                                      tBTM_SEC_CALLBACK* p_callback,
                                      void* p_ref_data) {
  tBTM_SEC_DEV_REC* p_dev_rec;
  tBTM_SEC_SERV_REC* p_serv_rec;
  tBTM_STATUS rc;
  uint16_t security_required;
  bool transport = false; /* should check PSM range in LE connection oriented
                             L2CAP connection */

  BTM_TRACE_DEBUG("%s() is_originator: %d", __func__, is_originator);
  /* Find or get oldest record */
  p_dev_rec = btm_find_or_alloc_dev(bd_addr);

  /* Find the service record for the PSM */
  p_serv_rec =
      btm_sec_find_mx_serv(is_originator, psm, mx_proto_id, mx_chan_id);

  /* If there is no application registered with this PSM do not allow connection
   */
  if (!p_serv_rec) {
    if (p_callback)
      (*p_callback)(&bd_addr, transport, p_ref_data, BTM_MODE_UNSUPPORTED);

    BTM_TRACE_ERROR(
        "Security Manager: MX service not found PSM:%d Proto:%d SCN:%d", psm,
        mx_proto_id, mx_chan_id);
    return BTM_NO_RESOURCES;
  }

  if ((btm_cb.security_mode == BTM_SEC_MODE_SC) &&
      (!btm_sec_is_serv_level0(psm))) {
    security_required = btm_sec_set_serv_level4_flags(
        p_serv_rec->security_flags, is_originator);
  } else {
    security_required = p_serv_rec->security_flags;
  }

  /* there are some devices (moto phone) which connects to several services at
   * the same time */
  /* we will process one after another */
  if ((p_dev_rec->p_callback) ||
      (btm_cb.pairing_state != BTM_PAIR_STATE_IDLE)) {
    BTM_TRACE_EVENT("%s() service PSM:%d Proto:%d SCN:%d delayed  state: %s",
                    __func__, psm, mx_proto_id, mx_chan_id,
                    btm_pair_state_descr(btm_cb.pairing_state));

    rc = BTM_CMD_STARTED;

    if ((btm_cb.security_mode == BTM_SEC_MODE_UNDEFINED ||
         btm_cb.security_mode == BTM_SEC_MODE_NONE ||
         btm_cb.security_mode == BTM_SEC_MODE_SERVICE ||
         btm_cb.security_mode == BTM_SEC_MODE_LINK) ||
        (BTM_SM4_KNOWN == p_dev_rec->sm4) ||
        (BTM_SEC_IS_SM4(p_dev_rec->sm4) &&
         (!btm_sec_is_upgrade_possible(p_dev_rec, is_originator)))) {
      /* legacy mode - local is legacy or local is lisbon/peer is legacy
       * or SM4 with no possibility of link key upgrade */
      if (is_originator) {
        if (((security_required & BTM_SEC_OUT_FLAGS) == 0) ||
            ((((security_required & BTM_SEC_OUT_FLAGS) ==
               BTM_SEC_OUT_AUTHENTICATE) &&
              btm_dev_authenticated(p_dev_rec))) ||
            ((((security_required & BTM_SEC_OUT_FLAGS) ==
               (BTM_SEC_OUT_AUTHENTICATE | BTM_SEC_OUT_ENCRYPT)) &&
              btm_dev_encrypted(p_dev_rec)))) {
          rc = BTM_SUCCESS;
        }
      } else {
        if (((security_required & BTM_SEC_IN_FLAGS) == 0) ||
            ((((security_required & BTM_SEC_IN_FLAGS) ==
               BTM_SEC_IN_AUTHENTICATE) &&
              btm_dev_authenticated(p_dev_rec))) ||
            (((security_required & BTM_SEC_IN_FLAGS) == BTM_SEC_IN_AUTHORIZE) &&
             (btm_dev_authorized(p_dev_rec) ||
              btm_serv_trusted(p_dev_rec, p_serv_rec))) ||
            (((security_required & BTM_SEC_IN_FLAGS) ==
              (BTM_SEC_IN_AUTHORIZE | BTM_SEC_IN_AUTHENTICATE)) &&
             ((btm_dev_authorized(p_dev_rec) ||
               btm_serv_trusted(p_dev_rec, p_serv_rec)) &&
              btm_dev_authenticated(p_dev_rec))) ||
            (((security_required & BTM_SEC_IN_FLAGS) ==
              (BTM_SEC_IN_AUTHORIZE | BTM_SEC_IN_ENCRYPT)) &&
             ((btm_dev_authorized(p_dev_rec) ||
               btm_serv_trusted(p_dev_rec, p_serv_rec)) &&
              btm_dev_encrypted(p_dev_rec))) ||
            ((((security_required & BTM_SEC_IN_FLAGS) ==
               (BTM_SEC_IN_AUTHENTICATE | BTM_SEC_IN_ENCRYPT)) &&
              btm_dev_encrypted(p_dev_rec)))) {
          // Check for 16 digits (or MITM)
          if (((security_required & BTM_SEC_IN_MIN_16_DIGIT_PIN) == 0) ||
              (((security_required & BTM_SEC_IN_MIN_16_DIGIT_PIN) ==
                BTM_SEC_IN_MIN_16_DIGIT_PIN) &&
               btm_dev_16_digit_authenticated(p_dev_rec))) {
            rc = BTM_SUCCESS;
          }
        }
      }
      if ((rc == BTM_SUCCESS) && (security_required & BTM_SEC_MODE4_LEVEL4) &&
          (p_dev_rec->link_key_type != BTM_LKEY_TYPE_AUTH_COMB_P_256)) {
        rc = BTM_CMD_STARTED;
      }
    }

    if (rc == BTM_SUCCESS) {
      BTM_TRACE_EVENT("%s: allow to bypass, checking authorization", __func__);
      /* the security in BTM_SEC_IN_FLAGS is fullfilled so far, check the
       * requirements in */
      /* btm_sec_execute_procedure */
      if ((is_originator &&
           (p_serv_rec->security_flags & BTM_SEC_OUT_AUTHORIZE)) ||
          (!is_originator &&
           (p_serv_rec->security_flags & BTM_SEC_IN_AUTHORIZE))) {
        BTM_TRACE_EVENT("%s: still need authorization", __func__);
        rc = BTM_CMD_STARTED;
      }
    }

    /* Check whether there is a pending security procedure, if so we should
     * always queue */
    /* the new security request */
    if (p_dev_rec->sec_state != BTM_SEC_STATE_IDLE) {
      BTM_TRACE_EVENT("%s: There is a pending security procedure", __func__);
      rc = BTM_CMD_STARTED;
    }
    if (rc == BTM_CMD_STARTED) {
      BTM_TRACE_EVENT("%s: call btm_sec_queue_mx_request", __func__);
      btm_sec_queue_mx_request(bd_addr, psm, is_originator, mx_proto_id,
                               mx_chan_id, p_callback, p_ref_data);
    } else /* rc == BTM_SUCCESS */
    {
      /* access granted */
      if (p_callback) {
        (*p_callback)(&bd_addr, transport, p_ref_data, (uint8_t)rc);
      }
    }

    BTM_TRACE_EVENT("%s: return with rc = 0x%02x in delayed state %s", __func__,
                    rc, btm_pair_state_descr(btm_cb.pairing_state));
    return rc;
  }

  if ((!is_originator) && ((security_required & BTM_SEC_MODE4_LEVEL4) ||
                           (btm_cb.security_mode == BTM_SEC_MODE_SC))) {
    bool local_supports_sc =
        controller_get_interface()->supports_secure_connections();
    /* acceptor receives service connection establishment Request for */
    /* Secure Connections Only service */
    if (!(local_supports_sc) ||
        !(p_dev_rec->remote_supports_secure_connections)) {
      BTM_TRACE_DEBUG("%s: SC only service,local_support_for_sc %d,",
                      "remote_support_for_sc %d: fail pairing", __func__,
                      local_supports_sc,
                      p_dev_rec->remote_supports_secure_connections);

      if (p_callback)
        (*p_callback)(&bd_addr, transport, (void*)p_ref_data,
                      BTM_MODE4_LEVEL4_NOT_SUPPORTED);

      return (BTM_MODE4_LEVEL4_NOT_SUPPORTED);
    }
  }

  p_dev_rec->p_cur_service = p_serv_rec;
  p_dev_rec->security_required = security_required;

  if (btm_cb.security_mode == BTM_SEC_MODE_SP ||
      btm_cb.security_mode == BTM_SEC_MODE_SP_DEBUG ||
      btm_cb.security_mode == BTM_SEC_MODE_SC) {
    if (BTM_SEC_IS_SM4(p_dev_rec->sm4)) {
      if ((p_dev_rec->security_required & BTM_SEC_MODE4_LEVEL4) &&
          (p_dev_rec->link_key_type != BTM_LKEY_TYPE_AUTH_COMB_P_256)) {
        /* BTM_LKEY_TYPE_AUTH_COMB_P_256 is the only acceptable key in this case
         */
        if ((p_dev_rec->sec_flags & BTM_SEC_LINK_KEY_KNOWN) != 0) {
          p_dev_rec->sm4 |= BTM_SM4_UPGRADE;
        }

        p_dev_rec->sec_flags &=
            ~(BTM_SEC_LINK_KEY_KNOWN | BTM_SEC_LINK_KEY_AUTHED |
              BTM_SEC_AUTHENTICATED);
        BTM_TRACE_DEBUG("%s: sec_flags:0x%x", __func__, p_dev_rec->sec_flags);
      } else {
        /* If we already have a link key, check if that link key is good enough
         */
        btm_sec_check_upgrade(p_dev_rec, is_originator);
      }
    }
  }

  p_dev_rec->is_originator = is_originator;
  p_dev_rec->p_callback = p_callback;
  p_dev_rec->p_ref_data = p_ref_data;

  /* Although authentication and encryption are per connection */
  /* authorization is per access request.  For example when serial connection */
  /* is up and authorized and client requests to read file (access to other */
  /* scn, we need to request user's permission again. */
  p_dev_rec->sec_flags &= ~(BTM_SEC_AUTHORIZED);

  BTM_TRACE_EVENT(
      "%s() proto_id:%d chan_id:%d State:%d Flags:0x%x Required:0x%x Service "
      "ID:%d",
      __func__, mx_proto_id, mx_chan_id, p_dev_rec->sec_state,
      p_dev_rec->sec_flags, p_dev_rec->security_required,
      p_dev_rec->p_cur_service->service_id);

  rc = btm_sec_execute_procedure(p_dev_rec);
  if (rc != BTM_CMD_STARTED) {
    if (p_callback) {
      p_dev_rec->p_callback = NULL;
      (*p_callback)(&bd_addr, transport, p_ref_data, (uint8_t)rc);
    }
  }

  return rc;
}

/*******************************************************************************
 *
 * Function         btm_sec_conn_req
 *
 * Description      This function is when the peer device is requesting
 *                  connection
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_sec_conn_req(const RawAddress& bda, uint8_t* dc) {
  tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev(bda);

  /* Some device may request a connection before we are done with the HCI_Reset
   * sequence */
  if (!controller_get_interface()->get_is_ready()) {
    BTM_TRACE_EVENT("Security Manager: connect request when device not ready");
    btsnd_hcic_reject_conn(bda, HCI_ERR_HOST_REJECT_DEVICE);
    return;
  }

  /* Security guys wants us not to allow connection from not paired devices */

  /* Check if connection is allowed for only paired devices */
  if (btm_cb.connect_only_paired) {
    if (!p_dev_rec || !(p_dev_rec->sec_flags & BTM_SEC_LINK_KEY_AUTHED)) {
      BTM_TRACE_EVENT(
          "Security Manager: connect request from non-paired device");
      btsnd_hcic_reject_conn(bda, HCI_ERR_HOST_REJECT_DEVICE);
      return;
    }
  }

#if (BTM_ALLOW_CONN_IF_NONDISCOVER == FALSE)
  /* If non-discoverable, only allow known devices to connect */
  if (btm_cb.btm_inq_vars.discoverable_mode == BTM_NON_DISCOVERABLE) {
    if (!p_dev_rec) {
      BTM_TRACE_EVENT(
          "Security Manager: connect request from not paired device");
      btsnd_hcic_reject_conn(bda, HCI_ERR_HOST_REJECT_DEVICE);
      return;
    }
  }
#endif

  if ((btm_cb.pairing_state != BTM_PAIR_STATE_IDLE) &&
      (btm_cb.pairing_flags & BTM_PAIR_FLAGS_WE_STARTED_DD) &&
      (btm_cb.pairing_bda == bda)) {
    BTM_TRACE_EVENT(
        "Security Manager: reject connect request from bonding device");

    /* incoming connection from bonding device is rejected */
    btm_cb.pairing_flags |= BTM_PAIR_FLAGS_REJECTED_CONNECT;
    btsnd_hcic_reject_conn(bda, HCI_ERR_HOST_REJECT_DEVICE);
    return;
  }

  /* Host is not interested or approved connection.  Save BDA and DC and */
  /* pass request to L2CAP */
  btm_cb.connecting_bda = bda;
  memcpy(btm_cb.connecting_dc, dc, DEV_CLASS_LEN);

  if (l2c_link_hci_conn_req(bda)) {
    if (!p_dev_rec) {
      /* accept the connection -> allocate a device record */
      p_dev_rec = btm_sec_alloc_dev(bda);
    }
    if (p_dev_rec) {
      p_dev_rec->sm4 |= BTM_SM4_CONN_PEND;
    }
  }
}

/*******************************************************************************
 *
 * Function         btm_sec_bond_cancel_complete
 *
 * Description      This function is called to report bond cancel complete
 *                  event.
 *
 * Returns          void
 *
 ******************************************************************************/
static void btm_sec_bond_cancel_complete(void) {
  tBTM_SEC_DEV_REC* p_dev_rec;

  if ((btm_cb.pairing_flags & BTM_PAIR_FLAGS_DISC_WHEN_DONE) ||
      (BTM_PAIR_STATE_WAIT_LOCAL_PIN == btm_cb.pairing_state &&
       BTM_PAIR_FLAGS_WE_STARTED_DD & btm_cb.pairing_flags) ||
      (btm_cb.pairing_state == BTM_PAIR_STATE_GET_REM_NAME &&
       BTM_PAIR_FLAGS_WE_CANCEL_DD & btm_cb.pairing_flags)) {
    /* for dedicated bonding in legacy mode, authentication happens at "link
     * level"
     * btm_sec_connected is called with failed status.
     * In theory, the code that handles is_pairing_device/true should clean out
     * security related code.
     * However, this function may clean out the security related flags and
     * btm_sec_connected would not know
     * this function also needs to do proper clean up.
     */
    p_dev_rec = btm_find_dev(btm_cb.pairing_bda);
    if (p_dev_rec != NULL) p_dev_rec->security_required = BTM_SEC_NONE;
    btm_sec_change_pairing_state(BTM_PAIR_STATE_IDLE);

    /* Notify application that the cancel succeeded */
    if (btm_cb.api.p_bond_cancel_cmpl_callback)
      btm_cb.api.p_bond_cancel_cmpl_callback(BTM_SUCCESS);
  }
}

/*******************************************************************************
 *
 * Function         btm_create_conn_cancel_complete
 *
 * Description      This function is called when the command complete message
 *                  is received from the HCI for the create connection cancel
 *                  command.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_create_conn_cancel_complete(uint8_t* p) {
  uint8_t status;
  STREAM_TO_UINT8(status, p);
  RawAddress bd_addr;
  STREAM_TO_BDADDR(bd_addr, p);
  BTM_TRACE_EVENT("btm_create_conn_cancel_complete(): in State: %s  status:%d",
                  btm_pair_state_descr(btm_cb.pairing_state), status);
  bluetooth::common::LogLinkLayerConnectionEvent(
      &bd_addr, bluetooth::common::kUnknownConnectionHandle,
      android::bluetooth::DIRECTION_OUTGOING, android::bluetooth::LINK_TYPE_ACL,
      android::bluetooth::hci::CMD_CREATE_CONNECTION_CANCEL,
      android::bluetooth::hci::EVT_COMMAND_COMPLETE,
      android::bluetooth::hci::BLE_EVT_UNKNOWN, status,
      android::bluetooth::hci::STATUS_UNKNOWN);

  /* if the create conn cancel cmd was issued by the bond cancel,
  ** the application needs to be notified that bond cancel succeeded
  */
  switch (status) {
    case HCI_SUCCESS:
      btm_sec_bond_cancel_complete();
      break;
    case HCI_ERR_CONNECTION_EXISTS:
    case HCI_ERR_NO_CONNECTION:
    default:
      /* Notify application of the error */
      if (btm_cb.api.p_bond_cancel_cmpl_callback)
        btm_cb.api.p_bond_cancel_cmpl_callback(BTM_ERR_PROCESSING);
      break;
  }
}

/*******************************************************************************
 *
 * Function         btm_sec_check_pending_reqs
 *
 * Description      This function is called at the end of the security procedure
 *                  to let L2CAP and RFCOMM know to re-submit any pending
 *                  requests
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_sec_check_pending_reqs(void) {
  if (btm_cb.pairing_state == BTM_PAIR_STATE_IDLE) {
    /* First, resubmit L2CAP requests */
    if (btm_cb.sec_req_pending) {
      btm_cb.sec_req_pending = false;
      l2cu_resubmit_pending_sec_req(nullptr);
    }

    /* Now, re-submit anything in the mux queue */
    fixed_queue_t* bq = btm_cb.sec_pending_q;

    btm_cb.sec_pending_q = fixed_queue_new(SIZE_MAX);

    tBTM_SEC_QUEUE_ENTRY* p_e;
    while ((p_e = (tBTM_SEC_QUEUE_ENTRY*)fixed_queue_try_dequeue(bq)) != NULL) {
      /* Check that the ACL is still up before starting security procedures */
      if (btm_bda_to_acl(p_e->bd_addr, p_e->transport) != NULL) {
        if (p_e->psm != 0) {
          BTM_TRACE_EVENT(
              "%s PSM:0x%04x Is_Orig:%u mx_proto_id:%u mx_chan_id:%u", __func__,
              p_e->psm, p_e->is_orig, p_e->mx_proto_id, p_e->mx_chan_id);

          btm_sec_mx_access_request(p_e->bd_addr, p_e->psm, p_e->is_orig,
                                    p_e->mx_proto_id, p_e->mx_chan_id,
                                    p_e->p_callback, p_e->p_ref_data);
        } else {
          BTM_SetEncryption(p_e->bd_addr, p_e->transport, p_e->p_callback,
                            p_e->p_ref_data, p_e->sec_act);
        }
      }

      osi_free(p_e);
    }
    fixed_queue_free(bq, NULL);
  }
}

/*******************************************************************************
 *
 * Function         btm_sec_init
 *
 * Description      This function is on the SEC startup
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_sec_init(uint8_t sec_mode) {
  btm_cb.security_mode = sec_mode;
  btm_cb.pairing_bda = RawAddress::kAny;
}

/*******************************************************************************
 *
 * Function         btm_sec_device_down
 *
 * Description      This function should be called when device is disabled or
 *                  turned off
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_sec_device_down(void) {
  BTM_TRACE_EVENT("%s() State: %s", __func__,
                  btm_pair_state_descr(btm_cb.pairing_state));
  btm_sec_change_pairing_state(BTM_PAIR_STATE_IDLE);
}

/*******************************************************************************
 *
 * Function         btm_sec_dev_reset
 *
 * Description      This function should be called after device reset
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_sec_dev_reset(void) {
  if (controller_get_interface()->supports_simple_pairing()) {
    /* set the default IO capabilities */
    btm_cb.devcb.loc_io_caps = btif_storage_get_local_io_caps();
    /* add mx service to use no security */
    BTM_SetSecurityLevel(false, "RFC_MUX", BTM_SEC_SERVICE_RFC_MUX,
                         BTM_SEC_NONE, BT_PSM_RFCOMM, BTM_SEC_PROTO_RFCOMM, 0);
  } else {
    btm_cb.security_mode = BTM_SEC_MODE_SERVICE;
  }

  BTM_TRACE_DEBUG("btm_sec_dev_reset sec mode: %d", btm_cb.security_mode);
}

/*******************************************************************************
 *
 * Function         btm_sec_abort_access_req
 *
 * Description      This function is called by the L2CAP or RFCOMM to abort
 *                  the pending operation.
 *
 * Parameters:      bd_addr       - Address of the peer device
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_sec_abort_access_req(const RawAddress& bd_addr) {
  tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev(bd_addr);

  if (!p_dev_rec) return;

  if ((p_dev_rec->sec_state != BTM_SEC_STATE_AUTHORIZING) &&
      (p_dev_rec->sec_state != BTM_SEC_STATE_AUTHENTICATING))
    return;

  p_dev_rec->sec_state = BTM_SEC_STATE_IDLE;

  BTM_TRACE_DEBUG("%s: clearing callback. p_dev_rec=%p, p_callback=%p",
                  __func__, p_dev_rec, p_dev_rec->p_callback);
  p_dev_rec->p_callback = NULL;
}

/*******************************************************************************
 *
 * Function         btm_sec_dd_create_conn
 *
 * Description      This function is called to create the ACL connection for
 *                  the dedicated boding process
 *
 * Returns          void
 *
 ******************************************************************************/
static tBTM_STATUS btm_sec_dd_create_conn(tBTM_SEC_DEV_REC* p_dev_rec) {
  tL2C_LCB* p_lcb =
      l2cu_find_lcb_by_bd_addr(p_dev_rec->bd_addr, BT_TRANSPORT_BR_EDR);
  if (p_lcb && (p_lcb->link_state == LST_CONNECTED ||
                p_lcb->link_state == LST_CONNECTING)) {
    BTM_TRACE_WARNING("%s Connection already exists", __func__);
    btm_sec_change_pairing_state(BTM_PAIR_STATE_WAIT_PIN_REQ);
    return BTM_CMD_STARTED;
  }

  /* Make sure an L2cap link control block is available */
  if (!p_lcb && (p_lcb = l2cu_allocate_lcb(p_dev_rec->bd_addr, true,
                                           BT_TRANSPORT_BR_EDR)) == NULL) {
    LOG(WARNING) << "Security Manager: failed allocate LCB "
                 << p_dev_rec->bd_addr;

    return (BTM_NO_RESOURCES);
  }

  /* set up the control block to indicated dedicated bonding */
  btm_cb.pairing_flags |= BTM_PAIR_FLAGS_DISC_WHEN_DONE;

  if (!l2cu_create_conn_br_edr(p_lcb)) {
    LOG(WARNING) << "Security Manager: failed create allocate LCB "
                 << p_dev_rec->bd_addr;

    l2cu_release_lcb(p_lcb);
    return (BTM_NO_RESOURCES);
  }

  btm_acl_update_busy_level(BTM_BLI_PAGE_EVT);

  VLOG(1) << "Security Manager: " << p_dev_rec->bd_addr;

  btm_sec_change_pairing_state(BTM_PAIR_STATE_WAIT_PIN_REQ);

  return (BTM_CMD_STARTED);
}

bool is_state_getting_name(void* data, void* context) {
  tBTM_SEC_DEV_REC* p_dev_rec = static_cast<tBTM_SEC_DEV_REC*>(data);

  if (p_dev_rec->sec_state == BTM_SEC_STATE_GETTING_NAME) {
    return false;
  }
  return true;
}

/*******************************************************************************
 *
 * Function         btm_sec_rmt_name_request_complete
 *
 * Description      This function is called when remote name was obtained from
 *                  the peer device
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_sec_rmt_name_request_complete(const RawAddress* p_bd_addr,
                                       uint8_t* p_bd_name, uint8_t status) {
  tBTM_SEC_DEV_REC* p_dev_rec;
  int i;
  DEV_CLASS dev_class;
  uint8_t old_sec_state;

  BTM_TRACE_EVENT("btm_sec_rmt_name_request_complete");
  if ((!p_bd_addr && !BTM_ACL_IS_CONNECTED(btm_cb.connecting_bda)) ||
      (p_bd_addr && !BTM_ACL_IS_CONNECTED(*p_bd_addr))) {
    btm_acl_resubmit_page();
  }

  /* If remote name request failed, p_bd_addr is null and we need to search */
  /* based on state assuming that we are doing 1 at a time */
  if (p_bd_addr)
    p_dev_rec = btm_find_dev(*p_bd_addr);
  else {
    list_node_t* node =
        list_foreach(btm_cb.sec_dev_rec, is_state_getting_name, NULL);
    if (node != NULL) {
      p_dev_rec = static_cast<tBTM_SEC_DEV_REC*>(list_node(node));
      p_bd_addr = &p_dev_rec->bd_addr;
    } else {
      p_dev_rec = NULL;
    }
  }

  /* Commenting out trace due to obf/compilation problems.
   */
  if (!p_bd_name) p_bd_name = (uint8_t*)"";

  if (p_dev_rec) {
    BTM_TRACE_EVENT(
        "%s PairState: %s  RemName: %s  status: %d State:%d  p_dev_rec: "
        "0x%08x ",
        __func__, btm_pair_state_descr(btm_cb.pairing_state), p_bd_name, status,
        p_dev_rec->sec_state, p_dev_rec);
  } else {
    BTM_TRACE_EVENT("%s PairState: %s  RemName: %s  status: %d", __func__,
                    btm_pair_state_descr(btm_cb.pairing_state), p_bd_name,
                    status);
  }

  if (p_dev_rec) {
    old_sec_state = p_dev_rec->sec_state;
    if (status == HCI_SUCCESS) {
      strlcpy((char*)p_dev_rec->sec_bd_name, (char*)p_bd_name,
              BTM_MAX_REM_BD_NAME_LEN);
      p_dev_rec->sec_flags |= BTM_SEC_NAME_KNOWN;
      BTM_TRACE_EVENT("setting BTM_SEC_NAME_KNOWN sec_flags:0x%x",
                      p_dev_rec->sec_flags);
    } else {
      /* Notify all clients waiting for name to be resolved even if it failed so
       * clients can continue */
      p_dev_rec->sec_bd_name[0] = 0;
    }

    if (p_dev_rec->sec_state == BTM_SEC_STATE_GETTING_NAME)
      p_dev_rec->sec_state = BTM_SEC_STATE_IDLE;

    /* Notify all clients waiting for name to be resolved */
    for (i = 0; i < BTM_SEC_MAX_RMT_NAME_CALLBACKS; i++) {
      if (btm_cb.p_rmt_name_callback[i] && p_bd_addr)
        (*btm_cb.p_rmt_name_callback[i])(*p_bd_addr, p_dev_rec->dev_class,
                                         p_dev_rec->sec_bd_name);
    }
  } else {
    dev_class[0] = 0;
    dev_class[1] = 0;
    dev_class[2] = 0;

    /* Notify all clients waiting for name to be resolved even if not found so
     * clients can continue */
    for (i = 0; i < BTM_SEC_MAX_RMT_NAME_CALLBACKS; i++) {
      if (btm_cb.p_rmt_name_callback[i] && p_bd_addr)
        (*btm_cb.p_rmt_name_callback[i])(*p_bd_addr, dev_class, (uint8_t*)"");
    }

    return;
  }

  /* If we were delaying asking UI for a PIN because name was not resolved, ask
   * now */
  if ((btm_cb.pairing_state == BTM_PAIR_STATE_WAIT_LOCAL_PIN) && p_bd_addr &&
      (btm_cb.pairing_bda == *p_bd_addr)) {
    BTM_TRACE_EVENT(
        "%s() delayed pin now being requested flags:0x%x, "
        "(p_pin_callback=0x%p)",
        __func__, btm_cb.pairing_flags, btm_cb.api.p_pin_callback);

    if ((btm_cb.pairing_flags & BTM_PAIR_FLAGS_PIN_REQD) == 0 &&
        btm_cb.api.p_pin_callback) {
      BTM_TRACE_EVENT("%s() calling pin_callback", __func__);
      btm_cb.pairing_flags |= BTM_PAIR_FLAGS_PIN_REQD;
      (*btm_cb.api.p_pin_callback)(
          p_dev_rec->bd_addr, p_dev_rec->dev_class, p_bd_name,
          (p_dev_rec->p_cur_service == NULL)
              ? false
              : (p_dev_rec->p_cur_service->security_flags &
                 BTM_SEC_IN_MIN_16_DIGIT_PIN));
    }

    /* Set the same state again to force the timer to be restarted */
    btm_sec_change_pairing_state(BTM_PAIR_STATE_WAIT_LOCAL_PIN);
    return;
  }

  /* Check if we were delaying bonding because name was not resolved */
  if (btm_cb.pairing_state == BTM_PAIR_STATE_GET_REM_NAME) {
    if (p_bd_addr && btm_cb.pairing_bda == *p_bd_addr) {
      BTM_TRACE_EVENT("%s() continue bonding sm4: 0x%04x, status:0x%x",
                      __func__, p_dev_rec->sm4, status);
      if (btm_cb.pairing_flags & BTM_PAIR_FLAGS_WE_CANCEL_DD) {
        btm_sec_bond_cancel_complete();
        return;
      }

      if (status != HCI_SUCCESS) {
        btm_sec_change_pairing_state(BTM_PAIR_STATE_IDLE);

        if (btm_cb.api.p_auth_complete_callback)
          (*btm_cb.api.p_auth_complete_callback)(
              p_dev_rec->bd_addr, p_dev_rec->dev_class, p_dev_rec->sec_bd_name,
              status);
        return;
      }

      /* if peer is very old legacy devices, HCI_RMT_HOST_SUP_FEAT_NOTIFY_EVT is
       * not reported */
      if (BTM_SEC_IS_SM4_UNKNOWN(p_dev_rec->sm4)) {
        /* set the KNOWN flag only if BTM_PAIR_FLAGS_REJECTED_CONNECT is not
         * set.*/
        /* If it is set, there may be a race condition */
        BTM_TRACE_DEBUG("%s IS_SM4_UNKNOWN Flags:0x%04x", __func__,
                        btm_cb.pairing_flags);
        if ((btm_cb.pairing_flags & BTM_PAIR_FLAGS_REJECTED_CONNECT) == 0)
          p_dev_rec->sm4 |= BTM_SM4_KNOWN;
      }

      BTM_TRACE_DEBUG("%s, SM4 Value: %x, Legacy:%d,IS SM4:%d, Unknown:%d",
                      __func__, p_dev_rec->sm4,
                      BTM_SEC_IS_SM4_LEGACY(p_dev_rec->sm4),
                      BTM_SEC_IS_SM4(p_dev_rec->sm4),
                      BTM_SEC_IS_SM4_UNKNOWN(p_dev_rec->sm4));

      /* BT 2.1 or carkit, bring up the connection to force the peer to request
       *PIN.
       ** Else prefetch (btm_sec_check_prefetch_pin will do the prefetching if
       *needed)
       */
      if ((p_dev_rec->sm4 != BTM_SM4_KNOWN) ||
          !btm_sec_check_prefetch_pin(p_dev_rec)) {
        /* if we rejected incoming connection request, we have to wait
         * HCI_Connection_Complete event */
        /*  before originating  */
        if (btm_cb.pairing_flags & BTM_PAIR_FLAGS_REJECTED_CONNECT) {
          BTM_TRACE_WARNING(
              "%s: waiting HCI_Connection_Complete after rejecting connection",
              __func__);
        }
        /* Both we and the peer are 2.1 - continue to create connection */
        else if (btm_sec_dd_create_conn(p_dev_rec) != BTM_CMD_STARTED) {
          BTM_TRACE_WARNING("%s: failed to start connection", __func__);

          btm_sec_change_pairing_state(BTM_PAIR_STATE_IDLE);

          if (btm_cb.api.p_auth_complete_callback) {
            (*btm_cb.api.p_auth_complete_callback)(
                p_dev_rec->bd_addr, p_dev_rec->dev_class,
                p_dev_rec->sec_bd_name, HCI_ERR_MEMORY_FULL);
          }
        }
      }
      return;
    } else {
      BTM_TRACE_WARNING("%s: wrong BDA, retry with pairing BDA", __func__);
      if (BTM_ReadRemoteDeviceName(btm_cb.pairing_bda, NULL,
                                   BT_TRANSPORT_BR_EDR) != BTM_CMD_STARTED) {
        BTM_TRACE_ERROR("%s: failed to start remote name request", __func__);
        if (btm_cb.api.p_auth_complete_callback) {
          (*btm_cb.api.p_auth_complete_callback)(
              p_dev_rec->bd_addr, p_dev_rec->dev_class, p_dev_rec->sec_bd_name,
              HCI_ERR_MEMORY_FULL);
        }
      };
      return;
    }
  }

  /* check if we were delaying link_key_callback because name was not resolved
   */
  if (p_dev_rec->link_key_not_sent) {
    /* If HCI connection complete has not arrived, wait for it */
    if (p_dev_rec->hci_handle == BTM_SEC_INVALID_HANDLE) return;

    p_dev_rec->link_key_not_sent = false;
    btm_send_link_key_notif(p_dev_rec);

    /* If its not us who perform authentication, we should tell stackserver */
    /* that some authentication has been completed                          */
    /* This is required when different entities receive link notification and
     * auth complete */
    if (!(p_dev_rec->security_required & BTM_SEC_OUT_AUTHENTICATE)) {
      if (btm_cb.api.p_auth_complete_callback)
        (*btm_cb.api.p_auth_complete_callback)(
            p_dev_rec->bd_addr, p_dev_rec->dev_class, p_dev_rec->sec_bd_name,
            HCI_SUCCESS);
    }
  }

  /* If this is a bonding procedure can disconnect the link now */
  if ((btm_cb.pairing_flags & BTM_PAIR_FLAGS_WE_STARTED_DD) &&
      (p_dev_rec->sec_flags & BTM_SEC_AUTHENTICATED)) {
    BTM_TRACE_WARNING("btm_sec_rmt_name_request_complete (none/ce)");
    p_dev_rec->security_required &= ~(BTM_SEC_OUT_AUTHENTICATE);
    l2cu_start_post_bond_timer(p_dev_rec->hci_handle);
    return;
  }

  if (old_sec_state != BTM_SEC_STATE_GETTING_NAME) return;

  /* If get name failed, notify the waiting layer */
  if (status != HCI_SUCCESS) {
    btm_sec_dev_rec_cback_event(p_dev_rec, BTM_ERR_PROCESSING, false);
    return;
  }

  if (p_dev_rec->sm4 & BTM_SM4_REQ_PEND) {
    BTM_TRACE_EVENT("waiting for remote features!!");
    return;
  }

  /* Remote Name succeeded, execute the next security procedure, if any */
  status = (uint8_t)btm_sec_execute_procedure(p_dev_rec);

  /* If result is pending reply from the user or from the device is pending */
  if (status == BTM_CMD_STARTED) return;

  /* There is no next procedure or start of procedure failed, notify the waiting
   * layer */
  btm_sec_dev_rec_cback_event(p_dev_rec, status, false);
}

/*******************************************************************************
 *
 * Function         btm_sec_rmt_host_support_feat_evt
 *
 * Description      This function is called when the
 *                  HCI_RMT_HOST_SUP_FEAT_NOTIFY_EVT is received
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_sec_rmt_host_support_feat_evt(uint8_t* p) {
  tBTM_SEC_DEV_REC* p_dev_rec;
  RawAddress bd_addr; /* peer address */
  BD_FEATURES features;

  STREAM_TO_BDADDR(bd_addr, p);
  p_dev_rec = btm_find_or_alloc_dev(bd_addr);

  BTM_TRACE_EVENT("btm_sec_rmt_host_support_feat_evt  sm4: 0x%x  p[0]: 0x%x",
                  p_dev_rec->sm4, p[0]);

  if (BTM_SEC_IS_SM4_UNKNOWN(p_dev_rec->sm4)) {
    p_dev_rec->sm4 = BTM_SM4_KNOWN;
    STREAM_TO_ARRAY(features, p, HCI_FEATURE_BYTES_PER_PAGE);
    if (HCI_SSP_HOST_SUPPORTED(features)) {
      p_dev_rec->sm4 = BTM_SM4_TRUE;
    }
    BTM_TRACE_EVENT(
        "btm_sec_rmt_host_support_feat_evt sm4: 0x%x features[0]: 0x%x",
        p_dev_rec->sm4, features[0]);
  }
}

/*******************************************************************************
 *
 * Function         btm_io_capabilities_req
 *
 * Description      This function is called when LM request for the IO
 *                  capability of the local device and
 *                  if the OOB data is present for the device in the event
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_io_capabilities_req(const RawAddress& p) {
  tBTM_SP_IO_REQ evt_data;
  uint8_t err_code = 0;
  tBTM_SEC_DEV_REC* p_dev_rec;
  bool is_orig = true;
  uint8_t callback_rc = BTM_SUCCESS;

  evt_data.bd_addr = p;

  /* setup the default response according to compile options */
  /* assume that the local IO capability does not change
   * loc_io_caps is initialized with the default value */
  evt_data.io_cap = btm_cb.devcb.loc_io_caps;
  evt_data.oob_data = BTM_OOB_NONE;
  evt_data.auth_req = BTM_DEFAULT_AUTH_REQ;

  BTM_TRACE_EVENT("%s: State: %s", __func__,
                  btm_pair_state_descr(btm_cb.pairing_state));

  p_dev_rec = btm_find_or_alloc_dev(evt_data.bd_addr);

  BTM_TRACE_DEBUG("%s:Security mode: %d, Num Read Remote Feat pages: %d",
                  __func__, btm_cb.security_mode, p_dev_rec->num_read_pages);

  if ((btm_cb.security_mode == BTM_SEC_MODE_SC) &&
      (p_dev_rec->num_read_pages == 0)) {
    BTM_TRACE_EVENT("%s: Device security mode is SC only.",
                    "To continue need to know remote features.", __func__);

    p_dev_rec->remote_features_needed = true;
    return;
  }

  p_dev_rec->sm4 |= BTM_SM4_TRUE;

  BTM_TRACE_EVENT("%s: State: %s  Flags: 0x%04x  p_cur_service: 0x%08x",
                  __func__, btm_pair_state_descr(btm_cb.pairing_state),
                  btm_cb.pairing_flags, p_dev_rec->p_cur_service);

  if (p_dev_rec->p_cur_service) {
    BTM_TRACE_EVENT("%s: cur_service psm: 0x%04x, security_flags: 0x%04x",
                    __func__, p_dev_rec->p_cur_service->psm,
                    p_dev_rec->p_cur_service->security_flags);
  }

  switch (btm_cb.pairing_state) {
    /* initiator connecting */
    case BTM_PAIR_STATE_IDLE:
      // TODO: Handle Idle pairing state
      // security_required = p_dev_rec->security_required;
      break;

    /* received IO capability response already->acceptor */
    case BTM_PAIR_STATE_INCOMING_SSP:
      is_orig = false;

      if (btm_cb.pairing_flags & BTM_PAIR_FLAGS_PEER_STARTED_DD) {
        /* acceptor in dedicated bonding */
        evt_data.auth_req = BTM_DEFAULT_DD_AUTH_REQ;
      }
      break;

    /* initiator, at this point it is expected to be dedicated bonding
    initiated by local device */
    case BTM_PAIR_STATE_WAIT_PIN_REQ:
      if (evt_data.bd_addr == btm_cb.pairing_bda) {
        evt_data.auth_req = BTM_DEFAULT_DD_AUTH_REQ;
      } else {
        err_code = HCI_ERR_HOST_BUSY_PAIRING;
      }
      break;

    /* any other state is unexpected */
    default:
      err_code = HCI_ERR_HOST_BUSY_PAIRING;
      BTM_TRACE_ERROR("%s: Unexpected Pairing state received %d", __func__,
                      btm_cb.pairing_state);
      break;
  }

  if (btm_cb.pairing_disabled) {
    /* pairing is not allowed */
    BTM_TRACE_DEBUG("%s: Pairing is not allowed -> fail pairing.", __func__);
    err_code = HCI_ERR_PAIRING_NOT_ALLOWED;
  } else if (btm_cb.security_mode == BTM_SEC_MODE_SC) {
    bool local_supports_sc =
        controller_get_interface()->supports_secure_connections();
    /* device in Secure Connections Only mode */
    if (!(local_supports_sc) ||
        !(p_dev_rec->remote_supports_secure_connections)) {
      BTM_TRACE_DEBUG("%s: SC only service, local_support_for_sc %d,",
                      " remote_support_for_sc 0x%02x -> fail pairing", __func__,
                      local_supports_sc,
                      p_dev_rec->remote_supports_secure_connections);

      err_code = HCI_ERR_PAIRING_NOT_ALLOWED;
    }
  }

  if (err_code != 0) {
    btsnd_hcic_io_cap_req_neg_reply(evt_data.bd_addr, err_code);
    return;
  }

  evt_data.is_orig = is_orig;

  if (is_orig) {
    /* local device initiated the pairing non-bonding -> use p_cur_service */
    if (!(btm_cb.pairing_flags & BTM_PAIR_FLAGS_WE_STARTED_DD) &&
        p_dev_rec->p_cur_service &&
        (p_dev_rec->p_cur_service->security_flags & BTM_SEC_OUT_AUTHENTICATE)) {
      if (btm_cb.security_mode == BTM_SEC_MODE_SC) {
        /* SC only mode device requires MITM protection */
        evt_data.auth_req = BTM_AUTH_SP_YES;
      } else {
        evt_data.auth_req =
            (p_dev_rec->p_cur_service->security_flags & BTM_SEC_OUT_MITM)
                ? BTM_AUTH_SP_YES
                : BTM_AUTH_SP_NO;
      }
    }
  }

  /* Notify L2CAP to increase timeout */
  l2c_pin_code_request(evt_data.bd_addr);

  btm_cb.pairing_bda = evt_data.bd_addr;

  if (evt_data.bd_addr == btm_cb.connecting_bda)
    memcpy(p_dev_rec->dev_class, btm_cb.connecting_dc, DEV_CLASS_LEN);

  btm_sec_change_pairing_state(BTM_PAIR_STATE_WAIT_LOCAL_IOCAPS);

  callback_rc = BTM_SUCCESS;
  if (p_dev_rec->sm4 & BTM_SM4_UPGRADE) {
    p_dev_rec->sm4 &= ~BTM_SM4_UPGRADE;

    /* link key upgrade: always use SPGB_YES - assuming we want to save the link
     * key */
    evt_data.auth_req = BTM_AUTH_SPGB_YES;
  } else if (btm_cb.api.p_sp_callback) {
    /* the callback function implementation may change the IO capability... */
    callback_rc = (*btm_cb.api.p_sp_callback)(BTM_SP_IO_REQ_EVT,
                                              (tBTM_SP_EVT_DATA*)&evt_data);
  }

  if ((callback_rc == BTM_SUCCESS) || (BTM_OOB_UNKNOWN != evt_data.oob_data)) {
    if ((btm_cb.pairing_flags & BTM_PAIR_FLAGS_WE_STARTED_DD)) {
      evt_data.auth_req =
          (BTM_AUTH_DD_BOND | (evt_data.auth_req & BTM_AUTH_YN_BIT));
    }

    if (btm_cb.security_mode == BTM_SEC_MODE_SC) {
      /* At this moment we know that both sides are SC capable, device in */
      /* SC only mode requires MITM for any service so let's set MITM bit */
      evt_data.auth_req |= BTM_AUTH_YN_BIT;
      BTM_TRACE_DEBUG(
          "%s: for device in \"SC only\" mode set auth_req to 0x%02x", __func__,
          evt_data.auth_req);
    }

    /* if the user does not indicate "reply later" by setting the oob_data to
     * unknown */
    /* send the response right now. Save the current IO capability in the
     * control block */
    btm_cb.devcb.loc_auth_req = evt_data.auth_req;
    btm_cb.devcb.loc_io_caps = evt_data.io_cap;

    BTM_TRACE_EVENT("%s: State: %s  IO_CAP:%d oob_data:%d auth_req:%d",
                    __func__, btm_pair_state_descr(btm_cb.pairing_state),
                    evt_data.io_cap, evt_data.oob_data, evt_data.auth_req);

    btsnd_hcic_io_cap_req_reply(evt_data.bd_addr, evt_data.io_cap,
                                evt_data.oob_data, evt_data.auth_req);
  }
}

/*******************************************************************************
 *
 * Function         btm_io_capabilities_rsp
 *
 * Description      This function is called when the IO capability of the
 *                  specified device is received
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_io_capabilities_rsp(uint8_t* p) {
  tBTM_SEC_DEV_REC* p_dev_rec;
  tBTM_SP_IO_RSP evt_data;

  STREAM_TO_BDADDR(evt_data.bd_addr, p);
  STREAM_TO_UINT8(evt_data.io_cap, p);
  STREAM_TO_UINT8(evt_data.oob_data, p);
  STREAM_TO_UINT8(evt_data.auth_req, p);

  /* Allocate a new device record or reuse the oldest one */
  p_dev_rec = btm_find_or_alloc_dev(evt_data.bd_addr);

  /* If no security is in progress, this indicates incoming security */
  if (btm_cb.pairing_state == BTM_PAIR_STATE_IDLE) {
    btm_cb.pairing_bda = evt_data.bd_addr;

    btm_sec_change_pairing_state(BTM_PAIR_STATE_INCOMING_SSP);

    /* Make sure we reset the trusted mask to help against attacks */
    BTM_SEC_CLR_TRUSTED_DEVICE(p_dev_rec->trusted_mask);

    /* work around for FW bug */
    btm_inq_stop_on_ssp();
  }

  /* Notify L2CAP to increase timeout */
  l2c_pin_code_request(evt_data.bd_addr);

  /* We must have a device record here.
   * Use the connecting device's CoD for the connection */
  if (evt_data.bd_addr == btm_cb.connecting_bda)
    memcpy(p_dev_rec->dev_class, btm_cb.connecting_dc, DEV_CLASS_LEN);

  /* peer sets dedicated bonding bit and we did not initiate dedicated bonding
   */
  if (btm_cb.pairing_state ==
          BTM_PAIR_STATE_INCOMING_SSP /* peer initiated bonding */
      && (evt_data.auth_req &
          BTM_AUTH_DD_BOND)) /* and dedicated bonding bit is set */
  {
    btm_cb.pairing_flags |= BTM_PAIR_FLAGS_PEER_STARTED_DD;
  }

  /* save the IO capability in the device record */
  p_dev_rec->rmt_io_caps = evt_data.io_cap;
  p_dev_rec->rmt_auth_req = evt_data.auth_req;

  if (btm_cb.api.p_sp_callback)
    (*btm_cb.api.p_sp_callback)(BTM_SP_IO_RSP_EVT,
                                (tBTM_SP_EVT_DATA*)&evt_data);
}

/*******************************************************************************
 *
 * Function         btm_proc_sp_req_evt
 *
 * Description      This function is called to process/report
 *                  HCI_USER_CONFIRMATION_REQUEST_EVT
 *                  or HCI_USER_PASSKEY_REQUEST_EVT
 *                  or HCI_USER_PASSKEY_NOTIFY_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_proc_sp_req_evt(tBTM_SP_EVT event, uint8_t* p) {
  tBTM_STATUS status = BTM_ERR_PROCESSING;
  tBTM_SP_EVT_DATA evt_data;
  RawAddress& p_bda = evt_data.cfm_req.bd_addr;
  tBTM_SEC_DEV_REC* p_dev_rec;

  /* All events start with bd_addr */
  STREAM_TO_BDADDR(p_bda, p);

  VLOG(2) << " BDA: " << p_bda << " event: 0x" << std::hex << +event
          << " State: " << btm_pair_state_descr(btm_cb.pairing_state);

  p_dev_rec = btm_find_dev(p_bda);
  if ((p_dev_rec != NULL) && (btm_cb.pairing_state != BTM_PAIR_STATE_IDLE) &&
      (btm_cb.pairing_bda == p_bda)) {
    evt_data.cfm_req.bd_addr = p_dev_rec->bd_addr;
    memcpy(evt_data.cfm_req.dev_class, p_dev_rec->dev_class, DEV_CLASS_LEN);

    strlcpy((char*)evt_data.cfm_req.bd_name, (char*)p_dev_rec->sec_bd_name,
            BTM_MAX_REM_BD_NAME_LEN);

    switch (event) {
      case BTM_SP_CFM_REQ_EVT:
        /* Numeric confirmation. Need user to conf the passkey */
        btm_sec_change_pairing_state(BTM_PAIR_STATE_WAIT_NUMERIC_CONFIRM);

        /* The device record must be allocated in the "IO cap exchange" step */
        STREAM_TO_UINT32(evt_data.cfm_req.num_val, p);
        BTM_TRACE_DEBUG("BTM_SP_CFM_REQ_EVT:  num_val: %u",
                        evt_data.cfm_req.num_val);

        evt_data.cfm_req.just_works = true;

        /* process user confirm req in association with the auth_req param */
        if (btm_cb.devcb.loc_io_caps == BTM_IO_CAP_IO) {
          if (p_dev_rec->rmt_io_caps == BTM_IO_CAP_UNKNOWN) {
            BTM_TRACE_ERROR(
                "%s did not receive IO cap response prior"
                " to BTM_SP_CFM_REQ_EVT, failing pairing request",
                __func__);
            status = BTM_WRONG_MODE;
            BTM_ConfirmReqReply(status, p_bda);
            return;
          }
          if ((p_dev_rec->rmt_io_caps == BTM_IO_CAP_IO) &&
              (btm_cb.devcb.loc_io_caps == BTM_IO_CAP_IO) &&
              ((p_dev_rec->rmt_auth_req & BTM_AUTH_SP_YES) ||
               (btm_cb.devcb.loc_auth_req & BTM_AUTH_SP_YES))) {
            /* Both devices are DisplayYesNo and one or both devices want to
               authenticate -> use authenticated link key */
            evt_data.cfm_req.just_works = false;
          }
        }

        BTM_TRACE_DEBUG(
            "btm_proc_sp_req_evt()  just_works:%d, io loc:%d, rmt:%d, auth "
            "loc:%d, rmt:%d",
            evt_data.cfm_req.just_works, btm_cb.devcb.loc_io_caps,
            p_dev_rec->rmt_io_caps, btm_cb.devcb.loc_auth_req,
            p_dev_rec->rmt_auth_req);

        evt_data.cfm_req.loc_auth_req = btm_cb.devcb.loc_auth_req;
        evt_data.cfm_req.rmt_auth_req = p_dev_rec->rmt_auth_req;
        evt_data.cfm_req.loc_io_caps = btm_cb.devcb.loc_io_caps;
        evt_data.cfm_req.rmt_io_caps = p_dev_rec->rmt_io_caps;
        break;

      case BTM_SP_KEY_NOTIF_EVT:
        /* Passkey notification (other side is a keyboard) */
        STREAM_TO_UINT32(evt_data.key_notif.passkey, p);
        BTM_TRACE_DEBUG("BTM_SP_KEY_NOTIF_EVT:  passkey: %u",
                        evt_data.key_notif.passkey);

        btm_sec_change_pairing_state(BTM_PAIR_STATE_WAIT_AUTH_COMPLETE);
        break;

      case BTM_SP_KEY_REQ_EVT:
        if (btm_cb.devcb.loc_io_caps != BTM_IO_CAP_NONE) {
          /* HCI_USER_PASSKEY_REQUEST_EVT */
          btm_sec_change_pairing_state(BTM_PAIR_STATE_KEY_ENTRY);
        }
        break;
    }

    if (btm_cb.api.p_sp_callback) {
      status = (*btm_cb.api.p_sp_callback)(event, &evt_data);
      if (status != BTM_NOT_AUTHORIZED) {
        return;
      }
      /* else BTM_NOT_AUTHORIZED means when the app wants to reject the req
       * right now */
    } else if ((event == BTM_SP_CFM_REQ_EVT) && (evt_data.cfm_req.just_works)) {
      /* automatically reply with just works if no sp_cback */
      status = BTM_SUCCESS;
    }

    if (event == BTM_SP_CFM_REQ_EVT) {
      BTM_TRACE_DEBUG("calling BTM_ConfirmReqReply with status: %d", status);
      BTM_ConfirmReqReply(status, p_bda);
    } else if (btm_cb.devcb.loc_io_caps != BTM_IO_CAP_NONE &&
               event == BTM_SP_KEY_REQ_EVT) {
      BTM_PasskeyReqReply(status, p_bda, 0);
    }
    return;
  }

  /* Something bad. we can only fail this connection */
  btm_cb.acl_disc_reason = HCI_ERR_HOST_REJECT_SECURITY;

  if (BTM_SP_CFM_REQ_EVT == event) {
    btsnd_hcic_user_conf_reply(p_bda, false);
  } else if (BTM_SP_KEY_NOTIF_EVT == event) {
    /* do nothing -> it very unlikely to happen.
    This event is most likely to be received by a HID host when it first
    connects to a HID device.
    Usually the Host initiated the connection in this case.
    On Mobile platforms, if there's a security process happening,
    the host probably can not initiate another connection.
    BTW (PC) is another story.  */
    p_dev_rec = btm_find_dev(p_bda);
    if (p_dev_rec != NULL) {
      btm_sec_disconnect(p_dev_rec->hci_handle, HCI_ERR_AUTH_FAILURE);
    }
  } else if (btm_cb.devcb.loc_io_caps != BTM_IO_CAP_NONE) {
    btsnd_hcic_user_passkey_neg_reply(p_bda);
  }
}

/*******************************************************************************
 *
 * Function         btm_keypress_notif_evt
 *
 * Description      This function is called when a key press notification is
 *                  received
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_keypress_notif_evt(uint8_t* p) {
  tBTM_SP_KEYPRESS evt_data;

  /* parse & report BTM_SP_KEYPRESS_EVT */
  if (btm_cb.api.p_sp_callback) {
    RawAddress& p_bda = evt_data.bd_addr;

    STREAM_TO_BDADDR(p_bda, p);
    evt_data.notif_type = *p;

    (*btm_cb.api.p_sp_callback)(BTM_SP_KEYPRESS_EVT,
                                (tBTM_SP_EVT_DATA*)&evt_data);
  }
}

/*******************************************************************************
 *
 * Function         btm_simple_pair_complete
 *
 * Description      This function is called when simple pairing process is
 *                  complete
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_simple_pair_complete(uint8_t* p) {
  tBTM_SP_COMPLT evt_data;
  tBTM_SEC_DEV_REC* p_dev_rec;
  uint8_t status;
  bool disc = false;

  status = *p++;
  STREAM_TO_BDADDR(evt_data.bd_addr, p);

  p_dev_rec = btm_find_dev(evt_data.bd_addr);
  if (p_dev_rec == NULL) {
    LOG(ERROR) << __func__ << " with unknown BDA: " << evt_data.bd_addr;
    return;
  }

  BTM_TRACE_EVENT(
      "btm_simple_pair_complete()  Pair State: %s  Status:%d  sec_state: %u",
      btm_pair_state_descr(btm_cb.pairing_state), status, p_dev_rec->sec_state);

  evt_data.status = BTM_ERR_PROCESSING;
  if (status == HCI_SUCCESS) {
    evt_data.status = BTM_SUCCESS;
    p_dev_rec->sec_flags |= BTM_SEC_AUTHENTICATED;
  } else {
    if (status == HCI_ERR_PAIRING_NOT_ALLOWED) {
      /* The test spec wants the peer device to get this failure code. */
      btm_sec_change_pairing_state(BTM_PAIR_STATE_WAIT_DISCONNECT);

      /* Change the timer to 1 second */
      alarm_set_on_mloop(btm_cb.pairing_timer, BT_1SEC_TIMEOUT_MS,
                         btm_sec_pairing_timeout, NULL);
    } else if (btm_cb.pairing_bda == evt_data.bd_addr) {
      /* stop the timer */
      alarm_cancel(btm_cb.pairing_timer);

      if (p_dev_rec->sec_state != BTM_SEC_STATE_AUTHENTICATING) {
        /* the initiating side: will receive auth complete event. disconnect ACL
         * at that time */
        disc = true;
      }
    } else
      disc = true;
  }

  /* Let the pairing state stay active, p_auth_complete_callback will report the
   * failure */
  evt_data.bd_addr = p_dev_rec->bd_addr;
  memcpy(evt_data.dev_class, p_dev_rec->dev_class, DEV_CLASS_LEN);

  if (btm_cb.api.p_sp_callback)
    (*btm_cb.api.p_sp_callback)(BTM_SP_COMPLT_EVT,
                                (tBTM_SP_EVT_DATA*)&evt_data);

  if (disc) {
    /* simple pairing failed */
    /* Avoid sending disconnect on HCI_ERR_PEER_USER */
    if ((status != HCI_ERR_PEER_USER) &&
        (status != HCI_ERR_CONN_CAUSE_LOCAL_HOST)) {
      btm_sec_send_hci_disconnect(p_dev_rec, HCI_ERR_AUTH_FAILURE,
                                  p_dev_rec->hci_handle);
    }
  }
}

/*******************************************************************************
 *
 * Function         btm_rem_oob_req
 *
 * Description      This function is called to process/report
 *                  HCI_REMOTE_OOB_DATA_REQUEST_EVT
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_rem_oob_req(uint8_t* p) {
  tBTM_SP_RMT_OOB evt_data;
  tBTM_SEC_DEV_REC* p_dev_rec;
  Octet16 c;
  Octet16 r;

  RawAddress& p_bda = evt_data.bd_addr;

  STREAM_TO_BDADDR(p_bda, p);

  VLOG(2) << __func__ << " BDA: " << p_bda;
  p_dev_rec = btm_find_dev(p_bda);
  if ((p_dev_rec != NULL) && btm_cb.api.p_sp_callback) {
    evt_data.bd_addr = p_dev_rec->bd_addr;
    memcpy(evt_data.dev_class, p_dev_rec->dev_class, DEV_CLASS_LEN);
    strlcpy((char*)evt_data.bd_name, (char*)p_dev_rec->sec_bd_name,
            BTM_MAX_REM_BD_NAME_LEN);

    btm_sec_change_pairing_state(BTM_PAIR_STATE_WAIT_LOCAL_OOB_RSP);
    if ((*btm_cb.api.p_sp_callback)(BTM_SP_RMT_OOB_EVT,
                                    (tBTM_SP_EVT_DATA*)&evt_data) ==
        BTM_NOT_AUTHORIZED) {
      BTM_RemoteOobDataReply(true, p_bda, c, r);
    }
    return;
  }

  /* something bad. we can only fail this connection */
  btm_cb.acl_disc_reason = HCI_ERR_HOST_REJECT_SECURITY;
  btsnd_hcic_rem_oob_neg_reply(p_bda);
}

/*******************************************************************************
 *
 * Function         btm_read_local_oob_complete
 *
 * Description      This function is called when read local oob data is
 *                  completed by the LM
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_read_local_oob_complete(uint8_t* p) {
  tBTM_SP_LOC_OOB evt_data;
  uint8_t status = *p++;

  BTM_TRACE_EVENT("btm_read_local_oob_complete:%d", status);
  if (status == HCI_SUCCESS) {
    evt_data.status = BTM_SUCCESS;
    STREAM_TO_ARRAY16(evt_data.c.data(), p);
    STREAM_TO_ARRAY16(evt_data.r.data(), p);
  } else
    evt_data.status = BTM_ERR_PROCESSING;

  if (btm_cb.api.p_sp_callback) {
    tBTM_SP_EVT_DATA btm_sp_evt_data;
    btm_sp_evt_data.loc_oob = evt_data;
    (*btm_cb.api.p_sp_callback)(BTM_SP_LOC_OOB_EVT, &btm_sp_evt_data);
  }
}

/*******************************************************************************
 *
 * Function         btm_sec_auth_collision
 *
 * Description      This function is called when authentication or encryption
 *                  needs to be retried at a later time.
 *
 * Returns          void
 *
 ******************************************************************************/
static void btm_sec_auth_collision(uint16_t handle) {
  tBTM_SEC_DEV_REC* p_dev_rec;

  if (!btm_cb.collision_start_time)
    btm_cb.collision_start_time = bluetooth::common::time_get_os_boottime_ms();

  if ((bluetooth::common::time_get_os_boottime_ms() -
       btm_cb.collision_start_time) < BTM_SEC_MAX_COLLISION_DELAY) {
    if (handle == BTM_SEC_INVALID_HANDLE) {
      p_dev_rec = btm_sec_find_dev_by_sec_state(BTM_SEC_STATE_AUTHENTICATING);
      if (p_dev_rec == NULL)
        p_dev_rec = btm_sec_find_dev_by_sec_state(BTM_SEC_STATE_ENCRYPTING);
    } else
      p_dev_rec = btm_find_dev_by_handle(handle);

    if (p_dev_rec != NULL) {
      BTM_TRACE_DEBUG(
          "btm_sec_auth_collision: state %d (retrying in a moment...)",
          p_dev_rec->sec_state);
      /* We will restart authentication after timeout */
      if (p_dev_rec->sec_state == BTM_SEC_STATE_AUTHENTICATING ||
          p_dev_rec->sec_state == BTM_SEC_STATE_ENCRYPTING)
        p_dev_rec->sec_state = 0;

      btm_cb.p_collided_dev_rec = p_dev_rec;
      alarm_set_on_mloop(btm_cb.sec_collision_timer, BT_1SEC_TIMEOUT_MS,
                         btm_sec_collision_timeout, NULL);
    }
  }
}

/******************************************************************************
 *
 * Function         btm_sec_auth_retry
 *
 * Description      This function is called when authentication or encryption
 *                  needs to be retried at a later time.
 *
 * Returns          TRUE if a security retry required
 *
 *****************************************************************************/
static bool btm_sec_auth_retry(uint16_t handle, uint8_t status) {
  tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev_by_handle(handle);
  if (!p_dev_rec) return false;

  /* keep the old sm4 flag and clear the retry bit in control block */
  uint8_t old_sm4 = p_dev_rec->sm4;
  p_dev_rec->sm4 &= ~BTM_SM4_RETRY;

  if ((btm_cb.pairing_state == BTM_PAIR_STATE_IDLE) &&
      ((old_sm4 & BTM_SM4_RETRY) == 0) && (HCI_ERR_KEY_MISSING == status) &&
      BTM_SEC_IS_SM4(p_dev_rec->sm4)) {
    /* This retry for missing key is for Lisbon or later only.
       Legacy device do not need this. the controller will drive the retry
       automatically
       set the retry bit */
    btm_cb.collision_start_time = 0;
    btm_restore_mode();
    p_dev_rec->sm4 |= BTM_SM4_RETRY;
    p_dev_rec->sec_flags &= ~BTM_SEC_LINK_KEY_KNOWN;
    BTM_TRACE_DEBUG("%s Retry for missing key sm4:x%x sec_flags:0x%x", __func__,
                    p_dev_rec->sm4, p_dev_rec->sec_flags);

    /* With BRCM controller, we do not need to delete the stored link key in
       controller.
       If the stack may sit on top of other controller, we may need this
       BTM_DeleteStoredLinkKey (bd_addr, NULL); */
    p_dev_rec->sec_state = BTM_SEC_STATE_IDLE;
    btm_sec_execute_procedure(p_dev_rec);
    return true;
  }

  return false;
}

/*******************************************************************************
 *
 * Function         btm_sec_auth_complete
 *
 * Description      This function is when authentication of the connection is
 *                  completed by the LM
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_sec_auth_complete(uint16_t handle, uint8_t status) {
  tBTM_PAIRING_STATE old_state = btm_cb.pairing_state;
  tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev_by_handle(handle);
  bool are_bonding = false;

  if (p_dev_rec) {
    VLOG(2) << __func__ << ": Security Manager: in state: "
            << btm_pair_state_descr(btm_cb.pairing_state)
            << " handle:" << handle << " status:" << status
            << "dev->sec_state:" << p_dev_rec->sec_state
            << " bda:" << p_dev_rec->bd_addr
            << "RName:" << p_dev_rec->sec_bd_name;
  } else {
    VLOG(2) << __func__ << ": Security Manager: in state: "
            << btm_pair_state_descr(btm_cb.pairing_state)
            << " handle:" << handle << " status:" << status;
  }

  /* For transaction collision we need to wait and repeat.  There is no need */
  /* for random timeout because only slave should receive the result */
  if ((status == HCI_ERR_LMP_ERR_TRANS_COLLISION) ||
      (status == HCI_ERR_DIFF_TRANSACTION_COLLISION)) {
    btm_sec_auth_collision(handle);
    return;
  } else if (btm_sec_auth_retry(handle, status)) {
    return;
  }

  btm_cb.collision_start_time = 0;

  btm_restore_mode();

  /* Check if connection was made just to do bonding.  If we authenticate
     the connection that is up, this is the last event received.
  */
  if (p_dev_rec && (btm_cb.pairing_flags & BTM_PAIR_FLAGS_WE_STARTED_DD) &&
      !(btm_cb.pairing_flags & BTM_PAIR_FLAGS_DISC_WHEN_DONE)) {
    p_dev_rec->security_required &= ~BTM_SEC_OUT_AUTHENTICATE;

    l2cu_start_post_bond_timer(p_dev_rec->hci_handle);
  }

  if (!p_dev_rec) return;

  if ((btm_cb.pairing_state != BTM_PAIR_STATE_IDLE) &&
      (btm_cb.pairing_flags & BTM_PAIR_FLAGS_WE_STARTED_DD) &&
      (p_dev_rec->bd_addr == btm_cb.pairing_bda))
    are_bonding = true;

  if ((btm_cb.pairing_state != BTM_PAIR_STATE_IDLE) &&
      (p_dev_rec->bd_addr == btm_cb.pairing_bda))
    btm_sec_change_pairing_state(BTM_PAIR_STATE_IDLE);

  if (p_dev_rec->sec_state != BTM_SEC_STATE_AUTHENTICATING) {
    if ((btm_cb.api.p_auth_complete_callback && status != HCI_SUCCESS) &&
        (old_state != BTM_PAIR_STATE_IDLE)) {
      (*btm_cb.api.p_auth_complete_callback)(p_dev_rec->bd_addr,
                                             p_dev_rec->dev_class,
                                             p_dev_rec->sec_bd_name, status);
    }
    return;
  }

  /* There can be a race condition, when we are starting authentication and
  ** the peer device is doing encryption.
  ** If first we receive encryption change up, then initiated authentication
  ** can not be performed.  According to the spec we can not do authentication
  ** on the encrypted link, so device is correct.
  */
  if ((status == HCI_ERR_COMMAND_DISALLOWED) &&
      ((p_dev_rec->sec_flags & (BTM_SEC_AUTHENTICATED | BTM_SEC_ENCRYPTED)) ==
       (BTM_SEC_AUTHENTICATED | BTM_SEC_ENCRYPTED))) {
    status = HCI_SUCCESS;
  }
  /* Currently we do not notify user if it is a keyboard which connects */
  /* User probably Disabled the keyboard while it was asleap.  Let her try */
  if (btm_cb.api.p_auth_complete_callback) {
    /* report the suthentication status */
    if ((old_state != BTM_PAIR_STATE_IDLE) || (status != HCI_SUCCESS))
      (*btm_cb.api.p_auth_complete_callback)(p_dev_rec->bd_addr,
                                             p_dev_rec->dev_class,
                                             p_dev_rec->sec_bd_name, status);
  }

  p_dev_rec->sec_state = BTM_SEC_STATE_IDLE;

  /* If this is a bonding procedure can disconnect the link now */
  if (are_bonding) {
    p_dev_rec->security_required &= ~BTM_SEC_OUT_AUTHENTICATE;

    if (status != HCI_SUCCESS) {
      if (((status != HCI_ERR_PEER_USER) &&
           (status != HCI_ERR_CONN_CAUSE_LOCAL_HOST)))
        btm_sec_send_hci_disconnect(p_dev_rec, HCI_ERR_PEER_USER,
                                    p_dev_rec->hci_handle);
    } else {
      BTM_TRACE_DEBUG("TRYING TO DECIDE IF CAN USE SMP_BR_CHNL");
      if (p_dev_rec->new_encryption_key_is_p256 &&
          (btm_sec_use_smp_br_chnl(p_dev_rec))
          /* no LE keys are available, do deriving */
          && (!(p_dev_rec->sec_flags & BTM_SEC_LE_LINK_KEY_KNOWN) ||
              /* or BR key is higher security than existing LE keys */
              (!(p_dev_rec->sec_flags & BTM_SEC_LE_LINK_KEY_AUTHED) &&
               (p_dev_rec->sec_flags & BTM_SEC_LINK_KEY_AUTHED)))) {
        BTM_TRACE_DEBUG(
            "link encrypted afer dedic bonding can use SMP_BR_CHNL");

        if (btm_sec_is_master(p_dev_rec)) {
          // Encryption is required to start SM over BR/EDR
          // indicate that this is encryption after authentication
          BTM_SetEncryption(p_dev_rec->bd_addr, BT_TRANSPORT_BR_EDR, NULL, NULL,
                            0);
        }
      }
      l2cu_start_post_bond_timer(p_dev_rec->hci_handle);
    }

    return;
  }

  /* If authentication failed, notify the waiting layer */
  if (status != HCI_SUCCESS) {
    btm_sec_dev_rec_cback_event(p_dev_rec, BTM_ERR_PROCESSING, false);

    if (btm_cb.pairing_flags & BTM_PAIR_FLAGS_DISC_WHEN_DONE) {
      btm_sec_send_hci_disconnect(p_dev_rec, HCI_ERR_AUTH_FAILURE,
                                  p_dev_rec->hci_handle);
    }
    return;
  }

  p_dev_rec->sec_flags |= BTM_SEC_AUTHENTICATED;

  if (p_dev_rec->pin_code_length >= 16 ||
      p_dev_rec->link_key_type == BTM_LKEY_TYPE_AUTH_COMB ||
      p_dev_rec->link_key_type == BTM_LKEY_TYPE_AUTH_COMB_P_256) {
    // If we have MITM protection we have a higher level of security than
    // provided by 16 digits PIN
    p_dev_rec->sec_flags |= BTM_SEC_16_DIGIT_PIN_AUTHED;
  }

  /* Authentication succeeded, execute the next security procedure, if any */
  status = btm_sec_execute_procedure(p_dev_rec);

  /* If there is no next procedure, or procedure failed to start, notify the
   * caller */
  if (status != BTM_CMD_STARTED)
    btm_sec_dev_rec_cback_event(p_dev_rec, status, false);
}

/*******************************************************************************
 *
 * Function         btm_sec_encrypt_change
 *
 * Description      This function is when encryption of the connection is
 *                  completed by the LM
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_sec_encrypt_change(uint16_t handle, uint8_t status,
                            uint8_t encr_enable) {
  tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev_by_handle(handle);
  tACL_CONN* p_acl = NULL;
  uint8_t acl_idx = btm_handle_to_acl_index(handle);
  BTM_TRACE_EVENT(
      "Security Manager: encrypt_change status:%d State:%d, encr_enable = %d",
      status, (p_dev_rec) ? p_dev_rec->sec_state : 0, encr_enable);
  BTM_TRACE_DEBUG("before update p_dev_rec->sec_flags=0x%x",
                  (p_dev_rec) ? p_dev_rec->sec_flags : 0);

  /* For transaction collision we need to wait and repeat.  There is no need */
  /* for random timeout because only slave should receive the result */
  if ((status == HCI_ERR_LMP_ERR_TRANS_COLLISION) ||
      (status == HCI_ERR_DIFF_TRANSACTION_COLLISION)) {
    btm_sec_auth_collision(handle);
    return;
  }
  btm_cb.collision_start_time = 0;

  if (!p_dev_rec) return;

  if ((status == HCI_SUCCESS) && encr_enable) {
    if (p_dev_rec->hci_handle == handle) {
      p_dev_rec->sec_flags |= (BTM_SEC_AUTHENTICATED | BTM_SEC_ENCRYPTED);
      if (p_dev_rec->pin_code_length >= 16 ||
          p_dev_rec->link_key_type == BTM_LKEY_TYPE_AUTH_COMB ||
          p_dev_rec->link_key_type == BTM_LKEY_TYPE_AUTH_COMB_P_256) {
        p_dev_rec->sec_flags |= BTM_SEC_16_DIGIT_PIN_AUTHED;
      }
    } else {
      p_dev_rec->sec_flags |= (BTM_SEC_LE_AUTHENTICATED | BTM_SEC_LE_ENCRYPTED);
    }
  }

  /* It is possible that we decrypted the link to perform role switch */
  /* mark link not to be encrypted, so that when we execute security next time
   * it will kick in again */
  if ((status == HCI_SUCCESS) && !encr_enable) {
    if (p_dev_rec->hci_handle == handle)
      p_dev_rec->sec_flags &= ~BTM_SEC_ENCRYPTED;
    else
      p_dev_rec->sec_flags &= ~BTM_SEC_LE_ENCRYPTED;
  }

  BTM_TRACE_DEBUG("after update p_dev_rec->sec_flags=0x%x",
                  p_dev_rec->sec_flags);

  if (acl_idx != MAX_L2CAP_LINKS) p_acl = &btm_cb.acl_db[acl_idx];

  if (p_acl != NULL)
    btm_sec_check_pending_enc_req(p_dev_rec, p_acl->transport, encr_enable);

  if (p_acl && p_acl->transport == BT_TRANSPORT_LE) {
    if (status == HCI_ERR_KEY_MISSING || status == HCI_ERR_AUTH_FAILURE ||
        status == HCI_ERR_ENCRY_MODE_NOT_ACCEPTABLE) {
      p_dev_rec->sec_flags &= ~(BTM_SEC_LE_LINK_KEY_KNOWN);
      p_dev_rec->ble.key_type = BTM_LE_KEY_NONE;
    }
    btm_ble_link_encrypted(p_dev_rec->ble.pseudo_addr, encr_enable);
    return;
  } else {
    /* BR/EDR connection, update the encryption key size to be 16 as always */
    p_dev_rec->enc_key_size = 16;
  }

  BTM_TRACE_DEBUG("in %s new_encr_key_256 is %d", __func__,
                  p_dev_rec->new_encryption_key_is_p256);

  if ((status == HCI_SUCCESS) && encr_enable &&
      (p_dev_rec->hci_handle == handle)) {
    /* if BR key is temporary no need for LE LTK derivation */
    bool derive_ltk = true;
    if (p_dev_rec->rmt_auth_req == BTM_AUTH_SP_NO &&
        btm_cb.devcb.loc_auth_req == BTM_AUTH_SP_NO) {
      derive_ltk = false;
      BTM_TRACE_DEBUG("%s: BR key is temporary, skip derivation of LE LTK",
                      __func__);
    }
    if (p_dev_rec->new_encryption_key_is_p256) {
      if (btm_sec_use_smp_br_chnl(p_dev_rec) && btm_sec_is_master(p_dev_rec) &&
          /* if LE key is not known, do deriving */
          (!(p_dev_rec->sec_flags & BTM_SEC_LE_LINK_KEY_KNOWN) ||
           /* or BR key is higher security than existing LE keys */
           (!(p_dev_rec->sec_flags & BTM_SEC_LE_LINK_KEY_AUTHED) &&
            (p_dev_rec->sec_flags & BTM_SEC_LINK_KEY_AUTHED))) &&
          derive_ltk) {
        /* BR/EDR is encrypted with LK that can be used to derive LE LTK */
        p_dev_rec->new_encryption_key_is_p256 = false;

        if (p_dev_rec->no_smp_on_br) {
          BTM_TRACE_DEBUG("%s NO SM over BR/EDR", __func__);
        } else {
          BTM_TRACE_DEBUG("%s start SM over BR/EDR", __func__);
          SMP_BR_PairWith(p_dev_rec->bd_addr);
        }
      }
    } else {
      // BR/EDR is successfully encrypted. Correct LK type if needed
      // (BR/EDR LK derived from LE LTK was used for encryption)
      if ((encr_enable == 1) && /* encryption is ON for SSP */
          /* LK type is for BR/EDR SC */
          (p_dev_rec->link_key_type == BTM_LKEY_TYPE_UNAUTH_COMB_P_256 ||
           p_dev_rec->link_key_type == BTM_LKEY_TYPE_AUTH_COMB_P_256)) {
        if (p_dev_rec->link_key_type == BTM_LKEY_TYPE_UNAUTH_COMB_P_256)
          p_dev_rec->link_key_type = BTM_LKEY_TYPE_UNAUTH_COMB;
        else /* BTM_LKEY_TYPE_AUTH_COMB_P_256 */
          p_dev_rec->link_key_type = BTM_LKEY_TYPE_AUTH_COMB;

        BTM_TRACE_DEBUG("updated link key type to %d",
                        p_dev_rec->link_key_type);
        btm_send_link_key_notif(p_dev_rec);
      }
    }
  }

  /* If this encryption was started by peer do not need to do anything */
  if (p_dev_rec->sec_state != BTM_SEC_STATE_ENCRYPTING) {
    if (BTM_SEC_STATE_DELAY_FOR_ENC == p_dev_rec->sec_state) {
      p_dev_rec->sec_state = BTM_SEC_STATE_IDLE;
      BTM_TRACE_DEBUG("%s: clearing callback. p_dev_rec=%p, p_callback=%p",
                      __func__, p_dev_rec, p_dev_rec->p_callback);
      p_dev_rec->p_callback = NULL;
      l2cu_resubmit_pending_sec_req(&p_dev_rec->bd_addr);
    }
    return;
  }

  p_dev_rec->sec_state = BTM_SEC_STATE_IDLE;
  /* If encryption setup failed, notify the waiting layer */
  if (status != HCI_SUCCESS) {
    btm_sec_dev_rec_cback_event(p_dev_rec, BTM_ERR_PROCESSING, false);
    return;
  }

  /* Encryption setup succeeded, execute the next security procedure, if any */
  status = (uint8_t)btm_sec_execute_procedure(p_dev_rec);
  /* If there is no next procedure, or procedure failed to start, notify the
   * caller */
  if (status != BTM_CMD_STARTED)
    btm_sec_dev_rec_cback_event(p_dev_rec, status, false);
}

/*******************************************************************************
 *
 * Function         btm_sec_connect_after_reject_timeout
 *
 * Description      Connection for bonding could not start because of the
 *                  collision. Initiate outgoing connection
 *
 * Returns          Pointer to the TLE struct
 *
 ******************************************************************************/
static void btm_sec_connect_after_reject_timeout(UNUSED_ATTR void* data) {
  tBTM_SEC_DEV_REC* p_dev_rec = btm_cb.p_collided_dev_rec;

  BTM_TRACE_EVENT("%s", __func__);
  btm_cb.p_collided_dev_rec = 0;

  if (btm_sec_dd_create_conn(p_dev_rec) != BTM_CMD_STARTED) {
    BTM_TRACE_WARNING("Security Manager: %s: failed to start connection",
                      __func__);

    btm_sec_change_pairing_state(BTM_PAIR_STATE_IDLE);

    if (btm_cb.api.p_auth_complete_callback)
      (*btm_cb.api.p_auth_complete_callback)(
          p_dev_rec->bd_addr, p_dev_rec->dev_class, p_dev_rec->sec_bd_name,
          HCI_ERR_MEMORY_FULL);
  }
}

/*******************************************************************************
 *
 * Function         btm_sec_connected
 *
 * Description      This function is when a connection to the peer device is
 *                  established
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_sec_connected(const RawAddress& bda, uint16_t handle, uint8_t status,
                       uint8_t enc_mode) {
  tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev(bda);
  uint8_t res;
  bool is_pairing_device = false;
  bool addr_matched;
  tACL_CONN* p_acl_cb;
  uint8_t bit_shift = 0;

  btm_acl_resubmit_page();

  if (p_dev_rec) {
    VLOG(2) << __func__ << ": Security Manager: in state: "
            << btm_pair_state_descr(btm_cb.pairing_state)
            << " handle:" << handle << " status:" << loghex(status)
            << " enc_mode:" << loghex(enc_mode) << " bda:" << bda
            << " RName:" << p_dev_rec->sec_bd_name;
  } else {
    VLOG(2) << __func__ << ": Security Manager: in state: "
            << btm_pair_state_descr(btm_cb.pairing_state)
            << " handle:" << handle << " status:" << loghex(status)
            << " enc_mode:" << loghex(enc_mode) << " bda:" << bda;
  }

  if (!p_dev_rec) {
    /* There is no device record for new connection.  Allocate one */
    if (status == HCI_SUCCESS) {
      p_dev_rec = btm_sec_alloc_dev(bda);
    } else {
      /* If the device matches with stored paring address
       * reset the paring state to idle */
      if ((btm_cb.pairing_state != BTM_PAIR_STATE_IDLE) &&
          btm_cb.pairing_bda == bda) {
        btm_sec_change_pairing_state(BTM_PAIR_STATE_IDLE);
      }

      /* can not find the device record and the status is error,
       * just ignore it */
      return;
    }
  } else /* Update the timestamp for this device */
  {
    bit_shift = (handle == p_dev_rec->ble_hci_handle) ? 8 : 0;
    p_dev_rec->timestamp = btm_cb.dev_rec_count++;
    if (p_dev_rec->sm4 & BTM_SM4_CONN_PEND) {
      /* tell L2CAP it's a bonding connection. */
      if ((btm_cb.pairing_state != BTM_PAIR_STATE_IDLE) &&
          (btm_cb.pairing_bda == p_dev_rec->bd_addr) &&
          (btm_cb.pairing_flags & BTM_PAIR_FLAGS_WE_STARTED_DD)) {
        /* if incoming connection failed while pairing, then try to connect and
         * continue */
        /* Motorola S9 disconnects without asking pin code */
        if ((status != HCI_SUCCESS) &&
            (btm_cb.pairing_state == BTM_PAIR_STATE_WAIT_PIN_REQ)) {
          BTM_TRACE_WARNING(
              "Security Manager: btm_sec_connected: incoming connection failed "
              "without asking PIN");

          p_dev_rec->sm4 &= ~BTM_SM4_CONN_PEND;
          if (p_dev_rec->sec_flags & BTM_SEC_NAME_KNOWN) {
            /* Start timer with 0 to initiate connection with new LCB */
            /* because L2CAP will delete current LCB with this event  */
            btm_cb.p_collided_dev_rec = p_dev_rec;
            alarm_set_on_mloop(btm_cb.sec_collision_timer, 0,
                               btm_sec_connect_after_reject_timeout, NULL);
          } else {
            btm_sec_change_pairing_state(BTM_PAIR_STATE_GET_REM_NAME);
            if (BTM_ReadRemoteDeviceName(p_dev_rec->bd_addr, NULL,
                                         BT_TRANSPORT_BR_EDR) !=
                BTM_CMD_STARTED) {
              BTM_TRACE_ERROR("%s cannot read remote name", __func__);
              btm_sec_change_pairing_state(BTM_PAIR_STATE_IDLE);
            }
          }
#if (BTM_DISC_DURING_RS == TRUE)
          p_dev_rec->rs_disc_pending = BTM_SEC_RS_NOT_PENDING; /* reset flag */
#endif
          return;
        } else {
          l2cu_update_lcb_4_bonding(p_dev_rec->bd_addr, true);
        }
      }
      /* always clear the pending flag */
      p_dev_rec->sm4 &= ~BTM_SM4_CONN_PEND;
    }
  }

  p_dev_rec->device_type |= BT_DEVICE_TYPE_BREDR;

#if (BTM_DISC_DURING_RS == TRUE)
  p_dev_rec->rs_disc_pending = BTM_SEC_RS_NOT_PENDING; /* reset flag */
#endif

  p_dev_rec->rs_disc_pending = BTM_SEC_RS_NOT_PENDING; /* reset flag */

  addr_matched = (btm_cb.pairing_bda == bda);

  if ((btm_cb.pairing_state != BTM_PAIR_STATE_IDLE) && addr_matched) {
    /* if we rejected incoming connection from bonding device */
    if ((status == HCI_ERR_HOST_REJECT_DEVICE) &&
        (btm_cb.pairing_flags & BTM_PAIR_FLAGS_REJECTED_CONNECT)) {
      BTM_TRACE_WARNING(
          "Security Manager: btm_sec_connected: HCI_Conn_Comp Flags:0x%04x, "
          "sm4: 0x%x",
          btm_cb.pairing_flags, p_dev_rec->sm4);

      btm_cb.pairing_flags &= ~BTM_PAIR_FLAGS_REJECTED_CONNECT;
      if (BTM_SEC_IS_SM4_UNKNOWN(p_dev_rec->sm4)) {
        /* Try again: RNR when no ACL causes HCI_RMT_HOST_SUP_FEAT_NOTIFY_EVT */
        btm_sec_change_pairing_state(BTM_PAIR_STATE_GET_REM_NAME);
        if (BTM_ReadRemoteDeviceName(bda, NULL, BT_TRANSPORT_BR_EDR) !=
            BTM_CMD_STARTED) {
          BTM_TRACE_ERROR("%s cannot read remote name", __func__);
          btm_sec_change_pairing_state(BTM_PAIR_STATE_IDLE);
        }
        return;
      }

      /* if we already have pin code */
      if (btm_cb.pairing_state != BTM_PAIR_STATE_WAIT_LOCAL_PIN) {
        /* Start timer with 0 to initiate connection with new LCB */
        /* because L2CAP will delete current LCB with this event  */
        btm_cb.p_collided_dev_rec = p_dev_rec;
        alarm_set_on_mloop(btm_cb.sec_collision_timer, 0,
                           btm_sec_connect_after_reject_timeout, NULL);
      }

      return;
    }
    /* wait for incoming connection without resetting pairing state */
    else if (status == HCI_ERR_CONNECTION_EXISTS) {
      BTM_TRACE_WARNING(
          "Security Manager: btm_sec_connected: Wait for incoming connection");
      return;
    }

    is_pairing_device = true;
  }

  /* If connection was made to do bonding restore link security if changed */
  btm_restore_mode();

  /* if connection fails during pin request, notify application */
  if (status != HCI_SUCCESS) {
    /* If connection failed because of during pairing, need to tell user */
    if (is_pairing_device) {
      p_dev_rec->security_required &= ~BTM_SEC_OUT_AUTHENTICATE;
      p_dev_rec->sec_flags &=
          ~((BTM_SEC_LINK_KEY_KNOWN | BTM_SEC_LINK_KEY_AUTHED) << bit_shift);
      BTM_TRACE_DEBUG("security_required:%x ", p_dev_rec->security_required);

      btm_sec_change_pairing_state(BTM_PAIR_STATE_IDLE);

      /* We need to notify host that the key is not known any more */
      if (btm_cb.api.p_auth_complete_callback) {
        (*btm_cb.api.p_auth_complete_callback)(p_dev_rec->bd_addr,
                                               p_dev_rec->dev_class,
                                               p_dev_rec->sec_bd_name, status);
      }
    }
    /*
        Do not send authentication failure, if following conditions hold good
         1.  BTM Sec Pairing state is idle
         2.  Link key for the remote device is present.
         3.  Remote is SSP capable.
     */
    else if ((p_dev_rec->link_key_type <= BTM_LKEY_TYPE_REMOTE_UNIT) &&
             (((status == HCI_ERR_AUTH_FAILURE) ||
               (status == HCI_ERR_KEY_MISSING) ||
               (status == HCI_ERR_HOST_REJECT_SECURITY) ||
               (status == HCI_ERR_PAIRING_NOT_ALLOWED) ||
               (status == HCI_ERR_UNIT_KEY_USED) ||
               (status == HCI_ERR_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED) ||
               (status == HCI_ERR_ENCRY_MODE_NOT_ACCEPTABLE) ||
               (status == HCI_ERR_REPEATED_ATTEMPTS)))) {
      p_dev_rec->security_required &= ~BTM_SEC_OUT_AUTHENTICATE;
      p_dev_rec->sec_flags &= ~(BTM_SEC_LE_LINK_KEY_KNOWN << bit_shift);

#ifdef BRCM_NOT_4_BTE
      /* If we rejected pairing, pass this special result code */
      if (btm_cb.acl_disc_reason == HCI_ERR_HOST_REJECT_SECURITY) {
        status = HCI_ERR_HOST_REJECT_SECURITY;
      }
#endif

      /* We need to notify host that the key is not known any more */
      if (btm_cb.api.p_auth_complete_callback) {
        (*btm_cb.api.p_auth_complete_callback)(p_dev_rec->bd_addr,
                                               p_dev_rec->dev_class,
                                               p_dev_rec->sec_bd_name, status);
      }
    }

    /* p_auth_complete_callback might have freed the p_dev_rec, ensure it exists
     * before accessing */
    p_dev_rec = btm_find_dev(bda);
    if (!p_dev_rec) {
      /* Don't callback when device security record was removed */
      VLOG(1) << __func__
              << ": device security record associated with this bda has been "
                 "removed! bda="
              << bda << ", do not callback!";
      return;
    }

    if (status == HCI_ERR_CONNECTION_TOUT ||
        status == HCI_ERR_LMP_RESPONSE_TIMEOUT ||
        status == HCI_ERR_UNSPECIFIED || status == HCI_ERR_PAGE_TIMEOUT)
      btm_sec_dev_rec_cback_event(p_dev_rec, BTM_DEVICE_TIMEOUT, false);
    else
      btm_sec_dev_rec_cback_event(p_dev_rec, BTM_ERR_PROCESSING, false);

    return;
  }

  /* If initiated dedicated bonding, return the link key now, and initiate
   * disconnect */
  /* If dedicated bonding, and we now have a link key, we are all done */
  if (is_pairing_device && (p_dev_rec->sec_flags & BTM_SEC_LINK_KEY_KNOWN)) {
    if (p_dev_rec->link_key_not_sent) {
      p_dev_rec->link_key_not_sent = false;
      btm_send_link_key_notif(p_dev_rec);
    }

    p_dev_rec->security_required &= ~BTM_SEC_OUT_AUTHENTICATE;

    /* remember flag before it is initialized */
    if (btm_cb.pairing_flags & BTM_PAIR_FLAGS_WE_STARTED_DD)
      res = true;
    else
      res = false;

    if (btm_cb.api.p_auth_complete_callback)
      (*btm_cb.api.p_auth_complete_callback)(
          p_dev_rec->bd_addr, p_dev_rec->dev_class, p_dev_rec->sec_bd_name,
          HCI_SUCCESS);

    btm_sec_change_pairing_state(BTM_PAIR_STATE_IDLE);

    if (res) {
      /* Let l2cap start bond timer */
      l2cu_update_lcb_4_bonding(p_dev_rec->bd_addr, true);
    }

    return;
  }

  p_dev_rec->hci_handle = handle;

  /* role may not be correct here, it will be updated by l2cap, but we need to
   */
  /* notify btm_acl that link is up, so starting of rmt name request will not */
  /* set paging flag up */
  p_acl_cb = btm_bda_to_acl(bda, BT_TRANSPORT_BR_EDR);
  if (p_acl_cb) {
/* whatever is in btm_establish_continue() without reporting the BTM_BL_CONN_EVT
 * event */
#if (BTM_BYPASS_EXTRA_ACL_SETUP == FALSE)
    /* For now there are a some devices that do not like sending */
    /* commands events and data at the same time. */
    /* Set the packet types to the default allowed by the device */
    btm_set_packet_types(p_acl_cb, btm_cb.btm_acl_pkt_types_supported);

    if (btm_cb.btm_def_link_policy)
      BTM_SetLinkPolicy(p_acl_cb->remote_addr, &btm_cb.btm_def_link_policy);
#endif
  }
  btm_acl_created(bda, p_dev_rec->dev_class, p_dev_rec->sec_bd_name, handle,
                  HCI_ROLE_SLAVE, BT_TRANSPORT_BR_EDR);

  /* Initialize security flags.  We need to do that because some            */
  /* authorization complete could have come after the connection is dropped */
  /* and that would set wrong flag that link has been authorized already    */
  p_dev_rec->sec_flags &= ~((BTM_SEC_AUTHORIZED | BTM_SEC_AUTHENTICATED |
                             BTM_SEC_ENCRYPTED | BTM_SEC_ROLE_SWITCHED)
                            << bit_shift);

  if (enc_mode != HCI_ENCRYPT_MODE_DISABLED)
    p_dev_rec->sec_flags |=
        ((BTM_SEC_AUTHENTICATED | BTM_SEC_ENCRYPTED) << bit_shift);

  if (btm_cb.security_mode == BTM_SEC_MODE_LINK)
    p_dev_rec->sec_flags |= (BTM_SEC_AUTHENTICATED << bit_shift);

  if (p_dev_rec->pin_code_length >= 16 ||
      p_dev_rec->link_key_type == BTM_LKEY_TYPE_AUTH_COMB ||
      p_dev_rec->link_key_type == BTM_LKEY_TYPE_AUTH_COMB_P_256) {
    p_dev_rec->sec_flags |= (BTM_SEC_16_DIGIT_PIN_AUTHED << bit_shift);
  }

  p_dev_rec->link_key_changed = false;

  /* After connection is established we perform security if we do not know */
  /* the name, or if we are originator because some procedure can have */
  /* been scheduled while connection was down */
  BTM_TRACE_DEBUG("is_originator:%d ", p_dev_rec->is_originator);
  if (!(p_dev_rec->sec_flags & BTM_SEC_NAME_KNOWN) ||
      p_dev_rec->is_originator) {
    res = btm_sec_execute_procedure(p_dev_rec);
    if (res != BTM_CMD_STARTED)
      btm_sec_dev_rec_cback_event(p_dev_rec, res, false);
  }
  return;
}

/*******************************************************************************
 *
 * Function         btm_sec_disconnect
 *
 * Description      This function is called to disconnect HCI link
 *
 * Returns          btm status
 *
 ******************************************************************************/
tBTM_STATUS btm_sec_disconnect(uint16_t handle, uint8_t reason) {
  tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev_by_handle(handle);

  /* In some weird race condition we may not have a record */
  if (!p_dev_rec) {
    btsnd_hcic_disconnect(handle, reason);
    return (BTM_SUCCESS);
  }

  /* If we are in the process of bonding we need to tell client that auth failed
   */
  if ((btm_cb.pairing_state != BTM_PAIR_STATE_IDLE) &&
      (btm_cb.pairing_bda == p_dev_rec->bd_addr) &&
      (btm_cb.pairing_flags & BTM_PAIR_FLAGS_WE_STARTED_DD)) {
    /* we are currently doing bonding.  Link will be disconnected when done */
    btm_cb.pairing_flags |= BTM_PAIR_FLAGS_DISC_WHEN_DONE;
    return (BTM_BUSY);
  }

  return (btm_sec_send_hci_disconnect(p_dev_rec, reason, handle));
}

/*******************************************************************************
 *
 * Function         btm_sec_disconnected
 *
 * Description      This function is when a connection to the peer device is
 *                  dropped
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_sec_disconnected(uint16_t handle, uint8_t reason) {
  tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev_by_handle(handle);
  uint8_t old_pairing_flags = btm_cb.pairing_flags;
  int result = HCI_ERR_AUTH_FAILURE;
  tBTM_SEC_CALLBACK* p_callback = NULL;
  tBT_TRANSPORT transport = BT_TRANSPORT_BR_EDR;

  /* If page was delayed for disc complete, can do it now */
  btm_cb.discing = false;

  btm_acl_resubmit_page();

  if (!p_dev_rec) return;

  transport =
      (handle == p_dev_rec->hci_handle) ? BT_TRANSPORT_BR_EDR : BT_TRANSPORT_LE;

  p_dev_rec->rs_disc_pending = BTM_SEC_RS_NOT_PENDING; /* reset flag */

#if (BTM_DISC_DURING_RS == TRUE)
  LOG_INFO(LOG_TAG, "%s clearing pending flag handle:%d reason:%d", __func__,
           handle, reason);
  p_dev_rec->rs_disc_pending = BTM_SEC_RS_NOT_PENDING; /* reset flag */
#endif

  /* clear unused flags */
  p_dev_rec->sm4 &= BTM_SM4_TRUE;

  VLOG(2) << __func__ << " bd_addr: " << p_dev_rec->bd_addr
          << " name: " << p_dev_rec->sec_bd_name
          << " state: " << btm_pair_state_descr(btm_cb.pairing_state)
          << " reason: " << reason << " sec_req: " << std::hex
          << p_dev_rec->security_required;

  BTM_TRACE_EVENT("%s before update sec_flags=0x%x", __func__,
                  p_dev_rec->sec_flags);

  /* If we are in the process of bonding we need to tell client that auth failed
   */
  if ((btm_cb.pairing_state != BTM_PAIR_STATE_IDLE) &&
      (btm_cb.pairing_bda == p_dev_rec->bd_addr)) {
    btm_sec_change_pairing_state(BTM_PAIR_STATE_IDLE);
    p_dev_rec->sec_flags &= ~BTM_SEC_LINK_KEY_KNOWN;
    if (btm_cb.api.p_auth_complete_callback) {
      /* If the disconnection reason is REPEATED_ATTEMPTS,
         send this error message to complete callback function
         to display the error message of Repeated attempts.
         All others, send HCI_ERR_AUTH_FAILURE. */
      if (reason == HCI_ERR_REPEATED_ATTEMPTS) {
        result = HCI_ERR_REPEATED_ATTEMPTS;
      } else if (old_pairing_flags & BTM_PAIR_FLAGS_WE_STARTED_DD) {
        result = HCI_ERR_HOST_REJECT_SECURITY;
      }
      (*btm_cb.api.p_auth_complete_callback)(p_dev_rec->bd_addr,
                                             p_dev_rec->dev_class,
                                             p_dev_rec->sec_bd_name, result);

      // |btm_cb.api.p_auth_complete_callback| may cause |p_dev_rec| to be
      // deallocated.
      p_dev_rec = btm_find_dev_by_handle(handle);
      if (!p_dev_rec) {
        return;
      }
    }
  }

  btm_ble_update_mode_operation(HCI_ROLE_UNKNOWN, &p_dev_rec->bd_addr,
                                HCI_SUCCESS);
  /* see sec_flags processing in btm_acl_removed */

  if (transport == BT_TRANSPORT_LE) {
    p_dev_rec->ble_hci_handle = BTM_SEC_INVALID_HANDLE;
    p_dev_rec->sec_flags &= ~(BTM_SEC_LE_AUTHENTICATED | BTM_SEC_LE_ENCRYPTED);
    p_dev_rec->enc_key_size = 0;

    // This is for chips that don't support being in connected and advertising
    // state at same time.
    if (!p_dev_rec->is_originator) {
      btm_ble_advertiser_notify_terminated_legacy(HCI_SUCCESS, handle);
    }
  } else {
    p_dev_rec->hci_handle = BTM_SEC_INVALID_HANDLE;
    p_dev_rec->sec_flags &=
        ~(BTM_SEC_AUTHORIZED | BTM_SEC_AUTHENTICATED | BTM_SEC_ENCRYPTED |
          BTM_SEC_ROLE_SWITCHED | BTM_SEC_16_DIGIT_PIN_AUTHED);

    // Remove temporary key.
    if (p_dev_rec->bond_type == BOND_TYPE_TEMPORARY)
      p_dev_rec->sec_flags &= ~(BTM_SEC_LINK_KEY_KNOWN);
  }

  /* Some devices hardcode sample LTK value from spec, instead of generating
   * one. Treat such devices as insecure, and remove such bonds on
   * disconnection.
   */
  if (is_sample_ltk(p_dev_rec->ble.keys.pltk)) {
    android_errorWriteLog(0x534e4554, "128437297");
    LOG(INFO) << __func__ << " removing bond to device that used sample LTK: " << p_dev_rec->bd_addr;

    bta_dm_remove_device(p_dev_rec->bd_addr);
  }

  BTM_TRACE_EVENT("%s after update sec_flags=0x%x", __func__,
                  p_dev_rec->sec_flags);

  if (p_dev_rec->sec_state == BTM_SEC_STATE_DISCONNECTING_BOTH) {
    p_dev_rec->sec_state = (transport == BT_TRANSPORT_LE)
                               ? BTM_SEC_STATE_DISCONNECTING
                               : BTM_SEC_STATE_DISCONNECTING_BLE;
    return;
  }
  p_dev_rec->sec_state = BTM_SEC_STATE_IDLE;
  p_dev_rec->security_required = BTM_SEC_NONE;

  p_callback = p_dev_rec->p_callback;

  /* if security is pending, send callback to clean up the security state */
  if (p_callback) {
    BTM_TRACE_DEBUG("%s: clearing callback. p_dev_rec=%p, p_callback=%p",
                    __func__, p_dev_rec, p_dev_rec->p_callback);
    p_dev_rec->p_callback =
        NULL; /* when the peer device time out the authentication before
                 we do, this call back must be reset here */
    (*p_callback)(&p_dev_rec->bd_addr, transport, p_dev_rec->p_ref_data,
                  BTM_ERR_PROCESSING);
  }
}

/** This function is called when a new connection link key is generated */
void btm_sec_link_key_notification(const RawAddress& p_bda,
                                   const Octet16& link_key, uint8_t key_type) {
  tBTM_SEC_DEV_REC* p_dev_rec = btm_find_or_alloc_dev(p_bda);
  bool we_are_bonding = false;
  bool ltk_derived_lk = false;

  VLOG(2) << __func__ << " BDA: " << p_bda << ", TYPE: " << +key_type;

  if ((key_type >= BTM_LTK_DERIVED_LKEY_OFFSET + BTM_LKEY_TYPE_COMBINATION) &&
      (key_type <=
       BTM_LTK_DERIVED_LKEY_OFFSET + BTM_LKEY_TYPE_AUTH_COMB_P_256)) {
    ltk_derived_lk = true;
    key_type -= BTM_LTK_DERIVED_LKEY_OFFSET;
  }
  /* If connection was made to do bonding restore link security if changed */
  btm_restore_mode();

  if (key_type != BTM_LKEY_TYPE_CHANGED_COMB)
    p_dev_rec->link_key_type = key_type;

  p_dev_rec->sec_flags |= BTM_SEC_LINK_KEY_KNOWN;

  /*
   * Until this point in time, we do not know if MITM was enabled, hence we
   * add the extended security flag here.
   */
  if (p_dev_rec->pin_code_length >= 16 ||
      p_dev_rec->link_key_type == BTM_LKEY_TYPE_AUTH_COMB ||
      p_dev_rec->link_key_type == BTM_LKEY_TYPE_AUTH_COMB_P_256) {
    p_dev_rec->sec_flags |= BTM_SEC_16_DIGIT_PIN_AUTHED;
  }

  /* BR/EDR connection, update the encryption key size to be 16 as always */
  p_dev_rec->enc_key_size = 16;
  p_dev_rec->link_key = link_key;

  if ((btm_cb.pairing_state != BTM_PAIR_STATE_IDLE) &&
      (btm_cb.pairing_bda == p_bda)) {
    if (btm_cb.pairing_flags & BTM_PAIR_FLAGS_WE_STARTED_DD)
      we_are_bonding = true;
    else
      btm_sec_change_pairing_state(BTM_PAIR_STATE_IDLE);
  }

  /* save LTK derived LK no matter what */
  if (ltk_derived_lk) {
    if (btm_cb.api.p_link_key_callback) {
      BTM_TRACE_DEBUG("%s() Save LTK derived LK (key_type = %d)", __func__,
                      p_dev_rec->link_key_type);
      (*btm_cb.api.p_link_key_callback)(p_bda, p_dev_rec->dev_class,
                                        p_dev_rec->sec_bd_name, link_key,
                                        p_dev_rec->link_key_type);
    }
  } else {
    if ((p_dev_rec->link_key_type == BTM_LKEY_TYPE_UNAUTH_COMB_P_256) ||
        (p_dev_rec->link_key_type == BTM_LKEY_TYPE_AUTH_COMB_P_256)) {
      p_dev_rec->new_encryption_key_is_p256 = true;
      BTM_TRACE_DEBUG("%s set new_encr_key_256 to %d", __func__,
                      p_dev_rec->new_encryption_key_is_p256);
    }
  }

  /* If name is not known at this point delay calling callback until the name is
   */
  /* resolved. Unless it is a HID Device and we really need to send all link
   * keys. */
  if ((!(p_dev_rec->sec_flags & BTM_SEC_NAME_KNOWN) &&
       ((p_dev_rec->dev_class[1] & BTM_COD_MAJOR_CLASS_MASK) !=
        BTM_COD_MAJOR_PERIPHERAL)) &&
      !ltk_derived_lk) {
    VLOG(2) << __func__ << " Delayed BDA: " << p_bda << " Type:" << +key_type;

    p_dev_rec->link_key_not_sent = true;

    /* If it is for bonding nothing else will follow, so we need to start name
     * resolution */
    if (we_are_bonding) {
      btsnd_hcic_rmt_name_req(p_bda, HCI_PAGE_SCAN_REP_MODE_R1,
                              HCI_MANDATARY_PAGE_SCAN_MODE, 0);
    }

    BTM_TRACE_EVENT("rmt_io_caps:%d, sec_flags:x%x, dev_class[1]:x%02x",
                    p_dev_rec->rmt_io_caps, p_dev_rec->sec_flags,
                    p_dev_rec->dev_class[1])
    return;
  }

  /* If its not us who perform authentication, we should tell stackserver */
  /* that some authentication has been completed                          */
  /* This is required when different entities receive link notification and auth
   * complete */
  if (!(p_dev_rec->security_required & BTM_SEC_OUT_AUTHENTICATE)
      /* for derived key, always send authentication callback for BR channel */
      || ltk_derived_lk) {
    if (btm_cb.api.p_auth_complete_callback)
      (*btm_cb.api.p_auth_complete_callback)(
          p_dev_rec->bd_addr, p_dev_rec->dev_class, p_dev_rec->sec_bd_name,
          HCI_SUCCESS);
  }

/* We will save link key only if the user authorized it - BTE report link key in
 * all cases */
#ifdef BRCM_NONE_BTE
  if (p_dev_rec->sec_flags & BTM_SEC_LINK_KEY_AUTHED)
#endif
  {
    if (btm_cb.api.p_link_key_callback) {
      if (ltk_derived_lk) {
        BTM_TRACE_DEBUG(
            "btm_sec_link_key_notification()  LTK derived LK is saved already"
            " (key_type = %d)",
            p_dev_rec->link_key_type);
      } else {
        (*btm_cb.api.p_link_key_callback)(p_bda, p_dev_rec->dev_class,
                                          p_dev_rec->sec_bd_name, link_key,
                                          p_dev_rec->link_key_type);
      }
    }
  }
}

/*******************************************************************************
 *
 * Function         btm_sec_link_key_request
 *
 * Description      This function is called when controller requests link key
 *
 * Returns          Pointer to the record or NULL
 *
 ******************************************************************************/
void btm_sec_link_key_request(const RawAddress& bda) {
  tBTM_SEC_DEV_REC* p_dev_rec = btm_find_or_alloc_dev(bda);

  VLOG(2) << __func__ << " bda: " << bda;

  if ((btm_cb.pairing_state == BTM_PAIR_STATE_WAIT_PIN_REQ) &&
      (btm_cb.collision_start_time != 0) &&
      (btm_cb.p_collided_dev_rec->bd_addr == bda)) {
    BTM_TRACE_EVENT(
        "btm_sec_link_key_request() rejecting link key req "
        "State: %d START_TIMEOUT : %d",
        btm_cb.pairing_state, btm_cb.collision_start_time);
    btsnd_hcic_link_key_neg_reply(bda);
    return;
  }
  if (p_dev_rec->sec_flags & BTM_SEC_LINK_KEY_KNOWN) {
    btsnd_hcic_link_key_req_reply(bda, p_dev_rec->link_key);
    return;
  }

  /* Notify L2CAP to increase timeout */
  l2c_pin_code_request(bda);

  /* The link key is not in the database and it is not known to the manager */
  btsnd_hcic_link_key_neg_reply(bda);
}

/*******************************************************************************
 *
 * Function         btm_sec_pairing_timeout
 *
 * Description      This function is called when host does not provide PIN
 *                  within requested time
 *
 * Returns          Pointer to the TLE struct
 *
 ******************************************************************************/
static void btm_sec_pairing_timeout(UNUSED_ATTR void* data) {
  tBTM_CB* p_cb = &btm_cb;
  tBTM_SEC_DEV_REC* p_dev_rec;
  tBTM_AUTH_REQ auth_req = (btm_cb.devcb.loc_io_caps == BTM_IO_CAP_NONE)
                               ? BTM_AUTH_AP_NO
                               : BTM_AUTH_AP_YES;
  uint8_t name[2];

  p_dev_rec = btm_find_dev(p_cb->pairing_bda);

  BTM_TRACE_EVENT("%s  State: %s   Flags: %u", __func__,
                  btm_pair_state_descr(p_cb->pairing_state),
                  p_cb->pairing_flags);

  switch (p_cb->pairing_state) {
    case BTM_PAIR_STATE_WAIT_PIN_REQ:
      btm_sec_bond_cancel_complete();
      break;

    case BTM_PAIR_STATE_WAIT_LOCAL_PIN:
      if ((btm_cb.pairing_flags & BTM_PAIR_FLAGS_PRE_FETCH_PIN) == 0)
        btsnd_hcic_pin_code_neg_reply(p_cb->pairing_bda);
      btm_sec_change_pairing_state(BTM_PAIR_STATE_IDLE);
      /* We need to notify the UI that no longer need the PIN */
      if (btm_cb.api.p_auth_complete_callback) {
        if (p_dev_rec == NULL) {
          name[0] = 0;
          (*btm_cb.api.p_auth_complete_callback)(p_cb->pairing_bda, NULL, name,
                                                 HCI_ERR_CONNECTION_TOUT);
        } else
          (*btm_cb.api.p_auth_complete_callback)(
              p_dev_rec->bd_addr, p_dev_rec->dev_class, p_dev_rec->sec_bd_name,
              HCI_ERR_CONNECTION_TOUT);
      }
      break;

    case BTM_PAIR_STATE_WAIT_NUMERIC_CONFIRM:
      btsnd_hcic_user_conf_reply(p_cb->pairing_bda, false);
      /* btm_sec_change_pairing_state (BTM_PAIR_STATE_IDLE); */
      break;

    case BTM_PAIR_STATE_KEY_ENTRY:
      if (btm_cb.devcb.loc_io_caps != BTM_IO_CAP_NONE) {
        btsnd_hcic_user_passkey_neg_reply(p_cb->pairing_bda);
      } else {
        btm_sec_change_pairing_state(BTM_PAIR_STATE_IDLE);
      }
      break;

    case BTM_PAIR_STATE_WAIT_LOCAL_IOCAPS:
      if (btm_cb.pairing_flags & BTM_PAIR_FLAGS_WE_STARTED_DD)
        auth_req |= BTM_AUTH_DD_BOND;

      btsnd_hcic_io_cap_req_reply(p_cb->pairing_bda, btm_cb.devcb.loc_io_caps,
                                  BTM_OOB_NONE, auth_req);
      btm_sec_change_pairing_state(BTM_PAIR_STATE_IDLE);
      break;

    case BTM_PAIR_STATE_WAIT_LOCAL_OOB_RSP:
      btsnd_hcic_rem_oob_neg_reply(p_cb->pairing_bda);
      btm_sec_change_pairing_state(BTM_PAIR_STATE_IDLE);
      break;

    case BTM_PAIR_STATE_WAIT_DISCONNECT:
      /* simple pairing failed. Started a 1-sec timer at simple pairing
       * complete.
       * now it's time to tear down the ACL link*/
      if (p_dev_rec == NULL) {
        LOG(ERROR) << __func__
                   << " BTM_PAIR_STATE_WAIT_DISCONNECT unknown BDA: "
                   << p_cb->pairing_bda;
        break;
      }
      btm_sec_send_hci_disconnect(p_dev_rec, HCI_ERR_AUTH_FAILURE,
                                  p_dev_rec->hci_handle);
      btm_sec_change_pairing_state(BTM_PAIR_STATE_IDLE);
      break;

    case BTM_PAIR_STATE_WAIT_AUTH_COMPLETE:
    case BTM_PAIR_STATE_GET_REM_NAME:
      /* We need to notify the UI that timeout has happened while waiting for
       * authentication*/
      btm_sec_change_pairing_state(BTM_PAIR_STATE_IDLE);
      if (btm_cb.api.p_auth_complete_callback) {
        if (p_dev_rec == NULL) {
          name[0] = 0;
          (*btm_cb.api.p_auth_complete_callback)(p_cb->pairing_bda, NULL, name,
                                                 HCI_ERR_CONNECTION_TOUT);
        } else
          (*btm_cb.api.p_auth_complete_callback)(
              p_dev_rec->bd_addr, p_dev_rec->dev_class, p_dev_rec->sec_bd_name,
              HCI_ERR_CONNECTION_TOUT);
      }
      break;

    default:
      BTM_TRACE_WARNING("%s not processed state: %s", __func__,
                        btm_pair_state_descr(btm_cb.pairing_state));
      btm_sec_change_pairing_state(BTM_PAIR_STATE_IDLE);
      break;
  }
}

/*******************************************************************************
 *
 * Function         btm_sec_pin_code_request
 *
 * Description      This function is called when controller requests PIN code
 *
 * Returns          Pointer to the record or NULL
 *
 ******************************************************************************/
void btm_sec_pin_code_request(const RawAddress& p_bda) {
  tBTM_SEC_DEV_REC* p_dev_rec;
  tBTM_CB* p_cb = &btm_cb;

  VLOG(2) << __func__ << " BDA: " << p_bda
          << " state: " << btm_pair_state_descr(btm_cb.pairing_state);

  if (btm_cb.pairing_state != BTM_PAIR_STATE_IDLE) {
    if ((p_bda == btm_cb.pairing_bda) &&
        (btm_cb.pairing_state == BTM_PAIR_STATE_WAIT_AUTH_COMPLETE)) {
      btsnd_hcic_pin_code_neg_reply(p_bda);
      return;
    } else if ((btm_cb.pairing_state != BTM_PAIR_STATE_WAIT_PIN_REQ) ||
               p_bda != btm_cb.pairing_bda) {
      BTM_TRACE_WARNING("btm_sec_pin_code_request() rejected - state: %s",
                        btm_pair_state_descr(btm_cb.pairing_state));
      btsnd_hcic_pin_code_neg_reply(p_bda);
      return;
    }
  }

  p_dev_rec = btm_find_or_alloc_dev(p_bda);
  /* received PIN code request. must be non-sm4 */
  p_dev_rec->sm4 = BTM_SM4_KNOWN;

  if (btm_cb.pairing_state == BTM_PAIR_STATE_IDLE) {
    btm_cb.pairing_bda = p_bda;

    btm_cb.pairing_flags = BTM_PAIR_FLAGS_PEER_STARTED_DD;
    /* Make sure we reset the trusted mask to help against attacks */
    BTM_SEC_CLR_TRUSTED_DEVICE(p_dev_rec->trusted_mask);
  }

  if (!p_cb->pairing_disabled && (p_cb->cfg.pin_type == HCI_PIN_TYPE_FIXED)) {
    BTM_TRACE_EVENT("btm_sec_pin_code_request fixed pin replying");
    btm_sec_change_pairing_state(BTM_PAIR_STATE_WAIT_AUTH_COMPLETE);
    btsnd_hcic_pin_code_req_reply(p_bda, p_cb->cfg.pin_code_len,
                                  p_cb->cfg.pin_code);
    return;
  }

  /* Use the connecting device's CoD for the connection */
  if ((p_bda == p_cb->connecting_bda) &&
      (p_cb->connecting_dc[0] || p_cb->connecting_dc[1] ||
       p_cb->connecting_dc[2]))
    memcpy(p_dev_rec->dev_class, p_cb->connecting_dc, DEV_CLASS_LEN);

  /* We could have started connection after asking user for the PIN code */
  if (btm_cb.pin_code_len != 0) {
    BTM_TRACE_EVENT("btm_sec_pin_code_request bonding sending reply");
    btsnd_hcic_pin_code_req_reply(p_bda, btm_cb.pin_code_len, p_cb->pin_code);

    /* Mark that we forwarded received from the user PIN code */
    btm_cb.pin_code_len = 0;

    /* We can change mode back right away, that other connection being
     * established */
    /* is not forced to be secure - found a FW issue, so we can not do this
    btm_restore_mode(); */

    btm_sec_change_pairing_state(BTM_PAIR_STATE_WAIT_AUTH_COMPLETE);
  }

  /* If pairing disabled OR (no PIN callback and not bonding) */
  /* OR we could not allocate entry in the database reject pairing request */
  else if (
      p_cb->pairing_disabled ||
      (p_cb->api.p_pin_callback == NULL)

      /* OR Microsoft keyboard can for some reason try to establish connection
       */
      /*  the only thing we can do here is to shut it up.  Normally we will be
         originator */
      /*  for keyboard bonding */
      || (!p_dev_rec->is_originator &&
          ((p_dev_rec->dev_class[1] & BTM_COD_MAJOR_CLASS_MASK) ==
           BTM_COD_MAJOR_PERIPHERAL) &&
          (p_dev_rec->dev_class[2] & BTM_COD_MINOR_KEYBOARD))) {
    BTM_TRACE_WARNING(
        "btm_sec_pin_code_request(): Pairing disabled:%d; PIN callback:%x, Dev "
        "Rec:%x!",
        p_cb->pairing_disabled, p_cb->api.p_pin_callback, p_dev_rec);

    btsnd_hcic_pin_code_neg_reply(p_bda);
  }
  /* Notify upper layer of PIN request and start expiration timer */
  else {
    btm_sec_change_pairing_state(BTM_PAIR_STATE_WAIT_LOCAL_PIN);
    /* Pin code request can not come at the same time as connection request */
    p_cb->connecting_bda = p_bda;
    memcpy(p_cb->connecting_dc, p_dev_rec->dev_class, DEV_CLASS_LEN);

    /* Check if the name is known */
    /* Even if name is not known we might not be able to get one */
    /* this is the case when we are already getting something from the */
    /* device, so HCI level is flow controlled */
    /* Also cannot send remote name request while paging, i.e. connection is not
     * completed */
    if (p_dev_rec->sec_flags & BTM_SEC_NAME_KNOWN) {
      BTM_TRACE_EVENT("btm_sec_pin_code_request going for callback");

      btm_cb.pairing_flags |= BTM_PAIR_FLAGS_PIN_REQD;
      if (p_cb->api.p_pin_callback) {
        (*p_cb->api.p_pin_callback)(
            p_bda, p_dev_rec->dev_class, p_dev_rec->sec_bd_name,
            (p_dev_rec->p_cur_service == NULL)
                ? false
                : (p_dev_rec->p_cur_service->security_flags &
                   BTM_SEC_IN_MIN_16_DIGIT_PIN));
      }
    } else {
      BTM_TRACE_EVENT("btm_sec_pin_code_request going for remote name");

      /* We received PIN code request for the device with unknown name */
      /* it is not user friendly just to ask for the PIN without name */
      /* try to get name at first */
      btsnd_hcic_rmt_name_req(p_dev_rec->bd_addr, HCI_PAGE_SCAN_REP_MODE_R1,
                              HCI_MANDATARY_PAGE_SCAN_MODE, 0);
    }
  }

  return;
}

/*******************************************************************************
 *
 * Function         btm_sec_update_clock_offset
 *
 * Description      This function is called to update clock offset
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_sec_update_clock_offset(uint16_t handle, uint16_t clock_offset) {
  tBTM_SEC_DEV_REC* p_dev_rec;
  tBTM_INQ_INFO* p_inq_info;

  p_dev_rec = btm_find_dev_by_handle(handle);
  if (p_dev_rec == NULL) return;

  p_dev_rec->clock_offset = clock_offset | BTM_CLOCK_OFFSET_VALID;

  p_inq_info = BTM_InqDbRead(p_dev_rec->bd_addr);
  if (p_inq_info == NULL) return;

  p_inq_info->results.clock_offset = clock_offset | BTM_CLOCK_OFFSET_VALID;
}

/******************************************************************
 * S T A T I C     F U N C T I O N S
 ******************************************************************/

/*******************************************************************************
 *
 * Function         btm_sec_execute_procedure
 *
 * Description      This function is called to start required security
 *                  procedure.  There is a case when multiplexing protocol
 *                  calls this function on the originating side, connection to
 *                  the peer will not be established.  This function in this
 *                  case performs only authorization.
 *
 * Returns          BTM_SUCCESS     - permission is granted
 *                  BTM_CMD_STARTED - in process
 *                  BTM_NO_RESOURCES  - permission declined
 *
 ******************************************************************************/
tBTM_STATUS btm_sec_execute_procedure(tBTM_SEC_DEV_REC* p_dev_rec) {
  BTM_TRACE_EVENT(
      "btm_sec_execute_procedure: Required:0x%x Flags:0x%x State:%d",
      p_dev_rec->security_required, p_dev_rec->sec_flags, p_dev_rec->sec_state);

  /* There is a chance that we are getting name.  Wait until done. */
  if (p_dev_rec->sec_state != 0) return (BTM_CMD_STARTED);

  /* If any security is required, get the name first */
  if (!(p_dev_rec->sec_flags & BTM_SEC_NAME_KNOWN) &&
      (p_dev_rec->hci_handle != BTM_SEC_INVALID_HANDLE)) {
    BTM_TRACE_EVENT("Security Manager: Start get name");
    if (!btm_sec_start_get_name(p_dev_rec)) {
      return (BTM_NO_RESOURCES);
    }
    return (BTM_CMD_STARTED);
  }

  /* If connection is not authenticated and authentication is required */
  /* start authentication and return PENDING to the caller */
  if ((((!(p_dev_rec->sec_flags & BTM_SEC_AUTHENTICATED)) &&
        ((p_dev_rec->is_originator &&
          (p_dev_rec->security_required & BTM_SEC_OUT_AUTHENTICATE)) ||
         (!p_dev_rec->is_originator &&
          (p_dev_rec->security_required & BTM_SEC_IN_AUTHENTICATE)))) ||
       (!(p_dev_rec->sec_flags & BTM_SEC_16_DIGIT_PIN_AUTHED) &&
        (!p_dev_rec->is_originator &&
         (p_dev_rec->security_required & BTM_SEC_IN_MIN_16_DIGIT_PIN)))) &&
      (p_dev_rec->hci_handle != BTM_SEC_INVALID_HANDLE)) {
/*
 * We rely on BTM_SEC_16_DIGIT_PIN_AUTHED being set if MITM is in use,
 * as 16 DIGIT is only needed if MITM is not used. Unfortunately, the
 * BTM_SEC_AUTHENTICATED is used for both MITM and non-MITM
 * authenticated connections, hence we cannot distinguish here.
 */

    BTM_TRACE_EVENT("Security Manager: Start authentication");

    /*
     * If we do have a link-key, but we end up here because we need an
     * upgrade, then clear the link-key known and authenticated flag before
     * restarting authentication.
     * WARNING: If the controller has link-key, it is optional and
     * recommended for the controller to send a Link_Key_Request.
     * In case we need an upgrade, the only alternative would be to delete
     * the existing link-key. That could lead to very bad user experience
     * or even IOP issues, if a reconnect causes a new connection that
     * requires an upgrade.
     */
    if ((p_dev_rec->sec_flags & BTM_SEC_LINK_KEY_KNOWN) &&
        (!(p_dev_rec->sec_flags & BTM_SEC_16_DIGIT_PIN_AUTHED) &&
         (!p_dev_rec->is_originator &&
          (p_dev_rec->security_required & BTM_SEC_IN_MIN_16_DIGIT_PIN)))) {
      p_dev_rec->sec_flags &=
          ~(BTM_SEC_LINK_KEY_KNOWN | BTM_SEC_LINK_KEY_AUTHED |
            BTM_SEC_AUTHENTICATED);
    }

    btm_sec_start_authentication(p_dev_rec);
    return (BTM_CMD_STARTED);
  }

  /* If connection is not encrypted and encryption is required */
  /* start encryption and return PENDING to the caller */
  if (!(p_dev_rec->sec_flags & BTM_SEC_ENCRYPTED) &&
      ((p_dev_rec->is_originator &&
        (p_dev_rec->security_required & BTM_SEC_OUT_ENCRYPT)) ||
       (!p_dev_rec->is_originator &&
        (p_dev_rec->security_required & BTM_SEC_IN_ENCRYPT))) &&
      (p_dev_rec->hci_handle != BTM_SEC_INVALID_HANDLE)) {

    BTM_TRACE_EVENT("Security Manager: Start encryption");

    btm_sec_start_encryption(p_dev_rec);
    return (BTM_CMD_STARTED);
  }

  if ((p_dev_rec->security_required & BTM_SEC_MODE4_LEVEL4) &&
      (p_dev_rec->link_key_type != BTM_LKEY_TYPE_AUTH_COMB_P_256)) {
    BTM_TRACE_EVENT(
        "%s: Security Manager: SC only service, but link key type is 0x%02x -",
        "security failure", __func__, p_dev_rec->link_key_type);
    return (BTM_FAILED_ON_SECURITY);
  }

  /* If connection is not authorized and authorization is required */
  /* start authorization and return PENDING to the caller */
  if (!(p_dev_rec->sec_flags & BTM_SEC_AUTHORIZED) &&
      ((p_dev_rec->is_originator &&
        (p_dev_rec->security_required & BTM_SEC_OUT_AUTHORIZE)) ||
       (!p_dev_rec->is_originator &&
        (p_dev_rec->security_required & BTM_SEC_IN_AUTHORIZE)))) {
    BTM_TRACE_EVENT(
        "service id:%d, is trusted:%d", p_dev_rec->p_cur_service->service_id,
        (BTM_SEC_IS_SERVICE_TRUSTED(p_dev_rec->trusted_mask,
                                    p_dev_rec->p_cur_service->service_id)));
    if ((!btm_sec_are_all_trusted(p_dev_rec->trusted_mask)) &&
        (p_dev_rec->p_cur_service->service_id < BTM_SEC_MAX_SERVICES) &&
        (!BTM_SEC_IS_SERVICE_TRUSTED(p_dev_rec->trusted_mask,
                                     p_dev_rec->p_cur_service->service_id))) {
      BTM_TRACE_EVENT("Security Manager: Start authorization");
      return (btm_sec_start_authorization(p_dev_rec));
    }
  }

  /* All required  security procedures already established */
  p_dev_rec->security_required &=
      ~(BTM_SEC_OUT_AUTHORIZE | BTM_SEC_IN_AUTHORIZE |
        BTM_SEC_OUT_AUTHENTICATE | BTM_SEC_IN_AUTHENTICATE |
        BTM_SEC_OUT_ENCRYPT | BTM_SEC_IN_ENCRYPT | BTM_SEC_FORCE_MASTER |
        BTM_SEC_ATTEMPT_MASTER | BTM_SEC_FORCE_SLAVE | BTM_SEC_ATTEMPT_SLAVE);

  BTM_TRACE_EVENT("Security Manager: trusted:0x%04x%04x",
                  p_dev_rec->trusted_mask[1], p_dev_rec->trusted_mask[0]);
  BTM_TRACE_EVENT("Security Manager: access granted");

  return (BTM_SUCCESS);
}

/*******************************************************************************
 *
 * Function         btm_sec_start_get_name
 *
 * Description      This function is called to start get name procedure
 *
 * Returns          true if started
 *
 ******************************************************************************/
static bool btm_sec_start_get_name(tBTM_SEC_DEV_REC* p_dev_rec) {
  uint8_t tempstate = p_dev_rec->sec_state;

  p_dev_rec->sec_state = BTM_SEC_STATE_GETTING_NAME;

  /* 0 and NULL are as timeout and callback params because they are not used in
   * security get name case */
  if ((btm_initiate_rem_name(p_dev_rec->bd_addr, BTM_RMT_NAME_SEC, 0, NULL)) !=
      BTM_CMD_STARTED) {
    p_dev_rec->sec_state = tempstate;
    return (false);
  }

  return (true);
}

/*******************************************************************************
 *
 * Function         btm_sec_start_authentication
 *
 * Description      This function is called to start authentication
 *
 ******************************************************************************/
static void btm_sec_start_authentication(tBTM_SEC_DEV_REC* p_dev_rec) {
  p_dev_rec->sec_state = BTM_SEC_STATE_AUTHENTICATING;
  btsnd_hcic_auth_request(p_dev_rec->hci_handle);
}

/*******************************************************************************
 *
 * Function         btm_sec_start_encryption
 *
 * Description      This function is called to start encryption
 *
 ******************************************************************************/
static void btm_sec_start_encryption(tBTM_SEC_DEV_REC* p_dev_rec) {
  btsnd_hcic_set_conn_encrypt(p_dev_rec->hci_handle, true);
  p_dev_rec->sec_state = BTM_SEC_STATE_ENCRYPTING;
}

/*******************************************************************************
 *
 * Function         btm_sec_start_authorization
 *
 * Description      This function is called to start authorization
 *
 * Returns          true if started
 *
 ******************************************************************************/
static uint8_t btm_sec_start_authorization(tBTM_SEC_DEV_REC* p_dev_rec) {
  uint8_t result;
  uint8_t* p_service_name = NULL;
  uint8_t service_id;

  if ((p_dev_rec->sec_flags & BTM_SEC_NAME_KNOWN) ||
      (p_dev_rec->hci_handle == BTM_SEC_INVALID_HANDLE)) {
    if (!btm_cb.api.p_authorize_callback) return (BTM_MODE_UNSUPPORTED);

    if (p_dev_rec->p_cur_service) {
#if BTM_SEC_SERVICE_NAME_LEN > 0
      if (p_dev_rec->is_originator)
        p_service_name = p_dev_rec->p_cur_service->orig_service_name;
      else
        p_service_name = p_dev_rec->p_cur_service->term_service_name;
#endif
      service_id = p_dev_rec->p_cur_service->service_id;
    } else
      service_id = 0;

    /* Send authorization request if not already sent during this service
     * connection */
    if (p_dev_rec->last_author_service_id == BTM_SEC_NO_LAST_SERVICE_ID ||
        p_dev_rec->last_author_service_id != service_id) {
      p_dev_rec->sec_state = BTM_SEC_STATE_AUTHORIZING;
      result = (*btm_cb.api.p_authorize_callback)(
          p_dev_rec->bd_addr, p_dev_rec->dev_class, p_dev_rec->sec_bd_name,
          p_service_name, service_id, p_dev_rec->is_originator);
    }

    else /* Already authorized once for this L2CAP bringup */
    {
      BTM_TRACE_DEBUG(
          "btm_sec_start_authorization: (Ignoring extra Authorization prompt "
          "for service %d)",
          service_id);
      return (BTM_SUCCESS);
    }

    if (result == BTM_SUCCESS) {
      p_dev_rec->sec_flags |= BTM_SEC_AUTHORIZED;

      /* Save the currently authorized service in case we are asked again by
       * another multiplexer layer */
      if (!p_dev_rec->is_originator)
        p_dev_rec->last_author_service_id = service_id;

      p_dev_rec->sec_state = BTM_SEC_STATE_IDLE;
    }
    return (result);
  }
  btm_sec_start_get_name(p_dev_rec);
  return (BTM_CMD_STARTED);
}

/*******************************************************************************
 *
 * Function         btm_sec_are_all_trusted
 *
 * Description      This function is called check if all services are trusted
 *
 * Returns          true if all are trusted, otherwise false
 *
 ******************************************************************************/
bool btm_sec_are_all_trusted(uint32_t p_mask[]) {
  uint32_t trusted_inx;
  for (trusted_inx = 0; trusted_inx < BTM_SEC_SERVICE_ARRAY_SIZE;
       trusted_inx++) {
    if (p_mask[trusted_inx] != BTM_SEC_TRUST_ALL) return (false);
  }

  return (true);
}

/*******************************************************************************
 *
 * Function         btm_sec_find_first_serv
 *
 * Description      Look for the first record in the service database
 *                  with specified PSM
 *
 * Returns          Pointer to the record or NULL
 *
 ******************************************************************************/
tBTM_SEC_SERV_REC* btm_sec_find_first_serv(CONNECTION_TYPE conn_type,
                                           uint16_t psm) {
  tBTM_SEC_SERV_REC* p_serv_rec = &btm_cb.sec_serv_rec[0];
  int i;
  bool is_originator = conn_type;

  if (is_originator && btm_cb.p_out_serv && btm_cb.p_out_serv->psm == psm) {
    /* If this is outgoing connection and the PSM matches p_out_serv,
     * use it as the current service */
    return btm_cb.p_out_serv;
  }

  /* otherwise, just find the first record with the specified PSM */
  for (i = 0; i < BTM_SEC_MAX_SERVICE_RECORDS; i++, p_serv_rec++) {
    if ((p_serv_rec->security_flags & BTM_SEC_IN_USE) &&
        (p_serv_rec->psm == psm))
      return (p_serv_rec);
  }
  return (NULL);
}

/*******************************************************************************
 *
 * Function         btm_sec_find_next_serv
 *
 * Description      Look for the next record in the service database
 *                  with specified PSM
 *
 * Returns          Pointer to the record or NULL
 *
 ******************************************************************************/
static tBTM_SEC_SERV_REC* btm_sec_find_next_serv(tBTM_SEC_SERV_REC* p_cur) {
  tBTM_SEC_SERV_REC* p_serv_rec = &btm_cb.sec_serv_rec[0];
  int i;

  for (i = 0; i < BTM_SEC_MAX_SERVICE_RECORDS; i++, p_serv_rec++) {
    if ((p_serv_rec->security_flags & BTM_SEC_IN_USE) &&
        (p_serv_rec->psm == p_cur->psm)) {
      if (p_cur != p_serv_rec) {
        return (p_serv_rec);
      }
    }
  }
  return (NULL);
}

/*******************************************************************************
 *
 * Function         btm_sec_find_mx_serv
 *
 * Description      Look for the record in the service database with specified
 *                  PSM and multiplexor channel information
 *
 * Returns          Pointer to the record or NULL
 *
 ******************************************************************************/
static tBTM_SEC_SERV_REC* btm_sec_find_mx_serv(uint8_t is_originator,
                                               uint16_t psm,
                                               uint32_t mx_proto_id,
                                               uint32_t mx_chan_id) {
  tBTM_SEC_SERV_REC* p_out_serv = btm_cb.p_out_serv;
  tBTM_SEC_SERV_REC* p_serv_rec = &btm_cb.sec_serv_rec[0];
  int i;

  BTM_TRACE_DEBUG("%s()", __func__);
  if (is_originator && p_out_serv && p_out_serv->psm == psm &&
      p_out_serv->mx_proto_id == mx_proto_id &&
      p_out_serv->orig_mx_chan_id == mx_chan_id) {
    /* If this is outgoing connection and the parameters match p_out_serv,
     * use it as the current service */
    return btm_cb.p_out_serv;
  }

  /* otherwise, the old way */
  for (i = 0; i < BTM_SEC_MAX_SERVICE_RECORDS; i++, p_serv_rec++) {
    if ((p_serv_rec->security_flags & BTM_SEC_IN_USE) &&
        (p_serv_rec->psm == psm) && (p_serv_rec->mx_proto_id == mx_proto_id) &&
        ((is_originator && (p_serv_rec->orig_mx_chan_id == mx_chan_id)) ||
         (!is_originator && (p_serv_rec->term_mx_chan_id == mx_chan_id)))) {
      return (p_serv_rec);
    }
  }
  return (NULL);
}

/*******************************************************************************
 *
 * Function         btm_sec_collision_timeout
 *
 * Description      Encryption could not start because of the collision
 *                  try to do it again
 *
 * Returns          Pointer to the TLE struct
 *
 ******************************************************************************/
static void btm_sec_collision_timeout(UNUSED_ATTR void* data) {
  BTM_TRACE_EVENT("%s()", __func__);

  tBTM_STATUS status = btm_sec_execute_procedure(btm_cb.p_collided_dev_rec);

  /* If result is pending reply from the user or from the device is pending */
  if (status != BTM_CMD_STARTED) {
    /* There is no next procedure or start of procedure failed, notify the
     * waiting layer */
    btm_sec_dev_rec_cback_event(btm_cb.p_collided_dev_rec, status, false);
  }
}

/*******************************************************************************
 *
 * Function         btm_send_link_key_notif
 *
 * Description      Call the link key callback.
 *
 * Returns          void
 *
 ******************************************************************************/
static void btm_send_link_key_notif(tBTM_SEC_DEV_REC* p_dev_rec) {
  if (btm_cb.api.p_link_key_callback)
    (*btm_cb.api.p_link_key_callback)(
        p_dev_rec->bd_addr, p_dev_rec->dev_class, p_dev_rec->sec_bd_name,
        p_dev_rec->link_key, p_dev_rec->link_key_type);
}

/*******************************************************************************
 *
 * Function         BTM_ReadTrustedMask
 *
 * Description      Get trusted mask for the peer device
 *
 * Parameters:      bd_addr   - Address of the device
 *
 * Returns          NULL, if the device record is not found.
 *                  otherwise, the trusted mask
 *
 ******************************************************************************/
uint32_t* BTM_ReadTrustedMask(const RawAddress& bd_addr) {
  tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev(bd_addr);
  if (p_dev_rec != NULL) return (p_dev_rec->trusted_mask);
  return NULL;
}

/*******************************************************************************
 *
 * Function         btm_restore_mode
 *
 * Description      This function returns the security mode to previous setting
 *                  if it was changed during bonding.
 *
 *
 * Parameters:      void
 *
 ******************************************************************************/
static void btm_restore_mode(void) {
  if (btm_cb.security_mode_changed) {
    btm_cb.security_mode_changed = false;
    BTM_TRACE_DEBUG("%s() Auth enable -> %d", __func__,
                    (btm_cb.security_mode == BTM_SEC_MODE_LINK));
    btsnd_hcic_write_auth_enable(
        (uint8_t)(btm_cb.security_mode == BTM_SEC_MODE_LINK));
  }

  if (btm_cb.pin_type_changed) {
    btm_cb.pin_type_changed = false;
    btsnd_hcic_write_pin_type(btm_cb.cfg.pin_type);
  }
}

bool is_sec_state_equal(void* data, void* context) {
  tBTM_SEC_DEV_REC* p_dev_rec = static_cast<tBTM_SEC_DEV_REC*>(data);
  uint8_t* state = static_cast<uint8_t*>(context);

  if (p_dev_rec->sec_state == *state) return false;

  return true;
}

/*******************************************************************************
 *
 * Function         btm_sec_find_dev_by_sec_state
 *
 * Description      Look for the record in the device database for the device
 *                  which is being authenticated or encrypted
 *
 * Returns          Pointer to the record or NULL
 *
 ******************************************************************************/
tBTM_SEC_DEV_REC* btm_sec_find_dev_by_sec_state(uint8_t state) {
  list_node_t* n = list_foreach(btm_cb.sec_dev_rec, is_sec_state_equal, &state);
  if (n) return static_cast<tBTM_SEC_DEV_REC*>(list_node(n));

  return NULL;
}

/*******************************************************************************
 *
 * Function         btm_sec_change_pairing_state
 *
 * Description      This function is called to change pairing state
 *
 ******************************************************************************/
static void btm_sec_change_pairing_state(tBTM_PAIRING_STATE new_state) {
  tBTM_PAIRING_STATE old_state = btm_cb.pairing_state;

  BTM_TRACE_EVENT("%s()  Old: %s", __func__,
                  btm_pair_state_descr(btm_cb.pairing_state));
  BTM_TRACE_EVENT("%s()  New: %s pairing_flags:0x%x", __func__,
                  btm_pair_state_descr(new_state), btm_cb.pairing_flags);

  btm_cb.pairing_state = new_state;

  if (new_state == BTM_PAIR_STATE_IDLE) {
    alarm_cancel(btm_cb.pairing_timer);

    btm_cb.pairing_flags = 0;
    btm_cb.pin_code_len = 0;

    /* Make sure the the lcb shows we are not bonding */
    l2cu_update_lcb_4_bonding(btm_cb.pairing_bda, false);

    btm_restore_mode();
    btm_sec_check_pending_reqs();
    btm_inq_clear_ssp();

    btm_cb.pairing_bda = RawAddress::kAny;
  } else {
    /* If transitioning out of idle, mark the lcb as bonding */
    if (old_state == BTM_PAIR_STATE_IDLE)
      l2cu_update_lcb_4_bonding(btm_cb.pairing_bda, true);

    alarm_set_on_mloop(btm_cb.pairing_timer, BTM_SEC_TIMEOUT_VALUE * 1000,
                       btm_sec_pairing_timeout, NULL);
  }
}

/*******************************************************************************
 *
 * Function         btm_pair_state_descr
 *
 * Description      Return state description for tracing
 *
 ******************************************************************************/
static const char* btm_pair_state_descr(tBTM_PAIRING_STATE state) {
  switch (state) {
    case BTM_PAIR_STATE_IDLE:
      return ("IDLE");
    case BTM_PAIR_STATE_GET_REM_NAME:
      return ("GET_REM_NAME");
    case BTM_PAIR_STATE_WAIT_PIN_REQ:
      return ("WAIT_PIN_REQ");
    case BTM_PAIR_STATE_WAIT_LOCAL_PIN:
      return ("WAIT_LOCAL_PIN");
    case BTM_PAIR_STATE_WAIT_NUMERIC_CONFIRM:
      return ("WAIT_NUM_CONFIRM");
    case BTM_PAIR_STATE_KEY_ENTRY:
      return ("KEY_ENTRY");
    case BTM_PAIR_STATE_WAIT_LOCAL_OOB_RSP:
      return ("WAIT_LOCAL_OOB_RSP");
    case BTM_PAIR_STATE_WAIT_LOCAL_IOCAPS:
      return ("WAIT_LOCAL_IOCAPS");
    case BTM_PAIR_STATE_INCOMING_SSP:
      return ("INCOMING_SSP");
    case BTM_PAIR_STATE_WAIT_AUTH_COMPLETE:
      return ("WAIT_AUTH_COMPLETE");
    case BTM_PAIR_STATE_WAIT_DISCONNECT:
      return ("WAIT_DISCONNECT");
  }

  return ("???");
}

/*******************************************************************************
 *
 * Function         btm_sec_dev_rec_cback_event
 *
 * Description      This function calls the callback function with the given
 *                  result and clear the callback function.
 *
 * Parameters:      void
 *
 ******************************************************************************/
void btm_sec_dev_rec_cback_event(tBTM_SEC_DEV_REC* p_dev_rec, uint8_t res,
                                 bool is_le_transport) {
  tBTM_SEC_CALLBACK* p_callback = p_dev_rec->p_callback;

  BTM_TRACE_DEBUG("%s: p_callback=%p, is_le_transport=%d, res=%d, p_dev_rec=%p",
                  __func__, p_dev_rec->p_callback, is_le_transport, res,
                  p_dev_rec);

  if (p_dev_rec->p_callback) {
    p_dev_rec->p_callback = NULL;

    if (is_le_transport)
      (*p_callback)(&p_dev_rec->ble.pseudo_addr, BT_TRANSPORT_LE,
                    p_dev_rec->p_ref_data, res);
    else
      (*p_callback)(&p_dev_rec->bd_addr, BT_TRANSPORT_BR_EDR,
                    p_dev_rec->p_ref_data, res);
  }

  btm_sec_check_pending_reqs();
}

/*******************************************************************************
 *
 * Function         btm_sec_queue_mx_request
 *
 * Description      Return state description for tracing
 *
 ******************************************************************************/
static bool btm_sec_queue_mx_request(const RawAddress& bd_addr, uint16_t psm,
                                     bool is_orig, uint32_t mx_proto_id,
                                     uint32_t mx_chan_id,
                                     tBTM_SEC_CALLBACK* p_callback,
                                     void* p_ref_data) {
  tBTM_SEC_QUEUE_ENTRY* p_e =
      (tBTM_SEC_QUEUE_ENTRY*)osi_malloc(sizeof(tBTM_SEC_QUEUE_ENTRY));

  p_e->psm = psm;
  p_e->is_orig = is_orig;
  p_e->p_callback = p_callback;
  p_e->p_ref_data = p_ref_data;
  p_e->mx_proto_id = mx_proto_id;
  p_e->mx_chan_id = mx_chan_id;
  p_e->transport = BT_TRANSPORT_BR_EDR;
  p_e->sec_act = 0;
  p_e->bd_addr = bd_addr;

  BTM_TRACE_EVENT(
      "%s() PSM: 0x%04x  Is_Orig: %u  mx_proto_id: %u  mx_chan_id: %u",
      __func__, psm, is_orig, mx_proto_id, mx_chan_id);

  fixed_queue_enqueue(btm_cb.sec_pending_q, p_e);

  return true;
}

static bool btm_sec_check_prefetch_pin(tBTM_SEC_DEV_REC* p_dev_rec) {
  uint8_t major = (uint8_t)(p_dev_rec->dev_class[1] & BTM_COD_MAJOR_CLASS_MASK);
  uint8_t minor = (uint8_t)(p_dev_rec->dev_class[2] & BTM_COD_MINOR_CLASS_MASK);
  bool rv = false;

  if ((major == BTM_COD_MAJOR_AUDIO) &&
      ((minor == BTM_COD_MINOR_CONFM_HANDSFREE) ||
       (minor == BTM_COD_MINOR_CAR_AUDIO))) {
    BTM_TRACE_EVENT(
        "%s() Skipping pre-fetch PIN for carkit COD Major: 0x%02x Minor: "
        "0x%02x",
        __func__, major, minor);

    if (!btm_cb.security_mode_changed) {
      btm_cb.security_mode_changed = true;
#ifdef APPL_AUTH_WRITE_EXCEPTION
      if (!(APPL_AUTH_WRITE_EXCEPTION)(p_dev_rec->bd_addr))
#endif
        btsnd_hcic_write_auth_enable(true);
    }
  } else {
    btm_sec_change_pairing_state(BTM_PAIR_STATE_WAIT_LOCAL_PIN);

    /* If we got a PIN, use that, else try to get one */
    if (btm_cb.pin_code_len) {
      BTM_PINCodeReply(p_dev_rec->bd_addr, BTM_SUCCESS, btm_cb.pin_code_len,
                       btm_cb.pin_code, p_dev_rec->trusted_mask);
    } else {
      /* pin was not supplied - pre-fetch pin code now */
      if (btm_cb.api.p_pin_callback &&
          ((btm_cb.pairing_flags & BTM_PAIR_FLAGS_PIN_REQD) == 0)) {
        BTM_TRACE_DEBUG("%s() PIN code callback called", __func__);
        if (btm_bda_to_acl(p_dev_rec->bd_addr, BT_TRANSPORT_BR_EDR) == NULL)
          btm_cb.pairing_flags |= BTM_PAIR_FLAGS_PIN_REQD;
        (btm_cb.api.p_pin_callback)(
            p_dev_rec->bd_addr, p_dev_rec->dev_class, p_dev_rec->sec_bd_name,
            (p_dev_rec->p_cur_service == NULL)
                ? false
                : (p_dev_rec->p_cur_service->security_flags &
                   BTM_SEC_IN_MIN_16_DIGIT_PIN));
      }
    }

    rv = true;
  }

  return rv;
}

/*******************************************************************************
 *
 * Function         btm_sec_auth_payload_tout
 *
 * Description      Processes the HCI Autheniticated Payload Timeout Event
 *                  indicating that a packet containing a valid MIC on the
 *                  connection handle was not received within the programmed
 *                  timeout value. (Spec Default is 30 secs, but can be
 *                  changed via the BTM_SecSetAuthPayloadTimeout() function.
 *
 ******************************************************************************/
void btm_sec_auth_payload_tout(uint8_t* p, uint16_t hci_evt_len) {
  uint16_t handle;

  STREAM_TO_UINT16(handle, p);
  handle = HCID_GET_HANDLE(handle);

  /* Will be exposed to upper layers in the future if/when determined necessary
   */
  BTM_TRACE_ERROR("%s on handle 0x%02x", __func__, handle);
}

/*******************************************************************************
 *
 * Function         btm_sec_queue_encrypt_request
 *
 * Description      encqueue encryption request when device has active security
 *                  process pending.
 *
 ******************************************************************************/
static bool btm_sec_queue_encrypt_request(const RawAddress& bd_addr,
                                          tBT_TRANSPORT transport,
                                          tBTM_SEC_CALLBACK* p_callback,
                                          void* p_ref_data,
                                          tBTM_BLE_SEC_ACT sec_act) {
  tBTM_SEC_QUEUE_ENTRY* p_e =
      (tBTM_SEC_QUEUE_ENTRY*)osi_malloc(sizeof(tBTM_SEC_QUEUE_ENTRY) + 1);

  p_e->psm = 0; /* if PSM 0, encryption request */
  p_e->p_callback = p_callback;
  p_e->p_ref_data = p_ref_data;
  p_e->transport = transport;
  p_e->sec_act = sec_act;
  p_e->bd_addr = bd_addr;
  fixed_queue_enqueue(btm_cb.sec_pending_q, p_e);

  return true;
}

/*******************************************************************************
 *
 * Function         btm_sec_set_peer_sec_caps
 *
 * Description      This function is called to set sm4 and rmt_sec_caps fields
 *                  based on the available peer device features.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_sec_set_peer_sec_caps(tACL_CONN* p_acl_cb,
                               tBTM_SEC_DEV_REC* p_dev_rec) {
  if ((btm_cb.security_mode == BTM_SEC_MODE_SP ||
       btm_cb.security_mode == BTM_SEC_MODE_SP_DEBUG ||
       btm_cb.security_mode == BTM_SEC_MODE_SC) &&
      HCI_SSP_HOST_SUPPORTED(p_acl_cb->peer_lmp_feature_pages[1])) {
    p_dev_rec->sm4 = BTM_SM4_TRUE;
    p_dev_rec->remote_supports_secure_connections =
        (HCI_SC_HOST_SUPPORTED(p_acl_cb->peer_lmp_feature_pages[1]));
  } else {
    p_dev_rec->sm4 = BTM_SM4_KNOWN;
    p_dev_rec->remote_supports_secure_connections = false;
  }

  BTM_TRACE_API("%s: sm4: 0x%02x, rmt_support_for_secure_connections %d",
                __func__, p_dev_rec->sm4,
                p_dev_rec->remote_supports_secure_connections);

  if (p_dev_rec->remote_features_needed) {
    BTM_TRACE_EVENT(
        "%s: Now device in SC Only mode, waiting for peer remote features!",
        __func__);
    btm_io_capabilities_req(p_dev_rec->bd_addr);
    p_dev_rec->remote_features_needed = false;
  }
}

/*******************************************************************************
 *
 * Function         btm_sec_is_serv_level0
 *
 * Description      This function is called to check if the service
 *                  corresponding to PSM is security mode 4 level 0 service.
 *
 * Returns          true if the service is security mode 4 level 0 service
 *
 ******************************************************************************/
static bool btm_sec_is_serv_level0(uint16_t psm) {
  if (psm == BT_PSM_SDP) {
    BTM_TRACE_DEBUG("%s: PSM: 0x%04x -> mode 4 level 0 service", __func__, psm);
    return true;
  }
  return false;
}

/*******************************************************************************
 *
 * Function         btm_sec_check_pending_enc_req
 *
 * Description      This function is called to send pending encryption callback
 *                  if waiting
 *
 * Returns          void
 *
 ******************************************************************************/
static void btm_sec_check_pending_enc_req(tBTM_SEC_DEV_REC* p_dev_rec,
                                          tBT_TRANSPORT transport,
                                          uint8_t encr_enable) {
  if (fixed_queue_is_empty(btm_cb.sec_pending_q)) return;

  uint8_t res = encr_enable ? BTM_SUCCESS : BTM_ERR_PROCESSING;
  list_t* list = fixed_queue_get_list(btm_cb.sec_pending_q);
  for (const list_node_t* node = list_begin(list); node != list_end(list);) {
    tBTM_SEC_QUEUE_ENTRY* p_e = (tBTM_SEC_QUEUE_ENTRY*)list_node(node);
    node = list_next(node);

    if (p_e->bd_addr == p_dev_rec->bd_addr && p_e->psm == 0 &&
        p_e->transport == transport) {
      if (encr_enable == 0 || transport == BT_TRANSPORT_BR_EDR ||
          p_e->sec_act == BTM_BLE_SEC_ENCRYPT ||
          p_e->sec_act == BTM_BLE_SEC_ENCRYPT_NO_MITM ||
          (p_e->sec_act == BTM_BLE_SEC_ENCRYPT_MITM &&
           p_dev_rec->sec_flags & BTM_SEC_LE_AUTHENTICATED)) {
        if (p_e->p_callback)
          (*p_e->p_callback)(&p_dev_rec->bd_addr, transport, p_e->p_ref_data,
                             res);
        fixed_queue_try_remove_from_queue(btm_cb.sec_pending_q, (void*)p_e);
      }
    }
  }
}

/*******************************************************************************
 *
 * Function         btm_sec_set_serv_level4_flags
 *
 * Description      This function is called to set security mode 4 level 4
 *                  flags.
 *
 * Returns          service security requirements updated to include secure
 *                  connections only mode.
 *
 ******************************************************************************/
static uint16_t btm_sec_set_serv_level4_flags(uint16_t cur_security,
                                              bool is_originator) {
  uint16_t sec_level4_flags =
      is_originator ? BTM_SEC_OUT_LEVEL4_FLAGS : BTM_SEC_IN_LEVEL4_FLAGS;

  return cur_security | sec_level4_flags;
}

/*******************************************************************************
 *
 * Function         btm_sec_clear_ble_keys
 *
 * Description      This function is called to clear out the BLE keys.
 *                  Typically when devices are removed in BTM_SecDeleteDevice,
 *                  or when a new BT Link key is generated.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_sec_clear_ble_keys(tBTM_SEC_DEV_REC* p_dev_rec) {
  BTM_TRACE_DEBUG("%s() Clearing BLE Keys", __func__);
  p_dev_rec->ble.key_type = BTM_LE_KEY_NONE;
  memset(&p_dev_rec->ble.keys, 0, sizeof(tBTM_SEC_BLE_KEYS));

#if (BLE_PRIVACY_SPT == TRUE)
  btm_ble_resolving_list_remove_dev(p_dev_rec);
#endif
}

/*******************************************************************************
 *
 * Function         btm_sec_is_a_bonded_dev
 *
 * Description       Is the specified device is a bonded device
 *
 * Returns          true - dev is bonded
 *
 ******************************************************************************/
bool btm_sec_is_a_bonded_dev(const RawAddress& bda) {
  tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev(bda);
  bool is_bonded = false;

  if (p_dev_rec && ((p_dev_rec->ble.key_type &&
                     (p_dev_rec->sec_flags & BTM_SEC_LE_LINK_KEY_KNOWN)) ||
                    (p_dev_rec->sec_flags & BTM_SEC_LINK_KEY_KNOWN))) {
    is_bonded = true;
  }
  BTM_TRACE_DEBUG("%s() is_bonded=%d", __func__, is_bonded);
  return (is_bonded);
}

/*******************************************************************************
 *
 * Function         btm_sec_is_le_capable_dev
 *
 * Description       Is the specified device is dual mode or LE only device
 *
 * Returns          true - dev is a dual mode
 *
 ******************************************************************************/
bool btm_sec_is_le_capable_dev(const RawAddress& bda) {
  tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev(bda);
  bool le_capable = false;

  if (p_dev_rec &&
      (p_dev_rec->device_type & BT_DEVICE_TYPE_BLE) == BT_DEVICE_TYPE_BLE)
    le_capable = true;
  return le_capable;
}

/*******************************************************************************
 *
 * Function         btm_sec_use_smp_br_chnl
 *
 * Description      The function checks if SMP BR connection can be used with
 *                  the peer.
 *                  Is called when authentication for dedicated bonding is
 *                  successfully completed.
 *
 * Returns          true - if SMP BR connection can be used (the link key is
 *                         generated from P-256 and the peer supports Security
 *                         Manager over BR).
 *
 ******************************************************************************/
static bool btm_sec_use_smp_br_chnl(tBTM_SEC_DEV_REC* p_dev_rec) {
  uint32_t ext_feat;
  uint8_t chnl_mask[L2CAP_FIXED_CHNL_ARRAY_SIZE];

  BTM_TRACE_DEBUG("%s() link_key_type = 0x%x", __func__,
                  p_dev_rec->link_key_type);

  if ((p_dev_rec->link_key_type != BTM_LKEY_TYPE_UNAUTH_COMB_P_256) &&
      (p_dev_rec->link_key_type != BTM_LKEY_TYPE_AUTH_COMB_P_256))
    return false;

  if (!L2CA_GetPeerFeatures(p_dev_rec->bd_addr, &ext_feat, chnl_mask))
    return false;

  if (!(chnl_mask[0] & L2CAP_FIXED_CHNL_SMP_BR_BIT)) return false;

  return true;
}

/*******************************************************************************
 *
 * Function         btm_sec_is_master
 *
 * Description      The function checks if the device is BR/EDR master after
 *                  pairing is completed.
 *
 * Returns          true - if the device is master.
 *
 ******************************************************************************/
static bool btm_sec_is_master(tBTM_SEC_DEV_REC* p_dev_rec) {
  tACL_CONN* p = btm_bda_to_acl(p_dev_rec->bd_addr, BT_TRANSPORT_BR_EDR);
  return (p && (p->link_role == BTM_ROLE_MASTER));
}
