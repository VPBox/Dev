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
#include "offload_hal_test_constants.h"

namespace android {
namespace wificond {
namespace offload_hal_test_constants {

const uint8_t kSsid1[] = {'G', 'o', 'o', 'g', 'l', 'e'};
const size_t kSsid1_size = sizeof(kSsid1);
const uint8_t kSsid2[] = {'X', 'f', 'i', 'n', 'i', 't', 'y'};
const size_t kSsid2_size = sizeof(kSsid2);
const uint8_t kBssid[6] = {0x12, 0xef, 0xa1, 0x2c, 0x97, 0x8b};
const int16_t kRssi = -60;
const int16_t kRssiThreshold = -76;
const uint32_t kFrequency1 = 2412;
const uint32_t kFrequency2 = 2437;
const uint8_t kBssidSize = 6;
const uint64_t kTsf = 0;
const uint16_t kCapability = 0;
const uint8_t kNetworkFlags = 0;
const uint32_t kDisconnectedModeScanIntervalMs = 5000;
const uint64_t kSubscriptionDurationMs = 10000;
const uint64_t kScanDurationMs[2] = {2000, 500};
const uint32_t kNumChannelsScanned[2] = {14, 6};
const uint8_t kDefaultNumTimesAChannelsIsScanned = 1;
const uint8_t kChannelNotScanned = 0;
const uint32_t kDefaultNumScansRequestedByWifi = 2;
const uint32_t kDefaultNumScansServicedByWifi = 2;
const uint64_t kScanDurationTotalMs = 2000;
const uint32_t kNumChannelsTotalScanned = 20;
const uint32_t kNumChannelsInHistogram = 256;
const uint64_t kDeathCode = 0xBEEF;

}  // namespace offload_hal_test_constants
}  // namespace wificond
}  // namespace android
