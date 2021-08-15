/******************************************************************************
 *
 *  Copyright 2003-2012 Broadcom Corporation
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
 *  This file contains the audio gateway functions performing SDP
 *  operations.
 *
 ******************************************************************************/

#include <cstring>

#include <base/bind.h>

#include "bt_common.h"
#include "bta_ag_api.h"
#include "bta_ag_int.h"
#include "bta_api.h"
#include "bta_sys.h"
#include "btif_config.h"
#include "btm_api.h"
#include "osi/include/osi.h"
#include "sdp_api.h"
#include "stack/include/btu.h"
#include "utl.h"

using bluetooth::Uuid;

/* Number of protocol elements in protocol element list. */
#define BTA_AG_NUM_PROTO_ELEMS 2

/* Number of elements in service class id list. */
#define BTA_AG_NUM_SVC_ELEMS 2

/* size of database for service discovery */
#ifndef BTA_AG_DISC_BUF_SIZE
#define BTA_AG_DISC_BUF_SIZE BT_DEFAULT_BUFFER_SIZE
#endif

/* declare sdp callback functions */
void bta_ag_sdp_cback_1(uint16_t status);
void bta_ag_sdp_cback_2(uint16_t status);
void bta_ag_sdp_cback_3(uint16_t status);
void bta_ag_sdp_cback_4(uint16_t status);
void bta_ag_sdp_cback_5(uint16_t status);
void bta_ag_sdp_cback_6(uint16_t status);

/* SDP callback function table */
typedef tSDP_DISC_CMPL_CB* tBTA_AG_SDP_CBACK;
const tBTA_AG_SDP_CBACK bta_ag_sdp_cback_tbl[] = {
    bta_ag_sdp_cback_1, bta_ag_sdp_cback_2, bta_ag_sdp_cback_3,
    bta_ag_sdp_cback_4, bta_ag_sdp_cback_5, bta_ag_sdp_cback_6};

/*******************************************************************************
 *
 * Function         bta_ag_sdp_cback
 *
 * Description      SDP callback function.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
static void bta_ag_sdp_cback(uint16_t status, uint8_t idx) {
  APPL_TRACE_DEBUG("%s status:0x%x", __func__, status);
  tBTA_AG_SCB* p_scb = bta_ag_scb_by_idx(idx);
  if (p_scb) {
    uint16_t event;
    /* set event according to int/acp */
    if (p_scb->role == BTA_AG_ACP) {
      event = BTA_AG_DISC_ACP_RES_EVT;
    } else {
      event = BTA_AG_DISC_INT_RES_EVT;
    }
    tBTA_AG_DATA disc_result = {.disc_result.status = status};
    do_in_main_thread(FROM_HERE, base::Bind(&bta_ag_sm_execute_by_handle, idx,
                                            event, disc_result));
  }
}

/*******************************************************************************
 *
 * Function         bta_ag_sdp_cback_1 to 6
 *
 * Description      SDP callback functions.  Since there is no way to
 *                  distinguish scb from the callback we need separate
 *                  callbacks for each scb.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_ag_sdp_cback_1(uint16_t status) { bta_ag_sdp_cback(status, 1); }
void bta_ag_sdp_cback_2(uint16_t status) { bta_ag_sdp_cback(status, 2); }
void bta_ag_sdp_cback_3(uint16_t status) { bta_ag_sdp_cback(status, 3); }
void bta_ag_sdp_cback_4(uint16_t status) { bta_ag_sdp_cback(status, 4); }
void bta_ag_sdp_cback_5(uint16_t status) { bta_ag_sdp_cback(status, 5); }
void bta_ag_sdp_cback_6(uint16_t status) { bta_ag_sdp_cback(status, 6); }

/******************************************************************************
 *
 * Function         bta_ag_add_record
 *
 * Description      This function is called by a server application to add
 *                  HSP or HFP information to an SDP record.  Prior to
 *                  calling this function the application must call
 *                  SDP_CreateRecord() to create an SDP record.
 *
 * Returns          true if function execution succeeded,
 *                  false if function execution failed.
 *
 *****************************************************************************/
bool bta_ag_add_record(uint16_t service_uuid, const char* p_service_name,
                       uint8_t scn, tBTA_AG_FEAT features,
                       uint32_t sdp_handle) {
  tSDP_PROTOCOL_ELEM proto_elem_list[BTA_AG_NUM_PROTO_ELEMS];
  uint16_t svc_class_id_list[BTA_AG_NUM_SVC_ELEMS];
  uint16_t browse_list[] = {UUID_SERVCLASS_PUBLIC_BROWSE_GROUP};
  uint16_t version;
  uint16_t profile_uuid;
  uint8_t network;
  bool result = true;
  bool codec_supported = false;
  uint8_t buf[2];

  APPL_TRACE_DEBUG("%s uuid: %x", __func__, service_uuid);

  for (auto& proto_element : proto_elem_list) {
    proto_element = {};
  }

  /* add the protocol element sequence */
  proto_elem_list[0].protocol_uuid = UUID_PROTOCOL_L2CAP;
  proto_elem_list[0].num_params = 0;
  proto_elem_list[1].protocol_uuid = UUID_PROTOCOL_RFCOMM;
  proto_elem_list[1].num_params = 1;
  proto_elem_list[1].params[0] = scn;
  result &=
      SDP_AddProtocolList(sdp_handle, BTA_AG_NUM_PROTO_ELEMS, proto_elem_list);

  /* add service class id list */
  svc_class_id_list[0] = service_uuid;
  svc_class_id_list[1] = UUID_SERVCLASS_GENERIC_AUDIO;
  result &= SDP_AddServiceClassIdList(sdp_handle, BTA_AG_NUM_SVC_ELEMS,
                                      svc_class_id_list);

  /* add profile descriptor list */
  if (service_uuid == UUID_SERVCLASS_AG_HANDSFREE) {
    profile_uuid = UUID_SERVCLASS_HF_HANDSFREE;
    version = BTA_HFP_VERSION;
  } else {
    profile_uuid = UUID_SERVCLASS_HEADSET;
    version = HSP_VERSION_1_2;
  }
  result &= SDP_AddProfileDescriptorList(sdp_handle, profile_uuid, version);

  /* add service name */
  if (p_service_name != nullptr && p_service_name[0] != 0) {
    result &= SDP_AddAttribute(
        sdp_handle, ATTR_ID_SERVICE_NAME, TEXT_STR_DESC_TYPE,
        (uint32_t)(strlen(p_service_name) + 1), (uint8_t*)p_service_name);
  }

  /* add features and network */
  if (service_uuid == UUID_SERVCLASS_AG_HANDSFREE) {
    network = (features & BTA_AG_FEAT_REJECT) ? 1 : 0;
    result &= SDP_AddAttribute(sdp_handle, ATTR_ID_DATA_STORES_OR_NETWORK,
                               UINT_DESC_TYPE, 1, &network);

    if (features & BTA_AG_FEAT_CODEC) codec_supported = true;

    features &= BTA_AG_SDP_FEAT_SPEC;

    /* Codec bit position is different in SDP and in BRSF */
    if (codec_supported) features |= 0x0020;

    UINT16_TO_BE_FIELD(buf, features);
    result &= SDP_AddAttribute(sdp_handle, ATTR_ID_SUPPORTED_FEATURES,
                               UINT_DESC_TYPE, 2, buf);
  }

  /* add browse group list */
  result &= SDP_AddUuidSequence(sdp_handle, ATTR_ID_BROWSE_GROUP_LIST, 1,
                                browse_list);

  return result;
}

/*******************************************************************************
 *
 * Function         bta_ag_create_records
 *
 * Description      Create SDP records for registered services.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_ag_create_records(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data) {
  int i;
  tBTA_SERVICE_MASK services;

  services = p_scb->reg_services >> BTA_HSP_SERVICE_ID;
  for (i = 0; i < BTA_AG_NUM_IDX && services != 0; i++, services >>= 1) {
    /* if service is set in mask */
    if (services & 1) {
      /* add sdp record if not already registered */
      if (bta_ag_cb.profile[i].sdp_handle == 0) {
        bta_ag_cb.profile[i].sdp_handle = SDP_CreateRecord();
        bta_ag_cb.profile[i].scn = BTM_AllocateSCN();
        bta_ag_add_record(bta_ag_uuid[i], data.api_register.p_name[i],
                          bta_ag_cb.profile[i].scn, data.api_register.features,
                          bta_ag_cb.profile[i].sdp_handle);
        bta_sys_add_uuid(bta_ag_uuid[i]);
      }
    }
  }
}

/*******************************************************************************
 *
 * Function         bta_ag_del_records
 *
 * Description      Delete SDP records for any registered services.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_ag_del_records(tBTA_AG_SCB* p_scb) {
  tBTA_AG_SCB* p = &bta_ag_cb.scb[0];
  tBTA_SERVICE_MASK services;
  tBTA_SERVICE_MASK others = 0;
  int i;

  /* get services of all other registered servers */
  for (i = 0; i < BTA_AG_NUM_IDX; i++, p++) {
    if (p_scb == p) {
      continue;
    }

    if (p->in_use && !p->dealloc) {
      others |= p->reg_services;
    }
  }

  others >>= BTA_HSP_SERVICE_ID;
  services = p_scb->reg_services >> BTA_HSP_SERVICE_ID;
  for (i = 0; i < BTA_AG_NUM_IDX && services != 0;
       i++, services >>= 1, others >>= 1) {
    /* if service registered for this scb and not registered for any other scb
     */
    if (((services & 1) == 1) && ((others & 1) == 0)) {
      APPL_TRACE_DEBUG("bta_ag_del_records %d", i);
      if (bta_ag_cb.profile[i].sdp_handle != 0) {
        SDP_DeleteRecord(bta_ag_cb.profile[i].sdp_handle);
        bta_ag_cb.profile[i].sdp_handle = 0;
      }
      BTM_FreeSCN(bta_ag_cb.profile[i].scn);
      BTM_SecClrService(bta_ag_sec_id[i]);
      bta_sys_remove_uuid(bta_ag_uuid[i]);
    }
  }
}

/*******************************************************************************
 *
 * Function         bta_ag_sdp_find_attr
 *
 * Description      Process SDP discovery results to find requested attributes
 *                  for requested service.
 *
 *
 * Returns          true if results found, false otherwise.
 *
 ******************************************************************************/
bool bta_ag_sdp_find_attr(tBTA_AG_SCB* p_scb, tBTA_SERVICE_MASK service) {
  tSDP_DISC_REC* p_rec = nullptr;
  tSDP_DISC_ATTR* p_attr;
  tSDP_PROTOCOL_ELEM pe;
  uint16_t uuid;
  bool result = false;

  if (service & BTA_HFP_SERVICE_MASK) {
    uuid = UUID_SERVCLASS_HF_HANDSFREE;
    /* If there is no cached peer version, use default one */
    if (p_scb->peer_version == HFP_HSP_VERSION_UNKNOWN) {
      p_scb->peer_version = HFP_VERSION_1_1; /* Default version */
    }
  } else if (service & BTA_HSP_SERVICE_MASK && p_scb->role == BTA_AG_INT) {
    uuid = UUID_SERVCLASS_HEADSET_HS;
    p_scb->peer_version = HSP_VERSION_1_2; /* Default version */
  } else {
    uuid = UUID_SERVCLASS_HEADSET_HS;
    p_scb->peer_version = HSP_VERSION_1_0;
  }

  /* loop through all records we found */
  while (true) {
    /* get next record; if none found, we're done */
    p_rec = SDP_FindServiceInDb(p_scb->p_disc_db, uuid, p_rec);
    if (p_rec == nullptr) {
      if (uuid == UUID_SERVCLASS_HEADSET_HS) {
        /* Search again in case the peer device uses the old HSP UUID */
        uuid = UUID_SERVCLASS_HEADSET;
        p_scb->peer_version = HSP_VERSION_1_0;
        p_rec = SDP_FindServiceInDb(p_scb->p_disc_db, uuid, p_rec);
        if (p_rec == nullptr) {
          break;
        }
      } else
        break;
    }

    /* get scn from proto desc list if initiator */
    if (p_scb->role == BTA_AG_INT) {
      if (SDP_FindProtocolListElemInRec(p_rec, UUID_PROTOCOL_RFCOMM, &pe)) {
        p_scb->peer_scn = (uint8_t)pe.params[0];
      } else {
        continue;
      }
    }

    /* get profile version (if failure, version parameter is not updated) */
    uint16_t peer_version = HFP_HSP_VERSION_UNKNOWN;
    if (!SDP_FindProfileVersionInRec(p_rec, uuid, &peer_version)) {
      APPL_TRACE_WARNING("%s: Get peer_version failed, using default 0x%04x",
                         __func__, p_scb->peer_version);
      peer_version = p_scb->peer_version;
    }

    if (service & BTA_HFP_SERVICE_MASK) {
      /* Update cached peer version if the new one is different */
      if (peer_version != p_scb->peer_version) {
        p_scb->peer_version = peer_version;
        if (btif_config_set_bin(
                p_scb->peer_addr.ToString(), HFP_VERSION_CONFIG_KEY,
                (const uint8_t*)&peer_version, sizeof(peer_version))) {
          btif_config_save();
        } else {
          APPL_TRACE_WARNING("%s: Failed to store peer HFP version for %s",
                             __func__, p_scb->peer_addr.ToString().c_str());
        }
      }
      /* get features if HFP */
      p_attr = SDP_FindAttributeInRec(p_rec, ATTR_ID_SUPPORTED_FEATURES);
      if (p_attr != nullptr) {
        /* Found attribute. Get value. */
        /* There might be race condition between SDP and BRSF.  */
        /* Do not update if we already received BRSF.           */
        uint16_t sdp_features = p_attr->attr_value.v.u16;
        bool sdp_wbs_support = sdp_features & BTA_AG_FEAT_WBS_SUPPORT;
        if (!p_scb->received_at_bac && sdp_wbs_support) {
          // Workaround for misbehaving HFs (e.g. some Hyundai car kit) that:
          // 1. Indicate WBS support in SDP and codec negotiation in BRSF
          // 2. But do not send required AT+BAC command
          // Will assume mSBC is enabled and try codec negotiation by default
          p_scb->codec_updated = true;
          p_scb->peer_codecs = BTA_AG_CODEC_CVSD & BTA_AG_CODEC_MSBC;
          p_scb->sco_codec = UUID_CODEC_MSBC;
        }
        if (sdp_features != p_scb->peer_sdp_features) {
          p_scb->peer_sdp_features = sdp_features;
          if (btif_config_set_bin(
                  p_scb->peer_addr.ToString(), HFP_SDP_FEATURES_CONFIG_KEY,
                  (const uint8_t*)&sdp_features, sizeof(sdp_features))) {
            btif_config_save();
          } else {
            APPL_TRACE_WARNING(
                "%s: Failed to store peer HFP SDP Features for %s", __func__,
                p_scb->peer_addr.ToString().c_str());
          }
        }
        if (p_scb->peer_features == 0) {
          p_scb->peer_features = sdp_features & HFP_SDP_BRSF_FEATURES_MASK;
        }
      }
    } else {
      /* No peer version caching for HSP, use discovered one directly */
      p_scb->peer_version = peer_version;
      /* get features if HSP */
      p_attr =
          SDP_FindAttributeInRec(p_rec, ATTR_ID_REMOTE_AUDIO_VOLUME_CONTROL);
      if (p_attr != nullptr) {
        /* Remote volume control of HSP */
        if (p_attr->attr_value.v.u8)
          p_scb->peer_features |= BTA_AG_PEER_FEAT_VOL;
        else
          p_scb->peer_features &= ~BTA_AG_PEER_FEAT_VOL;
      }
    }

    /* found what we needed */
    result = true;
    break;
  }
  return result;
}

/*******************************************************************************
 *
 * Function         bta_ag_do_disc
 *
 * Description      Do service discovery.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_ag_do_disc(tBTA_AG_SCB* p_scb, tBTA_SERVICE_MASK service) {
  Uuid uuid_list[1];
  uint16_t num_uuid = 1;
  uint16_t attr_list[4];
  uint8_t num_attr;

  /* HFP initiator; get proto list and features */
  if (service & BTA_HFP_SERVICE_MASK && p_scb->role == BTA_AG_INT) {
    attr_list[0] = ATTR_ID_SERVICE_CLASS_ID_LIST;
    attr_list[1] = ATTR_ID_PROTOCOL_DESC_LIST;
    attr_list[2] = ATTR_ID_BT_PROFILE_DESC_LIST;
    attr_list[3] = ATTR_ID_SUPPORTED_FEATURES;
    num_attr = 4;
    uuid_list[0] = Uuid::From16Bit(UUID_SERVCLASS_HF_HANDSFREE);
  }
  /* HFP acceptor; get features */
  else if (service & BTA_HFP_SERVICE_MASK && p_scb->role == BTA_AG_ACP) {
    attr_list[0] = ATTR_ID_SERVICE_CLASS_ID_LIST;
    attr_list[1] = ATTR_ID_BT_PROFILE_DESC_LIST;
    attr_list[2] = ATTR_ID_SUPPORTED_FEATURES;
    num_attr = 3;
    uuid_list[0] = Uuid::From16Bit(UUID_SERVCLASS_HF_HANDSFREE);
  }
  /* HSP initiator; get proto list */
  else if (service & BTA_HSP_SERVICE_MASK && p_scb->role == BTA_AG_INT) {
    attr_list[0] = ATTR_ID_SERVICE_CLASS_ID_LIST;
    attr_list[1] = ATTR_ID_PROTOCOL_DESC_LIST;
    attr_list[2] = ATTR_ID_BT_PROFILE_DESC_LIST;
    attr_list[3] = ATTR_ID_REMOTE_AUDIO_VOLUME_CONTROL;
    num_attr = 4;
    // Although UUID_SERVCLASS_HEADSET_HS (0x1131) is to be used in HSP 1.2,
    // some HSP 1.2 implementations, such as PTS, still use
    // UUID_SERVCLASS_HEADSET (0x1108) to store its service record. However,
    // most of such devices are HSP 1.0 devices.
    if (p_scb->hsp_version >= HSP_VERSION_1_2) {
      uuid_list[0] = Uuid::From16Bit(UUID_SERVCLASS_HEADSET_HS);
    } else {
      uuid_list[0] = Uuid::From16Bit(UUID_SERVCLASS_HEADSET);
    }
  } else {
    /* HSP acceptor; get features */
    attr_list[0] = ATTR_ID_SERVICE_CLASS_ID_LIST;
    attr_list[1] = ATTR_ID_PROTOCOL_DESC_LIST;
    attr_list[2] = ATTR_ID_BT_PROFILE_DESC_LIST;
    attr_list[3] = ATTR_ID_REMOTE_AUDIO_VOLUME_CONTROL;
    num_attr = 4;

    if (p_scb->hsp_version >= HSP_VERSION_1_2) {
      uuid_list[0] = Uuid::From16Bit(UUID_SERVCLASS_HEADSET_HS);
    } else {
      /* Legacy from HSP v1.0 */
      uuid_list[0] = Uuid::From16Bit(UUID_SERVCLASS_HEADSET);
    }
  }

  /* allocate buffer for sdp database */
  p_scb->p_disc_db = (tSDP_DISCOVERY_DB*)osi_malloc(BTA_AG_DISC_BUF_SIZE);
  /* set up service discovery database; attr happens to be attr_list len */
  if (SDP_InitDiscoveryDb(p_scb->p_disc_db, BTA_AG_DISC_BUF_SIZE, num_uuid,
                          uuid_list, num_attr, attr_list)) {
    if (SDP_ServiceSearchAttributeRequest(
            p_scb->peer_addr, p_scb->p_disc_db,
            bta_ag_sdp_cback_tbl[bta_ag_scb_to_idx(p_scb) - 1])) {
      return;
    } else {
      LOG(ERROR) << __func__ << ": failed to start SDP discovery for "
                 << p_scb->peer_addr;
    }
  } else {
    LOG(ERROR) << __func__ << ": failed to init SDP discovery database for "
               << p_scb->peer_addr;
  }
  // Failure actions
  bta_ag_free_db(p_scb, tBTA_AG_DATA::kEmpty);
  bta_ag_sm_execute(p_scb, BTA_AG_DISC_FAIL_EVT, tBTA_AG_DATA::kEmpty);
}

/*******************************************************************************
 *
 * Function         bta_ag_free_db
 *
 * Description      Free discovery database.
 *
 *
 * Returns          void
 *
 ******************************************************************************/
void bta_ag_free_db(tBTA_AG_SCB* p_scb, const tBTA_AG_DATA& data) {
  osi_free_and_reset((void**)&p_scb->p_disc_db);
}
