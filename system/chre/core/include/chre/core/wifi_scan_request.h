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

#ifndef CHRE_CORE_WIFI_SCAN_REQUEST_H_
#define CHRE_CORE_WIFI_SCAN_REQUEST_H_

#include <cstddef>

#include "chre_api/chre/wifi.h"
#include "chre/util/dynamic_vector.h"
#include "chre/util/fixed_size_vector.h"
#include "chre/util/time.h"

namespace chre {

/**
 * The maximum length for a Wifi SSID. This value is taken from 802.11 7.3.2.1
 * and can also be found in the CHRE API wifi.h.
 */
constexpr size_t kMaxWifiSsidLength = 32;

/**
 * This WifiScanType is designed to wrap constants provided by the CHRE API to
 * improve type-safety. In addition, an invalid wifi scan type is added for
 * handling an app that is not requesting wifi scans.
 */
enum class WifiScanType {
  Invalid,
  Active,
  ActivePlusPassiveDfs,
  Passive
};

/**
 * Translates a CHRE API enum wifi scan type to a WifiScanType. This funciton
 * also performs input validation and will default to WifiScanType::Invalid if
 * the provided value is not a valid enumeration.
 *
 * @param enumWifiScanType A potentially unsafe CHRE API enum wifi scan type.
 * @return a WifiScanType given a CHRE API wifi scan type.
 */
WifiScanType getWifiScanTypeForEnum(enum chreWifiScanType enumWifiScanType);

/**
 * An SSID can be modelled by a list of bytes.
 */
typedef FixedSizeVector<uint8_t, kMaxWifiSsidLength> WifiSsid;

/**
 * Models a request for wifi scans. This class implements the API set forth by
 * the RequestMultiplexer container in addition to specific functionality
 * required for requesting wifi scans.
 */
class WifiScanRequest {
 public:
  /**
   * Default constructs a wifi scan request to the minimal possible
   * configuration. The WifiScanType is set to Invalid and the frequency and
   * SSID lists are both cleared.
   */
  WifiScanRequest();

  /**
   * Constructs a request for wifi scan results given a scan type, maximum scan
   * age, frequencies and SSID list as specified by the CHRE API. More details
   * about the parameters here can be found in the CHRE API.
   *
   * @param wifiScanType The type of scan being requested.
   * @param maxScanAge The maximum age of a detected wifi network to be
   *        reported.
   * @param frequencies The list of frequencies to search for networks on.
   * @param ssids The list of SSIDs to specifically search for.
   */
  WifiScanRequest(WifiScanType wifiScanType,
                  const Nanoseconds& maxScanAge,
                  DynamicVector<uint32_t>&& frequencies,
                  DynamicVector<WifiSsid>&& ssids);

  /**
   * @return the type of this scan request.
   */
  WifiScanType getScanType() const;

  /**
   * @return the maximum age of a scan result for this request.
   */
  const Nanoseconds& getMaxScanAge() const;

  /**
   * @return the frequencies associated with this request.
   */
  const DynamicVector<uint32_t>& getFrequencies() const;

  /**
   * @return the SSIDs associated with this request.
   */
  const DynamicVector<WifiSsid>& getSsids() const;

 private:
  //! The type of request for this scan.
  WifiScanType mScanType;

  //! The maximum allowable age for a scan result.
  Nanoseconds mMaxScanAge;

  //! The list of frequencies associated with this scan request.
  DynamicVector<uint32_t> mFrequencies;

  //! The list of SSIDs associated with this scan request.
  DynamicVector<WifiSsid> mSsids;
};

}  // namespace chre

#endif  // CHRE_CORE_WIFI_SCAN_REQUEST_H_
