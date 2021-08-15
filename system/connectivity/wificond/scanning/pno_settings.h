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

#ifndef WIFICOND_SCANNING_PNO_SETTINGS_H_
#define WIFICOND_SCANNING_PNO_SETTINGS_H_

#include <vector>

#include <binder/Parcel.h>
#include <binder/Parcelable.h>

#include "wificond/scanning/pno_network.h"

namespace com {
namespace android {
namespace server {
namespace wifi {
namespace wificond {

class PnoSettings : public ::android::Parcelable {
 public:
  static const uint32_t kFastScanIterations;
  static const uint32_t kSlowScanIntervalMultiplier;

  PnoSettings()
      : interval_ms_(0),
        min_2g_rssi_(0),
        min_5g_rssi_(0) {}
  bool operator==(const PnoSettings& rhs) const {
    return (pno_networks_ == rhs.pno_networks_ &&
            min_2g_rssi_ == rhs.min_2g_rssi_ &&
            min_5g_rssi_ == rhs.min_5g_rssi_);
  }
  ::android::status_t writeToParcel(::android::Parcel* parcel) const override;
  ::android::status_t readFromParcel(const ::android::Parcel* parcel) override;

  int32_t interval_ms_;
  int32_t min_2g_rssi_;
  int32_t min_5g_rssi_;
  std::vector<PnoNetwork> pno_networks_;
};

}  // namespace wificond
}  // namespace wifi
}  // namespace server
}  // namespace android
}  // namespace com

#endif  // WIFICOND_SCANNING_PNO_SETTINGS_H_
