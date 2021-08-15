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

#include "chre/apps/wifi_offload/flatbuffers_serialization.h"
#include "chre/apps/wifi_offload/wifi_offload.h"

namespace wifi_offload {
namespace fbs {

size_t Serialize(const wifi_offload::ScanStats &stats, uint8_t *buffer,
                 size_t buffer_len) {
  return Serialize(stats, buffer, buffer_len, "ScanStats");
}

bool Deserialize(const uint8_t *buffer, size_t buffer_len,
                 wifi_offload::ScanStats *stats) {
  return Deserialize<wifi_offload::ScanStats>(buffer, buffer_len, stats,
                                              "ScanStats");
}

size_t Serialize(const wifi_offload::ScanConfig &config, uint8_t *buffer,
                 size_t buffer_len) {
  return Serialize(config, buffer, buffer_len, "ScanConfig");
}

bool Deserialize(const uint8_t *buffer, size_t buffer_len,
                 wifi_offload::ScanConfig *config) {
  return Deserialize<wifi_offload::ScanConfig>(buffer, buffer_len, config,
                                               "ScanConfig");
}

size_t Serialize(const wifi_offload::Vector<wifi_offload::ScanResult> &results,
                 uint8_t *buffer, size_t buffer_len) {
  wifi_offload::ScanResultMessage msg;
  msg.SetScanResults(results);
  return Serialize(msg, buffer, buffer_len, "ScanResults");
}

bool Deserialize(const uint8_t *buffer, size_t buffer_len,
                 wifi_offload::Vector<wifi_offload::ScanResult> *results) {
  wifi_offload::ScanResultMessage msg;
  if (Deserialize<wifi_offload::ScanResultMessage>(buffer, buffer_len, &msg,
                                                   "ScanResults")) {
    msg.GetScanResults(results);
    return true;
  } else {
    return false;
  }
}

}  // namespace fbs
}  // namespace wifi_offload
