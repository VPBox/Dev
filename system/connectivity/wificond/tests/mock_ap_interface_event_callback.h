/*
 * Copyright (C) 2017 The Android Open Source Project
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

#ifndef WIFICOND_TESTS_MOCK_AP_INTERFACE_EVENT_CALLBACK_H_
#define WIFICOND_TESTS_MOCK_AP_INTERFACE_EVENT_CALLBACK_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "android/net/wifi/IApInterfaceEventCallback.h"

namespace android {
namespace wificond {

class MockApInterfaceEventCallback
    : public net::wifi::IApInterfaceEventCallback {
 public:
  MockApInterfaceEventCallback() = default;
  ~MockApInterfaceEventCallback() = default;

  MOCK_METHOD0(onAsBinder, IBinder*());
  MOCK_METHOD1(onNumAssociatedStationsChanged, ::android::binder::Status(int));
  MOCK_METHOD2(onSoftApChannelSwitched, ::android::binder::Status(int, int));
};

}  // namespace wificond
}  // namespace android

#endif  // WIFICOND_TESTS_MOCK_AP_INTERFACE_EVENT_CALLBACK_H_
