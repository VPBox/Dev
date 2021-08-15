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

#include "chre/apps/wifi_offload/scan_result_message.h"
#include "chre/apps/wifi_offload/vector_serialization.h"

namespace wifi_offload {

void ScanResultMessage::SetScanResults(const Vector<ScanResult> &results) {
  scan_results_.clear();
  scan_results_.reserve(results.size());
  for (const auto &result : results) {
    scan_results_.emplace_back(result);
  }
}

void ScanResultMessage::GetScanResults(Vector<ScanResult> *results) {
  if (results == nullptr) {
    LOGE("ScanResultsMessage output pointer is null in GetScanResults.");
    return;
  }

  results->clear();
  results->reserve(scan_results_.size());
  for (const auto &result : scan_results_) {
    results->emplace_back(result);
  }
}

flatbuffers::Offset<ScanResultMessage::FbsType> ScanResultMessage::Serialize(
    flatbuffers::FlatBufferBuilder *builder) const {
  auto results = SerializeVector(scan_results_, builder);
  return fbs::CreateScanResultMessage(*builder, results);
}

bool ScanResultMessage::Deserialize(
    const ScanResultMessage::FbsType &fbs_result_message) {
  const auto &fbs_results = fbs_result_message.scan_results();
  if (fbs_results == nullptr || fbs_results->size() == 0) {
    LOGE(
        "Failed to deserialize ScanResultsMessage. Null or incomplete "
        "members.");
    return false;
  }

  if (!DeserializeVector<ScanResult>(*fbs_results, &scan_results_)) {
    LOGE(
        "Failed to deserialize ScanResultMessage. Null or incomplete members.");
    return false;
  }

  return true;
}

}  // namespace wifi_offload
