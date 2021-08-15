/*
 * Copyright (C) 2016, The Android Open Source Project
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

#include <android-base/logging.h>
#include <gtest/gtest.h>
#include "wificond/scanning/offload/offload_scan_utils.h"
#include "wificond/scanning/scan_result.h"
#include "wificond/tests/offload_hal_test_constants.h"
#include "wificond/tests/offload_test_utils.h"

using android::hardware::wifi::offload::V1_0::ScanResult;
using android::hardware::wifi::offload::V1_0::ScanParam;
using android::hardware::wifi::offload::V1_0::ScanFilter;
using android::hardware::wifi::offload::V1_0::NetworkInfo;
using android::hardware::wifi::offload::V1_0::ScanRecord;
using android::hardware::wifi::offload::V1_0::ScanStats;
using ::com::android::server::wifi::wificond::NativeScanResult;
using ::com::android::server::wifi::wificond::NativeScanStats;
using std::vector;

using namespace android::wificond::offload_hal_test_constants;

namespace android {
namespace wificond {

class OffloadScanUtilsTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    dummy_scan_results_ = OffloadTestUtils::createOffloadScanResults();
  }

  void TearDown() override { dummy_scan_results_.clear(); }

  vector<ScanResult> dummy_scan_results_;
};

TEST_F(OffloadScanUtilsTest, verifyConversion) {
  vector<NativeScanResult> native_scan_results;
  EXPECT_TRUE(OffloadScanUtils::convertToNativeScanResults(
      dummy_scan_results_, &native_scan_results));
  EXPECT_EQ(native_scan_results.size(), dummy_scan_results_.size());
  for (size_t i = 0; i < native_scan_results.size(); i++) {
    EXPECT_EQ(native_scan_results[i].frequency,
              dummy_scan_results_[i].frequency);
    EXPECT_EQ(native_scan_results[i].signal_mbm, dummy_scan_results_[i].rssi);
    EXPECT_EQ(native_scan_results[i].ssid.size(),
              dummy_scan_results_[i].networkInfo.ssid.size());
    EXPECT_EQ(native_scan_results[i].bssid.size(),
              dummy_scan_results_[i].bssid.elementCount());
    EXPECT_EQ(native_scan_results[i].capability,
              dummy_scan_results_[i].capability);
  }
}

TEST_F(OffloadScanUtilsTest, verifyScanParam) {
  vector<vector<uint8_t>> scan_ssids{kSsid1, kSsid2};
  vector<uint32_t> frequencies{kFrequency1, kFrequency2};
  ScanParam scanParam = OffloadScanUtils::createScanParam(
      scan_ssids, frequencies, kDisconnectedModeScanIntervalMs);
  EXPECT_EQ(scanParam.disconnectedModeScanIntervalMs,
            kDisconnectedModeScanIntervalMs);
  for (size_t i = 0; i < frequencies.size(); i++) {
    EXPECT_EQ(scanParam.frequencyList[i], frequencies[i]);
  }
  for (size_t j = 0; j < scan_ssids.size(); j++) {
    vector<uint8_t> ssid_result = scanParam.ssidList[j];
    vector<uint8_t> ssid_input = scan_ssids[j];
    for (size_t k = 0; k < ssid_result.size(); k++) {
      EXPECT_EQ(ssid_result[k], ssid_input[k]);
    }
  }
}

TEST_F(OffloadScanUtilsTest, verifyScanFilter) {
  vector<vector<uint8_t>> match_ssids{kSsid1, kSsid2};
  vector<uint8_t> security_flags{kNetworkFlags, kNetworkFlags};
  ScanFilter scanFilter = OffloadScanUtils::createScanFilter(
      match_ssids, security_flags, kRssiThreshold);
  EXPECT_EQ(kRssiThreshold, scanFilter.rssiThreshold);
  EXPECT_FALSE(scanFilter.preferredNetworkInfoList.size() == 0);
  for (size_t i = 0; i < security_flags.size(); ++i) {
    NetworkInfo nwInfo = scanFilter.preferredNetworkInfoList[i];
    vector<uint8_t> ssid = nwInfo.ssid;
    vector<uint8_t> match_ssid = match_ssids[i];
    EXPECT_EQ(nwInfo.flags, security_flags[i]);
    for (size_t j = 0; j < ssid.size(); j++) {
      EXPECT_EQ(ssid[j], match_ssid[j]);
    }
  }
}

TEST_F(OffloadScanUtilsTest, verifyScanStats) {
  NativeScanStats stats_expected;
  ScanStats offload_scan_stats =
      OffloadTestUtils::createScanStats(&stats_expected);
  NativeScanStats stats_returned =
      OffloadScanUtils::convertToNativeScanStats(offload_scan_stats);
  EXPECT_TRUE(stats_expected == stats_returned);
}

}  // namespace wificond
}  // namespace android
