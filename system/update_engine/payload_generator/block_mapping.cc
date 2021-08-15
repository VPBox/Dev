//
// Copyright (C) 2015 The Android Open Source Project
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

#include "update_engine/payload_generator/block_mapping.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <functional>
#include <string>
#include <utility>
#include <vector>

#include "update_engine/common/utils.h"

using std::string;
using std::vector;

namespace {

size_t HashValue(const brillo::Blob& blob) {
  std::hash<string> hash_fn;
  return hash_fn(string(blob.begin(), blob.end()));
}

}  // namespace

namespace chromeos_update_engine {

BlockMapping::BlockId BlockMapping::AddBlock(const brillo::Blob& block_data) {
  return AddBlock(-1, 0, block_data);
}

BlockMapping::BlockId BlockMapping::AddDiskBlock(int fd, off_t byte_offset) {
  brillo::Blob blob(block_size_);
  ssize_t bytes_read = 0;
  if (!utils::PReadAll(fd, blob.data(), block_size_, byte_offset, &bytes_read))
    return -1;
  if (static_cast<size_t>(bytes_read) != block_size_)
    return -1;
  return AddBlock(fd, byte_offset, blob);
}

bool BlockMapping::AddManyDiskBlocks(int fd,
                                     off_t initial_byte_offset,
                                     size_t num_blocks,
                                     vector<BlockId>* block_ids) {
  bool ret = true;
  block_ids->resize(num_blocks);
  for (size_t block = 0; block < num_blocks; block++) {
    (*block_ids)[block] =
        AddDiskBlock(fd, initial_byte_offset + block * block_size_);
    ret = ret && (*block_ids)[block] != -1;
  }
  return ret;
}

BlockMapping::BlockId BlockMapping::AddBlock(int fd,
                                             off_t byte_offset,
                                             const brillo::Blob& block_data) {
  if (block_data.size() != block_size_)
    return -1;
  size_t h = HashValue(block_data);

  // We either reuse a UniqueBlock or create a new one. If we need a new
  // UniqueBlock it could also be part of a new or existing bucket (if there is
  // a hash collision).
  vector<UniqueBlock>* bucket = nullptr;

  auto mapping_it = mapping_.find(h);
  if (mapping_it == mapping_.end()) {
    bucket = &mapping_[h];
  } else {
    for (UniqueBlock& existing_block : mapping_it->second) {
      bool equals = false;
      if (!existing_block.CompareData(block_data, &equals))
        return -1;
      if (equals)
        return existing_block.block_id;
    }
    bucket = &mapping_it->second;
  }

  // No existing block was found at this point, so we create and fill in a new
  // one.
  bucket->emplace_back();
  UniqueBlock* new_ublock = &bucket->back();

  new_ublock->times_read = 1;
  new_ublock->fd = fd;
  new_ublock->byte_offset = byte_offset;
  new_ublock->block_id = used_block_ids++;
  // We need to cache blocks that are not referencing any disk location.
  if (fd == -1)
    new_ublock->block_data = block_data;

  return new_ublock->block_id;
}

bool BlockMapping::UniqueBlock::CompareData(const brillo::Blob& other_block,
                                            bool* equals) {
  if (!block_data.empty()) {
    *equals = block_data == other_block;
    return true;
  }
  const size_t block_size = other_block.size();
  brillo::Blob blob(block_size);
  ssize_t bytes_read = 0;
  if (!utils::PReadAll(fd, blob.data(), block_size, byte_offset, &bytes_read))
    return false;
  if (static_cast<size_t>(bytes_read) != block_size)
    return false;
  *equals = blob == other_block;

  // We increase the number of times we had to read this block from disk and
  // we cache this block based on that. This caching method is optimized for
  // the common use case of having two partitions that share blocks between them
  // but have few repeated blocks inside each partition, such as the block
  // with all zeros or duplicated files.
  times_read++;
  if (times_read > 3)
    block_data = std::move(blob);
  return true;
}

bool MapPartitionBlocks(const string& old_part,
                        const string& new_part,
                        size_t old_size,
                        size_t new_size,
                        size_t block_size,
                        vector<BlockMapping::BlockId>* old_block_ids,
                        vector<BlockMapping::BlockId>* new_block_ids) {
  BlockMapping mapping(block_size);
  if (mapping.AddBlock(brillo::Blob(block_size, '\0')) != 0)
    return false;
  int old_fd = HANDLE_EINTR(open(old_part.c_str(), O_RDONLY));
  int new_fd = HANDLE_EINTR(open(new_part.c_str(), O_RDONLY));
  ScopedFdCloser old_fd_closer(&old_fd);
  ScopedFdCloser new_fd_closer(&new_fd);

  TEST_AND_RETURN_FALSE(mapping.AddManyDiskBlocks(
      old_fd, 0, old_size / block_size, old_block_ids));
  TEST_AND_RETURN_FALSE(mapping.AddManyDiskBlocks(
      new_fd, 0, new_size / block_size, new_block_ids));
  return true;
}

}  // namespace chromeos_update_engine
