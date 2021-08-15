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

#include <array>
#include <cstdint>
#include <cstring>
#include <memory>
#include <utility>

#include "android-base/logging.h"

#include "wifilogd/main_loop.h"
#include "wifilogd/protocol.h"

namespace android {
namespace wifilogd {

namespace {
constexpr auto kMainBufferSizeBytes = 128 * 1024;
// TODO(b/32840641): Tune the sleep time.
constexpr auto kTransientErrorSleepTimeNsec = 100 * 1000;  // 100 usec
}

MainLoop::MainLoop(const std::string& socket_name)
    : MainLoop(socket_name, std::make_unique<Os>(),
               std::make_unique<CommandProcessor>(kMainBufferSizeBytes)) {}

MainLoop::MainLoop(const std::string& socket_name, std::unique_ptr<Os> os,
                   std::unique_ptr<CommandProcessor> command_processor)
    : os_(std::move(os)), command_processor_(std::move(command_processor)) {
  Os::Errno err;
  std::tie(sock_fd_, err) = os_->GetControlSocket(socket_name);
  if (err) {
    PLOG(FATAL) << "Failed to get control socket";
  }
}

void MainLoop::RunOnce() {
  std::array<uint8_t, protocol::kMaxMessageSize> input_buf;
  size_t datagram_len;
  Os::Errno err;
  std::tie(datagram_len, err) =
      os_->ReceiveDatagram(sock_fd_, input_buf.data(), input_buf.size());
  if (err) {
    ProcessError(err);
    return;
  }

  if (datagram_len > protocol::kMaxMessageSize) {
    // TODO(b/32098735): Increment stats counter.
    datagram_len = protocol::kMaxMessageSize;
  }

  command_processor_->ProcessCommand(input_buf.data(), datagram_len,
                                     Os::kInvalidFd);
}

// Private methods below.

void MainLoop::ProcessError(Os::Errno err) {
  if (err == EINTR || err == ENOMEM) {
    // TODO(b/32098735): Increment stats counter.
    os_->Nanosleep(kTransientErrorSleepTimeNsec);
    return;
  }

  // Any other error is unexpected, and assumed to be non-recoverable.
  // (If, e.g., our socket is in a bad state, then we won't be able to receive
  // any new log messages.)
  PLOG(FATAL) << "Unexpected error";
}

}  // namespace wifilogd
}  // namespace android
