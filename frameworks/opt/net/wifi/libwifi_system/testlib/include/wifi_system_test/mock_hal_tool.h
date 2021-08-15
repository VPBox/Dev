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

#ifndef ANDROID_WIFI_SYSTEM_TEST_MOCK_HAL_TOOL_H
#define ANDROID_WIFI_SYSTEM_TEST_MOCK_HAL_TOOL_H

#include <gmock/gmock.h>
#include <wifi_system/hal_tool.h>

namespace android {
namespace wifi_system {

class MockHalTool : public HalTool {
 public:
  ~MockHalTool() override = default;

  MOCK_METHOD1(InitFunctionTable, bool(wifi_hal_fn*));
  MOCK_METHOD1(CanGetValidChannels, bool(wifi_hal_fn*));

};  // class MockHalTool

}  // namespace wifi_system
}  // namespace android

#endif  // ANDROID_WIFI_SYSTEM_TEST_MOCK_HAL_TOOL_H
