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

#include "update_engine/payload_consumer/fec_file_descriptor.h"

namespace chromeos_update_engine {

bool FecFileDescriptor::Open(const char* path, int flags) {
  return Open(path, flags, 0600);
}

bool FecFileDescriptor::Open(const char* path, int flags, mode_t mode) {
  if (!fh_.open(path, flags, mode))
    return false;

  if (!fh_.has_ecc()) {
    LOG(ERROR) << "No ECC data in the passed file";
    fh_.close();
    return false;
  }

  fec_status status;
  if (!fh_.get_status(status)) {
    LOG(ERROR) << "Couldn't load ECC status";
    fh_.close();
    return false;
  }

  dev_size_ = status.data_size;
  return true;
}

ssize_t FecFileDescriptor::Read(void* buf, size_t count) {
  return fh_.read(buf, count);
}

ssize_t FecFileDescriptor::Write(const void* buf, size_t count) {
  errno = EROFS;
  return -1;
}

off64_t FecFileDescriptor::Seek(off64_t offset, int whence) {
  if (fh_.seek(offset, whence)) {
    return offset;
  }
  return -1;
}

uint64_t FecFileDescriptor::BlockDevSize() {
  return dev_size_;
}

bool FecFileDescriptor::BlkIoctl(int request,
                                 uint64_t start,
                                 uint64_t length,
                                 int* result) {
  // No IOCTL pass-through in this mode.
  return false;
}

bool FecFileDescriptor::Close() {
  return fh_.close();
}

}  // namespace chromeos_update_engine
