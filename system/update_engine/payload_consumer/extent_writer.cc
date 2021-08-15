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

#include "update_engine/payload_consumer/extent_writer.h"

#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>

#include "update_engine/common/utils.h"
#include "update_engine/payload_consumer/payload_constants.h"

using std::min;

namespace chromeos_update_engine {

bool DirectExtentWriter::Write(const void* bytes, size_t count) {
  if (count == 0)
    return true;
  const char* c_bytes = reinterpret_cast<const char*>(bytes);
  size_t bytes_written = 0;
  while (bytes_written < count) {
    TEST_AND_RETURN_FALSE(cur_extent_ != extents_.end());
    uint64_t bytes_remaining_cur_extent =
        cur_extent_->num_blocks() * block_size_ - extent_bytes_written_;
    CHECK_NE(bytes_remaining_cur_extent, static_cast<uint64_t>(0));
    size_t bytes_to_write =
        static_cast<size_t>(min(static_cast<uint64_t>(count - bytes_written),
                                bytes_remaining_cur_extent));
    TEST_AND_RETURN_FALSE(bytes_to_write > 0);

    if (cur_extent_->start_block() != kSparseHole) {
      const off64_t offset =
          cur_extent_->start_block() * block_size_ + extent_bytes_written_;
      TEST_AND_RETURN_FALSE_ERRNO(fd_->Seek(offset, SEEK_SET) !=
                                  static_cast<off64_t>(-1));
      TEST_AND_RETURN_FALSE(
          utils::WriteAll(fd_, c_bytes + bytes_written, bytes_to_write));
    }
    bytes_written += bytes_to_write;
    extent_bytes_written_ += bytes_to_write;
    if (bytes_remaining_cur_extent == bytes_to_write) {
      // We filled this extent
      CHECK_EQ(extent_bytes_written_, cur_extent_->num_blocks() * block_size_);
      // move to next extent
      extent_bytes_written_ = 0;
      cur_extent_++;
    }
  }
  return true;
}

}  // namespace chromeos_update_engine
