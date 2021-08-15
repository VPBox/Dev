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

#include "chre/apps/wifi_offload/scan_filter.h"
#include "chre/apps/wifi_offload/vector_serialization.h"

namespace wifi_offload {

ScanFilter::ScanFilter() : min_rssi_threshold_dbm_(0) {}

bool ScanFilter::operator==(const ScanFilter &other) const {
  if (this == &other) {
    return true;
  }
  return networks_to_match_ == other.networks_to_match_ &&
         min_rssi_threshold_dbm_ == other.min_rssi_threshold_dbm_;
}

flatbuffers::Offset<ScanFilter::FbsType> ScanFilter::Serialize(
    flatbuffers::FlatBufferBuilder *builder) const {
  auto netList = SerializeVector(networks_to_match_, builder);
  return fbs::CreateScanFilter(*builder, netList, min_rssi_threshold_dbm_);
}

bool ScanFilter::Deserialize(const ScanFilter::FbsType &fbs_filter) {
  const auto &fbsNetList = fbs_filter.networks_to_match();
  if (fbsNetList == nullptr) {
    LOGE("Failed to deserialize ScanFilter. Null or incomplete members.");
    return false;
  }

  if (!DeserializeVector<PreferredNetwork>(*fbsNetList, &networks_to_match_)) {
    LOGE("Failed to deserialize ScanFilter. Null or incomplete members.");
    return false;
  }

  min_rssi_threshold_dbm_ = fbs_filter.min_rssi_threshold_dbm();
  return true;
}

void ScanFilter::Log() const {
  LOGI("ScanFilter:");
  LOGI("  min rssi threshold: %" PRId8 "dBm", min_rssi_threshold_dbm_);
  LOGI("  number of networks to match: %zu", networks_to_match_.size());
  for (auto &net : networks_to_match_) {
    net.Log();
  }
}

}  // namespace wifi_offload
