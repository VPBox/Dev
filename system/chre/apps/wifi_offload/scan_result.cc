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

#include "chre/apps/wifi_offload/scan_result.h"
#include "chre/apps/wifi_offload/channel_histogram.h"
#include "chre/apps/wifi_offload/utility.h"

namespace wifi_offload {
namespace {

SecurityMode ConvertSecurityModeChreToOffload(int chre_security_mode) {
  switch (chre_security_mode) {
    case CHRE_WIFI_SECURITY_MODE_OPEN:
      return SecurityMode::OPEN;
    case CHRE_WIFI_SECURITY_MODE_WEP:
      return SecurityMode::WEP;
    case CHRE_WIFI_SECURITY_MODE_PSK:
      return SecurityMode::PSK;
    case CHRE_WIFI_SECURITY_MODE_EAP:
      return SecurityMode::EAP;
    default:
      return SecurityMode::UNKNOWN;
  }
}

}  // namespace

ScanResult::ScanResult()
    : security_modes_(SecurityMode::UNKNOWN),
      capability_(Capability::UNKNOWN),
      frequency_scanned_mhz_(0),
      rssi_dbm_(-128),
      tsf_(0) {
  std::memset(bssid_, 0, sizeof(bssid_));
}

ScanResult::ScanResult(const ScanResult &other)
    : ssid_(other.ssid_),
      security_modes_(other.security_modes_),
      capability_(other.capability_),
      frequency_scanned_mhz_(other.frequency_scanned_mhz_),
      rssi_dbm_(other.rssi_dbm_),
      tsf_(other.tsf_) {
  std::memcpy(bssid_, other.bssid_, sizeof(bssid_));
}

ScanResult::ScanResult(const chreWifiScanResult &chre_scan_result) {
  UpdateFromChreWifiScanResult(chre_scan_result);
}

bool ScanResult::operator==(const ScanResult &other) const {
  if (this == &other) {
    return true;
  }
  return std::memcmp(bssid_, other.bssid_, sizeof(bssid_)) == 0 &&
         ssid_ == other.ssid_ && security_modes_ == other.security_modes_ &&
         capability_ == other.capability_ &&
         frequency_scanned_mhz_ == other.frequency_scanned_mhz_ &&
         rssi_dbm_ == other.rssi_dbm_ && tsf_ == other.tsf_;
}

flatbuffers::Offset<ScanResult::FbsType> ScanResult::Serialize(
    flatbuffers::FlatBufferBuilder *builder) const {
  auto ssid_offset = ssid_.Serialize(builder);
  auto bssid_offset = builder->CreateVector(bssid_, kBssidSize);
  return fbs::CreateScanResult(*builder, ssid_offset, security_modes_,
                               bssid_offset, capability_,
                               frequency_scanned_mhz_, rssi_dbm_, tsf_);
}

bool ScanResult::Deserialize(const ScanResult::FbsType &fbs_result) {
  if (fbs_result.ssid() == nullptr || !ssid_.Deserialize(*fbs_result.ssid())) {
    LOGE("Failed to deserialize ScanResult. Null or incomplete members.");
    return false;
  }

  security_modes_ = fbs_result.security_modes();
  if (security_modes_ & ~SecurityMode::ALL_SECURITY_MODES_MASK) {
    LOGE("Failed to deserialize ScanResult. Invalid security mode.");
    return false;
  }

  if (fbs_result.bssid() == nullptr ||
      fbs_result.bssid()->size() != kBssidSize) {
    LOGE("Failed to deserialize ScanResult. Null or incomplete members.");
    return false;
  }
  for (uint8_t i = 0; i < kBssidSize; i++) {
    bssid_[i] = fbs_result.bssid()->Get(i);
  }

  capability_ = fbs_result.capability();
  if ((capability_ == Capability::UNKNOWN) ||
      (capability_ & ~Capability::ALL_CAPABILITIES_MASK)) {
    LOGE("Failed to deserialize ScanResult. Invalid network capability.");
    return false;
  }

  frequency_scanned_mhz_ = fbs_result.frequency_scanned_mhz();
  if (!ChannelHistogram::IsSupportedFrequency(frequency_scanned_mhz_)) {
    LOGE("Failed to deserialize ScanResult. Invalid channel frequency.");
    return false;
  }

  rssi_dbm_ = fbs_result.rssi_dbm();
  if (rssi_dbm_ > 0) {
    LOGE("Failed to deserialize ScanResult. Positive rssi value.");
    return false;
  }

  tsf_ = fbs_result.tsf();
  return true;
}

void ScanResult::Log() const {
  LOGI("ScanResult:");
  ssid_.Log();
  LOGI("  security modes: 0x%" PRIx8, security_modes_);
  utility::LogBssid(bssid_);
  LOGI("  capability: 0x%" PRIx16, capability_);
  LOGI("  scanned frequency: %" PRIu32, frequency_scanned_mhz_);
  LOGI("  rssi: %" PRId8 "dBm", rssi_dbm_);
  LOGI("  tsf: %" PRIu64, tsf_);
}

void ScanResult::UpdateFromChreWifiScanResult(
    const chreWifiScanResult &chre_scan_result) {
  ssid_.SetData(chre_scan_result.ssid, chre_scan_result.ssidLen);

  security_modes_ = 0;
  for (const auto chre_security_mode :
       {CHRE_WIFI_SECURITY_MODE_OPEN, CHRE_WIFI_SECURITY_MODE_WEP,
        CHRE_WIFI_SECURITY_MODE_PSK, CHRE_WIFI_SECURITY_MODE_EAP}) {
    if (chre_scan_result.securityMode & chre_security_mode) {
      security_modes_ |= ConvertSecurityModeChreToOffload(chre_security_mode);
    }
  }

  std::memcpy(bssid_, chre_scan_result.bssid, CHRE_WIFI_BSSID_LEN);
  // TODO: make sure capability definition between two versions is the same
  // (802.11:7.3.1.4 vs. 802.11:8.4.1.4)
  capability_ = chre_scan_result.capabilityInfo;
  if (chre_scan_result.channelWidth == CHRE_WIFI_CHANNEL_WIDTH_20_MHZ) {
    frequency_scanned_mhz_ = chre_scan_result.primaryChannel;
  } else {
    // TODO: (b/62870147) Support other possible channel widths
    LOGW("Scan result channel width not supported %" PRIu8,
         chre_scan_result.channelWidth);
  }

  rssi_dbm_ = chre_scan_result.rssi;
  tsf_ = 0;  // tsf value not available
}

}  // namespace wifi_offload
