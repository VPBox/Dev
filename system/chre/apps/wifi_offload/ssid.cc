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

#include "chre/apps/wifi_offload/ssid.h"
#include "chre/apps/wifi_offload/utility.h"

namespace wifi_offload {

Ssid::Ssid(const Ssid &other) {
  SetData(other.ssid_vec_.data(), other.ssid_vec_.size());
}

void Ssid::SetData(const uint8_t *buff, size_t len) {
  if (len > kMaxSsidLen) {
    LOGE("Ssid buffer len %zu larger than max ssid len %zu. Truncating.", len,
         kMaxSsidLen);
    len = kMaxSsidLen;
  }

  ssid_vec_.clear();
  ssid_vec_.reserve(len);
  for (size_t i = 0; i < len; i++) {
    ssid_vec_.push_back(buff[i]);
  }
}

bool Ssid::operator==(const Ssid &other) const {
  return ssid_vec_ == other.ssid_vec_;
}

flatbuffers::Offset<Ssid::FbsType> Ssid::Serialize(
    flatbuffers::FlatBufferBuilder *builder) const {
  return fbs::CreateSsid(*builder, builder->CreateVector(ssid_vec_));
}

bool Ssid::Deserialize(const Ssid::FbsType &fbs_ssid) {
  const auto &ssid_vec = fbs_ssid.ssid();
  if (ssid_vec == nullptr) {
    LOGE("Failed to deserialize Ssid. Null or incomplete members.");
    return false;
  }

  if (ssid_vec->size() > kMaxSsidLen) {
    LOGE("Failed to deserialize Ssid. Ssid size is larger than max len.");
    return false;
  }

  SetData(ssid_vec->data(), ssid_vec->size());
  return true;
}

void Ssid::Log() const {
  utility::LogSsid(ssid_vec_.data(), static_cast<uint8_t>(ssid_vec_.size()));
}

void Ssid::ToChreWifiSsidListItem(chreWifiSsidListItem *chre_ssid) const {
  if (chre_ssid == nullptr) {
    LOGW("Failed to convert to chreWifiSsidListItem. Output pointer is null");
    return;
  }

  std::memcpy(chre_ssid->ssid, ssid_vec_.data(), ssid_vec_.size());
  chre_ssid->ssidLen = static_cast<uint8_t>(ssid_vec_.size());
}

}  // namespace wifi_offload
