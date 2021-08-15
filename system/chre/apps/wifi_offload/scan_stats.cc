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

#include "chre/apps/wifi_offload/scan_stats.h"
#include "chre/apps/wifi_offload/vector_serialization.h"

namespace wifi_offload {

ScanStats::ScanStats()
    : num_scans_requested_by_nanoapp_(0),
      num_scans_serviced_by_hardware_(0),
      num_scans_serviced_by_cache_(0),
      updated_at_chre_ms_(0),
      sent_at_chre_ms_(0),
      last_subscription_duration_ms_(0) {}

bool ScanStats::operator==(const ScanStats &other) const {
  if (this == &other) {
    return true;
  }
  return num_scans_requested_by_nanoapp_ ==
             other.num_scans_requested_by_nanoapp_ &&
         num_scans_serviced_by_hardware_ ==
             other.num_scans_serviced_by_hardware_ &&
         num_scans_serviced_by_cache_ == other.num_scans_serviced_by_cache_ &&
         updated_at_chre_ms_ == other.updated_at_chre_ms_ &&
         sent_at_chre_ms_ == other.sent_at_chre_ms_ &&
         last_subscription_duration_ms_ ==
             other.last_subscription_duration_ms_ &&
         channel_histogram_ == other.channel_histogram_ &&
         scan_records_ == other.scan_records_ &&
         rpc_log_records_ == other.rpc_log_records_;
}

flatbuffers::Offset<ScanStats::FbsType> ScanStats::Serialize(
    flatbuffers::FlatBufferBuilder *builder) const {
  auto histo = channel_histogram_.Serialize(builder);
  auto scan_recs = SerializeVector(scan_records_, builder);
  auto log_recs = SerializeVector(rpc_log_records_, builder);
  return fbs::CreateScanStats(*builder, num_scans_requested_by_nanoapp_,
                              num_scans_serviced_by_hardware_,
                              num_scans_serviced_by_cache_, updated_at_chre_ms_,
                              sent_at_chre_ms_, last_subscription_duration_ms_,
                              histo, scan_recs, log_recs);
}

bool ScanStats::Deserialize(const ScanStats::FbsType &fbs_stats) {
  const auto &histo = fbs_stats.channel_scan_count();
  if (histo == nullptr || !channel_histogram_.Deserialize(*histo)) {
    LOGE("Failed to deserialize ScanStats. Null or incomplete members.");
    return false;
  }

  const auto &scan_recs = fbs_stats.scan_records();
  if (scan_recs == nullptr ||
      !DeserializeVector<ScanRecord>(*scan_recs, &scan_records_)) {
    LOGE("Failed to deserialize ScanStats. Null or incomplete members.");
    return false;
  }

  const auto &log_recs = fbs_stats.rpc_log_records();
  if (log_recs == nullptr ||
      !DeserializeVector<RpcLogRecord>(*log_recs, &rpc_log_records_)) {
    LOGE("Failed to deserialize ScanStats. Null or incomplete members.");
    return false;
  }

  num_scans_requested_by_nanoapp_ = fbs_stats.num_scans_requested_by_nanoapp();
  num_scans_serviced_by_hardware_ = fbs_stats.num_scans_serviced_by_hardware();
  num_scans_serviced_by_cache_ = fbs_stats.num_scans_serviced_by_cache();
  updated_at_chre_ms_ = fbs_stats.updated_at_chre_ms();
  sent_at_chre_ms_ = fbs_stats.sent_at_chre_ms();
  last_subscription_duration_ms_ = fbs_stats.last_subscription_duration_ms();
  return true;
}

}  // namespace wifi_offload
