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

#include <cstdint>

#include <gtest/gtest.h>

#include "netdutils/MockSyscalls.h"
#include "netdutils/Status.h"
#include "netdutils/Syscalls.h"

using testing::Mock;
using testing::Return;
using testing::StrictMock;

namespace android {
namespace netdutils {
namespace {

// Force implicit conversion from UniqueFd -> Fd
inline Fd toFd(const UniqueFd& fd) {
    return fd;
}

}  // namespace

TEST(Fd, smoke) {
    // Expect the following lines to compile
    Fd fd1(1);
    Fd fd2(fd1);
    Fd fd3 = fd2;
    const Fd fd4(8);
    const Fd fd5(fd4);
    const Fd fd6 = fd5;
    EXPECT_TRUE(isWellFormed(fd3));
    EXPECT_TRUE(isWellFormed(fd6));

    // Corner case
    Fd zero(0);
    EXPECT_TRUE(isWellFormed(zero));

    // Invalid file descriptors
    Fd bad(-1);
    Fd weird(-9);
    EXPECT_FALSE(isWellFormed(bad));
    EXPECT_FALSE(isWellFormed(weird));

    // Default constructor
    EXPECT_EQ(Fd(-1), Fd());
    std::stringstream ss;
    ss << fd3 << " " << fd6 << " " << bad << " " << weird;
    EXPECT_EQ("Fd[1] Fd[8] Fd[-1] Fd[-9]", ss.str());
}

class UniqueFdTest : public testing::Test {
  protected:
    StrictMock<ScopedMockSyscalls> mSyscalls;
};

TEST_F(UniqueFdTest, operatorOstream) {
    UniqueFd u(97);
    EXPECT_CALL(mSyscalls, close(toFd(u))).WillOnce(Return(status::ok));
    std::stringstream ss;
    ss << u;
    EXPECT_EQ("UniqueFd[Fd[97]]", ss.str());
    u.reset();
}

TEST_F(UniqueFdTest, destructor) {
    {
        UniqueFd u(98);
        EXPECT_CALL(mSyscalls, close(toFd(u))).WillOnce(Return(status::ok));
    }
    // Expectation above should be upon leaving nested scope
    Mock::VerifyAndClearExpectations(&mSyscalls);
}

TEST_F(UniqueFdTest, reset) {
    UniqueFd u(99);
    EXPECT_CALL(mSyscalls, close(toFd(u))).WillOnce(Return(status::ok));
    u.reset();

    // Expectation above should be upon reset
    Mock::VerifyAndClearExpectations(&mSyscalls);
}

TEST_F(UniqueFdTest, moveConstructor) {
    constexpr Fd kFd(101);
    UniqueFd u1(kFd);
    {
        UniqueFd u2(std::move(u1));
        EXPECT_FALSE(isWellFormed(u1));
        EXPECT_TRUE(isWellFormed(u2));
        EXPECT_CALL(mSyscalls, close(kFd)).WillOnce(Return(status::ok));
    }
    // Expectation above should be upon leaving nested scope
    Mock::VerifyAndClearExpectations(&mSyscalls);
}

TEST_F(UniqueFdTest, moveAssignment) {
    constexpr Fd kFd(102);
    UniqueFd u1(kFd);
    {
        UniqueFd u2 = std::move(u1);
        EXPECT_FALSE(isWellFormed(u1));
        EXPECT_TRUE(isWellFormed(u2));
        UniqueFd u3;
        u3 = std::move(u2);
        EXPECT_FALSE(isWellFormed(u2));
        EXPECT_TRUE(isWellFormed(u3));
        EXPECT_CALL(mSyscalls, close(kFd)).WillOnce(Return(status::ok));
    }
    // Expectation above should be upon leaving nested scope
    Mock::VerifyAndClearExpectations(&mSyscalls);
}

TEST_F(UniqueFdTest, constConstructor) {
    constexpr Fd kFd(103);
    const UniqueFd u(kFd);
    EXPECT_CALL(mSyscalls, close(toFd(u))).WillOnce(Return(status::ok));
}

TEST_F(UniqueFdTest, closeFailure) {
    constexpr Fd kFd(103);
    UniqueFd u(kFd);
    EXPECT_CALL(mSyscalls, close(toFd(u))).WillOnce(Return(statusFromErrno(EINTR, "test")));
    EXPECT_DEBUG_DEATH(u.reset(), "");
}

}  // namespace netdutils
}  // namespace android
