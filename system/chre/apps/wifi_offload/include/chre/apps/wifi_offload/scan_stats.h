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

#ifndef CHRE_WIFI_OFFLOAD_SCAN_STATS_H_
#define CHRE_WIFI_OFFLOAD_SCAN_STATS_H_

#include "chre/apps/wifi_offload/wifi_offload.h"

#include "chre/apps/wifi_offload/channel_histogram.h"
#include "chre/apps/wifi_offload/flatbuffers_types_generated.h"
#include "chre/apps/wifi_offload/rpc_log_record.h"
#include "chre/apps/wifi_offload/scan_record.h"

namespace wifi_offload {

/**
 * Defines the scan statistics to be returned to the framework
 */
class ScanStats {
 public:
  /* Corresponding flatbuffers-generated data-type used to serialize and
   * deserialize instances of this class */
  using FbsType = fbs::ScanStats;

  ScanStats();
  ~ScanStats() = default;

  bool operator==(const ScanStats &other) const;

  flatbuffers::Offset<ScanStats::FbsType> Serialize(
      flatbuffers::FlatBufferBuilder *builder) const;

  bool Deserialize(const ScanStats::FbsType &fbs_stats);

  uint32_t num_scans_requested_by_nanoapp_;
  uint32_t num_scans_serviced_by_hardware_;
  uint32_t num_scans_serviced_by_cache_;
  uint32_t updated_at_chre_ms_;
  uint32_t sent_at_chre_ms_;
  /* The duration between when the framework subscribed for scan results to min
   * of time when the framework unsubscribed vs. current time */
  uint32_t last_subscription_duration_ms_;

  ChannelHistogram channel_histogram_;

  Vector<ScanRecord> scan_records_;
  Vector<RpcLogRecord> rpc_log_records_;
};

}  // namespace wifi_offload

#endif  // CHRE_WIFI_OFFLOAD_SCAN_STATS_H_
