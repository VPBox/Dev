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

#ifndef COMMAND_PROCESSOR_H_
#define COMMAND_PROCESSOR_H_

#include <memory>
#include <string>

#include "android-base/macros.h"
#include "android-base/unique_fd.h"

#include "wifilogd/local_utils.h"
#include "wifilogd/memory_reader.h"
#include "wifilogd/message_buffer.h"
#include "wifilogd/os.h"

namespace android {
namespace wifilogd {

class CommandProcessor {
 public:
  // Constructs a CommandProcessor with a buffer of |buffer_size_bytes|.
  explicit CommandProcessor(size_t buffer_size_bytes);

  // Constructs a CommandProcessor with a buffer of |buffer_size_bytes|.
  // The CommandProcessor will use |os| to call into operating system services.
  // This method allows tests to provide a MockOs.
  CommandProcessor(size_t buffer_size_bytes, std::unique_ptr<Os> os);

  virtual ~CommandProcessor();

  // Processes the given command, with the given file descriptor. The effect of
  // this call depends on the contents of |input_buf|. In particular, depending
  // on the command, |fd| may be used for reading or writing, or |fd| may be
  // ignored. However, |fd| is guaranteed to be closed before ProcessCommand()
  // returns, in all cases.
  //
  // (Ideally, we might want to take |fd| as a unique_fd. Unfortunately,
  // GoogleMock doesn't deal well with move-only parameters.
  // https://github.com/google/googletest/issues/395)
  virtual bool ProcessCommand(NONNULL const void* input_buf,
                              size_t n_bytes_read, int fd);

 private:
  // Copies |command_buffer| into the log buffer. Returns true if the
  // command was copied. If |command_len| exceeds protocol::kMaxMessageSize,
  // copies the first protocol::kMaxMessageSize of |command_buffer|, and returns
  // true.
  bool CopyCommandToLog(NONNULL const void* command_buffer, size_t command_len);

  // Dumps all of the logged messages to |dump_fd|. Returns true unless
  // an unrecoverable error was encountered.
  bool Dump(::android::base::unique_fd dump_fd);

  // Returns a human-friendly string representation of the AsciiMessage
  // contained at the head of the memory referenced by |memory_reader|.
  // Validates that |memory_reader| has enough bytes to contain an AsciiMessage
  // header, and the payload described by that header. Reports any errors in
  // the returned string.
  std::string FormatAsciiMessage(MemoryReader memory_reader);

  // The MessageBuffer is inlined, since there's not much value to mocking
  // simple data objects. See Testing on the Toilet Episode 173.
  //
  // Note that the messages in |current_log_buffer_| have not been validated,
  // expect to ensure that:
  // a) each message starts with a TimestampHeader, and
  // b) each message is large enough for a protocol::Command to follow the
  //    TimestampHeader,and
  // c) the protocol::Command::opcode for each message is a supported opcode.
  MessageBuffer current_log_buffer_;
  const std::unique_ptr<Os> os_;

  DISALLOW_COPY_AND_ASSIGN(CommandProcessor);
};

}  // namespace wifilogd
}  // namespace android

#endif  // COMMAND_PROCESSOR_H_
