/******************************************************************************
 *
 *  Copyright 2003-2016 Broadcom Corporation
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
#include <string.h>

#include "avrc_api.h"
#include "avrc_defs.h"
#include "avrc_int.h"
#include "bt_common.h"
#include "log/log.h"

/*****************************************************************************
 *  Global data
 ****************************************************************************/

/*******************************************************************************
 *
 * Function         avrc_ctrl_pars_vendor_cmd
 *
 * Description      This function parses the vendor specific commands defined by
 *                  Bluetooth SIG for AVRCP Conroller.
 *
 * Returns          AVRC_STS_NO_ERROR, if the message in p_data is parsed
 *                  successfully.
 *                  Otherwise, the error code defined by AVRCP 1.4
 *
 ******************************************************************************/
static tAVRC_STS avrc_ctrl_pars_vendor_cmd(tAVRC_MSG_VENDOR* p_msg,
                                           tAVRC_COMMAND* p_result) {
  tAVRC_STS status = AVRC_STS_NO_ERROR;

  uint8_t* p = p_msg->p_vendor_data;
  p_result->pdu = *p++;
  AVRC_TRACE_DEBUG("%s pdu:0x%x", __func__, p_result->pdu);
  if (!AVRC_IsValidAvcType(p_result->pdu, p_msg->hdr.ctype)) {
    AVRC_TRACE_DEBUG("%s detects wrong AV/C type!", __func__);
    status = AVRC_STS_BAD_CMD;
  }

  p++; /* skip the reserved byte */
  uint16_t len;
  BE_STREAM_TO_UINT16(len, p);
  if ((len + 4) != (p_msg->vendor_len)) {
    status = AVRC_STS_INTERNAL_ERR;
  }

  if (status != AVRC_STS_NO_ERROR) return status;

  switch (p_result->pdu) {
    case AVRC_PDU_SET_ABSOLUTE_VOLUME: {
      if (len != 1)
        status = AVRC_STS_INTERNAL_ERR;
      else {
        BE_STREAM_TO_UINT8(p_result->volume.volume, p);
        p_result->volume.volume = AVRC_MAX_VOLUME & p_result->volume.volume;
      }
      break;
    }
    case AVRC_PDU_REGISTER_NOTIFICATION: /* 0x31 */
      BE_STREAM_TO_UINT8(p_result->reg_notif.event_id, p);
      BE_STREAM_TO_UINT32(p_result->reg_notif.param, p);
      break;
    default:
      status = AVRC_STS_BAD_CMD;
      break;
  }
  return status;
}

/*******************************************************************************
 *
 * Function         avrc_pars_vendor_cmd
 *
 * Description      This function parses the vendor specific commands defined by
 *                  Bluetooth SIG
 *
 * Returns          AVRC_STS_NO_ERROR, if the message in p_data is parsed
 *                  successfully.
 *                  Otherwise, the error code defined by AVRCP 1.4
 *
 ******************************************************************************/
static tAVRC_STS avrc_pars_vendor_cmd(tAVRC_MSG_VENDOR* p_msg,
                                      tAVRC_COMMAND* p_result, uint8_t* p_buf,
                                      uint16_t buf_len) {
  tAVRC_STS status = AVRC_STS_NO_ERROR;
  uint8_t* p;
  uint16_t len;
  uint8_t xx, yy;
  uint8_t* p_u8;
  uint16_t* p_u16;
  uint32_t u32, u32_2, *p_u32;
  tAVRC_APP_SETTING* p_app_set;
  uint16_t size_needed;

  /* Check the vendor data */
  if (p_msg->vendor_len == 0) return AVRC_STS_NO_ERROR;
  if (p_msg->p_vendor_data == NULL) return AVRC_STS_INTERNAL_ERR;

  p = p_msg->p_vendor_data;
  p_result->pdu = *p++;
  AVRC_TRACE_DEBUG("%s pdu:0x%x", __func__, p_result->pdu);
  if (!AVRC_IsValidAvcType(p_result->pdu, p_msg->hdr.ctype)) {
    AVRC_TRACE_DEBUG("%s detects wrong AV/C type(0x%x)!", __func__,
                     p_msg->hdr.ctype);
    status = AVRC_STS_BAD_CMD;
  }

  p++; /* skip the reserved byte */
  BE_STREAM_TO_UINT16(len, p);
  if ((len + 4) != (p_msg->vendor_len)) {
    AVRC_TRACE_ERROR("%s incorrect length :%d, %d", __func__, len,
                     p_msg->vendor_len);
    status = AVRC_STS_INTERNAL_ERR;
  }

  if (status != AVRC_STS_NO_ERROR) return status;

  switch (p_result->pdu) {
    case AVRC_PDU_GET_CAPABILITIES: /* 0x10 */
      p_result->get_caps.capability_id = *p++;
      if (!AVRC_IS_VALID_CAP_ID(p_result->get_caps.capability_id))
        status = AVRC_STS_BAD_PARAM;
      else if (len != 1)
        return AVRC_STS_INTERNAL_ERR;
      break;

    case AVRC_PDU_LIST_PLAYER_APP_ATTR: /* 0x11 */
      /* no additional parameters */
      if (len != 0) return AVRC_STS_INTERNAL_ERR;
      break;

    case AVRC_PDU_LIST_PLAYER_APP_VALUES: /* 0x12 */
      if (len == 0) return AVRC_STS_INTERNAL_ERR;
      p_result->list_app_values.attr_id = *p++;
      if (!AVRC_IS_VALID_ATTRIBUTE(p_result->list_app_values.attr_id))
        status = AVRC_STS_BAD_PARAM;
      else if (len != 1)
        status = AVRC_STS_INTERNAL_ERR;
      break;

    case AVRC_PDU_GET_CUR_PLAYER_APP_VALUE: /* 0x13 */
    case AVRC_PDU_GET_PLAYER_APP_ATTR_TEXT: /* 0x15 */
      if (len == 0) return AVRC_STS_INTERNAL_ERR;
      BE_STREAM_TO_UINT8(p_result->get_cur_app_val.num_attr, p);
      if (len != (p_result->get_cur_app_val.num_attr + 1)) {
        status = AVRC_STS_INTERNAL_ERR;
        break;
      }

      if (p_result->get_cur_app_val.num_attr > AVRC_MAX_APP_ATTR_SIZE) {
        android_errorWriteLog(0x534e4554, "63146237");
        p_result->get_cur_app_val.num_attr = AVRC_MAX_APP_ATTR_SIZE;
      }

      p_u8 = p_result->get_cur_app_val.attrs;
      for (xx = 0, yy = 0; xx < p_result->get_cur_app_val.num_attr; xx++) {
        /* only report the valid player app attributes */
        if (AVRC_IsValidPlayerAttr(*p)) p_u8[yy++] = *p;
        p++;
      }
      p_result->get_cur_app_val.num_attr = yy;
      if (yy == 0) {
        status = AVRC_STS_BAD_PARAM;
      }
      break;

    case AVRC_PDU_SET_PLAYER_APP_VALUE: /* 0x14 */
      if (len == 0) return AVRC_STS_INTERNAL_ERR;
      BE_STREAM_TO_UINT8(p_result->set_app_val.num_val, p);
      size_needed = sizeof(tAVRC_APP_SETTING);
      if (p_buf && (len == ((p_result->set_app_val.num_val << 1) + 1))) {
        p_result->set_app_val.p_vals = (tAVRC_APP_SETTING*)p_buf;
        p_app_set = p_result->set_app_val.p_vals;
        for (xx = 0;
             ((xx < p_result->set_app_val.num_val) && (buf_len > size_needed));
             xx++) {
          p_app_set[xx].attr_id = *p++;
          p_app_set[xx].attr_val = *p++;
          if (!avrc_is_valid_player_attrib_value(p_app_set[xx].attr_id,
                                                 p_app_set[xx].attr_val))
            status = AVRC_STS_BAD_PARAM;
        }
        if (xx != p_result->set_app_val.num_val) {
          AVRC_TRACE_ERROR(
              "%s AVRC_PDU_SET_PLAYER_APP_VALUE not enough room:%d orig "
              "num_val:%d",
              __func__, xx, p_result->set_app_val.num_val);
          p_result->set_app_val.num_val = xx;
        }
      } else {
        AVRC_TRACE_ERROR(
            "%s AVRC_PDU_SET_PLAYER_APP_VALUE NULL decode buffer or bad len",
            __func__);
        status = AVRC_STS_INTERNAL_ERR;
      }
      break;

    case AVRC_PDU_GET_PLAYER_APP_VALUE_TEXT: /* 0x16 */
      if (len < 3)
        return AVRC_STS_INTERNAL_ERR;
      else {
        BE_STREAM_TO_UINT8(p_result->get_app_val_txt.attr_id, p);
        if (!AVRC_IS_VALID_ATTRIBUTE(p_result->get_app_val_txt.attr_id))
          status = AVRC_STS_BAD_PARAM;
        else {
          BE_STREAM_TO_UINT8(p_result->get_app_val_txt.num_val, p);
          if ((len - 2 /* attr_id & num_val */) !=
              p_result->get_app_val_txt.num_val)
            status = AVRC_STS_INTERNAL_ERR;
          else {
            if (p_result->get_app_val_txt.num_val > AVRC_MAX_APP_ATTR_SIZE) {
              android_errorWriteLog(0x534e4554, "63146237");
              p_result->get_app_val_txt.num_val = AVRC_MAX_APP_ATTR_SIZE;
            }

            p_u8 = p_result->get_app_val_txt.vals;
            for (xx = 0; xx < p_result->get_app_val_txt.num_val; xx++) {
              p_u8[xx] = *p++;
              if (!avrc_is_valid_player_attrib_value(
                      p_result->get_app_val_txt.attr_id, p_u8[xx])) {
                status = AVRC_STS_BAD_PARAM;
                break;
              }
            }
          }
        }
      }
      break;

    case AVRC_PDU_INFORM_DISPLAY_CHARSET: /* 0x17 */
      if (len < 3)
        return AVRC_STS_INTERNAL_ERR;
      else {
        BE_STREAM_TO_UINT8(p_result->inform_charset.num_id, p);
        if ((len - 1 /* num_id */) != p_result->inform_charset.num_id * 2)
          status = AVRC_STS_INTERNAL_ERR;
        else {
          p_u16 = p_result->inform_charset.charsets;
          if (p_result->inform_charset.num_id > AVRC_MAX_CHARSET_SIZE)
            p_result->inform_charset.num_id = AVRC_MAX_CHARSET_SIZE;
          for (xx = 0; xx < p_result->inform_charset.num_id; xx++) {
            BE_STREAM_TO_UINT16(p_u16[xx], p);
          }
        }
      }
      break;

    case AVRC_PDU_INFORM_BATTERY_STAT_OF_CT: /* 0x18 */
      if (len != 1)
        return AVRC_STS_INTERNAL_ERR;
      else {
        p_result->inform_battery_status.battery_status = *p++;
        if (!AVRC_IS_VALID_BATTERY_STATUS(
                p_result->inform_battery_status.battery_status))
          status = AVRC_STS_BAD_PARAM;
      }
      break;

    case AVRC_PDU_GET_ELEMENT_ATTR: /* 0x20 */
      if (len < 9)                  /* UID/8 and num_attr/1 */
        return AVRC_STS_INTERNAL_ERR;
      else {
        BE_STREAM_TO_UINT32(u32, p);
        BE_STREAM_TO_UINT32(u32_2, p);
        if (u32 == 0 && u32_2 == 0) {
          BE_STREAM_TO_UINT8(p_result->get_elem_attrs.num_attr, p);
          if ((len - 9 /* UID/8 and num_attr/1 */) !=
              (p_result->get_elem_attrs.num_attr * 4))
            status = AVRC_STS_INTERNAL_ERR;
          else {
            p_u32 = p_result->get_elem_attrs.attrs;
            if (p_result->get_elem_attrs.num_attr > AVRC_MAX_ELEM_ATTR_SIZE)
              p_result->get_elem_attrs.num_attr = AVRC_MAX_ELEM_ATTR_SIZE;
            for (xx = 0; xx < p_result->get_elem_attrs.num_attr; xx++) {
              BE_STREAM_TO_UINT32(p_u32[xx], p);
            }
          }
        } else
          status = AVRC_STS_NOT_FOUND;
      }
      break;

    case AVRC_PDU_GET_PLAY_STATUS: /* 0x30 */
      /* no additional parameters */
      if (len != 0) return AVRC_STS_INTERNAL_ERR;
      break;

    case AVRC_PDU_REGISTER_NOTIFICATION: /* 0x31 */
      if (len != 5)
        return AVRC_STS_INTERNAL_ERR;
      else {
        BE_STREAM_TO_UINT8(p_result->reg_notif.event_id, p);
        BE_STREAM_TO_UINT32(p_result->reg_notif.param, p);
      }
      break;

    case AVRC_PDU_SET_ABSOLUTE_VOLUME: /* 0x50 */
      if (len != 1)
        return AVRC_STS_INTERNAL_ERR;
      else
        p_result->volume.volume = *p++;
      break;

    case AVRC_PDU_REQUEST_CONTINUATION_RSP: /* 0x40 */
      if (len != 1) {
        return AVRC_STS_INTERNAL_ERR;
      }
      BE_STREAM_TO_UINT8(p_result->continu.target_pdu, p);
      break;

    case AVRC_PDU_ABORT_CONTINUATION_RSP: /* 0x41 */
      if (len != 1) {
        return AVRC_STS_INTERNAL_ERR;
      }
      BE_STREAM_TO_UINT8(p_result->abort.target_pdu, p);
      break;

    case AVRC_PDU_SET_ADDRESSED_PLAYER: /* 0x60 */
      if (len != 2) {
        AVRC_TRACE_ERROR("AVRC_PDU_SET_ADDRESSED_PLAYER length is incorrect:%d",
                         len);
        return AVRC_STS_INTERNAL_ERR;
      }
      BE_STREAM_TO_UINT16(p_result->addr_player.player_id, p);
      break;

    case AVRC_PDU_PLAY_ITEM:          /* 0x74 */
    case AVRC_PDU_ADD_TO_NOW_PLAYING: /* 0x90 */
      if (len != (AVRC_UID_SIZE + 3)) return AVRC_STS_INTERNAL_ERR;
      BE_STREAM_TO_UINT8(p_result->play_item.scope, p);
      if (p_result->play_item.scope > AVRC_SCOPE_NOW_PLAYING) {
        status = AVRC_STS_BAD_SCOPE;
      }
      BE_STREAM_TO_ARRAY(p, p_result->play_item.uid, AVRC_UID_SIZE);
      BE_STREAM_TO_UINT16(p_result->play_item.uid_counter, p);
      break;

    default:
      status = AVRC_STS_BAD_CMD;
      break;
  }

  return status;
}

/*******************************************************************************
 *
 * Function         AVRC_Ctrl_ParsCommand
 *
 * Description      This function is used to parse cmds received for CTRL
 *                  Currently it is for SetAbsVolume and Volume Change
 *                  Notification..
 *
 * Returns          AVRC_STS_NO_ERROR, if the message in p_data is parsed
 *                  successfully.
 *                  Otherwise, the error code defined by AVRCP 1.4
 *
 ******************************************************************************/
tAVRC_STS AVRC_Ctrl_ParsCommand(tAVRC_MSG* p_msg, tAVRC_COMMAND* p_result) {
  tAVRC_STS status = AVRC_STS_INTERNAL_ERR;

  if (p_msg && p_result) {
    switch (p_msg->hdr.opcode) {
      case AVRC_OP_VENDOR: /*  0x00    Vendor-dependent commands */
        status = avrc_ctrl_pars_vendor_cmd(&p_msg->vendor, p_result);
        break;

      default:
        AVRC_TRACE_ERROR("%s unknown opcode:0x%x", __func__, p_msg->hdr.opcode);
        break;
    }
    p_result->cmd.opcode = p_msg->hdr.opcode;
    p_result->cmd.status = status;
  }
  AVRC_TRACE_DEBUG("%s return status:0x%x", __func__, status);
  return status;
}

/*******************************************************************************
 *
 * Function         avrc_pars_browsing_cmd
 *
 * Description      This function parses the commands that go through the
 *                  browsing channel
 *
 * Returns          AVRC_STS_NO_ERROR, if the message in p_data is parsed
 *                  successfully.
 *                  Otherwise, the error code defined by AVRCP+1
 *
 ******************************************************************************/
static tAVRC_STS avrc_pars_browsing_cmd(tAVRC_MSG_BROWSE* p_msg,
                                        tAVRC_COMMAND* p_result, uint8_t* p_buf,
                                        uint16_t buf_len) {
  tAVRC_STS status = AVRC_STS_NO_ERROR;
  uint8_t* p = p_msg->p_browse_data;
  int count;

  p_result->pdu = *p++;
  AVRC_TRACE_DEBUG("avrc_pars_browsing_cmd() pdu:0x%x", p_result->pdu);
  /* skip over len */
  p += 2;

  switch (p_result->pdu) {
    case AVRC_PDU_SET_BROWSED_PLAYER: /* 0x70 */
      // For current implementation all players are browsable.
      BE_STREAM_TO_UINT16(p_result->br_player.player_id, p);
      break;

    case AVRC_PDU_GET_FOLDER_ITEMS: /* 0x71 */
      STREAM_TO_UINT8(p_result->get_items.scope, p);
      // To be modified later here (Scope) when all browsing commands are
      // supported
      if (p_result->get_items.scope > AVRC_SCOPE_NOW_PLAYING) {
        status = AVRC_STS_BAD_SCOPE;
      }
      BE_STREAM_TO_UINT32(p_result->get_items.start_item, p);
      BE_STREAM_TO_UINT32(p_result->get_items.end_item, p);
      if (p_result->get_items.start_item > p_result->get_items.end_item) {
        status = AVRC_STS_BAD_RANGE;
      }
      STREAM_TO_UINT8(p_result->get_items.attr_count, p);
      p_result->get_items.p_attr_list = NULL;
      if (p_result->get_items.attr_count && p_buf &&
          (p_result->get_items.attr_count != AVRC_FOLDER_ITEM_COUNT_NONE)) {
        p_result->get_items.p_attr_list = (uint32_t*)p_buf;
        count = p_result->get_items.attr_count;
        if (buf_len < (count << 2))
          p_result->get_items.attr_count = count = (buf_len >> 2);
        for (int idx = 0; idx < count; idx++) {
          BE_STREAM_TO_UINT32(p_result->get_items.p_attr_list[idx], p);
        }
      }
      break;

    case AVRC_PDU_CHANGE_PATH: /* 0x72 */
      BE_STREAM_TO_UINT16(p_result->chg_path.uid_counter, p);
      BE_STREAM_TO_UINT8(p_result->chg_path.direction, p);
      if (p_result->chg_path.direction != AVRC_DIR_UP &&
          p_result->chg_path.direction != AVRC_DIR_DOWN) {
        status = AVRC_STS_BAD_DIR;
      }
      BE_STREAM_TO_ARRAY(p, p_result->chg_path.folder_uid, AVRC_UID_SIZE);
      break;

    case AVRC_PDU_GET_ITEM_ATTRIBUTES: /* 0x73 */
      BE_STREAM_TO_UINT8(p_result->get_attrs.scope, p);
      if (p_result->get_attrs.scope > AVRC_SCOPE_NOW_PLAYING) {
        status = AVRC_STS_BAD_SCOPE;
        break;
      }
      BE_STREAM_TO_ARRAY(p, p_result->get_attrs.uid, AVRC_UID_SIZE);
      BE_STREAM_TO_UINT16(p_result->get_attrs.uid_counter, p);
      BE_STREAM_TO_UINT8(p_result->get_attrs.attr_count, p);
      p_result->get_attrs.p_attr_list = NULL;
      if (p_result->get_attrs.attr_count && p_buf) {
        p_result->get_attrs.p_attr_list = (uint32_t*)p_buf;
        count = p_result->get_attrs.attr_count;
        if (buf_len < (count << 2))
          p_result->get_attrs.attr_count = count = (buf_len >> 2);
        for (int idx = 0, count = 0; idx < p_result->get_attrs.attr_count;
             idx++) {
          BE_STREAM_TO_UINT32(p_result->get_attrs.p_attr_list[count], p);
          if (AVRC_IS_VALID_MEDIA_ATTRIBUTE(
                  p_result->get_attrs.p_attr_list[count])) {
            count++;
          }
        }

        if (p_result->get_attrs.attr_count != count && count == 0)
          status = AVRC_STS_BAD_PARAM;
        else
          p_result->get_attrs.attr_count = count;
      }
      break;

    case AVRC_PDU_GET_TOTAL_NUM_OF_ITEMS: /* 0x75 */
      BE_STREAM_TO_UINT8(p_result->get_num_of_items.scope, p);
      if (p_result->get_num_of_items.scope > AVRC_SCOPE_NOW_PLAYING) {
        status = AVRC_STS_BAD_SCOPE;
      }
      break;

    case AVRC_PDU_SEARCH: /* 0x80 */
      BE_STREAM_TO_UINT16(p_result->search.string.charset_id, p);
      BE_STREAM_TO_UINT16(p_result->search.string.str_len, p);
      p_result->search.string.p_str = p_buf;
      if (p_buf) {
        if (p_result->search.string.str_len > buf_len) {
          p_result->search.string.str_len = buf_len;
        } else {
          android_errorWriteLog(0x534e4554, "63146237");
        }
        BE_STREAM_TO_ARRAY(p, p_buf, p_result->search.string.str_len);
      } else {
        status = AVRC_STS_INTERNAL_ERR;
      }
      break;

    default:
      status = AVRC_STS_BAD_CMD;
      break;
  }
  return status;
}

/*******************************************************************************
 *
 * Function         AVRC_ParsCommand
 *
 * Description      This function is a superset of AVRC_ParsMetadata to parse
 *                  the command.
 *
 * Returns          AVRC_STS_NO_ERROR, if the message in p_data is parsed
 *                  successfully.
 *                  Otherwise, the error code defined by AVRCP 1.4
 *
 ******************************************************************************/
tAVRC_STS AVRC_ParsCommand(tAVRC_MSG* p_msg, tAVRC_COMMAND* p_result,
                           uint8_t* p_buf, uint16_t buf_len) {
  tAVRC_STS status = AVRC_STS_INTERNAL_ERR;
  uint16_t id;

  if (p_msg && p_result) {
    switch (p_msg->hdr.opcode) {
      case AVRC_OP_VENDOR: /*  0x00    Vendor-dependent commands */
        status = avrc_pars_vendor_cmd(&p_msg->vendor, p_result, p_buf, buf_len);
        break;

      case AVRC_OP_PASS_THRU: /*  0x7C    panel subunit opcode */
        status = avrc_pars_pass_thru(&p_msg->pass, &id);
        if (status == AVRC_STS_NO_ERROR) {
          p_result->pdu = (uint8_t)id;
        }
        break;

      case AVRC_OP_BROWSE:
        status =
            avrc_pars_browsing_cmd(&p_msg->browse, p_result, p_buf, buf_len);
        break;

      default:
        AVRC_TRACE_ERROR("%s unknown opcode:0x%x", __func__, p_msg->hdr.opcode);
        break;
    }
    p_result->cmd.opcode = p_msg->hdr.opcode;
    p_result->cmd.status = status;
  }
  AVRC_TRACE_DEBUG("%s return status:0x%x", __func__, status);
  return status;
}
