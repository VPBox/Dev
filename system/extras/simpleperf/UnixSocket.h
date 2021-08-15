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

#ifndef SIMPLE_PERF_UNIX_SOCKET_H_
#define SIMPLE_PERF_UNIX_SOCKET_H_

#include <unistd.h>

#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include <android-base/logging.h>

#include "IOEventLoop.h"
#include "utils.h"

// Class wrappers for unix socket communication operations.

class UnixSocketConnection;

// UnixSocketMessage is the message structure used for communication.
struct UnixSocketMessage {
  uint32_t len;
  uint32_t type;
  char data[0];
};

// We want to avoid memory copy by being able to cast from char array
// to UnixSocketMessage* directly (See the implementation in
// UnixSocketConnection::ConsumeDataInReadBuffer()). To access members
// of UnixSocketMessage and its extensions without causing alignment problems
// (On arm, some instructions (like LDRD) don't support unaligned address),
// we make sure all messages are stored at 8-bytes aligned addresses. Namely,
// each message will be padded to 8-bytes aligned size.
static constexpr uint32_t UnixSocketMessageAlignment = 8u;

// UnixSocketMessageBuffer is a circular buffer used to store
// UnixSocketMessages.
class UnixSocketMessageBuffer {
 public:
  explicit UnixSocketMessageBuffer(size_t capacity)
      : data_(capacity), read_head_(0), valid_bytes_(0) {}

  bool Empty() const { return valid_bytes_ == 0; }

  bool HalfFull() const { return valid_bytes_ * 2 >= data_.size(); }

  bool StoreMessage(const UnixSocketMessage& message) {
    uint32_t aligned_len = Align(message.len, UnixSocketMessageAlignment);
    if (data_.size() - valid_bytes_ < aligned_len) {
      return false;
    }
    uint32_t write_head = (read_head_ + valid_bytes_) % data_.size();
    if (message.len <= data_.size() - write_head) {
      memcpy(data_.data() + write_head, &message, message.len);
    } else {
      uint32_t len1 = data_.size() - write_head;
      memcpy(data_.data() + write_head, &message, len1);
      memcpy(data_.data(), reinterpret_cast<const char*>(&message) + len1,
             message.len - len1);
    }
    valid_bytes_ += aligned_len;
    return true;
  }

  size_t PeekData(const char** pdata) {
    *pdata = &data_[read_head_];
    if (read_head_ + valid_bytes_ <= data_.size()) {
      return valid_bytes_;
    }
    return data_.size() - read_head_;
  }

  void CommitData(size_t size) {
    CHECK_GE(valid_bytes_, size);
    read_head_ = (read_head_ + size) % data_.size();
    valid_bytes_ -= size;
  }

 private:
  std::vector<char> data_;
  uint32_t read_head_;
  uint32_t valid_bytes_;
};

// UnixSocketServer creates a unix socket server listening on a unix file path.
class UnixSocketServer {
 public:
  static std::unique_ptr<UnixSocketServer> Create(
      const std::string& server_path, bool is_abstract);

  ~UnixSocketServer();
  const std::string& GetPath() const { return path_; }
  std::unique_ptr<UnixSocketConnection> AcceptConnection();

 private:
  UnixSocketServer(int server_fd, const std::string& path)
      : server_fd_(server_fd), path_(path) {}
  const int server_fd_;
  const std::string path_;
};

// UnixSocketConnection is used to communicate between server and client.
// It is either created by accepting a connection in UnixSocketServer, or by
// connecting to a UnixSocketServer.
// UnixSocketConnection binds to a IOEventLoop, so it writes messages to fd
// when it is writable, and read messages from fd when it is readable. To send
// messages, UnixSocketConnection uses a buffer to store to-be-sent messages.
// And whenever it receives a complete message from fd, it calls the callback
// function.
// In UnixSocketConnection, although user can send messages concurrently from
// different threads, only the thread running IOEventLoop::RunLoop() can
// do IO operations, calling WriteData() and ReadData(). To make it work
// properly, the thread creating/destroying UnixSocketConnection should be
// the same thread running IOEventLoop::RunLoop().
class UnixSocketConnection {
 private:
  static constexpr size_t SEND_BUFFER_SIZE = 512 * 1024;
  static constexpr size_t READ_BUFFER_SIZE = 16 * 1024;

 public:
  explicit UnixSocketConnection(int fd)
      : fd_(fd),
        read_buffer_(READ_BUFFER_SIZE),
        read_buffer_size_(0),
        read_event_(nullptr),
        send_buffer_(SEND_BUFFER_SIZE),
        write_event_enabled_(true),
        write_event_(nullptr),
        no_more_message_(false) {}

  static std::unique_ptr<UnixSocketConnection> Connect(
      const std::string& server_path, bool is_abstract);

  ~UnixSocketConnection();

  bool IsClosed() {
    return fd_ == -1;
  }

  bool PrepareForIO(IOEventLoop& loop,
                    const std::function<bool(const UnixSocketMessage&)>&
                        receive_message_callback,
                    const std::function<bool()>& close_connection_callback);

  // Thread-safe function, can be called from signal handler.
  // The message is put into the send buffer. If [undelayed] is true, messages
  // in the send buffer are sent immediately, otherwise they will be sent
  // when the buffer is half full.
  bool SendMessage(const UnixSocketMessage& message, bool undelayed) {
    std::lock_guard<std::mutex> lock(send_buffer_and_write_event_mtx_);
    if (no_more_message_ || !send_buffer_.StoreMessage(message)) {
      return false;
    }
    // By buffering messages, we can effectively decrease context-switch times.
    if (undelayed || send_buffer_.HalfFull()) {
      return EnableWriteEventWithLock();
    }
    return true;
  }

  // Thread-safe function.
  // After NoMoreMessage(), the connection will not accept more messages
  // in SendMessage(), and it will be closed after sending existing messages
  // in send buffer.
  bool NoMoreMessage() {
    std::lock_guard<std::mutex> lock(send_buffer_and_write_event_mtx_);
    if (!no_more_message_) {
      no_more_message_ = true;
      return EnableWriteEventWithLock();
    }
    return true;
  }

 private:
  // The caller should have send_buffer_and_write_event_mtx_ locked.
  bool EnableWriteEventWithLock() {
    if (!write_event_enabled_) {
      if (!IOEventLoop::EnableEvent(write_event_)) {
        return false;
      }
      write_event_enabled_ = true;
    }
    return true;
  }
  // The caller should have send_buffer_and_write_event_mtx_ locked.
  bool DisableWriteEventWithLock() {
    if (write_event_enabled_) {
      if (!IOEventLoop::DisableEvent(write_event_)) {
        return false;
      }
      write_event_enabled_ = false;
    }
    return true;
  }

  // Below functions are only called in the thread running IO operations.
  bool WriteData();
  bool GetDataFromSendBuffer(const char** pdata, size_t* pdata_size);
  bool ReadData();
  bool ConsumeDataInReadBuffer();
  bool CloseConnection();

  // Below members can only be accessed in the thread running IO operations.
  int fd_;
  std::function<bool(const UnixSocketMessage&)> read_callback_;
  std::function<bool()> close_callback_;
  // read_buffer_ is used to cache data read from the other end.
  // read_buffer_size_ is the number of valid bytes in read_buffer_.
  std::vector<char> read_buffer_;
  size_t read_buffer_size_;
  IOEventRef read_event_;

  // send_buffer_and_write_event_mtx_ protects following members, which can be
  // accessed in multiple threads.
  std::mutex send_buffer_and_write_event_mtx_;
  UnixSocketMessageBuffer send_buffer_;
  bool write_event_enabled_;
  IOEventRef write_event_;
  bool no_more_message_;
};

#endif  // SIMPLE_PERF_UNIX_SOCKET_H_
