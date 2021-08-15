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

#ifndef WIFICOND_TEST_MOCK_OFFLOAD_H
#define WIFICOND_TEST_MOCK_OFFLOAD_H

#include <gmock/gmock.h>

#include <android/hardware/wifi/offload/1.0/IOffload.h>

namespace android {
namespace wificond {

typedef std::function<void(
    const android::hardware::wifi::offload::V1_0::OffloadStatus& status,
    const android::hardware::wifi::offload::V1_0::ScanStats& scanStats)>
        OnScanStatsCallback;
typedef std::function<void(
    const android::hardware::wifi::offload::V1_0::OffloadStatus& status)>
        StatusCallback;

class MockOffload : public android::hardware::wifi::offload::V1_0::IOffload {
 public:
  MockOffload();
  ~MockOffload() override = default;

  MOCK_METHOD3(
      configureScans,
      android::hardware::Return<void>(
          const android::hardware::wifi::offload::V1_0::ScanParam& param,
          const android::hardware::wifi::offload::V1_0::ScanFilter& filter,
          StatusCallback _hidl_cb));
  MOCK_METHOD1(getScanStats,
               android::hardware::Return<void>(OnScanStatsCallback cb));
  MOCK_METHOD2(subscribeScanResults,
               android::hardware::Return<void>(uint32_t delayMs,
                                               StatusCallback _hidl_cb));
  MOCK_METHOD0(unsubscribeScanResults, android::hardware::Return<void>());
  MOCK_METHOD1(
      setEventCallback,
      android::hardware::Return<void>(
          const android::sp<
              android::hardware::wifi::offload::V1_0::IOffloadCallback>& cb));
};

}  // namespace wificond
}  // namespace android

#endif  // WIFICOND_TEST_MOCK_OFFLOAD_H
