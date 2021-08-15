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
#include "mock_l2cap_layer.h"

static bluetooth::l2cap::MockL2capInterface* l2cap_interface = nullptr;

void bluetooth::l2cap::SetMockInterface(
    MockL2capInterface* mock_l2cap_interface) {
  l2cap_interface = mock_l2cap_interface;
}

uint16_t L2CA_Register(uint16_t psm, tL2CAP_APPL_INFO* p_cb_info,
                       bool enable_snoop) {
  VLOG(1) << __func__ << ": psm=" << psm << ", p_cb_info=" << p_cb_info
          << ", enable_snoop=" << enable_snoop;
  return l2cap_interface->Register(psm, p_cb_info, enable_snoop);
}

uint16_t L2CA_ConnectReq(uint16_t psm, const RawAddress& bd_addr) {
  return l2cap_interface->ConnectRequest(psm, bd_addr);
}

bool L2CA_ConnectRsp(const RawAddress& bd_addr, uint8_t id, uint16_t lcid,
                     uint16_t result, uint16_t status) {
  return l2cap_interface->ConnectResponse(bd_addr, id, lcid, result, status);
}

bool L2CA_DisconnectReq(uint16_t cid) {
  return l2cap_interface->DisconnectRequest(cid);
}

bool L2CA_DisconnectRsp(uint16_t cid) {
  return l2cap_interface->DisconnectResponse(cid);
}

bool L2CA_ConfigReq(uint16_t cid, tL2CAP_CFG_INFO* p_cfg) {
  return l2cap_interface->ConfigRequest(cid, p_cfg);
}

bool L2CA_ConfigRsp(uint16_t cid, tL2CAP_CFG_INFO* p_cfg) {
  return l2cap_interface->ConfigResponse(cid, p_cfg);
}

uint8_t L2CA_DataWrite(uint16_t cid, BT_HDR* p_data) {
  return l2cap_interface->DataWrite(cid, p_data);
}
