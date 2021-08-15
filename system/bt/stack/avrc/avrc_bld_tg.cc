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
#include <base/logging.h>
#include <string.h>

#include "avrc_api.h"
#include "avrc_defs.h"
#include "avrc_int.h"
#include "bt_common.h"
#include "bt_utils.h"
#include "osi/include/osi.h"

/*****************************************************************************
 *  Global data
 ****************************************************************************/
#define AVRC_ITEM_PLAYER_IS_VALID(_p_player)                 \
  ((_p_player)->name.p_str &&                                \
   ((_p_player)->major_type & AVRC_MJ_TYPE_INVALID) == 0 &&  \
   ((_p_player)->sub_type & AVRC_SUB_TYPE_INVALID) == 0 &&   \
   (((_p_player)->play_status <= AVRC_PLAYSTATE_REV_SEEK) || \
    ((_p_player)->play_status == AVRC_PLAYSTATE_ERROR)))

/* 17 = item_type(1) + item len(2) + min item (14) */
#define AVRC_MIN_LEN_GET_FOLDER_ITEMS_RSP 17

/*******************************************************************************
 *
 * Function         avrc_bld_get_capability_rsp
 *
 * Description      This function builds the Get Capability response.
 *
 * Returns          AVRC_STS_NO_ERROR, if the response is built successfully
 *                  Otherwise, the error code.
 *
 ******************************************************************************/
static tAVRC_STS avrc_bld_get_capability_rsp(tAVRC_GET_CAPS_RSP* p_rsp,
                                             BT_HDR* p_pkt) {
  uint8_t *p_data, *p_start, *p_len, *p_count;
  uint16_t len = 0;
  uint8_t xx;
  uint32_t* p_company_id;
  uint8_t* p_event_id;
  tAVRC_STS status = AVRC_STS_NO_ERROR;

  if (!(AVRC_IS_VALID_CAP_ID(p_rsp->capability_id))) {
    AVRC_TRACE_ERROR("%s bad parameter. p_rsp: %x", __func__, p_rsp);
    status = AVRC_STS_BAD_PARAM;
    return status;
  }

  AVRC_TRACE_API("%s", __func__);
  /* get the existing length, if any, and also the num attributes */
  p_start = (uint8_t*)(p_pkt + 1) + p_pkt->offset;
  p_data = p_len = p_start + 2; /* pdu + rsvd */

  BE_STREAM_TO_UINT16(len, p_data);
  UINT8_TO_BE_STREAM(p_data, p_rsp->capability_id);
  p_count = p_data;

  if (len == 0) {
    *p_count = p_rsp->count;
    p_data++;
    len = 2; /* move past the capability_id and count */
  } else {
    p_data = p_start + p_pkt->len;
    *p_count += p_rsp->count;
  }

  if (p_rsp->capability_id == AVRC_CAP_COMPANY_ID) {
    p_company_id = p_rsp->param.company_id;
    for (xx = 0; xx < p_rsp->count; xx++) {
      UINT24_TO_BE_STREAM(p_data, p_company_id[xx]);
    }
    len += p_rsp->count * 3;
  } else {
    p_event_id = p_rsp->param.event_id;
    *p_count = 0;
    for (xx = 0; xx < p_rsp->count; xx++) {
      if (AVRC_IS_VALID_EVENT_ID(p_event_id[xx])) {
        (*p_count)++;
        UINT8_TO_BE_STREAM(p_data, p_event_id[xx]);
      }
    }
    len += (*p_count);
  }
  UINT16_TO_BE_STREAM(p_len, len);
  p_pkt->len = (p_data - p_start);
  status = AVRC_STS_NO_ERROR;

  return status;
}

/*******************************************************************************
 *
 * Function         avrc_bld_list_app_settings_attr_rsp
 *
 * Description      This function builds the List Application Settings Attribute
 *                  response.
 *
 * Returns          AVRC_STS_NO_ERROR, if the response is built successfully
 *                  Otherwise, the error code.
 *
 ******************************************************************************/
static tAVRC_STS avrc_bld_list_app_settings_attr_rsp(
    tAVRC_LIST_APP_ATTR_RSP* p_rsp, BT_HDR* p_pkt) {
  uint8_t *p_data, *p_start, *p_len, *p_num;
  uint16_t len = 0;
  uint8_t xx;

  AVRC_TRACE_API("%s", __func__);
  /* get the existing length, if any, and also the num attributes */
  p_start = (uint8_t*)(p_pkt + 1) + p_pkt->offset;
  p_data = p_len = p_start + 2; /* pdu + rsvd */

  BE_STREAM_TO_UINT16(len, p_data);
  p_num = p_data;
  if (len == 0) {
    /* first time initialize the attribute count */
    *p_num = 0;
    p_data++;
  } else {
    p_data = p_start + p_pkt->len;
  }

  for (xx = 0; xx < p_rsp->num_attr; xx++) {
    if (AVRC_IsValidPlayerAttr(p_rsp->attrs[xx])) {
      (*p_num)++;
      UINT8_TO_BE_STREAM(p_data, p_rsp->attrs[xx]);
    }
  }

  len = *p_num + 1;
  UINT16_TO_BE_STREAM(p_len, len);
  p_pkt->len = (p_data - p_start);

  return AVRC_STS_NO_ERROR;
}

/*******************************************************************************
 *
 * Function         avrc_bld_list_app_settings_values_rsp
 *
 * Description      This function builds the List Application Setting Values
 *                  response.
 *
 * Returns          AVRC_STS_NO_ERROR, if the response is built successfully
 *                  Otherwise, the error code.
 *
 ******************************************************************************/
static tAVRC_STS avrc_bld_list_app_settings_values_rsp(
    tAVRC_LIST_APP_VALUES_RSP* p_rsp, BT_HDR* p_pkt) {
  uint8_t *p_data, *p_start, *p_len, *p_num;
  uint8_t xx;
  uint16_t len;

  AVRC_TRACE_API("%s", __func__);

  p_start = (uint8_t*)(p_pkt + 1) + p_pkt->offset;
  p_data = p_len = p_start + 2; /* pdu + rsvd */

  /* get the existing length, if any, and also the num attributes */
  BE_STREAM_TO_UINT16(len, p_data);
  p_num = p_data;
  /* first time initialize the attribute count */
  if (len == 0) {
    *p_num = p_rsp->num_val;
    p_data++;
  } else {
    p_data = p_start + p_pkt->len;
    *p_num += p_rsp->num_val;
  }

  for (xx = 0; xx < p_rsp->num_val; xx++) {
    UINT8_TO_BE_STREAM(p_data, p_rsp->vals[xx]);
  }

  len = *p_num + 1;
  UINT16_TO_BE_STREAM(p_len, len);
  p_pkt->len = (p_data - p_start);
  return AVRC_STS_NO_ERROR;
}

/*******************************************************************************
 *
 * Function         avrc_bld_get_cur_app_setting_value_rsp
 *
 * Description      This function builds the Get Current Application Setting
 *                  Value response.
 *
 * Returns          AVRC_STS_NO_ERROR, if the response is built successfully
 *                  Otherwise, the error code.
 *
 ******************************************************************************/
static tAVRC_STS avrc_bld_get_cur_app_setting_value_rsp(
    tAVRC_GET_CUR_APP_VALUE_RSP* p_rsp, BT_HDR* p_pkt) {
  uint8_t *p_data, *p_start, *p_len, *p_count;
  uint16_t len;
  uint8_t xx;

  if (!p_rsp->p_vals) {
    AVRC_TRACE_ERROR("%s NULL parameter", __func__);
    return AVRC_STS_BAD_PARAM;
  }

  AVRC_TRACE_API("%s", __func__);
  /* get the existing length, if any, and also the num attributes */
  p_start = (uint8_t*)(p_pkt + 1) + p_pkt->offset;
  p_data = p_len = p_start + 2; /* pdu + rsvd */

  BE_STREAM_TO_UINT16(len, p_data);
  p_count = p_data;
  if (len == 0) {
    /* first time initialize the attribute count */
    *p_count = 0;
    p_data++;
  } else {
    p_data = p_start + p_pkt->len;
  }

  for (xx = 0; xx < p_rsp->num_val; xx++) {
    if (avrc_is_valid_player_attrib_value(p_rsp->p_vals[xx].attr_id,
                                          p_rsp->p_vals[xx].attr_val)) {
      (*p_count)++;
      UINT8_TO_BE_STREAM(p_data, p_rsp->p_vals[xx].attr_id);
      UINT8_TO_BE_STREAM(p_data, p_rsp->p_vals[xx].attr_val);
    }
  }
  len = ((*p_count) << 1) + 1;
  UINT16_TO_BE_STREAM(p_len, len);
  p_pkt->len = (p_data - p_start);

  return AVRC_STS_NO_ERROR;
}

/*******************************************************************************
 *
 * Function         avrc_bld_set_app_setting_value_rsp
 *
 * Description      This function builds the Set Application Setting Value
 *                  response.
 *
 * Returns          AVRC_STS_NO_ERROR, if the response is built successfully
 *                  Otherwise, the error code.
 *
 ******************************************************************************/
static tAVRC_STS avrc_bld_set_app_setting_value_rsp(
    UNUSED_ATTR tAVRC_RSP* p_rsp, UNUSED_ATTR BT_HDR* p_pkt) {
  /* nothing to be added. */
  AVRC_TRACE_API("%s", __func__);
  return AVRC_STS_NO_ERROR;
}

/*******************************************************************************
 *
 * Function         avrc_bld_app_setting_text_rsp
 *
 * Description      This function builds the Get Application Settings Attribute
 *                  Text or Get Application Settings Value Text response.
 *
 * Returns          AVRC_STS_NO_ERROR, if the response is built successfully
 *                  Otherwise, the error code.
 *
 ******************************************************************************/
static tAVRC_STS avrc_bld_app_setting_text_rsp(
    tAVRC_GET_APP_ATTR_TXT_RSP* p_rsp, BT_HDR* p_pkt) {
  uint8_t *p_data, *p_start, *p_len, *p_count;
  uint16_t len, len_left;
  uint8_t xx;
  tAVRC_STS sts = AVRC_STS_NO_ERROR;
  uint8_t num_added = 0;

  if (!p_rsp->p_attrs) {
    AVRC_TRACE_ERROR("%s NULL parameter", __func__);
    return AVRC_STS_BAD_PARAM;
  }
  /* get the existing length, if any, and also the num attributes */
  p_start = (uint8_t*)(p_pkt + 1) + p_pkt->offset;
  p_data = p_len = p_start + 2; /* pdu + rsvd */

  /*
   * NOTE: The buffer is allocated within avrc_bld_init_rsp_buffer(), and is
   * always of size BT_DEFAULT_BUFFER_SIZE.
   */
  len_left = BT_DEFAULT_BUFFER_SIZE - BT_HDR_SIZE - p_pkt->offset - p_pkt->len;

  BE_STREAM_TO_UINT16(len, p_data);
  p_count = p_data;

  if (len == 0) {
    *p_count = 0;
    p_data++;
  } else {
    p_data = p_start + p_pkt->len;
  }

  for (xx = 0; xx < p_rsp->num_attr; xx++) {
    if (len_left < (p_rsp->p_attrs[xx].str_len + 4)) {
      AVRC_TRACE_ERROR("%s out of room (str_len:%d, left:%d)", __func__, xx,
                       p_rsp->p_attrs[xx].str_len, len_left);
      p_rsp->num_attr = num_added;
      sts = AVRC_STS_INTERNAL_ERR;
      break;
    }
    if (!p_rsp->p_attrs[xx].str_len || !p_rsp->p_attrs[xx].p_str) {
      AVRC_TRACE_ERROR("%s NULL attr text[%d]", __func__, xx);
      continue;
    }
    UINT8_TO_BE_STREAM(p_data, p_rsp->p_attrs[xx].attr_id);
    UINT16_TO_BE_STREAM(p_data, p_rsp->p_attrs[xx].charset_id);
    UINT8_TO_BE_STREAM(p_data, p_rsp->p_attrs[xx].str_len);
    ARRAY_TO_BE_STREAM(p_data, p_rsp->p_attrs[xx].p_str,
                       p_rsp->p_attrs[xx].str_len);
    (*p_count)++;
    num_added++;
  }
  len = p_data - p_count;
  UINT16_TO_BE_STREAM(p_len, len);
  p_pkt->len = (p_data - p_start);

  return sts;
}

/*******************************************************************************
 *
 * Function         avrc_bld_get_app_setting_attr_text_rsp
 *
 * Description      This function builds the Get Application Setting Attribute
 *                  Text response.
 *
 * Returns          AVRC_STS_NO_ERROR, if the response is built successfully
 *                  Otherwise, the error code.
 *
 ******************************************************************************/
static tAVRC_STS avrc_bld_get_app_setting_attr_text_rsp(
    tAVRC_GET_APP_ATTR_TXT_RSP* p_rsp, BT_HDR* p_pkt) {
  AVRC_TRACE_API("%s", __func__);
  return avrc_bld_app_setting_text_rsp(p_rsp, p_pkt);
}

/*******************************************************************************
 *
 * Function         avrc_bld_get_app_setting_value_text_rsp
 *
 * Description      This function builds the Get Application Setting Value Text
 *                  response.
 *
 * Returns          AVRC_STS_NO_ERROR, if the response is built successfully
 *                  Otherwise, the error code.
 *
 ******************************************************************************/
static tAVRC_STS avrc_bld_get_app_setting_value_text_rsp(
    tAVRC_GET_APP_ATTR_TXT_RSP* p_rsp, BT_HDR* p_pkt) {
  AVRC_TRACE_API("%s", __func__);
  return avrc_bld_app_setting_text_rsp(p_rsp, p_pkt);
}

/*******************************************************************************
 *
 * Function         avrc_bld_inform_charset_rsp
 *
 * Description      This function builds the Inform Displayable Character Set
 *                  response.
 *
 * Returns          AVRC_STS_NO_ERROR, if the response is built successfully
 *                  Otherwise, the error code.
 *
 ******************************************************************************/
static tAVRC_STS avrc_bld_inform_charset_rsp(UNUSED_ATTR tAVRC_RSP* p_rsp,
                                             UNUSED_ATTR BT_HDR* p_pkt) {
  /* nothing to be added. */
  AVRC_TRACE_API("%s", __func__);
  return AVRC_STS_NO_ERROR;
}

/*******************************************************************************
 *
 * Function         avrc_bld_inform_battery_status_rsp
 *
 * Description      This function builds the Inform Battery Status
 *                  response.
 *
 * Returns          AVRC_STS_NO_ERROR, if the response is built successfully
 *                  Otherwise, the error code.
 *
 ******************************************************************************/
static tAVRC_STS avrc_bld_inform_battery_status_rsp(
    UNUSED_ATTR tAVRC_RSP* p_rsp, UNUSED_ATTR BT_HDR* p_pkt) {
  /* nothing to be added. */
  AVRC_TRACE_API("%s", __func__);
  return AVRC_STS_NO_ERROR;
}

static void avrc_build_attribute_entries(int num_attrs,
                                         tAVRC_ATTR_ENTRY* p_attrs,
                                         int remaining_buffer_capacity,
                                         uint8_t** pp_data,
                                         uint8_t* p_attribute_count) {
  AVRC_TRACE_DEBUG("%s num_attrs: %d, remaining_buffer_capacity: %d", __func__,
                   num_attrs, remaining_buffer_capacity);
  uint8_t* p_data = *pp_data;
  /* Fill in the Attribute ID, Character Set, Length and Values */
  for (int index = 0; index < num_attrs; index++) {
    AVRC_TRACE_DEBUG("%s attr id[%d]: %d", __func__, index,
                     p_attrs[index].attr_id);
    CHECK(AVRC_IS_VALID_MEDIA_ATTRIBUTE(p_attrs[index].attr_id));
    if (!p_attrs[index].name.p_str) {
      p_attrs[index].name.str_len = 0;
    }
    /* 8 is the size of attr_id, char set and str_len */
    remaining_buffer_capacity -= 8;
    if (remaining_buffer_capacity < 0) {
      AVRC_TRACE_WARNING(
          "%s not enough buffer space for attr_id[%d]: %d,"
          " skipping %d attributes",
          __func__, index, p_attrs[index].attr_id, num_attrs - index);
      break;
    }
    if (remaining_buffer_capacity < p_attrs[index].name.str_len) {
      AVRC_TRACE_WARNING(
          "%s not enough buffer space for attr_id[%d]: %d,"
          " truncating attribute",
          __func__, index, p_attrs[index].attr_id);
      p_attrs[index].name.str_len = remaining_buffer_capacity;
      remaining_buffer_capacity = 0;
    }
    remaining_buffer_capacity -= p_attrs[index].name.str_len;
    UINT32_TO_BE_STREAM(p_data, p_attrs[index].attr_id);
    UINT16_TO_BE_STREAM(p_data, p_attrs[index].name.charset_id);
    UINT16_TO_BE_STREAM(p_data, p_attrs[index].name.str_len);
    ARRAY_TO_BE_STREAM(p_data, p_attrs[index].name.p_str,
                       p_attrs[index].name.str_len);
    (*p_attribute_count)++;
  }
  *pp_data = p_data;
  AVRC_TRACE_DEBUG("%s filled attributes, remaining_buffer_capacity: %d",
                   __func__, num_attrs, remaining_buffer_capacity);
}

/*******************************************************************************
 *
 * Function         avrc_bld_get_elem_attrs_rsp
 *
 * Description      This function builds the Get Element Attributes
 *                  response.
 *
 * Returns          AVRC_STS_NO_ERROR, if the response is built successfully
 *                  Otherwise, the error code.
 *
 ******************************************************************************/
static tAVRC_STS avrc_bld_get_elem_attrs_rsp(tAVRC_GET_ATTRS_RSP* p_rsp,
                                             BT_HDR* p_pkt) {
  AVRC_TRACE_API("%s", __func__);
  if (!p_rsp->p_attrs) {
    AVRC_TRACE_ERROR("%s NULL p_attrs", __func__);
    return AVRC_STS_BAD_PARAM;
  }
  /* Figure out how much we have left in current buffer */
  int remaining_buffer_capacity =
      BT_DEFAULT_BUFFER_SIZE - BT_HDR_SIZE - p_pkt->offset;
  if (remaining_buffer_capacity < 5) {
    AVRC_TRACE_ERROR("%s not enough buffer for packet header",
                     remaining_buffer_capacity);
    return AVRC_STS_INTERNAL_ERR;
  }
  /* Get to the beginning of PDU */
  uint8_t* p_pdu_start = (uint8_t*)(p_pkt + 1) + p_pkt->offset;
  /* Skip PDU ID and Reserved byte to get pointer to Parameter Length */
  uint8_t *p_data, *p_parameter_len;
  p_data = p_parameter_len = p_pdu_start + 2;
  /* Parse parameter length */
  uint16_t parameter_len;
  BE_STREAM_TO_UINT16(parameter_len, p_data);
  /* Get pointer to Attribute Count */
  uint8_t* p_attribute_count = p_data;
  /* Initialize field values when Parameter Length is 0 */
  if (parameter_len == 0) {
    *p_attribute_count = 0;
    p_data++;
  } else {
    // TODO: Why do we need this case?
    p_data = p_pdu_start + p_pkt->len;
  }
  remaining_buffer_capacity -= p_data - p_pdu_start;
  ;
  if (remaining_buffer_capacity < 0) {
    AVRC_TRACE_ERROR("%s not enough buffer capacity for response");
    return AVRC_STS_BAD_PARAM;
  }
  /* Fill in the Attribute ID, Character Set, Length and Values */
  avrc_build_attribute_entries(p_rsp->num_attrs, p_rsp->p_attrs,
                               remaining_buffer_capacity, &p_data,
                               p_attribute_count);
  parameter_len = p_data - p_attribute_count;
  UINT16_TO_BE_STREAM(p_parameter_len, parameter_len);
  p_pkt->len = (p_data - p_pdu_start);
  return AVRC_STS_NO_ERROR;
}

/*******************************************************************************
 *
 * Function         avrc_bld_get_play_status_rsp
 *
 * Description      This function builds the Get Play Status
 *                  response.
 *
 * Returns          AVRC_STS_NO_ERROR, if the response is built successfully
 *                  Otherwise, the error code.
 *
 ******************************************************************************/
static tAVRC_STS avrc_bld_get_play_status_rsp(tAVRC_GET_PLAY_STATUS_RSP* p_rsp,
                                              BT_HDR* p_pkt) {
  uint8_t *p_data, *p_start;

  AVRC_TRACE_API("%s", __func__);
  p_start = (uint8_t*)(p_pkt + 1) + p_pkt->offset;
  p_data = p_start + 2;

  /* add fixed lenth - song len(4) + song position(4) + status(1) */
  UINT16_TO_BE_STREAM(p_data, 9);
  UINT32_TO_BE_STREAM(p_data, p_rsp->song_len);
  UINT32_TO_BE_STREAM(p_data, p_rsp->song_pos);
  UINT8_TO_BE_STREAM(p_data, p_rsp->play_status);
  p_pkt->len = (p_data - p_start);

  return AVRC_STS_NO_ERROR;
}

/*******************************************************************************
 *
 * Function         avrc_bld_notify_rsp
 *
 * Description      This function builds the Notification response.
 *
 * Returns          AVRC_STS_NO_ERROR, if the response is built successfully
 *                  Otherwise, the error code.
 *
 ******************************************************************************/
static tAVRC_STS avrc_bld_notify_rsp(tAVRC_REG_NOTIF_RSP* p_rsp,
                                     BT_HDR* p_pkt) {
  uint8_t *p_data, *p_start;
  uint8_t* p_len;
  uint16_t len = 0;
  uint8_t xx;
  tAVRC_STS status = AVRC_STS_NO_ERROR;

  AVRC_TRACE_API("%s event_id %d", __func__, p_rsp->event_id);

  p_start = (uint8_t*)(p_pkt + 1) + p_pkt->offset;
  p_data = p_len = p_start + 2; /* pdu + rsvd */
  p_data += 2;

  UINT8_TO_BE_STREAM(p_data, p_rsp->event_id);
  switch (p_rsp->event_id) {
    case AVRC_EVT_PLAY_STATUS_CHANGE: /* 0x01 */
      /* p_rsp->param.play_status >= AVRC_PLAYSTATE_STOPPED is always true */
      if ((p_rsp->param.play_status <= AVRC_PLAYSTATE_REV_SEEK) ||
          (p_rsp->param.play_status == AVRC_PLAYSTATE_ERROR)) {
        UINT8_TO_BE_STREAM(p_data, p_rsp->param.play_status);
        len = 2;
      } else {
        AVRC_TRACE_ERROR("%s bad play state", __func__);
        status = AVRC_STS_BAD_PARAM;
      }
      break;

    case AVRC_EVT_TRACK_CHANGE: /* 0x02 */
      ARRAY_TO_BE_STREAM(p_data, p_rsp->param.track, AVRC_UID_SIZE);
      len = (uint8_t)(AVRC_UID_SIZE + 1);
      break;

    case AVRC_EVT_TRACK_REACHED_END:   /* 0x03 */
    case AVRC_EVT_TRACK_REACHED_START: /* 0x04 */
    case AVRC_EVT_NOW_PLAYING_CHANGE:  /* 0x09 */
    case AVRC_EVT_AVAL_PLAYERS_CHANGE: /* 0x0a */
      len = 1;
      break;

    case AVRC_EVT_PLAY_POS_CHANGED: /* 0x05 */
      UINT32_TO_BE_STREAM(p_data, p_rsp->param.play_pos);
      len = 5;
      break;

    case AVRC_EVT_BATTERY_STATUS_CHANGE: /* 0x06 */
      if (AVRC_IS_VALID_BATTERY_STATUS(p_rsp->param.battery_status)) {
        UINT8_TO_BE_STREAM(p_data, p_rsp->param.battery_status);
        len = 2;
      } else {
        AVRC_TRACE_ERROR("%s bad battery status", __func__);
        status = AVRC_STS_BAD_PARAM;
      }
      break;

    case AVRC_EVT_SYSTEM_STATUS_CHANGE: /* 0x07 */
      if (AVRC_IS_VALID_SYSTEM_STATUS(p_rsp->param.system_status)) {
        UINT8_TO_BE_STREAM(p_data, p_rsp->param.system_status);
        len = 2;
      } else {
        AVRC_TRACE_ERROR("%s bad system status", __func__);
        status = AVRC_STS_BAD_PARAM;
      }
      break;

    case AVRC_EVT_APP_SETTING_CHANGE: /* 0x08 */
      if (p_rsp->param.player_setting.num_attr > AVRC_MAX_APP_SETTINGS)
        p_rsp->param.player_setting.num_attr = AVRC_MAX_APP_SETTINGS;

      if (p_rsp->param.player_setting.num_attr > 0) {
        UINT8_TO_BE_STREAM(p_data, p_rsp->param.player_setting.num_attr);
        len = 2;
        for (xx = 0; xx < p_rsp->param.player_setting.num_attr; xx++) {
          if (avrc_is_valid_player_attrib_value(
                  p_rsp->param.player_setting.attr_id[xx],
                  p_rsp->param.player_setting.attr_value[xx])) {
            UINT8_TO_BE_STREAM(p_data, p_rsp->param.player_setting.attr_id[xx]);
            UINT8_TO_BE_STREAM(p_data,
                               p_rsp->param.player_setting.attr_value[xx]);
          } else {
            AVRC_TRACE_ERROR("%s bad player app seeting attribute or value",
                             __func__);
            status = AVRC_STS_BAD_PARAM;
            break;
          }
          len += 2;
        }
      } else
        status = AVRC_STS_BAD_PARAM;
      break;

    case AVRC_EVT_VOLUME_CHANGE: /* 0x0d */
      len = 2;
      UINT8_TO_BE_STREAM(p_data, (AVRC_MAX_VOLUME & p_rsp->param.volume));
      break;

    case AVRC_EVT_ADDR_PLAYER_CHANGE: /* 0x0b */
      UINT16_TO_BE_STREAM(p_data, p_rsp->param.addr_player.player_id);
      UINT16_TO_BE_STREAM(p_data, p_rsp->param.addr_player.uid_counter);
      len = 5;
      break;

    case AVRC_EVT_UIDS_CHANGE:                               /* 0x0c */
      UINT16_TO_BE_STREAM(p_data, p_rsp->param.uid_counter); /* uid counter */
      len = 3;
      break;

    default:
      status = AVRC_STS_BAD_PARAM;
      AVRC_TRACE_ERROR("%s unknown event_id", __func__);
  }

  UINT16_TO_BE_STREAM(p_len, len);
  p_pkt->len = (p_data - p_start);

  return status;
}

/*******************************************************************************
 *
 * Function         avrc_bld_next_rsp
 *
 * Description      This function builds the Request Continue or Abort
 *                  response.
 *
 * Returns          AVRC_STS_NO_ERROR, if the response is built successfully
 *                  Otherwise, the error code.
 *
 ******************************************************************************/
static tAVRC_STS avrc_bld_next_rsp(tAVRC_NEXT_RSP* p_rsp, BT_HDR* p_pkt) {
  uint8_t* p_start = (uint8_t*)(p_pkt + 1) + p_pkt->offset;
  uint8_t* p_data = (p_start + 2); /* Skip the pdu and reserved bits */

  UINT16_TO_BE_STREAM(p_data, 0x0001); /* only one attribute to be sent */
  UINT8_TO_BE_STREAM(p_data, p_rsp->target_pdu);

  AVRC_TRACE_API("%s: target_pdu: 0x%02x", __func__, p_rsp->target_pdu);
  return AVRC_STS_NO_ERROR;
}

/*****************************************************************************
 *
 * Function      avrc_bld_set_absolute_volume_rsp
 *
 * Description   This function builds the set absolute volume response
 *
 * Returns       AVRC_STS_NO_ERROR, if the response is build successfully
 *
 *****************************************************************************/
static tAVRC_STS avrc_bld_set_absolute_volume_rsp(uint8_t abs_vol,
                                                  BT_HDR* p_pkt) {
  AVRC_TRACE_API("%s", __func__);
  uint8_t* p_start = (uint8_t*)(p_pkt + 1) + p_pkt->offset;
  /* To calculate length */
  uint8_t* p_data = p_start + 2;
  /* add fixed lenth status(1) */
  UINT16_TO_BE_STREAM(p_data, 1);
  UINT8_TO_BE_STREAM(p_data, abs_vol);
  p_pkt->len = (p_data - p_start);
  return AVRC_STS_NO_ERROR;
}

/*******************************************************************************
 *
 * Function         avrc_bld_group_navigation_rsp
 *
 * Description      This function builds the Group Navigation
 *                  response.
 *
 * Returns          AVRC_STS_NO_ERROR, if the response is built successfully
 *                  Otherwise, the error code.
 *
 ******************************************************************************/
tAVRC_STS avrc_bld_group_navigation_rsp(uint16_t navi_id, BT_HDR* p_pkt) {
  if (!AVRC_IS_VALID_GROUP(navi_id)) {
    AVRC_TRACE_ERROR("%s bad navigation op id: %d", __func__, navi_id);
    return AVRC_STS_BAD_PARAM;
  }
  AVRC_TRACE_API("%s", __func__);
  uint8_t* p_data = (uint8_t*)(p_pkt + 1) + p_pkt->offset;
  UINT16_TO_BE_STREAM(p_data, navi_id);
  p_pkt->len = 2;
  return AVRC_STS_NO_ERROR;
}

/*******************************************************************************
 *
 * Function         avrc_bld_rejected_rsp
 *
 * Description      This function builds the General Response response.
 *
 * Returns          AVRC_STS_NO_ERROR, if the response is built successfully
 *
 ******************************************************************************/
static tAVRC_STS avrc_bld_rejected_rsp(tAVRC_RSP* p_rsp, BT_HDR* p_pkt) {
  uint8_t* p_start = (uint8_t*)(p_pkt + 1) + p_pkt->offset;
  uint8_t* p_data;
  uint8_t opcode = p_rsp->opcode;

  AVRC_TRACE_API("%s: status=%d, pdu:x%x, opcode=%x", __func__, p_rsp->status,
                 p_rsp->pdu, opcode);

  if (opcode == AVRC_OP_BROWSE) {
    p_data = p_start + 1;
    if ((AVRC_PDU_INVALID == *p_start) ||
        (avrc_opcode_from_pdu(*p_start) != AVRC_OP_BROWSE)) {
      /* if invalid or the given opcode is not recognized as a browsing command
       * opcode, */
      /* use general reject command */
      *p_start = AVRC_PDU_GENERAL_REJECT;
    }
  } else {
    p_data = p_start + 2;
  }
  AVRC_TRACE_DEBUG("%s pdu:x%x, Opcode:%x", __func__, *p_start, opcode);
  UINT16_TO_BE_STREAM(p_data, 1);
  UINT8_TO_BE_STREAM(p_data, p_rsp->status);
  p_pkt->len = p_data - p_start;
  return AVRC_STS_NO_ERROR;
}

/*****************************************************************************
 *  the following commands are introduced in AVRCP 1.4
 ****************************************************************************/

/*******************************************************************************
 *
 * Function         avrc_bld_ctrl_status_rsp
 *
 * Description      This function builds the responses with a uint8_t parameter.
 *
 * Returns          AVRC_STS_NO_ERROR, if the response is built successfully
 *                  Otherwise, the error code.
 *
 ******************************************************************************/
static tAVRC_STS avrc_bld_ctrl_status_rsp(tAVRC_RSP* p_rsp, BT_HDR* p_pkt) {
  uint8_t* p_start = (uint8_t*)(p_pkt + 1) + p_pkt->offset;
  AVRC_TRACE_DEBUG("pdu:x%x", *p_start);

  /* To calculate length */
  uint8_t* p_data = p_start + 2; /* pdu + rsvd */

  /* add fixed lenth - status(1) */
  UINT16_TO_BE_STREAM(p_data, 1);
  UINT8_TO_BE_STREAM(p_data, p_rsp->status);
  p_pkt->len = (p_data - p_start);
  return AVRC_STS_NO_ERROR;
}

/*******************************************************************************
 *
 * Function         avrc_bld_set_addr_player_rsp
 *
 * Description      This function builds the Set Addresses Player response.
 *
 * Returns          AVRC_STS_NO_ERROR, if the response is built successfully
 *                  Otherwise, the error code.
 *
 ******************************************************************************/
static tAVRC_STS avrc_bld_set_addr_player_rsp(tAVRC_RSP* p_rsp, BT_HDR* p_pkt) {
  AVRC_TRACE_API("%s", __func__);
  return avrc_bld_ctrl_status_rsp(p_rsp, p_pkt);
}

/*******************************************************************************
 *
 * Function         avrc_bld_set_browsed_player_rsp
 *
 * Description      This function builds the Set Browsed Player response.
 *
 *                  This message goes through the Browsing channel
 *
 * Returns          AVRC_STS_NO_ERROR, if the response is built successfully
 *                  Otherwise, the error code.
 *
 ******************************************************************************/
static tAVRC_STS avrc_bld_set_browsed_player_rsp(tAVRC_SET_BR_PLAYER_RSP* p_rsp,
                                                 BT_HDR* p_pkt) {
  uint8_t *p_data, *p_start;
  uint8_t* p_len;
  uint16_t len;
  tAVRC_NAME* p_folders = p_rsp->p_folders;
  uint16_t len_left;
  uint8_t* p_folder_depth;
  uint16_t mtu;

  /* make sure the given buffer can accomodate this response */
  len_left = BT_DEFAULT_BUFFER_SIZE - BT_HDR_SIZE;
  p_data = (uint8_t*)(p_pkt + 1);
  BE_STREAM_TO_UINT16(mtu, p_data);
  if (len_left > mtu) {
    len_left = mtu;
  }
  len_left = len_left - p_pkt->offset - p_pkt->len;
  AVRC_TRACE_DEBUG("len_left:%d, mtu:%d ", len_left, mtu);

  /* get the existing length, if any, and also the num attributes */
  p_start = (uint8_t*)(p_pkt + 1) + p_pkt->offset;
  p_data = p_len = p_start + 1; /* pdu */

  /* the existing len */
  BE_STREAM_TO_UINT16(len, p_data);
  /* find the position to add the folder depth.
   * 9 is sizeof (status + uid_counter + num_items + charset_id) */
  p_folder_depth = p_data + 9;
  if (len == 0) {
    /* first time initialize the attribute count */
    UINT8_TO_BE_STREAM(p_data, p_rsp->status);
    UINT16_TO_BE_STREAM(p_data, p_rsp->uid_counter);
    UINT32_TO_BE_STREAM(p_data, p_rsp->num_items);
    UINT16_TO_BE_STREAM(p_data, p_rsp->charset_id);
    *p_folder_depth = 0;
    p_data++;
    len = 10;
    /* assuming that we would never use a buffer that is too small for headers
     */
    len_left -= 12;
  } else {
    p_data = p_start + p_pkt->len;
  }

  for (uint8_t xx = 0;
       (xx < p_rsp->folder_depth) && (len_left > (p_folders[xx].str_len + 2));
       xx++) {
    (*p_folder_depth)++;
    UINT16_TO_BE_STREAM(p_data, p_folders[xx].str_len);
    ARRAY_TO_BE_STREAM(p_data, p_folders[xx].p_str, p_folders[xx].str_len);
    len += (p_folders[xx].str_len + 2);
  }
  UINT16_TO_BE_STREAM(p_len, len);
  p_pkt->len = (p_data - p_start);
  return AVRC_STS_NO_ERROR;
}

/*******************************************************************************
 *
 * Function         avrc_bld_get_folder_items_rsp
 *
 * Description      This function builds the Get Folder Items response.
 *                  The error code is returned in *p_status.
 *                  AVRC_STS_INTERNAL_ERR means no buffers.
 *                  Try again later or with smaller item_count
 *
 *                  This message goes through the Browsing channel
 *
 * Returns          AVRC_STS_NO_ERROR, if the response is built successfully
 *                  AVRC_STS_INTERNAL_ERR, if the given buffer does not have
 *                  enough room
 *                  Otherwise, the error code.
 *
 ******************************************************************************/
static tAVRC_STS avrc_bld_get_folder_items_rsp(tAVRC_GET_ITEMS_RSP* p_rsp,
                                               BT_HDR* p_pkt) {
  uint8_t *p_data, *p_start;
  uint8_t *p_len, xx;
  uint16_t len;
  uint16_t item_len;
  uint8_t *p_item_len, yy;
  tAVRC_ITEM_PLAYER* p_player;
  tAVRC_ITEM_FOLDER* p_folder;
  tAVRC_ITEM_MEDIA* p_media;
  tAVRC_ATTR_ENTRY* p_attr;
  tAVRC_ITEM* p_item_list = p_rsp->p_item_list;
  tAVRC_STS status = AVRC_STS_NO_ERROR;
  uint16_t len_left;
  uint8_t *p_num, *p;
  uint8_t *p_item_start, *p_attr_count;
  uint16_t item_count;
  uint16_t mtu;
  bool multi_items_add_fail = false;
  AVRC_TRACE_API("%s", __func__);

  /* make sure the given buffer can accomodate this response */
  len_left = BT_DEFAULT_BUFFER_SIZE - BT_HDR_SIZE;
  p = (uint8_t*)(p_pkt + 1);
  BE_STREAM_TO_UINT16(mtu, p);
  if (len_left > mtu) len_left = mtu;
  len_left = len_left - p_pkt->offset - p_pkt->len;

  /* get the existing length, if any, and also the num attributes */
  p_start = (uint8_t*)(p_pkt + 1) + p_pkt->offset;
  p_data = p_len = p_start + 1; /* pdu */

  /* the existing len */
  BE_STREAM_TO_UINT16(len, p_data);
  p_num = p_data + 3;
  if (len == 0) {
    /* first time initialize the attribute count */
    UINT8_TO_BE_STREAM(p_data, p_rsp->status);
    UINT16_TO_BE_STREAM(p_data, p_rsp->uid_counter);
    item_count = 0;
    p_data += 2;
    len = 5;
    len_left -= 5;
  } else {
    p_data = p_start + p_pkt->len;
    p = p_num;
    BE_STREAM_TO_UINT16(item_count, p);
  }
  AVRC_TRACE_DEBUG("len:%d, len_left:%d, num:%d", len, len_left, item_count);

  /* min len required = item_type(1) + item len(2) + min item (14) = 17 */
  for (xx = 0;
       xx < p_rsp->item_count && len_left > AVRC_MIN_LEN_GET_FOLDER_ITEMS_RSP &&
       !multi_items_add_fail;
       xx++) {
    p_item_start = p_data;
    UINT8_TO_BE_STREAM(p_data, p_item_list[xx].item_type);
    /* variable item lenth - save the location to add length */
    p_item_len = p_data;
    p_data += 2;
    item_len = 0;
    len_left -= 3; /* item_type(1) + item len(2) */
    switch (p_item_list[xx].item_type) {
      case AVRC_ITEM_PLAYER:
        /* min len required: 2 + 1 + 4 + 1 + 16 + 2 + 2 = 30 + str_len */
        p_player = &p_item_list[xx].u.player;
        item_len = AVRC_FEATURE_MASK_SIZE + p_player->name.str_len + 12;

        if ((len_left <= item_len) || !AVRC_ITEM_PLAYER_IS_VALID(p_player)) {
          p_data = p_item_start;
        } else {
          UINT16_TO_BE_STREAM(p_data, p_player->player_id);
          UINT8_TO_BE_STREAM(p_data, p_player->major_type);
          UINT32_TO_BE_STREAM(p_data, p_player->sub_type);
          UINT8_TO_BE_STREAM(p_data, p_player->play_status);
          ARRAY_TO_BE_STREAM(p_data, p_player->features,
                             AVRC_FEATURE_MASK_SIZE);
          UINT16_TO_BE_STREAM(p_data, p_player->name.charset_id);
          UINT16_TO_BE_STREAM(p_data, p_player->name.str_len);
          ARRAY_TO_BE_STREAM(p_data, p_player->name.p_str,
                             p_player->name.str_len);
        }
        break;

      case AVRC_ITEM_FOLDER:
        /* min len required: 8 + 1 + 1 + 2 + 2 = 14 + str_len */
        p_folder = &p_item_list[xx].u.folder;
        item_len = AVRC_UID_SIZE + p_folder->name.str_len + 6;

        if ((len_left > item_len) && p_folder->name.p_str &&
            p_folder->type <= AVRC_FOLDER_TYPE_YEARS) {
          ARRAY_TO_BE_STREAM(p_data, p_folder->uid, AVRC_UID_SIZE);
          UINT8_TO_BE_STREAM(p_data, p_folder->type);
          UINT8_TO_BE_STREAM(p_data, p_folder->playable);
          UINT16_TO_BE_STREAM(p_data, p_folder->name.charset_id);
          UINT16_TO_BE_STREAM(p_data, p_folder->name.str_len);
          ARRAY_TO_BE_STREAM(p_data, p_folder->name.p_str,
                             p_folder->name.str_len);
        } else {
          p_data = p_item_start;
        }
        break;

      case AVRC_ITEM_MEDIA:
        /* min len required: 8 + 1 + 2 + 2 + 1 = 14 + str_len */
        p_media = &p_item_list[xx].u.media;
        item_len = AVRC_UID_SIZE + p_media->name.str_len + 6;

        if ((len_left >= item_len) && p_media->name.p_str &&
            p_media->type <= AVRC_MEDIA_TYPE_VIDEO) {
          ARRAY_TO_BE_STREAM(p_data, p_media->uid, AVRC_UID_SIZE);
          UINT8_TO_BE_STREAM(p_data, p_media->type);
          UINT16_TO_BE_STREAM(p_data, p_media->name.charset_id);
          UINT16_TO_BE_STREAM(p_data, p_media->name.str_len);
          ARRAY_TO_BE_STREAM(p_data, p_media->name.p_str,
                             p_media->name.str_len);
          p_attr_count = p_data++;
          *p_attr_count = 0;
          len_left -= item_len;
          if (p_media->attr_count > 0) {
            p_attr = p_media->p_attr_list;
            for (yy = 0; yy < p_media->attr_count; yy++) {
              if (p_attr[yy].name.p_str &&
                  AVRC_IS_VALID_MEDIA_ATTRIBUTE(p_attr[yy].attr_id) &&
                  (len_left >= (p_attr[yy].name.str_len + 8))) {
                (*p_attr_count)++;
                UINT32_TO_BE_STREAM(p_data, p_attr[yy].attr_id);
                UINT16_TO_BE_STREAM(p_data, p_attr[yy].name.charset_id);
                UINT16_TO_BE_STREAM(p_data, p_attr[yy].name.str_len);
                ARRAY_TO_BE_STREAM(p_data, p_attr[yy].name.p_str,
                                   p_attr[yy].name.str_len);
                item_len += (p_attr[yy].name.str_len + 8);
                len_left -= (p_attr[yy].name.str_len + 8);
              } else if ((len_left < (p_attr[yy].name.str_len + 8)) &&
                         item_count > 0) {
                p_data = p_item_start;
                multi_items_add_fail = TRUE;
                break;
              }
            }
          }
        } else {
          if (len_left < item_len && item_count > 0)
            multi_items_add_fail = TRUE;
          p_data = p_item_start;
        }
        break;
    } /* switch item_type */

    if (p_item_start != p_data) {
      /* successfully added the item */
      item_count++;
      /* fill in variable item lenth */
      UINT16_TO_BE_STREAM(p_item_len, item_len);
    } else {
      if (!multi_items_add_fail) {
        /* some item is not added properly - set an error status */
        if (len_left < item_len)
          status = AVRC_STS_INTERNAL_ERR;
        else
          status = AVRC_STS_BAD_PARAM;
      }
    }
    if (!multi_items_add_fail) {
      len += item_len;
      len += 3; /* the item_type(1) and item_len(2) */
    }
    AVRC_TRACE_DEBUG("len:%d, len_left:%d, num:%d, item_len:%d", len, len_left,
                     item_count, item_len);
  } /* for item_count */

  UINT16_TO_BE_STREAM(p_num, item_count);
  UINT16_TO_BE_STREAM(p_len, len);
  p_pkt->len = (p_data - p_start);

  return status;
}

/*******************************************************************************
 *
 * Function         avrc_bld_change_path_rsp
 *
 * Description      This function builds the Change Path response.
 *
 *                  This message goes through the Browsing channel
 *
 * Returns          AVRC_STS_NO_ERROR, if the response is built successfully
 *                  Otherwise, the error code.
 *
 ******************************************************************************/
static tAVRC_STS avrc_bld_change_path_rsp(tAVRC_CHG_PATH_RSP* p_rsp,
                                          BT_HDR* p_pkt) {
  uint8_t *p_data, *p_start;

  /* get the existing length, if any, and also the num attributes */
  p_start = (uint8_t*)(p_pkt + 1) + p_pkt->offset;
  p_data = p_start + 1; /* pdu */
  /* add fixed length - status(1) + num_items(4) */
  UINT16_TO_BE_STREAM(p_data, 5);
  UINT8_TO_BE_STREAM(p_data, p_rsp->status);
  UINT32_TO_BE_STREAM(p_data, p_rsp->num_items);
  p_pkt->len = (p_data - p_start);
  return AVRC_STS_NO_ERROR;
}

/*******************************************************************************
 *
 * Function         avrc_bld_get_attrs_rsp
 *
 * Description      This function builds the GetItemAttributes response,
 *
 *                  The Get Item Attributes message goes through the
 *                  Browsing channel (already specified in the |p_pkt|)
 *
 * Returns          AVRC_STS_NO_ERROR, if the response is built successfully
 *                  AVRC_STS_INTERNAL_ERR, if the given buffer does not have
 *                  enough room
 *                  Otherwise, the error code.
 *
 ******************************************************************************/
static tAVRC_STS avrc_bld_get_item_attrs_rsp(tAVRC_GET_ATTRS_RSP* p_rsp,
                                             BT_HDR* p_pkt) {
  AVRC_TRACE_API("%s", __func__);
  if (!p_rsp->p_attrs) {
    AVRC_TRACE_ERROR("%s NULL p_attrs", __func__);
    return AVRC_STS_BAD_PARAM;
  }
  /* Figure out how much we have left in current buffer */
  int remaining_buffer_capacity =
      BT_DEFAULT_BUFFER_SIZE - BT_HDR_SIZE - p_pkt->offset;
  /* Get to the beginning of data section in buffer */
  uint8_t* p_data = (uint8_t*)(p_pkt + 1);
  /* Get the MTU size that is filled in earlier */
  uint16_t mtu;
  BE_STREAM_TO_UINT16(mtu, p_data);
  if (remaining_buffer_capacity > mtu) {
    remaining_buffer_capacity = mtu;
  }
  AVRC_TRACE_DEBUG("%s: remaining_buffer_capacity:%d, mtu:%d", __func__,
                   remaining_buffer_capacity, mtu);
  if (remaining_buffer_capacity < 5) {
    AVRC_TRACE_ERROR("%s: not enough space for packet header, remaining:%d < 5",
                     __func__, remaining_buffer_capacity);
    return AVRC_STS_INTERNAL_ERR;
  }
  /* Get to the beginning of PDU */
  uint8_t* p_pdu_start = (uint8_t*)(p_pkt + 1) + p_pkt->offset;
  /* Skip PDU ID to get pointer to Parameter length */
  uint8_t* p_parameter_len;
  p_data = p_parameter_len = p_pdu_start + 1;
  /* Parse existing parameter length */
  uint16_t parameter_len;
  BE_STREAM_TO_UINT16(parameter_len, p_data);
  /* Skip one byte to Number of Attributes */
  uint8_t* p_status = p_data++;
  uint8_t* p_attribute_count = p_data++;
  if (parameter_len == 0) {
    /* First time, initialize the status byte */
    *p_status = p_rsp->status;
    if (p_rsp->status != AVRC_STS_NO_ERROR) {
      // TODO(siyuanh): This is a hack
      parameter_len = 1;
      UINT16_TO_BE_STREAM(p_parameter_len, parameter_len);
      p_pkt->len = p_status - p_pdu_start;
      return AVRC_STS_NO_ERROR;
    }
    *p_attribute_count = 0;
  } else {
    // TODO(siyuanh): Why do wee need this case?
    p_data = p_pdu_start + p_pkt->len;
  }
  remaining_buffer_capacity -= p_data - p_pdu_start;
  /* Fill in the Attribute ID, Character Set, Length and Values */
  avrc_build_attribute_entries(p_rsp->num_attrs, p_rsp->p_attrs,
                               remaining_buffer_capacity, &p_data,
                               p_attribute_count);
  parameter_len = p_data - p_status;
  UINT16_TO_BE_STREAM(p_parameter_len, parameter_len);
  p_pkt->len = p_data - p_pdu_start;
  return AVRC_STS_NO_ERROR;
}

/*******************************************************************************
 *
 * Function         avrc_bld_get_num_of_item_rsp
 *
 * Description      This function builds the Get Total Number of Items response.
 *
 *                  This message goes through the Browsing channel
 *
 * Returns          AVRC_STS_NO_ERROR, if the response is built successfully
 *                  AVRC_STS_INTERNAL_ERR, if the given buffer does not have
 *                  enough room
 *                  Otherwise, the error code.
 *
 ******************************************************************************/
static tAVRC_STS avrc_bld_get_num_of_item_rsp(tAVRC_GET_NUM_OF_ITEMS_RSP* p_rsp,
                                              BT_HDR* p_pkt) {
  uint8_t *p_data, *p_start, *p_len;

  AVRC_TRACE_API("%s", __func__);
  /* get the existing length, if any, and also the num attributes */
  p_start = (uint8_t*)(p_pkt + 1) + p_pkt->offset;
  p_data = p_len = p_start + 1; /* pdu */

  if (p_rsp->status == AVRC_STS_NO_ERROR) {
    /* add fixed lenth - status(1) + uid_counter(2) + num_items(4) */
    UINT16_TO_BE_STREAM(p_data, 7);
    UINT8_TO_BE_STREAM(p_data, p_rsp->status);
    UINT16_TO_BE_STREAM(p_data, p_rsp->uid_counter);
    UINT32_TO_BE_STREAM(p_data, p_rsp->num_items);
    p_pkt->len = (p_data - p_start);
    return AVRC_STS_NO_ERROR;
  } else {
    /* add fixed lenth - status(1) */
    UINT16_TO_BE_STREAM(p_data, 7);
    UINT8_TO_BE_STREAM(p_data, p_rsp->status);
    p_pkt->len = (p_data - p_start);
    return p_rsp->status;
  }
}

/*******************************************************************************
 *
 * Function         avrc_bld_search_rsp
 *
 * Description      This function builds the Search response.
 *
 *                  This message goes through the Browsing channel
 *
 * Returns          AVRC_STS_NO_ERROR, if the response is built successfully
 *                  Otherwise, the error code.
 *
 ******************************************************************************/
static tAVRC_STS avrc_bld_search_rsp(tAVRC_SEARCH_RSP* p_rsp, BT_HDR* p_pkt) {
  uint8_t *p_data, *p_start, *p_len;

  AVRC_TRACE_API("%s", __func__);
  /* get the existing length, if any, and also the num attributes */
  p_start = (uint8_t*)(p_pkt + 1) + p_pkt->offset;
  p_data = p_len = p_start + 1; /* pdu */

  /* add fixed lenth - status(1) + uid_counter(2) + num_items(4) */
  UINT16_TO_BE_STREAM(p_data, 7);
  UINT8_TO_BE_STREAM(p_data, p_rsp->status);
  UINT16_TO_BE_STREAM(p_data, p_rsp->uid_counter);
  UINT32_TO_BE_STREAM(p_data, p_rsp->num_items);
  p_pkt->len = (p_data - p_start);
  return AVRC_STS_NO_ERROR;
}

/*******************************************************************************
 *
 * Function         avrc_bld_play_item_rsp
 *
 * Description      This function builds the Play Item response.
 *
 * Returns          AVRC_STS_NO_ERROR, if the response is built successfully
 *                  Otherwise, the error code.
 *
 ******************************************************************************/
static tAVRC_STS avrc_bld_play_item_rsp(tAVRC_RSP* p_rsp, BT_HDR* p_pkt) {
  AVRC_TRACE_API("%s", __func__);
  return avrc_bld_ctrl_status_rsp(p_rsp, p_pkt);
}

/*******************************************************************************
 *
 * Function         avrc_bld_add_to_now_playing_rsp
 *
 * Description      This function builds the Add to Now Playing response.
 *
 * Returns          AVRC_STS_NO_ERROR, if the response is built successfully
 *                  Otherwise, the error code.
 *
 ******************************************************************************/
static tAVRC_STS avrc_bld_add_to_now_playing_rsp(tAVRC_RSP* p_rsp,
                                                 BT_HDR* p_pkt) {
  AVRC_TRACE_API("%s", __func__);
  return avrc_bld_ctrl_status_rsp(p_rsp, p_pkt);
}

/*******************************************************************************
 *
 * Function         avrc_bld_init_rsp_buffer
 *
 * Description      This function initializes the response buffer based on PDU
 *
 * Returns          NULL, if no buffer or failure to build the message.
 *                  Otherwise, the buffer that contains the initialized message.
 *
 ******************************************************************************/
static BT_HDR* avrc_bld_init_rsp_buffer(tAVRC_RESPONSE* p_rsp) {
  uint16_t offset = 0;
  uint16_t chnl = AVCT_DATA_CTRL;
  uint8_t opcode = avrc_opcode_from_pdu(p_rsp->pdu);

  AVRC_TRACE_API("%s: pdu=%x, opcode=%x/%x", __func__, p_rsp->pdu, opcode,
                 p_rsp->rsp.opcode);
  if (opcode != p_rsp->rsp.opcode && p_rsp->rsp.status != AVRC_STS_NO_ERROR &&
      avrc_is_valid_opcode(p_rsp->rsp.opcode)) {
    opcode = p_rsp->rsp.opcode;
    AVRC_TRACE_API("%s opcode=%x", __func__, opcode);
  }

  switch (opcode) {
    case AVRC_OP_BROWSE:
      chnl = AVCT_DATA_BROWSE;
      offset = AVCT_BROWSE_OFFSET;
      break;

    case AVRC_OP_PASS_THRU:
      offset = AVRC_MSG_PASS_THRU_OFFSET;
      break;

    case AVRC_OP_VENDOR:
      offset = AVRC_MSG_VENDOR_OFFSET;
      break;
  }

  /* allocate and initialize the buffer */
  BT_HDR* p_pkt = (BT_HDR*)osi_malloc(BT_DEFAULT_BUFFER_SIZE);
  uint8_t *p_data, *p_start;

  p_pkt->layer_specific = chnl;
  p_pkt->event = opcode;
  p_pkt->offset = offset;
  p_data = (uint8_t*)(p_pkt + 1) + p_pkt->offset;
  p_start = p_data;

  /* pass thru - group navigation - has a two byte op_id, so dont do it here */
  if (opcode != AVRC_OP_PASS_THRU) *p_data++ = p_rsp->pdu;

  switch (opcode) {
    case AVRC_OP_VENDOR:
      /* reserved 0, packet_type 0 */
      UINT8_TO_BE_STREAM(p_data, 0);
    /* continue to the next "case to add length */

    case AVRC_OP_BROWSE:
      /* add fixed lenth - 0 */
      UINT16_TO_BE_STREAM(p_data, 0);
      break;
  }

  p_pkt->len = (p_data - p_start);
  p_rsp->rsp.opcode = opcode;

  return p_pkt;
}

/*******************************************************************************
 *
 * Function         AVRC_BldResponse
 *
 * Description      This function builds the given AVRCP response to the given
 *                  buffer
 *
 * Returns          AVRC_STS_NO_ERROR, if the response is built successfully
 *                  Otherwise, the error code.
 *
 ******************************************************************************/
tAVRC_STS AVRC_BldResponse(uint8_t handle, tAVRC_RESPONSE* p_rsp,
                           BT_HDR** pp_pkt) {
  tAVRC_STS status = AVRC_STS_BAD_PARAM;
  BT_HDR* p_pkt;
  bool alloc = false;
  uint8_t* p;
  uint16_t peer_mtu;

  if (!p_rsp || !pp_pkt) {
    AVRC_TRACE_API("%s Invalid parameters passed. p_rsp=%p, pp_pkt=%p",
                   __func__, p_rsp, pp_pkt);
    return AVRC_STS_BAD_PARAM;
  }

  if (*pp_pkt == NULL) {
    *pp_pkt = avrc_bld_init_rsp_buffer(p_rsp);
    if (*pp_pkt == NULL) {
      AVRC_TRACE_API("%s Failed to initialize response buffer", __func__);
      return AVRC_STS_INTERNAL_ERR;
    }

    if ((*pp_pkt)->layer_specific == AVCT_DATA_BROWSE) {
      p = (uint8_t*)((*pp_pkt) + 1);
      peer_mtu = AVCT_GetBrowseMtu(handle) - AVCT_HDR_LEN_SINGLE;
      UINT16_TO_BE_STREAM(p, peer_mtu);
    }

    alloc = true;
  }
  status = AVRC_STS_NO_ERROR;
  p_pkt = *pp_pkt;

  AVRC_TRACE_API("%s pdu=%x status=%x", __func__, p_rsp->rsp.pdu,
                 p_rsp->rsp.status);
  if (p_rsp->rsp.status != AVRC_STS_NO_ERROR) {
    return (avrc_bld_rejected_rsp(&p_rsp->rsp, p_pkt));
  }

  switch (p_rsp->pdu) {
    case AVRC_PDU_NEXT_GROUP:
    case AVRC_PDU_PREV_GROUP:
      status = avrc_bld_group_navigation_rsp(p_rsp->pdu, p_pkt);
      break;

    case AVRC_PDU_GET_CAPABILITIES:
      status = avrc_bld_get_capability_rsp(&p_rsp->get_caps, p_pkt);
      break;

    case AVRC_PDU_LIST_PLAYER_APP_ATTR:
      status =
          avrc_bld_list_app_settings_attr_rsp(&p_rsp->list_app_attr, p_pkt);
      break;

    case AVRC_PDU_LIST_PLAYER_APP_VALUES:
      status =
          avrc_bld_list_app_settings_values_rsp(&p_rsp->list_app_values, p_pkt);
      break;

    case AVRC_PDU_GET_CUR_PLAYER_APP_VALUE:
      status = avrc_bld_get_cur_app_setting_value_rsp(&p_rsp->get_cur_app_val,
                                                      p_pkt);
      break;

    case AVRC_PDU_SET_PLAYER_APP_VALUE:
      status = avrc_bld_set_app_setting_value_rsp(&p_rsp->set_app_val, p_pkt);
      break;

    case AVRC_PDU_GET_PLAYER_APP_ATTR_TEXT:
      status = avrc_bld_get_app_setting_attr_text_rsp(&p_rsp->get_app_attr_txt,
                                                      p_pkt);
      break;

    case AVRC_PDU_GET_PLAYER_APP_VALUE_TEXT:
      status = avrc_bld_get_app_setting_value_text_rsp(&p_rsp->get_app_val_txt,
                                                       p_pkt);
      break;

    case AVRC_PDU_INFORM_DISPLAY_CHARSET:
      status = avrc_bld_inform_charset_rsp(&p_rsp->inform_charset, p_pkt);
      break;

    case AVRC_PDU_INFORM_BATTERY_STAT_OF_CT:
      status = avrc_bld_inform_battery_status_rsp(&p_rsp->inform_battery_status,
                                                  p_pkt);
      break;

    case AVRC_PDU_GET_ELEMENT_ATTR:
      status = avrc_bld_get_elem_attrs_rsp(&p_rsp->get_attrs, p_pkt);
      break;

    case AVRC_PDU_GET_PLAY_STATUS:
      status = avrc_bld_get_play_status_rsp(&p_rsp->get_play_status, p_pkt);
      break;

    case AVRC_PDU_REGISTER_NOTIFICATION:
      status = avrc_bld_notify_rsp(&p_rsp->reg_notif, p_pkt);
      break;

    case AVRC_PDU_REQUEST_CONTINUATION_RSP:
      status = avrc_bld_next_rsp(&p_rsp->continu, p_pkt);
      break;

    case AVRC_PDU_ABORT_CONTINUATION_RSP:
      status = avrc_bld_next_rsp(&p_rsp->abort, p_pkt);
      break;

    case AVRC_PDU_SET_ADDRESSED_PLAYER:
      status = avrc_bld_set_addr_player_rsp(&p_rsp->addr_player, p_pkt);
      break;

    case AVRC_PDU_PLAY_ITEM:
      status = avrc_bld_play_item_rsp(&p_rsp->play_item, p_pkt);
      break;

    case AVRC_PDU_SET_ABSOLUTE_VOLUME:
      status = avrc_bld_set_absolute_volume_rsp(p_rsp->volume.volume, p_pkt);
      break;

    case AVRC_PDU_ADD_TO_NOW_PLAYING:
      status = avrc_bld_add_to_now_playing_rsp(&p_rsp->add_to_play, p_pkt);
      break;

    case AVRC_PDU_SET_BROWSED_PLAYER:
      status = avrc_bld_set_browsed_player_rsp(&p_rsp->br_player, p_pkt);
      break;

    case AVRC_PDU_GET_FOLDER_ITEMS:
      status = avrc_bld_get_folder_items_rsp(&p_rsp->get_items, p_pkt);
      break;

    case AVRC_PDU_CHANGE_PATH:
      status = avrc_bld_change_path_rsp(&p_rsp->chg_path, p_pkt);
      break;

    case AVRC_PDU_GET_ITEM_ATTRIBUTES:
      status = avrc_bld_get_item_attrs_rsp(&p_rsp->get_attrs, p_pkt);
      break;

    case AVRC_PDU_GET_TOTAL_NUM_OF_ITEMS:
      status = avrc_bld_get_num_of_item_rsp(&p_rsp->get_num_of_items, p_pkt);
      break;

    case AVRC_PDU_SEARCH:
      status = avrc_bld_search_rsp(&p_rsp->search, p_pkt);
      break;
  }

  if (alloc && (status != AVRC_STS_NO_ERROR)) {
    osi_free(p_pkt);
    *pp_pkt = NULL;
  }
  AVRC_TRACE_API("%s returning %d", __func__, status);
  return status;
}
