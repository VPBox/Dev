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

#include "l2c_api.h"

namespace bluetooth {
namespace l2cap {

class L2capInterface {
 public:
  virtual uint16_t Register(uint16_t psm, tL2CAP_APPL_INFO* p_cb_info,
                            bool enable_snoop) = 0;
  virtual uint16_t ConnectRequest(uint16_t psm, const RawAddress& bd_addr) = 0;
  virtual bool ConnectResponse(const RawAddress& bd_addr, uint8_t id,
                               uint16_t lcid, uint16_t result,
                               uint16_t status) = 0;
  virtual bool DisconnectRequest(uint16_t cid) = 0;
  virtual bool DisconnectResponse(uint16_t cid) = 0;
  virtual bool ConfigRequest(uint16_t cid, tL2CAP_CFG_INFO* p_cfg) = 0;
  virtual bool ConfigResponse(uint16_t cid, tL2CAP_CFG_INFO* p_cfg) = 0;
  virtual uint8_t DataWrite(uint16_t cid, BT_HDR* p_data) = 0;
  virtual ~L2capInterface() = default;
};

class MockL2capInterface : public L2capInterface {
 public:
  MOCK_METHOD3(Register, uint16_t(uint16_t psm, tL2CAP_APPL_INFO* p_cb_info,
                                  bool enable_snoop));
  MOCK_METHOD2(ConnectRequest,
               uint16_t(uint16_t psm, const RawAddress& bd_addr));
  MOCK_METHOD5(ConnectResponse,
               bool(const RawAddress& bd_addr, uint8_t id, uint16_t lcid,
                    uint16_t result, uint16_t status));
  MOCK_METHOD1(DisconnectRequest, bool(uint16_t cid));
  MOCK_METHOD1(DisconnectResponse, bool(uint16_t cid));
  MOCK_METHOD2(ConfigRequest, bool(uint16_t cid, tL2CAP_CFG_INFO* p_cfg));
  MOCK_METHOD2(ConfigResponse, bool(uint16_t cid, tL2CAP_CFG_INFO* p_cfg));
  MOCK_METHOD2(DataWrite, uint8_t(uint16_t cid, BT_HDR* p_data));
};

/**
 * Set the {@link MockL2capInterface} for testing
 *
 * @param mock_l2cap_interface pointer to mock l2cap interface, could be null
 */
void SetMockInterface(MockL2capInterface* mock_l2cap_interface);

}  // namespace l2cap
}  // namespace bluetooth
