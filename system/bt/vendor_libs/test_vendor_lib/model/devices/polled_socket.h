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

#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "packets/packet_view.h"

namespace test_vendor_lib {
namespace net {

#define WHILE_EINTR(fn) \
  do {                  \
  } while ((fn) == -1 && errno == EINTR)

class PolledSocket {
 public:
  PolledSocket(int file_descriptor);
  PolledSocket(PolledSocket&& p);
  virtual ~PolledSocket();

  size_t TrySend(packets::PacketView<true> packet);
  // size_t TrySendVector(const std::vector<const std::vector<uint8_t>&>& data);
  size_t TryReceive(size_t num_bytes, uint8_t* data);

 private:
  void CleanUp();
  int file_descriptor_{-1};
};

}  // namespace net
}  // namespace test_vendor_lib
