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

#include <vector>

#include <gtest/gtest.h>
#include <utils/StrongPointer.h>
#include <wifi_system/interface_tool.h>

#include "android/net/wifi/IApInterface.h"
#include "android/net/wifi/IWificond.h"
#include "wificond/tests/integration/process_utils.h"
#include "wificond/tests/mock_ap_interface_event_callback.h"

using android::net::wifi::IApInterface;
using android::net::wifi::IWificond;
using android::wifi_system::InterfaceTool;
using android::wificond::MockApInterfaceEventCallback;
using android::wificond::tests::integration::ScopedDevModeWificond;
using android::wificond::tests::integration::WaitForTrue;
using std::string;
using std::vector;

namespace android {
namespace wificond {
namespace {
const char kInterfaceName[] = "wlan0";
}  // namespace

TEST(ApInterfaceTest, CanCreateApInterfaces) {
  ScopedDevModeWificond dev_mode;
  sp<IWificond> service = dev_mode.EnterDevModeOrDie();

  // We should be able to create an AP interface.
  sp<IApInterface> ap_interface;
  EXPECT_TRUE(service->createApInterface(kInterfaceName, &ap_interface).isOk());
  EXPECT_NE(nullptr, ap_interface.get());

  // The interface should start out down.
  string if_name;
  EXPECT_TRUE(ap_interface->getInterfaceName(&if_name).isOk());
  EXPECT_TRUE(!if_name.empty());
  InterfaceTool if_tool;
  EXPECT_FALSE(if_tool.GetUpState(if_name.c_str()));

  // Mark the interface as up, just to test that we mark it down on tearDown.
  EXPECT_TRUE(if_tool.SetUpState(if_name.c_str(), true));
  EXPECT_TRUE(if_tool.GetUpState(if_name.c_str()));

  // We should not be able to create two AP interfaces.
  sp<IApInterface> ap_interface2;
  EXPECT_TRUE(service->createApInterface(
      kInterfaceName, &ap_interface2).isOk());
  EXPECT_EQ(nullptr, ap_interface2.get());

  // We can tear down the created interface.
  bool success = false;
  EXPECT_TRUE(service->tearDownApInterface(kInterfaceName, &success).isOk());
  EXPECT_TRUE(success);
  EXPECT_FALSE(if_tool.GetUpState(if_name.c_str()));

  // Teardown everything at the end of the test.
  EXPECT_TRUE(service->tearDownInterfaces().isOk());
}
}  // namespace wificond
}  // namespace android
