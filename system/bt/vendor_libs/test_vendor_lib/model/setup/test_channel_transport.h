/*
 * Copyright 2015 The Android Open Source Project
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

#include <functional>
#include <memory>
#include <string>
#include <vector>

//#include "base/files/scoped_file.h"

namespace test_vendor_lib {

// Manages communications between test channel and the controller. Mirrors the
// HciTransport for the test channel.
class TestChannelTransport {
 public:
  TestChannelTransport() {}

  ~TestChannelTransport() {}

  // Opens a port and returns the file descriptor for the socket.
  // Returns -1 on an error.
  int SetUp(int port);

  // Closes the port (if succesfully opened in SetUp).
  void CleanUp();

  // Waits for a connection request from the test channel program and
  // returns the file descriptor to watch for run-time parameters.
  // Returns -1 on an error.
  int Accept(int listen_fd);

  // Sets the callback that fires when data is read in WatchFd().
  void RegisterCommandHandler(const std::function<void(const std::string&, const std::vector<std::string>&)>& callback);

  // Send data back to the test channel.
  void SendResponse(int fd, const std::string&) const;

  void OnCommandReady(int fd, std::function<void(void)> unwatch);

 private:
  std::function<void(const std::string&, const std::vector<std::string>&)> command_handler_;

  int listen_fd_ = -1;

  TestChannelTransport(const TestChannelTransport& cmdPckt) = delete;
  TestChannelTransport& operator=(const TestChannelTransport& cmdPckt) = delete;
};

}  // namespace test_vendor_lib
