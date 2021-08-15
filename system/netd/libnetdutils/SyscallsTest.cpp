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

#include <array>
#include <cstdint>
#include <memory>

#include <gtest/gtest.h>

#include "netdutils/Handle.h"
#include "netdutils/Math.h"
#include "netdutils/MockSyscalls.h"
#include "netdutils/Netfilter.h"
#include "netdutils/Netlink.h"
#include "netdutils/Slice.h"
#include "netdutils/Status.h"
#include "netdutils/StatusOr.h"
#include "netdutils/Syscalls.h"

using testing::_;
using testing::ByMove;
using testing::Invoke;
using testing::Return;
using testing::StrictMock;

namespace android {
namespace netdutils {

class SyscallsTest : public testing::Test {
  protected:
    StrictMock<ScopedMockSyscalls> mSyscalls;
};

TEST(syscalls, scopedMock) {
    auto& old = sSyscalls.get();
    {
        StrictMock<ScopedMockSyscalls> s;
        EXPECT_EQ(&s, &sSyscalls.get());
    }
    EXPECT_EQ(&old, &sSyscalls.get());
}

TEST_F(SyscallsTest, open) {
    const char kPath[] = "/test/path/please/ignore";
    constexpr Fd kFd(40);
    constexpr int kFlags = 883;
    constexpr mode_t kMode = 37373;
    const auto& sys = sSyscalls.get();
    EXPECT_CALL(mSyscalls, open(kPath, kFlags, kMode)).WillOnce(Return(ByMove(UniqueFd(kFd))));
    EXPECT_CALL(mSyscalls, close(kFd)).WillOnce(Return(status::ok));
    auto result = sys.open(kPath, kFlags, kMode);
    EXPECT_EQ(status::ok, result.status());
    EXPECT_EQ(kFd, result.value());
}

TEST_F(SyscallsTest, getsockname) {
    constexpr Fd kFd(40);
    sockaddr_nl expected = {};
    auto& sys = sSyscalls.get();

    // Success
    EXPECT_CALL(mSyscalls, getsockname(kFd, _, _))
        .WillOnce(Invoke([expected](Fd, sockaddr* addr, socklen_t* addrlen) {
            memcpy(addr, &expected, sizeof(expected));
            EXPECT_EQ(*addrlen, static_cast<socklen_t>(sizeof(expected)));
            return status::ok;
        }));
    const auto result = sys.getsockname<sockaddr_nl>(kFd);
    EXPECT_TRUE(isOk(result));
    EXPECT_EQ(expected, result.value());

    // Failure
    const Status kError = statusFromErrno(EINVAL, "test");
    EXPECT_CALL(mSyscalls, getsockname(kFd, _, _)).WillOnce(Return(kError));
    EXPECT_EQ(kError, sys.getsockname<sockaddr_nl>(kFd).status());
}

TEST_F(SyscallsTest, setsockopt) {
    constexpr Fd kFd(40);
    constexpr int kLevel = 50;
    constexpr int kOptname = 70;
    sockaddr_nl expected = {};
    auto& sys = sSyscalls.get();

    // Success
    EXPECT_CALL(mSyscalls, setsockopt(kFd, kLevel, kOptname, &expected, sizeof(expected)))
        .WillOnce(Return(status::ok));
    EXPECT_EQ(status::ok, sys.setsockopt(kFd, kLevel, kOptname, expected));

    // Failure
    const Status kError = statusFromErrno(EINVAL, "test");
    EXPECT_CALL(mSyscalls, setsockopt(kFd, kLevel, kOptname, &expected, sizeof(expected)))
        .WillOnce(Return(kError));
    EXPECT_EQ(kError, sys.setsockopt(kFd, kLevel, kOptname, expected));
}

TEST_F(SyscallsTest, getsockopt) {
    constexpr Fd kFd(40);
    constexpr int kLevel = 50;
    constexpr int kOptname = 70;
    sockaddr_nl expected = {};
    socklen_t optLen = 0;
    auto& sys = sSyscalls.get();

    // Success
    EXPECT_CALL(mSyscalls, getsockopt(kFd, kLevel, kOptname, &expected, &optLen))
        .WillOnce(Return(status::ok));
    EXPECT_EQ(status::ok, sys.getsockopt(kFd, kLevel, kOptname, &expected, &optLen));

    // Failure
    const Status kError = statusFromErrno(EINVAL, "test");
    EXPECT_CALL(mSyscalls, getsockopt(kFd, kLevel, kOptname, &expected, &optLen))
        .WillOnce(Return(kError));
    EXPECT_EQ(kError, sys.getsockopt(kFd, kLevel, kOptname, &expected, &optLen));
}

TEST_F(SyscallsTest, bind) {
    constexpr Fd kFd(40);
    sockaddr_nl expected = {};
    auto& sys = sSyscalls.get();

    // Success
    EXPECT_CALL(mSyscalls, bind(kFd, asSockaddrPtr(&expected), sizeof(expected)))
        .WillOnce(Return(status::ok));
    EXPECT_EQ(status::ok, sys.bind(kFd, expected));

    // Failure
    const Status kError = statusFromErrno(EINVAL, "test");
    EXPECT_CALL(mSyscalls, bind(kFd, asSockaddrPtr(&expected), sizeof(expected)))
        .WillOnce(Return(kError));
    EXPECT_EQ(kError, sys.bind(kFd, expected));
}

TEST_F(SyscallsTest, connect) {
    constexpr Fd kFd(40);
    sockaddr_nl expected = {};
    auto& sys = sSyscalls.get();

    // Success
    EXPECT_CALL(mSyscalls, connect(kFd, asSockaddrPtr(&expected), sizeof(expected)))
        .WillOnce(Return(status::ok));
    EXPECT_EQ(status::ok, sys.connect(kFd, expected));

    // Failure
    const Status kError = statusFromErrno(EINVAL, "test");
    EXPECT_CALL(mSyscalls, connect(kFd, asSockaddrPtr(&expected), sizeof(expected)))
        .WillOnce(Return(kError));
    EXPECT_EQ(kError, sys.connect(kFd, expected));
}

TEST_F(SyscallsTest, sendto) {
    constexpr Fd kFd(40);
    constexpr int kFlags = 0;
    std::array<char, 10> payload;
    const auto slice = makeSlice(payload);
    sockaddr_nl expected = {};
    auto& sys = sSyscalls.get();

    // Success
    EXPECT_CALL(mSyscalls, sendto(kFd, slice, kFlags, asSockaddrPtr(&expected), sizeof(expected)))
        .WillOnce(Return(slice.size()));
    EXPECT_EQ(status::ok, sys.sendto(kFd, slice, kFlags, expected));
}

TEST_F(SyscallsTest, recvfrom) {
    constexpr Fd kFd(40);
    constexpr int kFlags = 0;
    std::array<char, 10> payload;
    const auto dst = makeSlice(payload);
    const auto used = take(dst, 8);
    sockaddr_nl expected = {};
    auto& sys = sSyscalls.get();

    // Success
    EXPECT_CALL(mSyscalls, recvfrom(kFd, dst, kFlags, _, _))
            .WillOnce(Invoke(
                    [expected, used](Fd, const Slice, int, sockaddr* src, socklen_t* srclen) {
                        *srclen = sizeof(expected);
                        memcpy(src, &expected, *srclen);
                        return used;
                    }));
    auto result = sys.recvfrom<sockaddr_nl>(kFd, dst, kFlags);
    EXPECT_EQ(status::ok, result.status());
    EXPECT_EQ(used, result.value().first);
    EXPECT_EQ(expected, result.value().second);
}

}  // namespace netdutils
}  // namespace android
