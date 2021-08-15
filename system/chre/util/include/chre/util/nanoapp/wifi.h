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

#ifndef CHRE_UTIL_NANOAPP_WIFI_H_
#define CHRE_UTIL_NANOAPP_WIFI_H_

#include <cstddef>
#include <cstdint>

#include <chre/wifi.h>

namespace chre {

//! The length of a string SSID with null-terminator.
constexpr size_t kMaxSsidStrLen = CHRE_WIFI_SSID_MAX_LEN + 1;

//! The length of a formatted BSSID string in XX:XX:XX:XX:XX:XX\0 format.
constexpr size_t kBssidStrLen = 18;

/**
 * @param buffer a pointer to a buffer to format into.
 * @param bufferLen a buffer to format into.
 * @param a pointer to SSID data.
 * @param ssidLen the length of the SSID data.
 * @return true if the SSID is printable and was copied into the output buffer.
 */
bool parseSsidToStr(char *buffer, size_t bufferLen,
                    const uint8_t *ssid, uint8_t ssidLen);

/**
 * Parses a BSSID into a XX:XX:XX:XX:XX:XX formatted string.
 *
 * @param bssid the BSSID to format into a string.
 * @param buffer the buffer to format the string into.
 * @param bufferLen the length of the buffer to format into.
 * @return true if the buffer is large enough and the string is formatted.
 */
bool parseBssidToStr(const uint8_t bssid[CHRE_WIFI_BSSID_LEN],
                     char *buffer, size_t bufferLen);

/**
 * Parses a WiFi band into a string.
 *
 * @param band the CHRE WiFi band to parse into a string.
 * @return a pointer to the string or some indication of invalid.
 */
const char *parseChreWifiBand(uint8_t band);

}  // namespace chre

#endif  // CHRE_UTIL_NANOAPP_WIFI_H_
