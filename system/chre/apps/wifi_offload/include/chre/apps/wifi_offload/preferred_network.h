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

#ifndef CHRE_WIFI_OFFLOAD_PREFERRED_NETWORK_H_
#define CHRE_WIFI_OFFLOAD_PREFERRED_NETWORK_H_

#include "chre/apps/wifi_offload/wifi_offload.h"

#include "chre/apps/wifi_offload/flatbuffers_types_generated.h"
#include "chre/apps/wifi_offload/ssid.h"

namespace wifi_offload {

enum SecurityMode : uint8_t {
  UNKNOWN = 0,
  OPEN = 0x1 << 0,
  WEP = 0x1 << 1,
  PSK = 0x1 << 2,
  EAP = 0x1 << 3,
  ALL_SECURITY_MODES_MASK = OPEN | WEP | PSK | EAP,
};

class PreferredNetwork {
 public:
  /* Corresponding flatbuffers-generated data-type used to serialize and
   * deserialize instances of this class */
  using FbsType = fbs::PreferredNetwork;

  PreferredNetwork();

  PreferredNetwork(PreferredNetwork &&other) = default;

  ~PreferredNetwork() = default;

  bool operator==(const PreferredNetwork &other) const;

  flatbuffers::Offset<FbsType> Serialize(
      flatbuffers::FlatBufferBuilder *builder) const;

  bool Deserialize(const FbsType &fbs_network);

  void Log() const;

  Ssid ssid_;
  /* SecurityMode flags that are associated with this SSID
   * More than one security mode can be supported, see SecurityMode */
  uint8_t security_modes_;
};

}  // namespace wifi_offload

#endif  // CHRE_WIFI_OFFLOAD_PREFERRED_NETWORK_H_
