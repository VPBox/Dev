/******************************************************************************
 *
 *  Copyright 2014 Google, Inc.
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

#define LOG_TAG "bt_btif_sock_sdp"

#include "btif_sock_sdp.h"

#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <hardware/bluetooth.h>
#include <hardware/bt_sock.h>

#include "../bta/pb/bta_pbs_int.h"
#include "../include/bta_op_api.h"
#include "bt_common.h"
#include "bt_target.h"
#include "bta_api.h"
#include "bta_jv_api.h"
#include "btif_common.h"
#include "btif_sock_util.h"
#include "btif_util.h"
#include "btm_api.h"
#include "btm_int.h"
#include "btu.h"
#include "hcimsgs.h"
#include "sdp_api.h"
#include "utl.h"

using bluetooth::Uuid;

// This module provides an abstraction on top of the lower-level SDP database
// code for registration and discovery of various bluetooth sockets.
//
// This code also provides for on-demand registration of "pre-registered"
// services as a backwards compatibility function to third-party applications
// expecting a bluez stack.

// Realm Character Set -- 0 is ASCII
#define BTA_PBS_REALM_CHARSET 0

// Specifies whether or not client's user id is required during obex
// authentication
#define BTA_PBS_USERID_REQ FALSE

static const tBTA_PBS_CFG bta_pbs_cfg = {
    BTA_PBS_REALM_CHARSET,  // realm_charset: Server only
    BTA_PBS_USERID_REQ,     // userid_req: Server only
    (BTA_PBS_SUPF_DOWNLOAD | BTA_PBS_SURF_BROWSE),  // supported_features
    BTA_PBS_REPOSIT_LOCAL,                          // supported_repositories
};

// object format lookup table
#define OBEX_PUSH_NUM_FORMATS 7

static const tBTA_OP_FMT bta_ops_obj_fmt[OBEX_PUSH_NUM_FORMATS] = {
    BTA_OP_VCARD21_FMT, BTA_OP_VCARD30_FMT, BTA_OP_VCAL_FMT, BTA_OP_ICAL_FMT,
    BTA_OP_VNOTE_FMT,   BTA_OP_VMSG_FMT,    BTA_OP_OTHER_FMT};

// TODO(jtgans): Figure out if we actually need this define. This is ifndef
// defined in bt_target.h, but nowhere else, so right now, unless something
// overrides this before bt_target.h sets it, it will always be bt_target.h's
// version.
#ifndef BTUI_OPS_FORMATS
#define BTUI_OPS_FORMATS                                          \
  (BTA_OP_VCARD21_MASK | BTA_OP_VCARD30_MASK | BTA_OP_VCAL_MASK | \
   BTA_OP_ICAL_MASK | BTA_OP_VNOTE_MASK | BTA_OP_VMSG_MASK | BTA_OP_ANY_MASK)
#endif

#define RESERVED_SCN_PBS 19
#define RESERVED_SCN_OPS 12

#define UUID_MAX_LENGTH 16

#define SPP_PROFILE_VERSION 0x0102

// Adds a protocol list and service name (if provided) to an SDP record given by
// |sdp_handle|, and marks it as browseable. This is a shortcut for defining a
// set of protocols that includes L2CAP, RFCOMM, and optionally OBEX. If
// |with_obex| is |true|, then an additional OBEX protocol UUID will be included
// at the end of the protocol list.
//
// Returns true if successful, otherwise false.
static bool create_base_record(const uint32_t sdp_handle, const char* name,
                               const uint16_t channel, const bool with_obex) {
  APPL_TRACE_DEBUG("create_base_record: scn: %d, name: %s, with_obex: %d",
                   channel, name, with_obex);

  // Setup the protocol list and add it.
  tSDP_PROTOCOL_ELEM proto_list[SDP_MAX_LIST_ELEMS];
  int num_proto_elements = with_obex ? 3 : 2;

  memset(proto_list, 0, num_proto_elements * sizeof(tSDP_PROTOCOL_ELEM));

  proto_list[0].protocol_uuid = UUID_PROTOCOL_L2CAP;
  proto_list[0].num_params = 0;
  proto_list[1].protocol_uuid = UUID_PROTOCOL_RFCOMM;
  proto_list[1].num_params = 1;
  proto_list[1].params[0] = channel;

  if (with_obex) {
    proto_list[2].protocol_uuid = UUID_PROTOCOL_OBEX;
    proto_list[2].num_params = 0;
  }

  // Mark the service as browseable.
  uint16_t list = UUID_SERVCLASS_PUBLIC_BROWSE_GROUP;

  const char* stage = "protocol_list";
  if (!SDP_AddProtocolList(sdp_handle, num_proto_elements, proto_list))
    goto error;

  // Add the name to the SDP record.
  if (name[0] != '\0') {
    stage = "service_name";
    if (!SDP_AddAttribute(sdp_handle, ATTR_ID_SERVICE_NAME, TEXT_STR_DESC_TYPE,
                          (uint32_t)strlen(name), (uint8_t*)name))
      goto error;
  }

  stage = "browseable";
  if (!SDP_AddUuidSequence(sdp_handle, ATTR_ID_BROWSE_GROUP_LIST, 1, &list))
    goto error;

  APPL_TRACE_DEBUG(
      "create_base_record: successfully created base service "
      "record, handle: 0x%08x, scn: %d, name: %s, with_obex: %d",
      sdp_handle, channel, name, with_obex);
  return true;

error:
  APPL_TRACE_ERROR(
      "create_base_record: failed to create base service "
      "record, stage: %s, scn: %d, name: %s, with_obex: %d",
      stage, channel, name, with_obex);
  return false;
}

// Registers a service with the given |name|, |uuid|, and |channel| in the SDP
// database as a generic L2CAP RFCOMM protocol, storing its |uuid| as a service
// class sequence.
static int add_sdp_by_uuid(const char* name, const Uuid& uuid,
                           const uint16_t channel) {
  APPL_TRACE_DEBUG("%s: uuid: %s, scn: %d, service_name: %s", __func__,
                   uuid.ToString().c_str(), channel, name);

  uint32_t handle = SDP_CreateRecord();
  if (handle == 0) {
    APPL_TRACE_ERROR(
        "%s: failed to create sdp record, scn: %d, service_name: %s", __func__,
        channel, name);
    return 0;
  }

  // Convert the |uuid| into a big-endian representation and add it as a
  // sequence.
  uint8_t type = UUID_DESC_TYPE;
  uint8_t type_len = UUID_MAX_LENGTH;
  uint8_t type_buf[48];
  // Store the address of type buf in a pointer on the stack, so we can pass
  // a double pointer to SDP_AddSequence
  uint8_t* type_buf_ptr = type_buf;
  uint8_t* tmp = type_buf;

  // Create the base SDP record.
  const char* stage = "create_base_record";
  if (!create_base_record(handle, name, channel, false /* with_obex */))
    goto error;

  // Do the conversion to big-endian -- tmp is only used to iterate through the
  // UUID array in the macro and serves no other purpose as the conversion
  // macros are not hygenic.

  { ARRAY_TO_BE_STREAM(tmp, uuid.To128BitBE().data(), UUID_MAX_LENGTH); }

  stage = "service_class_sequence";
  if (!SDP_AddSequence(handle, (uint16_t)ATTR_ID_SERVICE_CLASS_ID_LIST, 1,
                       &type, &type_len, &type_buf_ptr))
    goto error;

  APPL_TRACE_DEBUG(
      "%s: service registered successfully, service_name: %s, handle: 0x%08x",
      __func__, name, handle);
  return handle;

error:
  SDP_DeleteRecord(handle);
  APPL_TRACE_ERROR("%s: failed to register service stage: %s, service_name: %s",
                   __func__, stage, name);
  return 0;
}

// Registers a service with the given |name| and |channel| in the SDP
// database as a PBAP protocol.
static int add_pbap_sdp(const char* name, const int channel) {
  APPL_TRACE_DEBUG("add_pbap_sdp: scn %d, service_name %s", channel, name);

  uint32_t handle = SDP_CreateRecord();
  if (handle == 0) {
    APPL_TRACE_ERROR(
        "add_pbap_sdp: failed to create sdp record, "
        "service_name: %s",
        name);
    return 0;
  }

  uint16_t service = UUID_SERVCLASS_PBAP_PSE;

  // Create the base SDP record.
  const char* stage = "create_base_record";
  if (!create_base_record(handle, name, channel, true /* with_obex */))
    goto error;

  // Add service class
  stage = "service_class";
  if (!SDP_AddServiceClassIdList(handle, 1, &service)) goto error;

  // Add in the phone access descriptor
  stage = "profile_descriptor_list";
  if (!SDP_AddProfileDescriptorList(handle, UUID_SERVCLASS_PHONE_ACCESS,
                                    BTA_PBS_DEFAULT_VERSION))
    goto error;

  // Set up our supported repositories
  stage = "supported_repositories";
  if (!SDP_AddAttribute(handle, ATTR_ID_SUPPORTED_REPOSITORIES, UINT_DESC_TYPE,
                        1, (uint8_t*)&bta_pbs_cfg.supported_repositories))
    goto error;

  // Notify the system that we've got a new service class UUID.
  bta_sys_add_uuid(UUID_SERVCLASS_PBAP_PSE);
  APPL_TRACE_DEBUG(
      "add_pbap_sdp: service registered successfully, "
      "service_name: %s, handle: 0x%08x",
      name, handle);

  return handle;

error:
  SDP_DeleteRecord(handle);
  APPL_TRACE_ERROR(
      "add_pbap_sdp: failed to register PBAP service, stage: %s, "
      "service_name: %s",
      stage, name);
  return 0;
}
// Registers a service with the given |name| and |channel| as an OBEX Push
// protocol.
static int add_ops_sdp(const char* name, const int channel) {
  APPL_TRACE_DEBUG("add_ops_sdp: scn %d, service_name %s", channel, name);

  uint32_t handle = SDP_CreateRecord();
  if (handle == 0) {
    APPL_TRACE_ERROR(
        "add_ops_sdp: failed to create sdp record, "
        "service_name: %s",
        name);
    return 0;
  }

  // Add sequence for supported types.
  uint8_t desc_type[OBEX_PUSH_NUM_FORMATS];
  uint8_t type_len[OBEX_PUSH_NUM_FORMATS];
  uint8_t* type_value[OBEX_PUSH_NUM_FORMATS];
  uint8_t j = 0;

  uint16_t service = UUID_SERVCLASS_OBEX_OBJECT_PUSH;
  tBTA_UTL_COD cod;

  // Create the base SDP record.
  const char* stage = "create_base_record";
  if (!create_base_record(handle, name, channel, true /* with_obex */))
    goto error;

  // Add service class.
  stage = "service_class";
  if (!SDP_AddServiceClassIdList(handle, 1, &service)) goto error;

  // Add the OBEX push profile descriptor.
  stage = "profile_descriptor_list";
  if (!SDP_AddProfileDescriptorList(handle, UUID_SERVCLASS_OBEX_OBJECT_PUSH,
                                    0x0100))
    goto error;

  for (int i = 0; i < OBEX_PUSH_NUM_FORMATS; i++) {
    if ((BTUI_OPS_FORMATS >> i) & 1) {
      type_value[j] = (uint8_t*)(&bta_ops_obj_fmt[i]);
      desc_type[j] = UINT_DESC_TYPE;
      type_len[j++] = 1;
    }
  }

  stage = "supported_types";
  if (!SDP_AddSequence(handle, (uint16_t)ATTR_ID_SUPPORTED_FORMATS_LIST, j,
                       desc_type, type_len, type_value))
    goto error;

  // Set class of device.
  cod.service = BTM_COD_SERVICE_OBJ_TRANSFER;
  stage = "class_of_device";
  if (!utl_set_device_class(&cod, BTA_UTL_SET_COD_SERVICE_CLASS)) goto error;

  // Notify the system that we've got a new service class UUID.
  bta_sys_add_uuid(UUID_SERVCLASS_OBEX_OBJECT_PUSH);
  APPL_TRACE_DEBUG(
      "ad_maps_sdp: service registered successfully, "
      "service_name: %s, handle 0x%08x)",
      name, handle);

  return handle;

error:
  SDP_DeleteRecord(handle);
  APPL_TRACE_ERROR(
      "add_ops_sdp: failed to register OPS service, "
      "stage: %s, service_name: %s",
      stage, name);
  return 0;
}

// Registers a service with the given |name| and |channel| as a serial port
// profile protocol.
static int add_spp_sdp(const char* name, const int channel) {
  APPL_TRACE_DEBUG("add_spp_sdp: scn %d, service_name %s", channel, name);

  int handle = SDP_CreateRecord();
  if (handle == 0) {
    APPL_TRACE_ERROR(
        "add_spp_sdp: failed to create sdp record, "
        "service_name: %s",
        name);
    return 0;
  }

  // Create the base SDP record.
  const char* stage = "create_base_record";
  uint16_t service = UUID_SERVCLASS_SERIAL_PORT;

  if (!create_base_record(handle, name, channel, false /* with_obex */))
    goto error;

  stage = "service_class";
  if (!SDP_AddServiceClassIdList(handle, 1, &service)) goto error;

  stage = "profile_descriptor_list";
  if (!SDP_AddProfileDescriptorList(handle, UUID_SERVCLASS_SERIAL_PORT,
                                    SPP_PROFILE_VERSION))
    goto error;

  APPL_TRACE_DEBUG(
      "add_spp_sdp: service registered successfully, "
      "service_name: %s, handle 0x%08x)",
      name, handle);

  return handle;

error:
  SDP_DeleteRecord(handle);
  APPL_TRACE_ERROR(
      "add_spp_sdp: failed to register SPP service, "
      "stage: %s, service_name: %s",
      stage, name);
  return 0;
}

// Adds an RFCOMM SDP record for a service with the given |name|, |uuid|, and
// |channel|. This function attempts to identify the type of the service based
// upon its |uuid|, and will override the |channel| with a reserved channel
// number if the |uuid| matches one of the preregistered bluez SDP records.
static int add_rfc_sdp_by_uuid(const char* name, const Uuid& uuid,
                               const int channel) {
  APPL_TRACE_DEBUG("%s: uuid: %s, service_name: %s, channel: %d", __func__,
                   uuid.ToString().c_str(), name, channel);

  /*
   * Bluetooth Socket API relies on having preregistered bluez sdp records for
   * HSAG, HFAG, OPP & PBAP that are mapped to rc chan 10, 11,12 & 19. Today
   * HSAG and HFAG is routed to BRCM AG and are not using BT socket API so for
   * now we will need to support OPP and PBAP to enable 3rd party developer apps
   * running on BRCM Android.
   *
   * To do this we will check the UUID for the requested service and mimic the
   * SDP records of bluez upon reception.  See functions add_opush() and
   * add_pbap() in sdptool.c for actual records.
   */

  int final_channel = get_reserved_rfc_channel(uuid);

  if (final_channel == -1) {
    final_channel = channel;
  }

  int handle = 0;

  if (uuid == UUID_OBEX_OBJECT_PUSH) {
    handle = add_ops_sdp(name, final_channel);
  } else if (uuid == UUID_PBAP_PSE) {
    // PBAP Server is always channel 19
    handle = add_pbap_sdp(name, final_channel);
  } else if (uuid == UUID_SPP) {
    handle = add_spp_sdp(name, final_channel);
  } else if (uuid == UUID_MAP_MAS) {
    // Record created by new SDP create record interface
    handle = 0xff;
  } else {
    handle = add_sdp_by_uuid(name, uuid, final_channel);
  }

  return handle;
}

bool is_reserved_rfc_channel(const int channel) {
  switch (channel) {
    case RESERVED_SCN_PBS:
    case RESERVED_SCN_OPS:
      return true;
  }

  return false;
}

int get_reserved_rfc_channel(const bluetooth::Uuid& uuid) {
  if (uuid == UUID_PBAP_PSE) {
    return RESERVED_SCN_PBS;
  } else if (uuid == UUID_OBEX_OBJECT_PUSH) {
    return RESERVED_SCN_OPS;
  }

  return -1;
}

// Adds an SDP record to the SDP database using the given |name|, |uuid|, and
// |channel|. Note that if the |uuid| is empty, the |uuid| will be set based
// upon the |channel| passed in.
int add_rfc_sdp_rec(const char* name, Uuid uuid, const int channel) {
  if (uuid.IsEmpty()) {
    switch (channel) {
      case RESERVED_SCN_PBS:  // PBAP Reserved port
        uuid = UUID_PBAP_PSE;
        break;

      case RESERVED_SCN_OPS:
        uuid = UUID_OBEX_OBJECT_PUSH;
        break;

      default:
        uuid = UUID_SPP;
        break;
    }
  }

  return add_rfc_sdp_by_uuid(name, uuid, channel);
}

// Deletes an SDP record with the given |handle|.
void del_rfc_sdp_rec(int handle) {
  APPL_TRACE_DEBUG("del_rfc_sdp_rec: handle:0x%x", handle);

  if ((handle != -1) && (handle != 0)) BTA_JvDeleteRecord(handle);
}
