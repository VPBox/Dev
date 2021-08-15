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

#ifndef CHRE_WIFI_OFFLOAD_SCAN_RESULT_H_
#define CHRE_WIFI_OFFLOAD_SCAN_RESULT_H_

// First to pickup the LOG_TAG
#include "chre/apps/wifi_offload/wifi_offload.h"

#include "chre/apps/wifi_offload/flatbuffers_types_generated.h"
#include "chre/apps/wifi_offload/preferred_network.h"
#include "chre/apps/wifi_offload/ssid.h"

namespace wifi_offload {

/**
 * Scan Results returned by offload nanoapp to the offload HAL
 */
class ScanResult {
 public:
  /**
   * This is a bit mask describing the capabilities of a BSS.
   * See IEEE Std 802.11: 8.4.1.4
   */
  enum Capability : uint16_t {
    UNKNOWN = 0,
    ESS = 1 << 0,
    IBSS = 1 << 1,
    CF_POLLABLE = 1 << 2,
    CF_POLL_REQ = 1 << 3,
    PRIVACY = 1 << 4,
    SHORT_PREAMBLE = 1 << 5,
    PBCC = 1 << 6,
    CHANNEL_AGILITY = 1 << 7,
    SPECTURM_MGMT = 1 << 8,
    QOS = 1 << 9,
    SHORT_SLOT_TIME = 1 << 10,
    APSD = 1 << 11,
    RADIO_MEASUREMENT = 1 << 12,
    DSSS_OFDM = 1 << 13,
    DELAYED_BLOCK_ACK = 1 << 14,
    IMMEDIATE_BLOCK_ACK = 1 << 15,
    ALL_CAPABILITIES_MASK = 0xffff,
  };

  /* Corresponding flatbuffers-generated data-type used to serialize and
   * deserialize instances of this class */
  using FbsType = fbs::ScanResult;

  static constexpr int kBssidSize = CHRE_WIFI_BSSID_LEN;

  ScanResult();

  ScanResult(const ScanResult &other);

  ScanResult(ScanResult &&other) = default;

  explicit ScanResult(const chreWifiScanResult &chre_scan_result);

  ~ScanResult() = default;

  bool operator==(const ScanResult &other) const;

  flatbuffers::Offset<ScanResult::FbsType> Serialize(
      flatbuffers::FlatBufferBuilder *builder) const;

  bool Deserialize(const ScanResult::FbsType &fbs_result);

  void Log() const;

  Ssid ssid_;
  uint8_t security_modes_;  // SecurityMode flags, see SecurityMode
  uint8_t bssid_[kBssidSize];
  uint16_t capability_;  // Can have multiple bits set, see Capability
  uint32_t frequency_scanned_mhz_;
  int8_t rssi_dbm_;  // Signal strength
  uint64_t tsf_;     // TSF found in beacon/probe response

 private:
  void UpdateFromChreWifiScanResult(const chreWifiScanResult &chre_scan_result);
};

}  // namespace wifi_offload

#endif  // CHRE_WIFI_OFFLOAD_SCAN_RESULT_H_
