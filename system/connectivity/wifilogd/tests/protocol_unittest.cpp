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

#include <cstddef>
#include <type_traits>

#include "gtest/gtest.h"

#include "wifilogd/protocol.h"

namespace android {
namespace wifilogd {

// The protocol tests aim to provide friction against changes that
// break byte-stream compatibility. Byte-stream compatibility is
// important for two reasons:
//     1. We need to support non-C++ clients. Those clients may use bindings
//        that are implemented in their own language, rather than using a
//        C or C++ client library.
//     2. We need to maintain compatibility with older clients talking to
//        newer versions of wifilogd.

TEST(ProtocolTest, AsciiMessageChainingWorks) {
  using protocol::AsciiMessage;
  uint8_t tagLen = 3;
  uint16_t dataLen = 7;
  const auto ascii_message_header =
      AsciiMessage().set_tag_len(tagLen).set_data_len(dataLen);
  EXPECT_EQ(tagLen, ascii_message_header.tag_len);
  EXPECT_EQ(dataLen, ascii_message_header.data_len);
}

TEST(ProtocolTest, AsciiMessageNonChainingWorks) {
  using protocol::AsciiMessage;
  uint8_t tagLen = 3;
  uint16_t dataLen = 7;
  AsciiMessage ascii_message_header = AsciiMessage();
  ascii_message_header.set_tag_len(tagLen);
  ascii_message_header.set_data_len(dataLen);
  EXPECT_EQ(tagLen, ascii_message_header.tag_len);
  EXPECT_EQ(dataLen, ascii_message_header.data_len);
}

TEST(ProtocolTest, AsciiMessageLayoutIsUnchanged) {
  using protocol::AsciiMessage;
  ASSERT_TRUE(std::is_standard_layout<AsciiMessage>::value);

  EXPECT_EQ(0U, offsetof(AsciiMessage, data_len));
  EXPECT_EQ(2U, sizeof(AsciiMessage::data_len));

  EXPECT_EQ(2U, offsetof(AsciiMessage, tag_len));
  EXPECT_EQ(1U, sizeof(AsciiMessage::tag_len));

  EXPECT_EQ(3U, offsetof(AsciiMessage, severity));
  EXPECT_EQ(1U, sizeof(AsciiMessage::severity));

  EXPECT_EQ(4U, sizeof(AsciiMessage));
}

TEST(ProtocolTest, CommandLayoutIsUnchanged) {
  using protocol::Command;
  ASSERT_TRUE(std::is_standard_layout<Command>::value);

  EXPECT_EQ(0U, offsetof(Command, src_boottime_nsec));
  EXPECT_EQ(8U, sizeof(Command::src_boottime_nsec));

  EXPECT_EQ(8U, offsetof(Command, sequence_num));
  EXPECT_EQ(2U, sizeof(Command::sequence_num));

  EXPECT_EQ(10U, offsetof(Command, opcode));
  EXPECT_EQ(2U, sizeof(Command::opcode));

  EXPECT_EQ(12U, offsetof(Command, payload_len));
  EXPECT_EQ(2U, sizeof(Command::payload_len));

  // The |reserved| field fills out Command, in place of padding that
  // would otherwise be added by the compiler.
  EXPECT_EQ(14U, offsetof(Command, reserved));
  EXPECT_EQ(2U, sizeof(Command::reserved));

  EXPECT_EQ(16U, sizeof(Command));
}

TEST(ProtocolTest, MaxMessageSizeHasNotShrunk) {
  EXPECT_GE(protocol::kMaxMessageSize, 4096U);
}

TEST(ProtocolTest, MessageSeveritiesAreUnchanged) {
  using protocol::MessageSeverity;
  EXPECT_EQ(0U, static_cast<uint8_t>(MessageSeverity::kError));
  EXPECT_EQ(1U, static_cast<uint8_t>(MessageSeverity::kWarning));
  EXPECT_EQ(2U, static_cast<uint8_t>(MessageSeverity::kInformational));
  EXPECT_EQ(3U, static_cast<uint8_t>(MessageSeverity::kTrace));
  EXPECT_EQ(4U, static_cast<uint8_t>(MessageSeverity::kDump));
}

TEST(ProtocolTest, OpcodesAreUnchanged) {
  using protocol::Opcode;
  EXPECT_EQ(2U, sizeof(Opcode));
  EXPECT_EQ(0U, static_cast<uint16_t>(Opcode::kWriteAsciiMessage));
  EXPECT_EQ(0x20U, static_cast<uint16_t>(Opcode::kDumpBuffers));
}

}  // namespace wifilogd
}  // namespace android
