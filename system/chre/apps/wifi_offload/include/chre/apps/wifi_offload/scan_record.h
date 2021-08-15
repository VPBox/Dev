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

#ifndef CHRE_WIFI_OFFLOAD_SCAN_RECORD_H_
#define CHRE_WIFI_OFFLOAD_SCAN_RECORD_H_

#include "chre/apps/wifi_offload/wifi_offload.h"

#include "chre/apps/wifi_offload/flatbuffers_types_generated.h"

namespace wifi_offload {

class ScanRecord {
 public:
  /* Corresponding flatbuffers-generated data-type used to serialize and
   * deserialize instances of this class */
  using FbsType = fbs::ScanRecord;

  ScanRecord();
  ~ScanRecord() = default;

  bool operator==(const ScanRecord &other) const;

  flatbuffers::Offset<ScanRecord::FbsType> Serialize(
      flatbuffers::FlatBufferBuilder *builder) const;

  bool Deserialize(const ScanRecord::FbsType &fbs_record);

  uint32_t time_spent_scanning_ms_;
  uint32_t num_channels_scanned_;
  /* Number of ScanRecords aggregated into this record. Multiple ScanRecords
   * may be combined together into a single ScanRecord by adding up values to
   * save space if needed */
  uint32_t num_entries_aggregated_;
};

}  // namespace wifi_offload

#endif  // CHRE_WIFI_OFFLOAD_SCAN_RECORD_H_
