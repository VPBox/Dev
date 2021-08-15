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

#ifndef CHRE_WIFI_OFFLOAD_SCAN_CONFIG_H_
#define CHRE_WIFI_OFFLOAD_SCAN_CONFIG_H_

#include "chre/apps/wifi_offload/wifi_offload.h"

#include "chre/apps/wifi_offload/flatbuffers_types_generated.h"
#include "chre/apps/wifi_offload/scan_filter.h"
#include "chre/apps/wifi_offload/scan_params.h"

namespace wifi_offload {

class ScanConfig {
 public:
  /* Corresponding flatbuffers-generated data-type used to serialize and
   * deserialize instances of this class */
  using FbsType = fbs::ScanConfig;

  ScanConfig() = default;
  ~ScanConfig() = default;

  bool operator==(const ScanConfig &other) const;

  flatbuffers::Offset<ScanConfig::FbsType> Serialize(
      flatbuffers::FlatBufferBuilder *builder) const;

  bool Deserialize(const ScanConfig::FbsType &fbs_config);

  void Log() const;

  ScanParams scan_params_;
  ScanFilter scan_filter_;
};

}  // namespace wifi_offload

#endif  // CHRE_WIFI_OFFLOAD_SCAN_CONFIG_H_
