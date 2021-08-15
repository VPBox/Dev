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

#include <atomic>
#include <deque>
#include <iostream>
#include <mutex>

#include <arpa/inet.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <linux/netfilter/nfnetlink_log.h>

#include <netdutils/MockSyscalls.h>
#include "NFLogListener.h"

using ::testing::_;
using ::testing::DoAll;
using ::testing::Exactly;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::SaveArg;
using ::testing::StrictMock;

namespace android {
namespace net {

using netdutils::Slice;
using netdutils::StatusOr;
using netdutils::makeSlice;
using netdutils::status::ok;

constexpr int kNFLogPacketMsgType = (NFNL_SUBSYS_ULOG << 8) | NFULNL_MSG_PACKET;
constexpr int kNetlinkMsgDoneType = (NFNL_SUBSYS_NONE << 8) | NLMSG_DONE;

class MockNetlinkListener : public NetlinkListenerInterface {
  public:
    ~MockNetlinkListener() override = default;

    MOCK_METHOD1(send, netdutils::Status(const Slice msg));
    MOCK_METHOD2(subscribe, netdutils::Status(uint16_t type, const DispatchFn& fn));
    MOCK_METHOD1(unsubscribe, netdutils::Status(uint16_t type));
    MOCK_METHOD0(join, void());
    MOCK_METHOD1(registerSkErrorHandler, void(const SkErrorHandler& handler));
};

class NFLogListenerTest : public testing::Test {
  protected:
    NFLogListenerTest() {
        EXPECT_CALL(*mNLListener, subscribe(kNFLogPacketMsgType, _))
            .WillOnce(DoAll(SaveArg<1>(&mPacketFn), Return(ok)));
        EXPECT_CALL(*mNLListener, subscribe(kNetlinkMsgDoneType, _))
            .WillOnce(DoAll(SaveArg<1>(&mDoneFn), Return(ok)));
        mListener.reset(new NFLogListener(mNLListener));
    }

    ~NFLogListenerTest() {
        EXPECT_CALL(*mNLListener, unsubscribe(kNFLogPacketMsgType)).WillOnce(Return(ok));
        EXPECT_CALL(*mNLListener, unsubscribe(kNetlinkMsgDoneType)).WillOnce(Return(ok));
    }

    static StatusOr<size_t> sendOk(const Slice buf) { return buf.size(); }

    void subscribe(uint16_t type, const NFLogListenerInterface::DispatchFn& fn) {
        // Two sends for cfgCmdBind() & cfgMode(), one send at destruction time for cfgCmdUnbind()
        EXPECT_CALL(*mNLListener, send(_)).Times(Exactly(3)).WillRepeatedly(Invoke(sendOk));
        EXPECT_OK(mListener->subscribe(type, fn));
    }

    void sendEmptyMsg(uint16_t type) {
        struct {
            nlmsghdr nlmsg;
            nfgenmsg nfmsg;
        } msg = {};

        msg.nlmsg.nlmsg_type = kNFLogPacketMsgType;
        msg.nlmsg.nlmsg_len = sizeof(msg);
        msg.nfmsg.res_id = htons(type);
        mPacketFn(msg.nlmsg, drop(makeSlice(msg), sizeof(msg.nlmsg)));
    }

    NetlinkListenerInterface::DispatchFn mPacketFn;
    NetlinkListenerInterface::DispatchFn mDoneFn;
    std::shared_ptr<StrictMock<MockNetlinkListener>> mNLListener{
        new StrictMock<MockNetlinkListener>()};
    std::unique_ptr<NFLogListener> mListener;
};

TEST_F(NFLogListenerTest, subscribe) {
    constexpr uint16_t kType = 38;
    const auto dispatchFn = [](const nlmsghdr&, const nfgenmsg&, const Slice) {};
    subscribe(kType, dispatchFn);
}

TEST_F(NFLogListenerTest, nlmsgDone) {
    constexpr uint16_t kType = 38;
    const auto dispatchFn = [](const nlmsghdr&, const nfgenmsg&, const Slice) {};
    subscribe(kType, dispatchFn);
    mDoneFn({}, {});
}

TEST_F(NFLogListenerTest, dispatchOk) {
    int invocations = 0;
    constexpr uint16_t kType = 38;
    const auto dispatchFn = [&invocations, kType](const nlmsghdr&, const nfgenmsg& nfmsg,
                                                  const Slice) {
        EXPECT_EQ(kType, ntohs(nfmsg.res_id));
        ++invocations;
    };
    subscribe(kType, dispatchFn);
    sendEmptyMsg(kType);
    EXPECT_EQ(1, invocations);
}

TEST_F(NFLogListenerTest, dispatchUnknownType) {
    constexpr uint16_t kType = 38;
    constexpr uint16_t kBadType = kType + 1;
    const auto dispatchFn = [](const nlmsghdr&, const nfgenmsg&, const Slice) {
        // Expect no invocations
        ASSERT_TRUE(false);
    };
    subscribe(kType, dispatchFn);
    sendEmptyMsg(kBadType);
}

}  // namespace net
}  // namespace android
