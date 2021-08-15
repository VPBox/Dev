/******************************************************************************
 *
 *  Copyright 1999-2013 Broadcom Corporation
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

#define LOG_TAG "bt_srvc"

#include "bt_target.h"
#include "bt_utils.h"
#include "gatt_api.h"
#include "gatt_int.h"
#include "osi/include/log.h"
#include "osi/include/osi.h"
#include "srvc_dis_int.h"
#include "srvc_eng_int.h"

using base::StringPrintf;
#define DIS_MAX_NUM_INC_SVR 0
#define DIS_MAX_CHAR_NUM 9
#define DIS_MAX_ATTR_NUM (DIS_MAX_CHAR_NUM * 2 + DIS_MAX_NUM_INC_SVR + 1)

#ifndef DIS_ATTR_DB_SIZE
#define DIS_ATTR_DB_SIZE \
  GATT_DB_MEM_SIZE(DIS_MAX_NUM_INC_SVR, DIS_MAX_CHAR_NUM, 0)
#endif

#define uint64_t_TO_STREAM(p, u64)   \
  {                                  \
    *(p)++ = (uint8_t)(u64);         \
    *(p)++ = (uint8_t)((u64) >> 8);  \
    *(p)++ = (uint8_t)((u64) >> 16); \
    *(p)++ = (uint8_t)((u64) >> 24); \
    *(p)++ = (uint8_t)((u64) >> 32); \
    *(p)++ = (uint8_t)((u64) >> 40); \
    *(p)++ = (uint8_t)((u64) >> 48); \
    *(p)++ = (uint8_t)((u64) >> 56); \
  }

static const uint16_t dis_attr_uuid[DIS_MAX_CHAR_NUM] = {
    GATT_UUID_SYSTEM_ID,
    GATT_UUID_MODEL_NUMBER_STR,
    GATT_UUID_SERIAL_NUMBER_STR,
    GATT_UUID_FW_VERSION_STR,
    GATT_UUID_HW_VERSION_STR,
    GATT_UUID_SW_VERSION_STR,
    GATT_UUID_MANU_NAME,
    GATT_UUID_IEEE_DATA,
    GATT_UUID_PNP_ID};

tDIS_CB dis_cb;

static tDIS_ATTR_MASK dis_uuid_to_attr(uint16_t uuid) {
  switch (uuid) {
    case GATT_UUID_SYSTEM_ID:
      return DIS_ATTR_SYS_ID_BIT;
    case GATT_UUID_MODEL_NUMBER_STR:
      return DIS_ATTR_MODEL_NUM_BIT;
    case GATT_UUID_SERIAL_NUMBER_STR:
      return DIS_ATTR_SERIAL_NUM_BIT;
    case GATT_UUID_FW_VERSION_STR:
      return DIS_ATTR_FW_NUM_BIT;
    case GATT_UUID_HW_VERSION_STR:
      return DIS_ATTR_HW_NUM_BIT;
    case GATT_UUID_SW_VERSION_STR:
      return DIS_ATTR_SW_NUM_BIT;
    case GATT_UUID_MANU_NAME:
      return DIS_ATTR_MANU_NAME_BIT;
    case GATT_UUID_IEEE_DATA:
      return DIS_ATTR_IEEE_DATA_BIT;
    case GATT_UUID_PNP_ID:
      return DIS_ATTR_PNP_ID_BIT;
    default:
      return 0;
  };
}

/*******************************************************************************
 *   dis_valid_handle_range
 *
 *   validate a handle to be a DIS attribute handle or not.
 ******************************************************************************/
bool dis_valid_handle_range(uint16_t handle) {
  if (handle >= dis_cb.service_handle && handle <= dis_cb.max_handle)
    return true;
  else
    return false;
}
/*******************************************************************************
 *   dis_write_attr_value
 *
 *   Process write DIS attribute request.
 ******************************************************************************/
uint8_t dis_write_attr_value(UNUSED_ATTR tGATT_WRITE_REQ* p_data,
                             tGATT_STATUS* p_status) {
  *p_status = GATT_WRITE_NOT_PERMIT;
  return SRVC_ACT_RSP;
}
/*******************************************************************************
 *   DIS Attributes Database Server Request callback
 ******************************************************************************/
uint8_t dis_read_attr_value(UNUSED_ATTR uint8_t clcb_idx, uint16_t handle,
                            tGATT_VALUE* p_value, bool is_long,
                            tGATT_STATUS* p_status) {
  tDIS_DB_ENTRY* p_db_attr = dis_cb.dis_attr;
  uint8_t *p = p_value->value, i, *pp;
  uint16_t offset = p_value->offset;
  uint8_t act = SRVC_ACT_RSP;
  tGATT_STATUS st = GATT_NOT_FOUND;

  for (i = 0; i < DIS_MAX_CHAR_NUM; i++, p_db_attr++) {
    if (handle == p_db_attr->handle) {
      if ((p_db_attr->uuid == GATT_UUID_PNP_ID ||
           p_db_attr->uuid == GATT_UUID_SYSTEM_ID) &&
          is_long) {
        st = GATT_NOT_LONG;
        break;
      }
      st = GATT_SUCCESS;

      switch (p_db_attr->uuid) {
        case GATT_UUID_MANU_NAME:
        case GATT_UUID_MODEL_NUMBER_STR:
        case GATT_UUID_SERIAL_NUMBER_STR:
        case GATT_UUID_FW_VERSION_STR:
        case GATT_UUID_HW_VERSION_STR:
        case GATT_UUID_SW_VERSION_STR:
        case GATT_UUID_IEEE_DATA:
          pp = dis_cb.dis_value
                   .data_string[p_db_attr->uuid - GATT_UUID_MODEL_NUMBER_STR];
          if (pp != NULL) {
            if (strlen((char*)pp) > GATT_MAX_ATTR_LEN)
              p_value->len = GATT_MAX_ATTR_LEN;
            else
              p_value->len = (uint16_t)strlen((char*)pp);
          } else
            p_value->len = 0;

          if (offset > p_value->len) {
            st = GATT_INVALID_OFFSET;
            break;
          } else {
            p_value->len -= offset;
            pp += offset;
            ARRAY_TO_STREAM(p, pp, p_value->len);
            VLOG(1) << "GATT_UUID_MANU_NAME len=0x" << std::hex
                    << +p_value->len;
          }
          break;

        case GATT_UUID_SYSTEM_ID:
          uint64_t_TO_STREAM(p, dis_cb.dis_value.system_id); /* int_min */
          p_value->len = DIS_SYSTEM_ID_SIZE;
          break;

        case GATT_UUID_PNP_ID:
          UINT8_TO_STREAM(p, dis_cb.dis_value.pnp_id.vendor_id_src);
          UINT16_TO_STREAM(p, dis_cb.dis_value.pnp_id.vendor_id);
          UINT16_TO_STREAM(p, dis_cb.dis_value.pnp_id.product_id);
          UINT16_TO_STREAM(p, dis_cb.dis_value.pnp_id.product_version);
          p_value->len = DIS_PNP_ID_SIZE;
          break;
      }
      break;
    }
  }
  *p_status = st;
  return act;
}

/*******************************************************************************
 *
 * Function         dis_gatt_c_read_dis_value_cmpl
 *
 * Description      Client read DIS database complete callback.
 *
 * Returns          void
 *
 ******************************************************************************/
static void dis_gatt_c_read_dis_value_cmpl(uint16_t conn_id) {
  tSRVC_CLCB* p_clcb = srvc_eng_find_clcb_by_conn_id(conn_id);

  dis_cb.dis_read_uuid_idx = 0xff;

  srvc_eng_release_channel(conn_id);

  if (dis_cb.p_read_dis_cback && p_clcb) {
    LOG_INFO(LOG_TAG, "%s conn_id:%d attr_mask = 0x%04x", __func__, conn_id,
             p_clcb->dis_value.attr_mask);

    (*dis_cb.p_read_dis_cback)(p_clcb->bda, &p_clcb->dis_value);
    dis_cb.p_read_dis_cback = NULL;
  }
}

/*******************************************************************************
 *
 * Function         dis_gatt_c_read_dis_req
 *
 * Description      Read remote device DIS attribute request.
 *
 * Returns          void
 *
 ******************************************************************************/
bool dis_gatt_c_read_dis_req(uint16_t conn_id) {
  tGATT_READ_PARAM param;

  memset(&param, 0, sizeof(tGATT_READ_PARAM));

  param.service.s_handle = 1;
  param.service.e_handle = 0xFFFF;
  param.service.auth_req = 0;

  while (dis_cb.dis_read_uuid_idx < DIS_MAX_CHAR_NUM) {
    if (dis_uuid_to_attr(dis_attr_uuid[dis_cb.dis_read_uuid_idx]) &
        dis_cb.request_mask) {
      param.service.uuid =
          bluetooth::Uuid::From16Bit(dis_attr_uuid[dis_cb.dis_read_uuid_idx]);

      if (GATTC_Read(conn_id, GATT_READ_BY_TYPE, &param) == GATT_SUCCESS)
        return true;

      LOG(ERROR) << "Read DISInfo: " << param.service.uuid
                 << " GATT_Read Failed";
    }

    dis_cb.dis_read_uuid_idx++;
  }

  dis_gatt_c_read_dis_value_cmpl(conn_id);

  return (false);
}

/*******************************************************************************
 *
 * Function         dis_c_cmpl_cback
 *
 * Description      Client operation complete callback.
 *
 * Returns          void
 *
 ******************************************************************************/
void dis_c_cmpl_cback(tSRVC_CLCB* p_clcb, tGATTC_OPTYPE op, tGATT_STATUS status,
                      tGATT_CL_COMPLETE* p_data) {
  uint16_t read_type = dis_attr_uuid[dis_cb.dis_read_uuid_idx];
  uint8_t *pp = NULL, *p_str;
  uint16_t conn_id = p_clcb->conn_id;

  VLOG(1) << __func__
          << StringPrintf("op_code: 0x%02x  status: 0x%02x read_type: 0x%04x",
                          op, status, read_type);

  if (op != GATTC_OPTYPE_READ) return;

  if (p_data != NULL && status == GATT_SUCCESS) {
    pp = p_data->att_value.value;

    switch (read_type) {
      case GATT_UUID_SYSTEM_ID:
        VLOG(1) << "DIS_ATTR_SYS_ID_BIT";
        if (p_data->att_value.len == DIS_SYSTEM_ID_SIZE) {
          p_clcb->dis_value.attr_mask |= DIS_ATTR_SYS_ID_BIT;
          /* save system ID*/
          STREAM_TO_UINT64(p_clcb->dis_value.system_id, pp);
        }
        break;

      case GATT_UUID_PNP_ID:
        if (p_data->att_value.len == DIS_PNP_ID_SIZE) {
          p_clcb->dis_value.attr_mask |= DIS_ATTR_PNP_ID_BIT;
          STREAM_TO_UINT8(p_clcb->dis_value.pnp_id.vendor_id_src, pp);
          STREAM_TO_UINT16(p_clcb->dis_value.pnp_id.vendor_id, pp);
          STREAM_TO_UINT16(p_clcb->dis_value.pnp_id.product_id, pp);
          STREAM_TO_UINT16(p_clcb->dis_value.pnp_id.product_version, pp);
        }
        break;

      case GATT_UUID_MODEL_NUMBER_STR:
      case GATT_UUID_SERIAL_NUMBER_STR:
      case GATT_UUID_FW_VERSION_STR:
      case GATT_UUID_HW_VERSION_STR:
      case GATT_UUID_SW_VERSION_STR:
      case GATT_UUID_MANU_NAME:
      case GATT_UUID_IEEE_DATA:
        p_str = p_clcb->dis_value
                    .data_string[read_type - GATT_UUID_MODEL_NUMBER_STR];
        osi_free(p_str);
        p_str = (uint8_t*)osi_malloc(p_data->att_value.len + 1);
        p_clcb->dis_value.attr_mask |= dis_uuid_to_attr(read_type);
        memcpy(p_str, p_data->att_value.value, p_data->att_value.len);
        p_str[p_data->att_value.len] = 0;
        p_clcb->dis_value.data_string[read_type - GATT_UUID_MODEL_NUMBER_STR] =
            p_str;
        break;

      default:
        break;

        break;
    } /* end switch */
  }   /* end if */

  dis_cb.dis_read_uuid_idx++;

  dis_gatt_c_read_dis_req(conn_id);
}

/*******************************************************************************
 *
 * Function         DIS_SrInit
 *
 * Description      Initialize the Device Information Service Server.
 *
 ******************************************************************************/
tDIS_STATUS DIS_SrInit(tDIS_ATTR_MASK dis_attr_mask) {
  tGATT_STATUS status;

  if (dis_cb.enabled) {
    LOG(ERROR) << "DIS already initalized";
    return DIS_SUCCESS;
  }

  memset(&dis_cb, 0, sizeof(tDIS_CB));

  btgatt_db_element_t service[DIS_MAX_ATTR_NUM] = {};

  bluetooth::Uuid svc_uuid =
      bluetooth::Uuid::From16Bit(UUID_SERVCLASS_DEVICE_INFO);
  service[0].type = BTGATT_DB_PRIMARY_SERVICE;
  service[0].uuid = svc_uuid;

  for (int i = 0; dis_attr_mask != 0 && i < DIS_MAX_CHAR_NUM; i++) {
    dis_cb.dis_attr[i].uuid = dis_attr_uuid[i];

    bluetooth::Uuid char_uuid =
        bluetooth::Uuid::From16Bit(dis_cb.dis_attr[i].uuid);
    /* index 0 is service, so characteristics start from 1 */
    service[i + 1].type = BTGATT_DB_CHARACTERISTIC;
    service[i + 1].uuid = char_uuid;
    service[i + 1].properties = GATT_CHAR_PROP_BIT_READ;
    service[i + 1].permissions = GATT_PERM_READ;

    dis_attr_mask >>= 1;
  }

  /* Add a GAP service */
  status = GATTS_AddService(srvc_eng_cb.gatt_if, service,
                            sizeof(service) / sizeof(btgatt_db_element_t));
  if (status != GATT_SERVICE_STARTED) {
    LOG(ERROR) << "Can not create service, DIS_Init failed!";
    return GATT_ERROR;
  }

  dis_cb.service_handle = service[0].attribute_handle;
  dis_cb.max_handle = dis_cb.service_handle + DIS_MAX_ATTR_NUM;

  for (int i = 0; i < DIS_MAX_CHAR_NUM; i++) {
    dis_cb.dis_attr[i].handle = service[i + 1].attribute_handle;

    VLOG(1) << StringPrintf("%s:  handle of new attribute 0x%04x = %d",
                            __func__, dis_cb.dis_attr[i].uuid,
                            dis_cb.dis_attr[i].handle);
  }

  dis_cb.enabled = true;
  return (tDIS_STATUS)status;
}
/*******************************************************************************
 *
 * Function         DIS_SrUpdate
 *
 * Description      Update the DIS server attribute values
 *
 ******************************************************************************/
tDIS_STATUS DIS_SrUpdate(tDIS_ATTR_BIT dis_attr_bit, tDIS_ATTR* p_info) {
  uint8_t i = 1;
  tDIS_STATUS st = DIS_SUCCESS;

  if (dis_attr_bit & DIS_ATTR_SYS_ID_BIT) {
    dis_cb.dis_value.system_id = p_info->system_id;
  } else if (dis_attr_bit & DIS_ATTR_PNP_ID_BIT) {
    dis_cb.dis_value.pnp_id.vendor_id = p_info->pnp_id.vendor_id;
    dis_cb.dis_value.pnp_id.vendor_id_src = p_info->pnp_id.vendor_id_src;
    dis_cb.dis_value.pnp_id.product_id = p_info->pnp_id.product_id;
    dis_cb.dis_value.pnp_id.product_version = p_info->pnp_id.product_version;
  } else {
    st = DIS_ILLEGAL_PARAM;

    while (dis_attr_bit && i < (DIS_MAX_CHAR_NUM - 1)) {
      if (dis_attr_bit & (uint16_t)(1 << i)) {
        osi_free(dis_cb.dis_value.data_string[i - 1]);
        dis_cb.dis_value.data_string[i - 1] =
            (uint8_t*)osi_malloc(p_info->data_str.len + 1);
        memcpy(dis_cb.dis_value.data_string[i - 1], p_info->data_str.p_data,
               p_info->data_str.len);
        dis_cb.dis_value.data_string[i - 1][p_info->data_str.len] =
            0; /* make sure null terminate */
        st = DIS_SUCCESS;

        break;
      }
      i++;
    }
  }
  return st;
}
/*******************************************************************************
 *
 * Function         DIS_ReadDISInfo
 *
 * Description      Read remote device DIS information.
 *
 * Returns          void
 *
 ******************************************************************************/
bool DIS_ReadDISInfo(const RawAddress& peer_bda, tDIS_READ_CBACK* p_cback,
                     tDIS_ATTR_MASK mask) {
  uint16_t conn_id;

  /* Initialize the DIS client if it hasn't been initialized already. */
  srvc_eng_init();

  /* For now we only handle one at a time */
  if (dis_cb.dis_read_uuid_idx != 0xff) return (false);

  if (p_cback == NULL) return (false);

  dis_cb.p_read_dis_cback = p_cback;
  /* Mark currently active operation */
  dis_cb.dis_read_uuid_idx = 0;

  dis_cb.request_mask = mask;

  VLOG(1) << __func__ << " BDA: " << peer_bda
          << StringPrintf(" cl_read_uuid: 0x%04x",
                          dis_attr_uuid[dis_cb.dis_read_uuid_idx]);

  GATT_GetConnIdIfConnected(srvc_eng_cb.gatt_if, peer_bda, &conn_id,
                            BT_TRANSPORT_LE);

  /* need to enhance it as multiple service is needed */
  srvc_eng_request_channel(peer_bda, SRVC_ID_DIS);

  if (conn_id == GATT_INVALID_CONN_ID) {
    return GATT_Connect(srvc_eng_cb.gatt_if, peer_bda, true, BT_TRANSPORT_LE,
                        false);
  }

  return dis_gatt_c_read_dis_req(conn_id);
}
