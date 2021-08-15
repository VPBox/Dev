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

#ifndef WIFICOND_SCAN_STATS_H
#define WIFICOND_SCAN_STATS_H

#include <vector>

#include <binder/Parcel.h>
#include <binder/Parcelable.h>

namespace com {
namespace android {
namespace server {
namespace wifi {
namespace wificond {

class NativeScanStats : public ::android::Parcelable {
 public:
  NativeScanStats();
  NativeScanStats(uint32_t num_scans_requested_by_wifi,
                  uint32_t num_scans_serviced_by_wifi,
                  uint32_t subscription_duration_ms, uint32_t scan_duration_ms,
                  uint32_t num_channels_scanned,
                  std::vector<uint8_t> histogram_channels);

  bool operator==(const NativeScanStats&) const;
  ::android::status_t writeToParcel(::android::Parcel* parcel) const override;
  ::android::status_t readFromParcel(const ::android::Parcel* parcel) override;
  void DebugLog();

  uint32_t num_scans_requested_by_wifi_;
  uint32_t num_scans_serviced_by_wifi_;
  uint32_t subscription_duration_ms_;
  uint32_t scan_duration_ms_;
  uint32_t num_channels_scanned_;
  uint32_t time_stamp_;
  std::vector<uint8_t> histogram_channels_;
};

}  // namespace wificond
}  // namespace wifi
}  // namespace server
}  // namespace android
}  // namespace com
#endif  // WIFICOND_SCAN_STATS_H
