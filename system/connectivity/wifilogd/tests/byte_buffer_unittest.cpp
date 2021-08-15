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
#include <cstring>
#include <string>

#include "gtest/gtest.h"

#include "wifilogd/byte_buffer.h"

namespace android {
namespace wifilogd {
namespace {

constexpr size_t kBufferSizeBytes = 1024;
constexpr std::array<uint8_t, 1> kSmallestMessage{};
constexpr std::array<uint8_t, kBufferSizeBytes> kLargestMessage{};

class ByteBufferTest : public ::testing::Test {
 public:
  ByteBufferTest() {}

 protected:
  ByteBuffer<kBufferSizeBytes> buffer_;
};

}  // namespace

TEST_F(ByteBufferTest, AppendMinimalOnEmptyBufferSucceeds) {
  buffer_.AppendOrDie(kSmallestMessage.data(), kSmallestMessage.size());
}

TEST_F(ByteBufferTest, AppendMaximalOnEmptyBufferSucceeds) {
  buffer_.AppendOrDie(kLargestMessage.data(), kLargestMessage.size());
}

TEST_F(ByteBufferTest, AppendStoresOurData) {
  const std::string message1{"hello"};
  const std::string message2{"world"};
  buffer_.AppendOrDie(message1.data(), message1.size());
  buffer_.AppendOrDie(message2.data(), message2.size());

  const std::string expected{"helloworld"};
  EXPECT_EQ(0, std::memcmp(buffer_.data(), expected.data(), expected.size()));
}

TEST_F(ByteBufferTest, AssigningWorks) {
  const std::string message1{"hello"};
  buffer_.AppendOrDie(message1.data(), message1.size());

  ByteBuffer<kBufferSizeBytes> copy;
  ASSERT_NE(buffer_.size(), copy.size());

  copy = buffer_;
  ASSERT_EQ(buffer_.size(), copy.size());
  EXPECT_EQ(0, std::memcmp(copy.data(), buffer_.data(), buffer_.size()));
}

TEST_F(ByteBufferTest, ChainingWorks) {
  const std::string message1{"hello"};
  const std::string message2{"world"};
  buffer_.AppendOrDie(message1.data(), message1.size())
      .AppendOrDie(message2.data(), message2.size());

  const std::string expected{"helloworld"};
  EXPECT_EQ(0, std::memcmp(buffer_.data(), expected.data(), expected.size()));
}

TEST_F(ByteBufferTest, CopyingWorks) {
  const std::string message1{"hello"};
  buffer_.AppendOrDie(message1.data(), message1.size());

  const ByteBuffer<kBufferSizeBytes> copy{buffer_};
  ASSERT_EQ(buffer_.size(), copy.size());
  EXPECT_EQ(0, std::memcmp(copy.data(), buffer_.data(), buffer_.size()));
}

TEST_F(ByteBufferTest, DataDoesNotReturnNullOnFreshBuffer) {
  EXPECT_NE(nullptr, buffer_.data());
}

TEST_F(ByteBufferTest, DataDoesNotReturnNullAfterLargeWrite) {
  buffer_.AppendOrDie(kLargestMessage.data(), kLargestMessage.size());
  EXPECT_NE(nullptr, buffer_.data());
}

TEST_F(ByteBufferTest, SizeReturnsZeroOnFreshBuffer) {
  EXPECT_EQ(0U, buffer_.size());
}

TEST_F(ByteBufferTest, SizeIsCorrectAfterSmallWrite) {
  buffer_.AppendOrDie(kSmallestMessage.data(), kSmallestMessage.size());
  EXPECT_EQ(kSmallestMessage.size(), buffer_.size());
}

TEST_F(ByteBufferTest, SizeIsCorrectAfterLargeWrite) {
  buffer_.AppendOrDie(kLargestMessage.data(), kLargestMessage.size());
  EXPECT_EQ(kLargestMessage.size(), buffer_.size());
}

TEST_F(ByteBufferTest, SizeIsCorrectAfterMultipleWrites) {
  buffer_.AppendOrDie(kSmallestMessage.data(), kSmallestMessage.size())
      .AppendOrDie(kSmallestMessage.data(), kSmallestMessage.size())
      .AppendOrDie(kSmallestMessage.data(), kSmallestMessage.size());
  EXPECT_EQ(3 * kSmallestMessage.size(), buffer_.size());
}

// Per
// github.com/google/googletest/blob/master/googletest/docs/AdvancedGuide.md#death-tests,
// death tests should be specially named.
using ByteBufferDeathTest = ByteBufferTest;

TEST_F(ByteBufferDeathTest, AppendLargerThanBufferCausesDeath) {
  constexpr std::array<uint8_t, kBufferSizeBytes + 1> oversized_message{};
  EXPECT_DEATH(
      buffer_.AppendOrDie(oversized_message.data(), oversized_message.size()),
      "Check failed");
}

TEST_F(ByteBufferDeathTest, AppendLargerThanFreeSpaceCausesDeath) {
  buffer_.AppendOrDie(kLargestMessage.data(), kLargestMessage.size());
  EXPECT_DEATH(
      buffer_.AppendOrDie(kSmallestMessage.data(), kSmallestMessage.size()),
      "Check failed");
}

}  // namespace wifilogd
}  // namespace android
