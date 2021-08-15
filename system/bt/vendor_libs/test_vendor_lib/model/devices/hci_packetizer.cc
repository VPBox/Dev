//
// Copyright 2017 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "hci_packetizer.h"

#define LOG_TAG "hci_packetizer"

#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <log/log.h>
#include <unistd.h>

namespace test_vendor_lib {
namespace hci {
constexpr size_t HciPacketizer::COMMAND_PREAMBLE_SIZE;
constexpr size_t HciPacketizer::COMMAND_LENGTH_OFFSET;
constexpr size_t HciPacketizer::ACL_PREAMBLE_SIZE;
constexpr size_t HciPacketizer::ACL_LENGTH_OFFSET;
constexpr size_t HciPacketizer::SCO_PREAMBLE_SIZE;
constexpr size_t HciPacketizer::SCO_LENGTH_OFFSET;
constexpr size_t HciPacketizer::EVENT_PREAMBLE_SIZE;
constexpr size_t HciPacketizer::EVENT_LENGTH_OFFSET;

constexpr size_t HciPacketizer::PREAMBLE_SIZE_MAX;

size_t HciPacketizer::HciGetPacketLengthForType(hci::PacketType type, const uint8_t* preamble) {
  static const size_t packet_length_offset[static_cast<size_t>(hci::PacketType::EVENT) + 1] = {
      0,
      HciPacketizer::COMMAND_LENGTH_OFFSET,
      HciPacketizer::ACL_LENGTH_OFFSET,
      HciPacketizer::SCO_LENGTH_OFFSET,
      HciPacketizer::EVENT_LENGTH_OFFSET,
  };

  size_t offset = packet_length_offset[static_cast<size_t>(type)];
  if (type != hci::PacketType::ACL) return preamble[offset];
  return (((preamble[offset + 1]) << 8) | preamble[offset]);
}

const std::vector<uint8_t>& HciPacketizer::GetPacket() const {
  return packet_;
}

void HciPacketizer::OnDataReady(int fd, hci::PacketType packet_type) {
  static const size_t preamble_size[static_cast<size_t>(hci::PacketType::EVENT) + 1] = {
      0,
      HciPacketizer::COMMAND_PREAMBLE_SIZE,
      HciPacketizer::ACL_PREAMBLE_SIZE,
      HciPacketizer::SCO_PREAMBLE_SIZE,
      HciPacketizer::EVENT_PREAMBLE_SIZE,
  };

  switch (state_) {
    case HCI_PREAMBLE: {
      ssize_t bytes_read = TEMP_FAILURE_RETRY(
          read(fd, preamble_ + bytes_read_, preamble_size[static_cast<uint8_t>(packet_type)] - bytes_read_));
      if (bytes_read == 0) {
        ALOGE("%s: Will try again to read the header!", __func__);
        return;
      }
      if (bytes_read < 0) {
        // Ignore temporary failures.
        if (errno == EAGAIN) {
          return;
        }
        LOG_ALWAYS_FATAL("%s: Read header error: %s", __func__, strerror(errno));
      }
      bytes_read_ += bytes_read;
      if (bytes_read_ == preamble_size[static_cast<uint8_t>(packet_type)]) {
        size_t packet_length = HciGetPacketLengthForType(packet_type, preamble_);
        packet_.resize(preamble_size[static_cast<uint8_t>(packet_type)] + packet_length);
        memcpy(packet_.data(), preamble_, preamble_size[static_cast<uint8_t>(packet_type)]);
        ALOGI("%s: Read a preamble of size %d length %d %0x %0x %0x", __func__, static_cast<int>(bytes_read_),
              static_cast<int>(packet_length), preamble_[0], preamble_[1], preamble_[2]);
        bytes_remaining_ = packet_length;
        if (bytes_remaining_ == 0) {
          packet_ready_cb_();
          state_ = HCI_PREAMBLE;
          bytes_read_ = 0;
        } else {
          state_ = HCI_PAYLOAD;
          bytes_read_ = 0;
        }
      }
      break;
    }

    case HCI_PAYLOAD: {
      ssize_t bytes_read = TEMP_FAILURE_RETRY(
          read(fd, packet_.data() + preamble_size[static_cast<uint8_t>(packet_type)] + bytes_read_, bytes_remaining_));
      if (bytes_read == 0) {
        ALOGI("%s: Will try again to read the payload!", __func__);
        return;
      }
      if (bytes_read < 0) {
        // Ignore temporary failures.
        if (errno == EAGAIN) {
          return;
        }
        LOG_ALWAYS_FATAL("%s: Read payload error: %s", __func__, strerror(errno));
      }
      bytes_remaining_ -= bytes_read;
      bytes_read_ += bytes_read;
      if (bytes_remaining_ == 0) {
        packet_ready_cb_();
        state_ = HCI_PREAMBLE;
        bytes_read_ = 0;
      }
      break;
    }
  }
}

}  // namespace hci
}  // namespace test_vendor_lib
