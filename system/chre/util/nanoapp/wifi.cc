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

#include "chre/util/nanoapp/wifi.h"

#include <cctype>
#include <cinttypes>
#include <cstdio>
#include <cstring>

namespace chre {

bool parseSsidToStr(char *buffer, size_t bufferLen,
                    const uint8_t *ssid, uint8_t ssidLen) {
  // Ensure that there is enough space in the buffer to copy the SSID and
  // null-terminate it.
  bool success = (bufferLen >= static_cast<size_t>(ssidLen + 1));

  if (success) {
    // Verify that the ssid is entirely printable characters and ASCII spaces.
    for (uint8_t i = 0; i < ssidLen; i++) {
      if (!isgraph(ssid[i]) && ssid[i] != ' ') {
        success = false;
        break;
      }
    }
  }

  if (success) {
    // Copy the SSID to the buffer and null-terminate.
    memcpy(buffer, ssid, ssidLen + 1);
    buffer[ssidLen] = '\0';
  }

  return success;
}

bool parseBssidToStr(const uint8_t bssid[CHRE_WIFI_BSSID_LEN],
                     char *buffer, size_t bufferLen) {
  const char *kFormat = "%02" PRIx8 ":%02" PRIx8 ":%02" PRIx8
                        ":%02" PRIx8 ":%02" PRIx8 ":%02" PRIx8;

  bool success = false;
  if (bufferLen >= kBssidStrLen) {
    success = true;
    snprintf(buffer, bufferLen, kFormat, bssid[0], bssid[1], bssid[2],
             bssid[3], bssid[4], bssid[5]);
  }

  return success;
}

const char *parseChreWifiBand(uint8_t band) {
  switch (band) {
    case CHRE_WIFI_BAND_2_4_GHZ:
      return "2.4GHz";
    case CHRE_WIFI_BAND_MASK_5_GHZ:
      return "5GHz";
    default:
      return "<invalid>";
  }
}

}  // namespace chre
