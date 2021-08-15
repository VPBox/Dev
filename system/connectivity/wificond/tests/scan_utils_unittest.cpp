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

#include <functional>
#include <memory>
#include <vector>

#include <linux/netlink.h>

#include <gtest/gtest.h>

#include "android/net/wifi/IWifiScannerImpl.h"
#include "wificond/net/kernel-header-latest/nl80211.h"
#include "wificond/scanning/scan_result.h"
#include "wificond/scanning/scan_utils.h"
#include "wificond/tests/mock_netlink_manager.h"

using std::bind;
using std::placeholders::_1;
using std::placeholders::_2;
using std::unique_ptr;
using std::vector;
using testing::AllOf;
using testing::Invoke;
using testing::NiceMock;
using testing::Not;
using testing::Return;
using testing::_;

using android::net::wifi::IWifiScannerImpl;
using com::android::server::wifi::wificond::NativeScanResult;

namespace android {
namespace wificond {

namespace {

constexpr uint32_t kFakeInterfaceIndex = 12;
constexpr uint32_t kFakeScheduledScanIntervalMs = 20000;
constexpr uint32_t kFakeSequenceNumber = 1984;
constexpr int kFakeErrorCode = EIO;
constexpr int32_t kFake2gRssiThreshold = -80;
constexpr int32_t kFake5gRssiThreshold = -77;
constexpr bool kFakeUseRandomMAC = true;
constexpr bool kFakeRequestLowPower = true;
constexpr bool kFakeRequestSchedScanRelativeRssi = true;
constexpr int kFakeScanType = IWifiScannerImpl::SCAN_TYPE_LOW_SPAN;

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

// This is a helper function to mock the behavior of NetlinkManager::
// SendMessageAndGetResponses() when we expect a single packet response.
// |request_message| and |response| are mapped to existing parameters of
// SendMessageAndGetResponses().
// |mock_response| and |mock_return value| are additional parameters used
// for specifying expected results,
bool AppendMessageAndReturn(
    NL80211Packet& mock_response,
    bool mock_return_value,
    const NL80211Packet& request_message,
    vector<std::unique_ptr<const NL80211Packet>>* response) {
  response->push_back(std::make_unique<NL80211Packet>(mock_response));
  return mock_return_value;
}

}  // namespace

class ScanUtilsTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    ON_CALL(netlink_manager_,
            SendMessageAndGetResponses(_, _)).WillByDefault(Return(true));
  }

  NiceMock<MockNetlinkManager> netlink_manager_;
  ScanUtils scan_utils_{&netlink_manager_};
};

MATCHER_P(DoesNL80211PacketMatchCommand, command,
          "Check if the netlink packet matches |command|") {
  return arg.GetCommand() == command;
}

MATCHER_P(DoesNL80211PacketHaveAttribute, attr,
          "Check if the netlink packet has atttribute |attr|") {
  return arg.HasAttribute(attr);
}

MATCHER_P2(DoesNL80211PacketHaveAttributeWithUint32Value, attr, expected_value,
           "Check if the netlink packet has atttribute |attr| with "
           "|expected_value|") {
  uint32_t actual_value;
  if (!arg.GetAttributeValue(attr, &actual_value)) {
    return false;
  }
  return actual_value == expected_value;
}

TEST_F(ScanUtilsTest, CanGetScanResult) {
  vector<NativeScanResult> scan_results;
  EXPECT_CALL(
      netlink_manager_,
      SendMessageAndGetResponses(
          DoesNL80211PacketMatchCommand(NL80211_CMD_GET_SCAN), _));

  // We don't use EXPECT_TRUE here because we need to mock a complete
  // response for NL80211_CMD_GET_SCAN to satisfy the parsing code called
  // by GetScanResult.
  // TODO(b/34231002): Mock response for NL80211_CMD_GET_SCAN.
  // TODO(b/34231420): Add validation of interface index.
  scan_utils_.GetScanResult(kFakeInterfaceIndex, &scan_results);
}

TEST_F(ScanUtilsTest, CanSendScanRequest) {
  NL80211Packet response = CreateControlMessageAck();
  EXPECT_CALL(
      netlink_manager_,
      SendMessageAndGetResponses(
          DoesNL80211PacketMatchCommand(NL80211_CMD_TRIGGER_SCAN), _)).
              WillOnce(Invoke(bind(
                  AppendMessageAndReturn, response, true, _1, _2)));
  int errno_ignored;
  EXPECT_TRUE(scan_utils_.Scan(kFakeInterfaceIndex, kFakeUseRandomMAC,
                               kFakeScanType, {}, {}, &errno_ignored));
  // TODO(b/34231420): Add validation of requested scan ssids, threshold,
  // and frequencies.
}

TEST_F(ScanUtilsTest, CanSendScanRequestWithRandomAddr) {
  NL80211Packet response = CreateControlMessageAck();
  EXPECT_CALL(
      netlink_manager_,
      SendMessageAndGetResponses(
           AllOf(
               DoesNL80211PacketMatchCommand(NL80211_CMD_TRIGGER_SCAN),
               DoesNL80211PacketHaveAttributeWithUint32Value(
                   NL80211_ATTR_SCAN_FLAGS, NL80211_SCAN_FLAG_RANDOM_ADDR)),
           _)).
      WillOnce(Invoke(bind(AppendMessageAndReturn, response, true, _1, _2)));

  int errno_ignored;
  EXPECT_TRUE(scan_utils_.Scan(kFakeInterfaceIndex, true,
                               IWifiScannerImpl::SCAN_TYPE_DEFAULT,
                               {}, {}, &errno_ignored));
}

TEST_F(ScanUtilsTest, CanSendScanRequestForLowSpanScan) {
  NL80211Packet response = CreateControlMessageAck();
  EXPECT_CALL(
      netlink_manager_,
      SendMessageAndGetResponses(
           AllOf(
               DoesNL80211PacketMatchCommand(NL80211_CMD_TRIGGER_SCAN),
               DoesNL80211PacketHaveAttributeWithUint32Value(
                   NL80211_ATTR_SCAN_FLAGS, NL80211_SCAN_FLAG_LOW_SPAN)),
           _)).
      WillOnce(Invoke(bind(AppendMessageAndReturn, response, true, _1, _2)));

  int errno_ignored;
  EXPECT_TRUE(scan_utils_.Scan(kFakeInterfaceIndex, false,
                               IWifiScannerImpl::SCAN_TYPE_LOW_SPAN,
                               {}, {}, &errno_ignored));
}

TEST_F(ScanUtilsTest, CanSendScanRequestForLowPowerScan) {
  NL80211Packet response = CreateControlMessageAck();
  EXPECT_CALL(
      netlink_manager_,
      SendMessageAndGetResponses(
           AllOf(
               DoesNL80211PacketMatchCommand(NL80211_CMD_TRIGGER_SCAN),
               DoesNL80211PacketHaveAttributeWithUint32Value(
                   NL80211_ATTR_SCAN_FLAGS, NL80211_SCAN_FLAG_LOW_POWER)),
           _)).
      WillOnce(Invoke(bind(AppendMessageAndReturn, response, true, _1, _2)));

  int errno_ignored;
  EXPECT_TRUE(scan_utils_.Scan(kFakeInterfaceIndex, false,
                               IWifiScannerImpl::SCAN_TYPE_LOW_POWER,
                               {}, {}, &errno_ignored));
}

TEST_F(ScanUtilsTest, CanSendScanRequestForHighAccuracyScan) {
  NL80211Packet response = CreateControlMessageAck();
  EXPECT_CALL(
      netlink_manager_,
      SendMessageAndGetResponses(
           AllOf(
               DoesNL80211PacketMatchCommand(NL80211_CMD_TRIGGER_SCAN),
               DoesNL80211PacketHaveAttributeWithUint32Value(
                   NL80211_ATTR_SCAN_FLAGS, NL80211_SCAN_FLAG_HIGH_ACCURACY)),
           _)).
      WillOnce(Invoke(bind(AppendMessageAndReturn, response, true, _1, _2)));

  int errno_ignored;
  EXPECT_TRUE(scan_utils_.Scan(kFakeInterfaceIndex, false,
                               IWifiScannerImpl::SCAN_TYPE_HIGH_ACCURACY,
                               {}, {}, &errno_ignored));
}

TEST_F(ScanUtilsTest, CanSendScanRequestForHighAccuracyScanWithRandomAddr) {
  NL80211Packet response = CreateControlMessageAck();
  EXPECT_CALL(
      netlink_manager_,
      SendMessageAndGetResponses(
           AllOf(
               DoesNL80211PacketMatchCommand(NL80211_CMD_TRIGGER_SCAN),
               DoesNL80211PacketHaveAttributeWithUint32Value(
                   NL80211_ATTR_SCAN_FLAGS,
                   static_cast<uint32_t>(NL80211_SCAN_FLAG_RANDOM_ADDR |
                                         NL80211_SCAN_FLAG_HIGH_ACCURACY))),
           _)).
      WillOnce(Invoke(bind(AppendMessageAndReturn, response, true, _1, _2)));

  int errno_ignored;
  EXPECT_TRUE(scan_utils_.Scan(kFakeInterfaceIndex, true,
                               IWifiScannerImpl::SCAN_TYPE_HIGH_ACCURACY,
                               {}, {}, &errno_ignored));
}

TEST_F(ScanUtilsTest, CanHandleScanRequestFailure) {
  NL80211Packet response = CreateControlMessageError(kFakeErrorCode);
  EXPECT_CALL(
      netlink_manager_,
      SendMessageAndGetResponses(
          DoesNL80211PacketMatchCommand(NL80211_CMD_TRIGGER_SCAN), _)).
              WillOnce(Invoke(bind(
                  AppendMessageAndReturn, response, true, _1, _2)));
  int error_code;
  EXPECT_FALSE(scan_utils_.Scan(kFakeInterfaceIndex, kFakeUseRandomMAC,
                               kFakeScanType, {}, {}, &error_code));
  EXPECT_EQ(kFakeErrorCode, error_code);
}

TEST_F(ScanUtilsTest, CanSendSchedScanRequest) {
  NL80211Packet response = CreateControlMessageAck();
  EXPECT_CALL(
      netlink_manager_,
       SendMessageAndGetResponses(
           DoesNL80211PacketMatchCommand(NL80211_CMD_START_SCHED_SCAN), _)).
              WillOnce(Invoke(bind(
                  AppendMessageAndReturn, response, true, _1, _2)));

  const SchedScanReqFlags req_flags = {
    kFakeUseRandomMAC, kFakeRequestLowPower, kFakeRequestSchedScanRelativeRssi
  };
  int errno_ignored;
  EXPECT_TRUE(scan_utils_.StartScheduledScan(
      kFakeInterfaceIndex,
      SchedScanIntervalSetting(),
      kFake2gRssiThreshold, kFake5gRssiThreshold, req_flags, {}, {}, {},
      &errno_ignored));
  // TODO(b/34231420): Add validation of requested scan ssids, threshold,
  // and frequencies.
}

TEST_F(ScanUtilsTest, CanHandleSchedScanRequestFailure) {
  NL80211Packet response = CreateControlMessageError(kFakeErrorCode);
  EXPECT_CALL(
      netlink_manager_,
       SendMessageAndGetResponses(
           DoesNL80211PacketMatchCommand(NL80211_CMD_START_SCHED_SCAN), _)).
              WillOnce(Invoke(bind(
                  AppendMessageAndReturn, response, true, _1, _2)));
  const SchedScanReqFlags req_flags = {
    kFakeUseRandomMAC, kFakeRequestLowPower, kFakeRequestSchedScanRelativeRssi
  };
  int error_code;
  EXPECT_FALSE(scan_utils_.StartScheduledScan(
      kFakeInterfaceIndex,
      SchedScanIntervalSetting(),
      kFake2gRssiThreshold, kFake5gRssiThreshold,
      req_flags, {}, {}, {}, &error_code));
  EXPECT_EQ(kFakeErrorCode, error_code);
}

TEST_F(ScanUtilsTest, CanSendSchedScanRequestForLowPowerScan) {
  NL80211Packet response = CreateControlMessageAck();
  EXPECT_CALL(
      netlink_manager_,
       SendMessageAndGetResponses(
           AllOf(
               DoesNL80211PacketMatchCommand(NL80211_CMD_START_SCHED_SCAN),
               DoesNL80211PacketHaveAttributeWithUint32Value(
                   NL80211_ATTR_SCAN_FLAGS, NL80211_SCAN_FLAG_LOW_POWER)),
           _));
  int errno_ignored;
  const SchedScanReqFlags req_flags = {
    false, true, false
  };
  scan_utils_.StartScheduledScan(
      kFakeInterfaceIndex,
      SchedScanIntervalSetting(),
      kFake2gRssiThreshold, kFake5gRssiThreshold,
      req_flags, {}, {}, {}, &errno_ignored);
}

TEST_F(ScanUtilsTest, CanSpecifyScanPlansForSchedScanRequest) {
  EXPECT_CALL(
      netlink_manager_,
       SendMessageAndGetResponses(
           AllOf(
               DoesNL80211PacketMatchCommand(NL80211_CMD_START_SCHED_SCAN),
               DoesNL80211PacketHaveAttribute(NL80211_ATTR_SCHED_SCAN_PLANS),
               Not(DoesNL80211PacketHaveAttribute(
                   NL80211_ATTR_SCHED_SCAN_INTERVAL))),
           _));
  int errno_ignored;
  SchedScanIntervalSetting interval_setting{
      {{kFakeScheduledScanIntervalMs, 10 /* repeated times */}},
      kFakeScheduledScanIntervalMs * 3 /* interval for infinite scans */};
  const SchedScanReqFlags req_flags = {
    kFakeUseRandomMAC, kFakeRequestLowPower, kFakeRequestSchedScanRelativeRssi
  };
  scan_utils_.StartScheduledScan(
      kFakeInterfaceIndex,
      interval_setting,
      kFake2gRssiThreshold, kFake5gRssiThreshold,
      req_flags, {}, {}, {}, &errno_ignored);
}

TEST_F(ScanUtilsTest, CanSpecifySingleIntervalForSchedScanRequest) {
  EXPECT_CALL(
      netlink_manager_,
       SendMessageAndGetResponses(
           AllOf(
               DoesNL80211PacketMatchCommand(NL80211_CMD_START_SCHED_SCAN),
               DoesNL80211PacketHaveAttribute(NL80211_ATTR_SCHED_SCAN_INTERVAL),
               Not(DoesNL80211PacketHaveAttribute(
                   NL80211_ATTR_SCHED_SCAN_PLANS))),
           _));
  int errno_ignored;
  SchedScanIntervalSetting interval_setting{{}, kFakeScheduledScanIntervalMs};
  const SchedScanReqFlags req_flags = {
    kFakeUseRandomMAC, kFakeRequestLowPower, kFakeRequestSchedScanRelativeRssi
  };
  scan_utils_.StartScheduledScan(
      kFakeInterfaceIndex,
      interval_setting,
      kFake2gRssiThreshold, kFake5gRssiThreshold,
      req_flags, {}, {}, {}, &errno_ignored);
}

TEST_F(ScanUtilsTest, CanPrioritizeLastSeenSinceBootNetlinkAttribute) {
  constexpr uint64_t kLastSeenTimestampNanoSeconds = 123456;
  constexpr uint64_t kBssTsfTimestampMicroSeconds = 654321;
  NL80211NestedAttr bss(NL80211_ATTR_BSS);
  bss.AddAttribute(
      NL80211Attr<uint64_t>(NL80211_BSS_LAST_SEEN_BOOTTIME,
                            kLastSeenTimestampNanoSeconds));
  bss.AddAttribute(
      NL80211Attr<uint64_t>(NL80211_BSS_TSF, kBssTsfTimestampMicroSeconds));
  uint64_t timestamp_microseconds;
  EXPECT_TRUE(scan_utils_.GetBssTimestampForTesting(
      bss, &timestamp_microseconds));
  EXPECT_EQ(kLastSeenTimestampNanoSeconds/1000, timestamp_microseconds);
}

TEST_F(ScanUtilsTest, CanHandleMissingLastSeenSinceBootNetlinkAttribute) {
  constexpr uint64_t kBssTsfTimestampMicroSeconds = 654321;
  NL80211NestedAttr bss(NL80211_ATTR_BSS);
  bss.AddAttribute(
      NL80211Attr<uint64_t>(NL80211_BSS_TSF, kBssTsfTimestampMicroSeconds));
  uint64_t timestamp_microseconds;
  EXPECT_TRUE(scan_utils_.GetBssTimestampForTesting(
      bss, &timestamp_microseconds));
  EXPECT_EQ(kBssTsfTimestampMicroSeconds, timestamp_microseconds);
}

// Probe TSF is newer.
TEST_F(ScanUtilsTest, CanPickMostRecentTimestampBetweenBetweenProbeAndBeacon1) {
  constexpr uint64_t kBssBeaconTsfTimestampMicroSeconds = 654321;
  constexpr uint64_t kBssTsfTimestampMicroSeconds =
      kBssBeaconTsfTimestampMicroSeconds + 2000;
  NL80211NestedAttr bss(NL80211_ATTR_BSS);
  bss.AddAttribute(
      NL80211Attr<uint64_t>(NL80211_BSS_BEACON_TSF,
                            kBssBeaconTsfTimestampMicroSeconds));
  bss.AddAttribute(
      NL80211Attr<uint64_t>(NL80211_BSS_TSF, kBssTsfTimestampMicroSeconds));
  uint64_t timestamp_microseconds;
  EXPECT_TRUE(scan_utils_.GetBssTimestampForTesting(
      bss, &timestamp_microseconds));
  EXPECT_EQ(kBssTsfTimestampMicroSeconds, timestamp_microseconds);
}

// Beacon TSF is newer.
TEST_F(ScanUtilsTest, CanPickMostRecentTimestampBetweenBetweenProbeAndBeacon2) {
  constexpr uint64_t kBssTsfTimestampMicroSeconds = 654321;
  constexpr uint64_t kBssBeaconTsfTimestampMicroSeconds =
      kBssTsfTimestampMicroSeconds + 2000;
  NL80211NestedAttr bss(NL80211_ATTR_BSS);
  bss.AddAttribute(
      NL80211Attr<uint64_t>(NL80211_BSS_BEACON_TSF,
                            kBssBeaconTsfTimestampMicroSeconds));
  bss.AddAttribute(
      NL80211Attr<uint64_t>(NL80211_BSS_TSF, kBssTsfTimestampMicroSeconds));
  uint64_t timestamp_microseconds;
  EXPECT_TRUE(scan_utils_.GetBssTimestampForTesting(
      bss, &timestamp_microseconds));
  EXPECT_EQ(kBssBeaconTsfTimestampMicroSeconds, timestamp_microseconds);
}

}  // namespace wificond
}  // namespace android
