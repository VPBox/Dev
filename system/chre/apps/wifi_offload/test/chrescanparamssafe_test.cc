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

#include "chre/apps/wifi_offload/chre_scan_params_safe.h"
#include "include/utility.h"

class ChreScanParamsSafeTest : public testing::Test {
 public:
  wifi_offload_test::RandomGenerator random_gen_;
  wifi_offload::ScanParams nanoapp_scan_params_;

  void ConstructChreScanParamsSafeAndCompareWithOrigScanParams() {
    wifi_offload::ChreScanParamsSafe chre_scan_params_safe(
        nanoapp_scan_params_);
    const chreWifiScanParams *chre_scan_params =
        chre_scan_params_safe.GetChreWifiScanParams();

    EXPECT_EQ(CHRE_WIFI_SCAN_TYPE_ACTIVE_PLUS_PASSIVE_DFS,
              chre_scan_params->scanType);
    EXPECT_EQ(0, chre_scan_params->maxScanAgeMs);

    ASSERT_EQ(nanoapp_scan_params_.ssids_to_scan_.size(),
              chre_scan_params->ssidListLen);
    for (size_t i = 0; i < chre_scan_params->ssidListLen; i++) {
      chreWifiSsidListItem ssid_item;
      nanoapp_scan_params_.ssids_to_scan_[i].ToChreWifiSsidListItem(&ssid_item);
      ASSERT_EQ(ssid_item.ssidLen, chre_scan_params->ssidList[i].ssidLen);
      EXPECT_EQ(0,
                std::memcmp(ssid_item.ssid, chre_scan_params->ssidList[i].ssid,
                            chre_scan_params->ssidList[i].ssidLen));
    }
    ASSERT_EQ(nanoapp_scan_params_.frequencies_to_scan_mhz_.size(),
              chre_scan_params->frequencyListLen);
    EXPECT_EQ(
        0, std::memcmp(nanoapp_scan_params_.frequencies_to_scan_mhz_.data(),
                       chre_scan_params->frequencyList,
                       chre_scan_params->frequencyListLen * sizeof(uint32_t)));
  }
};

TEST_F(ChreScanParamsSafeTest,
       ConstructsChreScanParamsSafeAndComparesWithOriginalScanParams) {
  init(nanoapp_scan_params_.frequencies_to_scan_mhz_, random_gen_,
       CHRE_WIFI_FREQUENCY_LIST_MAX_LEN - 5);
  init(nanoapp_scan_params_.ssids_to_scan_, random_gen_,
       CHRE_WIFI_SSID_LIST_MAX_LEN - 2);
  ConstructChreScanParamsSafeAndCompareWithOrigScanParams();
}

TEST_F(ChreScanParamsSafeTest, ConstructsChreScanParamsSafeWithEmptyFreqList) {
  init(nanoapp_scan_params_.ssids_to_scan_, random_gen_,
       CHRE_WIFI_SSID_LIST_MAX_LEN - 2);
  ConstructChreScanParamsSafeAndCompareWithOrigScanParams();
}

TEST_F(ChreScanParamsSafeTest, ConstructsChreScanParamsSafeWithEmptySsidList) {
  init(nanoapp_scan_params_.frequencies_to_scan_mhz_, random_gen_,
       CHRE_WIFI_FREQUENCY_LIST_MAX_LEN - 5);
  ConstructChreScanParamsSafeAndCompareWithOrigScanParams();
}

TEST_F(ChreScanParamsSafeTest, ChreScanParamsSafeTruncatesLongLists) {
  // initialize frequency and ssid lists to exceed limit size
  init(nanoapp_scan_params_.frequencies_to_scan_mhz_, random_gen_,
       CHRE_WIFI_FREQUENCY_LIST_MAX_LEN + 5);
  init(nanoapp_scan_params_.ssids_to_scan_, random_gen_,
       CHRE_WIFI_SSID_LIST_MAX_LEN + 2);

  wifi_offload::ChreScanParamsSafe chre_scan_params_safe(nanoapp_scan_params_);
  const chreWifiScanParams *chre_scan_params =
      chre_scan_params_safe.GetChreWifiScanParams();

  EXPECT_EQ(CHRE_WIFI_SSID_LIST_MAX_LEN, chre_scan_params->ssidListLen);
  EXPECT_EQ(CHRE_WIFI_FREQUENCY_LIST_MAX_LEN,
            chre_scan_params->frequencyListLen);
}
