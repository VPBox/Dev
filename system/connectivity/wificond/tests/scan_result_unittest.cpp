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

#include <array>
#include <vector>

#include <linux/if_ether.h>

#include <gtest/gtest.h>

#include "wificond/scanning/scan_result.h"

using ::com::android::server::wifi::wificond::NativeScanResult;
using ::com::android::server::wifi::wificond::RadioChainInfo;
using std::array;
using std::vector;

namespace android {
namespace wificond {

namespace {


const uint8_t kFakeSsid[] =
    {'G', 'o', 'o', 'g', 'l', 'e', 'G', 'u', 'e', 's', 't'};
const array<uint8_t, ETH_ALEN> kFakeBssid = {0x45, 0x54, 0xad, 0x67, 0x98, 0xf6};
const uint8_t kFakeIE[] = {0x05, 0x11, 0x32, 0x11};
constexpr uint32_t kFakeFrequency = 5240;
constexpr int32_t kFakeSignalMbm= -32;
constexpr uint64_t kFakeTsf = 1200;
constexpr int16_t kFakeCapability = 0;
constexpr bool kFakeAssociated = true;
constexpr int32_t kFakeRadioChainIds[] = { 0, 1 };
constexpr int32_t kFakeRadioChainLevels[] = { -56, -64};

}  // namespace

class ScanResultTest : public ::testing::Test {
};

TEST_F(ScanResultTest, ParcelableTest) {
  std::vector<uint8_t> ssid(kFakeSsid, kFakeSsid + sizeof(kFakeSsid));
  array<uint8_t, ETH_ALEN> bssid = kFakeBssid;
  std::vector<uint8_t> ie(kFakeIE, kFakeIE + sizeof(kFakeIE));
  std::vector<RadioChainInfo> radio_chain_infos;
  radio_chain_infos.emplace_back(
      kFakeRadioChainIds[0], kFakeRadioChainLevels[0]);
  radio_chain_infos.emplace_back(
      kFakeRadioChainIds[1], kFakeRadioChainLevels[1]);

  NativeScanResult scan_result(ssid, bssid, ie, kFakeFrequency,
      kFakeSignalMbm, kFakeTsf, kFakeCapability, kFakeAssociated,
      radio_chain_infos);

  Parcel parcel;
  EXPECT_EQ(::android::OK, scan_result.writeToParcel(&parcel));

  NativeScanResult scan_result_copy;
  parcel.setDataPosition(0);
  EXPECT_EQ(::android::OK, scan_result_copy.readFromParcel(&parcel));

  EXPECT_EQ(ssid, scan_result_copy.ssid);
  EXPECT_EQ(bssid, scan_result_copy.bssid);
  EXPECT_EQ(ie, scan_result_copy.info_element);
  EXPECT_EQ(kFakeFrequency, scan_result_copy.frequency);
  EXPECT_EQ(kFakeSignalMbm, scan_result_copy.signal_mbm);
  EXPECT_EQ(kFakeTsf, scan_result_copy.tsf);
  EXPECT_EQ(kFakeCapability, scan_result_copy.capability);
  EXPECT_EQ(kFakeAssociated, scan_result_copy.associated);
  EXPECT_EQ(2u, scan_result_copy.radio_chain_infos.size());
  EXPECT_EQ(kFakeRadioChainIds[0], scan_result_copy.radio_chain_infos[0].chain_id);
  EXPECT_EQ(kFakeRadioChainIds[1], scan_result_copy.radio_chain_infos[1].chain_id);
  EXPECT_EQ(kFakeRadioChainLevels[0], scan_result_copy.radio_chain_infos[0].level);
  EXPECT_EQ(kFakeRadioChainLevels[1], scan_result_copy.radio_chain_infos[1].level);
}

}  // namespace wificond
}  // namespace android
