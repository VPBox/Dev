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

#include <errno.h>
#include <unistd.h>

#include <algorithm>
#include <cstring>
#include <memory>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

#include "android-base/unique_fd.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "wifilogd/byte_buffer.h"
#include "wifilogd/local_utils.h"
#include "wifilogd/protocol.h"
#include "wifilogd/tests/mock_os.h"

#include "wifilogd/command_processor.h"

namespace android {
namespace wifilogd {
namespace {

using ::android::base::unique_fd;
using ::testing::_;
using ::testing::AnyNumber;
using ::testing::AtLeast;
using ::testing::EndsWith;
using ::testing::HasSubstr;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::StartsWith;
using ::testing::StrictMock;
using local_utils::GetMaxVal;

// The CommandBuffer is deliberately larger than the maximal permitted
// command, so that we can test the CommandProcessor's handling of oversized
// inputs.
using CommandBuffer = ByteBuffer<protocol::kMaxMessageSize * 2>;

constexpr size_t kBufferSizeBytes = protocol::kMaxMessageSize * 16;
constexpr char kLogRecordSeparator = '\n';
constexpr size_t kMaxAsciiMessagePayloadLen = protocol::kMaxMessageSize -
                                              sizeof(protocol::Command) -
                                              sizeof(protocol::AsciiMessage);

class CommandProcessorTest : public ::testing::Test {
 public:
  CommandProcessorTest() {
    os_ = new StrictMock<MockOs>();
    auto& accumulator = written_to_os_;
    ON_CALL(*os_, Write(_, _, _))
        .WillByDefault(Invoke(
            [&accumulator](int /*fd*/, const void* write_buf, size_t buflen) {
              accumulator.append(static_cast<const char*>(write_buf), buflen);
              return std::tuple<size_t, Os::Errno>(buflen, 0);
            }));
    command_processor_ = std::unique_ptr<CommandProcessor>(
        new CommandProcessor(kBufferSizeBytes, std::unique_ptr<Os>(os_)));
  }

 protected:
  CommandBuffer BuildAsciiMessageCommandWithAdjustments(
      const std::string& tag, const std::string& message,
      ssize_t command_payload_len_adjustment,
      ssize_t ascii_message_tag_len_adjustment,
      ssize_t ascii_message_data_len_adjustment) {
    const size_t adjusted_tag_len =
        tag.length() + ascii_message_tag_len_adjustment;
    const size_t adjusted_data_len =
        message.length() + ascii_message_data_len_adjustment;
    const auto ascii_message_header =
        protocol::AsciiMessage()
            .set_tag_len(SAFELY_CLAMP(
                adjusted_tag_len, uint8_t, 0,
                GetMaxVal<decltype(protocol::AsciiMessage::tag_len)>()))
            .set_data_len(SAFELY_CLAMP(
                adjusted_data_len, uint16_t, 0,
                GetMaxVal<decltype(protocol::AsciiMessage::data_len)>()))
            .set_severity(protocol::MessageSeverity::kError);
    EXPECT_EQ(adjusted_tag_len, ascii_message_header.tag_len);
    EXPECT_EQ(adjusted_data_len, ascii_message_header.data_len);

    const size_t payload_len = sizeof(ascii_message_header) + tag.length() +
                               message.length() +
                               command_payload_len_adjustment;
    const auto command =
        protocol::Command()
            .set_opcode(protocol::Opcode::kWriteAsciiMessage)
            .set_payload_len(SAFELY_CLAMP(
                payload_len, uint16_t, 0,
                GetMaxVal<decltype(protocol::Command::payload_len)>()));
    EXPECT_EQ(payload_len, command.payload_len);
    return CommandBuffer()
        .AppendOrDie(&command, sizeof(command))
        .AppendOrDie(&ascii_message_header, sizeof(ascii_message_header))
        .AppendOrDie(tag.data(), tag.length())
        .AppendOrDie(message.data(), message.length());
  }

  CommandBuffer BuildAsciiMessageCommand(const std::string& tag,
                                         const std::string& message) {
    return BuildAsciiMessageCommandWithAdjustments(tag, message, 0, 0, 0);
  }

  bool SendAsciiMessageWithAdjustments(
      const std::string& tag, const std::string& message,
      ssize_t transport_len_adjustment, ssize_t command_payload_len_adjustment,
      ssize_t ascii_message_tag_len_adjustment,
      ssize_t ascii_message_data_len_adjustment) {
    const CommandBuffer& command_buffer(BuildAsciiMessageCommandWithAdjustments(
        tag, message, command_payload_len_adjustment,
        ascii_message_tag_len_adjustment, ascii_message_data_len_adjustment));
    EXPECT_CALL(*os_, GetTimestamp(CLOCK_MONOTONIC));
    EXPECT_CALL(*os_, GetTimestamp(CLOCK_BOOTTIME));
    EXPECT_CALL(*os_, GetTimestamp(CLOCK_REALTIME));
    return command_processor_->ProcessCommand(
        command_buffer.data(), command_buffer.size() + transport_len_adjustment,
        Os::kInvalidFd);
  }

  bool SendAsciiMessage(const std::string& tag, const std::string& message) {
    return SendAsciiMessageWithAdjustments(tag, message, 0, 0, 0, 0);
  }

  bool SendDumpBuffers() {
    const auto command = protocol::Command()
                             .set_opcode(protocol::Opcode::kDumpBuffers)
                             .set_payload_len(0);
    const auto buf = CommandBuffer().AppendOrDie(&command, sizeof(command));
    constexpr int kFakeFd = 100;
    return command_processor_->ProcessCommand(buf.data(), buf.size(), kFakeFd);
  }

  std::string written_to_os_;  // Must out-live |os_|
  std::unique_ptr<CommandProcessor> command_processor_;
  // We use a raw pointer to access the mock, since ownership passes
  // to |command_processor_|.
  StrictMock<MockOs>* os_;
};

}  // namespace

// A valid ASCII message should, of course, be processed successfully.
TEST_F(CommandProcessorTest, ProcessCommandOnValidAsciiMessageSucceeds) {
  EXPECT_TRUE(SendAsciiMessage("tag", "message"));
}

// If the buffer given to ProcessCommand() is shorter than a protocol::Command,
// then we discard the data.
TEST_F(CommandProcessorTest,
       ProcessCommandOnAsciiMessageShorterThanCommandFails) {
  const CommandBuffer& command_buffer(
      BuildAsciiMessageCommand("tag", "message"));
  EXPECT_FALSE(command_processor_->ProcessCommand(
      command_buffer.data(), sizeof(protocol::Command) - 1, Os::kInvalidFd));
}

// In all other cases, we save the data we got, and will try to salvage the
// contents when dumping.
TEST_F(CommandProcessorTest, ProcessCommandOnAsciiMessageWithEmtpyTagSucceeds) {
  EXPECT_TRUE(SendAsciiMessage("", "message"));
}

TEST_F(CommandProcessorTest,
       ProcessCommandOnAsciiMessageWithEmptyMessageSucceeds) {
  EXPECT_TRUE(SendAsciiMessage("tag", ""));
}

TEST_F(CommandProcessorTest,
       ProcessCommandOnAsciiMessageWithEmptyTagAndMessageSucceeds) {
  EXPECT_TRUE(SendAsciiMessage("", ""));
}

TEST_F(CommandProcessorTest,
       ProcessCommandOnAsciiMessageWithBadCommandLengthSucceeds) {
  EXPECT_TRUE(SendAsciiMessageWithAdjustments("tag", "message", 0, 1, 0, 0));
  EXPECT_TRUE(SendAsciiMessageWithAdjustments("tag", "message", 0, -1, 0, 0));
}

TEST_F(CommandProcessorTest,
       ProcessCommandOnAsciiMessageWithBadTagLengthSucceeds) {
  EXPECT_TRUE(SendAsciiMessageWithAdjustments("tag", "message", 0, 0, 1, 0));
  EXPECT_TRUE(SendAsciiMessageWithAdjustments("tag", "message", 0, 0, -1, 0));
}

TEST_F(CommandProcessorTest,
       ProcessCommandOnAsciiMessageWithBadMessageLengthSucceeds) {
  EXPECT_TRUE(SendAsciiMessageWithAdjustments("tag", "message", 0, 0, 0, 1));
  EXPECT_TRUE(SendAsciiMessageWithAdjustments("tag", "message", 0, 0, 0, -1));
}

TEST_F(CommandProcessorTest, ProcessCommandOnOverlyLargeAsciiMessageSucceeds) {
  const std::string tag{"tag"};
  EXPECT_TRUE(SendAsciiMessage(
      tag, std::string(kMaxAsciiMessagePayloadLen - tag.size() + 1, '.')));
}

TEST_F(CommandProcessorTest, ProcessCommandInvalidOpcodeReturnsFailure) {
  using opcode_enum_t = decltype(protocol::Command::opcode);
  using opcode_integral_t = std::underlying_type<opcode_enum_t>::type;
  constexpr auto invalid_opcode = GetMaxVal<opcode_integral_t>();

  const auto command =
      protocol::Command()
          .set_opcode(local_utils::CopyFromBufferOrDie<opcode_enum_t>(
              &invalid_opcode, sizeof(invalid_opcode)))
          .set_payload_len(0);
  const auto buf = CommandBuffer().AppendOrDie(&command, sizeof(command));
  constexpr int kFakeFd = 100;
  EXPECT_FALSE(
      command_processor_->ProcessCommand(buf.data(), buf.size(), kFakeFd));
}

TEST_F(CommandProcessorTest, ProcessCommandSucceedsEvenAfterFillingBuffer) {
  const std::string tag{"tag"};
  const std::string message(kMaxAsciiMessagePayloadLen - tag.size(), '.');
  for (size_t cumulative_payload_bytes = 0;
       cumulative_payload_bytes <= kBufferSizeBytes;
       cumulative_payload_bytes += (tag.size() + message.size())) {
    EXPECT_TRUE(SendAsciiMessage(tag, message));
  }
}

TEST_F(CommandProcessorTest,
       ProcessCommandDumpBuffersOutputIncludesCorrectlyFormattedTimestamps) {
  const CommandBuffer& command_buf(BuildAsciiMessageCommand("tag", "message"));
  EXPECT_CALL(*os_, GetTimestamp(CLOCK_MONOTONIC))
      .WillOnce(Return(Os::Timestamp{0, 999}));
  EXPECT_CALL(*os_, GetTimestamp(CLOCK_BOOTTIME))
      .WillOnce(Return(Os::Timestamp{1, 1000}));
  EXPECT_CALL(*os_, GetTimestamp(CLOCK_REALTIME))
      .WillOnce(Return(Os::Timestamp{123456, 123456000}));
  EXPECT_TRUE(command_processor_->ProcessCommand(
      command_buf.data(), command_buf.size(), Os::kInvalidFd));

  EXPECT_CALL(*os_, Write(_, _, _)).Times(AtLeast(1));
  EXPECT_TRUE(SendDumpBuffers());
  EXPECT_THAT(written_to_os_, StartsWith("0.000000 1.000001 123456.123456"));
}

TEST_F(CommandProcessorTest, ProcessCommandDumpBuffersSucceedsOnEmptyLog) {
  EXPECT_CALL(*os_, Write(_, _, _)).Times(0);
  EXPECT_TRUE(SendDumpBuffers());
}

TEST_F(CommandProcessorTest, ProcessCommandDumpBuffersIncludesAllMessages) {
  constexpr int kNumMessages = 5;
  for (size_t i = 0; i < kNumMessages; ++i) {
    ASSERT_TRUE(SendAsciiMessage("tag", "message"));
  }

  EXPECT_CALL(*os_, Write(_, _, _)).Times(AtLeast(1));
  EXPECT_TRUE(SendDumpBuffers());
  EXPECT_EQ(kNumMessages,
            std::count(written_to_os_.begin(), written_to_os_.end(),
                       kLogRecordSeparator));
}

TEST_F(CommandProcessorTest,
       ProcessCommandDumpBuffersAsciiMessageIncludesTagAndMessage) {
  ASSERT_TRUE(SendAsciiMessage("tag", "message"));
  EXPECT_CALL(*os_, Write(_, _, _)).Times(AtLeast(1));
  ASSERT_TRUE(SendDumpBuffers());
  EXPECT_THAT(written_to_os_, EndsWith("tag message\n"));
}

TEST_F(CommandProcessorTest,
       ProcessCommandDumpBuffersAsciiMessageHandlesEmptyTag) {
  ASSERT_TRUE(SendAsciiMessage("", "message"));
  EXPECT_CALL(*os_, Write(_, _, _)).Times(AtLeast(1));
  ASSERT_TRUE(SendDumpBuffers());
  EXPECT_THAT(written_to_os_, EndsWith("[empty] message\n"));
}

TEST_F(CommandProcessorTest,
       ProcessCommandDumpBuffersAsciiMessageHandlesEmptyMessage) {
  ASSERT_TRUE(SendAsciiMessage("tag", ""));
  EXPECT_CALL(*os_, Write(_, _, _)).Times(AtLeast(1));
  ASSERT_TRUE(SendDumpBuffers());
  EXPECT_THAT(written_to_os_, EndsWith("tag [empty]\n"));
}

TEST_F(CommandProcessorTest,
       ProcessCommandDumpBuffersAsciiMessageHandlesEmptyTagAndEmptyMessage) {
  ASSERT_TRUE(SendAsciiMessage("", ""));
  EXPECT_CALL(*os_, Write(_, _, _)).Times(AtLeast(1));
  ASSERT_TRUE(SendDumpBuffers());
  EXPECT_THAT(written_to_os_, EndsWith("[empty] [empty]\n"));
}

TEST_F(CommandProcessorTest,
       ProcessCommandDumpBuffersAsciiMessageSanitizesUnprintableChars) {
  ASSERT_TRUE(SendAsciiMessage("\xfftag\xff", "\xffmessage\xff"));
  EXPECT_CALL(*os_, Write(_, _, _)).Times(AtLeast(1));
  ASSERT_TRUE(SendDumpBuffers());
  EXPECT_THAT(written_to_os_, EndsWith("?tag? ?message?\n"));
}

TEST_F(
    CommandProcessorTest,
    ProcessCommandDumpBuffersAsciiMessageHandlesMessageTooShortForAsciiMessage) {  // NOLINT(whitespace/line_length)
  ASSERT_TRUE(SendAsciiMessageWithAdjustments("", "", -1, 0, 0, 0));
  EXPECT_CALL(*os_, Write(_, _, _)).Times(AtLeast(1));
  ASSERT_TRUE(SendDumpBuffers());
  EXPECT_THAT(written_to_os_, EndsWith("[truncated-header]\n"));
}

TEST_F(CommandProcessorTest,
       ProcessCommandDumpBuffersAsciiMessageHandlesMessageTooShortForTagStart) {
  constexpr char kTag[] = "tag";
  constexpr char kMessage[] = "message";
  ASSERT_TRUE(SendAsciiMessageWithAdjustments(
      kTag, kMessage, -(std::strlen(kTag) + std::strlen(kMessage)), 0, 0, 0));
  EXPECT_CALL(*os_, Write(_, _, _)).Times(AtLeast(1));
  ASSERT_TRUE(SendDumpBuffers());
  EXPECT_THAT(written_to_os_, EndsWith("[buffer-overrun] [buffer-overrun]\n"));
}

TEST_F(CommandProcessorTest,
       ProcessCommandDumpBuffersAsciiMessageHandlesMessageTooShortForTagEnd) {
  constexpr char kTag[] = "tag";
  constexpr char kMessage[] = "message";
  ASSERT_TRUE(SendAsciiMessageWithAdjustments(
      kTag, kMessage, -(1 + std::strlen(kMessage)), 0, 0, 0));
  EXPECT_CALL(*os_, Write(_, _, _)).Times(AtLeast(1));
  ASSERT_TRUE(SendDumpBuffers());
  EXPECT_THAT(written_to_os_,
              EndsWith("ta[buffer-overrun] [buffer-overrun]\n"));
}

TEST_F(
    CommandProcessorTest,
    ProcessCommandDumpBuffersAsciiMessageHandlesMessageTooShortForLogMessageStart) {  // NOLINT(whitespace/line_length)
  constexpr char kTag[] = "tag";
  constexpr char kMessage[] = "message";
  ASSERT_TRUE(SendAsciiMessageWithAdjustments(kTag, kMessage,
                                              -std::strlen(kMessage), 0, 0, 0));
  EXPECT_CALL(*os_, Write(_, _, _)).Times(AtLeast(1));
  ASSERT_TRUE(SendDumpBuffers());
  EXPECT_THAT(written_to_os_, EndsWith("tag [buffer-overrun]\n"));
}

TEST_F(
    CommandProcessorTest,
    ProcessCommandDumpBuffersAsciiMessageHandlesMessageTooShortForLogMessageEnd) {  // NOLINT(whitespace/line_length)
  ASSERT_TRUE(SendAsciiMessageWithAdjustments("tag", "message", -1, 0, 0, 0));
  EXPECT_CALL(*os_, Write(_, _, _)).Times(AtLeast(1));
  ASSERT_TRUE(SendDumpBuffers());
  EXPECT_THAT(written_to_os_, EndsWith("tag messag[buffer-overrun]\n"));
}

TEST_F(CommandProcessorTest,
       ProcessCommandDumpBuffersAsciiMessageHandlesMessageTooLongForTag) {
  ASSERT_TRUE(SendAsciiMessageWithAdjustments("tag", "", 100, 0, 0, 0));
  EXPECT_CALL(*os_, Write(_, _, _)).Times(AtLeast(1));
  ASSERT_TRUE(SendDumpBuffers());
  EXPECT_THAT(written_to_os_, EndsWith("tag [empty]\n"));
}

TEST_F(CommandProcessorTest,
       ProcessCommandDumpBuffersAsciiMessageHandlesMessageTooLongForMessage) {
  ASSERT_TRUE(SendAsciiMessageWithAdjustments("tag", "message", 100, 0, 0, 0));
  EXPECT_CALL(*os_, Write(_, _, _)).Times(AtLeast(1));
  ASSERT_TRUE(SendDumpBuffers());
  EXPECT_THAT(written_to_os_, EndsWith("tag message\n"));
}

TEST_F(CommandProcessorTest, ProcessCommandDumpBuffersStopsAfterFirstError) {
  ASSERT_TRUE(SendAsciiMessage("tag", "message"));
  ASSERT_TRUE(SendAsciiMessage("tag", "message"));

  EXPECT_CALL(*os_, Write(_, _, _))
      .WillOnce(Return(std::tuple<size_t, Os::Errno>{-1, EBADF}));
  ASSERT_FALSE(SendDumpBuffers());
}

TEST_F(CommandProcessorTest, ProcessCommandDumpBuffersContinuesPastEintr) {
  constexpr int kNumMessages = 5;
  for (size_t i = 0; i < kNumMessages; ++i) {
    ASSERT_TRUE(SendAsciiMessage("tag", "message"));
  }

  std::string written_to_os;
  EXPECT_CALL(*os_, Write(_, _, _))
      .WillRepeatedly(Invoke(
          [&written_to_os](int /*fd*/, const void* write_buf, size_t buflen) {
            written_to_os.append(static_cast<const char*>(write_buf), buflen);
            return std::tuple<size_t, Os::Errno>{buflen / 2, EINTR};
          }));
  EXPECT_TRUE(SendDumpBuffers());
  EXPECT_EQ(kNumMessages, std::count(written_to_os.begin(), written_to_os.end(),
                                     kLogRecordSeparator));
}

TEST_F(CommandProcessorTest, ProcessCommandDumpBuffersIsIdempotent) {
  ASSERT_TRUE(SendAsciiMessage("tag", "message"));

  EXPECT_CALL(*os_, Write(_, _, _)).Times(AtLeast(1));
  ASSERT_TRUE(SendDumpBuffers());
  ASSERT_GT(written_to_os_.size(), 0U);
  written_to_os_.clear();
  ASSERT_EQ(0U, written_to_os_.size());

  EXPECT_CALL(*os_, Write(_, _, _)).Times(AtLeast(1));
  EXPECT_TRUE(SendDumpBuffers());
  EXPECT_GT(written_to_os_.size(), 0U);
}

TEST_F(CommandProcessorTest,
       ProcessCommandDumpBuffersIsIdempotentEvenWithWriteFailure) {
  ASSERT_TRUE(SendAsciiMessage("tag", "message"));
  EXPECT_CALL(*os_, Write(_, _, _))
      .WillOnce(Return(std::tuple<size_t, Os::Errno>{-1, EBADF}));
  ASSERT_FALSE(SendDumpBuffers());
  ASSERT_EQ(0U, written_to_os_.size());

  EXPECT_CALL(*os_, Write(_, _, _)).Times(AtLeast(1));
  EXPECT_TRUE(SendDumpBuffers());
  EXPECT_GT(written_to_os_.size(), 0U);
}

// Strictly speaking, this is not a unit test. But there's no easy way to get
// unique_fd to call on an instance of our Os.
TEST_F(CommandProcessorTest, ProcessCommandClosesFd) {
  int pipe_fds[2];
  ASSERT_EQ(0, pipe(pipe_fds));

  const unique_fd our_fd{pipe_fds[0]};
  const int their_fd = pipe_fds[1];
  const CommandBuffer& command_buffer(
      BuildAsciiMessageCommand("tag", "message"));
  EXPECT_CALL(*os_, GetTimestamp(_)).Times(AnyNumber());
  EXPECT_TRUE(command_processor_->ProcessCommand(
      command_buffer.data(), command_buffer.size(), their_fd));
  EXPECT_EQ(-1, close(their_fd));
  EXPECT_EQ(EBADF, errno);
}

// Strictly speaking, this is not a unit test. But there's no easy way to get
// unique_fd to call on an instance of our Os.
TEST_F(CommandProcessorTest, ProcessCommandClosesFdEvenOnFailure) {
  int pipe_fds[2];
  ASSERT_EQ(0, pipe(pipe_fds));

  const unique_fd our_fd{pipe_fds[0]};
  const int their_fd = pipe_fds[1];
  const CommandBuffer command_buffer;
  EXPECT_FALSE(command_processor_->ProcessCommand(
      command_buffer.data(), command_buffer.size(), their_fd));
  EXPECT_EQ(-1, close(their_fd));
  EXPECT_EQ(EBADF, errno);
}

}  // namespace wifilogd
}  // namespace android
