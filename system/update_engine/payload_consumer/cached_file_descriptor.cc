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

#include "update_engine/payload_consumer/cached_file_descriptor.h"

#include <unistd.h>

#include <algorithm>

#include <base/logging.h>

#include "update_engine/common/utils.h"

namespace chromeos_update_engine {

off64_t CachedFileDescriptor::Seek(off64_t offset, int whence) {
  // Only support SEEK_SET and SEEK_CUR. I think these two would be enough. If
  // we want to support SEEK_END then we have to figure out the size of the
  // underlying file descriptor each time and it may not be a very good idea.
  CHECK(whence == SEEK_SET || whence == SEEK_CUR);
  off64_t next_offset = whence == SEEK_SET ? offset : offset_ + offset;

  if (next_offset != offset_) {
    // We sought somewhere other than what we are now. So we have to flush and
    // move to the new offset.
    if (!FlushCache()) {
      return -1;
    }
    // Then we have to seek there.
    if (fd_->Seek(next_offset, SEEK_SET) < 0) {
      return -1;
    }
    offset_ = next_offset;
  }
  return offset_;
}

ssize_t CachedFileDescriptor::Write(const void* buf, size_t count) {
  auto bytes = static_cast<const uint8_t*>(buf);
  size_t total_bytes_wrote = 0;
  while (total_bytes_wrote < count) {
    auto bytes_to_cache =
        std::min(count - total_bytes_wrote, cache_.size() - bytes_cached_);
    if (bytes_to_cache > 0) {  // Which means |cache_| is still have some space.
      memcpy(cache_.data() + bytes_cached_,
             bytes + total_bytes_wrote,
             bytes_to_cache);
      total_bytes_wrote += bytes_to_cache;
      bytes_cached_ += bytes_to_cache;
    }
    if (bytes_cached_ == cache_.size()) {
      // Cache is full; write it to the |fd_| as long as you can.
      if (!FlushCache()) {
        return -1;
      }
    }
  }
  offset_ += total_bytes_wrote;
  return total_bytes_wrote;
}

bool CachedFileDescriptor::Flush() {
  return FlushCache() && fd_->Flush();
}

bool CachedFileDescriptor::Close() {
  offset_ = 0;
  return FlushCache() && fd_->Close();
}

bool CachedFileDescriptor::FlushCache() {
  size_t begin = 0;
  while (begin < bytes_cached_) {
    auto bytes_wrote = fd_->Write(cache_.data() + begin, bytes_cached_ - begin);
    if (bytes_wrote < 0) {
      PLOG(ERROR) << "Failed to flush cached data!";
      return false;
    }
    begin += bytes_wrote;
  }
  bytes_cached_ = 0;
  return true;
}

}  // namespace chromeos_update_engine
