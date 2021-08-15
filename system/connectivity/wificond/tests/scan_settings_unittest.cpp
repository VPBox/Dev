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

#include <gtest/gtest.h>

#include "android/net/wifi/IWifiScannerImpl.h"
#include "wificond/scanning/channel_settings.h"
#include "wificond/scanning/hidden_network.h"
#include "wificond/scanning/pno_network.h"
#include "wificond/scanning/pno_settings.h"
#include "wificond/scanning/single_scan_settings.h"

using ::android::net::wifi::IWifiScannerImpl;
using ::com::android::server::wifi::wificond::ChannelSettings;
using ::com::android::server::wifi::wificond::HiddenNetwork;
using ::com::android::server::wifi::wificond::PnoNetwork;
using ::com::android::server::wifi::wificond::PnoSettings;
using ::com::android::server::wifi::wificond::SingleScanSettings;
using std::vector;

namespace android {
namespace wificond {

namespace {

const uint8_t kFakeSsid[] =
    {'G', 'o', 'o', 'g', 'l', 'e', 'G', 'u', 'e', 's', 't'};
const uint8_t kFakeSsid1[] =
    {'A', 'n', 'd', 'r', 'o', 'i', 'd', 'A', 'P', 'T', 'e', 's', 't'};

constexpr int32_t kFakePnoIntervalMs = 20000;
constexpr int32_t kFakePnoMin2gRssi = -80;
constexpr int32_t kFakePnoMin5gRssi = -85;

constexpr uint32_t kFakeFrequency = 5260;
constexpr uint32_t kFakeFrequency1 = 2460;
constexpr uint32_t kFakeFrequency2 = 2500;

}  // namespace

class ScanSettingsTest : public ::testing::Test {
};

TEST_F(ScanSettingsTest, ChannelSettingsParcelableTest) {
  ChannelSettings channel_settings;
  channel_settings.frequency_ = kFakeFrequency;

  Parcel parcel;
  EXPECT_EQ(::android::OK, channel_settings.writeToParcel(&parcel));

  ChannelSettings channel_settings_copy;
  parcel.setDataPosition(0);
  EXPECT_EQ(::android::OK, channel_settings_copy.readFromParcel(&parcel));

  EXPECT_EQ(channel_settings, channel_settings_copy);
}

TEST_F(ScanSettingsTest, HiddenNetworkParcelableTest) {
  HiddenNetwork hidden_network;
  hidden_network.ssid_ =
      vector<uint8_t>(kFakeSsid, kFakeSsid + sizeof(kFakeSsid));

  Parcel parcel;
  EXPECT_EQ(::android::OK, hidden_network.writeToParcel(&parcel));

  HiddenNetwork hidden_network_copy;
  parcel.setDataPosition(0);
  EXPECT_EQ(::android::OK, hidden_network_copy.readFromParcel(&parcel));

  EXPECT_EQ(hidden_network, hidden_network_copy);
}

TEST_F(ScanSettingsTest, SingleScanSettingsParcelableTest) {
  SingleScanSettings scan_settings;

  ChannelSettings channel, channel1, channel2;
  channel.frequency_ = kFakeFrequency;
  channel1.frequency_ = kFakeFrequency1;
  channel2.frequency_ = kFakeFrequency2;
  scan_settings.scan_type_ = IWifiScannerImpl::SCAN_TYPE_LOW_SPAN;

  HiddenNetwork network;
  network.ssid_ =
      vector<uint8_t>(kFakeSsid, kFakeSsid + sizeof(kFakeSsid));

  scan_settings.channel_settings_ = {channel, channel1, channel2};
  scan_settings.hidden_networks_ = {network};

  Parcel parcel;
  EXPECT_EQ(::android::OK, scan_settings.writeToParcel(&parcel));

  SingleScanSettings scan_settings_copy;
  parcel.setDataPosition(0);
  EXPECT_EQ(::android::OK, scan_settings_copy.readFromParcel(&parcel));

  EXPECT_EQ(scan_settings, scan_settings_copy);
}

TEST_F(ScanSettingsTest, SingleScanSettingsParcelableWriteInvalidScanType) {
  SingleScanSettings scan_settings;

  ChannelSettings channel, channel1, channel2;
  channel.frequency_ = kFakeFrequency;
  channel1.frequency_ = kFakeFrequency1;
  channel2.frequency_ = kFakeFrequency2;
  scan_settings.scan_type_ = IWifiScannerImpl::SCAN_TYPE_DEFAULT;

  HiddenNetwork network;
  network.ssid_ =
      vector<uint8_t>(kFakeSsid, kFakeSsid + sizeof(kFakeSsid));

  scan_settings.channel_settings_ = {channel, channel1, channel2};
  scan_settings.hidden_networks_ = {network};

  Parcel parcel;
  EXPECT_EQ(::android::BAD_VALUE, scan_settings.writeToParcel(&parcel));
}


TEST_F(ScanSettingsTest, PnoNetworkParcelableTest) {
  PnoNetwork pno_network;
  pno_network.ssid_ =
      vector<uint8_t>(kFakeSsid, kFakeSsid + sizeof(kFakeSsid));
  pno_network.is_hidden_ = true;

  Parcel parcel;
  EXPECT_EQ(::android::OK, pno_network.writeToParcel(&parcel));

  PnoNetwork pno_network_copy;
  parcel.setDataPosition(0);
  EXPECT_EQ(::android::OK, pno_network_copy.readFromParcel(&parcel));

  EXPECT_EQ(pno_network, pno_network_copy);
}

TEST_F(ScanSettingsTest, PnoSettingsParcelableTest) {
  PnoSettings pno_settings;

  PnoNetwork network, network1;
  network.ssid_ =
      vector<uint8_t>(kFakeSsid, kFakeSsid + sizeof(kFakeSsid));
  network.is_hidden_ = true;
  network1.ssid_ =
      vector<uint8_t>(kFakeSsid1, kFakeSsid1 + sizeof(kFakeSsid1));
  network1.is_hidden_ = false;

  pno_settings.interval_ms_ = kFakePnoIntervalMs;
  pno_settings.min_2g_rssi_ = kFakePnoMin2gRssi;
  pno_settings.min_5g_rssi_ = kFakePnoMin5gRssi;

  pno_settings.pno_networks_ = {network, network1};

  Parcel parcel;
  EXPECT_EQ(::android::OK, pno_settings.writeToParcel(&parcel));

  PnoSettings pno_settings_copy;
  parcel.setDataPosition(0);
  EXPECT_EQ(::android::OK, pno_settings_copy.readFromParcel(&parcel));

  EXPECT_EQ(pno_settings, pno_settings_copy);
}



}  // namespace wificond
}  // namespace android
