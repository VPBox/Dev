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

#ifndef ANDROID_WIFI_SYSTEM_TEST_MOCK_SUPPLICANT_MANAGER_H
#define ANDROID_WIFI_SYSTEM_TEST_MOCK_SUPPLICANT_MANAGER_H

#include <gmock/gmock.h>
#include <wifi_system/supplicant_manager.h>

namespace android {
namespace wifi_system {

class MockSupplicantManager : public SupplicantManager {
 public:
  ~MockSupplicantManager() override = default;

  MOCK_METHOD0(StartSupplicant, bool());
  MOCK_METHOD0(StopSupplicant, bool());
  MOCK_METHOD0(IsSupplicantRunning, bool());

};  // class MockSupplicantManager

}  // namespace wifi_system
}  // namespace android

#endif  // ANDROID_WIFI_SYSTEM_TEST_MOCK_SUPPLICANT_MANAGER_H
