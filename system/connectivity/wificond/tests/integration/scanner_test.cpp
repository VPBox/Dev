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

#include <android-base/logging.h>
#include <gtest/gtest.h>
#include <utils/StrongPointer.h>

#include "android/net/wifi/IClientInterface.h"
#include "android/net/wifi/IWificond.h"
#include "wificond/tests/integration/process_utils.h"

using android::net::wifi::IClientInterface;
using android::net::wifi::IWificond;
using android::net::wifi::IWifiScannerImpl;
using android::wificond::tests::integration::ScopedDevModeWificond;
using std::unique_ptr;
using std::vector;

namespace android {
namespace wificond {
namespace {
const char kInterfaceName[] = "wlan0";

sp<IWifiScannerImpl> InitInterfaceAndReturnScanner(sp<IWificond> service) {
  sp<IWifiScannerImpl> scanner;
  sp<IClientInterface> client_interface;
  if (!service->createClientInterface(
      kInterfaceName, &client_interface).isOk()) {
    LOG(FATAL) << "Failed to create client interface";
    return nullptr;
  }
  if (client_interface == nullptr) {
    LOG(FATAL) << "Failed to get a valid client interface handler";
    return nullptr;
  }
  if (!client_interface->getWifiScannerImpl(&scanner).isOk()) {
    LOG(FATAL) << "Failed to get a WifiScannerImpl handler";
    return nullptr;
  }
  return scanner;
}

}  // namespace

TEST(ScannerTest, CanGetValidWifiScannerImpl) {
  ScopedDevModeWificond dev_mode;
  sp<IWificond> service = dev_mode.EnterDevModeOrDie();
  EXPECT_NE(nullptr, InitInterfaceAndReturnScanner(service).get());
}

}  // namespace wificond
}  // namespace android
