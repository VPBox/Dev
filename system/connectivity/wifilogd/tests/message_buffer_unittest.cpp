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
#include <tuple>
#include <vector>

#include "gtest/gtest.h"

#include "wifilogd/message_buffer.h"

namespace android {
namespace wifilogd {
namespace {

constexpr size_t kBufferSizeBytes = 1024;
constexpr size_t kHeaderSizeBytes = MessageBuffer::GetHeaderSize();
constexpr std::array<uint8_t, 1> kSmallestMessage{};
constexpr std::array<uint8_t, kBufferSizeBytes - kHeaderSizeBytes>
    kLargestMessage{};

class MessageBufferTest : public ::testing::Test {
 public:
  MessageBufferTest() : buffer_{kBufferSizeBytes} {}

 protected:
  size_t FillBufferWithMultipleMessages() {
    constexpr std::array<uint8_t, kHeaderSizeBytes> message{};
    static_assert(kBufferSizeBytes % (kHeaderSizeBytes + message.size()) == 0,
                  "messages will not fill buffer to capacity");
    size_t n_written;
    for (n_written = 0;
         n_written < kBufferSizeBytes / (kHeaderSizeBytes + message.size());
         ++n_written) {
      EXPECT_TRUE(buffer_.Append(message.data(), message.size()));
    }
    EXPECT_EQ(0U, buffer_.GetFreeSize());
    return n_written;
  }

  std::vector<uint8_t> GetNextMessageAsByteVector() {
    const uint8_t* start;
    size_t len;
    std::tie(start, len) = buffer_.ConsumeNextMessage();
    return {start, start + len};
  }

  MessageBuffer buffer_;
};

}  // namespace

TEST_F(MessageBufferTest, AppendMinimalOnEmptyBufferSucceeds) {
  EXPECT_TRUE(buffer_.Append(kSmallestMessage.data(), kSmallestMessage.size()));
}

TEST_F(MessageBufferTest, AppendMaximalOnEmptyBufferSucceeds) {
  EXPECT_TRUE(buffer_.Append(kLargestMessage.data(), kLargestMessage.size()));
}

TEST_F(MessageBufferTest, AppendMaximalAfterFillAndClearSucceeds) {
  ASSERT_TRUE(buffer_.Append(kLargestMessage.data(), kLargestMessage.size()));
  ASSERT_FALSE(buffer_.CanFitNow(1));

  buffer_.Clear();
  EXPECT_TRUE(buffer_.Append(kLargestMessage.data(), kLargestMessage.size()));
}

TEST_F(MessageBufferTest, AppendUnalignedMessagesDoesNotCrash) {
  // Odd-length messages should trigger alignment problems, if any such
  // problems exist. We'll need more than one, though, since the first header
  // might be aligned by default.
  constexpr std::array<uint8_t, 1> message{};
  while (buffer_.CanFitNow(message.size())) {
    ASSERT_TRUE(buffer_.Append(message.data(), message.size()));
  }
}

TEST_F(MessageBufferTest, AppendLargerThanBufferFails) {
  constexpr std::array<uint8_t, kBufferSizeBytes + 1> oversized_message{};
  EXPECT_FALSE(
      buffer_.Append(oversized_message.data(), oversized_message.size()));
}

TEST_F(MessageBufferTest, AppendLargerThanFreeSpaceFails) {
  constexpr size_t expected_free = kBufferSizeBytes - kHeaderSizeBytes;
  ASSERT_FALSE(buffer_.CanFitNow(expected_free + 1));

  constexpr std::array<uint8_t, expected_free + 1> oversized_message{};
  EXPECT_FALSE(
      buffer_.Append(oversized_message.data(), oversized_message.size()));
}

TEST_F(MessageBufferTest, AppendMultipleMessagesToFillBufferDoesNotCrash) {
  FillBufferWithMultipleMessages();
}

TEST_F(MessageBufferTest, CanFitNowIsCorrectOnFreshBuffer) {
  EXPECT_TRUE(buffer_.CanFitNow(kLargestMessage.size()));
  EXPECT_FALSE(buffer_.CanFitNow(kLargestMessage.size() + 1));
}

TEST_F(MessageBufferTest, CanFitNowIsCorrectAfterSmallWrite) {
  ASSERT_TRUE(buffer_.Append(kSmallestMessage.data(), kSmallestMessage.size()));

  constexpr size_t expected_free =
      kBufferSizeBytes - (kSmallestMessage.size() + kHeaderSizeBytes) -
      kHeaderSizeBytes;
  EXPECT_TRUE(buffer_.CanFitNow(expected_free));
  EXPECT_FALSE(buffer_.CanFitNow(expected_free + 1));
}

TEST_F(MessageBufferTest, CanFitNowIsCorrectOnFullBuffer) {
  ASSERT_TRUE(buffer_.Append(kLargestMessage.data(), kLargestMessage.size()));
  EXPECT_FALSE(buffer_.CanFitNow(1));
}

TEST_F(MessageBufferTest, CanFitNowIsCorrectOnRewoundBuffer) {
  ASSERT_TRUE(buffer_.Append(kLargestMessage.data(), kLargestMessage.size()));
  buffer_.Rewind();
  EXPECT_FALSE(buffer_.CanFitNow(1));
}

TEST_F(MessageBufferTest, CanFitNowIsCorrectAfterClear) {
  ASSERT_TRUE(buffer_.Append(kLargestMessage.data(), kLargestMessage.size()));
  ASSERT_FALSE(buffer_.CanFitNow(1));

  buffer_.Clear();
  EXPECT_TRUE(buffer_.CanFitNow(kLargestMessage.size()));
}

TEST_F(MessageBufferTest, CanFitEverIsCorrectOnFreshBuffer) {
  EXPECT_TRUE(buffer_.CanFitEver(kLargestMessage.size()));
  EXPECT_FALSE(buffer_.CanFitEver(kLargestMessage.size() + 1));
}

TEST_F(MessageBufferTest, CanFitEverIsCorrectAfterSmallWrite) {
  ASSERT_TRUE(buffer_.Append(kSmallestMessage.data(), kSmallestMessage.size()));
  EXPECT_TRUE(buffer_.CanFitEver(kLargestMessage.size()));
  EXPECT_FALSE(buffer_.CanFitEver(kLargestMessage.size() + 1));
}

TEST_F(MessageBufferTest, CanFitEverIsCorrectOnFullBuffer) {
  ASSERT_TRUE(buffer_.Append(kLargestMessage.data(), kLargestMessage.size()));
  EXPECT_TRUE(buffer_.CanFitEver(kLargestMessage.size()));
  EXPECT_FALSE(buffer_.CanFitEver(kLargestMessage.size() + 1));
}

TEST_F(MessageBufferTest, CanFitEverIsCorrectAfterRewind) {
  ASSERT_TRUE(buffer_.Append(kLargestMessage.data(), kLargestMessage.size()));
  buffer_.Rewind();
  EXPECT_TRUE(buffer_.CanFitEver(kLargestMessage.size()));
  EXPECT_FALSE(buffer_.CanFitEver(kLargestMessage.size() + 1));
}

TEST_F(MessageBufferTest, CanFitEverIsCorrectAfterClear) {
  ASSERT_TRUE(buffer_.Append(kLargestMessage.data(), kLargestMessage.size()));
  buffer_.Clear();
  EXPECT_TRUE(buffer_.CanFitEver(kLargestMessage.size()));
  EXPECT_FALSE(buffer_.CanFitEver(kLargestMessage.size() + 1));
}

TEST_F(MessageBufferTest, ConsumeNextMessageReturnsNullOnFreshBuffer) {
  const std::tuple<const uint8_t*, size_t> expected{nullptr, 0};
  EXPECT_EQ(expected, buffer_.ConsumeNextMessage());
}

TEST_F(MessageBufferTest, ConsumeNextMessageReturnsNullAfterFillAndClear) {
  ASSERT_TRUE(buffer_.Append(kLargestMessage.data(), kLargestMessage.size()));
  buffer_.Clear();

  const std::tuple<const uint8_t*, size_t> expected{nullptr, 0};
  EXPECT_EQ(expected, buffer_.ConsumeNextMessage());
}

TEST_F(MessageBufferTest, ConsumeNextMessageCanReadMinimalMessage) {
  ASSERT_TRUE(buffer_.Append(kSmallestMessage.data(), kSmallestMessage.size()));

  const auto& ptr_and_size = buffer_.ConsumeNextMessage();
  EXPECT_NE(nullptr, std::get<0>(ptr_and_size));
  EXPECT_EQ(kSmallestMessage.size(), std::get<1>(ptr_and_size));
}

TEST_F(MessageBufferTest, ConsumeNextMessageCanReadMaximalMessage) {
  ASSERT_TRUE(buffer_.Append(kLargestMessage.data(), kLargestMessage.size()));

  const auto& ptr_and_size = buffer_.ConsumeNextMessage();
  EXPECT_NE(nullptr, std::get<0>(ptr_and_size));
  EXPECT_EQ(kLargestMessage.size(), std::get<1>(ptr_and_size));
}

TEST_F(MessageBufferTest,
       ConsumeNextMessageReturnsNullAfterMinimalMessageIsConsumed) {
  ASSERT_TRUE(buffer_.Append(kSmallestMessage.data(), kSmallestMessage.size()));
  buffer_.ConsumeNextMessage();

  constexpr std::tuple<const uint8_t*, size_t> expected{nullptr, 0};
  EXPECT_EQ(expected, buffer_.ConsumeNextMessage());
}

TEST_F(MessageBufferTest,
       ConsumeNextMessageReturnsNullAfterMaximalMessageIsConsumed) {
  ASSERT_TRUE(buffer_.Append(kLargestMessage.data(), kLargestMessage.size()));
  buffer_.ConsumeNextMessage();

  constexpr std::tuple<const uint8_t*, size_t> expected{nullptr, 0};
  EXPECT_EQ(expected, buffer_.ConsumeNextMessage());
}

TEST_F(MessageBufferTest,
       ConsumeNextMessageCanRetreiveAllMessagesFromFullBuffer) {
  const size_t n_written = FillBufferWithMultipleMessages();
  size_t n_read = 0;
  while (std::get<0>(buffer_.ConsumeNextMessage())) {
    ++n_read;
  }
  EXPECT_EQ(n_written, n_read);

  constexpr std::tuple<const uint8_t*, size_t> expected{nullptr, 0};
  EXPECT_EQ(expected, buffer_.ConsumeNextMessage());
}

TEST_F(MessageBufferTest,
       ConsumeNextMessageCanRetreiveAllMessagesFromRewoundBuffer) {
  const size_t n_written = FillBufferWithMultipleMessages();
  while (std::get<0>(buffer_.ConsumeNextMessage())) {
    // Silently consume message
  }
  buffer_.Rewind();

  size_t n_read = 0;
  while (std::get<0>(buffer_.ConsumeNextMessage())) {
    ++n_read;
  }
  EXPECT_EQ(n_written, n_read);
}

TEST_F(MessageBufferTest,
       ConsumeNextMessageCanRetreiveMultipleUnaliagnedMessages) {
  // As in AppendUnalignedMessagesDoesNotCrash, odd-length messages should
  // trigger alignment problems, if any such problems exist.
  const std::array<uint8_t, 1> message{};
  size_t n_written = 0;
  while (buffer_.CanFitNow(message.size())) {
    ASSERT_TRUE(buffer_.Append(message.data(), message.size()));
    ++n_written;
  }

  size_t n_read = 0;
  while (std::get<0>(buffer_.ConsumeNextMessage())) {
    ++n_read;
  }
  EXPECT_EQ(n_written, n_read);
}

TEST_F(MessageBufferTest, ConsumeNextMessageReturnsOurMessages) {
  const std::vector<uint8_t> message1{{'h', 'e', 'l', 'l', 'o'}};
  const std::vector<uint8_t> message2{{'w', 'o', 'r', 'l', 'd'}};
  ASSERT_TRUE(
      buffer_.Append(message1.data(), static_cast<uint16_t>(message1.size())));
  ASSERT_TRUE(
      buffer_.Append(message2.data(), static_cast<uint16_t>(message2.size())));
  EXPECT_EQ(message1, GetNextMessageAsByteVector());
  EXPECT_EQ(message2, GetNextMessageAsByteVector());
}

TEST_F(MessageBufferTest, GetFreeSizeIsCorrectOnFreshBuffer) {
  EXPECT_EQ(kBufferSizeBytes, buffer_.GetFreeSize());
}

TEST_F(MessageBufferTest, GetFreeSizeIsCorrectAfterSmallWrite) {
  ASSERT_TRUE(buffer_.Append(kSmallestMessage.data(), kSmallestMessage.size()));
  EXPECT_EQ(kBufferSizeBytes - kHeaderSizeBytes - kSmallestMessage.size(),
            buffer_.GetFreeSize());
}

TEST_F(MessageBufferTest, GetFreeSizeIsCorrectOnFullBuffer) {
  ASSERT_TRUE(buffer_.Append(kLargestMessage.data(), kLargestMessage.size()));
  EXPECT_EQ(0U, buffer_.GetFreeSize());
}

TEST_F(MessageBufferTest, GetFreeSizeIsCorrectAfterRewindOfFullBuffer) {
  ASSERT_TRUE(buffer_.Append(kLargestMessage.data(), kLargestMessage.size()));
  buffer_.Rewind();
  EXPECT_EQ(0U, buffer_.GetFreeSize());
}

TEST_F(MessageBufferTest, GetFreeSizeIsCorrectAfterClear) {
  ASSERT_TRUE(buffer_.Append(kLargestMessage.data(), kLargestMessage.size()));
  buffer_.Clear();
  EXPECT_EQ(kBufferSizeBytes, buffer_.GetFreeSize());
}

TEST_F(MessageBufferTest, RewindDoesNotAffectWritePointer) {
  const std::vector<uint8_t> message1{{'h', 'e', 'l', 'l', 'o'}};
  ASSERT_TRUE(
      buffer_.Append(message1.data(), static_cast<uint16_t>(message1.size())));
  buffer_.Rewind();

  const std::vector<uint8_t> message2{{'w', 'o', 'r', 'l', 'd'}};
  ASSERT_TRUE(
      buffer_.Append(message2.data(), static_cast<uint16_t>(message2.size())));

  EXPECT_EQ(message1, GetNextMessageAsByteVector());
  EXPECT_EQ(message2, GetNextMessageAsByteVector());
}

// Per
// github.com/google/googletest/blob/master/googletest/docs/AdvancedGuide.md#death-tests,
// death tests should be specially named.
using MessageBufferDeathTest = MessageBufferTest;

TEST_F(MessageBufferDeathTest, AppendZeroBytesCausesDeath) {
  constexpr std::array<uint8_t, 1> message{};
  EXPECT_DEATH(buffer_.Append(message.data(), 0), "Check failed");
}

TEST_F(MessageBufferDeathTest, ConstructionOfUselesslySmallBufferCausesDeath) {
  EXPECT_DEATH(MessageBuffer{kHeaderSizeBytes}, "Check failed");
}

}  // namespace wifilogd
}  // namespace android
