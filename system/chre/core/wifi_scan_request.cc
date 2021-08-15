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

#include "chre/core/wifi_scan_request.h"

namespace chre {

WifiScanType getWifiScanTypeForEnum(enum chreWifiScanType enumWifiScanType) {
  switch (enumWifiScanType) {
    case CHRE_WIFI_SCAN_TYPE_ACTIVE:
      return WifiScanType::Active;
    case CHRE_WIFI_SCAN_TYPE_ACTIVE_PLUS_PASSIVE_DFS:
      return WifiScanType::ActivePlusPassiveDfs;
    case CHRE_WIFI_SCAN_TYPE_PASSIVE:
      return WifiScanType::Passive;
    default:
      return WifiScanType::Invalid;
  }
}

WifiScanRequest::WifiScanRequest()
    : WifiScanRequest(WifiScanType::Invalid,
                      Nanoseconds(UINT64_MAX) /* maxScanAge */,
                      DynamicVector<uint32_t>() /* frequencies */,
                      DynamicVector<WifiSsid>() /* ssids */) {}

WifiScanRequest::WifiScanRequest(WifiScanType scanType,
                                 const Nanoseconds& maxScanAge,
                                 DynamicVector<uint32_t>&& frequencies,
                                 DynamicVector<WifiSsid>&& ssids)
    : mScanType(scanType),
      mMaxScanAge(maxScanAge),
      mFrequencies(std::move(frequencies)),
      mSsids(std::move(ssids)) {}

WifiScanType WifiScanRequest::getScanType() const {
  return mScanType;
}

const Nanoseconds& WifiScanRequest::getMaxScanAge() const {
  return mMaxScanAge;
}

const DynamicVector<uint32_t>& WifiScanRequest::getFrequencies() const {
  return mFrequencies;
}

const DynamicVector<WifiSsid>& WifiScanRequest::getSsids() const {
  return mSsids;
}

}  // namespace chre
