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

#ifndef SIMPLE_PERF_INPLACE_SAMPLER_CLIENT_H_
#define SIMPLE_PERF_INPLACE_SAMPLER_CLIENT_H_

#include <memory>
#include <set>
#include <vector>

#include "event_attr.h"
#include "record.h"
#include "UnixSocket.h"

class InplaceSamplerClient {
 public:
  static std::unique_ptr<InplaceSamplerClient> Create(const perf_event_attr& attr, pid_t pid,
                                                      const std::set<pid_t>& tids);
  uint64_t Id() const;
  bool IsClosed() {
    return conn_->IsClosed();
  }
  bool StartPolling(IOEventLoop& loop, const std::function<bool(Record*)>& record_callback,
                    const std::function<bool()>& close_callback);
  bool StopProfiling(IOEventLoop& loop, const std::function<bool()>& close_callback);

 private:
  InplaceSamplerClient(const perf_event_attr& attr, pid_t pid, const std::set<pid_t>& tids);
  bool ConnectServer();
  bool SendStartProfilingMessage();
  bool HandleMessage(const UnixSocketMessage& msg);

  const perf_event_attr attr_;
  const pid_t pid_;
  const std::set<pid_t> tids_;
  uint32_t sample_freq_;
  std::unique_ptr<UnixSocketConnection> conn_;
  std::function<bool(Record*)> record_callback_;
  bool got_start_profiling_reply_msg_;
};

#endif  // SIMPLE_PERF_INPLACE_SAMPLER_CLIENT_H_
