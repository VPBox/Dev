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

#ifndef UPDATE_ENGINE_PAYLOAD_GENERATOR_BLOCK_MAPPING_H_
#define UPDATE_ENGINE_PAYLOAD_GENERATOR_BLOCK_MAPPING_H_

#include <map>
#include <string>
#include <vector>

#include <brillo/secure_blob.h>
#include <gtest/gtest_prod.h>  // for FRIEND_TEST

#include "update_engine/payload_generator/payload_generation_config.h"

namespace chromeos_update_engine {

// BlockMapping allows to map data blocks (brillo::Blobs of block_size size)
// into unique integer values called "block ids". This mapping differs from a
// hash function in that two blocks with the same data will have the same id but
// also two blocks with the same id will have the same data. This is only valid
// in the context of the same BlockMapping instance.
class BlockMapping {
 public:
  using BlockId = int64_t;

  explicit BlockMapping(size_t block_size) : block_size_(block_size) {}

  // Add a single data block to the mapping. Returns its unique block id.
  // In case of error returns -1.
  BlockId AddBlock(const brillo::Blob& block_data);

  // Add a block from disk reading it from the file descriptor |fd| from the
  // offset in bytes |byte_offset|. The data block may or may not be cached, so
  // the file descriptor must be available until the BlockMapping is destroyed.
  // Returns the unique block id of the added block or -1 in case of error.
  BlockId AddDiskBlock(int fd, off_t byte_offset);

  // This is a helper method to add |num_blocks| contiguous blocks reading them
  // from the file descriptor |fd| starting at offset |initial_byte_offset|.
  // Returns whether it succeeded to add all the disk blocks and stores in
  // |block_ids| the block id for each one of the added blocks.
  bool AddManyDiskBlocks(int fd,
                         off_t initial_byte_offset,
                         size_t num_blocks,
                         std::vector<BlockId>* block_ids);

 private:
  FRIEND_TEST(BlockMappingTest, BlocksAreNotKeptInMemory);

  // Add a single block passed in |block_data|. If |fd| is not -1, the block
  // can be discarded to save RAM and retrieved later from |fd| at the position
  // |byte_offset|.
  BlockId AddBlock(int fd, off_t byte_offset, const brillo::Blob& block_data);

  size_t block_size_;

  BlockId used_block_ids{0};

  // The UniqueBlock represents the data of a block associated to a unique
  // block id.
  struct UniqueBlock {
    brillo::Blob block_data;

    // The block id assigned to this unique block.
    BlockId block_id;

    // The location on this unique block on disk (if not cached in block_data).
    int fd{-1};
    off_t byte_offset{0};

    // Number of times we have seen this data block. Used for caching.
    uint32_t times_read{0};

    // Compares the UniqueBlock data with the other_block data and stores if
    // they are equal in |equals|. Returns whether there was an error reading
    // the block from disk while comparing it.
    bool CompareData(const brillo::Blob& other_block, bool* equals);
  };

  // A mapping from hash values to possible block ids.
  std::map<size_t, std::vector<UniqueBlock>> mapping_;
};

// Maps the blocks of the old and new partitions |old_part| and |new_part| whose
// size in bytes are |old_size| and |new_size| into block ids where two blocks
// with the same data will have the same block id and vice versa, regardless of
// the partition they are on.
// The block ids number 0 corresponds to the block with all zeros, but any
// other block id number is assigned randomly.
bool MapPartitionBlocks(const std::string& old_part,
                        const std::string& new_part,
                        size_t old_size,
                        size_t new_size,
                        size_t block_size,
                        std::vector<BlockMapping::BlockId>* old_block_ids,
                        std::vector<BlockMapping::BlockId>* new_block_ids);

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_GENERATOR_BLOCK_MAPPING_H_
