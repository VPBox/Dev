//
// Copyright (C) 2016 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef UPDATE_ENGINE_COMMON_FILE_FETCHER_H_
#define UPDATE_ENGINE_COMMON_FILE_FETCHER_H_

#include <memory>
#include <string>
#include <utility>

#include <base/logging.h>
#include <base/macros.h>
#include <brillo/message_loops/message_loop.h>
#include <brillo/streams/stream.h>

#include "update_engine/common/http_fetcher.h"

// This is a concrete implementation of HttpFetcher that reads files
// asynchronously.

namespace chromeos_update_engine {

class FileFetcher : public HttpFetcher {
 public:
  // Returns whether the passed url is supported.
  static bool SupportedUrl(const std::string& url);

  FileFetcher() : HttpFetcher(nullptr) {}

  // Cleans up all internal state. Does not notify delegate.
  ~FileFetcher() override;

  // HttpFetcher overrides.
  void SetOffset(off_t offset) override { offset_ = offset; }
  void SetLength(size_t length) override { data_length_ = length; }
  void UnsetLength() override { SetLength(0); }

  // Begins the transfer if it hasn't already begun.
  void BeginTransfer(const std::string& url) override;

  // If the transfer is in progress, aborts the transfer early. The transfer
  // cannot be resumed.
  void TerminateTransfer() override;

  // Ignore all extra headers for files.
  void SetHeader(const std::string& header_name,
                 const std::string& header_value) override {}

  // Suspend the asynchronous file read.
  void Pause() override;

  // Resume the suspended file read.
  void Unpause() override;

  size_t GetBytesDownloaded() override {
    return static_cast<size_t>(bytes_copied_);
  }

  // Ignore all the time limits for files.
  void set_low_speed_limit(int low_speed_bps, int low_speed_sec) override {}
  void set_connect_timeout(int connect_timeout_seconds) override {}
  void set_max_retry_count(int max_retry_count) override {}

 private:
  // Cleans up the fetcher, resetting its status to a newly constructed one.
  void CleanUp();

  // Schedule a new asynchronous read if the stream is not paused and no other
  // read is in process. This method can be called at any point.
  void ScheduleRead();

  // Called from the main loop when a single read from |stream_| succeeds or
  // fails, calling OnReadDoneCallback() and OnReadErrorCallback() respectively.
  void OnReadDoneCallback(size_t bytes_read);
  void OnReadErrorCallback(const brillo::Error* error);

  // Whether the transfer was started and didn't finish yet.
  bool transfer_in_progress_{false};

  // Whether the transfer is paused.
  bool transfer_paused_{false};

  // Whether there's an ongoing asynchronous read. When this value is true, the
  // the |buffer_| is being used by the |stream_|.
  bool ongoing_read_{false};

  // Total number of bytes copied.
  uint64_t bytes_copied_{0};

  // The offset inside the file where the read should start.
  uint64_t offset_{0};

  // The length of the data or -1 if unknown (will read until EOF).
  int64_t data_length_{-1};

  brillo::StreamPtr stream_;

  // The buffer used for reading from the stream.
  brillo::Blob buffer_;

  DISALLOW_COPY_AND_ASSIGN(FileFetcher);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_COMMON_FILE_FETCHER_H_
