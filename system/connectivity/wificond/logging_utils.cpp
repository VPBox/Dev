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

#include "wificond/logging_utils.h"

#include <array>
#include <iomanip>

#include <android-base/macros.h>

using std::array;
using std::string;
using std::stringstream;

namespace android {
namespace wificond {

string LoggingUtils::GetMacString(const array<uint8_t, ETH_ALEN>& mac_address) {
  stringstream ss;
  for (const uint8_t& b : mac_address) {
    ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(b);
    if (&b != &mac_address.back()) {
      ss << ":";
    }
  }
  return ss.str();
}

string LoggingUtils::GetBandwidthString(ChannelBandwidth bandwidth) {
  switch (bandwidth) {
    case BW_20_NOHT:
      return "20MHz no HT";
    case BW_20:
      return "20MHz with HT";
    case BW_40:
      return "40MHz";
    case BW_80:
      return "80MHz";
    case BW_80P80:
      return "80+80MHz";
    case BW_160:
      return "160MHz";
    default:
      return "Invalid";
  }
  return "Invalid";
}

}  // namespace wificond
}  // namespace android
