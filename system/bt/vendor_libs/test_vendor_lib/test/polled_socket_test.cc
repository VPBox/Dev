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

#include "polled_socket.h"

#include <gtest/gtest.h>
#include <cstdint>
#include <cstring>
#include <vector>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace test_vendor_lib {
namespace net {

class PolledSocketTest : public ::testing::Test {
 public:
  static const uint16_t kPort = 6124;
  static const size_t kBufferSize = 16;

 protected:
  void SetUp() override {
    server_.Initialize(kPort);
    client_.SetServerName("localhost");
    client_.Initialize(kPort);
  }

  void TearDown() override {}

  void ConnectClient() {}

  PolledSocketServer server_;
  PolledSocketClient client_;

 private:
  std::vector<uint8_t> server_buffer_;
  std::vector<uint8_t> client_buffer_;
};

TEST_F(PolledSocketTest, SetUpTearDown) {}

TEST_F(PolledSocketTest, TestOneConnection) {
  std::vector<uint8_t> tx_buf = {'1', '2', '3'};
  std::vector<uint8_t> rx_buf(10);

  client_.Send(tx_buf);
  server_.Receive(3, rx_buf.data());
  client_.Send(tx_buf);
  server_.Receive(3, rx_buf.data());
  EXPECT_EQ(rx_buf.size(), tx_buf.size());
}

#if 0
TEST_F(PolledSocketTest, TestRepeatedConnections) {
  static const int num_connections = 300;
  for (int i = 0; i < num_connections; i++) {
    int socket_cli_fd = ConnectClient();
    WriteFromClient(socket_cli_fd);
    AwaitServerResponse(socket_cli_fd);
    close(socket_cli_fd);
  }
}

TEST_F(PolledSocketTest, TestMultipleConnections) {
  static const int num_connections = 300;
  int socket_cli_fd[num_connections];
  for (int i = 0; i < num_connections; i++) {
    socket_cli_fd[i] = ConnectClient();
    EXPECT_TRUE(socket_cli_fd[i] > 0);
    WriteFromClient(socket_cli_fd[i]);
  }
  for (int i = 0; i < num_connections; i++) {
    AwaitServerResponse(socket_cli_fd[i]);
    close(socket_cli_fd[i]);
  }
}

#endif
}  // namespace net
}  // namespace test_vendor_lib
