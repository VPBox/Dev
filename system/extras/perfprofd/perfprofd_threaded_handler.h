/*
 *
 * Copyright 2017, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SYSTEM_EXTRAS_PERFPROFD_PERFPROFD_THREADED_HANDLER_H_
#define SYSTEM_EXTRAS_PERFPROFD_PERFPROFD_THREADED_HANDLER_H_

#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <functional>

#include <inttypes.h>
#include <unistd.h>

#include <android-base/logging.h>
#include <android-base/stringprintf.h>

#include "perfprofd_record.pb.h"

#include "config.h"
#include "dropbox.h"
#include "perfprofdcore.h"
#include "perfprofd_io.h"

namespace android {
namespace perfprofd {

class ThreadedConfig : public Config {
 public:
  void Sleep(size_t seconds) override {
    if (seconds == 0) {
      return;
    }
    std::unique_lock<std::mutex> guard(mutex_);
    using namespace std::chrono_literals;
    cv_.wait_for(guard, seconds * 1s, [&]() { return interrupted_; });
  }
  bool ShouldStopProfiling() override {
    std::unique_lock<std::mutex> guard(mutex_);
    return interrupted_;
  }

  void ResetStopProfiling() {
    std::unique_lock<std::mutex> guard(mutex_);
    interrupted_ = false;
  }
  void StopProfiling() {
    std::unique_lock<std::mutex> guard(mutex_);
    interrupted_ = true;
    cv_.notify_all();
  }

  bool IsProfilingEnabled() const override {
    return true;
  }

  // Operator= to simplify setting the config values. This will retain the
  // original mutex, condition-variable etc.
  ThreadedConfig& operator=(const ThreadedConfig& rhs) {
    // Copy base fields.
    *static_cast<Config*>(this) = static_cast<const Config&>(rhs);

    return *this;
  }

 private:
  bool is_profiling = false;
  std::mutex mutex_;
  std::condition_variable cv_;
  bool interrupted_ = false;

  friend class ThreadedHandler;
};

class ThreadedHandler  {
 public:
  ThreadedHandler() : cur_config_(new ThreadedConfig()) {}
  explicit ThreadedHandler(ThreadedConfig* in) : cur_config_(in) {
    CHECK(cur_config_ != nullptr);
  }

  virtual ~ThreadedHandler() {}

  template <typename ConfigFn> bool StartProfiling(ConfigFn fn, std::string* error_msg) {
    std::lock_guard<std::mutex> guard(lock_);

    if (cur_config_->is_profiling) {
      *error_msg = "Already profiling";
      return false;
    }
    cur_config_->is_profiling = true;
    cur_config_->ResetStopProfiling();

    fn(*cur_config_);

    HandlerFn handler = GetResultHandler();
    auto profile_runner = [handler](ThreadedHandler* service) {
      ProfilingLoop(*service->cur_config_, handler);

      // This thread is done.
      std::lock_guard<std::mutex> unset_guard(service->lock_);
      service->cur_config_->is_profiling = false;
    };
    std::thread profiling_thread(profile_runner, this);
    profiling_thread.detach();  // Let it go.

    return true;
  }

  bool StopProfiling(std::string* error_msg) {
    std::lock_guard<std::mutex> guard(lock_);
    if (!cur_config_->is_profiling) {
      *error_msg = "Not profiling";
      return false;
    }

    cur_config_->StopProfiling();

    return true;
  }

 protected:
  // Handler for ProfilingLoop.
  virtual bool ResultHandler(android::perfprofd::PerfprofdRecord* encodedProfile,
                             Config* config) {
    CHECK(config != nullptr);
    if (encodedProfile == nullptr) {
      return false;
    }

    if (static_cast<ThreadedConfig*>(config)->send_to_dropbox) {
      std::string error_msg;
      if (!dropbox::SendToDropbox(encodedProfile, config->destination_directory, &error_msg)) {
        LOG(WARNING) << "Failed dropbox submission: " << error_msg;
        return false;
      }
      return true;
    }

    if (encodedProfile == nullptr) {
      return false;
    }
    std::string data_file_path(config->destination_directory);
    data_file_path += "/perf.data";
    std::string path = android::base::StringPrintf("%s.encoded.%d", data_file_path.c_str(), seq_);
    if (!SerializeProtobuf(encodedProfile, path.c_str(), config->compress)) {
      return false;
    }

    seq_++;
    return true;
  }

  template <typename Fn>
  void RunOnConfig(Fn& fn) {
    std::lock_guard<std::mutex> guard(lock_);
    fn(cur_config_->is_profiling, cur_config_.get());
  }

 private:
  // Helper for the handler.
  HandlerFn GetResultHandler() {
    return HandlerFn(std::bind(&ThreadedHandler::ResultHandler,
                               this,
                               std::placeholders::_1,
                               std::placeholders::_2));
  }

  std::mutex lock_;

  std::unique_ptr<ThreadedConfig> cur_config_;

  int seq_ = 0;
};

}  // namespace perfprofd
}  // namespace android

#endif  // SYSTEM_EXTRAS_PERFPROFD_PERFPROFD_THREADED_HANDLER_H_
