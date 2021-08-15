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

#ifndef CHRE_WIFI_OFFLOAD_SSID_H_
#define CHRE_WIFI_OFFLOAD_SSID_H_

#include "chre/apps/wifi_offload/wifi_offload.h"

#include "chre/apps/wifi_offload/flatbuffers_types_generated.h"

namespace wifi_offload {

/* SSID of the Access Point, maximum 32 characters */
class Ssid {
 public:
  /* Corresponding flatbuffers-generated data-type used to serialize and
   * deserialize instances of this class */
  using FbsType = fbs::Ssid;

  static constexpr size_t kMaxSsidLen = CHRE_WIFI_SSID_MAX_LEN;  // = 32

  Ssid() = default;

  Ssid(const Ssid &other);

  Ssid(Ssid &&other) = default;

  ~Ssid() = default;

  void SetData(const uint8_t *buff, size_t len);

  bool operator==(const Ssid &other) const;

  flatbuffers::Offset<Ssid::FbsType> Serialize(
      flatbuffers::FlatBufferBuilder *builder) const;

  bool Deserialize(const Ssid::FbsType &fbs_ssid);

  void Log() const;

  void ToChreWifiSsidListItem(chreWifiSsidListItem *chre_ssid) const;

 private:
  Vector<uint8_t> ssid_vec_;
};

}  // namespace wifi_offload

#endif  // CHRE_WIFI_OFFLOAD_SSID_H_
