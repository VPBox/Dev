/******************************************************************************
 *
 *  Copyright 2018 The Android Open Source Project
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

#include "mock_btm_layer.h"

static bluetooth::manager::MockBtmSecurityInternalInterface*
    btm_security_internal_interface = nullptr;

void bluetooth::manager::SetMockSecurityInternalInterface(
    MockBtmSecurityInternalInterface* mock_btm_security_internal_interface) {
  btm_security_internal_interface = mock_btm_security_internal_interface;
}

void btm_sec_abort_access_req(const RawAddress& bd_addr) {
  btm_security_internal_interface->AbortAccessRequest(bd_addr);
}

tBTM_STATUS btm_sec_mx_access_request(const RawAddress& bd_addr, uint16_t psm,
                                      bool is_originator, uint32_t mx_proto_id,
                                      uint32_t mx_chan_id,
                                      tBTM_SEC_CALLBACK* p_callback,
                                      void* p_ref_data) {
  return btm_security_internal_interface->MultiplexingProtocolAccessRequest(
      bd_addr, psm, is_originator, mx_proto_id, mx_chan_id, p_callback,
      p_ref_data);
}

uint16_t btm_get_max_packet_size(const RawAddress& addr) {
  return RFCOMM_DEFAULT_MTU;
}