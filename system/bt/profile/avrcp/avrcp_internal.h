/*
 * Copyright 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "stack/include/avrc_api.h"
#include "stack/include/sdp_api.h"

/**
 * Wrapper classes for the API functions currently defined in "system/bt/stack".
 * ConnectionHandler and AvrcpDevice both use this interface to manage AVRCP
 * SDP, connections, and packets. We use these intermediate interfaces instead
 * of calling the functions directly in order to make mocking and testing
 * easier.
 */
// TODO (apanicke): Update the api's in "system/bt/stack" so that no wrapper is
// required
class AvrcpInterface {
 public:
  virtual uint16_t AddRecord(uint16_t service_uuid, const char* p_service_name,
                             const char* p_provider_name, uint16_t categories,
                             uint32_t sdp_handle, bool browse_supported,
                             uint16_t profile_version) = 0;

  virtual uint16_t FindService(uint16_t service_uuid, const RawAddress& bd_addr,
                               tAVRC_SDP_DB_PARAMS* p_db,
                               tAVRC_FIND_CBACK p_cback) = 0;

  virtual uint16_t Open(uint8_t* p_handle, tAVRC_CONN_CB* p_ccb,
                        const RawAddress& bd_addr) = 0;

  virtual uint16_t OpenBrowse(uint8_t handle, uint8_t conn_role) = 0;

  virtual uint16_t GetPeerMtu(uint8_t handle) = 0;

  virtual uint16_t GetBrowseMtu(uint8_t handle) = 0;

  virtual uint16_t Close(uint8_t handle) = 0;

  virtual uint16_t CloseBrowse(uint8_t handle) = 0;

  virtual uint16_t MsgReq(uint8_t handle, uint8_t label, uint8_t ctype,
                          BT_HDR* p_pkt) = 0;

  virtual ~AvrcpInterface() = default;
};

class SdpInterface {
 public:
  virtual bool InitDiscoveryDb(tSDP_DISCOVERY_DB* a, uint32_t b, uint16_t c,
                               const bluetooth::Uuid* d, uint16_t e,
                               uint16_t* f) = 0;

  virtual bool ServiceSearchAttributeRequest(const RawAddress& a,
                                             tSDP_DISCOVERY_DB* b,
                                             tSDP_DISC_CMPL_CB* c) = 0;

  virtual tSDP_DISC_REC* FindServiceInDb(tSDP_DISCOVERY_DB* a, uint16_t b,
                                         t_sdp_disc_rec* c) = 0;

  virtual tSDP_DISC_ATTR* FindAttributeInRec(t_sdp_disc_rec* a, uint16_t b) = 0;

  virtual bool FindProfileVersionInRec(t_sdp_disc_rec* a, uint16_t b,
                                       uint16_t* c) = 0;

  virtual ~SdpInterface() = default;
};

class A2dpInterface {
 public:
  virtual RawAddress active_peer() = 0;
  virtual bool is_peer_in_silence_mode(const RawAddress& peer_address) = 0;

  virtual ~A2dpInterface() = default;
};
