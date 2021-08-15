/*
 * Copyright (C) 2016, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <array>
#include <memory>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <wifi_system_test/mock_interface_tool.h>

#include "wificond/client_interface_impl.h"
#include "wificond/tests/mock_i_send_mgmt_frame_event.h"
#include "wificond/tests/mock_netlink_manager.h"
#include "wificond/tests/mock_netlink_utils.h"
#include "wificond/tests/mock_scan_utils.h"

using android::wifi_system::MockInterfaceTool;
using std::unique_ptr;
using std::vector;
using testing::Mock;
using testing::NiceMock;
using testing::Return;
using testing::StrictMock;
using testing::_;

namespace android {
namespace wificond {
namespace {

const uint32_t kTestWiphyIndex = 2;
const char kTestInterfaceName[] = "testwifi0";
const uint32_t kTestInterfaceIndex = 42;
const uint64_t kCookie = 42;
const int32_t kAutoMcs = -1;
const int32_t kMcs = 5;
const uint8_t kTestFrame[] = {0x00, 0x01, 0x02, 0x03};

class ClientInterfaceImplTest : public ::testing::Test {
 protected:

  void SetUp() override {
    SetUp(WiphyFeatures());
  }

  /**
   * call SetUp(WiphyFeatures wiphy_features) in your test function if
   * you would like to change WiphyFeatures.
   */
  void SetUp(WiphyFeatures wiphy_features) {
    EXPECT_CALL(*netlink_utils_,
                SubscribeMlmeEvent(kTestInterfaceIndex, _));
    EXPECT_CALL(*netlink_utils_,
                GetWiphyInfo(kTestWiphyIndex, _, _, _))
      .WillOnce([wiphy_features](uint32_t wiphy_index, BandInfo* out_band_info,
          ScanCapabilities* out_scan_capabilities,
          WiphyFeatures* out_wiphy_features) {
        *out_wiphy_features = wiphy_features;
        return true;
      });
    EXPECT_CALL(*netlink_utils_,
                SubscribeFrameTxStatusEvent(kTestInterfaceIndex, _))
        .WillOnce([this](uint32_t interface_index,
            OnFrameTxStatusEventHandler handler) {
          frame_tx_status_event_handler_ = handler;
        });
    client_interface_.reset(new ClientInterfaceImpl{
        kTestWiphyIndex,
        kTestInterfaceName,
        kTestInterfaceIndex,
        std::array<uint8_t, ETH_ALEN>{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        if_tool_.get(),
        netlink_utils_.get(),
        scan_utils_.get()});
  }

  void TearDown() override {
    EXPECT_CALL(*netlink_utils_,
                UnsubscribeMlmeEvent(kTestInterfaceIndex));
    EXPECT_CALL(*netlink_utils_,
                UnsubscribeFrameTxStatusEvent(kTestInterfaceIndex));
  }

  unique_ptr<NiceMock<MockInterfaceTool>> if_tool_{
      new NiceMock<MockInterfaceTool>};
  unique_ptr<NiceMock<MockNetlinkManager>> netlink_manager_{
      new NiceMock<MockNetlinkManager>()};
  unique_ptr<NiceMock<MockNetlinkUtils>> netlink_utils_{
      new NiceMock<MockNetlinkUtils>(netlink_manager_.get())};
  unique_ptr<NiceMock<MockScanUtils>> scan_utils_{
      new NiceMock<MockScanUtils>(netlink_manager_.get())};
  unique_ptr<ClientInterfaceImpl> client_interface_;
  OnFrameTxStatusEventHandler frame_tx_status_event_handler_;
  sp<StrictMock<MockISendMgmtFrameEvent>> send_mgmt_frame_event_{
      new StrictMock<MockISendMgmtFrameEvent>()};
};  // class ClientInterfaceImplTest

}  // namespace

TEST_F(ClientInterfaceImplTest, SetMacAddressFailsOnInterfaceDownFailure) {
  EXPECT_CALL(*if_tool_, SetWifiUpState(false)).WillOnce(Return(false));
  EXPECT_FALSE(
      client_interface_->SetMacAddress(std::array<uint8_t, ETH_ALEN>()));
}

TEST_F(ClientInterfaceImplTest, SetMacAddressFailsOnAddressChangeFailure) {
  EXPECT_CALL(*if_tool_, SetWifiUpState(false)).WillOnce(Return(true));
  EXPECT_CALL(*if_tool_, SetMacAddress(_, _)).WillOnce(Return(false));
  EXPECT_FALSE(
      client_interface_->SetMacAddress(std::array<uint8_t, ETH_ALEN>()));
}

TEST_F(ClientInterfaceImplTest, SetMacAddressFailsOnInterfaceUpFailure) {
  EXPECT_CALL(*if_tool_, SetWifiUpState(false)).WillOnce(Return(true));
  EXPECT_CALL(*if_tool_, SetMacAddress(_, _)).WillOnce(Return(true));
  EXPECT_CALL(*if_tool_, SetWifiUpState(true)).WillOnce(Return(false));
  EXPECT_FALSE(
      client_interface_->SetMacAddress(std::array<uint8_t, ETH_ALEN>()));
}

TEST_F(ClientInterfaceImplTest, SetMacAddressReturnsTrueOnSuccess) {
  EXPECT_CALL(*if_tool_, SetWifiUpState(false)).WillOnce(Return(true));
  EXPECT_CALL(*if_tool_, SetMacAddress(_, _)).WillOnce(Return(true));
  EXPECT_CALL(*if_tool_, SetWifiUpState(true)).WillOnce(Return(true));
  EXPECT_TRUE(
      client_interface_->SetMacAddress(std::array<uint8_t, ETH_ALEN>()));
}

TEST_F(ClientInterfaceImplTest, SetMacAddressPassesCorrectAddressToIfTool) {
  EXPECT_CALL(*if_tool_, SetWifiUpState(false)).WillOnce(Return(true));
  EXPECT_CALL(*if_tool_, SetMacAddress(_,
      std::array<uint8_t, ETH_ALEN>{{1, 2, 3, 4, 5, 6}}))
    .WillOnce(Return(true));
  EXPECT_CALL(*if_tool_, SetWifiUpState(true)).WillOnce(Return(true));
  EXPECT_TRUE(client_interface_->SetMacAddress(
      std::array<uint8_t, ETH_ALEN>{{1, 2, 3, 4, 5, 6}}));
}

/**
 * If the device does not support sending mgmt frame at specified MCS rate,
 * and the caller specifies a MCS < 0, the call should still succeed (and the
 * driver will determine the MCS rate automatically).
 */
TEST_F(ClientInterfaceImplTest, SendMgmtFrameMcsUnsupportedAutoSelectMcs) {
  EXPECT_CALL(*netlink_utils_,
      SendMgmtFrame(kTestInterfaceIndex,
          vector<uint8_t>(std::begin(kTestFrame), std::end(kTestFrame)),
          kAutoMcs, _))
    .WillOnce([](uint32_t interface_index, const vector<uint8_t>& frame,
        int32_t mcs, uint64_t* out_cookie) {
      *out_cookie = kCookie;
      return true;
    });

  EXPECT_CALL(*send_mgmt_frame_event_, OnAck(_));

  client_interface_->SendMgmtFrame(
      vector<uint8_t>(std::begin(kTestFrame), std::end(kTestFrame)),
      send_mgmt_frame_event_, kAutoMcs);
  frame_tx_status_event_handler_(kCookie, true);
}

/**
 * If the device does not support sending mgmt frame at specified MCS rate,
 * and the caller specifies a MCS >= 0, the call should fail.
 */
TEST_F(ClientInterfaceImplTest, SendMgmtFrameMcsUnsupportedCallerSpecifiedMcs) {
  EXPECT_CALL(*send_mgmt_frame_event_,
      OnFailure(send_mgmt_frame_event_->SEND_MGMT_FRAME_ERROR_MCS_UNSUPPORTED));

  client_interface_->SendMgmtFrame(
      vector<uint8_t>(std::begin(kTestFrame), std::end(kTestFrame)),
      send_mgmt_frame_event_, kMcs);
}

/**
 * If the device does support sending mgmt frame at specified MCS rate and the
 * user specifies a valid MCS rate, the call should succeed.
 */
TEST_F(ClientInterfaceImplTest, SendMgmtFrameMcsSupported) {
  WiphyFeatures wiphy_features;
  wiphy_features.supports_tx_mgmt_frame_mcs = true;
  SetUp(wiphy_features);

  EXPECT_CALL(*netlink_utils_,
      SendMgmtFrame(kTestInterfaceIndex,
          vector<uint8_t>(std::begin(kTestFrame), std::end(kTestFrame)),
          kMcs, _))
    .WillOnce([](uint32_t interface_index, const vector<uint8_t>& frame,
        int32_t mcs, uint64_t* out_cookie) {
      *out_cookie = kCookie;
      return true;
    });

  EXPECT_CALL(*send_mgmt_frame_event_, OnAck(_));

  client_interface_->SendMgmtFrame(
      vector<uint8_t>(std::begin(kTestFrame), std::end(kTestFrame)),
      send_mgmt_frame_event_, kMcs);
  frame_tx_status_event_handler_(kCookie, true);
}

/**
 * Transmitted frame was not ACKed.
 */
TEST_F(ClientInterfaceImplTest, SendMgmtFrameNotAcked) {
  EXPECT_CALL(*netlink_utils_,
      SendMgmtFrame(kTestInterfaceIndex,
          vector<uint8_t>(std::begin(kTestFrame), std::end(kTestFrame)),
          kAutoMcs, _))
    .WillOnce([](uint32_t interface_index, const vector<uint8_t>& frame,
        int32_t mcs, uint64_t* out_cookie) {
      *out_cookie = kCookie;
      return true;
    });

  EXPECT_CALL(*send_mgmt_frame_event_,
      OnFailure(send_mgmt_frame_event_->SEND_MGMT_FRAME_ERROR_NO_ACK));

  client_interface_->SendMgmtFrame(
      vector<uint8_t>(std::begin(kTestFrame), std::end(kTestFrame)),
      send_mgmt_frame_event_, kAutoMcs);
  frame_tx_status_event_handler_(kCookie, false);
}

/**
 * Transmission failed due to unknown NL80211 error.
 */
TEST_F(ClientInterfaceImplTest, SendMgmtFrameUnknownError) {
  EXPECT_CALL(*netlink_utils_,
      SendMgmtFrame(kTestInterfaceIndex,
        vector<uint8_t>(std::begin(kTestFrame), std::end(kTestFrame)), kAutoMcs, _))
    .WillOnce([](uint32_t interface_index, const vector<uint8_t>& frame,
        int32_t mcs, uint64_t* out_cookie) {
      return false;
    });

  EXPECT_CALL(*send_mgmt_frame_event_,
      OnFailure(send_mgmt_frame_event_->SEND_MGMT_FRAME_ERROR_UNKNOWN));

  client_interface_->SendMgmtFrame(
      vector<uint8_t>(std::begin(kTestFrame), std::end(kTestFrame)),
      send_mgmt_frame_event_, kAutoMcs);
}

/**
 * Received cookie was different than expected; No callback should be triggered.
 */
TEST_F(ClientInterfaceImplTest, SendMgmtFrameWrongCookie) {
  EXPECT_CALL(*netlink_utils_,
      SendMgmtFrame(kTestInterfaceIndex,
          vector<uint8_t>(std::begin(kTestFrame), std::end(kTestFrame)),
          kAutoMcs, _))
    .WillOnce([](uint32_t interface_index, const vector<uint8_t>& frame,
        int32_t mcs, uint64_t* out_cookie) {
      *out_cookie = kCookie;
      return true;
    });

  client_interface_->SendMgmtFrame(
      vector<uint8_t>(std::begin(kTestFrame), std::end(kTestFrame)),
      send_mgmt_frame_event_, kAutoMcs);
  frame_tx_status_event_handler_(
      kCookie + 1, // wrong cookie
      false);

  // StrictMock<MockISendMgmtFrameEvent> will fail if any unexpected method is
  // called, guaranteeing no interaction with the callback.
}

/**
 * frame_tx_status_event_handler_ triggered even though no transmission is in
 * progress. No callback should be triggered.
 */
TEST_F(ClientInterfaceImplTest, SendMgmtFrameNoTxCallbackTriggered) {
  EXPECT_CALL(*netlink_utils_,
      SendMgmtFrame(kTestInterfaceIndex,
          vector<uint8_t>(std::begin(kTestFrame), std::end(kTestFrame)),
          kAutoMcs, _))
    .WillOnce([](uint32_t interface_index, const vector<uint8_t>& frame,
        int32_t mcs, uint64_t* out_cookie) {
      *out_cookie = kCookie;
      return true;
    });

  EXPECT_CALL(*send_mgmt_frame_event_,
      OnFailure(send_mgmt_frame_event_->SEND_MGMT_FRAME_ERROR_NO_ACK));

  client_interface_->SendMgmtFrame(
      vector<uint8_t>(std::begin(kTestFrame), std::end(kTestFrame)),
      send_mgmt_frame_event_, kAutoMcs);
  frame_tx_status_event_handler_(kCookie, false);

  // transmission has finished here.

  // Now send another Tx status event.
  frame_tx_status_event_handler_(kCookie + 1, false);
  // StrictMock<MockISendMgmtFrameEvent> will fail if any unexpected method is
  // called, guaranteeing no more interaction with the callback.
}

/**
 * Second transmission was started even though no Tx Status event was received
 * for the first transmission. Should discard first transmission, and second
 * transmission should work normally.
 *
 * Since timeout of this SendMgmtFrame() is managed by framework, and framework
 * does not notify wificond when the call times out, wificond should still work
 * when a second call is made, even though it seems as though the first call is
 * still incomplete.
 */
TEST_F(ClientInterfaceImplTest, SendMgmtFrameSecondTxWhileFirstTxIncomplete) {
  EXPECT_CALL(*netlink_utils_,
      SendMgmtFrame(kTestInterfaceIndex,
          vector<uint8_t>(std::begin(kTestFrame), std::end(kTestFrame)),
          kAutoMcs, _))
    .WillOnce([](uint32_t interface_index, const vector<uint8_t>& frame,
        int32_t mcs, uint64_t* out_cookie) {
      *out_cookie = kCookie;
      return true;
    })
    .WillOnce([](uint32_t interface_index, const vector<uint8_t>& frame,
        int32_t mcs, uint64_t* out_cookie) {
      *out_cookie = kCookie + 1;
      return true;
    });

  // first transmission; no tx status
  client_interface_->SendMgmtFrame(
      vector<uint8_t>(std::begin(kTestFrame), std::end(kTestFrame)),
      send_mgmt_frame_event_, kAutoMcs);

  sp<StrictMock<MockISendMgmtFrameEvent>> send_mgmt_frame_event2{
      new StrictMock<MockISendMgmtFrameEvent>()};

  EXPECT_CALL(*send_mgmt_frame_event2,
      OnFailure(send_mgmt_frame_event_->SEND_MGMT_FRAME_ERROR_NO_ACK));

  // second transmission; yes tx status
  client_interface_->SendMgmtFrame(
      vector<uint8_t>(std::begin(kTestFrame), std::end(kTestFrame)),
      send_mgmt_frame_event2, kAutoMcs);
  frame_tx_status_event_handler_(kCookie + 1, false);

  // now trigger tx status for first call; nothing should happen (implicitly
  // verified by StrictMock).
  frame_tx_status_event_handler_(kCookie, false);
}

/**
 * Tests that internal state is reset correctly between calls by performing
 * two transmissions in sequence.
 */
TEST_F(ClientInterfaceImplTest, SendMgmtFrameInternalStateResetBetweenCalls) {
  EXPECT_CALL(*netlink_utils_,
      SendMgmtFrame(kTestInterfaceIndex,
          vector<uint8_t>(std::begin(kTestFrame), std::end(kTestFrame)),
          kAutoMcs, _))
    .WillOnce([](uint32_t interface_index, const vector<uint8_t>& frame,
        int32_t mcs, uint64_t* out_cookie) {
      *out_cookie = kCookie;
      return true;
    });

  EXPECT_CALL(*send_mgmt_frame_event_, OnAck(_));

  client_interface_->SendMgmtFrame(
      vector<uint8_t>(std::begin(kTestFrame), std::end(kTestFrame)),
      send_mgmt_frame_event_, kAutoMcs);
  frame_tx_status_event_handler_(kCookie, true);

  Mock::VerifyAndClearExpectations(netlink_utils_.get());
  Mock::VerifyAndClearExpectations(send_mgmt_frame_event_.get());

  uint64_t new_cookie = kCookie + 1;

  EXPECT_CALL(*netlink_utils_,
      SendMgmtFrame(kTestInterfaceIndex,
          vector<uint8_t>(std::begin(kTestFrame), std::end(kTestFrame)),
          kAutoMcs, _))
    .WillOnce([new_cookie](uint32_t interface_index,
        const vector<uint8_t>& frame, int32_t mcs, uint64_t* out_cookie) {
      *out_cookie = new_cookie;
      return true;
    });

  EXPECT_CALL(*send_mgmt_frame_event_,
      OnFailure(send_mgmt_frame_event_->SEND_MGMT_FRAME_ERROR_NO_ACK));

  client_interface_->SendMgmtFrame(
      vector<uint8_t>(std::begin(kTestFrame), std::end(kTestFrame)),
      send_mgmt_frame_event_, kAutoMcs);
  frame_tx_status_event_handler_(new_cookie, false);
}

}  // namespace wificond
}  // namespace android
