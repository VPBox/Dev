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

#include "gtest/gtest.h"

#include "include/utility.h"

TEST(ScanResultTest, ConstructOffloadScanResultBasedOnChreWifiScanResult) {
  wifi_offload_test::RandomGenerator random_gen;
  chreWifiScanResult chre_scan_result;
  init(chre_scan_result, random_gen);
  wifi_offload::ScanResult nanoapp_scan_result(chre_scan_result);

  wifi_offload::Ssid ssid;
  ssid.SetData(chre_scan_result.ssid, chre_scan_result.ssidLen);
  ASSERT_EQ(ssid, nanoapp_scan_result.ssid_);
  EXPECT_EQ(chre_scan_result.securityMode, nanoapp_scan_result.security_modes_);
  EXPECT_EQ(0, std::memcmp(chre_scan_result.bssid, nanoapp_scan_result.bssid_,
                           CHRE_WIFI_BSSID_LEN));
  EXPECT_EQ(chre_scan_result.capabilityInfo, nanoapp_scan_result.capability_);
  EXPECT_EQ(chre_scan_result.primaryChannel,
            nanoapp_scan_result.frequency_scanned_mhz_);
  EXPECT_EQ(chre_scan_result.rssi, nanoapp_scan_result.rssi_dbm_);
  EXPECT_EQ(0, nanoapp_scan_result.tsf_);
}
