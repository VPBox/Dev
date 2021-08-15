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

namespace test_vendor_lib {
namespace net {

// Starts a server and calls the registered lambda for each connection.
class ServerPortFactory {
 public:
  // Opens a server port and sets the listen file descriptor.
  ServerPortFactory(int port, std::function<void(int fd)>& on_connection);

  // Closes the port (if succesfully opened in SetUp).
  ~ServerPortFactory() {}

  // Waits for a connection request and returns the file descriptor to watch.
  // Returns -1 on an error.
  void Accept(int listen_fd);

 private:
  std::function<void(int fd)> on_connection_;

  int port;
  int listen_fd_ = -1;

  ServerPortFactory(const ServerPortFactory&) = delete;
  ServerPortFactory& operator=(const ServerPortFactory&) = delete;
};

}  // namespace net
}  // namespace test_vendor_lib
