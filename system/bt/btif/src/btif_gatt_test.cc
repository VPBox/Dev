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

#define LOG_TAG "bt_btif_gatt"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hardware/bluetooth.h>
#include <hardware/bt_gatt.h>

#include "btif_common.h"
#include "btif_util.h"

#include "bta_gatt_api.h"
#include "bte_appl.h"
#include "btif_dm.h"
#include "btif_gatt.h"
#include "btif_gatt_util.h"
#include "btif_storage.h"
#include "gatt_api.h"
#include "osi/include/log.h"
#include "osi/include/osi.h"

using bluetooth::Uuid;
/*******************************************************************************
 * Typedefs & Macros
 ******************************************************************************/

typedef struct {
  tGATT_IF gatt_if;
  uint16_t conn_id;
} btif_test_cb_t;

/*******************************************************************************
 * Static variables
 ******************************************************************************/

static const char* disc_name[GATT_DISC_MAX] = {"Unknown",
                                               "GATT_DISC_SRVC_ALL",
                                               "GATT_DISC_SRVC_BY_UUID",
                                               "GATT_DISC_INC_SRVC",
                                               "GATT_DISC_CHAR",
                                               "GATT_DISC_CHAR_DSCPT"};

static btif_test_cb_t test_cb;

/*******************************************************************************
 * Callback functions
 ******************************************************************************/

static void btif_test_connect_cback(tGATT_IF, const RawAddress&,
                                    uint16_t conn_id, bool connected,
                                    tGATT_DISCONN_REASON, tBT_TRANSPORT) {
  LOG_DEBUG(LOG_TAG, "%s: conn_id=%d, connected=%d", __func__, conn_id,
            connected);
  test_cb.conn_id = connected ? conn_id : 0;
}

static void btif_test_command_complete_cback(uint16_t conn_id, tGATTC_OPTYPE op,
                                             tGATT_STATUS status,
                                             tGATT_CL_COMPLETE* p_data) {
  LOG_DEBUG(LOG_TAG, "%s: op_code=0x%02x, conn_id=0x%x. status=0x%x", __func__,
            op, conn_id, status);

  switch (op) {
    case GATTC_OPTYPE_READ:
    case GATTC_OPTYPE_WRITE:
    case GATTC_OPTYPE_CONFIG:
    case GATTC_OPTYPE_EXE_WRITE:
    case GATTC_OPTYPE_NOTIFICATION:
      break;

    case GATTC_OPTYPE_INDICATION:
      GATTC_SendHandleValueConfirm(conn_id, p_data->handle);
      break;

    default:
      LOG_DEBUG(LOG_TAG, "%s: Unknown op_code (0x%02x)", __func__, op);
      break;
  }
}

static void btif_test_discovery_result_cback(UNUSED_ATTR uint16_t conn_id,
                                             tGATT_DISC_TYPE disc_type,
                                             tGATT_DISC_RES* p_data) {
  LOG_DEBUG(LOG_TAG, "------ GATT Discovery result %-22s -------",
            disc_name[disc_type]);
  LOG_DEBUG(LOG_TAG, "      Attribute handle: 0x%04x (%d)", p_data->handle,
            p_data->handle);

  if (disc_type != GATT_DISC_CHAR_DSCPT) {
    LOG_DEBUG(LOG_TAG, "        Attribute type: %s",
              p_data->type.ToString().c_str());
  }

  switch (disc_type) {
    case GATT_DISC_SRVC_ALL:
      LOG_DEBUG(LOG_TAG, "          Handle range: 0x%04x ~ 0x%04x (%d ~ %d)",
                p_data->handle, p_data->value.group_value.e_handle,
                p_data->handle, p_data->value.group_value.e_handle);
      LOG_DEBUG(LOG_TAG, "          Service UUID: %s",
                p_data->value.group_value.service_type.ToString().c_str());
      break;

    case GATT_DISC_SRVC_BY_UUID:
      LOG_DEBUG(LOG_TAG, "          Handle range: 0x%04x ~ 0x%04x (%d ~ %d)",
                p_data->handle, p_data->value.handle, p_data->handle,
                p_data->value.handle);
      break;

    case GATT_DISC_INC_SRVC:
      LOG_DEBUG(LOG_TAG, "          Handle range: 0x%04x ~ 0x%04x (%d ~ %d)",
                p_data->value.incl_service.s_handle,
                p_data->value.incl_service.e_handle,
                p_data->value.incl_service.s_handle,
                p_data->value.incl_service.e_handle);
      LOG_DEBUG(LOG_TAG, "          Service UUID: %s",
                p_data->value.incl_service.service_type.ToString().c_str());
      break;

    case GATT_DISC_CHAR:
      LOG_DEBUG(LOG_TAG, "            Properties: 0x%02x",
                p_data->value.dclr_value.char_prop);
      LOG_DEBUG(LOG_TAG, "   Characteristic UUID: %s",
                p_data->value.dclr_value.char_uuid.ToString().c_str());
      break;

    case GATT_DISC_CHAR_DSCPT:
      LOG_DEBUG(LOG_TAG, "       Descriptor UUID: %s",
                p_data->type.ToString().c_str());
      break;
  }

  LOG_DEBUG(LOG_TAG,
            "-----------------------------------------------------------");
}

static void btif_test_discovery_complete_cback(
    UNUSED_ATTR uint16_t conn_id, UNUSED_ATTR tGATT_DISC_TYPE disc_type,
    tGATT_STATUS status) {
  LOG_DEBUG(LOG_TAG, "%s: status=%d", __func__, status);
}

static tGATT_CBACK btif_test_callbacks = {btif_test_connect_cback,
                                          btif_test_command_complete_cback,
                                          btif_test_discovery_result_cback,
                                          btif_test_discovery_complete_cback,
                                          NULL,
                                          NULL,
                                          NULL,
                                          NULL,
                                          NULL};

/*******************************************************************************
 * Implementation
 ******************************************************************************/

bt_status_t btif_gattc_test_command_impl(int command,
                                         const btgatt_test_params_t* params) {
  switch (command) {
    case 0x01: /* Enable */
    {
      LOG_DEBUG(LOG_TAG, "%s: ENABLE - enable=%d", __func__, params->u1);
      if (params->u1) {
        std::array<uint8_t, Uuid::kNumBytes128> tmp;
        tmp.fill(0xAE);
        test_cb.gatt_if = GATT_Register(bluetooth::Uuid::From128BitBE(tmp),
                                        &btif_test_callbacks);
        GATT_StartIf(test_cb.gatt_if);
      } else {
        GATT_Deregister(test_cb.gatt_if);
        test_cb.gatt_if = 0;
      }
      break;
    }

    case 0x02: /* Connect */
    {
      LOG_DEBUG(LOG_TAG, "%s: CONNECT - device=%s (dev_type=%d, addr_type=%d)",
                __func__, params->bda1->ToString().c_str(), params->u1,
                params->u2);

      if (params->u1 == BT_DEVICE_TYPE_BLE)
        BTM_SecAddBleDevice(*params->bda1, NULL, BT_DEVICE_TYPE_BLE,
                            params->u2);

      if (!GATT_Connect(test_cb.gatt_if, *params->bda1, true, BT_TRANSPORT_LE,
                        false)) {
        LOG_ERROR(LOG_TAG, "%s: GATT_Connect failed!", __func__);
      }
      break;
    }

    case 0x03: /* Disconnect */
    {
      LOG_DEBUG(LOG_TAG, "%s: DISCONNECT - conn_id=%d", __func__,
                test_cb.conn_id);
      GATT_Disconnect(test_cb.conn_id);
      break;
    }

    case 0x04: /* Discover */
    {
      if (params->u1 >= GATT_DISC_MAX) {
        LOG_ERROR(LOG_TAG, "%s: DISCOVER - Invalid type (%d)!", __func__,
                  params->u1);
        return (bt_status_t)0;
      }

      LOG_DEBUG(LOG_TAG,
                "%s: DISCOVER (%s), conn_id=%d, uuid=%s, handles=0x%04x-0x%04x",
                __func__, disc_name[params->u1], test_cb.conn_id,
                params->uuid1->ToString().c_str(), params->u2, params->u3);
      GATTC_Discover(test_cb.conn_id, params->u1, params->u2, params->u3,
                     *params->uuid1);
      break;
    }

    case 0xF0: /* Pairing configuration */
      LOG_DEBUG(LOG_TAG,
                "%s: Setting pairing config auth=%d, iocaps=%d, keys=%d/%d/%d",
                __func__, params->u1, params->u2, params->u3, params->u4,
                params->u5);

      bte_appl_cfg.ble_auth_req = params->u1;
      bte_appl_cfg.ble_io_cap = params->u2;
      bte_appl_cfg.ble_init_key = params->u3;
      bte_appl_cfg.ble_resp_key = params->u4;
      bte_appl_cfg.ble_max_key_size = params->u5;
      break;

    default:
      LOG_ERROR(LOG_TAG, "%s: UNKNOWN TEST COMMAND 0x%02x", __func__, command);
      break;
  }
  return (bt_status_t)0;
}
