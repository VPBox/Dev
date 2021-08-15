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
#include <memory>
#include <string>
#include <vector>

#include <linux/netlink.h>

#include <gtest/gtest.h>

#include "wificond/net/kernel-header-latest/nl80211.h"
#include "wificond/net/netlink_utils.h"
#include "wificond/tests/mock_netlink_manager.h"

using std::string;
using std::unique_ptr;
using std::vector;
using testing::DoAll;
using testing::NiceMock;
using testing::Return;
using testing::_;

namespace android {
namespace wificond {

namespace {

constexpr uint8_t kFakeMaxNumScanSSIDs = 10;
constexpr uint8_t kFakeMaxNumSchedScanSSIDs = 16;
constexpr uint8_t kFakeMaxMatchSets = 18;
constexpr uint8_t kFakeMaxNumScanPlans = 8;
constexpr uint8_t kFakeMaxScanPlanIntervals = 80;
constexpr uint8_t kFakeMaxScanPlanIterations = 10;
constexpr uint16_t kFakeFamilyId = 14;
constexpr uint32_t kFakeFrequency1 = 2412;
constexpr uint32_t kFakeFrequency2 = 2437;
constexpr uint32_t kFakeFrequency3 = 2484;
constexpr uint32_t kFakeFrequency4 = 5200;
constexpr uint32_t kFakeFrequency5 = 5400;
constexpr uint32_t kFakeFrequency6 = 5600;
// 802.11p channel which is not valid for wifi usage.
constexpr uint32_t kFakeInvalidFrequency = 5950;
constexpr uint32_t kFakeSequenceNumber = 162;
constexpr uint32_t kFakeProtocolFeatures = 0x02;
constexpr uint16_t kFakeWiphyIndex = 8;
constexpr uint16_t kFakeWiphyIndex1 = 10;
constexpr uint64_t kFakeCookie = 42;
constexpr int kFakeErrorCode = EIO;
constexpr int32_t kFakeMcs = 5;
constexpr bool kFakeSupportsRandomMacOneshotScan = true;
constexpr bool kFakeSupportsRandomMacSchedScan = false;
const char kFakeInterfaceName[] = "testif0";
const char kFakeCountryCode[] = "US";
const uint32_t kFakeInterfaceIndex = 34;
const uint32_t kFakeInterfaceIndex1 = 36;
const std::array<uint8_t, ETH_ALEN> kFakeInterfaceMacAddress = {0x45, 0x54, 0xad, 0x67, 0x98, 0xf6};
const std::array<uint8_t, ETH_ALEN> kFakeInterfaceMacAddress1 = {0x05, 0x04, 0xef, 0x27, 0x12, 0xff};
const uint8_t kFakeExtFeaturesForLowSpanScan[] = {0x0, 0x0, 0x40};
const uint8_t kFakeExtFeaturesForLowPowerScan[] = {0x0, 0x0, 0x80};
const uint8_t kFakeExtFeaturesForHighAccuracy[] = {0x0, 0x0, 0x0, 0x1};
const uint8_t kFakeExtFeaturesForAllScanType[] = {0x0, 0x0, 0xC0, 0x1};
const uint8_t kFakeFrame[] = {0x00, 0x01, 0x02, 0x03};


// Currently, control messages are only created by the kernel and sent to us.
// Therefore NL80211Packet doesn't have corresponding constructor.
// For test we manually create control messages using this helper function.
NL80211Packet CreateControlMessageError(int error_code) {
  vector<uint8_t> data;
  data.resize(NLMSG_HDRLEN + NLA_ALIGN(sizeof(int)), 0);
  // Initialize length field.
  nlmsghdr* nl_header = reinterpret_cast<nlmsghdr*>(data.data());
  nl_header->nlmsg_len = data.size();
  nl_header->nlmsg_type = NLMSG_ERROR;
  nl_header->nlmsg_seq = kFakeSequenceNumber;
  nl_header->nlmsg_pid = getpid();
  int* error_field = reinterpret_cast<int*>(data.data() + NLMSG_HDRLEN);
  *error_field = -error_code;

  return NL80211Packet(data);
}

NL80211Packet CreateControlMessageAck() {
  return CreateControlMessageError(0);
}

void AppendScanCapabilitiesAttributes(NL80211Packet* packet,
                                      bool supports_scan_plan) {
  packet->AddAttribute(NL80211Attr<uint8_t>(NL80211_ATTR_MAX_NUM_SCAN_SSIDS,
                                            kFakeMaxNumScanSSIDs));
  packet->AddAttribute(NL80211Attr<uint8_t>(
      NL80211_ATTR_MAX_NUM_SCHED_SCAN_SSIDS,
      kFakeMaxNumSchedScanSSIDs));
  packet->AddAttribute(NL80211Attr<uint8_t>(NL80211_ATTR_MAX_MATCH_SETS,
                                            kFakeMaxMatchSets));
  if (supports_scan_plan) {
    packet->AddAttribute(NL80211Attr<uint32_t>(
        NL80211_ATTR_MAX_NUM_SCHED_SCAN_PLANS,
        kFakeMaxNumScanPlans));
    packet->AddAttribute(NL80211Attr<uint32_t>(
        NL80211_ATTR_MAX_SCAN_PLAN_INTERVAL,
        kFakeMaxScanPlanIntervals));
    packet->AddAttribute(NL80211Attr<uint32_t>(
        NL80211_ATTR_MAX_SCAN_PLAN_ITERATIONS,
        kFakeMaxScanPlanIterations));
  }
}

NL80211NestedAttr GenerateBandsAttributeFor2g() {
  NL80211NestedAttr freq_2g_1(1);
  NL80211NestedAttr freq_2g_2(2);
  NL80211NestedAttr freq_2g_3(3);
  freq_2g_1.AddAttribute(NL80211Attr<uint32_t>(NL80211_FREQUENCY_ATTR_FREQ,
                                               kFakeFrequency1));
  freq_2g_2.AddAttribute(NL80211Attr<uint32_t>(NL80211_FREQUENCY_ATTR_FREQ,
                                               kFakeFrequency2));
  freq_2g_3.AddAttribute(NL80211Attr<uint32_t>(NL80211_FREQUENCY_ATTR_FREQ,
                                               kFakeFrequency3));

  NL80211NestedAttr band_2g_freqs(NL80211_BAND_ATTR_FREQS);
  band_2g_freqs.AddAttribute(freq_2g_1);
  band_2g_freqs.AddAttribute(freq_2g_2);
  band_2g_freqs.AddAttribute(freq_2g_3);

  NL80211NestedAttr band_2g_attr(1);
  band_2g_attr.AddAttribute(band_2g_freqs);

  NL80211NestedAttr band_attr(NL80211_ATTR_WIPHY_BANDS);
  band_attr.AddAttribute(band_2g_attr);
  return band_attr;
}

NL80211NestedAttr GenerateBandsAttributeFor5gAndDfs() {
  NL80211NestedAttr freq_5g_1(4);
  NL80211NestedAttr freq_5g_2(5);
  NL80211NestedAttr freq_5g_3(6);
  NL80211NestedAttr freq_dfs_1(7);
  freq_5g_1.AddAttribute(NL80211Attr<uint32_t>(NL80211_FREQUENCY_ATTR_FREQ,
                                               kFakeFrequency4));
  freq_5g_2.AddAttribute(NL80211Attr<uint32_t>(NL80211_FREQUENCY_ATTR_FREQ,
                                               kFakeFrequency5));
  // This channel is passive only.
  freq_5g_2.AddFlagAttribute(NL80211_FREQUENCY_ATTR_NO_IR);

  // This channel is not valid for wifi usage.
  // We should not include it in the parse result.
  freq_5g_3.AddAttribute(NL80211Attr<uint32_t>(NL80211_FREQUENCY_ATTR_FREQ,
                                               kFakeInvalidFrequency));

  // DFS frequency.
  freq_dfs_1.AddAttribute(NL80211Attr<uint32_t>(NL80211_FREQUENCY_ATTR_FREQ,
                                                kFakeFrequency6));
  freq_dfs_1.AddAttribute(NL80211Attr<uint32_t>(
      NL80211_FREQUENCY_ATTR_DFS_STATE,
      NL80211_DFS_USABLE));

  NL80211NestedAttr band_5g_freqs(NL80211_BAND_ATTR_FREQS);
  band_5g_freqs.AddAttribute(freq_5g_1);
  band_5g_freqs.AddAttribute(freq_5g_2);
  band_5g_freqs.AddAttribute(freq_5g_3);
  band_5g_freqs.AddAttribute(freq_dfs_1);

  NL80211NestedAttr band_5g_attr(1);
  band_5g_attr.AddAttribute(band_5g_freqs);

  NL80211NestedAttr band_attr(NL80211_ATTR_WIPHY_BANDS);
  band_attr.AddAttribute(band_5g_attr);
  return band_attr;
}

void AppendBandInfoAttributes(NL80211Packet* packet) {
  NL80211NestedAttr attr_2g = GenerateBandsAttributeFor2g();
  NL80211NestedAttr attr_5g_and_dfs = GenerateBandsAttributeFor5gAndDfs();
  attr_2g.Merge(attr_5g_and_dfs);
  packet->AddAttribute(attr_2g);
}

void AppendWiphyFeaturesAttributes(NL80211Packet* packet) {
  uint32_t wiphy_features = 0;
  if (kFakeSupportsRandomMacOneshotScan) {
      wiphy_features |= NL80211_FEATURE_SCAN_RANDOM_MAC_ADDR;
  }
  if (kFakeSupportsRandomMacSchedScan) {
      wiphy_features |= NL80211_FEATURE_SCHED_SCAN_RANDOM_MAC_ADDR;
  }
  packet->AddAttribute(
      NL80211Attr<uint32_t>(NL80211_ATTR_FEATURE_FLAGS, wiphy_features));
}

void AppendWiphyExtFeaturesAttributes(NL80211Packet* packet,
                                      bool support_low_span,
                                      bool support_low_power,
                                      bool support_high_accuracy,
                                      bool support_all) {
  ASSERT_LE(support_low_span + support_low_power + support_high_accuracy, 1);
  std::vector<uint8_t> ext_feature_flags_bytes;
  if (support_low_span) {
    ext_feature_flags_bytes =
        std::vector<uint8_t>(kFakeExtFeaturesForLowSpanScan,
                             kFakeExtFeaturesForLowSpanScan +
                             sizeof(kFakeExtFeaturesForLowSpanScan));
  } else if (support_low_power) {
    ext_feature_flags_bytes =
        std::vector<uint8_t>(kFakeExtFeaturesForLowPowerScan,
                             kFakeExtFeaturesForLowPowerScan +
                             sizeof(kFakeExtFeaturesForLowPowerScan));
  } else if (support_high_accuracy) {
    ext_feature_flags_bytes =
        std::vector<uint8_t>(kFakeExtFeaturesForHighAccuracy,
                             kFakeExtFeaturesForHighAccuracy +
                             sizeof(kFakeExtFeaturesForHighAccuracy));
  } else if (support_all) {
    ext_feature_flags_bytes =
        std::vector<uint8_t>(kFakeExtFeaturesForAllScanType,
                             kFakeExtFeaturesForAllScanType +
                             sizeof(kFakeExtFeaturesForAllScanType));
  }
  packet->AddAttribute(
      NL80211Attr<std::vector<uint8_t>>(NL80211_ATTR_EXT_FEATURES,
                                        ext_feature_flags_bytes));
}

void VerifyScanCapabilities(const ScanCapabilities& scan_capabilities,
                            bool supports_scan_plan) {
  EXPECT_EQ(scan_capabilities.max_num_scan_ssids,
            kFakeMaxNumScanSSIDs);
  EXPECT_EQ(scan_capabilities.max_num_sched_scan_ssids,
            kFakeMaxNumSchedScanSSIDs);
  EXPECT_EQ(scan_capabilities.max_match_sets,
            kFakeMaxMatchSets);
  if (supports_scan_plan) {
    EXPECT_EQ(scan_capabilities.max_num_scan_plans,
              kFakeMaxNumScanPlans);
    EXPECT_EQ(scan_capabilities.max_scan_plan_interval,
              kFakeMaxScanPlanIntervals);
    EXPECT_EQ(scan_capabilities.max_scan_plan_iterations,
              kFakeMaxScanPlanIterations);
  } else {
    EXPECT_EQ(scan_capabilities.max_num_scan_plans, (unsigned int) 0);
    EXPECT_EQ(scan_capabilities.max_scan_plan_interval, (unsigned int) 0);
    EXPECT_EQ(scan_capabilities.max_scan_plan_iterations, (unsigned int) 0);
  }
}

void VerifyBandInfo(const BandInfo& band_info) {
  vector<uint32_t> band_2g_expected = {kFakeFrequency1,
      kFakeFrequency2, kFakeFrequency3};
  vector<uint32_t> band_5g_expected = {kFakeFrequency4};
  // Frequency5 is doesn't belong to a DFS channel. However, our convetion
  // requires us to return any passive only channels in DFS band.
  vector<uint32_t> band_dfs_expected = {kFakeFrequency5, kFakeFrequency6};
  EXPECT_EQ(band_info.band_2g, band_2g_expected);
  EXPECT_EQ(band_info.band_5g, band_5g_expected);
  EXPECT_EQ(band_info.band_dfs, band_dfs_expected);
}

void VerifyWiphyFeatures(const WiphyFeatures& wiphy_features) {
  EXPECT_TRUE(wiphy_features.supports_random_mac_oneshot_scan);
  EXPECT_FALSE(wiphy_features.supports_random_mac_sched_scan);
}

}  // namespace

// This mocks the behavior of SendMessageAndGetResponses(), which returns a
// vector of NL80211Packet using passed in pointer.
ACTION_P(MakeupResponse, response) {
  // arg1 is the second parameter: vector<unique_ptr<const NL80211Packet>>* responses.
  for (auto& pkt : response) {
    arg1->push_back(unique_ptr<NL80211Packet>(new NL80211Packet(pkt)));
  }
}

class NetlinkUtilsTest : public ::testing::Test {
 protected:
  std::unique_ptr<NiceMock<MockNetlinkManager>> netlink_manager_;
  std::unique_ptr<NetlinkUtils> netlink_utils_;

  virtual void SetUp() {
    netlink_manager_.reset(new NiceMock<MockNetlinkManager>());
    netlink_utils_.reset(new NetlinkUtils(netlink_manager_.get()));

    ON_CALL(*netlink_manager_,
            GetSequenceNumber()).WillByDefault(Return(kFakeSequenceNumber));
    ON_CALL(*netlink_manager_,
            GetFamilyId()).WillByDefault(Return(kFakeFamilyId));
  }

  void SetSplitWiphyDumpSupported(bool supported) {
    netlink_utils_->supports_split_wiphy_dump_ = supported;
  }

};

TEST_F(NetlinkUtilsTest, CanGetWiphyIndex) {
  NL80211Packet new_wiphy(
      netlink_manager_->GetFamilyId(),
      NL80211_CMD_NEW_WIPHY,
      netlink_manager_->GetSequenceNumber(),
      getpid());
  // Insert wiphy_index attribute.
  NL80211Attr<uint32_t> wiphy_index_attr(NL80211_ATTR_WIPHY, kFakeWiphyIndex);
  new_wiphy.AddAttribute(wiphy_index_attr);
  // Mock a valid response from kernel.
  vector<NL80211Packet> response = {new_wiphy};

  EXPECT_CALL(*netlink_manager_, SendMessageAndGetResponses(_, _)).
      WillOnce(DoAll(MakeupResponse(response), Return(true)));

  uint32_t wiphy_index;
  EXPECT_TRUE(netlink_utils_->GetWiphyIndex(&wiphy_index));
  EXPECT_EQ(kFakeWiphyIndex, wiphy_index);
}

TEST_F(NetlinkUtilsTest, CanHandleGetWiphyIndexError) {
  // Mock an error response from kernel.
  vector<NL80211Packet> response = {CreateControlMessageError(kFakeErrorCode)};

  EXPECT_CALL(*netlink_manager_, SendMessageAndGetResponses(_, _)).
      WillOnce(DoAll(MakeupResponse(response), Return(true)));

  uint32_t wiphy_index;
  EXPECT_FALSE(netlink_utils_->GetWiphyIndex(&wiphy_index));
}

TEST_F(NetlinkUtilsTest, CanSetIntrerfaceMode) {
  // Mock a ACK response from kernel.
  vector<NL80211Packet> response = {CreateControlMessageAck()};

  EXPECT_CALL(*netlink_manager_, SendMessageAndGetResponses(_, _)).
      WillOnce(DoAll(MakeupResponse(response), Return(true)));

  EXPECT_TRUE(netlink_utils_->SetInterfaceMode(kFakeInterfaceIndex,
                                               NetlinkUtils::STATION_MODE));
}

TEST_F(NetlinkUtilsTest, CanHandleSetIntrerfaceModeError) {
  // Mock an error response from kernel.
  vector<NL80211Packet> response = {CreateControlMessageError(kFakeErrorCode)};

  EXPECT_CALL(*netlink_manager_, SendMessageAndGetResponses(_, _)).
      WillOnce(DoAll(MakeupResponse(response), Return(true)));

  EXPECT_FALSE(netlink_utils_->SetInterfaceMode(kFakeInterfaceIndex,
                                                NetlinkUtils::STATION_MODE));
}

TEST_F(NetlinkUtilsTest, CanGetInterfaces) {
  NL80211Packet new_interface(
      netlink_manager_->GetFamilyId(),
      NL80211_CMD_NEW_INTERFACE,
      netlink_manager_->GetSequenceNumber(),
      getpid());
  // Insert interface name attribute.
  NL80211Attr<string> if_name_attr(NL80211_ATTR_IFNAME, string(kFakeInterfaceName));
  new_interface.AddAttribute(if_name_attr);
  // Insert interface index attribute.
  NL80211Attr<uint32_t> if_index_attr(NL80211_ATTR_IFINDEX, kFakeInterfaceIndex);
  new_interface.AddAttribute(if_index_attr);
  // Insert mac address attribute.
  std::array<uint8_t, ETH_ALEN> if_mac_addr = kFakeInterfaceMacAddress;
  NL80211Attr<std::array<uint8_t, ETH_ALEN>> if_mac_attr(NL80211_ATTR_MAC, if_mac_addr);
  new_interface.AddAttribute(if_mac_attr);

  // Mock a valid response from kernel.
  vector<NL80211Packet> response = {new_interface};

  EXPECT_CALL(*netlink_manager_, SendMessageAndGetResponses(_, _)).
      WillOnce(DoAll(MakeupResponse(response), Return(true)));

  vector<InterfaceInfo> interfaces;
  EXPECT_TRUE(netlink_utils_->GetInterfaces(kFakeWiphyIndex, &interfaces));
  EXPECT_TRUE(interfaces.size() == 1);
  EXPECT_EQ(kFakeInterfaceIndex, interfaces[0].index);
  EXPECT_EQ(string(kFakeInterfaceName), interfaces[0].name);
  EXPECT_EQ(if_mac_addr, interfaces[0].mac_address);
}

TEST_F(NetlinkUtilsTest, SkipsPseudoDevicesWhenGetInterfaces) {
  // This might be a psuedo p2p interface without any interface index/name
  // attributes.
  NL80211Packet psuedo_interface(
      netlink_manager_->GetFamilyId(),
      NL80211_CMD_NEW_INTERFACE,
      netlink_manager_->GetSequenceNumber(),
      getpid());
  psuedo_interface.AddAttribute(NL80211Attr<uint64_t>(
      NL80211_ATTR_WDEV, 0));

  // This is a regular client interface.
  NL80211Packet expected_interface(
      netlink_manager_->GetFamilyId(),
      NL80211_CMD_NEW_INTERFACE,
      netlink_manager_->GetSequenceNumber(),
      getpid());
  expected_interface.AddAttribute(NL80211Attr<string>(
      NL80211_ATTR_IFNAME, string(kFakeInterfaceName)));
  expected_interface.AddAttribute(NL80211Attr<uint32_t>(
      NL80211_ATTR_IFINDEX, kFakeInterfaceIndex));
  std::array<uint8_t, ETH_ALEN> if_mac_addr = kFakeInterfaceMacAddress;
  expected_interface.AddAttribute(
      NL80211Attr<std::array<uint8_t, ETH_ALEN>>(NL80211_ATTR_MAC, if_mac_addr));

  // Kernel can send us the pseduo interface packet first
  vector<NL80211Packet> response = {psuedo_interface, expected_interface};

  EXPECT_CALL(*netlink_manager_, SendMessageAndGetResponses(_, _)).
      WillOnce(DoAll(MakeupResponse(response), Return(true)));

  vector<InterfaceInfo> interfaces;
  EXPECT_TRUE(netlink_utils_->GetInterfaces(kFakeWiphyIndex, &interfaces));
  EXPECT_TRUE(interfaces.size() == 1);
  EXPECT_EQ(kFakeInterfaceIndex, interfaces[0].index);
  EXPECT_EQ(string(kFakeInterfaceName), interfaces[0].name);
  EXPECT_EQ(if_mac_addr, interfaces[0].mac_address);
}

TEST_F(NetlinkUtilsTest, HandleP2p0WhenGetInterfaces) {
  NL80211Packet new_interface(
      netlink_manager_->GetFamilyId(),
      NL80211_CMD_NEW_INTERFACE,
      netlink_manager_->GetSequenceNumber(),
      getpid());
  // Insert interface name attribute.
  NL80211Attr<string> if_name_attr(NL80211_ATTR_IFNAME, string(kFakeInterfaceName));
  new_interface.AddAttribute(if_name_attr);
  // Insert interface index attribute.
  new_interface.AddAttribute(
      NL80211Attr<uint32_t>(NL80211_ATTR_IFINDEX, kFakeInterfaceIndex));
  // Insert mac address attribute.
  std::array<uint8_t, ETH_ALEN> if_mac_addr = kFakeInterfaceMacAddress;
  new_interface.AddAttribute(
      NL80211Attr<std::array<uint8_t, ETH_ALEN>>(NL80211_ATTR_MAC, if_mac_addr));

  // Create a new interface packet for p2p0.
  NL80211Packet new_interface_p2p0(
      netlink_manager_->GetFamilyId(),
      NL80211_CMD_NEW_INTERFACE,
      netlink_manager_->GetSequenceNumber(),
      getpid());

  // Insert interface name attribute.
  new_interface_p2p0.AddAttribute(
      NL80211Attr<string>(NL80211_ATTR_IFNAME, "p2p0"));
  // Insert interface index attribute.
  new_interface_p2p0.AddAttribute(
      NL80211Attr<uint32_t>(NL80211_ATTR_IFINDEX, kFakeInterfaceIndex1));
  // Insert mac address attribute.
  std::array<uint8_t, ETH_ALEN> if_mac_addr_p2p = kFakeInterfaceMacAddress1;
  new_interface_p2p0.AddAttribute(
      NL80211Attr<std::array<uint8_t, ETH_ALEN>>(NL80211_ATTR_MAC, if_mac_addr_p2p));

  // Mock response from kernel, including 2 interfaces.
  vector<NL80211Packet> response = {new_interface_p2p0, new_interface};

  EXPECT_CALL(*netlink_manager_, SendMessageAndGetResponses(_, _)).
      WillOnce(DoAll(MakeupResponse(response), Return(true)));

  vector<InterfaceInfo> interfaces;
  EXPECT_TRUE(netlink_utils_->GetInterfaces(kFakeWiphyIndex, &interfaces));
  EXPECT_TRUE(interfaces.size() == 2);

  EXPECT_EQ(kFakeInterfaceIndex1, interfaces[0].index);
  EXPECT_EQ(string("p2p0"), interfaces[0].name);
  EXPECT_EQ(if_mac_addr_p2p, interfaces[0].mac_address);

  EXPECT_EQ(kFakeInterfaceIndex, interfaces[1].index);
  EXPECT_EQ(string(kFakeInterfaceName), interfaces[1].name);
  EXPECT_EQ(if_mac_addr, interfaces[1].mac_address);
}

TEST_F(NetlinkUtilsTest, CanHandleGetInterfacesError) {
  // Mock an error response from kernel.
  vector<NL80211Packet> response = {CreateControlMessageError(kFakeErrorCode)};

  EXPECT_CALL(*netlink_manager_, SendMessageAndGetResponses(_, _)).
      WillOnce(DoAll(MakeupResponse(response), Return(true)));

  vector<InterfaceInfo> interfaces;
  EXPECT_FALSE(netlink_utils_->GetInterfaces(kFakeWiphyIndex, &interfaces));
}

TEST_F(NetlinkUtilsTest, CanGetWiphyInfo) {
  SetSplitWiphyDumpSupported(false);
  NL80211Packet new_wiphy(
      netlink_manager_->GetFamilyId(),
      NL80211_CMD_NEW_WIPHY,
      netlink_manager_->GetSequenceNumber(),
      getpid());
  new_wiphy.AddAttribute(NL80211Attr<uint32_t>(NL80211_ATTR_WIPHY,
                                               kFakeWiphyIndex));
  AppendBandInfoAttributes(&new_wiphy);
  AppendScanCapabilitiesAttributes(&new_wiphy, true);
  AppendWiphyFeaturesAttributes(&new_wiphy);
  vector<NL80211Packet> get_wiphy_response = {new_wiphy};

  EXPECT_CALL(*netlink_manager_, SendMessageAndGetResponses(_, _)).
      WillOnce(DoAll(MakeupResponse(get_wiphy_response), Return(true)));

  BandInfo band_info;
  ScanCapabilities scan_capabilities;
  WiphyFeatures wiphy_features;
  EXPECT_TRUE(netlink_utils_->GetWiphyInfo(kFakeWiphyIndex,
                                           &band_info,
                                           &scan_capabilities,
                                           &wiphy_features));
  VerifyBandInfo(band_info);
  VerifyScanCapabilities(scan_capabilities, true);
  VerifyWiphyFeatures(wiphy_features);
  EXPECT_FALSE(wiphy_features.supports_low_span_oneshot_scan);
  EXPECT_FALSE(wiphy_features.supports_low_power_oneshot_scan);
  EXPECT_FALSE(wiphy_features.supports_high_accuracy_oneshot_scan);
}

TEST_F(NetlinkUtilsTest, CanGetWiphyInfoWithNoDbsParam) {
  SetSplitWiphyDumpSupported(false);
  NL80211Packet new_wiphy(
      netlink_manager_->GetFamilyId(),
      NL80211_CMD_NEW_WIPHY,
      netlink_manager_->GetSequenceNumber(),
      getpid());
  new_wiphy.AddAttribute(NL80211Attr<uint32_t>(NL80211_ATTR_WIPHY,
                                               kFakeWiphyIndex));
  AppendBandInfoAttributes(&new_wiphy);
  AppendScanCapabilitiesAttributes(&new_wiphy, false);
  AppendWiphyFeaturesAttributes(&new_wiphy);
  AppendWiphyExtFeaturesAttributes(&new_wiphy, false, false, false, false);
  vector<NL80211Packet> get_wiphy_response = {new_wiphy};

  EXPECT_CALL(*netlink_manager_, SendMessageAndGetResponses(_, _)).
      WillOnce(DoAll(MakeupResponse(get_wiphy_response), Return(true)));

  BandInfo band_info;
  ScanCapabilities scan_capabilities;
  WiphyFeatures wiphy_features;
  EXPECT_TRUE(netlink_utils_->GetWiphyInfo(kFakeWiphyIndex,
                                           &band_info,
                                           &scan_capabilities,
                                           &wiphy_features));
  VerifyBandInfo(band_info);
  VerifyScanCapabilities(scan_capabilities, false);
  VerifyWiphyFeatures(wiphy_features);
  EXPECT_FALSE(wiphy_features.supports_low_span_oneshot_scan);
  EXPECT_FALSE(wiphy_features.supports_low_power_oneshot_scan);
  EXPECT_FALSE(wiphy_features.supports_high_accuracy_oneshot_scan);
}

TEST_F(NetlinkUtilsTest, CanGetWiphyInfoWithLowSpanScan) {
  SetSplitWiphyDumpSupported(false);
  NL80211Packet new_wiphy(
      netlink_manager_->GetFamilyId(),
      NL80211_CMD_NEW_WIPHY,
      netlink_manager_->GetSequenceNumber(),
      getpid());
  new_wiphy.AddAttribute(NL80211Attr<uint32_t>(NL80211_ATTR_WIPHY,
                                               kFakeWiphyIndex));
  AppendBandInfoAttributes(&new_wiphy);
  AppendScanCapabilitiesAttributes(&new_wiphy, false);
  AppendWiphyFeaturesAttributes(&new_wiphy);
  AppendWiphyExtFeaturesAttributes(&new_wiphy, true, false, false, false);
  vector<NL80211Packet> get_wiphy_response = {new_wiphy};

  EXPECT_CALL(*netlink_manager_, SendMessageAndGetResponses(_, _)).
      WillOnce(DoAll(MakeupResponse(get_wiphy_response), Return(true)));

  BandInfo band_info;
  ScanCapabilities scan_capabilities;
  WiphyFeatures wiphy_features;
  EXPECT_TRUE(netlink_utils_->GetWiphyInfo(kFakeWiphyIndex,
                                           &band_info,
                                           &scan_capabilities,
                                           &wiphy_features));
  VerifyBandInfo(band_info);
  VerifyScanCapabilities(scan_capabilities, false);
  VerifyWiphyFeatures(wiphy_features);
  EXPECT_TRUE(wiphy_features.supports_low_span_oneshot_scan);
  EXPECT_FALSE(wiphy_features.supports_low_power_oneshot_scan);
  EXPECT_FALSE(wiphy_features.supports_high_accuracy_oneshot_scan);
}

TEST_F(NetlinkUtilsTest, CanGetWiphyInfoWithLowPowerScan) {
  SetSplitWiphyDumpSupported(false);
  NL80211Packet new_wiphy(
      netlink_manager_->GetFamilyId(),
      NL80211_CMD_NEW_WIPHY,
      netlink_manager_->GetSequenceNumber(),
      getpid());
  new_wiphy.AddAttribute(NL80211Attr<uint32_t>(NL80211_ATTR_WIPHY,
                                               kFakeWiphyIndex));
  AppendBandInfoAttributes(&new_wiphy);
  AppendScanCapabilitiesAttributes(&new_wiphy, false);
  AppendWiphyFeaturesAttributes(&new_wiphy);
  AppendWiphyExtFeaturesAttributes(&new_wiphy, false, true, false, false);
  vector<NL80211Packet> get_wiphy_response = {new_wiphy};

  EXPECT_CALL(*netlink_manager_, SendMessageAndGetResponses(_, _)).
      WillOnce(DoAll(MakeupResponse(get_wiphy_response), Return(true)));

  BandInfo band_info;
  ScanCapabilities scan_capabilities;
  WiphyFeatures wiphy_features;
  EXPECT_TRUE(netlink_utils_->GetWiphyInfo(kFakeWiphyIndex,
                                           &band_info,
                                           &scan_capabilities,
                                           &wiphy_features));
  VerifyBandInfo(band_info);
  VerifyScanCapabilities(scan_capabilities, false);
  VerifyWiphyFeatures(wiphy_features);
  EXPECT_FALSE(wiphy_features.supports_low_span_oneshot_scan);
  EXPECT_TRUE(wiphy_features.supports_low_power_oneshot_scan);
  EXPECT_FALSE(wiphy_features.supports_high_accuracy_oneshot_scan);
}

TEST_F(NetlinkUtilsTest, CanGetWiphyInfoWithHighAccuracyScan) {
  SetSplitWiphyDumpSupported(false);
  NL80211Packet new_wiphy(
      netlink_manager_->GetFamilyId(),
      NL80211_CMD_NEW_WIPHY,
      netlink_manager_->GetSequenceNumber(),
      getpid());
  new_wiphy.AddAttribute(NL80211Attr<uint32_t>(NL80211_ATTR_WIPHY,
                                               kFakeWiphyIndex));
  AppendBandInfoAttributes(&new_wiphy);
  AppendScanCapabilitiesAttributes(&new_wiphy, false);
  AppendWiphyFeaturesAttributes(&new_wiphy);
  AppendWiphyExtFeaturesAttributes(&new_wiphy, false, false, true, false);
  vector<NL80211Packet> get_wiphy_response = {new_wiphy};

  EXPECT_CALL(*netlink_manager_, SendMessageAndGetResponses(_, _)).
      WillOnce(DoAll(MakeupResponse(get_wiphy_response), Return(true)));

  BandInfo band_info;
  ScanCapabilities scan_capabilities;
  WiphyFeatures wiphy_features;
  EXPECT_TRUE(netlink_utils_->GetWiphyInfo(kFakeWiphyIndex,
                                           &band_info,
                                           &scan_capabilities,
                                           &wiphy_features));
  VerifyBandInfo(band_info);
  VerifyScanCapabilities(scan_capabilities, false);
  VerifyWiphyFeatures(wiphy_features);
  EXPECT_FALSE(wiphy_features.supports_low_span_oneshot_scan);
  EXPECT_FALSE(wiphy_features.supports_low_power_oneshot_scan);
  EXPECT_TRUE(wiphy_features.supports_high_accuracy_oneshot_scan);
}

TEST_F(NetlinkUtilsTest, CanGetWiphyInfoWithAllDbsParams) {
  SetSplitWiphyDumpSupported(false);
  NL80211Packet new_wiphy(
      netlink_manager_->GetFamilyId(),
      NL80211_CMD_NEW_WIPHY,
      netlink_manager_->GetSequenceNumber(),
      getpid());
  new_wiphy.AddAttribute(NL80211Attr<uint32_t>(NL80211_ATTR_WIPHY,
                                               kFakeWiphyIndex));
  AppendBandInfoAttributes(&new_wiphy);
  AppendScanCapabilitiesAttributes(&new_wiphy, false);
  AppendWiphyFeaturesAttributes(&new_wiphy);
  AppendWiphyExtFeaturesAttributes(&new_wiphy, false, false, false, true);
  vector<NL80211Packet> get_wiphy_response = {new_wiphy};

  EXPECT_CALL(*netlink_manager_, SendMessageAndGetResponses(_, _)).
      WillOnce(DoAll(MakeupResponse(get_wiphy_response), Return(true)));

  BandInfo band_info;
  ScanCapabilities scan_capabilities;
  WiphyFeatures wiphy_features;
  EXPECT_TRUE(netlink_utils_->GetWiphyInfo(kFakeWiphyIndex,
                                           &band_info,
                                           &scan_capabilities,
                                           &wiphy_features));
  VerifyBandInfo(band_info);
  VerifyScanCapabilities(scan_capabilities, false);
  VerifyWiphyFeatures(wiphy_features);
  EXPECT_TRUE(wiphy_features.supports_low_span_oneshot_scan);
  EXPECT_TRUE(wiphy_features.supports_low_power_oneshot_scan);
  EXPECT_TRUE(wiphy_features.supports_high_accuracy_oneshot_scan);
}

TEST_F(NetlinkUtilsTest, CanGetWiphyInfoScanPlanNotSupported) {
  SetSplitWiphyDumpSupported(false);
  NL80211Packet new_wiphy(
      netlink_manager_->GetFamilyId(),
      NL80211_CMD_NEW_WIPHY,
      netlink_manager_->GetSequenceNumber(),
      getpid());
  new_wiphy.AddAttribute(NL80211Attr<uint32_t>(NL80211_ATTR_WIPHY,
                                               kFakeWiphyIndex));
  AppendBandInfoAttributes(&new_wiphy);
  AppendScanCapabilitiesAttributes(&new_wiphy, false);
  AppendWiphyFeaturesAttributes(&new_wiphy);
  vector<NL80211Packet> get_wiphy_response = {new_wiphy};

  EXPECT_CALL(*netlink_manager_, SendMessageAndGetResponses(_, _)).
      WillOnce(DoAll(MakeupResponse(get_wiphy_response), Return(true)));

  BandInfo band_info;
  ScanCapabilities scan_capabilities;
  WiphyFeatures wiphy_features;
  EXPECT_TRUE(netlink_utils_->GetWiphyInfo(kFakeWiphyIndex,
                                           &band_info,
                                           &scan_capabilities,
                                           &wiphy_features));
  VerifyBandInfo(band_info);
  VerifyScanCapabilities(scan_capabilities, false);
  VerifyWiphyFeatures(wiphy_features);
}

TEST_F(NetlinkUtilsTest, CanGetWiphyInfoSplitDump) {
  SetSplitWiphyDumpSupported(true);

  NL80211Packet new_wiphy_packet1(
      netlink_manager_->GetFamilyId(),
      NL80211_CMD_NEW_WIPHY,
      netlink_manager_->GetSequenceNumber(),
      getpid());
  new_wiphy_packet1.AddAttribute(NL80211Attr<uint32_t>(NL80211_ATTR_WIPHY,
                                                       kFakeWiphyIndex));
  new_wiphy_packet1.AddAttribute(GenerateBandsAttributeFor5gAndDfs());

  NL80211Packet new_wiphy_packet2(
      netlink_manager_->GetFamilyId(),
      NL80211_CMD_NEW_WIPHY,
      netlink_manager_->GetSequenceNumber(),
      getpid());
  new_wiphy_packet2.AddAttribute(NL80211Attr<uint32_t>(NL80211_ATTR_WIPHY,
                                                       kFakeWiphyIndex));
  new_wiphy_packet2.AddAttribute(GenerateBandsAttributeFor2g());
  AppendScanCapabilitiesAttributes(&new_wiphy_packet2, false);
  AppendWiphyFeaturesAttributes(&new_wiphy_packet2);

  // Insert a packet for wiphy with index kFakeWiphyIndex1.
  // This is unrelated and should be ingnored by |GetWiphyInfo|.
  NL80211Packet new_wiphy_packet3(
      netlink_manager_->GetFamilyId(),
      NL80211_CMD_NEW_WIPHY,
      netlink_manager_->GetSequenceNumber(),
      getpid());
  new_wiphy_packet3.AddAttribute(NL80211Attr<uint32_t>(NL80211_ATTR_WIPHY,
                                                       kFakeWiphyIndex1));
  AppendBandInfoAttributes(&new_wiphy_packet3);

  vector<NL80211Packet> get_wiphy_response =
      {new_wiphy_packet1, new_wiphy_packet2, new_wiphy_packet3};

  EXPECT_CALL(*netlink_manager_, SendMessageAndGetResponses(_, _)).
      WillOnce(DoAll(MakeupResponse(get_wiphy_response), Return(true)));

  BandInfo band_info;
  ScanCapabilities scan_capabilities;
  WiphyFeatures wiphy_features;
  EXPECT_TRUE(netlink_utils_->GetWiphyInfo(kFakeWiphyIndex,
                                           &band_info,
                                           &scan_capabilities,
                                           &wiphy_features));
  VerifyBandInfo(band_info);
  VerifyScanCapabilities(scan_capabilities, false);
  VerifyWiphyFeatures(wiphy_features);
}


TEST_F(NetlinkUtilsTest, CanHandleGetWiphyInfoError) {
  SetSplitWiphyDumpSupported(false);

  // Mock an error response from kernel.
  vector<NL80211Packet> get_wiphy_response = {CreateControlMessageError(kFakeErrorCode)};

  EXPECT_CALL(*netlink_manager_, SendMessageAndGetResponses(_, _)).
      WillOnce(DoAll(MakeupResponse(get_wiphy_response), Return(true)));

  BandInfo band_info;
  ScanCapabilities scan_capabilities;
  WiphyFeatures wiphy_features;
  EXPECT_FALSE(netlink_utils_->GetWiphyInfo(kFakeWiphyIndex,
                                           &band_info,
                                           &scan_capabilities,
                                           &wiphy_features));
}

TEST_F(NetlinkUtilsTest, CanGetProtocolFeatures) {
  // There is no specification for the response packet id for
  // NL80211_CMD_GET_PROTOCOL_FEATURES.
  // Still use NL80211_CMD_GET_PROTOCOL_FEATURES here.
  NL80211Packet get_features_response(
      netlink_manager_->GetFamilyId(),
      NL80211_CMD_GET_PROTOCOL_FEATURES,
      netlink_manager_->GetSequenceNumber(),
      getpid());
  get_features_response.AddAttribute(
      NL80211Attr<uint32_t>(NL80211_ATTR_PROTOCOL_FEATURES,
                            kFakeProtocolFeatures));
  vector<NL80211Packet> response = {get_features_response};

  EXPECT_CALL(*netlink_manager_, SendMessageAndGetResponses(_, _)).
      WillOnce(DoAll(MakeupResponse(response), Return(true)));

  uint32_t features;
  EXPECT_TRUE(netlink_utils_->GetProtocolFeatures(&features));
  EXPECT_EQ(kFakeProtocolFeatures, features);
}

TEST_F(NetlinkUtilsTest, CanHandleGetProtocolFeaturesError) {
  // Mock an error response from kernel.
  vector<NL80211Packet> response = {CreateControlMessageError(kFakeErrorCode)};

  EXPECT_CALL(*netlink_manager_, SendMessageAndGetResponses(_, _)).
      WillOnce(DoAll(MakeupResponse(response), Return(true)));

  uint32_t features_ignored;
  EXPECT_FALSE(netlink_utils_->GetProtocolFeatures(&features_ignored));
}

TEST_F(NetlinkUtilsTest, CanGetCountryCode) {
  // There is no specification for the response packet id for
  // NL80211_CMD_GET_REG.
  // Still use NL80211_CMD_GET_REG here.
  NL80211Packet get_country_code_response(
      netlink_manager_->GetFamilyId(),
      NL80211_CMD_GET_REG,
      netlink_manager_->GetSequenceNumber(),
      getpid());
  get_country_code_response.AddAttribute(
      NL80211Attr<string>(NL80211_ATTR_REG_ALPHA2,
                          kFakeCountryCode));
  vector<NL80211Packet> response = {get_country_code_response};

  EXPECT_CALL(*netlink_manager_, SendMessageAndGetResponses(_, _)).
      WillOnce(DoAll(MakeupResponse(response), Return(true)));

  string country_code;
  EXPECT_TRUE(netlink_utils_->GetCountryCode(&country_code));
  EXPECT_EQ(kFakeCountryCode, country_code);
}

TEST_F(NetlinkUtilsTest, CanHandleGetCountryCodeError) {
  // Mock an error response from kernel.
  vector<NL80211Packet> response = {CreateControlMessageError(kFakeErrorCode)};

  EXPECT_CALL(*netlink_manager_, SendMessageAndGetResponses(_, _)).
      WillOnce(DoAll(MakeupResponse(response), Return(true)));

  string country_code_ignored;
  EXPECT_FALSE(netlink_utils_->GetCountryCode(&country_code_ignored));
}

TEST_F(NetlinkUtilsTest, CanSendMgmtFrame) {
  // There is no specification for the response packet id for
  // NL80211_CMD_FRAME.
  // Still use NL80211_CMD_FRAME here.
  NL80211Packet send_mgmt_frame_response(
      netlink_manager_->GetFamilyId(),
      NL80211_CMD_FRAME,
      netlink_manager_->GetSequenceNumber(),
      getpid());
  send_mgmt_frame_response.AddAttribute(
      NL80211Attr<uint64_t>(NL80211_ATTR_COOKIE, kFakeCookie));
  vector<NL80211Packet> response = {send_mgmt_frame_response};

  EXPECT_CALL(*netlink_manager_, SendMessageAndGetResponses(_, _))
    .WillOnce(DoAll(MakeupResponse(response), Return(true)));

  uint64_t cookie;
  EXPECT_TRUE(netlink_utils_->SendMgmtFrame(kFakeInterfaceIndex,
      vector<uint8_t>(std::begin(kFakeFrame), std::end(kFakeFrame)),
      kFakeMcs, &cookie));
  EXPECT_EQ(kFakeCookie, cookie);
}

TEST_F(NetlinkUtilsTest, CanHandleSendMgmtFrameError) {
  // Mock an error response from kernel.
  vector<NL80211Packet> response = {CreateControlMessageError(kFakeErrorCode)};
  EXPECT_CALL(*netlink_manager_, SendMessageAndGetResponses(_, _)).
      WillOnce(DoAll(MakeupResponse(response), Return(true)));

  uint64_t cookie_ignored;
  EXPECT_FALSE(netlink_utils_->SendMgmtFrame(kFakeInterfaceIndex,
      vector<uint8_t>(std::begin(kFakeFrame), std::end(kFakeFrame)),
      kFakeMcs, &cookie_ignored));
}

}  // namespace wificond
}  // namespace android
