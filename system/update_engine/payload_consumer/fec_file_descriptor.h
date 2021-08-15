//
// Copyright (C) 2017 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_PAYLOAD_CONSUMER_FEC_FILE_DESCRIPTOR_H_
#define UPDATE_ENGINE_PAYLOAD_CONSUMER_FEC_FILE_DESCRIPTOR_H_

#include <fec/io.h>

#include "update_engine/payload_consumer/file_descriptor.h"

// A FileDescriptor implementation with error correction based on the "libfec"
// library. The libfec on the running system allows to parse the error
// correction blocks stored in partitions that have verity and error correction
// enabled. This information is present in the raw block device, but of course
// not available via the dm-verity block device.

namespace chromeos_update_engine {

// An error corrected file based on FEC.
class FecFileDescriptor : public FileDescriptor {
 public:
  FecFileDescriptor() = default;
  ~FecFileDescriptor() = default;

  // Interface methods.
  bool Open(const char* path, int flags, mode_t mode) override;
  bool Open(const char* path, int flags) override;
  ssize_t Read(void* buf, size_t count) override;
  ssize_t Write(const void* buf, size_t count) override;
  off64_t Seek(off64_t offset, int whence) override;
  uint64_t BlockDevSize() override;
  bool BlkIoctl(int request,
                uint64_t start,
                uint64_t length,
                int* result) override;
  bool Flush() override { return true; }
  bool Close() override;
  bool IsSettingErrno() override { return true; }
  bool IsOpen() override {
    // The bool operator on the fec::io class tells whether the internal
    // handle is open.
    return static_cast<bool>(fh_);
  }

 protected:
  fec::io fh_;
  uint64_t dev_size_{0};
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_CONSUMER_FEC_FILE_DESCRIPTOR_H_
