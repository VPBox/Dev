/******************************************************************************
 *
 *  Copyright 2002-2012 Broadcom Corporation
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
 *  Common API for the Advanced Audio Distribution Profile (A2DP)
 *
 ******************************************************************************/

#define LOG_TAG "a2dp_api"

#include "a2dp_api.h"

#include <string.h>

#include "a2dp_int.h"
#include "avdt_api.h"
#include "bt_common.h"
#include "bt_target.h"
#include "osi/include/log.h"
#include "sdpdefs.h"

using bluetooth::Uuid;

/*****************************************************************************
 *  Global data
 ****************************************************************************/
tA2DP_CB a2dp_cb;
static uint16_t a2dp_attr_list[] = {
    ATTR_ID_SERVICE_CLASS_ID_LIST, /* update A2DP_NUM_ATTR, if changed */
    ATTR_ID_BT_PROFILE_DESC_LIST,  ATTR_ID_SUPPORTED_FEATURES,
    ATTR_ID_SERVICE_NAME,          ATTR_ID_PROTOCOL_DESC_LIST,
    ATTR_ID_PROVIDER_NAME};

/******************************************************************************
 *
 * Function         a2dp_sdp_cback
 *
 * Description      This is the SDP callback function used by A2DP_FindService.
 *                  This function will be executed by SDP when the service
 *                  search is completed.  If the search is successful, it
 *                  finds the first record in the database that matches the
 *                  UUID of the search.  Then retrieves various parameters
 *                  from the record.  When it is finished it calls the
 *                  application callback function.
 *
 * Returns          Nothing.
 *
 *****************************************************************************/
static void a2dp_sdp_cback(uint16_t status) {
  tSDP_DISC_REC* p_rec = NULL;
  tSDP_DISC_ATTR* p_attr;
  bool found = false;
  tA2DP_Service a2dp_svc;
  tSDP_PROTOCOL_ELEM elem;
  RawAddress peer_address = RawAddress::kEmpty;

  LOG_INFO(LOG_TAG, "%s: status: %d", __func__, status);

  if (status == SDP_SUCCESS) {
    /* loop through all records we found */
    do {
      /* get next record; if none found, we're done */
      if ((p_rec = SDP_FindServiceInDb(
               a2dp_cb.find.p_db, a2dp_cb.find.service_uuid, p_rec)) == NULL) {
        break;
      }
      memset(&a2dp_svc, 0, sizeof(tA2DP_Service));
      peer_address = p_rec->remote_bd_addr;

      /* get service name */
      if ((p_attr = SDP_FindAttributeInRec(p_rec, ATTR_ID_SERVICE_NAME)) !=
          NULL) {
        a2dp_svc.p_service_name = (char*)p_attr->attr_value.v.array;
        a2dp_svc.service_len = SDP_DISC_ATTR_LEN(p_attr->attr_len_type);
      }

      /* get provider name */
      if ((p_attr = SDP_FindAttributeInRec(p_rec, ATTR_ID_PROVIDER_NAME)) !=
          NULL) {
        a2dp_svc.p_provider_name = (char*)p_attr->attr_value.v.array;
        a2dp_svc.provider_len = SDP_DISC_ATTR_LEN(p_attr->attr_len_type);
      }

      /* get supported features */
      if ((p_attr = SDP_FindAttributeInRec(
               p_rec, ATTR_ID_SUPPORTED_FEATURES)) != NULL) {
        a2dp_svc.features = p_attr->attr_value.v.u16;
      }

      /* get AVDTP version */
      if (SDP_FindProtocolListElemInRec(p_rec, UUID_PROTOCOL_AVDTP, &elem)) {
        a2dp_svc.avdt_version = elem.params[0];
        LOG_VERBOSE(LOG_TAG, "avdt_version: 0x%x", a2dp_svc.avdt_version);
      }

      /* we've got everything, we're done */
      found = true;
      break;

    } while (true);
  }

  a2dp_cb.find.service_uuid = 0;
  osi_free_and_reset((void**)&a2dp_cb.find.p_db);
  /* return info from sdp record in app callback function */
  if (a2dp_cb.find.p_cback != NULL) {
    (*a2dp_cb.find.p_cback)(found, &a2dp_svc, peer_address);
  }

  return;
}

/*******************************************************************************
 *
 * Function         a2dp_set_avdt_sdp_ver
 *
 * Description      This function allows the script wrapper to change the
 *                  avdt version of a2dp.
 *
 * Returns          None
 *
 ******************************************************************************/
void a2dp_set_avdt_sdp_ver(uint16_t avdt_sdp_ver) {
  a2dp_cb.avdt_sdp_ver = avdt_sdp_ver;
}

/******************************************************************************
 *
 * Function         A2DP_AddRecord
 *
 * Description      This function is called by a server application to add
 *                  SRC or SNK information to an SDP record.  Prior to
 *                  calling this function the application must call
 *                  SDP_CreateRecord() to create an SDP record.
 *
 *                  Input Parameters:
 *                      service_uuid:  Indicates SRC or SNK.
 *
 *                      p_service_name:  Pointer to a null-terminated character
 *                      string containing the service name.
 *
 *                      p_provider_name:  Pointer to a null-terminated character
 *                      string containing the provider name.
 *
 *                      features:  Profile supported features.
 *
 *                      sdp_handle:  SDP handle returned by SDP_CreateRecord().
 *
 *                  Output Parameters:
 *                      None.
 *
 * Returns          A2DP_SUCCESS if function execution succeeded,
 *                  A2DP_INVALID_PARAMS if bad parameters are given.
 *                  A2DP_FAIL if function execution failed.
 *
 *****************************************************************************/
tA2DP_STATUS A2DP_AddRecord(uint16_t service_uuid, char* p_service_name,
                            char* p_provider_name, uint16_t features,
                            uint32_t sdp_handle) {
  uint16_t browse_list[1];
  bool result = true;
  uint8_t temp[8];
  uint8_t* p;
  tSDP_PROTOCOL_ELEM proto_list[A2DP_NUM_PROTO_ELEMS];

  LOG_VERBOSE(LOG_TAG, "%s: uuid: 0x%x", __func__, service_uuid);

  if ((sdp_handle == 0) || (service_uuid != UUID_SERVCLASS_AUDIO_SOURCE &&
                            service_uuid != UUID_SERVCLASS_AUDIO_SINK))
    return A2DP_INVALID_PARAMS;

  /* add service class id list */
  result &= SDP_AddServiceClassIdList(sdp_handle, 1, &service_uuid);

  memset((void*)proto_list, 0,
         A2DP_NUM_PROTO_ELEMS * sizeof(tSDP_PROTOCOL_ELEM));

  /* add protocol descriptor list   */
  proto_list[0].protocol_uuid = UUID_PROTOCOL_L2CAP;
  proto_list[0].num_params = 1;
  proto_list[0].params[0] = AVDT_PSM;
  proto_list[1].protocol_uuid = UUID_PROTOCOL_AVDTP;
  proto_list[1].num_params = 1;
  proto_list[1].params[0] = a2dp_cb.avdt_sdp_ver;

  result &= SDP_AddProtocolList(sdp_handle, A2DP_NUM_PROTO_ELEMS, proto_list);

  /* add profile descriptor list   */
  result &= SDP_AddProfileDescriptorList(
      sdp_handle, UUID_SERVCLASS_ADV_AUDIO_DISTRIBUTION, A2DP_VERSION);

  /* add supported feature */
  if (features != 0) {
    p = temp;
    UINT16_TO_BE_STREAM(p, features);
    result &= SDP_AddAttribute(sdp_handle, ATTR_ID_SUPPORTED_FEATURES,
                               UINT_DESC_TYPE, (uint32_t)2, (uint8_t*)temp);
  }

  /* add provider name */
  if (p_provider_name != NULL) {
    result &= SDP_AddAttribute(
        sdp_handle, ATTR_ID_PROVIDER_NAME, TEXT_STR_DESC_TYPE,
        (uint32_t)(strlen(p_provider_name) + 1), (uint8_t*)p_provider_name);
  }

  /* add service name */
  if (p_service_name != NULL) {
    result &= SDP_AddAttribute(
        sdp_handle, ATTR_ID_SERVICE_NAME, TEXT_STR_DESC_TYPE,
        (uint32_t)(strlen(p_service_name) + 1), (uint8_t*)p_service_name);
  }

  /* add browse group list */
  browse_list[0] = UUID_SERVCLASS_PUBLIC_BROWSE_GROUP;
  result &= SDP_AddUuidSequence(sdp_handle, ATTR_ID_BROWSE_GROUP_LIST, 1,
                                browse_list);

  return (result ? A2DP_SUCCESS : A2DP_FAIL);
}

/******************************************************************************
 *
 * Function         A2DP_FindService
 *
 * Description      This function is called by a client application to
 *                  perform service discovery and retrieve SRC or SNK SDP
 *                  record information from a server.  Information is
 *                  returned for the first service record found on the
 *                  server that matches the service UUID.  The callback
 *                  function will be executed when service discovery is
 *                  complete.  There can only be one outstanding call to
 *                  A2DP_FindService() at a time; the application must wait
 *                  for the callback before it makes another call to
 *                  the function.
 *
 *                  Input Parameters:
 *                      service_uuid:  Indicates SRC or SNK.
 *
 *                      bd_addr:  BD address of the peer device.
 *
 *                      p_db:  Pointer to the information to initialize
 *                             the discovery database.
 *
 *                      p_cback:  Pointer to the A2DP_FindService()
 *                      callback function.
 *
 *                  Output Parameters:
 *                      None.
 *
 * Returns          A2DP_SUCCESS if function execution succeeded,
 *                  A2DP_INVALID_PARAMS if bad parameters are given.
 *                  A2DP_BUSY if discovery is already in progress.
 *                  A2DP_FAIL if function execution failed.
 *
 *****************************************************************************/
tA2DP_STATUS A2DP_FindService(uint16_t service_uuid, const RawAddress& bd_addr,
                              tA2DP_SDP_DB_PARAMS* p_db,
                              tA2DP_FIND_CBACK* p_cback) {
  bool result = true;

  LOG_INFO(LOG_TAG, "%s: peer: %s UUID: 0x%x", __func__,
           bd_addr.ToString().c_str(), service_uuid);
  if ((service_uuid != UUID_SERVCLASS_AUDIO_SOURCE &&
       service_uuid != UUID_SERVCLASS_AUDIO_SINK) ||
      p_db == NULL || p_cback == NULL) {
    LOG_ERROR(LOG_TAG,
              "%s: cannot find service for peer %s UUID 0x%x: "
              "invalid parameters",
              __func__, bd_addr.ToString().c_str(), service_uuid);
    return A2DP_INVALID_PARAMS;
  }

  if (a2dp_cb.find.service_uuid == UUID_SERVCLASS_AUDIO_SOURCE ||
      a2dp_cb.find.service_uuid == UUID_SERVCLASS_AUDIO_SINK) {
    LOG_ERROR(LOG_TAG, "%s: cannot find service for peer %s UUID 0x%x: busy",
              __func__, bd_addr.ToString().c_str(), service_uuid);
    return A2DP_BUSY;
  }

  if (p_db->p_attrs == NULL || p_db->num_attr == 0) {
    p_db->p_attrs = a2dp_attr_list;
    p_db->num_attr = A2DP_NUM_ATTR;
  }

  if (a2dp_cb.find.p_db == NULL)
    a2dp_cb.find.p_db = (tSDP_DISCOVERY_DB*)osi_malloc(p_db->db_len);

  Uuid uuid_list = Uuid::From16Bit(service_uuid);
  result = SDP_InitDiscoveryDb(a2dp_cb.find.p_db, p_db->db_len, 1, &uuid_list,
                               p_db->num_attr, p_db->p_attrs);

  if (result) {
    /* store service_uuid */
    a2dp_cb.find.service_uuid = service_uuid;
    a2dp_cb.find.p_cback = p_cback;

    /* perform service search */
    result = SDP_ServiceSearchAttributeRequest(bd_addr, a2dp_cb.find.p_db,
                                               a2dp_sdp_cback);
    if (!result) {
      a2dp_cb.find.service_uuid = 0;
    }
  }
  if (!result) {
    LOG_ERROR(LOG_TAG,
              "%s: cannot find service for peer %s UUID 0x%x: "
              "SDP error",
              __func__, bd_addr.ToString().c_str(), service_uuid);
    return A2DP_FAIL;
  }

  return A2DP_SUCCESS;
}

/******************************************************************************
 *
 * Function         A2DP_SetTraceLevel
 *
 * Description      Sets the trace level for A2D. If 0xff is passed, the
 *                  current trace level is returned.
 *
 *                  Input Parameters:
 *                      new_level:  The level to set the A2DP tracing to:
 *                      0xff-returns the current setting.
 *                      0-turns off tracing.
 *                      >= 1-Errors.
 *                      >= 2-Warnings.
 *                      >= 3-APIs.
 *                      >= 4-Events.
 *                      >= 5-Debug.
 *
 * Returns          The new trace level or current trace level if
 *                  the input parameter is 0xff.
 *
 *****************************************************************************/
uint8_t A2DP_SetTraceLevel(uint8_t new_level) {
  if (new_level != 0xFF) a2dp_cb.trace_level = new_level;

  return (a2dp_cb.trace_level);
}

/******************************************************************************
 * Function         A2DP_BitsSet
 *
 * Description      Check the given num for the number of bits set
 * Returns          A2DP_SET_ONE_BIT, if one and only one bit is set
 *                  A2DP_SET_ZERO_BIT, if all bits clear
 *                  A2DP_SET_MULTL_BIT, if multiple bits are set
 *****************************************************************************/
uint8_t A2DP_BitsSet(uint64_t num) {
  if (num == 0) return A2DP_SET_ZERO_BIT;
  if ((num & (num - 1)) == 0) return A2DP_SET_ONE_BIT;
  return A2DP_SET_MULTL_BIT;
}

/*******************************************************************************
 *
 * Function         A2DP_Init
 *
 * Description      This function is called to initialize the control block
 *                  for this layer.  It must be called before accessing any
 *                  other API functions for this layer.  It is typically called
 *                  once during the start up of the stack.
 *
 * Returns          void
 *
 ******************************************************************************/
void A2DP_Init(void) {
  memset(&a2dp_cb, 0, sizeof(tA2DP_CB));

  a2dp_cb.avdt_sdp_ver = AVDT_VERSION;

#if defined(A2DP_INITIAL_TRACE_LEVEL)
  a2dp_cb.trace_level = A2DP_INITIAL_TRACE_LEVEL;
#else
  a2dp_cb.trace_level = BT_TRACE_LEVEL_NONE;
#endif
}

uint16_t A2DP_GetAvdtpVersion() { return a2dp_cb.avdt_sdp_ver; }
