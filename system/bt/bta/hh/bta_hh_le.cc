/******************************************************************************
 *
 *  Copyright 2009-2013 Broadcom Corporation
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

#define LOG_TAG "bt_bta_hh"

#include "bta_api.h"
#include "bta_hh_int.h"
#include "osi/include/osi.h"

#if (BTA_HH_LE_INCLUDED == TRUE)

#include <string.h>

#include <base/bind.h>
#include <base/callback.h>

#include "bta_gatt_api.h"
#include "bta_gatt_queue.h"
#include "bta_hh_co.h"
#include "btm_api.h"
#include "btm_ble_api.h"
#include "btm_int.h"
#include "device/include/interop.h"
#include "osi/include/log.h"
#include "srvc_api.h"
#include "stack/include/l2c_api.h"
#include "utl.h"

using bluetooth::Uuid;
using std::vector;

#ifndef BTA_HH_LE_RECONN
#define BTA_HH_LE_RECONN TRUE
#endif

#define BTA_HH_APP_ID_LE 0xff

#define BTA_HH_LE_RPT_TYPE_VALID(x) \
  ((x) <= BTA_LE_HID_RPT_FEATURE && (x) >= BTA_LE_HID_RPT_INPUT)

#define BTA_HH_LE_PROTO_BOOT_MODE 0x00
#define BTA_HH_LE_PROTO_REPORT_MODE 0x01

#define BTA_LE_HID_RTP_UUID_MAX 5
static const uint16_t bta_hh_uuid_to_rtp_type[BTA_LE_HID_RTP_UUID_MAX][2] = {
    {GATT_UUID_HID_REPORT, BTA_HH_RPTT_INPUT},
    {GATT_UUID_HID_BT_KB_INPUT, BTA_HH_RPTT_INPUT},
    {GATT_UUID_HID_BT_KB_OUTPUT, BTA_HH_RPTT_OUTPUT},
    {GATT_UUID_HID_BT_MOUSE_INPUT, BTA_HH_RPTT_INPUT},
    {GATT_UUID_BATTERY_LEVEL, BTA_HH_RPTT_INPUT}};

static void bta_hh_gattc_callback(tBTA_GATTC_EVT event, tBTA_GATTC* p_data);
static void bta_hh_le_add_dev_bg_conn(tBTA_HH_DEV_CB* p_cb, bool check_bond);
// TODO(jpawlowski): uncomment when fixed
// static void bta_hh_process_cache_rpt (tBTA_HH_DEV_CB *p_cb,
//                                       tBTA_HH_RPT_CACHE_ENTRY *p_rpt_cache,
//                                       uint8_t num_rpt);

#if (BTA_HH_DEBUG == TRUE)
static const char* bta_hh_le_rpt_name[4] = {"UNKNOWN", "INPUT", "OUTPUT",
                                            "FEATURE"};

/*******************************************************************************
 *
 * Function         bta_hh_le_hid_report_dbg
 *
 * Description      debug function to print out all HID report available on
 *                  remote device.
 *
 * Returns          void
 *
 ******************************************************************************/
static void bta_hh_le_hid_report_dbg(tBTA_HH_DEV_CB* p_cb) {
  APPL_TRACE_DEBUG("%s: HID Report DB", __func__);

  if (!p_cb->hid_srvc.in_use) return;

  tBTA_HH_LE_RPT* p_rpt = &p_cb->hid_srvc.report[0];

  for (int j = 0; j < BTA_HH_LE_RPT_MAX; j++, p_rpt++) {
    const char* rpt_name = "Unknown";

    if (!p_rpt->in_use) break;

    if (p_rpt->uuid == GATT_UUID_HID_REPORT) rpt_name = "Report";
    if (p_rpt->uuid == GATT_UUID_HID_BT_KB_INPUT) rpt_name = "Boot KB Input";
    if (p_rpt->uuid == GATT_UUID_HID_BT_KB_OUTPUT) rpt_name = "Boot KB Output";
    if (p_rpt->uuid == GATT_UUID_HID_BT_MOUSE_INPUT) rpt_name = "Boot MI Input";

    APPL_TRACE_DEBUG(
        "\t\t [%s- 0x%04x] [Type: %s], [ReportID: %d] [srvc_inst_id: %d] "
        "[char_inst_id: %d] [Clt_cfg: %d]",
        rpt_name, p_rpt->uuid,
        ((p_rpt->rpt_type < 4) ? bta_hh_le_rpt_name[p_rpt->rpt_type]
                               : "UNKNOWN"),
        p_rpt->rpt_id, p_rpt->srvc_inst_id, p_rpt->char_inst_id,
        p_rpt->client_cfg_value);
  }
}

/*******************************************************************************
 *
 * Function         bta_hh_uuid_to_str
 *
 * Description
 *
 * Returns          void
 *
 ******************************************************************************/
static const char* bta_hh_uuid_to_str(uint16_t uuid) {
  switch (uuid) {
    case GATT_UUID_HID_INFORMATION:
      return "GATT_UUID_HID_INFORMATION";
    case GATT_UUID_HID_REPORT_MAP:
      return "GATT_UUID_HID_REPORT_MAP";
    case GATT_UUID_HID_CONTROL_POINT:
      return "GATT_UUID_HID_CONTROL_POINT";
    case GATT_UUID_HID_REPORT:
      return "GATT_UUID_HID_REPORT";
    case GATT_UUID_HID_PROTO_MODE:
      return "GATT_UUID_HID_PROTO_MODE";
    case GATT_UUID_HID_BT_KB_INPUT:
      return "GATT_UUID_HID_BT_KB_INPUT";
    case GATT_UUID_HID_BT_KB_OUTPUT:
      return "GATT_UUID_HID_BT_KB_OUTPUT";
    case GATT_UUID_HID_BT_MOUSE_INPUT:
      return "GATT_UUID_HID_BT_MOUSE_INPUT";
    case GATT_UUID_CHAR_CLIENT_CONFIG:
      return "GATT_UUID_CHAR_CLIENT_CONFIG";
    case GATT_UUID_EXT_RPT_REF_DESCR:
      return "GATT_UUID_EXT_RPT_REF_DESCR";
    case GATT_UUID_RPT_REF_DESCR:
      return "GATT_UUID_RPT_REF_DESCR";
    default:
      return "Unknown UUID";
  }
}

#endif
/*******************************************************************************
 *
 * Function         bta_hh_le_enable
 *
 * Description      initialize LE HID related functionality
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_hh_le_enable(void) {
  uint8_t xx;

  bta_hh_cb.gatt_if = BTA_GATTS_INVALID_IF;

  for (xx = 0; xx < ARRAY_SIZE(bta_hh_cb.le_cb_index); xx++)
    bta_hh_cb.le_cb_index[xx] = BTA_HH_IDX_INVALID;

  BTA_GATTC_AppRegister(bta_hh_gattc_callback,
                        base::Bind([](uint8_t client_id, uint8_t r_status) {
                          tBTA_HH bta_hh;
                          bta_hh.status = BTA_HH_ERR;

                          if (r_status == GATT_SUCCESS) {
                            bta_hh_cb.gatt_if = client_id;
                            bta_hh.status = BTA_HH_OK;
                          } else {
                            bta_hh_cb.gatt_if = BTA_GATTS_INVALID_IF;
                          }

                          /* null check is needed in case HID profile is shut
                           * down before BTA_GATTC_AppRegister is done */
                          if (bta_hh_cb.p_cback) {
                            /* signal BTA call back event */
                            (*bta_hh_cb.p_cback)(BTA_HH_ENABLE_EVT, &bta_hh);
                          }
                        }));
}

/*******************************************************************************
 *
 * Function         bta_hh_le_is_hh_gatt_if
 *
 * Description      Check to see if client_if is BTA HH LE GATT interface
 *
 *
 * Returns          whether it is HH GATT IF
 *
 ******************************************************************************/
bool bta_hh_le_is_hh_gatt_if(tGATT_IF client_if) {
  return (bta_hh_cb.gatt_if == client_if);
}

/*******************************************************************************
 *
 * Function         bta_hh_le_deregister
 *
 * Description      De-register BTA HH from BTA GATTC
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_hh_le_deregister(void) { BTA_GATTC_AppDeregister(bta_hh_cb.gatt_if); }

/*******************************************************************************
 *
 * Function         bta_hh_is_le_device
 *
 * Description      Check to see if the remote device is a LE only device
 *
 * Parameters:
 *
 ******************************************************************************/
bool bta_hh_is_le_device(tBTA_HH_DEV_CB* p_cb, const RawAddress& remote_bda) {
  p_cb->is_le_device = BTM_UseLeLink(remote_bda);

  return p_cb->is_le_device;
}

/******************************************************************************
 *
 * Function         bta_hh_le_get_le_cb
 *
 * Description      Allocate bta_hh_cb.le_cb_index
 *
 * Parameters:
 *
 ******************************************************************************/
uint8_t bta_hh_le_get_le_dev_hdl(uint8_t cb_index) {
  uint8_t i;
  for (i = 0; i < ARRAY_SIZE(bta_hh_cb.le_cb_index); i++) {
    if (bta_hh_cb.le_cb_index[i] == cb_index) return BTA_HH_GET_LE_DEV_HDL(i);
  }

  for (i = 0; i < ARRAY_SIZE(bta_hh_cb.le_cb_index); i++) {
    if (bta_hh_cb.le_cb_index[i] == BTA_HH_IDX_INVALID)
      return BTA_HH_GET_LE_DEV_HDL(i);
  }
  return BTA_HH_IDX_INVALID;
}

/*******************************************************************************
 *
 * Function         bta_hh_le_open_conn
 *
 * Description      open a GATT connection first.
 *
 * Parameters:
 *
 ******************************************************************************/
void bta_hh_le_open_conn(tBTA_HH_DEV_CB* p_cb, const RawAddress& remote_bda) {
  tBTA_HH_STATUS status = BTA_HH_ERR_NO_RES;

  /* update cb_index[] map */
  p_cb->hid_handle = bta_hh_le_get_le_dev_hdl(p_cb->index);
  if (p_cb->hid_handle == BTA_HH_IDX_INVALID) {
    bta_hh_sm_execute(p_cb, BTA_HH_SDP_CMPL_EVT, (tBTA_HH_DATA*)&status);
    return;
  }

  p_cb->addr = remote_bda;
  bta_hh_cb.le_cb_index[BTA_HH_GET_LE_CB_IDX(p_cb->hid_handle)] = p_cb->index;
  p_cb->in_use = true;

  BTA_GATTC_Open(bta_hh_cb.gatt_if, remote_bda, true, GATT_TRANSPORT_LE, false);
}

/*******************************************************************************
 *
 * Function         bta_hh_le_find_dev_cb_by_conn_id
 *
 * Description      Utility function find a device control block by connection
 *                  ID.
 *
 ******************************************************************************/
tBTA_HH_DEV_CB* bta_hh_le_find_dev_cb_by_conn_id(uint16_t conn_id) {
  uint8_t i;
  tBTA_HH_DEV_CB* p_dev_cb = &bta_hh_cb.kdev[0];

  for (i = 0; i < BTA_HH_MAX_DEVICE; i++, p_dev_cb++) {
    if (p_dev_cb->in_use && p_dev_cb->conn_id == conn_id) return p_dev_cb;
  }
  return NULL;
}

/*******************************************************************************
 *
 * Function         bta_hh_le_find_dev_cb_by_bda
 *
 * Description      Utility function find a device control block by BD address.
 *
 ******************************************************************************/
tBTA_HH_DEV_CB* bta_hh_le_find_dev_cb_by_bda(const RawAddress& bda) {
  uint8_t i;
  tBTA_HH_DEV_CB* p_dev_cb = &bta_hh_cb.kdev[0];

  for (i = 0; i < BTA_HH_MAX_DEVICE; i++, p_dev_cb++) {
    if (p_dev_cb->in_use && p_dev_cb->addr == bda) return p_dev_cb;
  }
  return NULL;
}

/*******************************************************************************
 *
 * Function         bta_hh_le_find_service_inst_by_battery_inst_id
 *
 * Description      find HID service instance ID by battery service instance ID
 *
 ******************************************************************************/
uint8_t bta_hh_le_find_service_inst_by_battery_inst_id(tBTA_HH_DEV_CB* p_cb,
                                                       uint8_t ba_inst_id) {
  if (p_cb->hid_srvc.in_use && p_cb->hid_srvc.incl_srvc_inst == ba_inst_id) {
    return p_cb->hid_srvc.srvc_inst_id;
  }
  return BTA_HH_IDX_INVALID;
}

/*******************************************************************************
 *
 * Function         bta_hh_le_find_report_entry
 *
 * Description      find the report entry by service instance and report UUID
 *                  and instance ID
 *
 ******************************************************************************/
tBTA_HH_LE_RPT* bta_hh_le_find_report_entry(
    tBTA_HH_DEV_CB* p_cb, uint8_t srvc_inst_id, /* service instance ID */
    uint16_t rpt_uuid, uint8_t char_inst_id) {
  uint8_t i;
  uint8_t hid_inst_id = srvc_inst_id;
  tBTA_HH_LE_RPT* p_rpt;

  if (rpt_uuid == GATT_UUID_BATTERY_LEVEL) {
    hid_inst_id =
        bta_hh_le_find_service_inst_by_battery_inst_id(p_cb, srvc_inst_id);

    if (hid_inst_id == BTA_HH_IDX_INVALID) return NULL;
  }

  p_rpt = &p_cb->hid_srvc.report[0];

  for (i = 0; i < BTA_HH_LE_RPT_MAX; i++, p_rpt++) {
    if (p_rpt->uuid == rpt_uuid && p_rpt->srvc_inst_id == srvc_inst_id &&
        p_rpt->char_inst_id == char_inst_id) {
      return p_rpt;
    }
  }
  return NULL;
}

/*******************************************************************************
 *
 * Function         bta_hh_le_find_rpt_by_idtype
 *
 * Description      find a report entry by report ID and protocol mode
 *
 * Returns          void
 *
 ******************************************************************************/
tBTA_HH_LE_RPT* bta_hh_le_find_rpt_by_idtype(tBTA_HH_LE_RPT* p_head,
                                             uint8_t mode,
                                             tBTA_HH_RPT_TYPE r_type,
                                             uint8_t rpt_id) {
  tBTA_HH_LE_RPT* p_rpt = p_head;
  uint8_t i;

#if (BTA_HH_DEBUG == TRUE)
  APPL_TRACE_DEBUG("bta_hh_le_find_rpt_by_idtype: r_type: %d rpt_id: %d",
                   r_type, rpt_id);
#endif

  for (i = 0; i < BTA_HH_LE_RPT_MAX; i++, p_rpt++) {
    if (p_rpt->in_use && p_rpt->rpt_id == rpt_id && r_type == p_rpt->rpt_type) {
      /* return battery report w/o condition */
      if (p_rpt->uuid == GATT_UUID_BATTERY_LEVEL) return p_rpt;

      if (mode == BTA_HH_PROTO_RPT_MODE && p_rpt->uuid == GATT_UUID_HID_REPORT)
        return p_rpt;

      if (mode == BTA_HH_PROTO_BOOT_MODE &&
          (p_rpt->uuid >= GATT_UUID_HID_BT_KB_INPUT &&
           p_rpt->uuid <= GATT_UUID_HID_BT_MOUSE_INPUT))
        return p_rpt;
    }
  }
  return NULL;
}

/*******************************************************************************
 *
 * Function         bta_hh_le_find_alloc_report_entry
 *
 * Description      find or allocate a report entry in the HID service report
 *                  list.
 *
 ******************************************************************************/
tBTA_HH_LE_RPT* bta_hh_le_find_alloc_report_entry(tBTA_HH_DEV_CB* p_cb,
                                                  uint8_t srvc_inst_id,
                                                  uint16_t rpt_uuid,
                                                  uint8_t inst_id) {
  uint8_t i, hid_inst_id = srvc_inst_id;
  tBTA_HH_LE_RPT* p_rpt;

  if (rpt_uuid == GATT_UUID_BATTERY_LEVEL) {
    hid_inst_id =
        bta_hh_le_find_service_inst_by_battery_inst_id(p_cb, srvc_inst_id);

    if (hid_inst_id == BTA_HH_IDX_INVALID) return NULL;
  }
  p_rpt = &p_cb->hid_srvc.report[0];

  for (i = 0; i < BTA_HH_LE_RPT_MAX; i++, p_rpt++) {
    if (!p_rpt->in_use ||
        (p_rpt->uuid == rpt_uuid && p_rpt->srvc_inst_id == srvc_inst_id &&
         p_rpt->char_inst_id == inst_id)) {
      if (!p_rpt->in_use) {
        p_rpt->in_use = true;
        p_rpt->index = i;
        p_rpt->srvc_inst_id = srvc_inst_id;
        p_rpt->char_inst_id = inst_id;
        p_rpt->uuid = rpt_uuid;

        /* assign report type */
        for (i = 0; i < BTA_LE_HID_RTP_UUID_MAX; i++) {
          if (bta_hh_uuid_to_rtp_type[i][0] == rpt_uuid) {
            p_rpt->rpt_type = (tBTA_HH_RPT_TYPE)bta_hh_uuid_to_rtp_type[i][1];

            if (rpt_uuid == GATT_UUID_HID_BT_KB_INPUT ||
                rpt_uuid == GATT_UUID_HID_BT_KB_OUTPUT)
              p_rpt->rpt_id = BTA_HH_KEYBD_RPT_ID;

            if (rpt_uuid == GATT_UUID_HID_BT_MOUSE_INPUT)
              p_rpt->rpt_id = BTA_HH_MOUSE_RPT_ID;

            break;
          }
        }
      }
      return p_rpt;
    }
  }
  return NULL;
}

static const gatt::Descriptor* find_descriptor_by_short_uuid(
    uint16_t conn_id, uint16_t char_handle, uint16_t short_uuid) {
  const gatt::Characteristic* p_char =
      BTA_GATTC_GetCharacteristic(conn_id, char_handle);

  if (!p_char) {
    LOG_WARN(LOG_TAG, "%s No such characteristic: %d", __func__, char_handle);
    return NULL;
  }

  for (const gatt::Descriptor& desc : p_char->descriptors) {
    if (desc.uuid == Uuid::From16Bit(short_uuid)) return &desc;
  }

  return NULL;
}

/*******************************************************************************
 *
 * Function         bta_hh_le_read_char_descriptor
 *
 * Description      read characteristic descriptor
 *
 ******************************************************************************/
static tBTA_HH_STATUS bta_hh_le_read_char_descriptor(tBTA_HH_DEV_CB* p_cb,
                                                     uint16_t char_handle,
                                                     uint16_t short_uuid,
                                                     GATT_READ_OP_CB cb,
                                                     void* cb_data) {
  const gatt::Descriptor* p_desc =
      find_descriptor_by_short_uuid(p_cb->conn_id, char_handle, short_uuid);
  if (!p_desc) return BTA_HH_ERR;

  BtaGattQueue::ReadDescriptor(p_cb->conn_id, p_desc->handle, cb, cb_data);
  return BTA_HH_OK;
}

/*******************************************************************************
 *
 * Function         bta_hh_le_save_report_ref
 *
 * Description      save report reference information and move to next one.
 *
 * Parameters:
 *
 ******************************************************************************/
void bta_hh_le_save_report_ref(tBTA_HH_DEV_CB* p_dev_cb, tBTA_HH_LE_RPT* p_rpt,
                               tGATT_STATUS status, uint8_t* value,
                               uint16_t len) {
  if (status == GATT_INSUF_AUTHENTICATION) {
    /* close connection right away */
    p_dev_cb->status = BTA_HH_ERR_AUTH_FAILED;
    /* close the connection and report service discovery complete with error */
    bta_hh_le_api_disc_act(p_dev_cb);
    return;
  }

  /* if the length of the descriptor value is right, parse it */
  if (status == GATT_SUCCESS && len == 2) {
    uint8_t* pp = value;

    STREAM_TO_UINT8(p_rpt->rpt_id, pp);
    STREAM_TO_UINT8(p_rpt->rpt_type, pp);

    if (p_rpt->rpt_type > BTA_HH_RPTT_FEATURE) /* invalid report type */
      p_rpt->rpt_type = BTA_HH_RPTT_RESRV;

#if (BTA_HH_DEBUG == TRUE)
    APPL_TRACE_DEBUG("%s: report ID: %d", __func__, p_rpt->rpt_id);
#endif
    tBTA_HH_RPT_CACHE_ENTRY rpt_entry;
    rpt_entry.rpt_id = p_rpt->rpt_id;
    rpt_entry.rpt_type = p_rpt->rpt_type;
    rpt_entry.rpt_uuid = p_rpt->uuid;
    rpt_entry.srvc_inst_id = p_rpt->srvc_inst_id;
    rpt_entry.char_inst_id = p_rpt->char_inst_id;

    bta_hh_le_co_rpt_info(p_dev_cb->addr, &rpt_entry, p_dev_cb->app_id);
  }

  if (p_rpt->index < BTA_HH_LE_RPT_MAX - 1)
    p_rpt++;
  else
    p_rpt = NULL;
}

/*******************************************************************************
 *
 * Function         bta_hh_le_register_input_notif
 *
 * Description      Register for all notifications for the report applicable
 *                  for the protocol mode.
 *
 * Parameters:
 *
 ******************************************************************************/
void bta_hh_le_register_input_notif(tBTA_HH_DEV_CB* p_dev_cb,
                                    uint8_t proto_mode, bool register_ba) {
  tBTA_HH_LE_RPT* p_rpt = &p_dev_cb->hid_srvc.report[0];

#if (BTA_HH_DEBUG == TRUE)
  APPL_TRACE_DEBUG("%s: bta_hh_le_register_input_notif mode: %d", __func__,
                   proto_mode);
#endif

  for (int i = 0; i < BTA_HH_LE_RPT_MAX; i++, p_rpt++) {
    if (p_rpt->rpt_type == BTA_HH_RPTT_INPUT) {
      if (register_ba && p_rpt->uuid == GATT_UUID_BATTERY_LEVEL) {
        BTA_GATTC_RegisterForNotifications(bta_hh_cb.gatt_if, p_dev_cb->addr,
                                           p_rpt->char_inst_id);
      }
      /* boot mode, deregister report input notification */
      else if (proto_mode == BTA_HH_PROTO_BOOT_MODE) {
        if (p_rpt->uuid == GATT_UUID_HID_REPORT &&
            p_rpt->client_cfg_value == GATT_CLT_CONFIG_NOTIFICATION) {
          APPL_TRACE_DEBUG("%s ---> Deregister Report ID: %d", __func__,
                           p_rpt->rpt_id);
          BTA_GATTC_DeregisterForNotifications(
              bta_hh_cb.gatt_if, p_dev_cb->addr, p_rpt->char_inst_id);
        }
        /* register boot reports notification */
        else if (p_rpt->uuid == GATT_UUID_HID_BT_KB_INPUT ||
                 p_rpt->uuid == GATT_UUID_HID_BT_MOUSE_INPUT) {
          APPL_TRACE_DEBUG("%s <--- Register Boot Report ID: %d", __func__,
                           p_rpt->rpt_id);
          BTA_GATTC_RegisterForNotifications(bta_hh_cb.gatt_if, p_dev_cb->addr,
                                             p_rpt->char_inst_id);
        }
      } else if (proto_mode == BTA_HH_PROTO_RPT_MODE) {
        if ((p_rpt->uuid == GATT_UUID_HID_BT_KB_INPUT ||
             p_rpt->uuid == GATT_UUID_HID_BT_MOUSE_INPUT) &&
            p_rpt->client_cfg_value == GATT_CLT_CONFIG_NOTIFICATION) {
          APPL_TRACE_DEBUG("%s ---> Deregister Boot Report ID: %d", __func__,
                           p_rpt->rpt_id);
          BTA_GATTC_DeregisterForNotifications(
              bta_hh_cb.gatt_if, p_dev_cb->addr, p_rpt->char_inst_id);
        } else if (p_rpt->uuid == GATT_UUID_HID_REPORT &&
                   p_rpt->client_cfg_value == GATT_CLT_CONFIG_NOTIFICATION) {
          APPL_TRACE_DEBUG("%s <--- Register Report ID: %d", __func__,
                           p_rpt->rpt_id);
          BTA_GATTC_RegisterForNotifications(bta_hh_cb.gatt_if, p_dev_cb->addr,
                                             p_rpt->char_inst_id);
        }
      }
      /*
      else unknow protocol mode */
    }
  }
}

/*******************************************************************************
 *
 * Function         bta_hh_le_deregister_input_notif
 *
 * Description      Deregister all notifications
 *
 ******************************************************************************/
void bta_hh_le_deregister_input_notif(tBTA_HH_DEV_CB* p_dev_cb) {
  tBTA_HH_LE_RPT* p_rpt = &p_dev_cb->hid_srvc.report[0];

  for (uint8_t i = 0; i < BTA_HH_LE_RPT_MAX; i++, p_rpt++) {
    if (p_rpt->rpt_type == BTA_HH_RPTT_INPUT) {
      if (p_rpt->uuid == GATT_UUID_HID_REPORT &&
          p_rpt->client_cfg_value == GATT_CLT_CONFIG_NOTIFICATION) {
        APPL_TRACE_DEBUG("%s ---> Deregister Report ID: %d", __func__,
                         p_rpt->rpt_id);
        BTA_GATTC_DeregisterForNotifications(bta_hh_cb.gatt_if, p_dev_cb->addr,
                                             p_rpt->char_inst_id);
      } else if ((p_rpt->uuid == GATT_UUID_HID_BT_KB_INPUT ||
                  p_rpt->uuid == GATT_UUID_HID_BT_MOUSE_INPUT) &&
                 p_rpt->client_cfg_value == GATT_CLT_CONFIG_NOTIFICATION) {
        APPL_TRACE_DEBUG("%s ---> Deregister Boot Report ID: %d", __func__,
                         p_rpt->rpt_id);
        BTA_GATTC_DeregisterForNotifications(bta_hh_cb.gatt_if, p_dev_cb->addr,
                                             p_rpt->char_inst_id);
      }
    }
  }
}

/*******************************************************************************
 *
 * Function         bta_hh_le_open_cmpl
 *
 * Description      HID over GATT connection sucessfully opened
 *
 ******************************************************************************/
void bta_hh_le_open_cmpl(tBTA_HH_DEV_CB* p_cb) {
  if (p_cb->disc_active == BTA_HH_LE_DISC_NONE) {
#if (BTA_HH_DEBUG == TRUE)
    bta_hh_le_hid_report_dbg(p_cb);
#endif
    bta_hh_le_register_input_notif(p_cb, p_cb->mode, true);
    bta_hh_sm_execute(p_cb, BTA_HH_OPEN_CMPL_EVT, NULL);

#if (BTA_HH_LE_RECONN == TRUE)
    if (p_cb->status == BTA_HH_OK) {
      bta_hh_le_add_dev_bg_conn(p_cb, true);
    }
#endif
  }
}

/*******************************************************************************
 *
 * Function         bta_hh_le_write_ccc
 *
 * Description      Utility function to find and write client configuration of
 *                  a characteristic
 *
 ******************************************************************************/
bool bta_hh_le_write_ccc(tBTA_HH_DEV_CB* p_cb, uint8_t char_handle,
                         uint16_t clt_cfg_value, GATT_WRITE_OP_CB cb,
                         void* cb_data) {
  const gatt::Descriptor* p_desc = find_descriptor_by_short_uuid(
      p_cb->conn_id, char_handle, GATT_UUID_CHAR_CLIENT_CONFIG);
  if (!p_desc) return false;

  vector<uint8_t> value(2);
  uint8_t* ptr = value.data();
  UINT16_TO_STREAM(ptr, clt_cfg_value);

  BtaGattQueue::WriteDescriptor(p_cb->conn_id, p_desc->handle, std::move(value),
                                GATT_WRITE, cb, cb_data);
  return true;
}

bool bta_hh_le_write_rpt_clt_cfg(tBTA_HH_DEV_CB* p_cb);

static void write_rpt_ctl_cfg_cb(uint16_t conn_id, tGATT_STATUS status,
                                 uint16_t handle, void* data) {
  uint8_t srvc_inst_id, hid_inst_id;

  tBTA_HH_DEV_CB* p_dev_cb = (tBTA_HH_DEV_CB*)data;
  const gatt::Characteristic* characteristic =
      BTA_GATTC_GetOwningCharacteristic(conn_id, handle);
  uint16_t char_uuid = characteristic->uuid.As16Bit();

  srvc_inst_id = BTA_GATTC_GetOwningService(conn_id, handle)->handle;
  hid_inst_id = srvc_inst_id;
  switch (char_uuid) {
    case GATT_UUID_BATTERY_LEVEL: /* battery level clt cfg registered */
      hid_inst_id = bta_hh_le_find_service_inst_by_battery_inst_id(
          p_dev_cb, srvc_inst_id);
      FALLTHROUGH_INTENDED; /* FALLTHROUGH */
    case GATT_UUID_HID_BT_KB_INPUT:
    case GATT_UUID_HID_BT_MOUSE_INPUT:
    case GATT_UUID_HID_REPORT:
      if (status == GATT_SUCCESS)
        p_dev_cb->hid_srvc.report[p_dev_cb->clt_cfg_idx].client_cfg_value =
            GATT_CLT_CONFIG_NOTIFICATION;
      p_dev_cb->clt_cfg_idx++;
      bta_hh_le_write_rpt_clt_cfg(p_dev_cb);
      break;

    default:
      APPL_TRACE_ERROR("Unknown char ID clt cfg: 0x%04x", char_uuid);
  }
}
/*******************************************************************************
 *
 * Function         bta_hh_le_write_rpt_clt_cfg
 *
 * Description      write client configuration. This is only for input report
 *                  enable all input notification upon connection open.
 *
 ******************************************************************************/
bool bta_hh_le_write_rpt_clt_cfg(tBTA_HH_DEV_CB* p_cb) {
  uint8_t i;
  tBTA_HH_LE_RPT* p_rpt = &p_cb->hid_srvc.report[p_cb->clt_cfg_idx];

  for (i = p_cb->clt_cfg_idx; i < BTA_HH_LE_RPT_MAX && p_rpt->in_use;
       i++, p_rpt++) {
    /* enable notification for all input report, regardless mode */
    if (p_rpt->rpt_type == BTA_HH_RPTT_INPUT) {
      if (bta_hh_le_write_ccc(p_cb, p_rpt->char_inst_id,
                              GATT_CLT_CONFIG_NOTIFICATION,
                              write_rpt_ctl_cfg_cb, p_cb)) {
        p_cb->clt_cfg_idx = i;
        return true;
      }
    }
  }
  p_cb->clt_cfg_idx = 0;

  /* client configuration is completed, send open callback */
  if (p_cb->state == BTA_HH_W4_CONN_ST) {
    p_cb->disc_active &= ~BTA_HH_LE_DISC_HIDS;

    bta_hh_le_open_cmpl(p_cb);
  }
  return false;
}

static void write_proto_mode_cb(uint16_t conn_id, tGATT_STATUS status,
                                uint16_t handle, void* data) {
  tBTA_HH_DEV_CB* p_dev_cb = (tBTA_HH_DEV_CB*)data;

  if (p_dev_cb->state == BTA_HH_CONN_ST) {
    /* Set protocol finished in CONN state*/

    uint16_t cb_evt = p_dev_cb->w4_evt;
    if (cb_evt == 0) return;

    tBTA_HH_CBDATA cback_data;

    cback_data.handle = p_dev_cb->hid_handle;
    cback_data.status = (status == GATT_SUCCESS) ? BTA_HH_OK : BTA_HH_ERR;

    if (status == GATT_SUCCESS)
      bta_hh_le_register_input_notif(p_dev_cb, p_dev_cb->mode, false);

    p_dev_cb->w4_evt = 0;
    (*bta_hh_cb.p_cback)(cb_evt, (tBTA_HH*)&cback_data);
  } else if (p_dev_cb->state == BTA_HH_W4_CONN_ST) {
    p_dev_cb->status = (status == GATT_SUCCESS) ? BTA_HH_OK : BTA_HH_ERR_PROTO;

    if ((p_dev_cb->disc_active & BTA_HH_LE_DISC_HIDS) == 0)
      bta_hh_le_open_cmpl(p_dev_cb);
  }
}

/*******************************************************************************
 *
 * Function         bta_hh_le_set_protocol_mode
 *
 * Description      Set remote device protocol mode.
 *
 ******************************************************************************/
bool bta_hh_le_set_protocol_mode(tBTA_HH_DEV_CB* p_cb,
                                 tBTA_HH_PROTO_MODE mode) {
  tBTA_HH_CBDATA cback_data;

  APPL_TRACE_DEBUG("%s attempt mode: %s", __func__,
                   (mode == BTA_HH_PROTO_RPT_MODE) ? "Report" : "Boot");

  cback_data.handle = p_cb->hid_handle;
  /* boot mode is not supported in the remote device */
  if (p_cb->hid_srvc.proto_mode_handle == 0) {
    p_cb->mode = BTA_HH_PROTO_RPT_MODE;

    if (mode == BTA_HH_PROTO_BOOT_MODE) {
      APPL_TRACE_ERROR("Set Boot Mode failed!! No PROTO_MODE Char!");
      cback_data.status = BTA_HH_ERR;
    } else {
      /* if set to report mode, need to de-register all input report
       * notification */
      bta_hh_le_register_input_notif(p_cb, p_cb->mode, false);
      cback_data.status = BTA_HH_OK;
    }
    if (p_cb->state == BTA_HH_W4_CONN_ST) {
      p_cb->status =
          (cback_data.status == BTA_HH_OK) ? BTA_HH_OK : BTA_HH_ERR_PROTO;
    } else
      (*bta_hh_cb.p_cback)(BTA_HH_SET_PROTO_EVT, (tBTA_HH*)&cback_data);
  } else if (p_cb->mode != mode) {
    p_cb->mode = mode;
    mode = (mode == BTA_HH_PROTO_BOOT_MODE) ? BTA_HH_LE_PROTO_BOOT_MODE
                                            : BTA_HH_LE_PROTO_REPORT_MODE;

    BtaGattQueue::WriteCharacteristic(
        p_cb->conn_id, p_cb->hid_srvc.proto_mode_handle, {mode},
        GATT_WRITE_NO_RSP, write_proto_mode_cb, p_cb);
    return true;
  }

  return false;
}

/*******************************************************************************
 * Function         get_protocol_mode_cb
 *
 * Description      Process the Read protocol mode, send GET_PROTO_EVT to
 *                  application with the protocol mode.
 *
 ******************************************************************************/
static void get_protocol_mode_cb(uint16_t conn_id, tGATT_STATUS status,
                                 uint16_t handle, uint16_t len, uint8_t* value,
                                 void* data) {
  tBTA_HH_DEV_CB* p_dev_cb = (tBTA_HH_DEV_CB*)data;
  tBTA_HH_HSDATA hs_data;

  hs_data.status = BTA_HH_ERR;
  hs_data.handle = p_dev_cb->hid_handle;
  hs_data.rsp_data.proto_mode = p_dev_cb->mode;

  if (status == GATT_SUCCESS && len) {
    hs_data.status = BTA_HH_OK;
    /* match up BTE/BTA report/boot mode def*/
    hs_data.rsp_data.proto_mode = *(value);
    /* LE repot mode is the opposite value of BR/EDR report mode, flip it here
     */
    if (hs_data.rsp_data.proto_mode == 0)
      hs_data.rsp_data.proto_mode = BTA_HH_PROTO_BOOT_MODE;
    else
      hs_data.rsp_data.proto_mode = BTA_HH_PROTO_RPT_MODE;

    p_dev_cb->mode = hs_data.rsp_data.proto_mode;
  }

#if (BTA_HH_DEBUG == TRUE)
  APPL_TRACE_DEBUG("LE GET_PROTOCOL Mode = [%s]",
                   (hs_data.rsp_data.proto_mode == BTA_HH_PROTO_RPT_MODE)
                       ? "Report"
                       : "Boot");
#endif

  p_dev_cb->w4_evt = 0;
  (*bta_hh_cb.p_cback)(BTA_HH_GET_PROTO_EVT, (tBTA_HH*)&hs_data);
}

/*******************************************************************************
 *
 * Function         bta_hh_le_get_protocol_mode
 *
 * Description      Get remote device protocol mode.
 *
 ******************************************************************************/
void bta_hh_le_get_protocol_mode(tBTA_HH_DEV_CB* p_cb) {
  tBTA_HH_HSDATA hs_data;
  p_cb->w4_evt = BTA_HH_GET_PROTO_EVT;

  if (p_cb->hid_srvc.in_use && p_cb->hid_srvc.proto_mode_handle != 0) {
    BtaGattQueue::ReadCharacteristic(p_cb->conn_id,
                                     p_cb->hid_srvc.proto_mode_handle,
                                     get_protocol_mode_cb, p_cb);
    return;
  }

  /* no service support protocol_mode, by default report mode */
  hs_data.status = BTA_HH_OK;
  hs_data.handle = p_cb->hid_handle;
  hs_data.rsp_data.proto_mode = BTA_HH_PROTO_RPT_MODE;
  p_cb->w4_evt = 0;
  (*bta_hh_cb.p_cback)(BTA_HH_GET_PROTO_EVT, (tBTA_HH*)&hs_data);
}

/*******************************************************************************
 *
 * Function         bta_hh_le_dis_cback
 *
 * Description      DIS read complete callback
 *
 * Parameters:
 *
 ******************************************************************************/
void bta_hh_le_dis_cback(const RawAddress& addr, tDIS_VALUE* p_dis_value) {
  tBTA_HH_DEV_CB* p_cb = bta_hh_le_find_dev_cb_by_bda(addr);

  if (p_cb == NULL || p_dis_value == NULL) {
    APPL_TRACE_ERROR("received unexpected/error DIS callback");
    return;
  }

  p_cb->disc_active &= ~BTA_HH_LE_DISC_DIS;
  /* plug in the PnP info for this device */
  if (p_dis_value->attr_mask & DIS_ATTR_PNP_ID_BIT) {
#if (BTA_HH_DEBUG == TRUE)
    APPL_TRACE_DEBUG(
        "Plug in PnP info: product_id = %02x, vendor_id = %04x, version = %04x",
        p_dis_value->pnp_id.product_id, p_dis_value->pnp_id.vendor_id,
        p_dis_value->pnp_id.product_version);
#endif
    p_cb->dscp_info.product_id = p_dis_value->pnp_id.product_id;
    p_cb->dscp_info.vendor_id = p_dis_value->pnp_id.vendor_id;
    p_cb->dscp_info.version = p_dis_value->pnp_id.product_version;
  }
  bta_hh_le_open_cmpl(p_cb);
}

/*******************************************************************************
 *
 * Function         bta_hh_le_pri_service_discovery
 *
 * Description      Initialize GATT discovery on the remote LE HID device by
 *                  opening a GATT connection first.
 *
 * Parameters:
 *
 ******************************************************************************/
void bta_hh_le_pri_service_discovery(tBTA_HH_DEV_CB* p_cb) {
  bta_hh_le_co_reset_rpt_cache(p_cb->addr, p_cb->app_id);

  p_cb->disc_active |= (BTA_HH_LE_DISC_HIDS | BTA_HH_LE_DISC_DIS);

  /* read DIS info */
  if (!DIS_ReadDISInfo(p_cb->addr, bta_hh_le_dis_cback, DIS_ATTR_PNP_ID_BIT)) {
    APPL_TRACE_ERROR("read DIS failed");
    p_cb->disc_active &= ~BTA_HH_LE_DISC_DIS;
  }

  /* in parallel */
  /* start primary service discovery for HID service */
  Uuid pri_srvc = Uuid::From16Bit(UUID_SERVCLASS_LE_HID);
  BTA_GATTC_ServiceSearchRequest(p_cb->conn_id, &pri_srvc);
  return;
}

/*******************************************************************************
 *
 * Function         bta_hh_le_encrypt_cback
 *
 * Description      link encryption complete callback for bond verification.
 *
 * Returns          None
 *
 ******************************************************************************/
void bta_hh_le_encrypt_cback(const RawAddress* bd_addr,
                             UNUSED_ATTR tGATT_TRANSPORT transport,
                             UNUSED_ATTR void* p_ref_data, tBTM_STATUS result) {
  uint8_t idx = bta_hh_find_cb(*bd_addr);
  tBTA_HH_DEV_CB* p_dev_cb;

  if (idx != BTA_HH_IDX_INVALID)
    p_dev_cb = &bta_hh_cb.kdev[idx];
  else {
    APPL_TRACE_ERROR("unexpected encryption callback, ignore");
    return;
  }
  p_dev_cb->status = (result == BTM_SUCCESS) ? BTA_HH_OK : BTA_HH_ERR_SEC;
  p_dev_cb->reason = result;

  bta_hh_sm_execute(p_dev_cb, BTA_HH_ENC_CMPL_EVT, NULL);
}

/*******************************************************************************
 *
 * Function         bta_hh_security_cmpl
 *
 * Description      Security check completed, start the service discovery
 *                  if no cache available, otherwise report connection open
 *                  completed
 *
 * Parameters:
 *
 ******************************************************************************/
void bta_hh_security_cmpl(tBTA_HH_DEV_CB* p_cb,
                          UNUSED_ATTR tBTA_HH_DATA* p_buf) {
  APPL_TRACE_DEBUG("%s", __func__);
  if (p_cb->status == BTA_HH_OK) {
    if (!p_cb->hid_srvc.in_use) {
      APPL_TRACE_DEBUG("bta_hh_security_cmpl no reports loaded, try to load");

      /* start loading the cache if not in stack */
      // TODO(jpawlowski): cache storage is broken, fix it
      // tBTA_HH_RPT_CACHE_ENTRY     *p_rpt_cache;
      // uint8_t                       num_rpt = 0;
      // if ((p_rpt_cache = bta_hh_le_co_cache_load(p_cb->addr, &num_rpt,
      // p_cb->app_id)) != NULL)
      // {
      //     bta_hh_process_cache_rpt(p_cb, p_rpt_cache, num_rpt);
      // }
    }
    /*  discovery has been done for HID service */
    if (p_cb->app_id != 0 && p_cb->hid_srvc.in_use) {
      APPL_TRACE_DEBUG("%s: discovery has been done for HID service", __func__);
      /* configure protocol mode */
      if (!bta_hh_le_set_protocol_mode(p_cb, p_cb->mode)) {
        bta_hh_le_open_cmpl(p_cb);
      }
    }
    /* start primary service discovery for HID service */
    else {
      APPL_TRACE_DEBUG("%s: Starting service discovery", __func__);
      bta_hh_le_pri_service_discovery(p_cb);
    }
  } else {
    APPL_TRACE_ERROR("%s() - encryption failed; status=0x%04x, reason=0x%04x",
                     __func__, p_cb->status, p_cb->reason);
    if (!(p_cb->status == BTA_HH_ERR_SEC && p_cb->reason == BTM_ERR_PROCESSING))
      bta_hh_le_api_disc_act(p_cb);
  }
}

/*******************************************************************************
 *
 * Function         bta_hh_le_notify_enc_cmpl
 *
 * Description      process GATT encryption complete event
 *
 * Returns
 *
 ******************************************************************************/
void bta_hh_le_notify_enc_cmpl(tBTA_HH_DEV_CB* p_cb, tBTA_HH_DATA* p_buf) {
  if (p_cb == NULL || !p_cb->security_pending || p_buf == NULL ||
      p_buf->le_enc_cmpl.client_if != bta_hh_cb.gatt_if) {
    return;
  }

  p_cb->security_pending = false;
  bta_hh_start_security(p_cb, NULL);
}

/*******************************************************************************
 *
 * Function         bta_hh_clear_service_cache
 *
 * Description      clear the service cache
 *
 * Parameters:
 *
 ******************************************************************************/
void bta_hh_clear_service_cache(tBTA_HH_DEV_CB* p_cb) {
  tBTA_HH_LE_HID_SRVC* p_hid_srvc = &p_cb->hid_srvc;

  p_cb->app_id = 0;
  p_cb->dscp_info.descriptor.dsc_list = NULL;

  osi_free_and_reset((void**)&p_hid_srvc->rpt_map);
  memset(p_hid_srvc, 0, sizeof(tBTA_HH_LE_HID_SRVC));
}

/*******************************************************************************
 *
 * Function         bta_hh_start_security
 *
 * Description      start the security check of the established connection
 *
 * Parameters:
 *
 ******************************************************************************/
void bta_hh_start_security(tBTA_HH_DEV_CB* p_cb,
                           UNUSED_ATTR tBTA_HH_DATA* p_buf) {
  uint8_t sec_flag = 0;
  tBTM_SEC_DEV_REC* p_dev_rec;

  p_dev_rec = btm_find_dev(p_cb->addr);
  if (p_dev_rec) {
    if (p_dev_rec->sec_state == BTM_SEC_STATE_ENCRYPTING ||
        p_dev_rec->sec_state == BTM_SEC_STATE_AUTHENTICATING) {
      /* if security collision happened, wait for encryption done */
      p_cb->security_pending = true;
      return;
    }
  }

  /* verify bond */
  BTM_GetSecurityFlagsByTransport(p_cb->addr, &sec_flag, BT_TRANSPORT_LE);

  /* if link has been encrypted */
  if (sec_flag & BTM_SEC_FLAG_ENCRYPTED) {
    p_cb->status = BTA_HH_OK;
    bta_hh_sm_execute(p_cb, BTA_HH_ENC_CMPL_EVT, NULL);
  }
  /* if bonded and link not encrypted */
  else if (sec_flag & BTM_SEC_FLAG_LKEY_KNOWN) {
    sec_flag = BTM_BLE_SEC_ENCRYPT;
    p_cb->status = BTA_HH_ERR_AUTH_FAILED;
    BTM_SetEncryption(p_cb->addr, BTA_TRANSPORT_LE, bta_hh_le_encrypt_cback,
                      NULL, sec_flag);
  }
  /* unbonded device, report security error here */
  else if (p_cb->sec_mask != BTA_SEC_NONE) {
    sec_flag = BTM_BLE_SEC_ENCRYPT_NO_MITM;
    p_cb->status = BTA_HH_ERR_AUTH_FAILED;
    bta_hh_clear_service_cache(p_cb);
    BTM_SetEncryption(p_cb->addr, BTA_TRANSPORT_LE, bta_hh_le_encrypt_cback,
                      NULL, sec_flag);
  }
  /* otherwise let it go through */
  else {
    bta_hh_sm_execute(p_cb, BTA_HH_ENC_CMPL_EVT, NULL);
  }
}

/*******************************************************************************
 *
 * Function         bta_hh_gatt_open
 *
 * Description      process GATT open event.
 *
 * Parameters:
 *
 ******************************************************************************/
void bta_hh_gatt_open(tBTA_HH_DEV_CB* p_cb, tBTA_HH_DATA* p_buf) {
  tBTA_GATTC_OPEN* p_data = &p_buf->le_open;
  uint8_t* p2;

  /* if received invalid callback data , ignore it */
  if (p_cb == NULL || p_data == NULL) return;

  p2 = p_data->remote_bda.address;

  APPL_TRACE_DEBUG(
      "bta_hh_gatt_open BTA_GATTC_OPEN_EVT bda= [%08x%04x] status =%d",
      ((p2[0]) << 24) + ((p2[1]) << 16) + ((p2[2]) << 8) + (p2[3]),
      ((p2[4]) << 8) + p2[5], p_data->status);

  if (p_data->status == GATT_SUCCESS) {
    p_cb->hid_handle = bta_hh_le_get_le_dev_hdl(p_cb->index);
    if (p_cb->hid_handle == BTA_HH_IDX_INVALID) {
      p_cb->conn_id = p_data->conn_id;
      bta_hh_le_api_disc_act(p_cb);
      return;
    }
    p_cb->is_le_device = true;
    p_cb->in_use = true;
    p_cb->conn_id = p_data->conn_id;

    bta_hh_cb.le_cb_index[BTA_HH_GET_LE_CB_IDX(p_cb->hid_handle)] = p_cb->index;

    BtaGattQueue::Clean(p_cb->conn_id);

#if (BTA_HH_DEBUG == TRUE)
    APPL_TRACE_DEBUG("hid_handle = %2x conn_id = %04x cb_index = %d",
                     p_cb->hid_handle, p_cb->conn_id, p_cb->index);
#endif

    bta_hh_sm_execute(p_cb, BTA_HH_START_ENC_EVT, NULL);

  } else {
    /* open failure */
    tBTA_HH_DATA bta_hh_data;
    bta_hh_data.status = BTA_HH_ERR;
    bta_hh_sm_execute(p_cb, BTA_HH_SDP_CMPL_EVT, &bta_hh_data);
  }
}

/*******************************************************************************
 *
 * Function         bta_hh_le_close
 *
 * Description      This function process the GATT close event and post it as a
 *                  BTA HH internal event
 *
 * Parameters:
 *
 ******************************************************************************/
void bta_hh_le_close(tBTA_GATTC_CLOSE* p_data) {
  tBTA_HH_DEV_CB* p_dev_cb = bta_hh_le_find_dev_cb_by_bda(p_data->remote_bda);
  uint16_t sm_event = BTA_HH_GATT_CLOSE_EVT;

  if (p_dev_cb != NULL) {
    tBTA_HH_LE_CLOSE* p_buf =
        (tBTA_HH_LE_CLOSE*)osi_malloc(sizeof(tBTA_HH_LE_CLOSE));
    p_buf->hdr.event = sm_event;
    p_buf->hdr.layer_specific = (uint16_t)p_dev_cb->hid_handle;
    p_buf->conn_id = p_data->conn_id;
    p_buf->reason = p_data->reason;

    p_dev_cb->conn_id = GATT_INVALID_CONN_ID;
    p_dev_cb->security_pending = false;
    bta_sys_sendmsg(p_buf);
  }
}

/*******************************************************************************
 *
 * Function         bta_hh_le_gatt_disc_cmpl
 *
 * Description      Check to see if the remote device is a LE only device
 *
 * Parameters:
 *
 ******************************************************************************/
void bta_hh_le_gatt_disc_cmpl(tBTA_HH_DEV_CB* p_cb, tBTA_HH_STATUS status) {
  APPL_TRACE_DEBUG("bta_hh_le_gatt_disc_cmpl ");

  /* if open sucessful or protocol mode not desired, keep the connection open
   * but inform app */
  if (status == BTA_HH_OK || status == BTA_HH_ERR_PROTO) {
    /* assign a special APP ID temp, since device type unknown */
    p_cb->app_id = BTA_HH_APP_ID_LE;

    /* set report notification configuration */
    p_cb->clt_cfg_idx = 0;
    bta_hh_le_write_rpt_clt_cfg(p_cb);
  } else /* error, close the GATT connection */
  {
    /* close GATT connection if it's on */
    bta_hh_le_api_disc_act(p_cb);
  }
}

static void read_hid_info_cb(uint16_t conn_id, tGATT_STATUS status,
                             uint16_t handle, uint16_t len, uint8_t* value,
                             void* data) {
  if (status != GATT_SUCCESS) {
    APPL_TRACE_ERROR("%s: error: %d", __func__, status);
    return;
  }

  if (len != 4) {
    APPL_TRACE_ERROR("%s: wrong length: %d", __func__, len);
    return;
  }

  tBTA_HH_DEV_CB* p_dev_cb = (tBTA_HH_DEV_CB*)data;
  uint8_t* pp = value;
  /* save device information */
  STREAM_TO_UINT16(p_dev_cb->dscp_info.version, pp);
  STREAM_TO_UINT8(p_dev_cb->dscp_info.ctry_code, pp);
  STREAM_TO_UINT8(p_dev_cb->dscp_info.flag, pp);
}

static void read_hid_report_map_cb(uint16_t conn_id, tGATT_STATUS status,
                                   uint16_t handle, uint16_t len,
                                   uint8_t* value, void* data) {
  if (status != GATT_SUCCESS) {
    APPL_TRACE_ERROR("%s: error reading characteristic: %d", __func__, status);
    return;
  }

  tBTA_HH_DEV_CB* p_dev_cb = (tBTA_HH_DEV_CB*)data;
  tBTA_HH_LE_HID_SRVC* p_srvc = &p_dev_cb->hid_srvc;

  osi_free_and_reset((void**)&p_srvc->rpt_map);

  if (len > 0) {
    p_srvc->rpt_map = (uint8_t*)osi_malloc(len);

    uint8_t* pp = value;
    STREAM_TO_ARRAY(p_srvc->rpt_map, pp, len);
    p_srvc->descriptor.dl_len = len;
    p_srvc->descriptor.dsc_list = p_dev_cb->hid_srvc.rpt_map;
  }
}

static void read_ext_rpt_ref_desc_cb(uint16_t conn_id, tGATT_STATUS status,
                                     uint16_t handle, uint16_t len,
                                     uint8_t* value, void* data) {
  if (status != GATT_SUCCESS) {
    APPL_TRACE_ERROR("%s: error: %d", __func__, status);
    return;
  }

  /* if the length of the descriptor value is right, parse it assume it's a 16
   * bits UUID */
  if (len != Uuid::kNumBytes16) {
    APPL_TRACE_ERROR("%s: we support only 16bit UUID: %d", __func__, len);
    return;
  }

  tBTA_HH_DEV_CB* p_dev_cb = (tBTA_HH_DEV_CB*)data;
  uint8_t* pp = value;

  STREAM_TO_UINT16(p_dev_cb->hid_srvc.ext_rpt_ref, pp);

#if (BTA_HH_DEBUG == TRUE)
  APPL_TRACE_DEBUG("%s: External Report Reference UUID 0x%04x", __func__,
                   p_dev_cb->hid_srvc.ext_rpt_ref);
#endif
}

static void read_report_ref_desc_cb(uint16_t conn_id, tGATT_STATUS status,
                                    uint16_t handle, uint16_t len,
                                    uint8_t* value, void* data) {
  if (status != GATT_SUCCESS) {
    APPL_TRACE_ERROR("%s: error: %d", __func__, status);
    return;
  }

  tBTA_HH_DEV_CB* p_dev_cb = (tBTA_HH_DEV_CB*)data;
  const gatt::Descriptor* p_desc = BTA_GATTC_GetDescriptor(conn_id, handle);

  if (!p_desc) {
    APPL_TRACE_ERROR("%s: error: descriptor is null!", __func__);
    return;
  }

  const gatt::Characteristic* characteristic =
      BTA_GATTC_GetOwningCharacteristic(conn_id, handle);
  const gatt::Service* service =
      BTA_GATTC_GetOwningService(conn_id, characteristic->value_handle);

  tBTA_HH_LE_RPT* p_rpt;
  p_rpt = bta_hh_le_find_report_entry(p_dev_cb, service->handle,
                                      GATT_UUID_HID_REPORT,
                                      characteristic->value_handle);
  if (p_rpt) bta_hh_le_save_report_ref(p_dev_cb, p_rpt, status, value, len);
}

void read_pref_conn_params_cb(uint16_t conn_id, tGATT_STATUS status,
                              uint16_t handle, uint16_t len, uint8_t* value,
                              void* data) {
  if (status != GATT_SUCCESS) {
    APPL_TRACE_ERROR("%s: error: %d", __func__, status);
    return;
  }

  if (len != 8) {
    APPL_TRACE_ERROR("%s: we support only 16bit UUID: %d", __func__, len);
    return;
  }

  // TODO(jpawlowski): this should be done by GAP profile, remove when GAP is
  // fixed.
  uint8_t* pp = value;
  uint16_t min_interval, max_interval, latency, timeout;
  STREAM_TO_UINT16(min_interval, pp);
  STREAM_TO_UINT16(max_interval, pp);
  STREAM_TO_UINT16(latency, pp);
  STREAM_TO_UINT16(timeout, pp);

  // Make sure both min, and max are bigger than 11.25ms, lower values can
  // introduce audio issues if A2DP is also active.
  L2CA_AdjustConnectionIntervals(&min_interval, &max_interval,
                                 BTM_BLE_CONN_INT_MIN_LIMIT);

  // If the device has no preferred connection timeout, use the default.
  if (timeout == BTM_BLE_CONN_PARAM_UNDEF) timeout = BTM_BLE_CONN_TIMEOUT_DEF;

  if (min_interval < BTM_BLE_CONN_INT_MIN ||
      min_interval > BTM_BLE_CONN_INT_MAX ||
      max_interval < BTM_BLE_CONN_INT_MIN ||
      max_interval > BTM_BLE_CONN_INT_MAX ||
      latency > BTM_BLE_CONN_LATENCY_MAX ||
      timeout < BTM_BLE_CONN_SUP_TOUT_MIN ||
      timeout > BTM_BLE_CONN_SUP_TOUT_MAX || max_interval < min_interval) {
    APPL_TRACE_ERROR(
        "%s: Invalid connection parameters. min=%d, max=%d, latency=%d, "
        "timeout=%d",
        __func__, min_interval, max_interval, latency, timeout);
    return;
  }

  tBTA_HH_DEV_CB* p_dev_cb = (tBTA_HH_DEV_CB*)data;

  if (interop_match_addr(INTEROP_HID_PREF_CONN_SUP_TIMEOUT_3S,
                         (RawAddress*)&p_dev_cb->addr)) {
    if (timeout < 300) timeout = 300;
  }

  BTM_BleSetPrefConnParams(p_dev_cb->addr, min_interval, max_interval, latency,
                           timeout);
  L2CA_UpdateBleConnParams(p_dev_cb->addr, min_interval, max_interval, latency,
                           timeout);
}

/*******************************************************************************
 *
 * Function         bta_hh_le_search_hid_chars
 *
 * Description      This function discover all characteristics a service and
 *                  all descriptors available.
 *
 * Parameters:
 *
 ******************************************************************************/
static void bta_hh_le_search_hid_chars(tBTA_HH_DEV_CB* p_dev_cb,
                                       const gatt::Service* service) {
  tBTA_HH_LE_RPT* p_rpt;

  for (const gatt::Characteristic& charac : service->characteristics) {
    if (!charac.uuid.Is16Bit()) continue;

    uint16_t uuid16 = charac.uuid.As16Bit();
    LOG_DEBUG(LOG_TAG, "%s: %s %s", __func__, bta_hh_uuid_to_str(uuid16),
              charac.uuid.ToString().c_str());

    switch (uuid16) {
      case GATT_UUID_HID_CONTROL_POINT:
        p_dev_cb->hid_srvc.control_point_handle = charac.value_handle;
        break;
      case GATT_UUID_HID_INFORMATION:
        /* only one instance per HID service */
        BtaGattQueue::ReadCharacteristic(p_dev_cb->conn_id, charac.value_handle,
                                         read_hid_info_cb, p_dev_cb);
        break;
      case GATT_UUID_HID_REPORT_MAP:
        /* only one instance per HID service */
        BtaGattQueue::ReadCharacteristic(p_dev_cb->conn_id, charac.value_handle,
                                         read_hid_report_map_cb, p_dev_cb);
        /* descriptor is optional */
        bta_hh_le_read_char_descriptor(p_dev_cb, charac.value_handle,
                                       GATT_UUID_EXT_RPT_REF_DESCR,
                                       read_ext_rpt_ref_desc_cb, p_dev_cb);
        break;

      case GATT_UUID_HID_REPORT:
        p_rpt = bta_hh_le_find_alloc_report_entry(
            p_dev_cb, p_dev_cb->hid_srvc.srvc_inst_id, GATT_UUID_HID_REPORT,
            charac.value_handle);
        if (p_rpt == NULL) {
          APPL_TRACE_ERROR("%s: Add report entry failed !!!", __func__);
          break;
        }

        if (p_rpt->rpt_type != BTA_HH_RPTT_INPUT) break;

        bta_hh_le_read_char_descriptor(p_dev_cb, charac.value_handle,
                                       GATT_UUID_RPT_REF_DESCR,
                                       read_report_ref_desc_cb, p_dev_cb);
        break;

      /* found boot mode report types */
      case GATT_UUID_HID_BT_KB_OUTPUT:
      case GATT_UUID_HID_BT_MOUSE_INPUT:
      case GATT_UUID_HID_BT_KB_INPUT:
        if (bta_hh_le_find_alloc_report_entry(p_dev_cb, service->handle, uuid16,
                                              charac.value_handle) == NULL)
          APPL_TRACE_ERROR("%s: Add report entry failed !!!", __func__);

        break;

      default:
        APPL_TRACE_DEBUG("%s: not processing %s 0x%04d", __func__,
                         bta_hh_uuid_to_str(uuid16), uuid16);
    }
  }

  /* Make sure PROTO_MODE is processed as last */
  for (const gatt::Characteristic& charac : service->characteristics) {
    if (charac.uuid == Uuid::From16Bit(GATT_UUID_HID_PROTO_MODE)) {
      p_dev_cb->hid_srvc.proto_mode_handle = charac.value_handle;
      bta_hh_le_set_protocol_mode(p_dev_cb, p_dev_cb->mode);
      break;
    }
  }
}

/*******************************************************************************
 *
 * Function         bta_hh_le_srvc_search_cmpl
 *
 * Description      This function process the GATT service search complete.
 *
 * Parameters:
 *
 ******************************************************************************/
void bta_hh_le_srvc_search_cmpl(tBTA_GATTC_SEARCH_CMPL* p_data) {
  tBTA_HH_DEV_CB* p_dev_cb = bta_hh_le_find_dev_cb_by_conn_id(p_data->conn_id);

  /* service search exception or no HID service is supported on remote */
  if (p_dev_cb == NULL) return;

  if (p_data->status != GATT_SUCCESS) {
    p_dev_cb->status = BTA_HH_ERR_SDP;
    /* close the connection and report service discovery complete with error */
    bta_hh_le_api_disc_act(p_dev_cb);
    return;
  }

  const std::list<gatt::Service>* services =
      BTA_GATTC_GetServices(p_data->conn_id);

  bool have_hid = false;
  for (const gatt::Service& service : *services) {
    if (service.uuid == Uuid::From16Bit(UUID_SERVCLASS_LE_HID) &&
        service.is_primary && !have_hid) {
      have_hid = true;

      /* found HID primamry service */
      p_dev_cb->hid_srvc.in_use = true;
      p_dev_cb->hid_srvc.srvc_inst_id = service.handle;
      p_dev_cb->hid_srvc.proto_mode_handle = 0;
      p_dev_cb->hid_srvc.control_point_handle = 0;

      bta_hh_le_search_hid_chars(p_dev_cb, &service);

      APPL_TRACE_DEBUG("%s: have HID service inst_id= %d", __func__,
                       p_dev_cb->hid_srvc.srvc_inst_id);
    } else if (service.uuid == Uuid::From16Bit(UUID_SERVCLASS_SCAN_PARAM)) {
      p_dev_cb->scan_refresh_char_handle = 0;

      for (const gatt::Characteristic& charac : service.characteristics) {
        if (charac.uuid == Uuid::From16Bit(GATT_UUID_SCAN_REFRESH)) {
          p_dev_cb->scan_refresh_char_handle = charac.value_handle;

          if (charac.properties & GATT_CHAR_PROP_BIT_NOTIFY)
            p_dev_cb->scps_notify |= BTA_HH_LE_SCPS_NOTIFY_SPT;
          else
            p_dev_cb->scps_notify = BTA_HH_LE_SCPS_NOTIFY_NONE;

          break;
        }
      }
    } else if (service.uuid == Uuid::From16Bit(UUID_SERVCLASS_GAP_SERVER)) {
      // TODO(jpawlowski): this should be done by GAP profile, remove when GAP
      // is fixed.
      for (const gatt::Characteristic& charac : service.characteristics) {
        if (charac.uuid == Uuid::From16Bit(GATT_UUID_GAP_PREF_CONN_PARAM)) {
          /* read the char value */
          BtaGattQueue::ReadCharacteristic(p_dev_cb->conn_id,
                                           charac.value_handle,
                                           read_pref_conn_params_cb, p_dev_cb);
          break;
        }
      }
    }
  }

  bta_hh_le_gatt_disc_cmpl(p_dev_cb, p_dev_cb->status);
}

/*******************************************************************************
 *
 * Function         bta_hh_le_input_rpt_notify
 *
 * Description      process the notificaton event, most likely for input report.
 *
 * Parameters:
 *
 ******************************************************************************/
void bta_hh_le_input_rpt_notify(tBTA_GATTC_NOTIFY* p_data) {
  tBTA_HH_DEV_CB* p_dev_cb = bta_hh_le_find_dev_cb_by_conn_id(p_data->conn_id);
  uint8_t app_id;
  uint8_t* p_buf;
  tBTA_HH_LE_RPT* p_rpt;

  if (p_dev_cb == NULL) {
    APPL_TRACE_ERROR(
        "%s: notification received from Unknown device, conn_id: 0x%04x",
        __func__, p_data->conn_id);
    return;
  }

  const gatt::Characteristic* p_char =
      BTA_GATTC_GetCharacteristic(p_dev_cb->conn_id, p_data->handle);
  if (p_char == NULL) {
    APPL_TRACE_ERROR(
        "%s: notification received for Unknown Characteristic, conn_id: "
        "0x%04x, handle: 0x%04x",
        __func__, p_dev_cb->conn_id, p_data->handle);
    return;
  }

  app_id = p_dev_cb->app_id;

  p_rpt =
      bta_hh_le_find_report_entry(p_dev_cb, p_dev_cb->hid_srvc.srvc_inst_id,
                                  p_char->uuid.As16Bit(), p_char->value_handle);
  if (p_rpt == NULL) {
    APPL_TRACE_ERROR(
        "%s: notification received for Unknown Report, uuid: %s, handle: "
        "0x%04x",
        __func__, p_char->uuid.ToString().c_str(), p_char->value_handle);
    return;
  }

  if (p_char->uuid == Uuid::From16Bit(GATT_UUID_HID_BT_MOUSE_INPUT))
    app_id = BTA_HH_APP_ID_MI;
  else if (p_char->uuid == Uuid::From16Bit(GATT_UUID_HID_BT_KB_INPUT))
    app_id = BTA_HH_APP_ID_KB;

  APPL_TRACE_DEBUG("Notification received on report ID: %d", p_rpt->rpt_id);

  /* need to append report ID to the head of data */
  if (p_rpt->rpt_id != 0) {
    p_buf = (uint8_t*)osi_malloc(p_data->len + 1);

    p_buf[0] = p_rpt->rpt_id;
    memcpy(&p_buf[1], p_data->value, p_data->len);
    ++p_data->len;
  } else {
    p_buf = p_data->value;
  }

  bta_hh_co_data((uint8_t)p_dev_cb->hid_handle, p_buf, p_data->len,
                 p_dev_cb->mode, 0, /* no sub class*/
                 p_dev_cb->dscp_info.ctry_code, p_dev_cb->addr, app_id);

  if (p_buf != p_data->value) osi_free(p_buf);
}

/*******************************************************************************
 *
 * Function         bta_hh_gatt_open_fail
 *
 * Description      action function to process the open fail
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_hh_le_open_fail(tBTA_HH_DEV_CB* p_cb, tBTA_HH_DATA* p_data) {
  tBTA_HH_CONN conn_dat;

  /* open failure in the middle of service discovery, clear all services */
  if (p_cb->disc_active & BTA_HH_LE_DISC_HIDS) {
    bta_hh_clear_service_cache(p_cb);
  }

  p_cb->disc_active = BTA_HH_LE_DISC_NONE;
  /* Failure in opening connection or GATT discovery failure */
  conn_dat.handle = p_cb->hid_handle;
  conn_dat.bda = p_cb->addr;
  conn_dat.le_hid = true;
  conn_dat.scps_supported = p_cb->scps_supported;

  if (p_cb->status == BTA_HH_OK)
    conn_dat.status = (p_data->le_close.reason == GATT_CONN_UNKNOWN)
                          ? p_cb->status
                          : BTA_HH_ERR;
  else
    conn_dat.status = p_cb->status;

  /* Report OPEN fail event */
  (*bta_hh_cb.p_cback)(BTA_HH_OPEN_EVT, (tBTA_HH*)&conn_dat);
}

/*******************************************************************************
 *
 * Function         bta_hh_gatt_close
 *
 * Description      action function to process the GATT close int he state
 *                  machine.
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_hh_gatt_close(tBTA_HH_DEV_CB* p_cb, tBTA_HH_DATA* p_data) {
  tBTA_HH_CBDATA disc_dat = {BTA_HH_OK, 0};

  /* deregister all notification */
  bta_hh_le_deregister_input_notif(p_cb);
  /* finaliza device driver */
  bta_hh_co_close(p_cb->hid_handle, p_cb->app_id);
  /* update total conn number */
  bta_hh_cb.cnt_num--;

  disc_dat.handle = p_cb->hid_handle;
  disc_dat.status = p_cb->status;

  (*bta_hh_cb.p_cback)(BTA_HH_CLOSE_EVT, (tBTA_HH*)&disc_dat);

  /* if no connection is active and HH disable is signaled, disable service */
  if (bta_hh_cb.cnt_num == 0 && bta_hh_cb.w4_disable) {
    bta_hh_disc_cmpl();
  } else {
#if (BTA_HH_LE_RECONN == TRUE)
    if (p_data->le_close.reason == GATT_CONN_TIMEOUT) {
      bta_hh_le_add_dev_bg_conn(p_cb, false);
    }
#endif
  }

  return;
}

/*******************************************************************************
 *
 * Function         bta_hh_le_api_disc_act
 *
 * Description      initaite a Close API to a remote HID device
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_hh_le_api_disc_act(tBTA_HH_DEV_CB* p_cb) {
  if (p_cb->conn_id != GATT_INVALID_CONN_ID) {
    BtaGattQueue::Clean(p_cb->conn_id);
    BTA_GATTC_Close(p_cb->conn_id);
    /* remove device from background connection if intended to disconnect,
       do not allow reconnection */
    bta_hh_le_remove_dev_bg_conn(p_cb);
  }
}

/*******************************************************************************
 *
 * Function         read_report_cb
 *
 * Description      Process the Read report complete, send GET_REPORT_EVT to
 *                  application with the report data.
 *
 * Parameters:
 *
 ******************************************************************************/
static void read_report_cb(uint16_t conn_id, tGATT_STATUS status,
                           uint16_t handle, uint16_t len, uint8_t* value,
                           void* data) {
  const gatt::Characteristic* p_char =
      BTA_GATTC_GetCharacteristic(conn_id, handle);

  if (p_char == NULL) return;

  uint16_t char_uuid = p_char->uuid.As16Bit();

  if (char_uuid != GATT_UUID_HID_REPORT &&
      char_uuid != GATT_UUID_HID_BT_KB_INPUT &&
      char_uuid != GATT_UUID_HID_BT_KB_OUTPUT &&
      char_uuid != GATT_UUID_HID_BT_MOUSE_INPUT &&
      char_uuid != GATT_UUID_BATTERY_LEVEL) {
    APPL_TRACE_ERROR("%s: Unexpected Read UUID: 0x%04x", __func__, char_uuid);
    return;
  }

  tBTA_HH_DEV_CB* p_dev_cb = (tBTA_HH_DEV_CB*)data;
  if (p_dev_cb->w4_evt != BTA_HH_GET_RPT_EVT) {
    APPL_TRACE_ERROR("Unexpected READ cmpl, w4_evt = %d", p_dev_cb->w4_evt);
    return;
  }

  /* GET_REPORT */
  BT_HDR* p_buf = NULL;
  tBTA_HH_LE_RPT* p_rpt;
  tBTA_HH_HSDATA hs_data;
  uint8_t* pp;

  memset(&hs_data, 0, sizeof(hs_data));
  hs_data.status = BTA_HH_ERR;
  hs_data.handle = p_dev_cb->hid_handle;

  if (status == GATT_SUCCESS) {
    const gatt::Service* p_svc =
        BTA_GATTC_GetOwningService(conn_id, p_char->value_handle);

    p_rpt = bta_hh_le_find_report_entry(p_dev_cb, p_svc->handle, char_uuid,
                                        p_char->value_handle);

    if (p_rpt != NULL && len) {
      p_buf = (BT_HDR*)osi_malloc(sizeof(BT_HDR) + len + 1);
      /* pack data send to app */
      hs_data.status = BTA_HH_OK;
      p_buf->len = len + 1;
      p_buf->layer_specific = 0;
      p_buf->offset = 0;

      /* attach report ID as the first byte of the report before sending it to
       * USB HID driver */
      pp = (uint8_t*)(p_buf + 1);
      UINT8_TO_STREAM(pp, p_rpt->rpt_id);
      memcpy(pp, value, len);

      hs_data.rsp_data.p_rpt_data = p_buf;
    }
  }

  p_dev_cb->w4_evt = 0;
  (*bta_hh_cb.p_cback)(BTA_HH_GET_RPT_EVT, (tBTA_HH*)&hs_data);

  osi_free_and_reset((void**)&p_buf);
}

/*******************************************************************************
 *
 * Function         bta_hh_le_get_rpt
 *
 * Description      GET_REPORT on a LE HID Report
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_hh_le_get_rpt(tBTA_HH_DEV_CB* p_cb, tBTA_HH_RPT_TYPE r_type,
                       uint8_t rpt_id) {
  tBTA_HH_LE_RPT* p_rpt = bta_hh_le_find_rpt_by_idtype(
      p_cb->hid_srvc.report, p_cb->mode, r_type, rpt_id);

  if (p_rpt == NULL) {
    APPL_TRACE_ERROR("%s: no matching report", __func__);
    return;
  }

  p_cb->w4_evt = BTA_HH_GET_RPT_EVT;
  BtaGattQueue::ReadCharacteristic(p_cb->conn_id, p_rpt->char_inst_id,
                                   read_report_cb, p_cb);
}

static void write_report_cb(uint16_t conn_id, tGATT_STATUS status,
                            uint16_t handle, void* data) {
  tBTA_HH_CBDATA cback_data;
  tBTA_HH_DEV_CB* p_dev_cb = (tBTA_HH_DEV_CB*)data;
  uint16_t cb_evt = p_dev_cb->w4_evt;

  if (cb_evt == 0) return;

#if (BTA_HH_DEBUG == TRUE)
  APPL_TRACE_DEBUG("bta_hh_le_write_cmpl w4_evt: %d", p_dev_cb->w4_evt);
#endif

  const gatt::Characteristic* p_char =
      BTA_GATTC_GetCharacteristic(conn_id, handle);
  uint16_t uuid = p_char->uuid.As16Bit();
  if (uuid != GATT_UUID_HID_REPORT && uuid != GATT_UUID_HID_BT_KB_INPUT &&
      uuid != GATT_UUID_HID_BT_MOUSE_INPUT &&
      uuid != GATT_UUID_HID_BT_KB_OUTPUT) {
    return;
  }

  /* Set Report finished */
  cback_data.handle = p_dev_cb->hid_handle;
  cback_data.status = (status == GATT_SUCCESS) ? BTA_HH_OK : BTA_HH_ERR;
  p_dev_cb->w4_evt = 0;
  (*bta_hh_cb.p_cback)(cb_evt, (tBTA_HH*)&cback_data);
}
/*******************************************************************************
 *
 * Function         bta_hh_le_write_rpt
 *
 * Description      SET_REPORT/or DATA output on a LE HID Report
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_hh_le_write_rpt(tBTA_HH_DEV_CB* p_cb, tBTA_HH_RPT_TYPE r_type,
                         BT_HDR* p_buf, uint16_t w4_evt) {
  tBTA_HH_LE_RPT* p_rpt;
  uint8_t rpt_id;

  if (p_buf == NULL || p_buf->len == 0) {
    APPL_TRACE_ERROR("%s: Illegal data", __func__);
    return;
  }

  /* strip report ID from the data */
  uint8_t* vec_start = (uint8_t*)(p_buf + 1) + p_buf->offset;
  STREAM_TO_UINT8(rpt_id, vec_start);
  vector<uint8_t> value(vec_start, vec_start + p_buf->len - 1);

  p_rpt = bta_hh_le_find_rpt_by_idtype(p_cb->hid_srvc.report, p_cb->mode,
                                       r_type, rpt_id);
  if (p_rpt == NULL) {
    APPL_TRACE_ERROR("%s: no matching report", __func__);
    osi_free(p_buf);
    return;
  }

  p_cb->w4_evt = w4_evt;

  const gatt::Characteristic* p_char =
      BTA_GATTC_GetCharacteristic(p_cb->conn_id, p_rpt->char_inst_id);

  tGATT_WRITE_TYPE write_type = GATT_WRITE;
  if (p_char && (p_char->properties & GATT_CHAR_PROP_BIT_WRITE_NR))
    write_type = GATT_WRITE_NO_RSP;

  BtaGattQueue::WriteCharacteristic(p_cb->conn_id, p_rpt->char_inst_id,
                                    std::move(value), write_type,
                                    write_report_cb, p_cb);
}

/*******************************************************************************
 *
 * Function         bta_hh_le_suspend
 *
 * Description      send LE suspend or exit suspend mode to remote device.
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_hh_le_suspend(tBTA_HH_DEV_CB* p_cb,
                       tBTA_HH_TRANS_CTRL_TYPE ctrl_type) {
  ctrl_type -= BTA_HH_CTRL_SUSPEND;

  // We don't care about response
  BtaGattQueue::WriteCharacteristic(
      p_cb->conn_id, p_cb->hid_srvc.control_point_handle, {(uint8_t)ctrl_type},
      GATT_WRITE_NO_RSP, NULL, NULL);
}

/*******************************************************************************
 *
 * Function         bta_hh_le_write_dev_act
 *
 * Description      Write LE device action. can be SET/GET/DATA transaction.
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_hh_le_write_dev_act(tBTA_HH_DEV_CB* p_cb, tBTA_HH_DATA* p_data) {
  switch (p_data->api_sndcmd.t_type) {
    case HID_TRANS_SET_PROTOCOL:
      p_cb->w4_evt = BTA_HH_SET_PROTO_EVT;
      bta_hh_le_set_protocol_mode(p_cb, p_data->api_sndcmd.param);
      break;

    case HID_TRANS_GET_PROTOCOL:
      bta_hh_le_get_protocol_mode(p_cb);
      break;

    case HID_TRANS_GET_REPORT:
      bta_hh_le_get_rpt(p_cb, p_data->api_sndcmd.param,
                        p_data->api_sndcmd.rpt_id);
      break;

    case HID_TRANS_SET_REPORT:
      bta_hh_le_write_rpt(p_cb, p_data->api_sndcmd.param,
                          p_data->api_sndcmd.p_data, BTA_HH_SET_RPT_EVT);
      break;

    case HID_TRANS_DATA: /* output report */

      bta_hh_le_write_rpt(p_cb, p_data->api_sndcmd.param,
                          p_data->api_sndcmd.p_data, BTA_HH_DATA_EVT);
      break;

    case HID_TRANS_CONTROL:
      /* no handshake event will be generated */
      /* if VC_UNPLUG is issued, set flag */
      if (p_data->api_sndcmd.param == BTA_HH_CTRL_SUSPEND ||
          p_data->api_sndcmd.param == BTA_HH_CTRL_EXIT_SUSPEND) {
        bta_hh_le_suspend(p_cb, p_data->api_sndcmd.param);
      }
      break;

    default:
      APPL_TRACE_ERROR("%s unsupported transaction for BLE HID device: %d",
                       __func__, p_data->api_sndcmd.t_type);
      break;
  }
}

/*******************************************************************************
 *
 * Function         bta_hh_le_get_dscp_act
 *
 * Description      Send ReportDescriptor to application for all HID services.
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_hh_le_get_dscp_act(tBTA_HH_DEV_CB* p_cb) {
  if (p_cb->hid_srvc.in_use) {
    p_cb->dscp_info.descriptor.dl_len = p_cb->hid_srvc.descriptor.dl_len;
    p_cb->dscp_info.descriptor.dsc_list = p_cb->hid_srvc.descriptor.dsc_list;

    (*bta_hh_cb.p_cback)(BTA_HH_GET_DSCP_EVT, (tBTA_HH*)&p_cb->dscp_info);
  }
}

/*******************************************************************************
 *
 * Function         bta_hh_le_add_dev_bg_conn
 *
 * Description      Remove a LE HID device from back ground connection
 *                  procedure.
 *
 * Returns          void
 *
 ******************************************************************************/
static void bta_hh_le_add_dev_bg_conn(tBTA_HH_DEV_CB* p_cb, bool check_bond) {
  uint8_t sec_flag = 0;
  bool to_add = true;

  if (check_bond) {
    /* start reconnection if remote is a bonded device */
    /* verify bond */
    BTM_GetSecurityFlagsByTransport(p_cb->addr, &sec_flag, BT_TRANSPORT_LE);

    if ((sec_flag & BTM_SEC_FLAG_LKEY_KNOWN) == 0) to_add = false;
  }

  if (/*p_cb->dscp_info.flag & BTA_HH_LE_NORMAL_CONN &&*/
      !p_cb->in_bg_conn && to_add) {
    /* add device into BG connection to accept remote initiated connection */
    BTA_GATTC_Open(bta_hh_cb.gatt_if, p_cb->addr, false, GATT_TRANSPORT_LE,
                   false);
    p_cb->in_bg_conn = true;
  }
  return;
}

/*******************************************************************************
 *
 * Function         bta_hh_le_add_device
 *
 * Description      Add a LE HID device as a known device, and also add the
 *                  address
 *                  into back ground connection WL for incoming connection.
 *
 * Returns          void
 *
 ******************************************************************************/
uint8_t bta_hh_le_add_device(tBTA_HH_DEV_CB* p_cb,
                             tBTA_HH_MAINT_DEV* p_dev_info) {
  p_cb->hid_handle = bta_hh_le_get_le_dev_hdl(p_cb->index);
  if (p_cb->hid_handle == BTA_HH_INVALID_HANDLE) return BTA_HH_INVALID_HANDLE;
  bta_hh_cb.le_cb_index[BTA_HH_GET_LE_CB_IDX(p_cb->hid_handle)] = p_cb->index;

  /* update DI information */
  bta_hh_update_di_info(
      p_cb, p_dev_info->dscp_info.vendor_id, p_dev_info->dscp_info.product_id,
      p_dev_info->dscp_info.version, p_dev_info->dscp_info.flag);

  /* add to BTA device list */
  bta_hh_add_device_to_list(
      p_cb, p_cb->hid_handle, p_dev_info->attr_mask,
      &p_dev_info->dscp_info.descriptor, p_dev_info->sub_class,
      p_dev_info->dscp_info.ssr_max_latency, p_dev_info->dscp_info.ssr_min_tout,
      p_dev_info->app_id);

  bta_hh_le_add_dev_bg_conn(p_cb, false);

  return p_cb->hid_handle;
}

/*******************************************************************************
 *
 * Function         bta_hh_le_remove_dev_bg_conn
 *
 * Description      Remove a LE HID device from back ground connection
 *                  procedure.
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_hh_le_remove_dev_bg_conn(tBTA_HH_DEV_CB* p_dev_cb) {
  if (p_dev_cb->in_bg_conn) {
    p_dev_cb->in_bg_conn = false;

    BTA_GATTC_CancelOpen(bta_hh_cb.gatt_if, p_dev_cb->addr, false);
  }

  /* deregister all notifications */
  bta_hh_le_deregister_input_notif(p_dev_cb);
}

/*******************************************************************************
 *
 * Function         bta_hh_gattc_callback
 *
 * Description      This is GATT client callback function used in BTA HH.
 *
 * Parameters:
 *
 ******************************************************************************/
static void bta_hh_gattc_callback(tBTA_GATTC_EVT event, tBTA_GATTC* p_data) {
  tBTA_HH_DEV_CB* p_dev_cb;
#if (BTA_HH_DEBUG == TRUE)
  APPL_TRACE_DEBUG("bta_hh_gattc_callback event = %d", event);
#endif
  if (p_data == NULL) return;

  switch (event) {
    case BTA_GATTC_DEREG_EVT: /* 1 */
      bta_hh_cleanup_disable(p_data->reg_oper.status);
      break;

    case BTA_GATTC_OPEN_EVT: /* 2 */
      p_dev_cb = bta_hh_le_find_dev_cb_by_bda(p_data->open.remote_bda);
      if (p_dev_cb) {
        bta_hh_sm_execute(p_dev_cb, BTA_HH_GATT_OPEN_EVT,
                          (tBTA_HH_DATA*)&p_data->open);
      }
      break;

    case BTA_GATTC_CLOSE_EVT: /* 5 */
      bta_hh_le_close(&p_data->close);
      break;

    case BTA_GATTC_SEARCH_CMPL_EVT: /* 6 */
      bta_hh_le_srvc_search_cmpl(&p_data->search_cmpl);
      break;

    case BTA_GATTC_NOTIF_EVT: /* 10 */
      bta_hh_le_input_rpt_notify(&p_data->notify);
      break;

    case BTA_GATTC_ENC_CMPL_CB_EVT: /* 17 */
      p_dev_cb = bta_hh_le_find_dev_cb_by_bda(p_data->enc_cmpl.remote_bda);
      if (p_dev_cb) {
        bta_hh_sm_execute(p_dev_cb, BTA_HH_GATT_ENC_CMPL_EVT,
                          (tBTA_HH_DATA*)&p_data->enc_cmpl);
      }
      break;

    default:
      break;
  }
}

static void read_report_descriptor_ccc_cb(uint16_t conn_id, tGATT_STATUS status,
                                          uint16_t handle, uint16_t len,
                                          uint8_t* value, void* data) {
  tBTA_HH_LE_RPT* p_rpt = (tBTA_HH_LE_RPT*)data;
  uint8_t* pp = value;
  STREAM_TO_UINT16(p_rpt->client_cfg_value, pp);

  APPL_TRACE_DEBUG("Read Client Configuration: 0x%04x",
                   p_rpt->client_cfg_value);
}

/*******************************************************************************
 *
 * Function         bta_hh_le_hid_read_rpt_clt_cfg
 *
 * Description      a test command to read report descriptor client
 *                  configuration
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_hh_le_hid_read_rpt_clt_cfg(const RawAddress& bd_addr, uint8_t rpt_id) {
  tBTA_HH_DEV_CB* p_cb = NULL;
  tBTA_HH_LE_RPT* p_rpt;
  uint8_t index = BTA_HH_IDX_INVALID;

  index = bta_hh_find_cb(bd_addr);
  if (index == BTA_HH_IDX_INVALID) {
    APPL_TRACE_ERROR("%s: unknown device", __func__);
    return;
  }

  p_cb = &bta_hh_cb.kdev[index];

  p_rpt = bta_hh_le_find_rpt_by_idtype(p_cb->hid_srvc.report, p_cb->mode,
                                       BTA_HH_RPTT_INPUT, rpt_id);

  if (p_rpt == NULL) {
    APPL_TRACE_ERROR("%s: no matching report", __func__);
    return;
  }

  bta_hh_le_read_char_descriptor(p_cb, p_rpt->char_inst_id,
                                 GATT_UUID_CHAR_CLIENT_CONFIG,
                                 read_report_descriptor_ccc_cb, p_rpt);
  return;
}

/*******************************************************************************
 *
 * Function         bta_hh_process_cache_rpt
 *
 * Description      Process the cached reports
 *
 * Parameters:
 *
 ******************************************************************************/
// TODO(jpawlowski): uncomment when fixed
// static void bta_hh_process_cache_rpt (tBTA_HH_DEV_CB *p_cb,
//                                       tBTA_HH_RPT_CACHE_ENTRY *p_rpt_cache,
//                                       uint8_t num_rpt)
// {
//     uint8_t                       i = 0;
//     tBTA_HH_LE_RPT              *p_rpt;

//     if (num_rpt != 0)  /* no cache is found */
//     {
//         p_cb->hid_srvc.in_use = true;

//         /* set the descriptor info */
//         p_cb->hid_srvc.descriptor.dl_len =
//                 p_cb->dscp_info.descriptor.dl_len;
//         p_cb->hid_srvc.descriptor.dsc_list =
//                     p_cb->dscp_info.descriptor.dsc_list;

//         for (; i <num_rpt; i ++, p_rpt_cache ++)
//         {
//             if ((p_rpt = bta_hh_le_find_alloc_report_entry (p_cb,
//                                                p_rpt_cache->srvc_inst_id,
//                                                p_rpt_cache->rpt_uuid,
//                                                p_rpt_cache->char_inst_id,
//                                                p_rpt_cache->prop))  == NULL)
//             {
//                 APPL_TRACE_ERROR("bta_hh_process_cache_rpt: allocation report
//                 entry failure");
//                 break;
//             }
//             else
//             {
//                 p_rpt->rpt_type =  p_rpt_cache->rpt_type;
//                 p_rpt->rpt_id   =  p_rpt_cache->rpt_id;

//                 if (p_rpt->uuid == GATT_UUID_HID_BT_KB_INPUT ||
//                     p_rpt->uuid == GATT_UUID_HID_BT_MOUSE_INPUT ||
//                     (p_rpt->uuid == GATT_UUID_HID_REPORT && p_rpt->rpt_type
//                     == BTA_HH_RPTT_INPUT))
//                 {
//                     p_rpt->client_cfg_value =
//                     GATT_CLT_CONFIG_NOTIFICATION;
//                 }
//             }
//         }
//     }
// }

#endif
