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

#include <algorithm>

#include "chre/apps/wifi_offload/chre_scan_params_safe.h"
#include "chre/apps/wifi_offload/utility.h"

namespace wifi_offload {

ChreScanParamsSafe::ChreScanParamsSafe(const ScanParams &scan_params) {
  // point chre_scan_params_ pointers to allocated buffers
  chre_scan_params_.frequencyList = chre_scan_params_frequencies_;
  chre_scan_params_.ssidList = chre_scan_params_ssids_;

  UpdateFromScanParams(scan_params);
}

const chreWifiScanParams *ChreScanParamsSafe::GetChreWifiScanParams() {
  return &chre_scan_params_;
}

void ChreScanParamsSafe::Log() const {
  LOGI("chreWifiScanParams:");
  LOGI("  scan type: %" PRIu8, chre_scan_params_.scanType);
  LOGI("  max scan age (ms): %" PRIu32, chre_scan_params_.maxScanAgeMs);
  LOGI("  frequency list length: %" PRIu16, chre_scan_params_.frequencyListLen);
  for (size_t i = 0; i < chre_scan_params_.frequencyListLen; i++) {
    LOGI("  frequency: %" PRIu32, chre_scan_params_.frequencyList[i]);
  }
  LOGI("  ssid list length: %" PRIu8, chre_scan_params_.ssidListLen);
  for (size_t i = 0; i < chre_scan_params_.ssidListLen; i++) {
    utility::LogSsid(chre_scan_params_.ssidList[i].ssid,
                     chre_scan_params_.ssidList[i].ssidLen);
  }
}

void ChreScanParamsSafe::UpdateFromScanParams(const ScanParams &scan_params) {
  chre_scan_params_.scanType = CHRE_WIFI_SCAN_TYPE_ACTIVE_PLUS_PASSIVE_DFS;
  chre_scan_params_.maxScanAgeMs = 0;

  chre_scan_params_.frequencyListLen = std::min(
      static_cast<uint16_t>(scan_params.frequencies_to_scan_mhz_.size()),
      static_cast<uint16_t>(CHRE_WIFI_FREQUENCY_LIST_MAX_LEN));
  if (chre_scan_params_.frequencyListLen <
      scan_params.frequencies_to_scan_mhz_.size()) {
    LOGW("Too many ScanParams frequencies %zu truncated to max %" PRIu16,
         scan_params.frequencies_to_scan_mhz_.size(),
         chre_scan_params_.frequencyListLen);
  }

  std::copy(scan_params.frequencies_to_scan_mhz_.begin(),
            scan_params.frequencies_to_scan_mhz_.begin() +
                chre_scan_params_.frequencyListLen,
            chre_scan_params_frequencies_);

  chre_scan_params_.ssidListLen =
      std::min(static_cast<uint8_t>(scan_params.ssids_to_scan_.size()),
               static_cast<uint8_t>(CHRE_WIFI_SSID_LIST_MAX_LEN));
  if (chre_scan_params_.ssidListLen < scan_params.ssids_to_scan_.size()) {
    LOGW("Too many ScanParams ssids %zu truncated to max %" PRIu8,
         scan_params.ssids_to_scan_.size(), chre_scan_params_.ssidListLen);
  }

  for (size_t i = 0; i < chre_scan_params_.ssidListLen; i++) {
    scan_params.ssids_to_scan_[i].ToChreWifiSsidListItem(
        &chre_scan_params_ssids_[i]);
  }
}

}  // namespace wifi_offload
