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

#include "gtest/gtest.h"

#include "wifilogd/byte_buffer.h"
#include "wifilogd/local_utils.h"
#include "wifilogd/memory_reader.h"

namespace android {
namespace wifilogd {

using local_utils::GetMaxVal;

TEST(MemoryReaderTest, BoolConversionNonEmptyBufferYieldsTrue) {
  constexpr std::array<uint8_t, 1> buffer{};
  MemoryReader memory_reader(buffer.data(), buffer.size());
  EXPECT_TRUE(memory_reader);
}

TEST(MemoryReaderTest, BoolConversionEmptyBufferYieldsFalse) {
  constexpr std::array<uint8_t, 0> buffer{};
  MemoryReader memory_reader(buffer.data(), buffer.size());
  EXPECT_FALSE(memory_reader);
}

TEST(MemoryReaderTest, BoolConversionNullBufferYieldsFalse) {
  MemoryReader memory_reader(nullptr, 10);
  EXPECT_FALSE(memory_reader);
}

TEST(MemoryReaderTest, CopyAssignmentWorks) {
  constexpr std::array<uint8_t, 3> buffer{{1, 2, 3}};

  MemoryReader reader1{buffer.data(), buffer.size()};
  EXPECT_EQ(buffer[0], *reader1.GetBytesOrDie(sizeof(uint8_t)));

  // A copy of reader1 should reflect the byte already read.
  MemoryReader reader2 = reader1;
  EXPECT_EQ(buffer[1], *reader2.GetBytesOrDie(sizeof(uint8_t)));

  // reader1 should _not_ reflect the additional read on reader2.
  EXPECT_GT(reader1.size(), reader2.size());
}

TEST(MemoryReaderTest, CopyConstructionWorks) {
  constexpr std::array<uint8_t, 3> buffer{{1, 2, 3}};

  MemoryReader reader1{buffer.data(), buffer.size()};
  EXPECT_EQ(buffer[0], *reader1.GetBytesOrDie(sizeof(uint8_t)));

  // A copy of reader1 should reflect the byte already read.
  MemoryReader reader2(reader1);
  EXPECT_EQ(buffer[1], *reader2.GetBytesOrDie(sizeof(uint8_t)));

  // reader1 should _not_ reflect the additional read on reader2.
  EXPECT_GT(reader1.size(), reader2.size());
}

TEST(MemoryReaderTest, CopyOutOrDieCopiesData) {
  constexpr struct Message {
    int a;
    char b;
  } original_message{5, 'c'};
  const auto& duplicate_message =
      MemoryReader(&original_message, sizeof(original_message))
          .CopyOutOrDie<Message>();
  EXPECT_EQ(original_message.a, duplicate_message.a);
  EXPECT_EQ(original_message.b, duplicate_message.b);
}

TEST(MemoryReaderTest, CopyOutOrDieWorksForMultipleObjects) {
  constexpr struct M1 {
    int a;
    char b;
  } first_message{1, 'a'};
  constexpr struct M2 {
    uint64_t a;
    uint64_t b;
  } second_message{0, GetMaxVal(second_message.b)};
  const auto& buf = ByteBuffer<sizeof(first_message) + sizeof(second_message)>()
                        .AppendOrDie(&first_message, sizeof(first_message))
                        .AppendOrDie(&second_message, sizeof(second_message));

  MemoryReader memory_reader(buf.data(), buf.size());
  const auto& copy_of_first_message = memory_reader.CopyOutOrDie<M1>();
  const auto& copy_of_second_message = memory_reader.CopyOutOrDie<M2>();
  EXPECT_EQ(first_message.a, copy_of_first_message.a);
  EXPECT_EQ(first_message.b, copy_of_first_message.b);
  EXPECT_EQ(second_message.a, copy_of_second_message.a);
  EXPECT_EQ(second_message.b, copy_of_second_message.b);
}

TEST(MemoryReaderTest, GetBytesOrDieSucceedsOnSmallRead) {
  constexpr std::array<uint8_t, 1024> buffer{};
  MemoryReader memory_reader(buffer.data(), buffer.size());
  EXPECT_EQ(buffer.data(), memory_reader.GetBytesOrDie(1));
}

TEST(MemoryReaderTest, GetBytesOrDieSucceedsOnFullRead) {
  constexpr std::array<uint8_t, 1024> buffer{};
  MemoryReader memory_reader(buffer.data(), buffer.size());
  EXPECT_EQ(buffer.data(), memory_reader.GetBytesOrDie(buffer.size()));
}

TEST(MemoryReaderTest, GetBytesOrDieCanConsumeFullBufferInParts) {
  constexpr std::array<uint8_t, 1024> buffer{};
  static_assert(buffer.size() % 2 == 0, "Test will not consume whole buffer");
  MemoryReader memory_reader(buffer.data(), buffer.size());
  EXPECT_EQ(buffer.data(), memory_reader.GetBytesOrDie(buffer.size() / 2));
  EXPECT_EQ(buffer.data() + buffer.size() / 2,
            memory_reader.GetBytesOrDie(buffer.size() / 2));
}

TEST(MemoryReaderTest, SizeIsCorrectForNonEmptyBuffer) {
  constexpr std::array<uint8_t, 1> buffer{};
  MemoryReader memory_reader(buffer.data(), buffer.size());
  EXPECT_EQ(buffer.size(), memory_reader.size());
}

TEST(MemoryReaderTest, SizeIsCorrectForEmptyBuffer) {
  constexpr std::array<uint8_t, 0> buffer{};
  MemoryReader memory_reader(buffer.data(), buffer.size());
  EXPECT_EQ(buffer.size(), memory_reader.size());
}

TEST(MemoryReaderTest, SizeIsCorrectAfterSmallRead) {
  constexpr std::array<uint8_t, 1024> buffer{};
  MemoryReader memory_reader(buffer.data(), buffer.size());
  memory_reader.GetBytesOrDie(1);
  EXPECT_EQ(buffer.size() - 1, memory_reader.size());
}

TEST(MemoryReaderTest, SizeIsCorrectAfterFullRead) {
  constexpr std::array<uint8_t, 1024> buffer{};
  MemoryReader memory_reader(buffer.data(), buffer.size());
  memory_reader.GetBytesOrDie(buffer.size());
  EXPECT_EQ(0U, memory_reader.size());
}

TEST(MemoryReaderTest, SizeIsZeroForNullBuffer) {
  MemoryReader memory_reader(nullptr, 0);
  EXPECT_EQ(0U, memory_reader.size());
}

// Per
// github.com/google/googletest/blob/master/googletest/docs/AdvancedGuide.md#death-tests,
// death tests should be specially named.

TEST(MemoryReaderDeathTest, CopyOutOrDieWithNullBufferCausesDeath) {
  constexpr struct Message {
    int a;
    char b;
  } original{5, 'c'};
  EXPECT_DEATH(MemoryReader(nullptr, sizeof(original)).CopyOutOrDie<Message>(),
               "Check failed");
}

TEST(MemoryReaderDeathTest, CopyOutOrDieWithShortBufferCausesDeath) {
  constexpr struct Message {
    int a;
    char b;
  } original{5, 'c'};
  EXPECT_DEATH(
      MemoryReader(&original, sizeof(original) - 1).CopyOutOrDie<Message>(),
      "Check failed");
}

TEST(MemoryReaderDeathTest, CopyOutOrDieOverrunWithMultipleReadsCausesDeath) {
  constexpr struct Message {
    int a;
    char b;
  } message{5, 'c'};
  MemoryReader memory_reader(&message, sizeof(message));
  memory_reader.CopyOutOrDie<Message>();
  EXPECT_DEATH(memory_reader.CopyOutOrDie<Message>(), "Check failed");
}

TEST(MemoryReaderTest, GetBytesOrDieWithNullBufferCausesDeath) {
  constexpr std::array<uint8_t, 1024> buffer{};
  MemoryReader memory_reader(nullptr, buffer.size());
  EXPECT_DEATH(memory_reader.GetBytesOrDie(buffer.size()), "Check failed");
}

TEST(MemoryReaderTest, GetBytesOrDieWithShortBufferCausesDeath) {
  constexpr std::array<uint8_t, 1024> buffer{};
  MemoryReader memory_reader(buffer.data(), buffer.size());
  EXPECT_DEATH(memory_reader.GetBytesOrDie(buffer.size() + 1), "Check failed");
}

TEST(MemoryReaderTest, GetBytesOrDieOverrunWithMultipleReadsCausesDeath) {
  constexpr std::array<uint8_t, 1024> buffer{};
  MemoryReader memory_reader(buffer.data(), buffer.size());
  EXPECT_EQ(buffer.data(), memory_reader.GetBytesOrDie(buffer.size()));
  EXPECT_DEATH(memory_reader.GetBytesOrDie(1), "Check failed");
}

}  // namespace wifilogd
}  // namespace android
