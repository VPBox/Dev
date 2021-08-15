//
// Copyright (C) 2009 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_PAYLOAD_CONSUMER_FILE_WRITER_H_
#define UPDATE_ENGINE_PAYLOAD_CONSUMER_FILE_WRITER_H_

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <base/logging.h>

#include "update_engine/common/error_code.h"
#include "update_engine/common/utils.h"

// FileWriter is a class that is used to (synchronously, for now) write to
// a file. This file is a thin wrapper around open/write/close system calls,
// but provides and interface that can be customized by subclasses that wish
// to filter the data.

namespace chromeos_update_engine {

class FileWriter {
 public:
  FileWriter() {}
  virtual ~FileWriter() {}

  // Wrapper around write. Returns true if all requested bytes
  // were written, or false on any error, regardless of progress.
  virtual bool Write(const void* bytes, size_t count) = 0;

  // Same as the Write method above but returns a detailed |error| code
  // in addition if the returned value is false. By default this method
  // returns kActionExitDownloadWriteError as the error code, but subclasses
  // can override if they wish to return more specific error codes.
  virtual bool Write(const void* bytes, size_t count, ErrorCode* error) {
    *error = ErrorCode::kDownloadWriteError;
    return Write(bytes, count);
  }

  // Wrapper around close. Returns 0 on success or -errno on error.
  virtual int Close() = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(FileWriter);
};

// Direct file writer is probably the simplest FileWriter implementation.
// It calls the system calls directly.

class DirectFileWriter : public FileWriter {
 public:
  DirectFileWriter() = default;

  // FileWriter overrides.
  bool Write(const void* bytes, size_t count) override;
  int Close() override;

  // Wrapper around open. Returns 0 on success or -errno on error.
  int Open(const char* path, int flags, mode_t mode);

  int fd() const { return fd_; }

 private:
  int fd_{-1};

  DISALLOW_COPY_AND_ASSIGN(DirectFileWriter);
};

class ScopedFileWriterCloser {
 public:
  explicit ScopedFileWriterCloser(FileWriter* writer) : writer_(writer) {}
  ~ScopedFileWriterCloser() {
    int err = writer_->Close();
    if (err)
      LOG(ERROR) << "FileWriter::Close failed: "
                 << utils::ErrnoNumberAsString(-err);
  }

 private:
  FileWriter* writer_;

  DISALLOW_COPY_AND_ASSIGN(ScopedFileWriterCloser);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_CONSUMER_FILE_WRITER_H_
