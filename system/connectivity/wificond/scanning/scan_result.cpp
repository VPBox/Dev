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

#include "wificond/scanning/scan_result.h"

#include <algorithm>

#include <android-base/logging.h>

#include "wificond/logging_utils.h"
#include "wificond/parcelable_utils.h"

using android::status_t;
using android::OK;
using std::string;

namespace com {
namespace android {
namespace server {
namespace wifi {
namespace wificond {

NativeScanResult::NativeScanResult(std::vector<uint8_t>& ssid_,
                                   std::array<uint8_t, ETH_ALEN>& bssid_,
                                   std::vector<uint8_t>& info_element_,
                                   uint32_t frequency_,
                                   int32_t signal_mbm_,
                                   uint64_t tsf_,
                                   uint16_t capability_,
                                   bool associated_,
                                   std::vector<RadioChainInfo>& radio_chain_infos_)
    : ssid(ssid_),
      bssid(bssid_),
      info_element(info_element_),
      frequency(frequency_),
      signal_mbm(signal_mbm_),
      tsf(tsf_),
      capability(capability_),
      associated(associated_),
      radio_chain_infos(radio_chain_infos_) {
}

status_t NativeScanResult::writeToParcel(::android::Parcel* parcel) const {
  RETURN_IF_FAILED(parcel->writeByteVector(ssid));
  RETURN_IF_FAILED(parcel->writeByteVector(
      std::vector<uint8_t>(bssid.begin(), bssid.end())));
  RETURN_IF_FAILED(parcel->writeByteVector(info_element));
  RETURN_IF_FAILED(parcel->writeUint32(frequency));
  RETURN_IF_FAILED(parcel->writeInt32(signal_mbm));
  RETURN_IF_FAILED(parcel->writeUint64(tsf));
  // There is no writeUint16() available.
  // Use writeUint32() instead.
  RETURN_IF_FAILED(parcel->writeUint32(capability));
  RETURN_IF_FAILED(parcel->writeInt32(associated ? 1 : 0));
  RETURN_IF_FAILED(parcel->writeInt32(radio_chain_infos.size()));
  for (const auto& radio_chain_info : radio_chain_infos) {
    // For Java readTypedList():
    // A leading number 1 means this object is not null.
    RETURN_IF_FAILED(parcel->writeInt32(1));
    RETURN_IF_FAILED(radio_chain_info.writeToParcel(parcel));
  }
  return ::android::OK;
}

status_t NativeScanResult::readFromParcel(const ::android::Parcel* parcel) {
  RETURN_IF_FAILED(parcel->readByteVector(&ssid));
  {
    std::vector<uint8_t> bssid_vec;
    RETURN_IF_FAILED(parcel->readByteVector(&bssid_vec));
    if (bssid_vec.size() != ETH_ALEN) {
      LOG(ERROR) << "bssid length expected " << ETH_ALEN << " bytes, but got "
                 << bssid_vec.size() << " bytes";
      return ::android::BAD_VALUE;
    }
    std::copy_n(bssid_vec.begin(), ETH_ALEN, bssid.begin());
  }
  RETURN_IF_FAILED(parcel->readByteVector(&info_element));
  RETURN_IF_FAILED(parcel->readUint32(&frequency));
  RETURN_IF_FAILED(parcel->readInt32(&signal_mbm));
  RETURN_IF_FAILED(parcel->readUint64(&tsf));
  // There is no readUint16() available.
  // Use readUint32() instead.
  capability = static_cast<uint16_t>(parcel->readUint32());
  associated = (parcel->readInt32() != 0);
  int32_t num_radio_chain_infos = 0;
  RETURN_IF_FAILED(parcel->readInt32(&num_radio_chain_infos));
  for (int i = 0; i < num_radio_chain_infos; i++) {
    RadioChainInfo radio_chain_info;
    // From Java writeTypedList():
    // A leading number 1 means this object is not null.
    // We never expect a 0 or other values here.
    int32_t leading_number = 0;
    RETURN_IF_FAILED(parcel->readInt32(&leading_number));
    if (leading_number != 1) {
      LOG(ERROR) << "Unexpected leading number before an object: "
                 << leading_number;
      return ::android::BAD_VALUE;
    }
    RETURN_IF_FAILED(radio_chain_info.readFromParcel(parcel));
    radio_chain_infos.push_back(radio_chain_info);
  }
  return ::android::OK;
}

void NativeScanResult::DebugLog() {
  LOG(INFO) << "Scan result:";
  // |ssid| might be an encoded array but we just print it as ASCII here.
  string ssid_str(ssid.data(), ssid.data() + ssid.size());
  LOG(INFO) << "SSID: " << ssid_str;

  LOG(INFO) << "BSSID: "
            << ::android::wificond::LoggingUtils::GetMacString(bssid);
  LOG(INFO) << "FREQUENCY: " << frequency;
  LOG(INFO) << "SIGNAL: " << signal_mbm/100 << "dBm";
  LOG(INFO) << "TSF: " << tsf;
  LOG(INFO) << "CAPABILITY: " << capability;
  LOG(INFO) << "ASSOCIATED: " << associated;
  for (const auto& radio_chain_info : radio_chain_infos) {
    LOG(INFO) << "RADIO CHAIN ID: " << radio_chain_info.chain_id;
    LOG(INFO) << "RADIO CHAIN LEVEL: " << radio_chain_info.level;
  }

}

}  // namespace wificond
}  // namespace wifi
}  // namespace server
}  // namespace android
}  // namespace com
