/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include <string.h>

#include <algorithm>
#include <cinttypes>
#include <string>
#include <tuple>
#include <utility>

#include "android-base/logging.h"
#include "android-base/stringprintf.h"

#include "wifilogd/byte_buffer.h"
#include "wifilogd/command_processor.h"
#include "wifilogd/local_utils.h"
#include "wifilogd/memory_reader.h"
#include "wifilogd/protocol.h"

namespace android {
namespace wifilogd {

using ::android::base::unique_fd;

using local_utils::CopyFromBufferOrDie;
using local_utils::GetMaxVal;

namespace {

uint32_t NsecToUsec(uint32_t nsec);

class TimestampHeader {
 public:
  TimestampHeader& set_since_boot_awake_only(Os::Timestamp new_value) {
    since_boot_awake_only = new_value;
    return *this;
  }

  TimestampHeader& set_since_boot_with_sleep(Os::Timestamp new_value) {
    since_boot_with_sleep = new_value;
    return *this;
  }

  TimestampHeader& set_since_epoch(Os::Timestamp new_value) {
    since_epoch = new_value;
    return *this;
  }

  // Returns a string with a formatted representation of the timestamps
  // contained within this header.
  std::string ToString() const {
    const auto& awake_time = since_boot_awake_only;
    const auto& up_time = since_boot_with_sleep;
    const auto& wall_time = since_epoch;
    return base::StringPrintf("%" PRIu32 ".%06" PRIu32
                              " "
                              "%" PRIu32 ".%06" PRIu32
                              " "
                              "%" PRIu32 ".%06" PRIu32,
                              awake_time.secs, NsecToUsec(awake_time.nsecs),
                              up_time.secs, NsecToUsec(up_time.nsecs),
                              wall_time.secs, NsecToUsec(wall_time.nsecs));
  }

  Os::Timestamp since_boot_awake_only;
  Os::Timestamp since_boot_with_sleep;
  Os::Timestamp since_epoch;  // non-monotonic
};

constexpr char kUnprintableCharReplacement = '?';

std::string MakeSanitizedString(const uint8_t* buf, size_t buf_len);

std::string GetStringFromMemoryReader(NONNULL MemoryReader* buffer_reader,
                                      const size_t desired_len) {
  constexpr char kBufferOverrunError[] = "[buffer-overrun]";
  constexpr char kZeroLengthError[] = "[empty]";
  if (!desired_len) {
    // TODO(b/32098735): Increment stats counter.
    return kZeroLengthError;
  }

  auto effective_len = desired_len;
  if (buffer_reader->size() < effective_len) {
    // TODO(b/32098735): Increment stats counter.
    effective_len = buffer_reader->size();
  }

  auto out = MakeSanitizedString(buffer_reader->GetBytesOrDie(effective_len),
                                 effective_len);
  if (effective_len < desired_len) {
    out += kBufferOverrunError;
  }

  return out;
}

std::string MakeSanitizedString(const uint8_t* buf, size_t buf_len) {
  std::string out(buf_len, '\0');
  std::replace_copy_if(buf, buf + buf_len, &out.front(),
                       [](auto c) { return !local_utils::IsAsciiPrintable(c); },
                       kUnprintableCharReplacement);
  return out;
}

uint32_t NsecToUsec(uint32_t nsec) { return nsec / 1000; }

}  // namespace

CommandProcessor::CommandProcessor(size_t buffer_size_bytes)
    : current_log_buffer_(buffer_size_bytes), os_(new Os()) {}

CommandProcessor::CommandProcessor(size_t buffer_size_bytes,
                                   std::unique_ptr<Os> os)
    : current_log_buffer_(buffer_size_bytes), os_(std::move(os)) {}

CommandProcessor::~CommandProcessor() {}

bool CommandProcessor::ProcessCommand(const void* input_buffer,
                                      size_t n_bytes_read, int fd) {
  unique_fd wrapped_fd(fd);

  if (n_bytes_read < sizeof(protocol::Command)) {
    // TODO(b/32098735): Increment stats counter.
    return false;
  }

  const auto& command_header =
      CopyFromBufferOrDie<protocol::Command>(input_buffer, n_bytes_read);
  switch (command_header.opcode) {
    using protocol::Opcode;
    case Opcode::kWriteAsciiMessage:
      // Copy the entire command to the log. This defers the cost of
      // validating the rest of the CommandHeader until we dump the
      // message.
      //
      // Note that most messages will be written but never read. So, in
      // the common case, the validation cost is actually eliminated,
      // rather than just deferred.
      return CopyCommandToLog(input_buffer, n_bytes_read);
    case Opcode::kDumpBuffers:
      return Dump(std::move(wrapped_fd));
  }

  LOG(DEBUG) << "Received unexpected opcode "
             << local_utils::CastEnumToInteger(command_header.opcode);
  // TODO(b/32098735): Increment stats counter.
  return false;
}

// Private methods below.

bool CommandProcessor::CopyCommandToLog(const void* command_buffer,
                                        size_t command_len_in) {
  const uint16_t command_len =
      SAFELY_CLAMP(command_len_in, uint16_t, 0, protocol::kMaxMessageSize);

  uint16_t total_size;
  static_assert(GetMaxVal(total_size) > sizeof(TimestampHeader) &&
                    GetMaxVal(total_size) - sizeof(TimestampHeader) >=
                        protocol::kMaxMessageSize,
                "total_size cannot represent some input messages");
  total_size = sizeof(TimestampHeader) + command_len;
  CHECK(current_log_buffer_.CanFitEver(total_size));

  if (!current_log_buffer_.CanFitNow(total_size)) {
    // TODO(b/31867689):
    // 1. compress current buffer
    // 2. move old buffer to linked list
    // 3. prune old buffers, if needed
    current_log_buffer_.Clear();
  }
  CHECK(current_log_buffer_.CanFitNow(total_size));

  const auto tstamp_header =
      TimestampHeader()
          .set_since_boot_awake_only(os_->GetTimestamp(CLOCK_MONOTONIC))
          .set_since_boot_with_sleep(os_->GetTimestamp(CLOCK_BOOTTIME))
          .set_since_epoch(os_->GetTimestamp(CLOCK_REALTIME));
  const auto message_buf =
      ByteBuffer<sizeof(TimestampHeader) + protocol::kMaxMessageSize>()
          .AppendOrDie(&tstamp_header, sizeof(tstamp_header))
          .AppendOrDie(command_buffer, command_len);
  bool did_write = current_log_buffer_.Append(
      message_buf.data(),
      SAFELY_CLAMP(message_buf.size(), uint16_t, 0, GetMaxVal<uint16_t>()));
  if (!did_write) {
    // Given that we checked for enough free space, Append() should
    // have succeeded. Hence, a failure here indicates a logic error,
    // rather than a runtime error.
    LOG(FATAL) << "Unexpected failure to Append()";
  }

  return true;
}

bool CommandProcessor::Dump(unique_fd dump_fd) {
  MessageBuffer::ScopedRewinder rewinder(&current_log_buffer_);
  while (auto buffer_reader =
             MemoryReader(current_log_buffer_.ConsumeNextMessage())) {
    const auto& tstamp_header = buffer_reader.CopyOutOrDie<TimestampHeader>();
    const auto& command_header =
        buffer_reader.CopyOutOrDie<protocol::Command>();

    // TOOO(b/32256098): validate |buffer_reader.size()| against payload_len,
    // and use a smaller size if necessary. Update a stats counter if
    // payload_len and
    // buflen do not match.

    std::string output_string = tstamp_header.ToString();
    switch (command_header.opcode) {
      using protocol::Opcode;
      case Opcode::kWriteAsciiMessage:
        output_string += ' ' + FormatAsciiMessage(buffer_reader);
        break;
      case Opcode::kDumpBuffers:
        LOG(FATAL) << "Unexpected DumpBuffers command in log";
    }
    output_string += '\n';

    size_t n_written;
    Os::Errno err;
    std::tie(n_written, err) =
        os_->Write(dump_fd, output_string.data(), output_string.size());
    if (err == EINTR) {
      // The next write will probably succeed. We dont't retry the current
      // message, however, because we want to guarantee forward progress.
      //
      // TODO(b/32098735): Increment a counter, and attempt to output that
      // counter after we've dumped all the log messages.
    } else if (err) {
      // Any error other than EINTR is considered unrecoverable.
      LOG(ERROR) << "Terminating log dump, due to " << strerror(err);
      return false;
    }
  }

  return true;
}

std::string CommandProcessor::FormatAsciiMessage(MemoryReader buffer_reader) {
  constexpr char kShortHeaderError[] = "[truncated-header]";
  if (buffer_reader.size() < sizeof(protocol::AsciiMessage)) {
    // TODO(b/32098735): Increment stats counter.
    return kShortHeaderError;
  }

  const auto& ascii_message_header =
      buffer_reader.CopyOutOrDie<protocol::AsciiMessage>();
  const std::string& formatted_tag =
      GetStringFromMemoryReader(&buffer_reader, ascii_message_header.tag_len);
  const std::string& formatted_msg =
      GetStringFromMemoryReader(&buffer_reader, ascii_message_header.data_len);
  return formatted_tag + ' ' + formatted_msg;
}

}  // namespace wifilogd
}  // namespace android
