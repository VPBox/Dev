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

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include <cstdint>

#include "cutils/sockets.h"

namespace android {
namespace wifilogd {
namespace protocol {

constexpr char kServiceSocketName[] = "wifilog";
constexpr char kServiceSocketPath[] = ANDROID_SOCKET_DIR "/wifilog";

// SOCK_DGRAM require a contiguous memory allocation [1]. Limit the message
// size to one normal page, to maximize reliability. (This size isn't a
// significant limitation as most messages will be much smaller. We don't
// want to go much smaller, though, as packet-fate blobs may be in the
// neighborhood of 2KB.)
//
// [1] http://stackoverflow.com/a/4822037
constexpr size_t kMaxMessageSize = 4096;

enum class Opcode : uint16_t {
  kWriteAsciiMessage,
  kDumpBuffers = 0x20,
};

enum class MessageSeverity : uint8_t {
  kError,
  kWarning,
  kInformational,
  kTrace,
  kDump,
};

struct Command {
  Command& set_opcode(Opcode new_opcode) {
    opcode = new_opcode;
    return *this;
  }

  Command& set_payload_len(uint16_t new_payload_len) {
    payload_len = new_payload_len;
    return *this;
  }

  uint64_t src_boottime_nsec;  // For latency measurement.
  // For drop detection. Sequence numbers are meaningful only within
  // the context of a single tag. (This is to minimize synchronization
  // requirements for multi-threaded clients.)
  uint16_t sequence_num;
  Opcode opcode;
  uint16_t payload_len;
  uint16_t reserved;  // Must be zero.
  // Payload follows, with content depending on |opcode|.
};

struct AsciiMessage {  // Old-style log messages.
  AsciiMessage& set_data_len(uint16_t new_data_len) {
    data_len = new_data_len;
    return *this;
  }

  AsciiMessage& set_tag_len(uint8_t new_tag_len) {
    tag_len = new_tag_len;
    return *this;
  }

  AsciiMessage& set_severity(MessageSeverity new_severity) {
    severity = new_severity;
    return *this;
  }

  uint16_t data_len;
  uint8_t tag_len;
  MessageSeverity severity;
  // Payload follows.
  // uint8_t tag[tag_len];
  // uint8_t data[data_len];
};

}  // namespace protocol
}  // namespace wifilogd
}  // namespace android

#endif  // PROTOCOL_H_
