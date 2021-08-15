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

#include "chre/apps/wifi_offload/scan_config.h"

namespace wifi_offload {

bool ScanConfig::operator==(const ScanConfig &other) const {
  if (this == &other) {
    return true;
  }
  return scan_params_ == other.scan_params_ &&
         scan_filter_ == other.scan_filter_;
}

flatbuffers::Offset<ScanConfig::FbsType> ScanConfig::Serialize(
    flatbuffers::FlatBufferBuilder *builder) const {
  auto params_offset = scan_params_.Serialize(builder);
  auto filter_offset = scan_filter_.Serialize(builder);
  return fbs::CreateScanConfig(*builder, params_offset, filter_offset);
}

bool ScanConfig::Deserialize(const ScanConfig::FbsType &fbs_config) {
  if (fbs_config.scan_params() == nullptr ||
      fbs_config.scan_filter() == nullptr) {
    LOGE("Failed to deserialize ScanConfig. Null or incomplete members.");
    return false;
  }

  return scan_params_.Deserialize(*fbs_config.scan_params()) &&
         scan_filter_.Deserialize(*fbs_config.scan_filter());
}

void ScanConfig::Log() const {
  scan_params_.Log();
  scan_filter_.Log();
}

}  // namespace wifi_offload
