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

#ifndef CHRE_WIFI_OFFLOAD_SCAN_PARAMS_H_
#define CHRE_WIFI_OFFLOAD_SCAN_PARAMS_H_

#include "chre/apps/wifi_offload/wifi_offload.h"

#include "chre/apps/wifi_offload/flatbuffers_types_generated.h"
#include "chre/apps/wifi_offload/ssid.h"

namespace wifi_offload {

/**
 * Parameters for performing offload scans
 */
class ScanParams {
 public:
  /* Corresponding flatbuffers-generated data-type used to serialize and
   * deserialize instances of this class */
  using FbsType = fbs::ScanParams;

  ScanParams();

  ~ScanParams() = default;

  bool operator==(const ScanParams &other) const;

  flatbuffers::Offset<ScanParams::FbsType> Serialize(
      flatbuffers::FlatBufferBuilder *builder) const;

  bool Deserialize(const ScanParams::FbsType &fbs_params);

  void Log() const;

  Vector<Ssid> ssids_to_scan_;
  Vector<uint32_t> frequencies_to_scan_mhz_;
  uint32_t disconnected_mode_scan_interval_ms_;  // 0 means disable
};

}  // namespace wifi_offload

#endif  // CHRE_WIFI_OFFLOAD_SCAN_PARAMS_H_
