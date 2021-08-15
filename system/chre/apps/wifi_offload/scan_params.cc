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

#include "chre/apps/wifi_offload/scan_params.h"
#include "chre/apps/wifi_offload/channel_histogram.h"
#include "chre/apps/wifi_offload/vector_serialization.h"

namespace wifi_offload {

ScanParams::ScanParams() : disconnected_mode_scan_interval_ms_(0) {}

bool ScanParams::operator==(const ScanParams &other) const {
  if (this == &other) {
    return true;
  }
  return ssids_to_scan_ == other.ssids_to_scan_ &&
         frequencies_to_scan_mhz_ == other.frequencies_to_scan_mhz_ &&
         disconnected_mode_scan_interval_ms_ ==
             other.disconnected_mode_scan_interval_ms_;
}

flatbuffers::Offset<ScanParams::FbsType> ScanParams::Serialize(
    flatbuffers::FlatBufferBuilder *builder) const {
  auto ssid_vec = SerializeVector(ssids_to_scan_, builder);
  auto freq_vec = builder->CreateVector(frequencies_to_scan_mhz_);
  return fbs::CreateScanParams(*builder, ssid_vec, freq_vec,
                               disconnected_mode_scan_interval_ms_);
}

bool ScanParams::Deserialize(const ScanParams::FbsType &fbs_params) {
  const auto &ssid_vec = fbs_params.ssids_to_scan();
  if (ssid_vec == nullptr ||
      !DeserializeVector<Ssid>(*ssid_vec, &ssids_to_scan_)) {
    LOGE("Failed to deserialize ScanParams. Null or incomplete members.");
    return false;
  }

  const auto &freq_vec = fbs_params.frequencies_to_scan_mhz();
  if (freq_vec == nullptr) {
    LOGE("Failed to deserialize ScanParams. Null or incomplete members.");
    return false;
  }
  frequencies_to_scan_mhz_.clear();
  frequencies_to_scan_mhz_.reserve(freq_vec->size());
  for (const auto &freq : *freq_vec) {
    if (!ChannelHistogram::IsSupportedFrequency(freq)) {
      LOGE("Failed to deserialize ScanParams. Invalid frequency to scan.");
      return false;
    }
    frequencies_to_scan_mhz_.push_back(freq);
  }
  disconnected_mode_scan_interval_ms_ =
      fbs_params.disconnected_mode_scan_interval_ms();

  return true;
}

void ScanParams::Log() const {
  LOGI("ScanParams:");
  LOGI("  disconnected mode scan interval (ms): %" PRIu32,
       disconnected_mode_scan_interval_ms_);
  LOGI("  number of ssids to scan: %zu", ssids_to_scan_.size());
  for (const auto &ssid : ssids_to_scan_) {
    ssid.Log();
  }
  LOGI("  number of frequencies to scan: %zu", frequencies_to_scan_mhz_.size());
  for (const auto &freq : frequencies_to_scan_mhz_) {
    LOGI("  frequency: %" PRIu32, freq);
  }
}

}  // namespace wifi_offload
