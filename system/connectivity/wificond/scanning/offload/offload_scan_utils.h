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
#ifndef WIFICOND_OFFLOAD_SCAN_UTILS_H_
#define WIFICOND_OFFLOAD_SCAN_UTILS_H_

#include <android/hardware/wifi/offload/1.0/IOffload.h>
#include "wificond/scanning/offload/offload_callback.h"

#include <vector>

namespace com {
namespace android {
namespace server {
namespace wifi {
namespace wificond {

class NativeScanResult;
class NativeScanStats;

}  // namespace wificond
}  // namespace wifi
}  // namespace server
}  // namespace android
}  // namespace com

namespace android {
namespace wificond {

// Provides utility methods for Offload Scan Manager
class OffloadScanUtils {
 public:
  static bool convertToNativeScanResults(
      const std::vector<android::hardware::wifi::offload::V1_0::ScanResult>&,
      std::vector<::com::android::server::wifi::wificond::NativeScanResult>*);
  static android::hardware::wifi::offload::V1_0::ScanParam createScanParam(
      const std::vector<std::vector<uint8_t>>& ssid_list,
      const std::vector<uint32_t>& frequency_list, uint32_t scan_interval_ms);
  /* Creates ScanFilter using ssids, security flags and rssi_threshold
   * The caller must ensure that the number of ssids match the number of
   * security flags, also there must be ordering maintained among the two lists.
   * For eg: (ssid[0], flags[0]) describe the SSID and security settings of one
   * network
   */
  static android::hardware::wifi::offload::V1_0::ScanFilter createScanFilter(
      const std::vector<std::vector<uint8_t>>& ssids,
      const std::vector<uint8_t>& flags, int8_t rssi_threshold);
  static ::com::android::server::wifi::wificond::NativeScanStats
      convertToNativeScanStats(
          const android::hardware::wifi::offload::V1_0::ScanStats& /* scanStats */);
};

}  // namespace wificond
}  // namespace android

#endif  // WIFICOND_OFFLOAD_SCAN_UTILS_H_
