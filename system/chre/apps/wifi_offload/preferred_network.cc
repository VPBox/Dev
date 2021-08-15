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

#include "chre/apps/wifi_offload/preferred_network.h"

namespace wifi_offload {

PreferredNetwork::PreferredNetwork() : security_modes_(SecurityMode::UNKNOWN) {}

bool PreferredNetwork::operator==(const PreferredNetwork &other) const {
  if (this == &other) {
    return true;
  }
  return ssid_ == other.ssid_ && security_modes_ == other.security_modes_;
}

flatbuffers::Offset<PreferredNetwork::FbsType> PreferredNetwork::Serialize(
    flatbuffers::FlatBufferBuilder *builder) const {
  return fbs::CreatePreferredNetwork(*builder, ssid_.Serialize(builder),
                                     security_modes_);
}

bool PreferredNetwork::Deserialize(
    const PreferredNetwork::FbsType &fbs_network) {
  const auto &fbs_ssid = fbs_network.ssid();
  if (fbs_ssid == nullptr || !ssid_.Deserialize(*fbs_ssid)) {
    LOGE("Failed to deserialize PreferredNetwork. Null or incomplete members.");
    return false;
  }

  security_modes_ = fbs_network.security_modes();
  if (security_modes_ & ~SecurityMode::ALL_SECURITY_MODES_MASK) {
    LOGE("Failed to deserialize PreferredNetwork. Invalid security mode.");
    return false;
  }

  return true;
}

void PreferredNetwork::Log() const {
  ssid_.Log();
  LOGI("  security modes: 0x%" PRIx8, security_modes_);
}

}  // namespace wifi_offload
