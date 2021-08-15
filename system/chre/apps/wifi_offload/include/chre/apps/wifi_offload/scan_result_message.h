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

#ifndef CHRE_WIFI_OFFLOAD_SCAN_RESULT_MESSAGE_H_
#define CHRE_WIFI_OFFLOAD_SCAN_RESULT_MESSAGE_H_

#include "chre/apps/wifi_offload/wifi_offload.h"

#include "chre/apps/wifi_offload/scan_result.h"

namespace wifi_offload {

/**
 * Container for vector of scan results that provides serialization methods
 */
class ScanResultMessage {
 public:
  /* Corresponding flatbuffers-generated data-type used to serialize and
   * deserialize instances of this class */
  using FbsType = fbs::ScanResultMessage;

  ScanResultMessage() = default;
  ~ScanResultMessage() = default;

  void SetScanResults(const Vector<ScanResult> &results);
  void GetScanResults(Vector<ScanResult> *results);

  flatbuffers::Offset<ScanResultMessage::FbsType> Serialize(
      flatbuffers::FlatBufferBuilder *builder) const;

  bool Deserialize(const ScanResultMessage::FbsType &fbs_result_message);

 private:
  Vector<ScanResult> scan_results_;
};

}  // namespace wifi_offload

#endif  // CHRE_WIFI_OFFLOAD_SCAN_RESULT_MESSAGE_H_
