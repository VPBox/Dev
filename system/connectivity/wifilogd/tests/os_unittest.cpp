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
#include <iostream>
#include <memory>
#include <tuple>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "wifilogd/local_utils.h"
#include "wifilogd/os.h"
#include "wifilogd/tests/mock_raw_os.h"

// This function must be defined in the same namespace as |timespec|. Hence the
// placement of this function at the top level.
inline void PrintTo(const timespec& ts, ::std::ostream* os) {
  *os << "[secs:" << ts.tv_sec << " "
      << "nsecs:" << ts.tv_nsec << "]";
}

namespace android {
namespace wifilogd {
namespace {

using ::testing::_;
using ::testing::Invoke;
using ::testing::InSequence;
using ::testing::Matcher;
using ::testing::MatcherInterface;
using ::testing::MatchResultListener;
using ::testing::NotNull;
using ::testing::Pointee;
using ::testing::Return;
using ::testing::SetArgumentPointee;
using ::testing::SetErrnoAndReturn;
using ::testing::StrictMock;
using ::testing::StrEq;

using local_utils::GetMaxVal;

class OsTest : public ::testing::Test {
 public:
  OsTest() {
    raw_os_ = new StrictMock<MockRawOs>();
    os_ = std::unique_ptr<Os>(new Os(std::unique_ptr<RawOs>(raw_os_)));
  }

 protected:
  std::unique_ptr<Os> os_;
  // We use a raw pointer to access the mock, since ownership passes
  // to |os_|.
  MockRawOs* raw_os_;
};

class TimespecMatcher : public MatcherInterface<const timespec&> {
 public:
  explicit TimespecMatcher(const timespec& expected) : expected_(expected) {}

  virtual void DescribeTo(::std::ostream* os) const {
    *os << "equals ";
    PrintTo(expected_, os);
  }

  virtual bool MatchAndExplain(const timespec& actual,
                               MatchResultListener* /* listener */) const {
    return actual.tv_sec == expected_.tv_sec &&
           actual.tv_nsec == expected_.tv_nsec;
  }

 private:
  const timespec& expected_;
};

Matcher<const timespec&> EqualsTimespec(const timespec& expected) {
  return MakeMatcher(new TimespecMatcher(expected));
}

}  // namespace

TEST_F(OsTest, GetControlSocketReturnsFdAndZeroOnSuccess) {
  constexpr char kSocketName[] = "fake-daemon";
  constexpr int kFakeValidFd = 100;
  EXPECT_CALL(*raw_os_, GetControlSocket(StrEq(kSocketName)))
      .WillOnce(Return(kFakeValidFd));

  constexpr std::tuple<int, Os::Errno> kExpectedResult{kFakeValidFd, 0};
  EXPECT_EQ(kExpectedResult, os_->GetControlSocket(kSocketName));
}

TEST_F(OsTest, GetControlSocketReturnsInvalidFdAndErrorOnFailure) {
  constexpr char kSocketName[] = "fake-daemon";
  constexpr Os::Errno kError = EINVAL;
  EXPECT_CALL(*raw_os_, GetControlSocket(StrEq(kSocketName)))
      .WillOnce(SetErrnoAndReturn(kError, -1));

  constexpr std::tuple<int, Os::Errno> kExpectedResult{Os::kInvalidFd, kError};
  EXPECT_EQ(kExpectedResult, os_->GetControlSocket(kSocketName));
}

TEST_F(OsTest, GetTimestampSucceeds) {
  constexpr auto kFakeSecs = 1U;
  constexpr auto kFakeNsecs = 2U;
  constexpr struct timespec fake_time { kFakeSecs, kFakeNsecs };
  EXPECT_CALL(*raw_os_, ClockGettime(_, _))
      .WillOnce(DoAll(SetArgumentPointee<1>(fake_time), Return(0)));

  const Os::Timestamp received = os_->GetTimestamp(CLOCK_REALTIME);
  EXPECT_EQ(kFakeSecs, received.secs);
  EXPECT_EQ(kFakeNsecs, received.nsecs);
}

TEST_F(OsTest, NanosleepPassesNormalValueToSyscall) {
  constexpr auto kSleepTimeNsec = 100;
  EXPECT_CALL(*raw_os_,
              Nanosleep(Pointee(EqualsTimespec({0, kSleepTimeNsec})), _));
  os_->Nanosleep(kSleepTimeNsec);
}

TEST_F(OsTest, NanosleepPassesMaxmimalValueToSyscall) {
  EXPECT_CALL(*raw_os_,
              Nanosleep(Pointee(EqualsTimespec({0, Os::kMaxNanos})), _));
  os_->Nanosleep(Os::kMaxNanos);
}

TEST_F(OsTest, NanosleepPassesZeroValueToSyscall) {
  EXPECT_CALL(*raw_os_, Nanosleep(Pointee(EqualsTimespec({0, 0})), _));
  os_->Nanosleep(0);
}

TEST_F(OsTest, NanosleepClampsOverlyLargeValue) {
  EXPECT_CALL(*raw_os_,
              Nanosleep(Pointee(EqualsTimespec({0, Os::kMaxNanos})), _));
  os_->Nanosleep(Os::kMaxNanos + 1);
}

TEST_F(OsTest, NanosleepRetriesOnInterruptedCall) {
  InSequence seq;
  EXPECT_CALL(*raw_os_, Nanosleep(_, NotNull()))
      .WillOnce(Invoke([](const timespec* /* desired */, timespec* remaining) {
        *remaining = {0, 100};
        errno = EINTR;
        return -1;
      }));
  EXPECT_CALL(*raw_os_, Nanosleep(Pointee(EqualsTimespec({0, 100})), _));
  os_->Nanosleep(Os::kMaxNanos);
}

TEST_F(OsTest, NanosleepRetriesMultipleTimesIfNecessary) {
  InSequence seq;
  EXPECT_CALL(*raw_os_, Nanosleep(_, NotNull()))
      .WillOnce(Invoke([](const timespec* /* desired */, timespec* remaining) {
        *remaining = {0, 100};
        errno = EINTR;
        return -1;
      }));
  EXPECT_CALL(*raw_os_, Nanosleep(_, NotNull()))
      .WillOnce(Invoke([](const timespec* /* desired */, timespec* remaining) {
        *remaining = {0, 50};
        errno = EINTR;
        return -1;
      }));
  EXPECT_CALL(*raw_os_, Nanosleep(Pointee(EqualsTimespec({0, 50})), _));
  os_->Nanosleep(Os::kMaxNanos);
}

TEST_F(OsTest, NanosleepIgnoresEintrWithZeroTimeRemaining) {
  InSequence seq;
  EXPECT_CALL(*raw_os_, Nanosleep(_, NotNull()))
      .WillOnce(Invoke([](const timespec* /* desired */, timespec* remaining) {
        *remaining = {0, 0};
        errno = EINTR;
        return -1;
      }));
  EXPECT_CALL(*raw_os_, Nanosleep(_, _)).Times(0);
  os_->Nanosleep(Os::kMaxNanos);
}

TEST_F(OsTest, ReceiveDatagramReturnsCorrectValueForMaxSizedDatagram) {
  constexpr int kFakeFd = 100;
  std::array<uint8_t, 8192> buffer{};
  EXPECT_CALL(*raw_os_, Recv(kFakeFd, buffer.data(), buffer.size(), MSG_TRUNC))
      .WillOnce(Return(buffer.size()));

  constexpr std::tuple<size_t, Os::Errno> kExpectedResult{buffer.size(), 0};
  EXPECT_EQ(kExpectedResult,
            os_->ReceiveDatagram(kFakeFd, buffer.data(), buffer.size()));
}

TEST_F(OsTest, ReceieveDatagramReturnsCorrectValueForRegularSizedDatagram) {
  constexpr int kFakeFd = 100;
  constexpr auto kReadBufferSize = 8192;
  constexpr auto kDatagramSize = kReadBufferSize / 2;
  std::array<uint8_t, kReadBufferSize> buffer{};
  EXPECT_CALL(*raw_os_, Recv(kFakeFd, buffer.data(), buffer.size(), MSG_TRUNC))
      .WillOnce(Return(kDatagramSize));

  constexpr std::tuple<size_t, Os::Errno> kExpectedResult{kDatagramSize, 0};
  EXPECT_EQ(kExpectedResult,
            os_->ReceiveDatagram(kFakeFd, buffer.data(), buffer.size()));
}

TEST_F(OsTest, ReceieveDatagramReturnsCorrectValueForOversizedDatagram) {
  constexpr int kFakeFd = 100;
  constexpr auto kReadBufferSize = 8192;
  constexpr auto kDatagramSize = kReadBufferSize * 2;
  std::array<uint8_t, kReadBufferSize> buffer{};
  EXPECT_CALL(*raw_os_, Recv(kFakeFd, buffer.data(), buffer.size(), MSG_TRUNC))
      .WillOnce(Return(kDatagramSize));

  constexpr std::tuple<size_t, Os::Errno> kExpectedResult{kDatagramSize, 0};
  EXPECT_EQ(kExpectedResult,
            os_->ReceiveDatagram(kFakeFd, buffer.data(), buffer.size()));
}

TEST_F(OsTest, ReceieveDatagramReturnsCorrectValueForZeroByteDatagram) {
  constexpr int kFakeFd = 100;
  std::array<uint8_t, 8192> buffer{};
  EXPECT_CALL(*raw_os_, Recv(kFakeFd, buffer.data(), buffer.size(), MSG_TRUNC))
      .WillOnce(Return(0));

  constexpr std::tuple<size_t, Os::Errno> kExpectedResult{0, 0};
  EXPECT_EQ(kExpectedResult,
            os_->ReceiveDatagram(kFakeFd, buffer.data(), buffer.size()));
}

TEST_F(OsTest, ReceieveDatagramReturnsCorrectValueOnFailure) {
  constexpr int kFakeFd = 100;
  constexpr Os::Errno kError = EBADF;
  std::array<uint8_t, 8192> buffer{};
  EXPECT_CALL(*raw_os_, Recv(kFakeFd, buffer.data(), buffer.size(), MSG_TRUNC))
      .WillOnce(SetErrnoAndReturn(kError, -1));

  constexpr std::tuple<size_t, Os::Errno> kExpectedResult{0, kError};
  EXPECT_EQ(kExpectedResult,
            os_->ReceiveDatagram(kFakeFd, buffer.data(), buffer.size()));
}

TEST_F(OsTest, WriteReturnsCorrectValueForSuccessfulWrite) {
  constexpr int kFakeFd = 100;
  constexpr std::array<uint8_t, 8192> buffer{};
  EXPECT_CALL(*raw_os_, Write(kFakeFd, buffer.data(), buffer.size()))
      .WillOnce(Return(buffer.size()));

  constexpr std::tuple<size_t, Os::Errno> kExpectedResult{buffer.size(), 0};
  EXPECT_EQ(kExpectedResult, os_->Write(kFakeFd, buffer.data(), buffer.size()));
}

TEST_F(OsTest, WriteReturnsCorrectValueForTruncatedWrite) {
  constexpr int kFakeFd = 100;
  constexpr int kBytesWritten = 4096;
  constexpr std::array<uint8_t, 8192> buffer{};
  EXPECT_CALL(*raw_os_, Write(kFakeFd, buffer.data(), buffer.size()))
      .WillOnce(Return(kBytesWritten));

  constexpr std::tuple<size_t, Os::Errno> kExpectedResult{kBytesWritten, 0};
  EXPECT_EQ(kExpectedResult, os_->Write(kFakeFd, buffer.data(), buffer.size()));
}

TEST_F(OsTest, WriteReturnsCorrectValueForSuccessfulZeroByteWrite) {
  constexpr int kFakeFd = 100;
  constexpr std::array<uint8_t, 0> buffer{};
  EXPECT_CALL(*raw_os_, Write(kFakeFd, buffer.data(), 0)).WillOnce(Return(0));

  constexpr std::tuple<size_t, Os::Errno> kExpectedResult{0, 0};
  EXPECT_EQ(kExpectedResult, os_->Write(kFakeFd, buffer.data(), buffer.size()));
}

TEST_F(OsTest, WriteReturnsCorrectValueForFailedWrite) {
  constexpr int kFakeFd = 100;
  constexpr Os::Errno kError = EBADF;
  constexpr std::array<uint8_t, 8192> buffer{};
  EXPECT_CALL(*raw_os_, Write(kFakeFd, buffer.data(), buffer.size()))
      .WillOnce(SetErrnoAndReturn(kError, -1));

  constexpr std::tuple<size_t, Os::Errno> kExpectedResult{0, kError};
  EXPECT_EQ(kExpectedResult, os_->Write(kFakeFd, buffer.data(), buffer.size()));
}

TEST_F(OsTest, WriteReturnsCorrectValueForFailedZeroByteWrite) {
  constexpr int kFakeFd = 100;
  constexpr Os::Errno kError = EBADF;
  constexpr std::array<uint8_t, 0> buffer{};
  EXPECT_CALL(*raw_os_, Write(kFakeFd, buffer.data(), 0))
      .WillOnce(SetErrnoAndReturn(kError, -1));

  constexpr std::tuple<size_t, Os::Errno> kExpectedResult{0, kError};
  EXPECT_EQ(kExpectedResult, os_->Write(kFakeFd, buffer.data(), buffer.size()));
}

// Per
// github.com/google/googletest/blob/master/googletest/docs/AdvancedGuide.md#death-tests,
// death tests should be specially named.
using OsDeathTest = OsTest;

TEST_F(OsDeathTest, GetTimestampOverlyLargeNsecsCausesDeath) {
  constexpr auto kFakeSecs = 1U;
  constexpr auto kFakeNsecs = 1000 * 1000 * 1000;
  constexpr struct timespec fake_time { kFakeSecs, kFakeNsecs };
  ON_CALL(*raw_os_, ClockGettime(_, _))
      .WillByDefault(DoAll(SetArgumentPointee<1>(fake_time), Return(0)));
  EXPECT_DEATH(os_->GetTimestamp(CLOCK_REALTIME), "Check failed");
}

TEST_F(OsDeathTest, GetTimestampRawOsErrorCausesDeath) {
  ON_CALL(*raw_os_, ClockGettime(_, _)).WillByDefault(Return(-1));
  EXPECT_DEATH(os_->GetTimestamp(CLOCK_REALTIME), "Unexpected error");
}

TEST_F(OsDeathTest, NanosleepUnexpectedErrorCausesDeath) {
  ON_CALL(*raw_os_, Nanosleep(Pointee(EqualsTimespec({0, Os::kMaxNanos})), _))
      .WillByDefault(SetErrnoAndReturn(EFAULT, -1));
  EXPECT_DEATH(os_->Nanosleep(Os::kMaxNanos), "Unexpected error");
}

TEST_F(OsDeathTest, ReceiveDatagramWithOverlyLargeBufferCausesDeath) {
  constexpr int kFakeFd = 100;
  std::array<uint8_t, 8192> buffer{};
  EXPECT_DEATH(
      os_->ReceiveDatagram(kFakeFd, buffer.data(), GetMaxVal<size_t>()),
      "Check failed");
}

TEST_F(OsDeathTest, WriteWithOverlyLargeBufferCausesDeath) {
  constexpr int kFakeFd = 100;
  constexpr std::array<uint8_t, 8192> buffer{};
  EXPECT_DEATH(os_->Write(kFakeFd, buffer.data(), GetMaxVal<size_t>()),
               "Check failed");
}

TEST_F(OsDeathTest, WriteWithOverrunCausesDeath) {
  constexpr int kFakeFd = 100;
  constexpr std::array<uint8_t, 8192> buffer{};
  ON_CALL(*raw_os_, Write(kFakeFd, buffer.data(), buffer.size()))
      .WillByDefault(Return(buffer.size() + 1));
  EXPECT_DEATH(os_->Write(kFakeFd, buffer.data(), buffer.size()),
               "Check failed");
}

}  // namespace wifilogd
}  // namespace android
