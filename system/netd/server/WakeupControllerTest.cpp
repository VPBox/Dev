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

#include <linux/netfilter/nfnetlink_log.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "NetlinkManager.h"
#include "WakeupController.h"

using ::testing::StrictMock;
using ::testing::Test;
using ::testing::DoAll;
using ::testing::SaveArg;
using ::testing::Return;
using ::testing::_;

namespace android {
namespace net {

const uint32_t kDefaultPacketCopyRange = WakeupController::kDefaultPacketCopyRange;

using netdutils::status::ok;

class MockNetdEventListener {
  public:
    MOCK_METHOD10(onWakeupEvent,
                  void(const std::string& prefix, int uid, int ether, int ipNextHeader,
                       const std::vector<uint8_t>& dstHw, const std::string& srcIp,
                       const std::string& dstIp, int srcPort, int dstPort, uint64_t timestampNs));
};

class MockIptablesRestore : public IptablesRestoreInterface {
  public:
    ~MockIptablesRestore() override = default;
    MOCK_METHOD3(execute, int(const IptablesTarget target, const std::string& commands,
                              std::string* output));
};

class MockNFLogListener : public NFLogListenerInterface {
  public:
    ~MockNFLogListener() override = default;
    MOCK_METHOD2(subscribe, netdutils::Status(uint16_t nfLogGroup, const DispatchFn& fn));
    MOCK_METHOD3(subscribe,
            netdutils::Status(uint16_t nfLogGroup, uint32_t copyRange, const DispatchFn& fn));
    MOCK_METHOD1(unsubscribe, netdutils::Status(uint16_t nfLogGroup));
};

class WakeupControllerTest : public Test {
  protected:
    WakeupControllerTest() {
        EXPECT_CALL(mListener,
            subscribe(NetlinkManager::NFLOG_WAKEUP_GROUP, kDefaultPacketCopyRange, _))
            .WillOnce(DoAll(SaveArg<2>(&mMessageHandler), Return(ok)));
        EXPECT_CALL(mListener,
            unsubscribe(NetlinkManager::NFLOG_WAKEUP_GROUP)).WillOnce(Return(ok));
        EXPECT_OK(mController.init(&mListener));
    }

    StrictMock<MockNetdEventListener> mEventListener;
    StrictMock<MockIptablesRestore> mIptables;
    StrictMock<MockNFLogListener> mListener;
    WakeupController mController{
        [this](const WakeupController::ReportArgs& args) {
            mEventListener.onWakeupEvent(args.prefix, args.uid, args.ethertype, args.ipNextHeader,
                                         args.dstHw, args.srcIp, args.dstIp, args.srcPort,
                                         args.dstPort, args.timestampNs);
        },
        &mIptables};
    NFLogListenerInterface::DispatchFn mMessageHandler;
};

TEST_F(WakeupControllerTest, msgHandlerWithPartialAttributes) {
    const char kPrefix[] = "test:prefix";
    const uid_t kUid = 8734;
    const gid_t kGid = 2222;
    const uint64_t kNsPerS = 1000000000ULL;
    const uint64_t kTsNs = 9999 + (34 * kNsPerS);

    struct Msg {
        nlmsghdr nlmsg;
        nfgenmsg nfmsg;
        nlattr uidAttr;
        uid_t uid;
        nlattr gidAttr;
        gid_t gid;
        nlattr tsAttr;
        timespec ts;
        nlattr prefixAttr;
        char prefix[sizeof(kPrefix)];
    } msg = {};

    msg.uidAttr.nla_type = NFULA_UID;
    msg.uidAttr.nla_len = sizeof(msg.uidAttr) + sizeof(msg.uid);
    msg.uid = htonl(kUid);

    msg.gidAttr.nla_type = NFULA_GID;
    msg.gidAttr.nla_len = sizeof(msg.gidAttr) + sizeof(msg.gid);
    msg.gid = htonl(kGid);

    msg.tsAttr.nla_type = NFULA_TIMESTAMP;
    msg.tsAttr.nla_len = sizeof(msg.tsAttr) + sizeof(msg.ts);
    msg.ts.tv_sec = htonl(kTsNs / kNsPerS);
    msg.ts.tv_nsec = htonl(kTsNs % kNsPerS);

    msg.prefixAttr.nla_type = NFULA_PREFIX;
    msg.prefixAttr.nla_len = sizeof(msg.prefixAttr) + sizeof(msg.prefix);
    memcpy(msg.prefix, kPrefix, sizeof(kPrefix));

    auto payload = drop(netdutils::makeSlice(msg), offsetof(Msg, uidAttr));
    EXPECT_CALL(mEventListener,
            onWakeupEvent(kPrefix, kUid, -1, -1, std::vector<uint8_t>(), "", "", -1, -1, kTsNs));
    mMessageHandler(msg.nlmsg, msg.nfmsg, payload);
}

TEST_F(WakeupControllerTest, msgHandler) {
    const char kPrefix[] = "test:prefix";
    const uid_t kUid = 8734;
    const gid_t kGid = 2222;
    const std::vector<uint8_t> kMacAddr = {11, 22, 33, 44, 55, 66};
    const char* kSrcIpAddr = "192.168.2.1";
    const char* kDstIpAddr = "192.168.2.23";
    const uint16_t kEthertype = 0x800;
    const uint8_t kIpNextHeader = 6;
    const uint16_t kSrcPort = 1238;
    const uint16_t kDstPort = 4567;
    const uint64_t kNsPerS = 1000000000ULL;
    const uint64_t kTsNs = 9999 + (34 * kNsPerS);

    struct Msg {
        nlmsghdr nlmsg;
        nfgenmsg nfmsg;
        nlattr uidAttr;
        uid_t uid;
        nlattr gidAttr;
        gid_t gid;
        nlattr tsAttr;
        timespec ts;
        nlattr prefixAttr;
        char prefix[sizeof(kPrefix)];
        nlattr packetHeaderAttr;
        struct nfulnl_msg_packet_hdr packetHeader;
        nlattr hardwareAddrAttr;
        struct nfulnl_msg_packet_hw hardwareAddr;
        nlattr packetPayloadAttr;
        struct iphdr ipHeader;
        struct tcphdr tcpHeader;
    } msg = {};

    msg.prefixAttr.nla_type = NFULA_PREFIX;
    msg.prefixAttr.nla_len = sizeof(msg.prefixAttr) + sizeof(msg.prefix);
    memcpy(msg.prefix, kPrefix, sizeof(kPrefix));

    msg.uidAttr.nla_type = NFULA_UID;
    msg.uidAttr.nla_len = sizeof(msg.uidAttr) + sizeof(msg.uid);
    msg.uid = htonl(kUid);

    msg.gidAttr.nla_type = NFULA_GID;
    msg.gidAttr.nla_len = sizeof(msg.gidAttr) + sizeof(msg.gid);
    msg.gid = htonl(kGid);

    msg.tsAttr.nla_type = NFULA_TIMESTAMP;
    msg.tsAttr.nla_len = sizeof(msg.tsAttr) + sizeof(msg.ts);
    msg.ts.tv_sec = htonl(kTsNs / kNsPerS);
    msg.ts.tv_nsec = htonl(kTsNs % kNsPerS);

    msg.packetHeaderAttr.nla_type = NFULA_PACKET_HDR;
    msg.packetHeaderAttr.nla_len = sizeof(msg.packetHeaderAttr) + sizeof(msg.packetHeader);
    msg.packetHeader.hw_protocol = htons(kEthertype);

    msg.hardwareAddrAttr.nla_type = NFULA_HWADDR;
    msg.hardwareAddrAttr.nla_len = sizeof(msg.hardwareAddrAttr) + sizeof(msg.hardwareAddr);
    msg.hardwareAddr.hw_addrlen = htons(kMacAddr.size());
    std::copy(kMacAddr.begin(), kMacAddr.end(), msg.hardwareAddr.hw_addr);

    msg.packetPayloadAttr.nla_type = NFULA_PAYLOAD;
    msg.packetPayloadAttr.nla_len =
            sizeof(msg.packetPayloadAttr) + sizeof(msg.ipHeader) + sizeof(msg.tcpHeader);
    msg.ipHeader.protocol = IPPROTO_TCP;
    msg.ipHeader.ihl = sizeof(msg.ipHeader) / 4; // ipv4 IHL counts 32 bit words.
    inet_pton(AF_INET, kSrcIpAddr, &msg.ipHeader.saddr);
    inet_pton(AF_INET, kDstIpAddr, &msg.ipHeader.daddr);
    msg.tcpHeader.th_sport = htons(kSrcPort);
    msg.tcpHeader.th_dport = htons(kDstPort);

    auto payload = drop(netdutils::makeSlice(msg), offsetof(Msg, uidAttr));
    EXPECT_CALL(mEventListener, onWakeupEvent(kPrefix, kUid, kEthertype, kIpNextHeader, kMacAddr,
                                              kSrcIpAddr, kDstIpAddr, kSrcPort, kDstPort, kTsNs));
    mMessageHandler(msg.nlmsg, msg.nfmsg, payload);
}

TEST_F(WakeupControllerTest, badAttr) {
    const char kPrefix[] = "test:prefix";
    const uid_t kUid = 8734;
    const gid_t kGid = 2222;
    const uint64_t kNsPerS = 1000000000ULL;
    const uint64_t kTsNs = 9999 + (34 * kNsPerS);

    struct Msg {
        nlmsghdr nlmsg;
        nfgenmsg nfmsg;
        nlattr uidAttr;
        uid_t uid;
        nlattr invalid0;
        nlattr invalid1;
        nlattr gidAttr;
        gid_t gid;
        nlattr tsAttr;
        timespec ts;
        nlattr prefixAttr;
        char prefix[sizeof(kPrefix)];
    } msg = {};

    msg.uidAttr.nla_type = 999;
    msg.uidAttr.nla_len = sizeof(msg.uidAttr) + sizeof(msg.uid);
    msg.uid = htonl(kUid);

    msg.invalid0.nla_type = 0;
    msg.invalid0.nla_len = 0;
    msg.invalid1.nla_type = 0;
    msg.invalid1.nla_len = 1;

    msg.gidAttr.nla_type = NFULA_GID;
    msg.gidAttr.nla_len = sizeof(msg.gidAttr) + sizeof(msg.gid);
    msg.gid = htonl(kGid);

    msg.tsAttr.nla_type = NFULA_TIMESTAMP;
    msg.tsAttr.nla_len = sizeof(msg.tsAttr) - 2;
    msg.ts.tv_sec = htonl(kTsNs / kNsPerS);
    msg.ts.tv_nsec = htonl(kTsNs % kNsPerS);

    msg.prefixAttr.nla_type = NFULA_UID;
    msg.prefixAttr.nla_len = sizeof(msg.prefixAttr) + sizeof(msg.prefix);
    memcpy(msg.prefix, kPrefix, sizeof(kPrefix));

    auto payload = drop(netdutils::makeSlice(msg), offsetof(Msg, uidAttr));
    EXPECT_CALL(mEventListener,
            onWakeupEvent("", 1952805748, -1, -1, std::vector<uint8_t>(), "", "", -1, -1, 0));
    mMessageHandler(msg.nlmsg, msg.nfmsg, payload);
}

TEST_F(WakeupControllerTest, unterminatedString) {
    char ones[20] = {};
    memset(ones, 1, sizeof(ones));

    struct Msg {
        nlmsghdr nlmsg;
        nfgenmsg nfmsg;
        nlattr prefixAttr;
        char prefix[sizeof(ones)];
    } msg = {};

    msg.prefixAttr.nla_type = NFULA_PREFIX;
    msg.prefixAttr.nla_len = sizeof(msg.prefixAttr) + sizeof(msg.prefix);
    memcpy(msg.prefix, ones, sizeof(ones));

    const auto expected = std::string(ones, sizeof(ones) - 1);
    auto payload = drop(netdutils::makeSlice(msg), offsetof(Msg, prefixAttr));
    EXPECT_CALL(mEventListener,
            onWakeupEvent(expected, -1, -1, -1, std::vector<uint8_t>(), "", "", -1, -1, 0));
    mMessageHandler(msg.nlmsg, msg.nfmsg, payload);
}

TEST_F(WakeupControllerTest, addInterface) {
    const char kPrefix[] = "test:prefix";
    const char kIfName[] = "wlan8";
    const uint32_t kMark = 0x12345678;
    const uint32_t kMask = 0x0F0F0F0F;
    const char kExpected[] =
        "*mangle\n-A wakeupctrl_mangle_INPUT -i test:prefix"
        " -j NFLOG --nflog-prefix wlan8 --nflog-group 3 --nflog-threshold 8"
        " -m mark --mark 0x12345678/0x0f0f0f0f -m limit --limit 10/s\nCOMMIT\n";
    EXPECT_CALL(mIptables, execute(V4V6, kExpected, _)).WillOnce(Return(0));
    EXPECT_OK(mController.addInterface(kPrefix, kIfName, kMark, kMask));
}

TEST_F(WakeupControllerTest, delInterface) {
    const char kPrefix[] = "test:prefix";
    const char kIfName[] = "wlan8";
    const uint32_t kMark = 0x12345678;
    const uint32_t kMask = 0xF0F0F0F0;
    const char kExpected[] =
        "*mangle\n-D wakeupctrl_mangle_INPUT -i test:prefix"
        " -j NFLOG --nflog-prefix wlan8 --nflog-group 3 --nflog-threshold 8"
        " -m mark --mark 0x12345678/0xf0f0f0f0 -m limit --limit 10/s\nCOMMIT\n";
    EXPECT_CALL(mIptables, execute(V4V6, kExpected, _)).WillOnce(Return(0));
    EXPECT_OK(mController.delInterface(kPrefix, kIfName, kMark, kMask));
}

}  // namespace net
}  // namespace android
