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
#pragma once

#include <gmock/gmock.h>

#include "btm_int.h"

namespace bluetooth {
namespace manager {

class BtmSecurityInternalInterface {
 public:
  virtual void AbortAccessRequest(const RawAddress& bd_addr) = 0;
  virtual tBTM_STATUS MultiplexingProtocolAccessRequest(
      const RawAddress& bd_addr, uint16_t psm, bool is_originator,
      uint32_t mx_proto_id, uint32_t mx_chan_id, tBTM_SEC_CALLBACK* p_callback,
      void* p_ref_data) = 0;
  virtual ~BtmSecurityInternalInterface() = default;
};

class MockBtmSecurityInternalInterface : public BtmSecurityInternalInterface {
 public:
  MOCK_METHOD1(AbortAccessRequest, void(const RawAddress& bd_addr));
  MOCK_METHOD7(MultiplexingProtocolAccessRequest,
               tBTM_STATUS(const RawAddress& bd_addr, uint16_t psm,
                           bool is_originator, uint32_t mx_proto_id,
                           uint32_t mx_chan_id, tBTM_SEC_CALLBACK* p_callback,
                           void* p_ref_data));
};

/**
 * Set the {@link MockBtmSecurityInternalInterface} for testing
 *
 * @param mock_btm_security_internal_interface pointer to mock btm security
 * internal interface, could be null
 */
void SetMockSecurityInternalInterface(
    MockBtmSecurityInternalInterface* mock_btm_security_internal_interface);

}  // namespace manager
}  // namespace bluetooth