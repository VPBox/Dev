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

#include "chre/apps/wifi_offload/scan_record.h"

namespace wifi_offload {

ScanRecord::ScanRecord()
    : time_spent_scanning_ms_(0),
      num_channels_scanned_(0),
      num_entries_aggregated_(0) {}

bool ScanRecord::operator==(const ScanRecord &other) const {
  if (this == &other) {
    return true;
  }
  return time_spent_scanning_ms_ == other.time_spent_scanning_ms_ &&
         num_channels_scanned_ == other.num_channels_scanned_ &&
         num_entries_aggregated_ == other.num_entries_aggregated_;
}

flatbuffers::Offset<ScanRecord::FbsType> ScanRecord::Serialize(
    flatbuffers::FlatBufferBuilder *builder) const {
  return fbs::CreateScanRecord(*builder, time_spent_scanning_ms_,
                               num_channels_scanned_, num_entries_aggregated_);
}

bool ScanRecord::Deserialize(const ScanRecord::FbsType &fbs_record) {
  time_spent_scanning_ms_ = fbs_record.time_spent_scanning_ms();
  num_channels_scanned_ = fbs_record.num_channels_scanned();
  num_entries_aggregated_ = fbs_record.num_entries_aggregated();
  return true;
}

}  // namespace wifi_offload
