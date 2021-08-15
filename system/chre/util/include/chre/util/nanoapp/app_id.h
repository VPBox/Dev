
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

#ifndef CHRE_UTIL_NANOAPP_APP_ID_H_
#define CHRE_UTIL_NANOAPP_APP_ID_H_

#include <chre/common.h>
#include <stdint.h>

/**
 * @file
 * Helpers for defining static nanoapp IDs, e.g. for use with
 * CHRE_STATIC_NANOAPP_INIT.
 */

//! Sample Vendor ID for example nanoapps. The vendor ID portion is the 5 most
//! significant bytes of the app ID. This vendor ID should not appear in
//! production.
#define CHRE_VENDOR_ID_EXAMPLE  UINT64_C(0x0123456789000000)

namespace chre {

/**
 * Constructs a fully qualified nanoapp ID from a vendor portion (most
 * significant 5 bytes) and app number (least significant 3 bytes)
 *
 * @param vendor Value with vendor unique identifier in the most significant 5
 *        bytes
 * @param appNumber Value with vendor-scoped app number in the least significant
 *        3 bytes
 *
 * @return app ID combining vendor and app number
 */
constexpr uint64_t makeNanoappId(uint64_t vendor, uint32_t appNumber) {
  return ((vendor & CHRE_VENDOR_ID_MASK) | (appNumber & ~CHRE_VENDOR_ID_MASK));
}

/**
 * @return App ID combining the given 3-byte app number and Google's 5-byte
 *         vendor ID
 */
constexpr uint64_t makeExampleNanoappId(uint32_t appNumber) {
  return makeNanoappId(CHRE_VENDOR_ID_EXAMPLE, appNumber);
}

/**
 * @return App ID combining the given 3-byte app number and Google's 5-byte
 *         vendor ID
 */
constexpr uint64_t makeGoogleNanoappId(uint32_t appNumber) {
  return makeNanoappId(CHRE_VENDOR_ID_GOOGLE, appNumber);
}

constexpr uint64_t kHelloWorldAppId       = makeExampleNanoappId(1);
constexpr uint64_t kMessageWorldAppId     = makeExampleNanoappId(2);
constexpr uint64_t kTimerWorldAppId       = makeExampleNanoappId(3);
constexpr uint64_t kSensorWorldAppId      = makeExampleNanoappId(4);
constexpr uint64_t kGnssWorldAppId        = makeExampleNanoappId(5);
constexpr uint64_t kWifiWorldAppId        = makeExampleNanoappId(6);
constexpr uint64_t kWwanWorldAppId        = makeExampleNanoappId(7);
// 8 = reserved (previously used by ImuCal)
constexpr uint64_t kSpammerAppId          = makeExampleNanoappId(9);
constexpr uint64_t kUnloadTesterAppId     = makeExampleNanoappId(10);
constexpr uint64_t kAshWorldAppId         = makeExampleNanoappId(11);
constexpr uint64_t kAudioWorldAppId       = makeExampleNanoappId(12);
constexpr uint64_t kHostAwakeWorldAppId   = makeExampleNanoappId(13);
constexpr uint64_t kAudioStressTestAppId  = makeExampleNanoappId(14);

}  // namespace chre

#endif  // CHRE_UTIL_NANOAPP_APP_ID_H_
