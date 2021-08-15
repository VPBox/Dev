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

#ifndef UPDATE_ENGINE_PAYLOAD_CONSUMER_FAKE_FILE_DESCRIPTOR_H_
#define UPDATE_ENGINE_PAYLOAD_CONSUMER_FAKE_FILE_DESCRIPTOR_H_

#include <algorithm>
#include <limits>
#include <utility>
#include <vector>

#include <brillo/secure_blob.h>

#include "update_engine/payload_consumer/file_descriptor.h"

namespace chromeos_update_engine {

// A fake file descriptor with configurable errors. The file descriptor always
// reads a fixed sequence of bytes, consisting of the concatenation of the
// numbers 0, 1, 2... each one encoded in 4 bytes as the big-endian 16-bit
// number encoded in hexadecimal. For example, the beginning of the stream in
// ASCII is 0000000100020003... which corresponds to the numbers 0, 1, 2 and 3.
class FakeFileDescriptor : public FileDescriptor {
 public:
  FakeFileDescriptor() = default;
  ~FakeFileDescriptor() override = default;

  // FileDescriptor override methods.
  bool Open(const char* path, int flags, mode_t mode) override {
    if (open_)
      return false;
    open_ = true;
    return true;
  }

  bool Open(const char* path, int flags) override {
    return Open(path, flags, 0);
  }

  ssize_t Read(void* buf, size_t count) override;

  ssize_t Write(const void* buf, size_t count) override {
    // Read-only block device.
    errno = EROFS;
    return -1;
  }

  off64_t Seek(off64_t offset, int whence) override;

  uint64_t BlockDevSize() override { return size_; }

  bool BlkIoctl(int request,
                uint64_t start,
                uint64_t length,
                int* result) override {
    return false;
  }

  bool Flush() override { return open_; }

  bool Close() override {
    if (!open_)
      return false;
    open_ = false;
    return true;
  }

  bool IsSettingErrno() override { return true; }

  bool IsOpen() override { return open_; }

  // Fake class configuration methods.

  // Set the size of the file.
  void SetFileSize(uint64_t size) { size_ = size; }

  // Marks the range starting from |offset| bytes into the file and |length|
  // size as a failure range. Reads from this range will always fail.
  void AddFailureRange(uint64_t offset, uint64_t length) {
    if (length == 0)
      return;
    failure_ranges_.emplace_back(offset, length);
  }

  // Return the list of ranges of bytes requested with a Read() as (offset,
  // length), regardless of the Read() return value.
  std::vector<std::pair<uint64_t, uint64_t>> GetReadOps() const {
    return read_ops_;
  }

 private:
  // Whether the fake file is open.
  bool open_{false};

  // The current file pointer offset into the fake file.
  uint64_t offset_{0};

  // The size of the file. Reads beyond |max_size_| will an EOF condition.
  off64_t size_{std::numeric_limits<off64_t>::max()};

  // The list of ranges represented as (start, length) in bytes where reads will
  // always fail.
  std::vector<std::pair<uint64_t, uint64_t>> failure_ranges_;

  // List of reads performed as (offset, length) of the read request.
  std::vector<std::pair<uint64_t, uint64_t>> read_ops_;

  DISALLOW_COPY_AND_ASSIGN(FakeFileDescriptor);
};

// Return a blob with the first |size| bytes of a FakeFileDescriptor stream.
brillo::Blob FakeFileDescriptorData(size_t size);

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_CONSUMER_FAKE_FILE_DESCRIPTOR_H_
