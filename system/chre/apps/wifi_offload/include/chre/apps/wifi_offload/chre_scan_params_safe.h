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

#ifndef CHRE_WIFI_OFFLOAD_CHRE_SCAN_PARAMS_SAFE_H_
#define CHRE_WIFI_OFFLOAD_CHRE_SCAN_PARAMS_SAFE_H_

#include "chre/apps/wifi_offload/scan_params.h"
#include "chre/apps/wifi_offload/wifi_offload.h"

namespace wifi_offload {

/* A class to encapsulate chreWifiScanParams struct with pointers to its const
 * fields and some useful conversion methods */
class ChreScanParamsSafe {
 public:
  ChreScanParamsSafe() = delete;

  ChreScanParamsSafe(const ScanParams &scan_params);

  ~ChreScanParamsSafe() = default;

  const chreWifiScanParams *GetChreWifiScanParams();

  void Log() const;

 private:
  void UpdateFromScanParams(const ScanParams &scan_params);

  chreWifiScanParams chre_scan_params_;
  // Storage for *(chre_scan_params_.frequencyList)
  uint32_t chre_scan_params_frequencies_[CHRE_WIFI_FREQUENCY_LIST_MAX_LEN];
  // Storage for *(chre_scan_params_.ssidList)
  chreWifiSsidListItem chre_scan_params_ssids_[CHRE_WIFI_SSID_LIST_MAX_LEN];
};

}  // namespace wifi_offload

#endif  // CHRE_WIFI_OFFLOAD_CHRE_SCAN_PARAMS_SAFE_H_
