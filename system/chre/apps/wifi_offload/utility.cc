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
#include <cctype>

#include "chre/apps/wifi_offload/utility.h"
#include "chre/apps/wifi_offload/wifi_offload.h"

namespace wifi_offload {
namespace utility {
namespace {

// The length of a string SSID with null-terminator.
constexpr size_t kMaxSsidStrLen = CHRE_WIFI_SSID_MAX_LEN + 1;
// The length of a formatted BSSID string in XX:XX:XX:XX:XX:XX\0 format.
constexpr size_t kBssidStrLen = 18;

bool ParseSsidToStr(const uint8_t *ssid, size_t ssid_len, char *ssid_str,
                    size_t ssid_str_len) {
  if (ssid_str_len < ssid_len + 1) {
    return false;
  }
  // Verify that the ssid is entirely printable characters and ASCII spaces.
  for (uint8_t i = 0; i < ssid_len; i++) {
    if (!std::isgraph(ssid[i]) && ssid[i] != ' ') {
      return false;
    }
  }

  std::memcpy(ssid_str, ssid, ssid_len);
  ssid_str[ssid_len] = '\0';
  return true;
}

bool ParseBssidToStr(const uint8_t bssid[CHRE_WIFI_BSSID_LEN], char *bssid_str,
                     size_t bssid_str_len) {
  if (bssid_str_len < kBssidStrLen) {
    return false;
  }

  const char *kFormat = "%02" PRIx8 ":%02" PRIx8 ":%02" PRIx8 ":%02" PRIx8
                        ":%02" PRIx8 ":%02" PRIx8;
  std::snprintf(bssid_str, bssid_str_len, kFormat, bssid[0], bssid[1], bssid[2],
                bssid[3], bssid[4], bssid[5]);
  return true;
}

const char *ParseChreWifiBand(uint8_t band) {
  switch (band) {
    case CHRE_WIFI_BAND_2_4_GHZ:
      return "2.4GHz";
    case CHRE_WIFI_BAND_5_GHZ:
      return "5GHz";
    default:
      return "<invalid>";
  }
}

}  // namespace

int Ieee80211FrequencyToChannel(int freq) {
  /* see 802.11-2007 17.3.8.3.2 and Annex J */
  if (freq == 2484)
    return 14;
  else if (freq < 2484)
    return (freq - 2407) / 5;
  else if (freq >= 4910 && freq <= 4980)
    return (freq - 4000) / 5;
  else if (freq <= 45000) /* DMG band lower limit */
    return (freq - 5000) / 5;
  else if (freq >= 58320 && freq <= 64800)
    return (freq - 56160) / 2160;
  else
    return 0;
}

void LogSsid(const uint8_t *ssid, uint8_t ssid_len) {
  const char *ssid_str = "<non-printable>";
  char ssid_buffer[std::max<size_t>(kMaxSsidStrLen,
                                    CHRE_WIFI_SSID_MAX_LEN * 3)];
  if (ssid_len == 0) {
    ssid_str = "<empty>";
  } else if (ParseSsidToStr(ssid, ssid_len, ssid_buffer, kMaxSsidStrLen)) {
    ssid_str = ssid_buffer;
  } else {
    // ssid has non-printable ASCII chars, parse in hex format
    char *buf_ptr = ssid_buffer;
    for (size_t i = 0; i < ssid_len; i++) {
      buf_ptr += std::sprintf(buf_ptr, "%02" PRIx8 ":", ssid[i]);
    }
    buf_ptr[-1] = '\0';
    ssid_str = ssid_buffer;
  }
  LOGI("  ssid: %s", ssid_str);
}

void LogBssid(const uint8_t *bssid) {
  const char *bssid_str = "<non-printable>";
  char bssidBuffer[kBssidStrLen];
  if (ParseBssidToStr(bssid, bssidBuffer, kBssidStrLen)) {
    bssid_str = bssidBuffer;
  }
  LOGI("  bssid: %s", bssid_str);
}

void LogChreScanResult(const chreWifiScanResult &result) {
  LOGI("chreWifiScanResult:");
  LogSsid(result.ssid, result.ssidLen);
  LOGI("  age (ms): %" PRIu32, result.ageMs);
  LOGI("  capability info: 0x%" PRIx16, result.capabilityInfo);
  LogBssid(result.bssid);
  LOGI("  flags: 0x%" PRIx8, result.flags);
  LOGI("  rssi: %" PRId8 "dBm", result.rssi);
  LOGI("  band: %s (%" PRIu8 ")", ParseChreWifiBand(result.band), result.band);
  LOGI("  primary channel: %" PRIu32, result.primaryChannel);
  LOGI("  center frequency primary: %" PRIu32, result.centerFreqPrimary);
  LOGI("  center frequency secondary: %" PRIu32, result.centerFreqSecondary);
  LOGI("  channel width: %" PRIu8, result.channelWidth);
  LOGI("  security mode: %" PRIu8, result.securityMode);
}

const char *GetErrorCodeName(ErrorCode error_code) {
  switch (error_code) {
    case SUCCESS:
      return "SUCCESS";
    case FAILED_TO_ALLOCATE_MESSAGE_BUFFER:
      return "FAILED_TO_ALLOCATE_MESSAGE_BUFFER";
    case FAILED_TO_SERIALIZE_MESSAGE:
      return "FAILED_TO_SERIALIZE_MESSAGE";
    case FAILED_TO_SEND_MESSAGE:
      return "FAILED_TO_SEND_MESSAGE";
    case FAILED_TO_DESERIALIZE_SCAN_CONFIG:
      return "FAILED_TO_DESERIALIZE_SCAN_CONFIG";
    case INVALID_SUBSCRIBE_MESSAGE_SIZE:
      return "INVALID_SUBSCRIBE_MESSAGE_SIZE";
    case SCAN_CONFIG_NOT_INITIALIZED:
      return "SCAN_CONFIG_NOT_INITIALIZED";
    case UNSPECIFIED_HOST_ENDPOINT:
      return "UNSPECIFIED_HOST_ENDPOINT";
    case FAILED_TO_SEND_SCAN_RESULTS:
      return "FAILED_TO_SEND_SCAN_RESULTS";
    case FAILED_TO_SEND_SCAN_STATS:
      return "FAILED_TO_SEND_SCAN_STATS";
    case SCAN_MONITORING_NOT_SUPPORTED:
      return "SCAN_MONITORING_NOT_SUPPORTED";
    case FAILED_TO_START_SCAN_MONITORING:
      return "FAILED_TO_START_SCAN_MONITORING";
    case FAILED_TO_STOP_SCAN_MONITORING:
      return "FAILED_TO_STOP_SCAN_MONITORING";
    case FAILED_TO_CONFIGURE_SCAN_MONITORING_ASYNC:
      return "FAILED_TO_CONFIGURE_SCAN_MONITORING_ASYNC";
    case ONDEMAND_SCAN_NOT_SUPPORTED:
      return "ONDEMAND_SCAN_NOT_SUPPORTED";
    case FAILED_TO_SEND_ONDEMAND_SCAN_REQUEST:
      return "FAILED_TO_SEND_ONDEMAND_SCAN_REQUEST";
    case FAILED_TO_SEND_ONDEMAND_SCAN_REQUEST_ASYNC:
      return "FAILED_TO_SEND_ONDEMAND_SCAN_REQUEST_ASYNC";
    case OUT_OF_ORDER_SCAN_RESULTS:
      return "OUT_OF_ORDER_SCAN_RESULTS";
    case INCOMPLETE_SCAN_RESULTS_BEFORE_SCAN_REQUEST:
      return "INCOMPLETE_SCAN_RESULTS_BEFORE_SCAN_REQUEST";
    case FAILED_TO_SET_SCAN_TIMER:
      return "FAILED_TO_SET_SCAN_TIMER";
    default:
      return "UNKNOWN_ERROR";
  }
}

}  // namespace utility
}  // namespace wifi_offload
