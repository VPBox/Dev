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

#ifndef WIFICOND_TESTS_MOCK_OFFLOAD_CALLBACK_HANDLERS_H_
#define WIFICOND_TESTS_MOCK_OFFLOAD_CALLBACK_HANDLERS_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <android/hardware/wifi/offload/1.0/IOffload.h>
#include "wificond/scanning/offload/offload_callback_handlers.h"

namespace android {
namespace wificond {

class MockOffloadCallbackHandlers : public OffloadCallbackHandlers {
 public:
  MockOffloadCallbackHandlers();
  ~MockOffloadCallbackHandlers() override = default;

  MOCK_METHOD1(
      OnScanResultHandler,
      void(const std::vector<
           android::hardware::wifi::offload::V1_0::ScanResult>& scanResult));
  MOCK_METHOD1(OnErrorHandler,
               void(const android::hardware::wifi::offload::V1_0::OffloadStatus&
                    status));
};

}  // namespace wificond
}  // namespace android

#endif  // WIFICOND_TESTS_MOCK_OFFLOAD_CALLBACK_HANDLERS_H_
