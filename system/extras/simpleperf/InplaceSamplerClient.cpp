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

#include "InplaceSamplerClient.h"

#include <algorithm>

#include "environment.h"
#include "inplace_sampler_lib.h"
#include "utils.h"

static constexpr uint64_t EVENT_ID_FOR_INPLACE_SAMPLER = ULONG_MAX;

std::unique_ptr<InplaceSamplerClient> InplaceSamplerClient::Create(const perf_event_attr& attr,
                                                                   pid_t pid,
                                                                   const std::set<pid_t>& tids) {
  if (pid == -1) {
    LOG(ERROR) << "inplace-sampler can't monitor system wide events.";
    return nullptr;
  }
  std::unique_ptr<InplaceSamplerClient> sampler(new InplaceSamplerClient(attr, pid, tids));
  if (!sampler->ConnectServer()) {
    return nullptr;
  }
  return sampler;
}

InplaceSamplerClient::InplaceSamplerClient(const perf_event_attr& attr, pid_t pid,
                                           const std::set<pid_t>& tids)
    : attr_(attr), pid_(pid), tids_(tids), got_start_profiling_reply_msg_(false) {
  if (attr_.freq) {
    sample_freq_ = attr_.sample_freq;
  } else {
    sample_freq_ = std::max(1u, static_cast<uint32_t>(1000000000 / attr_.sample_period));
  }
}

uint64_t InplaceSamplerClient::Id() const {
  return EVENT_ID_FOR_INPLACE_SAMPLER;
}

bool InplaceSamplerClient::ConnectServer() {
  std::string server_path = "inplace_sampler_server_" + std::to_string(pid_);
  // Try to connect server in 1s.
  uint64_t timeout = GetSystemClock() + 10000000000ull;
  while (GetSystemClock() < timeout) {
    conn_ = UnixSocketConnection::Connect(server_path, true);
    if (conn_ != nullptr) {
      return true;
    }
    usleep(10);
  }
  LOG(ERROR) << "Can't find inplace_sampler_server for process " << pid_;
  return false;
}

bool InplaceSamplerClient::StartPolling(IOEventLoop& loop,
                                        const std::function<bool(Record*)>& record_callback,
                                        const std::function<bool()>& close_callback) {
  record_callback_ = record_callback;
  CHECK(conn_ != nullptr);
  auto read_callback = [&](const UnixSocketMessage& msg) {
    return HandleMessage(msg);
  };
  if (!conn_->PrepareForIO(loop, read_callback, close_callback)) {
    return false;
  }
  if (!SendStartProfilingMessage()) {
    return false;
  }
  // If the inplace sampler doesn't reply in 3 seconds, report the error.
  timeval tv;
  tv.tv_sec = 3;
  tv.tv_usec = 0;
  auto check_reply_callback = [this]() {
    if (!got_start_profiling_reply_msg_) {
      LOG(ERROR) << "can't receive START_PROFILING_REPLY from process " << pid_;
      return false;
    }
    return true;
  };
  return loop.AddPeriodicEvent(tv, check_reply_callback);
}

bool InplaceSamplerClient::SendStartProfilingMessage() {
  std::string options;
  options += "freq=" + std::to_string(sample_freq_);
  if (attr_.sample_type & PERF_SAMPLE_CALLCHAIN) {
    options += " dump_callchain=1";
  }
  if (!tids_.empty()) {
    options += " tids=";
    bool first = true;
    for (auto& tid : tids_) {
      if (first) {
        first = false;
      } else {
        options.push_back(',');
      }
      options += std::to_string(tid);
    }
  }
  size_t size = sizeof(UnixSocketMessage) + options.size() + 1;
  std::unique_ptr<char[]> data(new char[size]);
  UnixSocketMessage* msg = reinterpret_cast<UnixSocketMessage*>(data.get());
  msg->len = size;
  msg->type = START_PROFILING;
  strcpy(msg->data, options.c_str());
  return conn_->SendMessage(*msg, true);
}

bool InplaceSamplerClient::StopProfiling(IOEventLoop& loop,
                                         const std::function<bool()>& close_callback) {
  auto read_callback = [&](const UnixSocketMessage& msg) {
    return HandleMessage(msg);
  };
  if (!conn_->PrepareForIO(loop, read_callback, close_callback)) {
    return false;
  }
  // Notify inplace sampler to send buffered data and close the connection.
  UnixSocketMessage msg;
  msg.len = sizeof(UnixSocketMessage);
  msg.type = END_PROFILING;
  return conn_->SendMessage(msg, true);
}

bool InplaceSamplerClient::HandleMessage(const UnixSocketMessage& msg) {
  const char* p = msg.data;
  if (msg.type == START_PROFILING_REPLY) {
      got_start_profiling_reply_msg_ = true;
      if (strcmp(p, "ok") != 0) {
        LOG(ERROR) << "receive reply from inplace_sampler_server of " << pid_ << ": " << p;
        return false;
      }
  } else if (msg.type == THREAD_INFO) {
    uint64_t time;
    uint32_t tid;
    MoveFromBinaryFormat(time, p);
    MoveFromBinaryFormat(tid, p);
    CommRecord r(attr_, pid_, tid, p, Id(), time);
    if (!record_callback_(&r)) {
      return false;
    }
  } else if (msg.type == MAP_INFO) {
    uint64_t time;
    uint64_t start;
    uint64_t len;
    uint64_t pgoff;
    MoveFromBinaryFormat(time, p);
    MoveFromBinaryFormat(start, p);
    MoveFromBinaryFormat(len, p);
    MoveFromBinaryFormat(pgoff, p);
    MmapRecord r(attr_, false, pid_, pid_, start, len, pgoff, p, Id(), time);
    if (!record_callback_(&r)) {
      return false;
    }
  } else if (msg.type == SAMPLE_INFO) {
    uint64_t time;
    uint32_t tid;
    uint32_t period;
    uint32_t ip_nr;
    MoveFromBinaryFormat(time, p);
    MoveFromBinaryFormat(tid, p);
    MoveFromBinaryFormat(period, p);
    MoveFromBinaryFormat(ip_nr, p);
    std::vector<uint64_t> ips(ip_nr);
    MoveFromBinaryFormat(ips.data(), ip_nr, p);
    // Don't know which cpu tid is running on, use cpu 0.
    SampleRecord r(attr_, Id(), ips[0], pid_, tid, time, 0, period, ips, {}, 0);
    if (!record_callback_(&r)) {
      return false;
    }
  } else {
    LOG(ERROR) << "Unexpected msg type: " << msg.type;
    return false;
  }
  return true;
}
