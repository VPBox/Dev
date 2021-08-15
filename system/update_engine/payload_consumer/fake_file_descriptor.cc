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

#include "update_engine/payload_consumer/fake_file_descriptor.h"

namespace chromeos_update_engine {

ssize_t FakeFileDescriptor::Read(void* buf, size_t count) {
  // Record the read operation so it can later be inspected.
  read_ops_.emplace_back(offset_, count);

  // Check for the EOF condition first to avoid reporting it as a failure.
  if (offset_ >= static_cast<uint64_t>(size_) || count == 0)
    return 0;
  // Find the first offset greater or equal than the current position where a
  // failure will occur. This will mark the end of the read chunk.
  uint64_t first_failure = size_;
  for (const auto& failure : failure_ranges_) {
    // A failure range that includes the current offset results in an
    // immediate failure to read any bytes.
    if (failure.first <= offset_ && offset_ < failure.first + failure.second) {
      errno = EIO;
      return -1;
    }
    if (failure.first > offset_)
      first_failure = std::min(first_failure, failure.first);
  }
  count = std::min(static_cast<uint64_t>(count), first_failure - offset_);
  static const char kHexChars[] = "0123456789ABCDEF";
  for (size_t i = 0; i < count; ++i) {
    // Encode the 16-bit number "offset_ / 4" as a hex digit in big-endian.
    uint16_t current_num = offset_ / 4;
    uint8_t current_digit = (current_num >> (4 * (3 - offset_ % 4))) & 0x0f;

    static_cast<uint8_t*>(buf)[i] = kHexChars[current_digit];
    offset_++;
  }

  return count;
}

off64_t FakeFileDescriptor::Seek(off64_t offset, int whence) {
  switch (whence) {
    case SEEK_SET:
      offset_ = offset;
      break;
    case SEEK_CUR:
      offset_ += offset;
      break;
    case SEEK_END:
      if (offset > size_)
        offset_ = 0;
      else
        offset_ = size_ - offset_;
      break;
    default:
      errno = EINVAL;
      return -1;
  }
  return offset_;
}

brillo::Blob FakeFileDescriptorData(size_t size) {
  brillo::Blob ret(size);
  FakeFileDescriptor fd;
  fd.SetFileSize(size);
  fd.Read(ret.data(), size);
  return ret;
}

}  // namespace chromeos_update_engine
