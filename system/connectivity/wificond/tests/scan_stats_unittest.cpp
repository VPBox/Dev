/*
 * Copyright (C) 2017, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <vector>

#include <gtest/gtest.h>

#include "wificond/scanning/offload/scan_stats.h"
#include "wificond/tests/offload_hal_test_constants.h"

using ::com::android::server::wifi::wificond::NativeScanStats;
using namespace android::wificond::offload_hal_test_constants;

namespace android {
namespace wificond {

class ScanStatsTest : public ::testing::Test {};

TEST_F(ScanStatsTest, ParcelableTest) {
  std::vector<uint8_t> histogram_channels;
  for (size_t i = 0; i < kNumChannelsInHistogram; i++) {
    histogram_channels.push_back(kNumChannelsInHistogram - 1 - i);
  }
  NativeScanStats scan_stats_in(kDefaultNumScansRequestedByWifi,
                                kDefaultNumScansServicedByWifi,
                                kScanDurationTotalMs, kSubscriptionDurationMs,
                                kNumChannelsTotalScanned, histogram_channels);
  Parcel parcel;
  EXPECT_EQ(::android::OK, scan_stats_in.writeToParcel(&parcel));
  NativeScanStats scan_stats_out;
  parcel.setDataPosition(0);
  EXPECT_EQ(::android::OK, scan_stats_out.readFromParcel(&parcel));
  EXPECT_TRUE(scan_stats_in == scan_stats_out);
}

}  // namespace wificond
}  // namespace android
