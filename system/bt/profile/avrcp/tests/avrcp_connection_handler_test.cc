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

#include <base/bind.h>
#include <base/logging.h>
#include <base/message_loop/message_loop.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "avrcp_internal.h"
#include "avrcp_test_helper.h"
#include "connection_handler.h"

using ::testing::_;
using ::testing::Return;
using ::testing::SaveArg;
using ::testing::SaveArgPointee;
using ::testing::SetArgPointee;
using ::testing::MockFunction;
using ::testing::NiceMock;
using ::testing::StrictMock;

namespace bluetooth {
namespace avrcp {

using device_ptr = std::shared_ptr<Device>;

class AvrcpConnectionHandlerTest : public testing::Test {
 public:
  void SetUp() override {
    ON_CALL(mock_avrcp_, Close(_)).WillByDefault(Return(0));
  }

  void SetUpSdp(tAVRC_FIND_CBACK* sdp_cb, bool browsing, bool absolute_volume) {
    EXPECT_CALL(mock_avrcp_, FindService(_, _, _, _))
        .Times(1)
        .WillOnce(DoAll(SaveArg<3>(sdp_cb), Return(0)));

    static tSDP_DISC_ATTR fake_features;

    fake_features = {
        .p_next_attr = nullptr,
        .attr_id = 0,
        .attr_len_type = 0,
        .attr_value.v.u16 = 0,
    };

    if (browsing) fake_features.attr_value.v.u16 |= AVRC_SUPF_CT_BROWSE;
    if (absolute_volume) fake_features.attr_value.v.u16 |= AVRC_SUPF_CT_CAT2;

    EXPECT_CALL(mock_sdp_, FindAttributeInRec(_, _))
        .Times(4)
        .WillRepeatedly(Return(&fake_features));

    EXPECT_CALL(mock_sdp_, FindServiceInDb(_, _, _))
        .Times(2)
        .WillOnce(Return((tSDP_DISC_REC*)0x01))   // Return any non null pointer
        .WillOnce(Return((tSDP_DISC_REC*)0x01));  // Return any non null pointer

    EXPECT_CALL(mock_sdp_, FindProfileVersionInRec(_, _, _))
        .Times(2)
        .WillRepeatedly(DoAll(SetArgPointee<2>(AVRC_REV_1_6), Return(true)));
  }

 protected:
  ConnectionHandler* connection_handler_ = nullptr;

  // We use NiceMock's here because each function of this code does quite a few
  // operations. This way it is much easier to write a higher number of smaller
  // tests without having a large amount of warnings.
  NiceMock<MockFunction<void(device_ptr)>> device_cb;
  NiceMock<MockAvrcpInterface> mock_avrcp_;
  NiceMock<MockSdpInterface> mock_sdp_;
  NiceMock<MockVolumeInterface> mock_volume_;
};

TEST_F(AvrcpConnectionHandlerTest, initializeTest) {
  // Set an Expectation that Open will be called as an acceptor and save the
  // connection callback once it is called
  tAVRC_CONN_CB conn_cb;
  EXPECT_CALL(mock_avrcp_, Open(_, _, RawAddress::kAny))
      .Times(1)
      .WillOnce(
          DoAll(SetArgPointee<0>(1), SaveArgPointee<1>(&conn_cb), Return(0)));

  auto bound_callback = base::Bind(&MockFunction<void(device_ptr)>::Call,
                                   base::Unretained(&device_cb));
  ASSERT_TRUE(ConnectionHandler::Initialize(bound_callback, &mock_avrcp_,
                                            &mock_sdp_, &mock_volume_));
  connection_handler_ = ConnectionHandler::Get();

  // Check that the callback was sent with us as the acceptor
  ASSERT_EQ(conn_cb.conn, 1);

  connection_handler_ = nullptr;
  ConnectionHandler::CleanUp();
}

// Check that disconnecting without an active connection
TEST_F(AvrcpConnectionHandlerTest, notConnectedDisconnectTest) {
  // Set an Expectation that Open will be called twice as an acceptor and save
  // the connection callback once it is called.
  tAVRC_CONN_CB conn_cb;
  EXPECT_CALL(mock_avrcp_, Open(_, _, RawAddress::kAny))
      .Times(1)
      .WillOnce(
          DoAll(SetArgPointee<0>(1), SaveArgPointee<1>(&conn_cb), Return(0)));

  // Initialize the interface
  auto bound_callback = base::Bind(&MockFunction<void(device_ptr)>::Call,
                                   base::Unretained(&device_cb));
  ASSERT_TRUE(ConnectionHandler::Initialize(bound_callback, &mock_avrcp_,
                                            &mock_sdp_, &mock_volume_));
  connection_handler_ = ConnectionHandler::Get();

  // Call the callback with a message saying the connection has closed
  conn_cb.ctrl_cback.Run(1, AVRC_CLOSE_IND_EVT, 0, &RawAddress::kAny);

  connection_handler_ = nullptr;
  ConnectionHandler::CleanUp();
};

// Test calling the connection callback after the handler is cleaned up
TEST_F(AvrcpConnectionHandlerTest, disconnectAfterCleanupTest) {
  // Set an Expectation that Open will be called twice as an acceptor and save
  // the connection callback once it is called.
  tAVRC_CONN_CB conn_cb;
  EXPECT_CALL(mock_avrcp_, Open(_, _, RawAddress::kAny))
      .Times(1)
      .WillOnce(
          DoAll(SetArgPointee<0>(1), SaveArgPointee<1>(&conn_cb), Return(0)));

  // Initialize the interface
  auto bound_callback = base::Bind(&MockFunction<void(device_ptr)>::Call,
                                   base::Unretained(&device_cb));
  ASSERT_TRUE(ConnectionHandler::Initialize(bound_callback, &mock_avrcp_,
                                            &mock_sdp_, &mock_volume_));
  connection_handler_ = ConnectionHandler::Get();

  connection_handler_ = nullptr;
  ConnectionHandler::CleanUp();

  // Call the callback with a message saying the connection has closed
  conn_cb.ctrl_cback.Run(1, AVRC_CLOSE_IND_EVT, 0, &RawAddress::kAny);
};

// Check that we can handle having a remote device connect to us, start SDP, and
// open another acceptor connection
TEST_F(AvrcpConnectionHandlerTest, remoteDeviceConnectionTest) {
  // Set an Expectation that Open will be called twice as an acceptor and save
  // the connection callback once it is called.
  tAVRC_CONN_CB conn_cb;
  EXPECT_CALL(mock_avrcp_, Open(_, _, RawAddress::kAny))
      .Times(2)
      .WillOnce(
          DoAll(SetArgPointee<0>(1), SaveArgPointee<1>(&conn_cb), Return(0)))
      .WillOnce(
          DoAll(SetArgPointee<0>(2), SaveArgPointee<1>(&conn_cb), Return(0)));

  // Initialize the interface
  auto bound_callback = base::Bind(&MockFunction<void(device_ptr)>::Call,
                                   base::Unretained(&device_cb));
  ASSERT_TRUE(ConnectionHandler::Initialize(bound_callback, &mock_avrcp_,
                                            &mock_sdp_, &mock_volume_));
  connection_handler_ = ConnectionHandler::Get();

  // Check that the callback was sent with us as the acceptor
  ASSERT_EQ(conn_cb.conn, 1);

  // Set an Expectations that SDP will be performed
  tAVRC_FIND_CBACK sdp_cb;
  SetUpSdp(&sdp_cb, false, false);

  // Set an expectation that a device will be created
  EXPECT_CALL(device_cb, Call(_)).Times(1);

  // Set an Expectation that OpenBrowse will be called in acceptor mode when the
  // device connects.
  EXPECT_CALL(mock_avrcp_, OpenBrowse(1, AVCT_ACP)).Times(1);

  // Call the callback with a message saying that a remote device has connected
  conn_cb.ctrl_cback.Run(1, AVRC_OPEN_IND_EVT, 0, &RawAddress::kAny);

  // Run the SDP callback with status success
  sdp_cb.Run(0);

  connection_handler_ = nullptr;
  ConnectionHandler::CleanUp();
}

// Check that when a device does not support absolute volume, that the
// handler reports that via the volume interface.
TEST_F(AvrcpConnectionHandlerTest, noAbsoluteVolumeTest) {
  // Set an Expectation that Open will be called twice as an acceptor and save
  // the connection callback once it is called.
  tAVRC_CONN_CB conn_cb;
  EXPECT_CALL(mock_avrcp_, Open(_, _, RawAddress::kAny))
      .Times(2)
      .WillOnce(
          DoAll(SetArgPointee<0>(1), SaveArgPointee<1>(&conn_cb), Return(0)))
      .WillOnce(
          DoAll(SetArgPointee<0>(2), SaveArgPointee<1>(&conn_cb), Return(0)));

  // Initialize the interface
  auto bound_callback = base::Bind(&MockFunction<void(device_ptr)>::Call,
                                   base::Unretained(&device_cb));
  ASSERT_TRUE(ConnectionHandler::Initialize(bound_callback, &mock_avrcp_,
                                            &mock_sdp_, &mock_volume_));
  connection_handler_ = ConnectionHandler::Get();

  // Set an Expectations that SDP will be performed
  tAVRC_FIND_CBACK sdp_cb;
  SetUpSdp(&sdp_cb, false, false);

  EXPECT_CALL(mock_volume_, DeviceConnected(RawAddress::kAny)).Times(1);

  // Call the callback with a message saying that a remote device has connected
  conn_cb.ctrl_cback.Run(1, AVRC_OPEN_IND_EVT, 0, &RawAddress::kAny);

  // Run the SDP callback with status success
  sdp_cb.Run(0);

  connection_handler_ = nullptr;
  ConnectionHandler::CleanUp();
}

// Check that when a device does support absolute volume, that the handler
// doesn't report it. Instead that will be left up to the device.
TEST_F(AvrcpConnectionHandlerTest, absoluteVolumeTest) {
  // Set an Expectation that Open will be called twice as an acceptor and save
  // the connection callback once it is called.
  tAVRC_CONN_CB conn_cb;
  EXPECT_CALL(mock_avrcp_, Open(_, _, RawAddress::kAny))
      .Times(2)
      .WillOnce(
          DoAll(SetArgPointee<0>(1), SaveArgPointee<1>(&conn_cb), Return(0)))
      .WillOnce(
          DoAll(SetArgPointee<0>(2), SaveArgPointee<1>(&conn_cb), Return(0)));

  // Initialize the interface
  auto bound_callback = base::Bind(&MockFunction<void(device_ptr)>::Call,
                                   base::Unretained(&device_cb));

  StrictMock<MockVolumeInterface> strict_volume;
  ASSERT_TRUE(ConnectionHandler::Initialize(bound_callback, &mock_avrcp_,
                                            &mock_sdp_, &strict_volume));
  connection_handler_ = ConnectionHandler::Get();

  // Set an Expectations that SDP will be performed with absolute volume
  // supported
  tAVRC_FIND_CBACK sdp_cb;
  SetUpSdp(&sdp_cb, false, true);

  // Call the callback with a message saying that a remote device has connected
  conn_cb.ctrl_cback.Run(1, AVRC_OPEN_IND_EVT, 0, &RawAddress::kAny);

  // Run the SDP callback with status success
  sdp_cb.Run(0);

  connection_handler_ = nullptr;
  ConnectionHandler::CleanUp();
}

TEST_F(AvrcpConnectionHandlerTest, disconnectTest) {
  // Set an Expectation that Open will be called twice as an acceptor and save
  // the connection callback once it is called.
  tAVRC_CONN_CB conn_cb;
  EXPECT_CALL(mock_avrcp_, Open(_, _, RawAddress::kAny))
      .Times(2)
      .WillOnce(
          DoAll(SetArgPointee<0>(1), SaveArgPointee<1>(&conn_cb), Return(0)))
      .WillOnce(DoAll(SetArgPointee<0>(2), Return(0)));

  // Initialize the interface
  auto bound_callback = base::Bind(&MockFunction<void(device_ptr)>::Call,
                                   base::Unretained(&device_cb));
  ASSERT_TRUE(ConnectionHandler::Initialize(bound_callback, &mock_avrcp_,
                                            &mock_sdp_, &mock_volume_));
  connection_handler_ = ConnectionHandler::Get();

  // Call the callback with a message saying that a remote device has connected
  conn_cb.ctrl_cback.Run(1, AVRC_OPEN_IND_EVT, 0, &RawAddress::kAny);

  // Set up the expectation that Close will be called
  EXPECT_CALL(mock_avrcp_, Close(1)).Times(1);

  // Call the callback with a message saying that a remote device has
  // disconnected
  conn_cb.ctrl_cback.Run(1, AVRC_CLOSE_IND_EVT, 0, &RawAddress::kAny);

  connection_handler_ = nullptr;
  ConnectionHandler::CleanUp();
}

// Check that we can handle having a remote device connect to us, start SDP, and
// open another acceptor connection
TEST_F(AvrcpConnectionHandlerTest, multipleRemoteDeviceConnectionTest) {
  // Set an Expectation that Open will be called three times as an acceptor and
  // save the connection callback once it is called.
  tAVRC_CONN_CB conn_cb;
  EXPECT_CALL(mock_avrcp_, Open(_, _, RawAddress::kAny))
      .Times(3)
      .WillOnce(
          DoAll(SetArgPointee<0>(1), SaveArgPointee<1>(&conn_cb), Return(0)))
      .WillOnce(
          DoAll(SetArgPointee<0>(2), SaveArgPointee<1>(&conn_cb), Return(0)))
      .WillOnce(
          DoAll(SetArgPointee<0>(3), SaveArgPointee<1>(&conn_cb), Return(0)));

  // Initialize the interface
  auto bound_callback = base::Bind(&MockFunction<void(device_ptr)>::Call,
                                   base::Unretained(&device_cb));
  ASSERT_TRUE(ConnectionHandler::Initialize(bound_callback, &mock_avrcp_,
                                            &mock_sdp_, &mock_volume_));
  connection_handler_ = ConnectionHandler::Get();

  // Check that the callback was sent with us as the acceptor
  ASSERT_EQ(conn_cb.conn, 1);

  // Set an Expectations that SDP will be performed
  tAVRC_FIND_CBACK sdp_cb;
  SetUpSdp(&sdp_cb, false, false);

  // Set an expectation that a device will be created
  EXPECT_CALL(device_cb, Call(_)).Times(1);

  // Set an Expectation that OpenBrowse will be called in acceptor mode when the
  // device connects on handle 1
  EXPECT_CALL(mock_avrcp_, OpenBrowse(1, AVCT_ACP)).Times(1);

  // Call the callback with a message saying that a remote device has connected
  conn_cb.ctrl_cback.Run(1, AVRC_OPEN_IND_EVT, 0, &RawAddress::kAny);

  // Run the SDP callback with status success
  sdp_cb.Run(0);

  // Set an Expectations that SDP will be performed again
  SetUpSdp(&sdp_cb, false, false);

  // Set an expectation that a device will be created again
  EXPECT_CALL(device_cb, Call(_)).Times(1);

  // Set an Expectation that OpenBrowse will be called in acceptor mode when the
  // device connects on handle 2
  EXPECT_CALL(mock_avrcp_, OpenBrowse(2, AVCT_ACP)).Times(1);

  // Call the callback with a message saying that a remote device has connected
  // with a different address
  conn_cb.ctrl_cback.Run(2, AVRC_OPEN_IND_EVT, 0, &RawAddress::kEmpty);

  // Run the SDP callback with status success
  sdp_cb.Run(0);

  connection_handler_ = nullptr;
  ConnectionHandler::CleanUp();
}

TEST_F(AvrcpConnectionHandlerTest, cleanupTest) {
  // Set Up Expectations for Initialize
  tAVRC_CONN_CB conn_cb;
  EXPECT_CALL(mock_avrcp_, Open(_, _, _))
      .WillOnce(
          DoAll(SetArgPointee<0>(1), SaveArgPointee<1>(&conn_cb), Return(0)))
      .WillOnce(
          DoAll(SetArgPointee<0>(2), SaveArgPointee<1>(&conn_cb), Return(0)))
      .WillOnce(
          DoAll(SetArgPointee<0>(3), SaveArgPointee<1>(&conn_cb), Return(0)));

  // Initialize the interface
  auto bound_callback = base::Bind(&MockFunction<void(device_ptr)>::Call,
                                   base::Unretained(&device_cb));
  ASSERT_TRUE(ConnectionHandler::Initialize(bound_callback, &mock_avrcp_,
                                            &mock_sdp_, &mock_volume_));
  connection_handler_ = ConnectionHandler::Get();

  // Call the callback twice with a message saying that a remote device has
  // connected
  conn_cb.ctrl_cback.Run(1, AVRC_OPEN_IND_EVT, 0, &RawAddress::kAny);
  conn_cb.ctrl_cback.Run(2, AVRC_OPEN_IND_EVT, 0, &RawAddress::kEmpty);

  // Set an Expectation that Close will be called twice with handles 1 and 2
  EXPECT_CALL(mock_avrcp_, Close(1));
  EXPECT_CALL(mock_avrcp_, Close(2));

  // Cleanup the object causing all open connections to be closed
  connection_handler_ = nullptr;
  ConnectionHandler::CleanUp();
}

TEST_F(AvrcpConnectionHandlerTest, connectToRemoteDeviceTest) {
  // Initialize the interface
  auto bound_callback = base::Bind(&MockFunction<void(device_ptr)>::Call,
                                   base::Unretained(&device_cb));
  ASSERT_TRUE(ConnectionHandler::Initialize(bound_callback, &mock_avrcp_,
                                            &mock_sdp_, &mock_volume_));
  connection_handler_ = ConnectionHandler::Get();

  // Set an Expectation that SDP will be performed
  tAVRC_FIND_CBACK sdp_cb;
  SetUpSdp(&sdp_cb, false, false);

  // Connect to the device which starts SDP
  connection_handler_->ConnectDevice(RawAddress::kEmpty);

  // Set an expectation that the handler will try to open an AVRCP connection
  // after doing SDP
  tAVRC_CONN_CB conn_cb;
  EXPECT_CALL(mock_avrcp_, Open(_, _, RawAddress::kEmpty))
      .Times(1)
      .WillOnce(
          DoAll(SetArgPointee<0>(1), SaveArgPointee<1>(&conn_cb), Return(0)));

  // Complete SDP
  sdp_cb.Run(0);

  // Check that the callback was sent with us as the initiator
  ASSERT_EQ(conn_cb.conn, 0);

  // Set an expectation that a device will be created
  EXPECT_CALL(device_cb, Call(_)).Times(1);

  // Set an Expectation that OpenBrowse will NOT be called since the SDP entry
  // didn't list browsing as a feature
  EXPECT_CALL(mock_avrcp_, OpenBrowse(_, _)).Times(0);

  // Call the callback with a message saying that a remote device has connected
  // with a different address
  conn_cb.ctrl_cback.Run(2, AVRC_OPEN_IND_EVT, 0, &RawAddress::kEmpty);

  // Cleanup the object causing all open connections to be closed
  connection_handler_ = nullptr;
  ConnectionHandler::CleanUp();
}

TEST_F(AvrcpConnectionHandlerTest, connectToBrowsableRemoteDeviceTest) {
  // Initialize the interface
  auto bound_callback = base::Bind(&MockFunction<void(device_ptr)>::Call,
                                   base::Unretained(&device_cb));
  ASSERT_TRUE(ConnectionHandler::Initialize(bound_callback, &mock_avrcp_,
                                            &mock_sdp_, &mock_volume_));
  connection_handler_ = ConnectionHandler::Get();

  // Set an Expectation that SDP will be performed
  tAVRC_FIND_CBACK sdp_cb;
  SetUpSdp(&sdp_cb, true, false);

  // Connect to the device which starts SDP
  connection_handler_->ConnectDevice(RawAddress::kEmpty);

  // Set an expectation that the handler will try to open an AVRCP connection
  // after doing SDP
  tAVRC_CONN_CB conn_cb;
  EXPECT_CALL(mock_avrcp_, Open(_, _, RawAddress::kEmpty))
      .Times(1)
      .WillOnce(
          DoAll(SetArgPointee<0>(1), SaveArgPointee<1>(&conn_cb), Return(0)));

  // Complete SDP
  sdp_cb.Run(0);

  // Check that the callback was sent with us as the initiator
  ASSERT_EQ(conn_cb.conn, 0);

  // Set an expectation that a device will be created
  EXPECT_CALL(device_cb, Call(_)).Times(1);

  // Set an Expectation that OpenBrowse will be called since browsing is listed
  // as supported in SDP
  EXPECT_CALL(mock_avrcp_, OpenBrowse(1, AVCT_INT)).Times(1);

  // Call the callback with a message saying that a remote device has connected
  // with a different address
  conn_cb.ctrl_cback.Run(1, AVRC_OPEN_IND_EVT, 0, &RawAddress::kEmpty);

  // Cleanup the object causing all open connections to be closed
  connection_handler_ = nullptr;
  ConnectionHandler::CleanUp();
}

TEST_F(AvrcpConnectionHandlerTest, disconnectWhileDoingSdpTest) {
  // Set an Expectation that Open will be called twice as an acceptor and save
  // the connection callback once it is called.
  tAVRC_CONN_CB conn_cb;
  EXPECT_CALL(mock_avrcp_, Open(_, _, RawAddress::kAny))
      .Times(2)
      .WillOnce(
          DoAll(SetArgPointee<0>(1), SaveArgPointee<1>(&conn_cb), Return(0)))
      .WillOnce(DoAll(SetArgPointee<0>(2), Return(0)));

  // Initialize the interface
  auto bound_callback = base::Bind(&MockFunction<void(device_ptr)>::Call,
                                   base::Unretained(&device_cb));
  ASSERT_TRUE(ConnectionHandler::Initialize(bound_callback, &mock_avrcp_,
                                            &mock_sdp_, &mock_volume_));
  connection_handler_ = ConnectionHandler::Get();

  // Set an Expectation that SDP will be performed
  tAVRC_FIND_CBACK sdp_cb;
  SetUpSdp(&sdp_cb, true, false);

  // Call the callback with a message saying that a remote device has connected
  conn_cb.ctrl_cback.Run(1, AVRC_OPEN_IND_EVT, 0, &RawAddress::kAny);

  // Call the callback with a message saying that a remote device has
  // disconnected
  conn_cb.ctrl_cback.Run(1, AVRC_CLOSE_IND_EVT, 0, &RawAddress::kAny);

  // Signal that SDP has completed
  sdp_cb.Run(0);

  connection_handler_ = nullptr;
  ConnectionHandler::CleanUp();
}

}  // namespace avrcp
}  // namespace bluetooth
