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

#include "wificond/scanning/offload/scan_stats.h"

#include <android-base/logging.h>

#include "wificond/parcelable_utils.h"

using android::status_t;

namespace com {
namespace android {
namespace server {
namespace wifi {
namespace wificond {

NativeScanStats::NativeScanStats(uint32_t num_scans_requested_by_wifi,
                                 uint32_t num_scans_serviced_by_wifi,
                                 uint32_t subscription_duration_ms,
                                 uint32_t scan_duration_ms,
                                 uint32_t num_channels_scanned,
                                 std::vector<uint8_t> histogram_channels)
    : num_scans_requested_by_wifi_(num_scans_requested_by_wifi),
      num_scans_serviced_by_wifi_(num_scans_serviced_by_wifi),
      subscription_duration_ms_(subscription_duration_ms),
      scan_duration_ms_(scan_duration_ms),
      num_channels_scanned_(num_channels_scanned),
      time_stamp_(0),
      histogram_channels_(histogram_channels) {}

NativeScanStats::NativeScanStats()
    : num_scans_requested_by_wifi_(0),
      num_scans_serviced_by_wifi_(0),
      subscription_duration_ms_(0),
      scan_duration_ms_(0),
      num_channels_scanned_(0),
      time_stamp_(0) {}

bool NativeScanStats::operator==(const NativeScanStats& rhs) const {
  if ((rhs.num_scans_requested_by_wifi_ != num_scans_requested_by_wifi_) ||
      (rhs.num_scans_serviced_by_wifi_ != num_scans_serviced_by_wifi_) ||
      (rhs.scan_duration_ms_ != scan_duration_ms_) ||
      (rhs.num_channels_scanned_ != num_channels_scanned_)) {
    return false;
  }
  if (rhs.histogram_channels_.size() != histogram_channels_.size()) {
    return false;
  }
  for (size_t i = 0; i < histogram_channels_.size(); i++) {
    if (rhs.histogram_channels_[i] != histogram_channels_[i]) {
      return false;
    }
  }
  return true;
}

status_t NativeScanStats::writeToParcel(::android::Parcel* parcel) const {
  RETURN_IF_FAILED(parcel->writeUint32(num_scans_requested_by_wifi_));
  RETURN_IF_FAILED(parcel->writeUint32(num_scans_serviced_by_wifi_));
  RETURN_IF_FAILED(parcel->writeUint32(subscription_duration_ms_));
  RETURN_IF_FAILED(parcel->writeUint32(scan_duration_ms_));
  RETURN_IF_FAILED(parcel->writeUint32(num_channels_scanned_));
  RETURN_IF_FAILED(parcel->writeByteVector(histogram_channels_));
  return ::android::OK;
}

status_t NativeScanStats::readFromParcel(const ::android::Parcel* parcel) {
  RETURN_IF_FAILED(parcel->readUint32(&num_scans_requested_by_wifi_));
  RETURN_IF_FAILED(parcel->readUint32(&num_scans_serviced_by_wifi_));
  RETURN_IF_FAILED(parcel->readUint32(&subscription_duration_ms_));
  RETURN_IF_FAILED(parcel->readUint32(&scan_duration_ms_));
  RETURN_IF_FAILED(parcel->readUint32(&num_channels_scanned_));
  RETURN_IF_FAILED(parcel->readByteVector(&histogram_channels_));
  return ::android::OK;
}

void NativeScanStats::DebugLog() {
  LOG(INFO) << "num_scans_requested_by_wifi=" << num_scans_requested_by_wifi_;
  LOG(INFO) << "num_scans_serviced_by_wifi=" << num_scans_serviced_by_wifi_;
  LOG(INFO) << "subscription_duration=" << subscription_duration_ms_;
  LOG(INFO) << "scan_duration_ms_=" << scan_duration_ms_;
  LOG(INFO) << "num_channels_scanned=" << num_channels_scanned_;
  for (size_t i = 0; i < histogram_channels_.size(); i++) {
    if (histogram_channels_[i] > 0) {
      LOG(INFO) << "Channel=" << i << " ScannedTimes="
                << static_cast<uint32_t>(histogram_channels_[i]);
    }
  }
}

}  // namespace wificond
}  // namespace wifi
}  // namespace server
}  // namespace android
}  // namespace com
