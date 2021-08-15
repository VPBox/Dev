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

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <wifi_system_test/mock_interface_tool.h>

#include "android/net/wifi/IApInterface.h"
#include "wificond/tests/mock_netlink_manager.h"
#include "wificond/tests/mock_netlink_utils.h"
#include "wificond/tests/mock_scan_utils.h"
#include "wificond/server.h"

using android::net::wifi::IApInterface;
using android::net::wifi::IClientInterface;
using android::wifi_system::InterfaceTool;
using android::wifi_system::MockInterfaceTool;
using std::unique_ptr;
using std::vector;
using testing::Eq;
using testing::Invoke;
using testing::NiceMock;
using testing::Return;
using testing::Sequence;
using testing::StrEq;
using testing::_;

using namespace std::placeholders;

namespace android {
namespace wificond {
namespace {

const char kFakeInterfaceName[] = "testif0";
const char kFakeInterfaceName1[] = "testif1";
const char kFakeInterfaceNameP2p[] = "testif-p2p0";
const char kFateInterfaceNameInvalid[] = "testif-invalid";
const uint32_t kFakeInterfaceIndex = 34;
const uint32_t kFakeInterfaceIndex1 = 36;
const uint32_t kFakeInterfaceIndexP2p = 36;
const std::array<uint8_t, ETH_ALEN> kFakeInterfaceMacAddress = {0x45, 0x54, 0xad, 0x67, 0x98, 0xf6};
const std::array<uint8_t, ETH_ALEN> kFakeInterfaceMacAddress1 = {0x05, 0x04, 0xef, 0x27, 0x12, 0xff};
const std::array<uint8_t, ETH_ALEN> kFakeInterfaceMacAddressP2p = {0x15, 0x24, 0xef, 0x27, 0x12, 0xff};

// This is a helper function to mock the behavior of
// NetlinkUtils::GetInterfaces().
// |wiphy_index| is mapped to first parameters of GetInterfaces().
// |response| is mapped to second parameters of GetInterfaces().
// |mock_response| and |mock_return_value| are additional parameters used
// for specifying expected results,
bool MockGetInterfacesResponse(
    const vector<InterfaceInfo>& mock_response,
    bool mock_return_value,
    uint32_t wiphy_index,
    vector<InterfaceInfo>* response) {
  for (const auto& interface : mock_response) {
    response->emplace_back(interface);
  }
  return mock_return_value;
}

class ServerTest : public ::testing::Test {
 protected:
  void SetUp() override {
    ON_CALL(*if_tool_, SetUpState(_, _)).WillByDefault(Return(true));
    ON_CALL(*netlink_utils_, GetWiphyIndex(_)).WillByDefault(Return(true));
    ON_CALL(*netlink_utils_, GetWiphyIndex(_, _)).WillByDefault(Return(true));
    ON_CALL(*netlink_utils_, GetInterfaces(_, _))
      .WillByDefault(Invoke(bind(
          MockGetInterfacesResponse, mock_interfaces, true, _1, _2)));
  }

  NiceMock<MockInterfaceTool>* if_tool_ = new NiceMock<MockInterfaceTool>;

  unique_ptr<NiceMock<MockNetlinkManager>> netlink_manager_{
      new NiceMock<MockNetlinkManager>()};

  unique_ptr<NiceMock<MockNetlinkUtils>> netlink_utils_{
      new NiceMock<MockNetlinkUtils>(netlink_manager_.get())};
  unique_ptr<NiceMock<MockScanUtils>> scan_utils_{
      new NiceMock<MockScanUtils>(netlink_manager_.get())};
  const vector<InterfaceInfo> mock_interfaces = {
      // Client interface
      InterfaceInfo(
          kFakeInterfaceIndex,
          std::string(kFakeInterfaceName),
          std::array<uint8_t, ETH_ALEN>(kFakeInterfaceMacAddress)),
      // AP Interface
      InterfaceInfo(
          kFakeInterfaceIndex1,
          std::string(kFakeInterfaceName1),
          std::array<uint8_t, ETH_ALEN>(kFakeInterfaceMacAddress1)),
      // p2p interface
      InterfaceInfo(
          kFakeInterfaceIndexP2p,
          std::string(kFakeInterfaceNameP2p),
          std::array<uint8_t, ETH_ALEN>(kFakeInterfaceMacAddressP2p))
  };

  Server server_{unique_ptr<InterfaceTool>(if_tool_),
                 netlink_utils_.get(),
                 scan_utils_.get()};
};  // class ServerTest

}  // namespace

TEST_F(ServerTest, CanSetUpApInterface) {
  sp<IApInterface> ap_if;
  EXPECT_CALL(*netlink_utils_, SubscribeRegDomainChange(_, _));

  EXPECT_TRUE(server_.createApInterface(kFakeInterfaceName, &ap_if).isOk());
  EXPECT_NE(nullptr, ap_if.get());
}

TEST_F(ServerTest, CanSupportMultipleInterfaces) {
  sp<IApInterface> ap_if;

  EXPECT_TRUE(server_.createApInterface(kFakeInterfaceName, &ap_if).isOk());
  EXPECT_NE(nullptr, ap_if.get());

  sp<IApInterface> second_ap_if;
  // We won't throw on a second interface request.
  EXPECT_TRUE(server_.createApInterface(kFakeInterfaceName, &second_ap_if).isOk());
  // But this time we won't get an interface back.
  EXPECT_NE(nullptr, second_ap_if.get());
}

TEST_F(ServerTest, CanDestroyInterfaces) {
  sp<IApInterface> ap_if;

  EXPECT_TRUE(server_.createApInterface(kFakeInterfaceName, &ap_if).isOk());

  // When we tear down the interface, we expect the driver to be unloaded.
  EXPECT_CALL(*netlink_utils_, UnsubscribeRegDomainChange(_));
  EXPECT_TRUE(server_.tearDownInterfaces().isOk());
  // After a tearDown, we should be able to create another interface.
  EXPECT_TRUE(server_.createApInterface(kFakeInterfaceName, &ap_if).isOk());
}

TEST_F(ServerTest, CanTeardownApInterface) {
  sp<IApInterface> ap_if;

  EXPECT_TRUE(server_.createApInterface(kFakeInterfaceName, &ap_if).isOk());
  EXPECT_NE(nullptr, ap_if.get());

  // Try to remove an invalid iface name, this should fail.
  bool success = true;
  EXPECT_TRUE(server_.tearDownApInterface(
      kFateInterfaceNameInvalid, &success).isOk());
  EXPECT_FALSE(success);

  EXPECT_TRUE(server_.tearDownApInterface(kFakeInterfaceName, &success).isOk());
  EXPECT_TRUE(success);
}

TEST_F(ServerTest, CanTeardownClientInterface) {
  sp<IClientInterface> client_if;

  EXPECT_TRUE(server_.createClientInterface(
      kFakeInterfaceName, &client_if).isOk());
  EXPECT_NE(nullptr, client_if.get());

  // Try to remove an invalid iface name, this should fail.
  bool success = true;
  EXPECT_TRUE(server_.tearDownClientInterface(
      kFateInterfaceNameInvalid, &success).isOk());
  EXPECT_FALSE(success);

  EXPECT_TRUE(server_.tearDownClientInterface(
      kFakeInterfaceName, &success).isOk());
  EXPECT_TRUE(success);
}

TEST_F(ServerTest, CanCreateTeardownApAndClientInterface) {
  sp<IClientInterface> client_if;
  sp<IApInterface> ap_if;

  EXPECT_TRUE(server_.createClientInterface(kFakeInterfaceName, &client_if).isOk());
  EXPECT_NE(nullptr, client_if.get());

  EXPECT_TRUE(server_.createApInterface(kFakeInterfaceName1, &ap_if).isOk());
  EXPECT_NE(nullptr, ap_if.get());

  bool success = true;
  // Try to remove an invalid iface name, this should fail.
  EXPECT_TRUE(server_.tearDownClientInterface(
      kFateInterfaceNameInvalid, &success).isOk());
  EXPECT_FALSE(success);
  EXPECT_TRUE(server_.tearDownApInterface(
      kFateInterfaceNameInvalid, &success).isOk());
  EXPECT_FALSE(success);

  EXPECT_TRUE(server_.tearDownClientInterface(
      kFakeInterfaceName, &success).isOk());
  EXPECT_TRUE(success);

  EXPECT_TRUE(server_.tearDownApInterface(
      kFakeInterfaceName1, &success).isOk());
  EXPECT_TRUE(success);
}

TEST_F(ServerTest, CanDestroyApAndClientInterfaces) {
  sp<IClientInterface> client_if;
  sp<IApInterface> ap_if;

  EXPECT_TRUE(server_.createClientInterface(
      kFakeInterfaceName, &client_if).isOk());
  EXPECT_NE(nullptr, client_if.get());

  EXPECT_TRUE(server_.createApInterface(kFakeInterfaceName1, &ap_if).isOk());
  EXPECT_NE(nullptr, ap_if.get());

  // When we tear down the interfaces, we expect the iface to be unloaded.
  EXPECT_CALL(*if_tool_, SetUpState(StrEq(kFakeInterfaceName), Eq(false))).Times(2);
  EXPECT_CALL(*if_tool_, SetUpState(StrEq(kFakeInterfaceName1), Eq(false))).Times(2);
  EXPECT_CALL(*if_tool_, SetUpState(StrEq(kFakeInterfaceNameP2p), Eq(false)));

  EXPECT_TRUE(server_.tearDownInterfaces().isOk());
}
}  // namespace wificond
}  // namespace android
