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

#include "h4_packetizer.h"

#define LOG_TAG "h4_packetizer"

#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <log/log.h>
#include <sys/uio.h>
#include <unistd.h>

namespace test_vendor_lib {
namespace hci {
constexpr size_t H4Packetizer::COMMAND_PREAMBLE_SIZE;
constexpr size_t H4Packetizer::COMMAND_LENGTH_OFFSET;
constexpr size_t H4Packetizer::ACL_PREAMBLE_SIZE;
constexpr size_t H4Packetizer::ACL_LENGTH_OFFSET;
constexpr size_t H4Packetizer::SCO_PREAMBLE_SIZE;
constexpr size_t H4Packetizer::SCO_LENGTH_OFFSET;
constexpr size_t H4Packetizer::EVENT_PREAMBLE_SIZE;
constexpr size_t H4Packetizer::EVENT_LENGTH_OFFSET;

constexpr size_t H4Packetizer::PREAMBLE_SIZE_MAX;

size_t H4Packetizer::HciGetPacketLengthForType(hci::PacketType type, const uint8_t* preamble) {
  static const size_t packet_length_offset[static_cast<size_t>(hci::PacketType::EVENT) + 1] = {
      0,
      H4Packetizer::COMMAND_LENGTH_OFFSET,
      H4Packetizer::ACL_LENGTH_OFFSET,
      H4Packetizer::SCO_LENGTH_OFFSET,
      H4Packetizer::EVENT_LENGTH_OFFSET,
  };

  size_t offset = packet_length_offset[static_cast<size_t>(type)];
  if (type != hci::PacketType::ACL) return preamble[offset];
  return (((preamble[offset + 1]) << 8) | preamble[offset]);
}

H4Packetizer::H4Packetizer(int fd, PacketReadCallback command_cb, PacketReadCallback event_cb,
                           PacketReadCallback acl_cb, PacketReadCallback sco_cb)
    : uart_fd_(fd), command_cb_(command_cb), event_cb_(event_cb), acl_cb_(acl_cb), sco_cb_(sco_cb) {}

size_t H4Packetizer::Send(uint8_t type, const uint8_t* data, size_t length) {
  struct iovec iov[] = {{&type, sizeof(type)}, {const_cast<uint8_t*>(data), length}};
  ssize_t ret = 0;
  do {
    ret = TEMP_FAILURE_RETRY(writev(uart_fd_, iov, sizeof(iov) / sizeof(iov[0])));
  } while (-1 == ret && EAGAIN == errno);

  if (ret == -1) {
    ALOGE("%s error writing to UART (%s)", __func__, strerror(errno));
  } else if (ret < static_cast<ssize_t>(length + 1)) {
    ALOGE("%s: %d / %d bytes written - something went wrong...", __func__, static_cast<int>(ret),
          static_cast<int>(length + 1));
  }
  return ret;
}

void H4Packetizer::OnPacketReady() {
  ALOGE("%s: before switch", __func__);
  switch (hci_packet_type_) {
    case hci::PacketType::COMMAND:
      command_cb_(packet_);
      break;
    case hci::PacketType::ACL:
      acl_cb_(packet_);
      break;
    case hci::PacketType::SCO:
      sco_cb_(packet_);
      break;
    case hci::PacketType::EVENT:
      event_cb_(packet_);
      break;
    default:
      LOG_ALWAYS_FATAL("%s: Unimplemented packet type %d", __func__, static_cast<int>(hci_packet_type_));
  }
  // Get ready for the next type byte.
  hci_packet_type_ = hci::PacketType::UNKNOWN;
}

void H4Packetizer::OnDataReady(int fd) {
  if (hci_packet_type_ == hci::PacketType::UNKNOWN) {
    uint8_t buffer[1] = {0};
    ssize_t bytes_read = TEMP_FAILURE_RETRY(read(fd, buffer, 1));
    if (bytes_read != 1) {
      if (bytes_read == 0) {
        ALOGI("%s: Nothing ready, will retry!", __func__);
        return;
      } else if (bytes_read < 0) {
        if (errno == EAGAIN) {
          // No data, try again later.
          return;
        } else {
          LOG_ALWAYS_FATAL("%s: Read packet type error: %s", __func__, strerror(errno));
        }
      } else {
        LOG_ALWAYS_FATAL("%s: More bytes read than expected (%u)!", __func__, static_cast<unsigned int>(bytes_read));
      }
    }
    hci_packet_type_ = static_cast<hci::PacketType>(buffer[0]);
    if (hci_packet_type_ != hci::PacketType::ACL && hci_packet_type_ != hci::PacketType::SCO &&
        hci_packet_type_ != hci::PacketType::COMMAND && hci_packet_type_ != hci::PacketType::EVENT) {
      LOG_ALWAYS_FATAL("%s: Unimplemented packet type %d", __func__, static_cast<int>(hci_packet_type_));
    }
  } else {
    static const size_t preamble_size[static_cast<size_t>(hci::PacketType::EVENT) + 1] = {
        0,
        H4Packetizer::COMMAND_PREAMBLE_SIZE,
        H4Packetizer::ACL_PREAMBLE_SIZE,
        H4Packetizer::SCO_PREAMBLE_SIZE,
        H4Packetizer::EVENT_PREAMBLE_SIZE,
    };

    switch (state_) {
      case HCI_PREAMBLE: {
        size_t preamble_bytes = preamble_size[static_cast<size_t>(hci_packet_type_)];
        ssize_t bytes_read = TEMP_FAILURE_RETRY(read(fd, preamble_ + bytes_read_, preamble_bytes - bytes_read_));
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
        if (bytes_read_ == preamble_bytes) {
          size_t packet_length = HciGetPacketLengthForType(hci_packet_type_, preamble_);
          packet_.resize(preamble_bytes + packet_length);
          memcpy(packet_.data(), preamble_, preamble_bytes);
          ALOGI("%s: Read a preamble of size %d length %d %0x %0x %0x", __func__, static_cast<int>(bytes_read_),
                static_cast<int>(packet_length), preamble_[0], preamble_[1], preamble_[2]);
          bytes_remaining_ = packet_length;
          if (bytes_remaining_ == 0) {
            OnPacketReady();
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
        size_t preamble_bytes = preamble_size[static_cast<size_t>(hci_packet_type_)];
        ssize_t bytes_read =
            TEMP_FAILURE_RETRY(read(fd, packet_.data() + preamble_bytes + bytes_read_, bytes_remaining_));
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
          OnPacketReady();
          state_ = HCI_PREAMBLE;
          bytes_read_ = 0;
        }
        break;
      }
    }
  }
}

}  // namespace hci
}  // namespace test_vendor_lib
