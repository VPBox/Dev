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

#include "update_engine/payload_consumer/extent_reader.h"

#include <algorithm>

#include <sys/types.h>
#include <unistd.h>

#include "update_engine/common/utils.h"
#include "update_engine/payload_consumer/payload_constants.h"

using google::protobuf::RepeatedPtrField;

namespace chromeos_update_engine {

bool DirectExtentReader::Init(FileDescriptorPtr fd,
                              const RepeatedPtrField<Extent>& extents,
                              uint32_t block_size) {
  fd_ = fd;
  extents_ = extents;
  block_size_ = block_size;
  cur_extent_ = extents_.begin();

  extents_upper_bounds_.reserve(extents_.size() + 1);
  // We add this pad as the first element to not bother with boundary checks
  // later.
  extents_upper_bounds_.emplace_back(0);
  for (const auto& extent : extents_) {
    total_size_ += extent.num_blocks() * block_size_;
    extents_upper_bounds_.emplace_back(total_size_);
  }
  return true;
}

bool DirectExtentReader::Seek(uint64_t offset) {
  TEST_AND_RETURN_FALSE(offset <= total_size_);
  if (offset_ == offset) {
    return true;
  }
  // The first item is zero and upper_bound never returns it because it always
  // return the item which is greater than the given value.
  auto extent_idx =
      std::upper_bound(
          extents_upper_bounds_.begin(), extents_upper_bounds_.end(), offset) -
      extents_upper_bounds_.begin() - 1;
  cur_extent_ = std::next(extents_.begin(), extent_idx);
  offset_ = offset;
  cur_extent_bytes_read_ = offset_ - extents_upper_bounds_[extent_idx];
  return true;
}

bool DirectExtentReader::Read(void* buffer, size_t count) {
  auto bytes = reinterpret_cast<uint8_t*>(buffer);
  uint64_t bytes_read = 0;
  while (bytes_read < count) {
    if (cur_extent_ == extents_.end()) {
      TEST_AND_RETURN_FALSE(bytes_read == count);
    }
    uint64_t cur_extent_bytes_left =
        cur_extent_->num_blocks() * block_size_ - cur_extent_bytes_read_;
    uint64_t bytes_to_read =
        std::min(count - bytes_read, cur_extent_bytes_left);

    ssize_t out_bytes_read;
    TEST_AND_RETURN_FALSE(utils::PReadAll(
        fd_,
        bytes + bytes_read,
        bytes_to_read,
        cur_extent_->start_block() * block_size_ + cur_extent_bytes_read_,
        &out_bytes_read));
    TEST_AND_RETURN_FALSE(out_bytes_read ==
                          static_cast<ssize_t>(bytes_to_read));

    bytes_read += bytes_to_read;
    cur_extent_bytes_read_ += bytes_to_read;
    offset_ += bytes_to_read;
    if (cur_extent_bytes_read_ == cur_extent_->num_blocks() * block_size_) {
      // We have to advance the cur_extent_;
      cur_extent_++;
      cur_extent_bytes_read_ = 0;
    }
  }
  return true;
}

}  // namespace chromeos_update_engine
