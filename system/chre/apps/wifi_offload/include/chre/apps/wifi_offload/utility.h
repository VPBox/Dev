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

#ifndef CHRE_WIFI_OFFLOAD_UTILITY_H_
#define CHRE_WIFI_OFFLOAD_UTILITY_H_

#include "chre/apps/wifi_offload/wifi_offload.h"

#include "chre/apps/wifi_offload/error_codes.h"
#include "chre/apps/wifi_offload/scan_config.h"
#include "chre/apps/wifi_offload/scan_filter.h"
#include "chre/apps/wifi_offload/scan_params.h"
#include "chre/apps/wifi_offload/scan_result.h"
#include "chre/apps/wifi_offload/ssid.h"

namespace wifi_offload {
namespace utility {

/**
 * Maps channel frequency to channel number based in IEEE 802.11.
 *
 * @param freq Frequency in MHz we want to map to a channel number
 *
 * @return Channel number for a given frequency, 0 for unknown frequencies
 */
int Ieee80211FrequencyToChannel(int freq);

void LogSsid(const uint8_t *ssid, uint8_t ssid_len);

void LogBssid(const uint8_t *bssid);

void LogChreScanResult(const chreWifiScanResult &result);

const char *GetErrorCodeName(ErrorCode error_code);

}  // namespace utility
}  // namespace wifi_offload

#endif  // CHRE_WIFI_OFFLOAD_UTILITY_H_
