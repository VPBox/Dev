/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef WIFICOND_SCANNING_SCAN_RESULT_H_
#define WIFICOND_SCANNING_SCAN_RESULT_H_

#include <array>
#include <vector>

#include <linux/if_ether.h>

#include <binder/Parcel.h>
#include <binder/Parcelable.h>

#include "wificond/scanning/radio_chain_info.h"

namespace com {
namespace android {
namespace server {
namespace wifi {
namespace wificond {

// This is the class to represent a scan result for wificond internal use.
class NativeScanResult : public ::android::Parcelable {
 public:
  NativeScanResult() = default;
  NativeScanResult(std::vector<uint8_t>& ssid,
                   std::array<uint8_t, ETH_ALEN>& bssid,
                   std::vector<uint8_t>& info_element,
                   uint32_t frequency,
                   int32_t signal_mbm,
                   uint64_t tsf,
                   uint16_t capability,
                   bool associated,
                   std::vector<RadioChainInfo>& radio_chain_infos);
  ::android::status_t writeToParcel(::android::Parcel* parcel) const override;
  ::android::status_t readFromParcel(const ::android::Parcel* parcel) override;

  void DebugLog();

  // SSID of the BSS.
  std::vector<uint8_t> ssid;
  // BSSID of the BSS.
  std::array<uint8_t, ETH_ALEN> bssid;
  // Binary array containing the raw information elements from the probe
  // response/beacon.
  std::vector<uint8_t> info_element;
  // Frequency in MHz.
  uint32_t frequency;
  // Signal strength of probe response/beacon in (100 * dBm).
  int32_t signal_mbm;
  // TSF of the received probe response/beacon.
  uint64_t tsf;
  // This is a bit mask describing the capabilities of a BSS.
  // See IEEE Std 802.11: 8.4.1.4
  // Bit 0 - ESS
  // Bit 1 - IBSS
  // Bit 2 - CF Pollable
  // Bit 3 - CF-Poll Request
  // Bit 4 - Privacy
  // Bit 5 - Short Preamble
  // Bit 6 - PBCC
  // Bit 7 - Channel Agility
  // Bit 8 - Spectrum Mgmt
  // Bit 9 - QoS
  // Bit 10 - Short Slot Time
  // Bit 11 - APSD
  // Bit 12 - Radio Measurement
  // Bit 13 - DSSS-OFDM
  // Bit 14 - Delayed Block Ack
  // Bit 15 - Immediate Block Ack
  uint16_t capability;
  bool associated;
  std::vector<RadioChainInfo> radio_chain_infos;
};

}  // namespace wificond
}  // namespace wifi
}  // namespace server
}  // namespace android
}  // namespace com

#endif  // WIFICOND_SCANNING_SCAN_RESULT_H_
