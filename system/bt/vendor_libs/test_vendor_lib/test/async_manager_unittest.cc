/*
 * Copyright 2016 The Android Open Source Project
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

#include "model/setup/async_manager.h"
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

class AsyncManagerSocketTest : public ::testing::Test {
 public:
  static const uint16_t kPort = 6111;
  static const size_t kBufferSize = 16;

  bool CheckBufferEquals() {
    return strcmp(server_buffer_, client_buffer_) == 0;
  }

 protected:
  int StartServer() {
    struct sockaddr_in serv_addr;
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    EXPECT_FALSE(fd < 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(kPort);
    int reuse_flag = 1;
    EXPECT_FALSE(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse_flag, sizeof(reuse_flag)) < 0);
    EXPECT_FALSE(bind(fd, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0);

    listen(fd, 1);
    return fd;
  }

  int AcceptConnection(int fd) {
    struct sockaddr_in cli_addr;
    memset(&cli_addr, 0, sizeof(cli_addr));
    socklen_t clilen = sizeof(cli_addr);

    int connection_fd = accept(fd, (struct sockaddr*)&cli_addr, &clilen);
    EXPECT_FALSE(connection_fd < 0);

    return connection_fd;
  }

  void ReadIncomingMessage(int fd) {
    int n = TEMP_FAILURE_RETRY(read(fd, server_buffer_, kBufferSize - 1));
    EXPECT_FALSE(n < 0);

    if (n == 0) {  // got EOF
      async_manager_.StopWatchingFileDescriptor(fd);
      close(fd);
    } else {
      n = write(fd, "1", 1);
    }
  }

  void SetUp() override {
    memset(server_buffer_, 0, kBufferSize);

    socket_fd_ = StartServer();

    async_manager_.WatchFdForNonBlockingReads(socket_fd_, [this](int fd) {
      int connection_fd = AcceptConnection(fd);

      async_manager_.WatchFdForNonBlockingReads(connection_fd, [this](int fd) { ReadIncomingMessage(fd); });
    });
  }

  void TearDown() override {
    async_manager_.StopWatchingFileDescriptor(socket_fd_);
    close(socket_fd_);
    EXPECT_TRUE(CheckBufferEquals());
  }

  int ConnectClient() {
    int socket_cli_fd = socket(AF_INET, SOCK_STREAM, 0);
    EXPECT_FALSE(socket_cli_fd < 0);

    struct hostent* server;
    server = gethostbyname("localhost");
    EXPECT_FALSE(server == NULL);

    struct sockaddr_in serv_addr;
    memset((void*)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = *(reinterpret_cast<in_addr_t*>(server->h_addr));
    serv_addr.sin_port = htons(kPort);

    int result = connect(socket_cli_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    EXPECT_FALSE(result < 0);

    return socket_cli_fd;
  }

  void WriteFromClient(int socket_cli_fd) {
    strcpy(client_buffer_, "1");
    int n = write(socket_cli_fd, client_buffer_, strlen(client_buffer_));
    EXPECT_TRUE(n > 0);
  }

  void AwaitServerResponse(int socket_cli_fd) {
    int n = read(socket_cli_fd, client_buffer_, 1);
    EXPECT_TRUE(n > 0);
  }

 private:
  AsyncManager async_manager_;
  int socket_fd_;
  char server_buffer_[kBufferSize];
  char client_buffer_[kBufferSize];
};

TEST_F(AsyncManagerSocketTest, TestOneConnection) {
  int socket_cli_fd = ConnectClient();

  WriteFromClient(socket_cli_fd);

  AwaitServerResponse(socket_cli_fd);

  close(socket_cli_fd);
}

TEST_F(AsyncManagerSocketTest, TestRepeatedConnections) {
  static const int num_connections = 300;
  for (int i = 0; i < num_connections; i++) {
    int socket_cli_fd = ConnectClient();
    WriteFromClient(socket_cli_fd);
    AwaitServerResponse(socket_cli_fd);
    close(socket_cli_fd);
  }
}

TEST_F(AsyncManagerSocketTest, TestMultipleConnections) {
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

}  // namespace test_vendor_lib
