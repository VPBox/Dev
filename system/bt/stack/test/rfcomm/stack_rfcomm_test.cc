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

#include <base/logging.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "bt_types.h"
#include "btm_api.h"
#include "l2c_api.h"
#include "osi/include/osi.h"
#include "port_api.h"

#include "btm_int.h"
#include "rfc_int.h"

#include "mock_btm_layer.h"
#include "mock_l2cap_layer.h"
#include "stack_rfcomm_test_utils.h"
#include "stack_test_packet_utils.h"

std::string DumpByteBufferToString(uint8_t* p_data, size_t len) {
  std::stringstream str;
  str.setf(std::ios_base::hex, std::ios::basefield);
  str.setf(std::ios_base::uppercase);
  str.fill('0');
  for (size_t i = 0; i < len; ++i) {
    str << std::setw(2) << static_cast<uint16_t>(p_data[i]);
    str << " ";
  }
  return str.str();
}

std::string DumpBtHdrToString(BT_HDR* p_hdr) {
  uint8_t* p_hdr_data = p_hdr->data + p_hdr->offset;
  return DumpByteBufferToString(p_hdr_data, p_hdr->len);
}

void PrintTo(BT_HDR* value, ::std::ostream* os) {
  *os << DumpBtHdrToString(value);
}

void PrintTo(tL2CAP_CFG_INFO* value, ::std::ostream* os) {
  *os << DumpByteBufferToString((uint8_t*)value, sizeof(tL2CAP_CFG_INFO));
}

namespace {

using testing::_;
using testing::DoAll;
using testing::Return;
using testing::Test;
using testing::StrictMock;
using testing::SaveArg;
using testing::SaveArgPointee;
using testing::Pointee;
using testing::StrEq;
using testing::NotNull;

using bluetooth::CreateL2capDataPacket;
using bluetooth::CreateAclPacket;
using bluetooth::AllocateWrappedIncomingL2capAclPacket;
using bluetooth::AllocateWrappedOutgoingL2capAclPacket;

using bluetooth::rfcomm::GetDlci;
using bluetooth::rfcomm::GetAddressField;
using bluetooth::rfcomm::GetControlField;
using bluetooth::rfcomm::CreateMccPnFrame;
using bluetooth::rfcomm::CreateMccMscFrame;
using bluetooth::rfcomm::CreateMultiplexerControlFrame;
using bluetooth::rfcomm::CreateRfcommPacket;
using bluetooth::rfcomm::CreateQuickDataPacket;
using bluetooth::rfcomm::CreateQuickPnPacket;
using bluetooth::rfcomm::CreateQuickSabmPacket;
using bluetooth::rfcomm::CreateQuickUaPacket;
using bluetooth::rfcomm::CreateQuickMscPacket;

MATCHER_P(PointerMemoryEqual, ptr,
          DumpByteBufferToString((uint8_t*)ptr, sizeof(*ptr))) {
  return memcmp(arg, ptr, sizeof(*ptr)) == 0;
}

MATCHER_P(BtHdrEqual, expected, DumpBtHdrToString(expected)) {
  auto arg_hdr = static_cast<BT_HDR*>(arg);
  uint8_t* arg_data = arg_hdr->data + arg_hdr->offset;
  auto expected_hdr = static_cast<BT_HDR*>(expected);
  uint8_t* expected_data = expected_hdr->data + expected_hdr->offset;
  return memcmp(arg_data, expected_data, expected_hdr->len) == 0;
}

bluetooth::rfcomm::MockRfcommCallback* rfcomm_callback = nullptr;

void port_mgmt_cback_0(uint32_t code, uint16_t port_handle) {
  rfcomm_callback->PortManagementCallback(code, port_handle, 0);
}

void port_mgmt_cback_1(uint32_t code, uint16_t port_handle) {
  rfcomm_callback->PortManagementCallback(code, port_handle, 1);
}

void port_event_cback_0(uint32_t code, uint16_t port_handle) {
  rfcomm_callback->PortEventCallback(code, port_handle, 0);
}

void port_event_cback_1(uint32_t code, uint16_t port_handle) {
  rfcomm_callback->PortEventCallback(code, port_handle, 1);
}

RawAddress GetTestAddress(int index) {
  CHECK_LT(index, UINT8_MAX);
  RawAddress result = {
      {0xAA, 0x00, 0x11, 0x22, 0x33, static_cast<uint8_t>(index)}};
  return result;
}

class StackRfcommTest : public Test {
 public:
  void StartServerPort(uint16_t uuid, uint8_t scn, uint16_t mtu,
                       tPORT_CALLBACK* management_callback,
                       tPORT_CALLBACK* event_callback,
                       uint16_t* server_handle) {
    VLOG(1) << "Step 1";
    ASSERT_EQ(RFCOMM_CreateConnection(uuid, scn, true, mtu, RawAddress::kAny,
                                      server_handle, management_callback),
              PORT_SUCCESS);
    ASSERT_EQ(PORT_SetEventMask(*server_handle, PORT_EV_RXCHAR), PORT_SUCCESS);
    ASSERT_EQ(PORT_SetEventCallback(*server_handle, event_callback),
              PORT_SUCCESS);
  }

  void ConnectServerL2cap(const RawAddress& peer_addr, uint16_t acl_handle,
                          uint16_t lcid) {
    VLOG(1) << "Step 1";
    // Remote device connect to this channel, we shall accept
    static const uint8_t cmd_id = 0x07;
    EXPECT_CALL(l2cap_interface_,
                ConnectResponse(peer_addr, cmd_id, lcid, L2CAP_CONN_OK, 0));
    tL2CAP_CFG_INFO cfg_req = {.mtu_present = true, .mtu = L2CAP_MTU_SIZE};
    EXPECT_CALL(l2cap_interface_,
                ConfigRequest(lcid, PointerMemoryEqual(&cfg_req)))
        .WillOnce(Return(true));
    l2cap_appl_info_.pL2CA_ConnectInd_Cb(peer_addr, lcid, BT_PSM_RFCOMM,
                                         cmd_id);

    VLOG(1) << "Step 2";
    // MTU configuration is done
    cfg_req.mtu_present = false;
    l2cap_appl_info_.pL2CA_ConfigCfm_Cb(lcid, &cfg_req);

    VLOG(1) << "Step 3";
    // Remote device also ask to configure MTU size
    EXPECT_CALL(l2cap_interface_,
                ConfigResponse(lcid, PointerMemoryEqual(&cfg_req)))
        .WillOnce(Return(true));
    l2cap_appl_info_.pL2CA_ConfigInd_Cb(lcid, &cfg_req);

    VLOG(1) << "Step 4";
    // Remote device connect to server channel 0
    BT_HDR* sabm_channel_0 = AllocateWrappedIncomingL2capAclPacket(
        CreateQuickSabmPacket(RFCOMM_MX_DLCI, lcid, acl_handle));
    BT_HDR* ua_channel_0 = AllocateWrappedOutgoingL2capAclPacket(
        CreateQuickUaPacket(RFCOMM_MX_DLCI, lcid, acl_handle));
    EXPECT_CALL(l2cap_interface_, DataWrite(lcid, BtHdrEqual(ua_channel_0)))
        .WillOnce(Return(L2CAP_DW_SUCCESS));
    // Packet should be freed by RFCOMM
    l2cap_appl_info_.pL2CA_DataInd_Cb(lcid, sabm_channel_0);
    osi_free(ua_channel_0);
  }

  void ConnectServerPort(const RawAddress& peer_addr, uint16_t port_handle,
                         uint8_t scn, uint16_t mtu, uint16_t acl_handle,
                         uint16_t lcid, int port_callback_index) {
    VLOG(1) << "Step 1";
    // Negotiate parameters on scn
    BT_HDR* uih_pn_cmd_from_peer = AllocateWrappedIncomingL2capAclPacket(
        CreateQuickPnPacket(true, GetDlci(false, scn), true, mtu,
                            RFCOMM_PN_CONV_LAYER_CBFC_I >> 4, 0, RFCOMM_K_MAX,
                            lcid, acl_handle));
    BT_HDR* uih_pn_rsp_to_peer = AllocateWrappedOutgoingL2capAclPacket(
        CreateQuickPnPacket(false, GetDlci(false, scn), false, mtu,
                            RFCOMM_PN_CONV_LAYER_CBFC_R >> 4, 0, RFCOMM_K_MAX,
                            lcid, acl_handle));
    EXPECT_CALL(l2cap_interface_,
                DataWrite(lcid, BtHdrEqual(uih_pn_rsp_to_peer)))
        .WillOnce(Return(L2CAP_DW_SUCCESS));
    // uih_pn_cmd_from_peer should be freed by this method
    l2cap_appl_info_.pL2CA_DataInd_Cb(lcid, uih_pn_cmd_from_peer);
    osi_free(uih_pn_rsp_to_peer);

    VLOG(1) << "Step 2";
    // Remote device connect to scn
    tBTM_SEC_CALLBACK* security_callback = nullptr;
    void* p_port = nullptr;
    BT_HDR* sabm_channel_dlci = AllocateWrappedIncomingL2capAclPacket(
        CreateQuickSabmPacket(GetDlci(false, scn), lcid, acl_handle));
    EXPECT_CALL(btm_security_internal_interface_,
                MultiplexingProtocolAccessRequest(peer_addr, BT_PSM_RFCOMM,
                                                  false, BTM_SEC_PROTO_RFCOMM,
                                                  scn, NotNull(), NotNull()))
        .WillOnce(DoAll(SaveArg<5>(&security_callback), SaveArg<6>(&p_port),
                        Return(BTM_SUCCESS)));
    // sabm_channel_dlci should be freed by this method
    l2cap_appl_info_.pL2CA_DataInd_Cb(lcid, sabm_channel_dlci);

    VLOG(1) << "Step 3";
    // Confirm security check should trigger port as connected
    EXPECT_CALL(
        rfcomm_callback_,
        PortManagementCallback(PORT_SUCCESS, port_handle, port_callback_index));
    BT_HDR* ua_channel_dlci = AllocateWrappedOutgoingL2capAclPacket(
        CreateQuickUaPacket(GetDlci(false, scn), lcid, acl_handle));
    EXPECT_CALL(l2cap_interface_, DataWrite(lcid, BtHdrEqual(ua_channel_dlci)))
        .WillOnce(Return(L2CAP_DW_SUCCESS));
    ASSERT_TRUE(security_callback);
    security_callback(&peer_addr, BT_TRANSPORT_BR_EDR, p_port, BTM_SUCCESS);
    osi_free(ua_channel_dlci);

    VLOG(1) << "Step 4";
    // Remote also need to configure its modem signal before we can send data
    BT_HDR* uih_msc_cmd_from_peer = AllocateWrappedIncomingL2capAclPacket(
        CreateQuickMscPacket(true, GetDlci(false, scn), lcid, acl_handle, true,
                             false, true, true, false, true));
    BT_HDR* uih_msc_response_to_peer = AllocateWrappedOutgoingL2capAclPacket(
        CreateQuickMscPacket(false, GetDlci(false, scn), lcid, acl_handle,
                             false, false, true, true, false, true));
    // We also have to do modem configuration ourself
    EXPECT_CALL(l2cap_interface_,
                DataWrite(lcid, BtHdrEqual(uih_msc_response_to_peer)))
        .WillOnce(Return(L2CAP_DW_SUCCESS));
    BT_HDR* uih_msc_cmd_to_peer = AllocateWrappedOutgoingL2capAclPacket(
        CreateQuickMscPacket(false, GetDlci(false, scn), lcid, acl_handle, true,
                             false, true, true, false, true));
    EXPECT_CALL(l2cap_interface_,
                DataWrite(lcid, BtHdrEqual(uih_msc_cmd_to_peer)))
        .WillOnce(Return(L2CAP_DW_SUCCESS));
    // uih_msc_cmd_from_peer should be freed by this method
    l2cap_appl_info_.pL2CA_DataInd_Cb(lcid, uih_msc_cmd_from_peer);
    osi_free(uih_msc_response_to_peer);

    VLOG(1) << "Step 5";
    // modem configuration is done
    BT_HDR* uih_msc_response_from_peer = AllocateWrappedIncomingL2capAclPacket(
        CreateQuickMscPacket(true, GetDlci(false, scn), lcid, acl_handle, false,
                             false, true, true, false, true));
    // uih_msc_response_from_peer should be freed by this method
    l2cap_appl_info_.pL2CA_DataInd_Cb(lcid, uih_msc_response_from_peer);
  }

  void StartClientPort(const RawAddress& peer_bd_addr, uint16_t uuid,
                       uint8_t scn, uint16_t mtu,
                       tPORT_CALLBACK* management_callback,
                       tPORT_CALLBACK* event_callback, uint16_t lcid,
                       uint16_t acl_handle, uint16_t* client_handle,
                       bool is_first_connection) {
    VLOG(1) << "Step 1";
    BT_HDR* uih_pn_channel_3 =
        AllocateWrappedOutgoingL2capAclPacket(CreateQuickPnPacket(
            true, GetDlci(false, scn), true, mtu, RFCOMM_PN_CONV_LAYER_TYPE_1,
            RFCOMM_PN_PRIORITY_0, RFCOMM_K, lcid, acl_handle));
    if (is_first_connection) {
      EXPECT_CALL(l2cap_interface_, ConnectRequest(BT_PSM_RFCOMM, peer_bd_addr))
          .WillOnce(Return(lcid));
    } else {
      EXPECT_CALL(l2cap_interface_,
                  DataWrite(lcid, BtHdrEqual(uih_pn_channel_3)))
          .WillOnce(Return(L2CAP_DW_SUCCESS));
    }
    ASSERT_EQ(RFCOMM_CreateConnection(uuid, scn, false, mtu, peer_bd_addr,
                                      client_handle, management_callback),
              PORT_SUCCESS);
    ASSERT_EQ(PORT_SetEventMask(*client_handle, PORT_EV_RXCHAR), PORT_SUCCESS);
    ASSERT_EQ(PORT_SetEventCallback(*client_handle, event_callback),
              PORT_SUCCESS);
    osi_free(uih_pn_channel_3);
  }

  void TestConnectClientPortL2cap(uint16_t acl_handle, uint16_t lcid) {
    VLOG(1) << "Step 1";
    // Send configuration request when L2CAP connect is succsseful
    tL2CAP_CFG_INFO cfg_req = {.mtu_present = true, .mtu = L2CAP_MTU_SIZE};
    EXPECT_CALL(l2cap_interface_,
                ConfigRequest(lcid, PointerMemoryEqual(&cfg_req)))
        .WillOnce(Return(true));
    l2cap_appl_info_.pL2CA_ConnectCfm_Cb(lcid, L2CAP_CONN_OK);

    VLOG(1) << "Step 2";
    // Remote device confirms our configuration request
    cfg_req.mtu_present = false;
    l2cap_appl_info_.pL2CA_ConfigCfm_Cb(lcid, &cfg_req);

    VLOG(1) << "Step 3";
    // Remote device also asks to configure MTU
    // Once configuration is done, we connect to multiplexer control channel 0
    EXPECT_CALL(l2cap_interface_,
                ConfigResponse(lcid, PointerMemoryEqual(&cfg_req)))
        .WillOnce(Return(true));
    // multiplexer control channel's DLCI is always 0
    BT_HDR* sabm_channel_0 = AllocateWrappedOutgoingL2capAclPacket(
        CreateQuickSabmPacket(RFCOMM_MX_DLCI, lcid, acl_handle));
    EXPECT_CALL(l2cap_interface_, DataWrite(lcid, BtHdrEqual(sabm_channel_0)))
        .WillOnce(Return(L2CAP_DW_SUCCESS));
    l2cap_appl_info_.pL2CA_ConfigInd_Cb(lcid, &cfg_req);
    osi_free(sabm_channel_0);
  }

  void ConnectClientPort(const RawAddress& peer_addr, uint16_t port_handle,
                         uint8_t scn, uint16_t mtu, uint16_t acl_handle,
                         uint16_t lcid, int port_callback_index,
                         bool is_first_connection) {
    VLOG(1) << "Step 1";
    if (is_first_connection) {
      VLOG(1) << "Step 1.5";
      // Once remote accept multiplexer control channel 0
      // We change to desired channel on non-initiating device (remote device)
      BT_HDR* ua_channel_0 = AllocateWrappedIncomingL2capAclPacket(
          CreateQuickUaPacket(RFCOMM_MX_DLCI, lcid, acl_handle));
      BT_HDR* uih_pn_channel_3 =
          AllocateWrappedOutgoingL2capAclPacket(CreateQuickPnPacket(
              true, GetDlci(false, scn), true, mtu,
              RFCOMM_PN_CONV_LAYER_CBFC_I >> 4, RFCOMM_PN_PRIORITY_0,
              RFCOMM_K_MAX, lcid, acl_handle));
      EXPECT_CALL(l2cap_interface_,
                  DataWrite(lcid, BtHdrEqual(uih_pn_channel_3)))
          .WillOnce(Return(L2CAP_DW_SUCCESS));
      l2cap_appl_info_.pL2CA_DataInd_Cb(lcid, ua_channel_0);
      osi_free(uih_pn_channel_3);
    }

    VLOG(1) << "Step 2";
    // Once remote accept service channel change, we start security procedure
    BT_HDR* uih_pn_channel_3_accept =
        AllocateWrappedIncomingL2capAclPacket(CreateQuickPnPacket(
            false, GetDlci(false, scn), false, mtu,
            RFCOMM_PN_CONV_LAYER_CBFC_I >> 4, RFCOMM_PN_PRIORITY_0,
            RFCOMM_K_MAX, lcid, acl_handle));
    tBTM_SEC_CALLBACK* security_callback = nullptr;
    void* p_port = nullptr;
    EXPECT_CALL(btm_security_internal_interface_,
                MultiplexingProtocolAccessRequest(peer_addr, BT_PSM_RFCOMM,
                                                  true, BTM_SEC_PROTO_RFCOMM,
                                                  scn, NotNull(), NotNull()))
        .WillOnce(DoAll(SaveArg<5>(&security_callback), SaveArg<6>(&p_port),
                        Return(BTM_SUCCESS)));
    l2cap_appl_info_.pL2CA_DataInd_Cb(lcid, uih_pn_channel_3_accept);

    VLOG(1) << "Step 3";
    // Once security procedure is done, we officially connect to target scn
    BT_HDR* sabm_channel_3 = AllocateWrappedOutgoingL2capAclPacket(
        CreateQuickSabmPacket(GetDlci(false, scn), lcid, acl_handle));
    EXPECT_CALL(l2cap_interface_, DataWrite(lcid, BtHdrEqual(sabm_channel_3)))
        .WillOnce(Return(L2CAP_DW_SUCCESS));
    ASSERT_TRUE(security_callback);
    security_callback(&peer_addr, BT_TRANSPORT_BR_EDR, p_port, BTM_SUCCESS);
    osi_free(sabm_channel_3);

    VLOG(1) << "Step 4";
    // When target scn is accepted by remote, we need to configure modem signal
    // state beofre using the port
    EXPECT_CALL(
        rfcomm_callback_,
        PortManagementCallback(PORT_SUCCESS, port_handle, port_callback_index));
    BT_HDR* uih_msc_cmd = AllocateWrappedOutgoingL2capAclPacket(
        CreateQuickMscPacket(true, GetDlci(false, scn), lcid, acl_handle, true,
                             false, true, true, false, true));
    EXPECT_CALL(l2cap_interface_, DataWrite(lcid, BtHdrEqual(uih_msc_cmd)))
        .WillOnce(Return(L2CAP_DW_SUCCESS));
    BT_HDR* ua_channel_3 = AllocateWrappedIncomingL2capAclPacket(
        CreateQuickUaPacket(GetDlci(false, scn), lcid, acl_handle));
    l2cap_appl_info_.pL2CA_DataInd_Cb(lcid, ua_channel_3);
    osi_free(uih_msc_cmd);

    VLOG(1) << "Step 5";
    // modem configuration is done
    BT_HDR* uih_msc_response = AllocateWrappedIncomingL2capAclPacket(
        CreateQuickMscPacket(false, GetDlci(false, scn), lcid, acl_handle,
                             false, false, true, true, false, true));
    l2cap_appl_info_.pL2CA_DataInd_Cb(lcid, uih_msc_response);

    VLOG(1) << "Step 6";
    // Remote also need to configure its modem signal before we can send data
    BT_HDR* uih_msc_cmd_from_peer = AllocateWrappedIncomingL2capAclPacket(
        CreateQuickMscPacket(false, GetDlci(false, scn), lcid, acl_handle, true,
                             false, true, true, false, true));
    BT_HDR* uih_msc_response_to_peer = AllocateWrappedOutgoingL2capAclPacket(
        CreateQuickMscPacket(true, GetDlci(false, scn), lcid, acl_handle, false,
                             false, true, true, false, true));
    EXPECT_CALL(l2cap_interface_,
                DataWrite(lcid, BtHdrEqual(uih_msc_response_to_peer)))
        .WillOnce(Return(L2CAP_DW_SUCCESS));
    l2cap_appl_info_.pL2CA_DataInd_Cb(lcid, uih_msc_cmd_from_peer);
    osi_free(uih_msc_response_to_peer);
  }

  void SendAndVerifyOutgoingTransmission(uint16_t port_handle,
                                         bool is_initiator, uint8_t scn,
                                         bool cr, const std::string& message,
                                         int credits, uint16_t acl_handle,
                                         uint16_t lcid) {
    VLOG(1) << "Step 1";
    BT_HDR* data_packet = AllocateWrappedOutgoingL2capAclPacket(
        CreateQuickDataPacket(GetDlci(is_initiator, scn), cr, lcid, acl_handle,
                              credits, message));
    uint16_t transmitted_length = 0;
    EXPECT_CALL(l2cap_interface_, DataWrite(lcid, BtHdrEqual(data_packet)))
        .WillOnce(Return(L2CAP_DW_SUCCESS));
    ASSERT_EQ(PORT_WriteData(port_handle, message.data(), message.size(),
                             &transmitted_length),
              PORT_SUCCESS);
    ASSERT_EQ(transmitted_length, message.size());
  }

  void ReceiveAndVerifyIncomingTransmission(uint16_t port_handle,
                                            bool is_initiator, uint8_t scn,
                                            bool cr, const std::string& message,
                                            int credits, uint16_t acl_handle,
                                            uint16_t lcid,
                                            int port_callback_index) {
    VLOG(1) << "Step 1";
    BT_HDR* data_packet = AllocateWrappedIncomingL2capAclPacket(
        CreateQuickDataPacket(GetDlci(is_initiator, scn), cr, lcid, acl_handle,
                              credits, message));
    EXPECT_CALL(rfcomm_callback_,
                PortEventCallback(_, port_handle, port_callback_index));
    l2cap_appl_info_.pL2CA_DataInd_Cb(lcid, data_packet);

    VLOG(1) << "Step 2";
    char buffer[L2CAP_MTU_SIZE] = {};
    uint16_t length = 0;
    int status = PORT_ReadData(port_handle, buffer, L2CAP_MTU_SIZE, &length);
    ASSERT_EQ(status, PORT_SUCCESS);
    ASSERT_THAT(buffer, StrEq(message));
  }

 protected:
  void SetUp() override {
    Test::SetUp();
    bluetooth::manager::SetMockSecurityInternalInterface(
        &btm_security_internal_interface_);
    bluetooth::l2cap::SetMockInterface(&l2cap_interface_);
    rfcomm_callback = &rfcomm_callback_;
    EXPECT_CALL(l2cap_interface_, Register(BT_PSM_RFCOMM, _, _))
        .WillOnce(
            DoAll(SaveArgPointee<1>(&l2cap_appl_info_), Return(BT_PSM_RFCOMM)));
    RFCOMM_Init();
    rfc_cb.trace_level = BT_TRACE_LEVEL_DEBUG;
  }

  void TearDown() override {
    rfcomm_callback = nullptr;
    bluetooth::l2cap::SetMockInterface(nullptr);
    bluetooth::manager::SetMockSecurityInternalInterface(nullptr);
    testing::Test::TearDown();
  }
  StrictMock<bluetooth::manager::MockBtmSecurityInternalInterface>
      btm_security_internal_interface_;
  StrictMock<bluetooth::l2cap::MockL2capInterface> l2cap_interface_;
  StrictMock<bluetooth::rfcomm::MockRfcommCallback> rfcomm_callback_;
  tL2CAP_APPL_INFO l2cap_appl_info_;
};

TEST_F(StackRfcommTest, SingleServerConnectionHelloWorld) {
  // Prepare a server channel at kTestChannelNumber0
  static const uint16_t acl_handle = 0x0009;
  static const uint16_t lcid = 0x0054;
  static const uint16_t test_uuid = 0x1112;
  static const uint8_t test_scn = 8;
  static const uint16_t test_mtu = 1600;
  static const RawAddress test_address = GetTestAddress(0);
  uint16_t server_handle = 0;
  ASSERT_NO_FATAL_FAILURE(StartServerPort(test_uuid, test_scn, test_mtu,
                                          port_mgmt_cback_0, port_event_cback_0,
                                          &server_handle));
  ASSERT_NO_FATAL_FAILURE(ConnectServerL2cap(test_address, acl_handle, lcid));
  ASSERT_NO_FATAL_FAILURE(ConnectServerPort(
      test_address, server_handle, test_scn, test_mtu, acl_handle, lcid, 0));
  ASSERT_NO_FATAL_FAILURE(ReceiveAndVerifyIncomingTransmission(
      server_handle, false, test_scn, true, "Hello World!\r", 50, acl_handle,
      lcid, 0));
  ASSERT_NO_FATAL_FAILURE(
      SendAndVerifyOutgoingTransmission(server_handle, false, test_scn, false,
                                        "\r!dlroW olleH", 4, acl_handle, lcid));
}

TEST_F(StackRfcommTest, MultiServerPortSameDeviceHelloWorld) {
  // Prepare a server channel at kTestChannelNumber0
  static const uint16_t acl_handle = 0x0009;
  static const uint16_t lcid = 0x0054;
  static const uint16_t test_mtu = 1600;
  static const RawAddress test_address = GetTestAddress(0);

  // Service 0
  uint16_t server_handle_0 = 0;
  static const uint8_t test_scn_0 = 8;
  static const uint16_t test_uuid_0 = 0x1112;
  ASSERT_NO_FATAL_FAILURE(StartServerPort(test_uuid_0, test_scn_0, test_mtu,
                                          port_mgmt_cback_0, port_event_cback_0,
                                          &server_handle_0));
  ASSERT_NO_FATAL_FAILURE(ConnectServerL2cap(test_address, acl_handle, lcid));
  ASSERT_NO_FATAL_FAILURE(ConnectServerPort(test_address, server_handle_0,
                                            test_scn_0, test_mtu, acl_handle,
                                            lcid, 0));

  // Service 1
  uint16_t server_handle_1 = 0;
  static const uint8_t test_scn_1 = 10;
  static const uint16_t test_uuid_1 = 0x111F;
  ASSERT_NE(test_scn_1, test_scn_0);
  ASSERT_NE(test_uuid_1, test_uuid_0);
  ASSERT_NO_FATAL_FAILURE(StartServerPort(test_uuid_1, test_scn_1, test_mtu,
                                          port_mgmt_cback_1, port_event_cback_1,
                                          &server_handle_1));
  // No L2CAP setup for 2nd device
  ASSERT_NO_FATAL_FAILURE(ConnectServerPort(test_address, server_handle_1,
                                            test_scn_1, test_mtu, acl_handle,
                                            lcid, 1));

  // Use service 0
  ASSERT_NO_FATAL_FAILURE(ReceiveAndVerifyIncomingTransmission(
      server_handle_0, false, test_scn_0, true, "Hello World0!\r", 50,
      acl_handle, lcid, 0));
  ASSERT_NO_FATAL_FAILURE(SendAndVerifyOutgoingTransmission(
      server_handle_0, false, test_scn_0, false, "\r!0dlroW olleH", 4,
      acl_handle, lcid));
  // Use service 1
  ASSERT_NO_FATAL_FAILURE(ReceiveAndVerifyIncomingTransmission(
      server_handle_1, false, test_scn_1, true, "Hello World1!\r", 50,
      acl_handle, lcid, 1));
  ASSERT_NO_FATAL_FAILURE(SendAndVerifyOutgoingTransmission(
      server_handle_1, false, test_scn_1, false, "\r!1dlroW olleH", 4,
      acl_handle, lcid));
}

TEST_F(StackRfcommTest, SameServerPortMultiDeviceHelloWorld) {
  // Prepare a server channel at kTestChannelNumber0
  static const uint16_t test_mtu = 1600;
  static const uint8_t test_scn = 3;
  static const uint16_t test_uuid = 0x1112;

  // Service 0
  static const RawAddress test_address_0 = GetTestAddress(0);
  static const uint16_t acl_handle_0 = 0x0009;
  static const uint16_t lcid_0 = 0x0054;
  uint16_t server_handle_0 = 0;
  ASSERT_NO_FATAL_FAILURE(StartServerPort(test_uuid, test_scn, test_mtu,
                                          port_mgmt_cback_0, port_event_cback_0,
                                          &server_handle_0));
  ASSERT_NO_FATAL_FAILURE(
      ConnectServerL2cap(test_address_0, acl_handle_0, lcid_0));
  ASSERT_NO_FATAL_FAILURE(ConnectServerPort(test_address_0, server_handle_0,
                                            test_scn, test_mtu, acl_handle_0,
                                            lcid_0, 0));

  // Service 1
  static const RawAddress test_address_1 = GetTestAddress(1);
  static const uint16_t acl_handle_1 = 0x0012;
  static const uint16_t lcid_1 = 0x0045;
  uint16_t server_handle_1 = 0;
  ASSERT_NO_FATAL_FAILURE(StartServerPort(test_uuid, test_scn, test_mtu,
                                          port_mgmt_cback_1, port_event_cback_1,
                                          &server_handle_1));
  ASSERT_NO_FATAL_FAILURE(
      ConnectServerL2cap(test_address_1, acl_handle_1, lcid_1));
  ASSERT_NO_FATAL_FAILURE(ConnectServerPort(test_address_1, server_handle_1,
                                            test_scn, test_mtu, acl_handle_1,
                                            lcid_1, 1));

  // Use service 0
  ASSERT_NO_FATAL_FAILURE(ReceiveAndVerifyIncomingTransmission(
      server_handle_0, false, test_scn, true, "Hello World0!\r", 50,
      acl_handle_0, lcid_0, 0));
  ASSERT_NO_FATAL_FAILURE(SendAndVerifyOutgoingTransmission(
      server_handle_0, false, test_scn, false, "\r!0dlroW olleH", 4,
      acl_handle_0, lcid_0));
  // Use service 1
  ASSERT_NO_FATAL_FAILURE(ReceiveAndVerifyIncomingTransmission(
      server_handle_1, false, test_scn, true, "Hello World1!\r", 50,
      acl_handle_1, lcid_1, 1));
  ASSERT_NO_FATAL_FAILURE(SendAndVerifyOutgoingTransmission(
      server_handle_1, false, test_scn, false, "\r!1dlroW olleH", 4,
      acl_handle_1, lcid_1));
}

TEST_F(StackRfcommTest, SingleClientConnectionHelloWorld) {
  static const uint16_t acl_handle = 0x0009;
  static const uint16_t lcid = 0x0054;
  static const uint16_t test_uuid = 0x1112;
  static const uint8_t test_scn = 8;
  static const uint16_t test_mtu = 1600;
  static const RawAddress test_address = GetTestAddress(0);
  uint16_t client_handle = 0;
  ASSERT_NO_FATAL_FAILURE(StartClientPort(
      test_address, test_uuid, test_scn, test_mtu, port_mgmt_cback_0,
      port_event_cback_0, lcid, acl_handle, &client_handle, true));
  ASSERT_NO_FATAL_FAILURE(TestConnectClientPortL2cap(acl_handle, lcid));
  ASSERT_NO_FATAL_FAILURE(ConnectClientPort(test_address, client_handle,
                                            test_scn, test_mtu, acl_handle,
                                            lcid, 0, true));
  ASSERT_NO_FATAL_FAILURE(SendAndVerifyOutgoingTransmission(
      client_handle, false, test_scn, true, "\r!dlroW olleH", -1, acl_handle,
      lcid));
  ASSERT_NO_FATAL_FAILURE(ReceiveAndVerifyIncomingTransmission(
      client_handle, false, test_scn, false, "Hello World!\r", -1, acl_handle,
      lcid, 0));
}

TEST_F(StackRfcommTest, MultiClientPortSameDeviceHelloWorld) {
  static const uint16_t acl_handle = 0x0009;
  static const uint16_t lcid = 0x0054;
  static const uint16_t test_mtu = 1600;
  static const RawAddress test_address = GetTestAddress(0);

  // Connection 0
  static const uint16_t test_uuid_0 = 0x1112;
  static const uint8_t test_scn_0 = 8;
  uint16_t client_handle_0 = 0;
  ASSERT_NO_FATAL_FAILURE(StartClientPort(
      test_address, test_uuid_0, test_scn_0, test_mtu, port_mgmt_cback_0,
      port_event_cback_0, lcid, acl_handle, &client_handle_0, true));
  ASSERT_NO_FATAL_FAILURE(TestConnectClientPortL2cap(acl_handle, lcid));
  ASSERT_NO_FATAL_FAILURE(ConnectClientPort(test_address, client_handle_0,
                                            test_scn_0, test_mtu, acl_handle,
                                            lcid, 0, true));

  // Connection 1
  static const uint16_t test_uuid_1 = 0x111F;
  static const uint8_t test_scn_1 = 10;
  uint16_t client_handle_1 = 0;
  ASSERT_NO_FATAL_FAILURE(StartClientPort(
      test_address, test_uuid_1, test_scn_1, test_mtu, port_mgmt_cback_1,
      port_event_cback_1, lcid, acl_handle, &client_handle_1, false));
  ASSERT_NO_FATAL_FAILURE(ConnectClientPort(test_address, client_handle_1,
                                            test_scn_1, test_mtu, acl_handle,
                                            lcid, 1, false));

  // Use connection 0
  ASSERT_NO_FATAL_FAILURE(SendAndVerifyOutgoingTransmission(
      client_handle_0, false, test_scn_0, true, "\r!dlroW olleH", -1,
      acl_handle, lcid));
  ASSERT_NO_FATAL_FAILURE(ReceiveAndVerifyIncomingTransmission(
      client_handle_0, false, test_scn_0, false, "Hello World!\r", -1,
      acl_handle, lcid, 0));

  // Use connection 1
  ASSERT_NO_FATAL_FAILURE(SendAndVerifyOutgoingTransmission(
      client_handle_1, false, test_scn_1, true, "\r!dlroW olleH", -1,
      acl_handle, lcid));
  ASSERT_NO_FATAL_FAILURE(ReceiveAndVerifyIncomingTransmission(
      client_handle_1, false, test_scn_1, false, "Hello World!\r", -1,
      acl_handle, lcid, 1));
}

TEST_F(StackRfcommTest, SameClientPortMultiDeviceHelloWorld) {
  static const uint16_t test_uuid = 0x1112;
  static const uint8_t test_scn = 8;
  static const uint16_t test_mtu = 1600;

  // Connection 0
  static const RawAddress test_address_0 = GetTestAddress(0);
  static const uint16_t acl_handle_0 = 0x0009;
  static const uint16_t lcid_0 = 0x0054;
  uint16_t client_handle_0 = 0;
  ASSERT_NO_FATAL_FAILURE(StartClientPort(
      test_address_0, test_uuid, test_scn, test_mtu, port_mgmt_cback_0,
      port_event_cback_0, lcid_0, acl_handle_0, &client_handle_0, true));
  ASSERT_NO_FATAL_FAILURE(TestConnectClientPortL2cap(acl_handle_0, lcid_0));
  ASSERT_NO_FATAL_FAILURE(ConnectClientPort(test_address_0, client_handle_0,
                                            test_scn, test_mtu, acl_handle_0,
                                            lcid_0, 0, true));

  // Connection 1
  static const RawAddress test_address_1 = GetTestAddress(1);
  static const uint16_t acl_handle_1 = 0x0012;
  static const uint16_t lcid_1 = 0x0045;
  uint16_t client_handle_1 = 0;
  ASSERT_NO_FATAL_FAILURE(StartClientPort(
      test_address_1, test_uuid, test_scn, test_mtu, port_mgmt_cback_1,
      port_event_cback_1, lcid_1, acl_handle_1, &client_handle_1, true));
  ASSERT_NO_FATAL_FAILURE(TestConnectClientPortL2cap(acl_handle_1, lcid_1));
  ASSERT_NO_FATAL_FAILURE(ConnectClientPort(test_address_1, client_handle_1,
                                            test_scn, test_mtu, acl_handle_1,
                                            lcid_1, 1, true));

  // Use connection 0
  ASSERT_NO_FATAL_FAILURE(SendAndVerifyOutgoingTransmission(
      client_handle_0, false, test_scn, true, "\r!dlroW olleH", -1,
      acl_handle_0, lcid_0));
  ASSERT_NO_FATAL_FAILURE(ReceiveAndVerifyIncomingTransmission(
      client_handle_0, false, test_scn, false, "Hello World!\r", -1,
      acl_handle_0, lcid_0, 0));

  // Use connection 1
  ASSERT_NO_FATAL_FAILURE(SendAndVerifyOutgoingTransmission(
      client_handle_1, false, test_scn, true, "\r!dlroW olleH", -1,
      acl_handle_1, lcid_1));
  ASSERT_NO_FATAL_FAILURE(ReceiveAndVerifyIncomingTransmission(
      client_handle_1, false, test_scn, false, "Hello World!\r", -1,
      acl_handle_1, lcid_1, 1));
}

TEST_F(StackRfcommTest, TestConnectionCollision) {
  static const uint16_t acl_handle = 0x0008;
  static const uint16_t old_lcid = 0x004a;
  static const uint16_t new_lcid = 0x005c;
  static const uint16_t test_uuid = 0x1112;
  static const uint8_t test_server_scn = 3;
  static const uint8_t test_peer_scn = 10;
  // Must be smaller than L2CAP_MTU_SIZE by at least 4 bytes
  static const uint16_t test_mtu = 1000;
  static const RawAddress test_address = GetTestAddress(0);
  uint16_t server_handle = 0;
  VLOG(1) << "Step 1";
  // Prepare a server port
  int status = RFCOMM_CreateConnection(test_uuid, test_server_scn, true,
                                       test_mtu, RawAddress::kAny,
                                       &server_handle, port_mgmt_cback_0);
  ASSERT_EQ(status, PORT_SUCCESS);
  status = PORT_SetEventMask(server_handle, PORT_EV_RXCHAR);
  ASSERT_EQ(status, PORT_SUCCESS);
  status = PORT_SetEventCallback(server_handle, port_event_cback_0);
  ASSERT_EQ(status, PORT_SUCCESS);

  VLOG(1) << "Step 2";
  // Try to connect to a client port
  uint16_t client_handle_1 = 0;
  EXPECT_CALL(l2cap_interface_, ConnectRequest(BT_PSM_RFCOMM, test_address))
      .Times(1)
      .WillOnce(Return(old_lcid));
  status = RFCOMM_CreateConnection(test_uuid, test_peer_scn, false, test_mtu,
                                   test_address, &client_handle_1,
                                   port_mgmt_cback_1);
  ASSERT_EQ(status, PORT_SUCCESS);
  status = PORT_SetEventMask(client_handle_1, PORT_EV_RXCHAR);
  ASSERT_EQ(status, PORT_SUCCESS);
  status = PORT_SetEventCallback(client_handle_1, port_event_cback_1);
  ASSERT_EQ(status, PORT_SUCCESS);

  VLOG(1) << "Step 3";
  // While our connection is pending, remote device tries to connect to
  // new_lcid, with L2CAP command id: pending_cmd_id
  static const uint8_t pending_cmd_id = 0x05;
  // RFCOMM starts timer for collision:
  l2cap_appl_info_.pL2CA_ConnectInd_Cb(test_address, new_lcid, BT_PSM_RFCOMM,
                                       pending_cmd_id);

  VLOG(1) << "Step 4";
  // Remote reject our connection request saying PSM not allowed
  // This should trigger RFCOMM to accept remote L2CAP connection at new_lcid
  EXPECT_CALL(l2cap_interface_, ConnectResponse(test_address, pending_cmd_id,
                                                new_lcid, L2CAP_CONN_OK, 0))
      .WillOnce(Return(true));
  // Followed by configure request for MTU size
  tL2CAP_CFG_INFO our_cfg_req = {.mtu_present = true, .mtu = L2CAP_MTU_SIZE};
  EXPECT_CALL(l2cap_interface_,
              ConfigRequest(new_lcid, PointerMemoryEqual(&our_cfg_req)))
      .WillOnce(Return(true));
  l2cap_appl_info_.pL2CA_ConnectCfm_Cb(old_lcid, L2CAP_CONN_NO_PSM);

  VLOG(1) << "Step 5";
  // Remote device also ask to configure MTU size as well
  tL2CAP_CFG_INFO peer_cfg_req = {.mtu_present = true, .mtu = test_mtu};
  // We responded by saying OK
  tL2CAP_CFG_INFO our_cfg_rsp = {.result = L2CAP_CFG_OK,
                                 .mtu = peer_cfg_req.mtu};
  EXPECT_CALL(l2cap_interface_,
              ConfigResponse(new_lcid, PointerMemoryEqual(&our_cfg_rsp)))
      .WillOnce(Return(true));
  l2cap_appl_info_.pL2CA_ConfigInd_Cb(new_lcid, &peer_cfg_req);

  VLOG(1) << "Step 6";
  // Remote device accepted our MTU size
  tL2CAP_CFG_INFO peer_cfg_rsp = {.mtu_present = true, .mtu = L2CAP_MTU_SIZE};
  l2cap_appl_info_.pL2CA_ConfigCfm_Cb(new_lcid, &peer_cfg_rsp);

  // L2CAP collision and connection setup done

  VLOG(1) << "Step 7";
  // Remote device connect multiplexer channel
  BT_HDR* sabm_channel_0 = AllocateWrappedIncomingL2capAclPacket(
      CreateQuickSabmPacket(RFCOMM_MX_DLCI, new_lcid, acl_handle));
  // We accept
  BT_HDR* ua_channel_0 = AllocateWrappedOutgoingL2capAclPacket(
      CreateQuickUaPacket(RFCOMM_MX_DLCI, new_lcid, acl_handle));
  EXPECT_CALL(l2cap_interface_, DataWrite(new_lcid, BtHdrEqual(ua_channel_0)))
      .WillOnce(Return(L2CAP_DW_SUCCESS));
  // And immediately try to configure test_peer_scn
  BT_HDR* uih_pn_cmd_to_peer = AllocateWrappedOutgoingL2capAclPacket(
      CreateQuickPnPacket(false, GetDlci(true, test_peer_scn), true, test_mtu,
                          RFCOMM_PN_CONV_LAYER_CBFC_I >> 4, 0, RFCOMM_K_MAX,
                          new_lcid, acl_handle));
  EXPECT_CALL(l2cap_interface_,
              DataWrite(new_lcid, BtHdrEqual(uih_pn_cmd_to_peer)))
      .WillOnce(Return(L2CAP_DW_SUCCESS));
  // Packet should be freed by this method
  l2cap_appl_info_.pL2CA_DataInd_Cb(new_lcid, sabm_channel_0);
  osi_free(ua_channel_0);
  osi_free(uih_pn_cmd_to_peer);

  VLOG(1) << "Step 8";
  // Peer tries to configure test_server_scn
  BT_HDR* uih_pn_cmd_from_peer = AllocateWrappedIncomingL2capAclPacket(
      CreateQuickPnPacket(true, GetDlci(false, test_server_scn), true, test_mtu,
                          RFCOMM_PN_CONV_LAYER_CBFC_I >> 4, 0, RFCOMM_K_MAX,
                          new_lcid, acl_handle));
  // We, as acceptor, must accept
  BT_HDR* uih_pn_rsp_to_peer = AllocateWrappedOutgoingL2capAclPacket(
      CreateQuickPnPacket(false, GetDlci(false, test_server_scn), false,
                          test_mtu, RFCOMM_PN_CONV_LAYER_CBFC_R >> 4, 0,
                          RFCOMM_K_MAX, new_lcid, acl_handle));
  EXPECT_CALL(l2cap_interface_,
              DataWrite(new_lcid, BtHdrEqual(uih_pn_rsp_to_peer)))
      .Times(1)
      .WillOnce(Return(L2CAP_DW_SUCCESS));
  l2cap_appl_info_.pL2CA_DataInd_Cb(new_lcid, uih_pn_cmd_from_peer);
  osi_free(uih_pn_rsp_to_peer);

  VLOG(1) << "Step 9";
  // Remote never replies our configuration request for test_peer_scn
  // But instead connect to test_server_scn directly
  BT_HDR* sabm_server_scn =
      AllocateWrappedIncomingL2capAclPacket(CreateQuickSabmPacket(
          GetDlci(false, test_server_scn), new_lcid, acl_handle));
  // We must do security check first
  tBTM_SEC_CALLBACK* security_callback = nullptr;
  void* p_port = nullptr;
  EXPECT_CALL(btm_security_internal_interface_,
              MultiplexingProtocolAccessRequest(
                  test_address, BT_PSM_RFCOMM, false, BTM_SEC_PROTO_RFCOMM,
                  test_server_scn, NotNull(), NotNull()))
      .WillOnce(DoAll(SaveArg<5>(&security_callback), SaveArg<6>(&p_port),
                      Return(BTM_SUCCESS)));
  l2cap_appl_info_.pL2CA_DataInd_Cb(new_lcid, sabm_server_scn);

  VLOG(1) << "Step 10";
  // After security check, we accept the connection
  ASSERT_TRUE(security_callback);
  BT_HDR* ua_server_scn =
      AllocateWrappedOutgoingL2capAclPacket(CreateQuickUaPacket(
          GetDlci(false, test_server_scn), new_lcid, acl_handle));
  EXPECT_CALL(l2cap_interface_, DataWrite(new_lcid, BtHdrEqual(ua_server_scn)))
      .WillOnce(Return(L2CAP_DW_SUCCESS));
  // Callback should come from server port instead, client port will timeout
  // in 20 seconds
  EXPECT_CALL(rfcomm_callback_,
              PortManagementCallback(PORT_SUCCESS, server_handle, 0));
  security_callback(&test_address, BT_TRANSPORT_BR_EDR, p_port, BTM_SUCCESS);
  osi_free(ua_server_scn);

  VLOG(1) << "Step 11";
  // MPX_CTRL Modem Status Command (MSC)
  BT_HDR* uih_msc_cmd_from_peer = AllocateWrappedIncomingL2capAclPacket(
      CreateQuickMscPacket(true, GetDlci(false, test_server_scn), new_lcid,
                           acl_handle, true, false, true, true, false, true));
  BT_HDR* uih_msc_rsp_to_peer = AllocateWrappedOutgoingL2capAclPacket(
      CreateQuickMscPacket(false, GetDlci(false, test_server_scn), new_lcid,
                           acl_handle, false, false, true, true, false, true));
  // MPX_CTRL Modem Status Response (MSC)
  EXPECT_CALL(l2cap_interface_,
              DataWrite(new_lcid, BtHdrEqual(uih_msc_rsp_to_peer)))
      .WillOnce(Return(L2CAP_DW_SUCCESS));
  BT_HDR* uih_msc_cmd_to_peer = AllocateWrappedOutgoingL2capAclPacket(
      CreateQuickMscPacket(false, GetDlci(false, test_server_scn), new_lcid,
                           acl_handle, true, false, true, true, false, true));
  EXPECT_CALL(l2cap_interface_,
              DataWrite(new_lcid, BtHdrEqual(uih_msc_cmd_to_peer)))
      .WillOnce(Return(L2CAP_DW_SUCCESS));
  l2cap_appl_info_.pL2CA_DataInd_Cb(new_lcid, uih_msc_cmd_from_peer);
  osi_free(uih_msc_rsp_to_peer);
  osi_free(uih_msc_cmd_to_peer);

  VLOG(1) << "Step 12";
  BT_HDR* uih_msc_rsp_from_peer = AllocateWrappedIncomingL2capAclPacket(
      CreateQuickMscPacket(true, GetDlci(false, test_server_scn), new_lcid,
                           acl_handle, false, false, true, true, false, true));
  l2cap_appl_info_.pL2CA_DataInd_Cb(new_lcid, uih_msc_rsp_from_peer);
}

}  // namespace
