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

#include <string>
#include <vector>

#include <gtest/gtest.h>
#include <utils/StrongPointer.h>
#include <wifi_system/interface_tool.h>

#include "android/net/wifi/IClientInterface.h"
#include "android/net/wifi/IWificond.h"
#include "wificond/tests/integration/process_utils.h"

using android::net::wifi::IClientInterface;
using android::net::wifi::IWificond;
using android::wifi_system::InterfaceTool;
using android::wificond::tests::integration::ScopedDevModeWificond;
using std::string;
using std::vector;

namespace android {
namespace wificond {
namespace {

const char kInterfaceName[] = "wlan0";
}  // namespace

TEST(ClientInterfaceTest, CanCreateClientInterfaces) {
  ScopedDevModeWificond dev_mode;
  sp<IWificond> service = dev_mode.EnterDevModeOrDie();

  // We should be able to create an client interface.
  sp<IClientInterface> client_interface;
  EXPECT_TRUE(service->createClientInterface(
      kInterfaceName, &client_interface).isOk());
  EXPECT_NE(nullptr, client_interface.get());

  // The interface should start out down.
  string if_name;
  EXPECT_TRUE(client_interface->getInterfaceName(&if_name).isOk());
  EXPECT_TRUE(!if_name.empty());
  InterfaceTool if_tool;
  EXPECT_FALSE(if_tool.GetUpState(if_name.c_str()));

  // Mark the interface as up, just to test that we mark it down on tearDown.
  EXPECT_TRUE(if_tool.SetUpState(if_name.c_str(), true));
  EXPECT_TRUE(if_tool.GetUpState(if_name.c_str()));

  // We should not be able to create two client interfaces.
  sp<IClientInterface> client_interface2;
  EXPECT_TRUE(service->createClientInterface(
      kInterfaceName, &client_interface2).isOk());
  EXPECT_EQ(nullptr, client_interface2.get());

  // We can tear down the created interface.
  bool succes = false;
  EXPECT_TRUE(service->tearDownClientInterface(kInterfaceName, &succes).isOk());
  EXPECT_TRUE(succes);
  EXPECT_FALSE(if_tool.GetUpState(if_name.c_str()));

  // Teardown everything at the end of the test.
  EXPECT_TRUE(service->tearDownInterfaces().isOk());
}

TEST(ClientInterfaceTest, CanGetMacAddress) {
  ScopedDevModeWificond dev_mode;
  sp<IWificond> service = dev_mode.EnterDevModeOrDie();
  sp<IClientInterface> client_interface;
  EXPECT_TRUE(service->createClientInterface(
      kInterfaceName, &client_interface).isOk());
  ASSERT_NE(nullptr, client_interface.get());
  vector<uint8_t> mac_address;
  EXPECT_TRUE(client_interface->getMacAddress(&mac_address).isOk());
  EXPECT_TRUE(mac_address.size() == 6);
}

}  // namespace wificond
}  // namespace android
