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

#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <algorithm>

#include <android-base/logging.h>

#include "IOEventLoop.h"

static bool CreateUnixSocketAddress(const std::string& server_path,
                                    bool is_abstract, sockaddr_un& serv_addr) {
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sun_family = AF_UNIX;
  size_t sun_path_len = sizeof(serv_addr.sun_path);
  char* p = serv_addr.sun_path;
  if (is_abstract) {
    sun_path_len--;
    p++;
  }
  if (server_path.size() + 1 > sun_path_len) {
    LOG(ERROR) << "can't create unix domain socket as server path is too long: "
               << server_path;
    return false;
  }
  strcpy(p, server_path.c_str());
  return true;
}

std::unique_ptr<UnixSocketServer> UnixSocketServer::Create(
    const std::string& server_path, bool is_abstract) {
  int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sockfd < 0) {
    PLOG(ERROR) << "socket() failed";
    return nullptr;
  }
  sockaddr_un serv_addr;
  if (!CreateUnixSocketAddress(server_path, is_abstract, serv_addr)) {
    return nullptr;
  }
  if (bind(sockfd, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) <
      0) {
    PLOG(ERROR) << "bind() failed for " << server_path;
    return nullptr;
  }
  if (listen(sockfd, 1) < 0) {
    PLOG(ERROR) << "listen() failed";
    return nullptr;
  }
  return std::unique_ptr<UnixSocketServer>(
      new UnixSocketServer(sockfd, server_path));
}

UnixSocketServer::~UnixSocketServer() { close(server_fd_); }

std::unique_ptr<UnixSocketConnection> UnixSocketServer::AcceptConnection() {
  int sockfd = accept(server_fd_, nullptr, nullptr);
  if (sockfd < 0) {
    PLOG(ERROR) << "accept() failed";
    return nullptr;
  }
  return std::unique_ptr<UnixSocketConnection>(
      new UnixSocketConnection(sockfd));
}

std::unique_ptr<UnixSocketConnection> UnixSocketConnection::Connect(
    const std::string& server_path, bool is_abstract) {
  int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sockfd < 0) {
    PLOG(DEBUG) << "socket() failed";
    return nullptr;
  }
  sockaddr_un serv_addr;
  if (!CreateUnixSocketAddress(server_path, is_abstract, serv_addr)) {
    return nullptr;
  }
  if (connect(sockfd, reinterpret_cast<sockaddr*>(&serv_addr),
              sizeof(serv_addr)) < 0) {
    PLOG(DEBUG) << "connect() failed, server_path = " << server_path;
    return nullptr;
  }
  return std::unique_ptr<UnixSocketConnection>(
      new UnixSocketConnection(sockfd));
}

bool UnixSocketConnection::PrepareForIO(
    IOEventLoop& loop, const std::function<bool(const UnixSocketMessage&)>&
                           receive_message_callback,
    const std::function<bool()>& close_connection_callback) {
  read_callback_ = receive_message_callback;
  close_callback_ = close_connection_callback;
  read_event_ = loop.AddReadEvent(fd_, [&]() { return ReadData(); });
  if (read_event_ == nullptr) {
    return false;
  }
  std::lock_guard<std::mutex> lock(send_buffer_and_write_event_mtx_);
  write_event_ = loop.AddWriteEvent(fd_, [&]() { return WriteData(); });
  if (write_event_ == nullptr) {
    return false;
  }
  return DisableWriteEventWithLock();
}

bool UnixSocketConnection::WriteData() {
  const char* write_data;
  size_t write_data_size;
  if (!GetDataFromSendBuffer(&write_data, &write_data_size)) {
    return false;
  }
  if (write_data_size == 0u) {
    return true;
  }
  // Use MSG_NOSIGNAL to prevent receiving SIGPIPE.
  ssize_t result =
      TEMP_FAILURE_RETRY(send(fd_, write_data, write_data_size, MSG_NOSIGNAL));
  if (result >= 0) {
    std::lock_guard<std::mutex> lock(send_buffer_and_write_event_mtx_);
    send_buffer_.CommitData(result);
  } else if (errno != EAGAIN) {
    PLOG(ERROR) << "send() failed";
    return false;
  }
  return true;
}

bool UnixSocketConnection::GetDataFromSendBuffer(const char** pdata,
                                                 size_t* pdata_size) {
  {
    std::lock_guard<std::mutex> lock(send_buffer_and_write_event_mtx_);
    *pdata_size = send_buffer_.PeekData(pdata);
    if (*pdata_size != 0u) {
      return true;
    }
    // The send buffer is empty. If we can receive more messages, just disable
    // the write event temporarily, otherwise close the connection.
    if (!no_more_message_) {
      return DisableWriteEventWithLock();
    }
  }
  return CloseConnection();
}

bool UnixSocketConnection::ReadData() {
  ssize_t result =
      TEMP_FAILURE_RETRY(read(fd_, &read_buffer_[read_buffer_size_],
                              read_buffer_.size() - read_buffer_size_));
  if (result < 0) {
    if (errno == EAGAIN) {
      return true;
    }
    PLOG(ERROR) << "read() failed";
    return false;
  } else if (result == 0) {
    // The connection is closed, and no need to write pending messages.
    return CloseConnection();
  }
  read_buffer_size_ += result;
  return ConsumeDataInReadBuffer();
}

bool UnixSocketConnection::ConsumeDataInReadBuffer() {
  char* p = read_buffer_.data();
  size_t left_size = read_buffer_size_;
  uint32_t aligned_len = 0;
  while (left_size >= sizeof(UnixSocketMessage)) {
    UnixSocketMessage* msg = reinterpret_cast<UnixSocketMessage*>(p);
    aligned_len = Align(msg->len, UnixSocketMessageAlignment);
    if (left_size < aligned_len) {
      break;
    }
    if (!read_callback_(*msg)) {
      return false;
    }
    p += aligned_len;
    left_size -= aligned_len;
  }
  if (left_size > 0u) {
    // Move the unfinished message to the start of read_buffer_.
    memmove(read_buffer_.data(), p, left_size);
    // Extend the buffer to store this big message.
    if (aligned_len > read_buffer_.size()) {
      read_buffer_.resize(aligned_len);
    }
  }
  read_buffer_size_ = left_size;
  return true;
}

bool UnixSocketConnection::CloseConnection() {
  // Disable read_event and write_event here, so ReadData() and WriteData()
  // won't be called in the future.
  if (!IOEventLoop::DisableEvent(read_event_)) {
    return false;
  }
  {
    std::lock_guard<std::mutex> lock(send_buffer_and_write_event_mtx_);
    no_more_message_ = true;
    if (!DisableWriteEventWithLock()) {
      return false;
    }
  }
  close(fd_);
  fd_ = -1;
  return close_callback_();
}

UnixSocketConnection::~UnixSocketConnection() {
  if (fd_ != -1) {
    // It only happens when IO operations are not finished properly by
    // CloseConnection(). Don't call CloseConnection() here as the
    // IOEventLoop used to register read/write events may have been destroyed.
    close(fd_);
  }
}
