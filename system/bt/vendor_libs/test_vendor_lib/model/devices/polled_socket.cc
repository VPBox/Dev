/*
 * Copyright 2018 The Android Open Source Project
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

#define LOG_TAG "polled_socket"

#include "polled_socket.h"

#include <base/logging.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>

#include "osi/include/log.h"

namespace test_vendor_lib {
namespace net {

PolledSocket::PolledSocket(int file_descriptor) : file_descriptor_(file_descriptor) {}

PolledSocket::PolledSocket(PolledSocket&& p) : file_descriptor_(p.file_descriptor_) {
  p.file_descriptor_ = -1;
}

PolledSocket::~PolledSocket() {
  CleanUp();
}

void PolledSocket::CleanUp() {
  if (file_descriptor_ != -1) {
    WHILE_EINTR(close(file_descriptor_));
  }
  file_descriptor_ = -1;
}

size_t PolledSocket::TrySend(packets::PacketView<true> packet) {
  if (file_descriptor_ == -1) {
    return 0;
  }
  // Could skip this copy if the packet is guaranteed to be contiguous.
  std::vector<uint8_t> copy;
  copy.reserve(packet.size());
  for (const auto&& c : packet) {
    copy.push_back(c);
  }
  int ret = write(file_descriptor_, copy.data(), copy.size());
  if (ret == -1) {
    ALOGW("%s error %s", __func__, strerror(errno));
    return 0;
  } else {
    return static_cast<size_t>(ret);
  }
}

/*
void PolledSocket::TrySendVector(
  const std::vector<std::vector<uint8_t>&>& raw_vectors) {
if (file_descriptor_ < 0) {
  return;
}
for (const std::vector<uint8_t>& v : raw_vectors) {
  Send(v);
}
  std::vector<struct iovec> iovecs;
  for (auto v : raw_vectors) {
    struct iovec one_iovec;
    one_iovec.iov_base = v.data();
    one_iovec.iov_base = v.size();
    iovecs.push_back(one_iovec);
  }
  int ret = writev(file_descriptor_, iovecs.data(), iovecs.size());
  if (ret == -1) {
    return 0;
  } else {
    return static_cast<size_t>(ret);
  }
}
*/

size_t PolledSocket::TryReceive(size_t num_bytes, uint8_t* data) {
  if (file_descriptor_ == -1) return 0;
  int ret;
  WHILE_EINTR(ret = read(file_descriptor_, data, num_bytes));
  if (ret < 0) {
    if (errno == EAGAIN) {
      return 0;
    } else {
      ALOGW("%s error %s", __func__, strerror(errno));
      CleanUp();
      return 0;
    }
  }
  return ret;
}

}  // namespace net
}  // namespace test_vendor_lib
