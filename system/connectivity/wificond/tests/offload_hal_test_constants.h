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

#ifndef WIFICOND_OFFLOAD_HAL_TEST_CONSTANTS_H_
#define WIFICOND_OFFLOAD_HAL_TEST_CONSTANTS_H_

#include <cstdint>

namespace android {
namespace wificond {
namespace offload_hal_test_constants {

extern const uint8_t kSsid1[];
extern const size_t kSsid1_size;
extern const uint8_t kSsid2[];
extern const size_t kSsid2_size;
extern const uint8_t kBssid[6];
extern const int16_t kRssi;
extern const int16_t kRssiThreshold;
extern const uint32_t kFrequency1;
extern const uint32_t kFrequency2;
extern const uint8_t kBssidSize;
extern const uint64_t kTsf;
extern const uint16_t kCapability;
extern const uint8_t kNetworkFlags;
extern const uint32_t kDisconnectedModeScanIntervalMs;
extern const uint64_t kSubscriptionDurationMs;
extern const uint64_t kScanDurationMs[2];
extern const uint32_t kNumChannelsScanned[2];
extern const uint8_t kDefaultNumTimesAChannelsIsScanned;
extern const uint8_t kChannelNotScanned;
extern const uint32_t kDefaultNumScansRequestedByWifi;
extern const uint32_t kDefaultNumScansServicedByWifi;
extern const uint64_t kScanDurationTotalMs;
extern const uint32_t kNumChannelsTotalScanned;
extern const uint32_t kNumChannelsInHistogram;
extern const uint64_t kDeathCode;

}  // namespace offload_hal_test_constants
}  // namespace wificond
}  // namespace android

#endif  // WIFICOND_OFFLOAD_HAL_TEST_CONSTANTS_H_
