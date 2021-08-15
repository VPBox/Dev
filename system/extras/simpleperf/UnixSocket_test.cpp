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

#include "UnixSocket.h"

#include <gtest/gtest.h>

#include <string>
#include <thread>

TEST(UnixSocket, message_buffer_smoke) {
  struct Message {
    uint32_t len;
    uint32_t type;
    char data[10];
  } send_msg;
  constexpr size_t send_data_size = 1024;
  std::vector<char> send_data(send_data_size);
  std::vector<char> read_data;
  for (size_t i = 0; i < send_data_size; ++i) {
    send_data[i] = i & 0xff;
  }
  UnixSocketMessageBuffer buffer(100);
  size_t per_msg_bytes = 0;
  size_t send_bytes = 0;
  while (true) {
    // Send data as much as possible.
    while (send_bytes < send_data_size) {
      size_t n = std::min(per_msg_bytes, send_data_size - send_bytes);
      per_msg_bytes = (per_msg_bytes + 1) % 10;
      memcpy(send_msg.data, &send_data[send_bytes], n);
      send_msg.len = sizeof(UnixSocketMessage) + n;
      send_msg.type = n;
      if (!buffer.StoreMessage(
              *reinterpret_cast<UnixSocketMessage*>(&send_msg))) {
        break;
      }
      send_bytes += n;
    }
    if (buffer.Empty()) {
      break;
    }
    // Read one message.
    std::vector<char> read_buf;
    auto read_func = [&](size_t size) {
      while (read_buf.size() < size) {
        const char* p;
        size_t n = buffer.PeekData(&p);
        n = std::min(n, size - read_buf.size());
        read_buf.insert(read_buf.end(), p, p + n);
        buffer.CommitData(n);
      }
    };
    read_func(sizeof(UnixSocketMessage));
    Message* msg = reinterpret_cast<Message*>(read_buf.data());
    size_t aligned_len = Align(msg->len, UnixSocketMessageAlignment);
    read_func(aligned_len);
    msg = reinterpret_cast<Message*>(read_buf.data());
    ASSERT_EQ(msg->len, msg->type + sizeof(UnixSocketMessage));
    read_data.insert(read_data.end(), msg->data, msg->data + msg->type);
  }
  ASSERT_EQ(send_data, read_data);
}

static void ClientToTestUndelayedMessage(const std::string& path,
                                         bool& client_success) {
  std::unique_ptr<UnixSocketConnection> client =
      UnixSocketConnection::Connect(path, true);
  ASSERT_TRUE(client != nullptr);
  IOEventLoop loop;
  // For each message received from the server, the client replies a msg
  // with type + 1.
  auto receive_message_callback = [&](const UnixSocketMessage& msg) {
    if (msg.len != sizeof(UnixSocketMessage)) {
      return false;
    }
    UnixSocketMessage reply_msg;
    reply_msg.len = sizeof(UnixSocketMessage);
    reply_msg.type = msg.type + 1;
    return client->SendMessage(reply_msg, true);
  };
  auto close_connection_callback = [&]() { return loop.ExitLoop(); };
  ASSERT_TRUE(client->PrepareForIO(loop, receive_message_callback,
                                   close_connection_callback));
  ASSERT_TRUE(loop.RunLoop());
  client_success = true;
}

TEST(UnixSocket, undelayed_message) {
  std::string path = "unix_socket_test_" + std::to_string(getpid());
  std::unique_ptr<UnixSocketServer> server =
      UnixSocketServer::Create(path, true);
  ASSERT_TRUE(server != nullptr);
  bool client_success = false;
  std::thread thread(
      [&]() { ClientToTestUndelayedMessage(path, client_success); });
  std::unique_ptr<UnixSocketConnection> conn = server->AcceptConnection();
  ASSERT_TRUE(conn != nullptr);
  IOEventLoop loop;
  uint32_t need_reply_type = 1;
  // For each message received from the client, the server replies a msg
  // with type + 1, and exits when type reaches 10.
  auto receive_message_callback = [&](const UnixSocketMessage& msg) {
    if (msg.len != sizeof(UnixSocketMessage) || msg.type != need_reply_type) {
      return false;
    }
    if (need_reply_type >= 10) {
      return conn->NoMoreMessage();
    }
    UnixSocketMessage new_msg;
    new_msg.len = sizeof(UnixSocketMessage);
    new_msg.type = msg.type + 1;
    need_reply_type = msg.type + 2;
    return conn->SendMessage(new_msg, true);
  };
  auto close_connection_callback = [&]() { return loop.ExitLoop(); };
  ASSERT_TRUE(conn->PrepareForIO(loop, receive_message_callback,
                                 close_connection_callback));
  UnixSocketMessage msg;
  msg.len = sizeof(UnixSocketMessage);
  msg.type = 0;
  ASSERT_TRUE(conn->SendMessage(msg, true));
  ASSERT_TRUE(loop.RunLoop());
  thread.join();
  ASSERT_TRUE(client_success);
}

static void ClientToTestBufferedMessage(const std::string& path,
                                        bool& client_success) {
  std::unique_ptr<UnixSocketConnection> client =
      UnixSocketConnection::Connect(path, true);
  ASSERT_TRUE(client != nullptr);
  IOEventLoop loop;
  // The client exits once receiving a message from the server.
  auto receive_message_callback = [&](const UnixSocketMessage& msg) {
    if (msg.len != sizeof(UnixSocketMessage) || msg.type != 0) {
      return false;
    }
    return client->NoMoreMessage();
  };
  auto close_connection_callback = [&]() { return loop.ExitLoop(); };
  ASSERT_TRUE(client->PrepareForIO(loop, receive_message_callback,
                                   close_connection_callback));
  // The client sends buffered messages until the send buffer is full.
  UnixSocketMessage msg;
  msg.len = sizeof(UnixSocketMessage);
  msg.type = 0;
  while (true) {
    msg.type++;
    if (!client->SendMessage(msg, false)) {
      break;
    }
  }
  ASSERT_TRUE(loop.RunLoop());
  client_success = true;
}

TEST(UnixSocket, buffered_message) {
  std::string path = "unix_socket_test_" + std::to_string(getpid());
  std::unique_ptr<UnixSocketServer> server =
      UnixSocketServer::Create(path, true);
  ASSERT_TRUE(server != nullptr);
  bool client_success = false;
  std::thread thread(
      [&]() { ClientToTestBufferedMessage(path, client_success); });
  std::unique_ptr<UnixSocketConnection> conn = server->AcceptConnection();
  ASSERT_TRUE(conn != nullptr);
  IOEventLoop loop;
  uint32_t need_reply_type = 1;
  auto receive_message_callback = [&](const UnixSocketMessage& msg) {
    // The server checks if the type of received message is increased by one
    // each time.
    if (msg.len != sizeof(UnixSocketMessage) || msg.type != need_reply_type) {
      return false;
    }
    if (need_reply_type == 1) {
      // Notify the client to exit.
      UnixSocketMessage new_msg;
      new_msg.len = sizeof(UnixSocketMessage);
      new_msg.type = 0;
      if (!conn->SendMessage(new_msg, true)) {
        return false;
      }
    }
    need_reply_type++;
    return true;
  };
  auto close_connection_callback = [&]() { return loop.ExitLoop(); };
  ASSERT_TRUE(conn->PrepareForIO(loop, receive_message_callback,
                                 close_connection_callback));
  ASSERT_TRUE(loop.RunLoop());
  thread.join();
  ASSERT_TRUE(client_success);
}
