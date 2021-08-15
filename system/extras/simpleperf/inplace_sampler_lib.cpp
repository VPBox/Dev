/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include "inplace_sampler_lib.h"

#include <inttypes.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/ucontext.h>
#include <unistd.h>

#include <map>
#include <memory>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>

#include <android-base/logging.h>
#include <android-base/macros.h>
#include <backtrace/Backtrace.h>
#define LOG_TAG "InplaceSampler"
#include <log/log.h>

#include "environment.h"
#include "UnixSocket.h"
#include "utils.h"

#define DEFAULT_SIGNO  SIGRTMAX
static constexpr int DEFAULT_SAMPLE_FREQ = 4000;
static constexpr int CHECK_THREADS_INTERVAL_IN_MS = 200;

namespace {

struct ThreadInfo {
  std::string name;
};

// SampleManager controls the whole sampling process:
//   Read commands from simpleperf
//   Set up timers to send signals for each profiled thread regularly.
//   Send thread info and map info to simpleperf.
class SampleManager {
 public:
  SampleManager(std::unique_ptr<UnixSocketConnection> conn) : conn_(std::move(conn)),
      tid_(gettid()), signo_(DEFAULT_SIGNO), sample_freq_(DEFAULT_SAMPLE_FREQ),
      sample_period_in_ns_(0), dump_callchain_(false), monitor_all_threads_(true) {
  }
  void Run();

 private:
  bool HandleMessage(const UnixSocketMessage& msg);
  bool ParseStartProfilingMessage(const UnixSocketMessage& msg);
  bool SendStartProfilingReplyMessage(bool ok);
  bool StartProfiling();
  bool InstallSignalHandler();
  bool CheckThreads();
  bool CheckThreadNameChange(uint64_t timestamp);
  bool CheckMapChange(uint64_t timestamp);
  void SendThreadMapInfo();
  void SendFakeSampleRecord();

  std::unique_ptr<UnixSocketConnection> conn_;

  int tid_;
  int signo_;
  uint32_t sample_freq_;
  uint32_t sample_period_in_ns_;
  bool dump_callchain_;
  bool monitor_all_threads_;
  std::set<int> monitor_tid_filter_;
  std::map<int, ThreadInfo> threads_;
  std::map<uint64_t, ThreadMmap> maps_;
  std::queue<std::unique_ptr<char[]>> thread_map_info_q_;

  IOEventLoop loop_;
};

void SampleManager::Run() {
  auto read_callback = [&](const UnixSocketMessage& msg) {
    return HandleMessage(msg);
  };
  auto close_callback = [&]() {
    return loop_.ExitLoop();
  };
  if (!conn_->PrepareForIO(loop_, read_callback, close_callback)) {
    return;
  }
  loop_.RunLoop();
}

bool SampleManager::HandleMessage(const UnixSocketMessage& msg) {
  if (msg.type == START_PROFILING) {
    if (!ParseStartProfilingMessage(msg)) {
      if (!SendStartProfilingReplyMessage(false)) {
        return false;
      }
      return conn_->NoMoreMessage();
    }
    if (!SendStartProfilingReplyMessage(true)) {
      return false;
    }
    return StartProfiling();
  }
  if (msg.type == END_PROFILING) {
    // Close connection after clearing send buffer.
    return conn_->NoMoreMessage();
  }
  LOG(ERROR) << "Unexpected msg type: " << msg.type;
  return false;
}

bool SampleManager::ParseStartProfilingMessage(const UnixSocketMessage& msg) {
  char* option = const_cast<char*>(msg.data);
  while (option != nullptr && *option != '\0') {
    char* next_option = strchr(option, ' ');
    if (next_option != nullptr) {
      *next_option++ = '\0';
    }
    char* equal_op = strchr(option, '=');
    if (equal_op != nullptr) {
      char* key = option;
      *equal_op = '\0';
      char* value = equal_op + 1;
      if (strcmp(key, "freq") == 0) {
        sample_freq_ = atoi(value);
      } else if (strcmp(key, "signal") == 0) {
        signo_ = atoi(value);
      } else if (strcmp(key, "tids") == 0) {
        monitor_all_threads_ = false;
        while (*value != '\0') {
          int tid = static_cast<int>(strtol(value, &value, 10));
          monitor_tid_filter_.insert(tid);
          if (*value == ',') {
            ++value;
          }
        }
      } else if (strcmp(key, "dump_callchain") == 0) {
        dump_callchain_ = (strcmp(value, "1") == 0);
      }
    }
    option = next_option;
  }
  if (sample_freq_ == 0 || sample_freq_ > 1000000000) {
    LOG(ERROR) << "Unexpected sample_freq: " << sample_freq_;
    return false;
  }
  if (sample_freq_ == 1) {
    sample_period_in_ns_ = 999999999;
  } else {
    sample_period_in_ns_ = 1000000000 / sample_freq_;
  }
  return true;
}

bool SampleManager::SendStartProfilingReplyMessage(bool ok) {
  const char* s = ok ? "ok" : "error";
  size_t size = sizeof(UnixSocketMessage) + strlen(s) + 1;
  std::unique_ptr<char[]> data(new char[size]);
  UnixSocketMessage* msg = reinterpret_cast<UnixSocketMessage*>(data.get());
  msg->len = size;
  msg->type = START_PROFILING_REPLY;
  strcpy(msg->data, s);
  return conn_->SendMessage(*msg, true);
}

bool SampleManager::StartProfiling() {
  if (!InstallSignalHandler()) {
    return false;
  }
  if (!CheckThreads()) {
    return false;
  }
  timeval tv;
  tv.tv_sec = CHECK_THREADS_INTERVAL_IN_MS / 1000;
  tv.tv_usec = CHECK_THREADS_INTERVAL_IN_MS % 1000 * 1000;
  return loop_.AddPeriodicEvent(tv, [&]() {
    return CheckThreads();
  });
}

bool SampleManager::InstallSignalHandler() {
  return true;
}

bool SampleManager::CheckThreads() {
  uint64_t timestamp = GetSystemClock();
  if (!CheckMapChange(timestamp)) {
    return false;
  }
  if (!CheckThreadNameChange(timestamp)) {
    return false;
  }
  SendThreadMapInfo();
  // For testing.
  SendFakeSampleRecord();
  return true;
}

bool SampleManager::CheckThreadNameChange(uint64_t timestamp) {
  std::vector<pid_t> tids = GetThreadsInProcess(getpid());
  std::map<pid_t, std::string> current;
  for (auto& tid : tids) {
    if (tid == tid_) {
      // Skip sample thread.
      continue;
    }
    if (monitor_all_threads_ || monitor_tid_filter_.find(tid) != monitor_tid_filter_.end()) {
      std::string name;
      if (GetThreadName(tid, &name)) {
        current[tid] = name;
      }
    }
  }
  // Check new threads or threads with new names.
  for (auto& pair : current) {
    pid_t tid = pair.first;
    auto it = threads_.find(tid);
    if (it == threads_.end() || it->second.name != pair.second) {
      threads_[tid].name = pair.second;
      size_t size = sizeof(UnixSocketMessage) + sizeof(uint64_t) + sizeof(uint32_t) +
          pair.second.size() + 1;
      std::unique_ptr<char[]> data(new char[size]);
      UnixSocketMessage* msg = reinterpret_cast<UnixSocketMessage*>(data.get());
      msg->len = size;
      msg->type = THREAD_INFO;
      char* p = msg->data;
      MoveToBinaryFormat(timestamp, p);
      MoveToBinaryFormat(static_cast<uint32_t>(tid), p);
      MoveToBinaryFormat(pair.second.c_str(), pair.second.size() + 1, p);
      thread_map_info_q_.push(std::move(data));
    }
  }
  // Check deleted threads.
  for (auto it = threads_.begin(); it != threads_.end();) {
    int tid = it->first;
    if (current.find(tid) == current.end()) {
      it = threads_.erase(it);
    } else {
      ++it;
    }
  }
  return true;
}

bool SampleManager::CheckMapChange(uint64_t timestamp) {
  std::vector<ThreadMmap> maps;
  if (!GetThreadMmapsInProcess(getpid(), &maps)) {
    return false;
  }
  // Check new maps or changed maps.
  for (auto& map : maps) {
    if (!(map.prot & PROT_EXEC)) {
      continue;
    }
    auto it = maps_.find(map.start_addr);
    if (it == maps_.end() || it->second.len != map.len || it->second.pgoff != map.pgoff ||
        it->second.name != map.name) {
      maps_[map.start_addr] = map;
      size_t size = sizeof(UnixSocketMessage) + sizeof(uint64_t) * 4 + map.name.size() + 1;
      std::unique_ptr<char[]> data(new char[size]);
      UnixSocketMessage* msg = reinterpret_cast<UnixSocketMessage*>(data.get());
      msg->len = size;
      msg->type = MAP_INFO;
      char* p = msg->data;
      MoveToBinaryFormat(timestamp, p);
      MoveToBinaryFormat(map.start_addr, p);
      MoveToBinaryFormat(map.len, p);
      MoveToBinaryFormat(map.pgoff, p);
      MoveToBinaryFormat(map.name.c_str(), map.name.size() + 1, p);
      thread_map_info_q_.push(std::move(data));
    }
  }
  return true;
}

void SampleManager::SendThreadMapInfo() {
  while (!thread_map_info_q_.empty()) {
    auto& data = thread_map_info_q_.front();
    UnixSocketMessage* msg = reinterpret_cast<UnixSocketMessage*>(data.get());
    if (!conn_->SendMessage(*msg, false)) {
      break;
    }
    thread_map_info_q_.pop();
  }
}

static void FakeFunction() {
}

void SampleManager::SendFakeSampleRecord() {
  size_t size = sizeof(UnixSocketMessage) + sizeof(uint64_t) * 2 + sizeof(uint32_t) *  3;
  std::unique_ptr<char[]> data(new char[size]);
  UnixSocketMessage* msg = reinterpret_cast<UnixSocketMessage*>(data.get());
  uint64_t ip = static_cast<uint64_t>(reinterpret_cast<uintptr_t>(&FakeFunction));
  msg->len = size;
  msg->type = SAMPLE_INFO;
  char* p = msg->data;
  MoveToBinaryFormat(GetSystemClock(), p);
  MoveToBinaryFormat(static_cast<uint32_t>(tid_), p);
  MoveToBinaryFormat(1u, p);
  MoveToBinaryFormat(1u, p);
  MoveToBinaryFormat(ip, p);
  conn_->SendMessage(*msg, false);
}

static void* CommunicationThread(void*) {
  pthread_setname_np(pthread_self(), "inplace_sampler");
  std::string server_path = "inplace_sampler_server_" + std::to_string(getpid());
  std::unique_ptr<UnixSocketServer> server = UnixSocketServer::Create(server_path, true);
  if (server == nullptr) {
    LOG(ERROR) << "failed to create server at path " << server_path;
    return nullptr;
  }
  LOG(INFO) << "Create inplace_sampler_server at " << server_path;
  while (true) {
    std::unique_ptr<UnixSocketConnection> conn = server->AcceptConnection();
    if (conn == nullptr) {
      break;
    }
    SampleManager manager(std::move(conn));
    manager.Run();
  }
  return nullptr;
}

__attribute__((constructor)) void InitSampler() {
  pthread_attr_t attr;
  if (pthread_attr_init(&attr) != 0) {
    LOG(ERROR) << "pthread_attr_init failed";
    return;
  }
  if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0) {
    LOG(ERROR) << "pthread_attr_setdetachstate failed";
    return;
  }
  pthread_t thread;
  if (pthread_create(&thread, &attr, CommunicationThread, nullptr) != 0) {
    LOG(ERROR) << "pthread_create failed";
    return;
  }
  pthread_attr_destroy(&attr);
}

}  // namespace
