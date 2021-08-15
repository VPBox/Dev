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

#ifndef WIFICOND_OFFLOAD_TEST_UTILS_H_
#define WIFICOND_OFFLOAD_TEST_UTILS_H_

#include <android/hardware/wifi/offload/1.0/IOffload.h>
#include <vector>

#include "wificond/scanning/offload/scan_stats.h"
#include "wificond/scanning/scan_result.h"

namespace android {
namespace wificond {

class OffloadTestUtils {
 public:
  static std::vector<android::hardware::wifi::offload::V1_0::ScanResult>
      createOffloadScanResults();
  static android::hardware::wifi::offload::V1_0::ScanStats createScanStats(
      ::com::android::server::wifi::wificond::
          NativeScanStats* /* nativeScanStats */);
  static android::hardware::wifi::offload::V1_0::OffloadStatus
      createOffloadStatus(
          android::hardware::wifi::offload::V1_0::OffloadStatusCode code);
  static android::hardware::wifi::offload::V1_0::OffloadStatus
      createOffloadStatus(
          android::hardware::wifi::offload::V1_0::OffloadStatusCode code,
          const std::string& desc);
};

}  // namespace wificond
}  // namespace android

#endif  // WIFICOND_OFFLOAD_TEST_UTILS_H
