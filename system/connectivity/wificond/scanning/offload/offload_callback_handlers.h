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
#ifndef ANDROID_HARDWARE_WIFI_OFFLOAD_V1_0_OFFLOADCALLBACK_HANDLERS_H
#define ANDROID_HARDWARE_WIFI_OFFLOAD_V1_0_OFFLOADCALLBACK_HANDLERS_H

#include <android/hardware/wifi/offload/1.0/IOffload.h>
#include <vector>

namespace android {
namespace wificond {

class OffloadCallbackHandlers {
 public:
  virtual ~OffloadCallbackHandlers() {}

  virtual void OnScanResultHandler(
      const std::vector<::android::hardware::wifi::offload::V1_0::ScanResult>&
          scanResult) = 0;
  virtual void OnErrorHandler(
      const ::android::hardware::wifi::offload::V1_0::OffloadStatus&
          status) = 0;
};

}  // namespace wificond
}  // namespace android

#endif  // ANDROID_HARDWARE_WIFI_OFFLOAD_V1_0_OFFLOADCALLBACK_HANDLERS_H
