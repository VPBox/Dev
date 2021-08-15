/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef ANDROID_WIFI_SYSTEM_TEST_MOCK_INTERFACE_TOOL_H
#define ANDROID_WIFI_SYSTEM_TEST_MOCK_INTERFACE_TOOL_H

#include <gmock/gmock.h>
#include <wifi_system/interface_tool.h>

namespace android {
namespace wifi_system {

class MockInterfaceTool : public InterfaceTool {
 public:
  ~MockInterfaceTool() override = default;

  MOCK_METHOD1(GetUpState, bool(const char* if_name));
  MOCK_METHOD2(SetUpState, bool(const char* if_name, bool request_up));
  MOCK_METHOD1(SetWifiUpState, bool(bool request_up));
  MOCK_METHOD2(SetMacAddress, bool(const char* if_name,
    const std::array<uint8_t, ETH_ALEN>& address));
  MOCK_METHOD1(GetFactoryMacAddress, std::array<uint8_t, ETH_ALEN>(const char* if_name));

};  // class MockInterfaceTool

}  // namespace wifi_system
}  // namespace android

#endif  // ANDROID_WIFI_SYSTEM_TEST_MOCK_INTERFACE_TOOL_H
