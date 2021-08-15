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

#ifndef WIFICOND_LOGGING_UTILS_H_
#define WIFICOND_LOGGING_UTILS_H_

#include <array>
#include <sstream>

#include <linux/if_ether.h>

#include <android-base/macros.h>

#include "wificond/net/netlink_manager.h"

namespace android {
namespace wificond {

class LoggingUtils {
 public:
  LoggingUtils() = default;
  static std::string GetMacString(const std::array<uint8_t, ETH_ALEN>& mac_address);
  static std::string GetBandwidthString(ChannelBandwidth bandwidth);

 private:

  DISALLOW_COPY_AND_ASSIGN(LoggingUtils);
};

}  // namespace wificond
}  // namespace android

#endif  // WIFICOND_LOGGING_UTILS_H_
