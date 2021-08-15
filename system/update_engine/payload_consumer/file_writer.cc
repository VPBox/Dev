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

#include "update_engine/payload_consumer/file_writer.h"

#include <errno.h>

namespace chromeos_update_engine {

int DirectFileWriter::Open(const char* path, int flags, mode_t mode) {
  CHECK_EQ(fd_, -1);
  fd_ = open(path, flags, mode);
  if (fd_ < 0)
    return -errno;
  return 0;
}

bool DirectFileWriter::Write(const void* bytes, size_t count) {
  CHECK_GE(fd_, 0);
  const char* char_bytes = reinterpret_cast<const char*>(bytes);

  size_t bytes_written = 0;
  while (bytes_written < count) {
    ssize_t rc = write(fd_, char_bytes + bytes_written, count - bytes_written);
    if (rc < 0)
      return false;
    bytes_written += rc;
  }
  CHECK_EQ(bytes_written, count);
  return bytes_written == count;
}

int DirectFileWriter::Close() {
  CHECK_GE(fd_, 0);
  int rc = close(fd_);

  // This can be any negative number that's not -1. This way, this FileWriter
  // won't be used again for another file.
  fd_ = -2;

  if (rc < 0)
    return -errno;
  return rc;
}

}  // namespace chromeos_update_engine
