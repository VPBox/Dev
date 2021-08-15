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

#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "update_engine/common/test_utils.h"
#include "update_engine/common/utils.h"

using std::string;
using std::vector;

namespace chromeos_update_engine {

class BlockMappingTest : public ::testing::Test {
 protected:
  // Old new partition files used in testing.
  test_utils::ScopedTempFile old_part_{"BlockMappingTest_old.XXXXXX"};
  test_utils::ScopedTempFile new_part_{"BlockMappingTest_new.XXXXXX"};

  size_t block_size_{1024};
  BlockMapping bm_{block_size_};  // BlockMapping under test.
};

TEST_F(BlockMappingTest, FirstAddedBlockIsZero) {
  brillo::Blob blob(block_size_);
  // The BlockMapping just assigns the block ids in order, so it doesn't matter
  // what are the contents of the first block.
  blob[0] = 42;
  EXPECT_EQ(0, bm_.AddBlock(blob));
  blob[0] = 5;
  EXPECT_EQ(1, bm_.AddBlock(blob));
}

TEST_F(BlockMappingTest, BlocksAreNotKeptInMemory) {
  test_utils::WriteFileString(old_part_.path(), string(block_size_, 'a'));
  int old_fd = HANDLE_EINTR(open(old_part_.path().c_str(), O_RDONLY));
  ScopedFdCloser old_fd_closer(&old_fd);

  EXPECT_EQ(0, bm_.AddDiskBlock(old_fd, 0));

  // Check that the block_data is not stored on memory if we just used the block
  // once.
  for (const auto& it : bm_.mapping_) {
    for (const BlockMapping::UniqueBlock& ublock : it.second) {
      EXPECT_TRUE(ublock.block_data.empty());
    }
  }

  brillo::Blob block(block_size_, 'a');
  for (int i = 0; i < 5; ++i) {
    // Re-add the same block 5 times.
    EXPECT_EQ(0, bm_.AddBlock(block));
  }

  for (const auto& it : bm_.mapping_) {
    for (const BlockMapping::UniqueBlock& ublock : it.second) {
      EXPECT_FALSE(ublock.block_data.empty());
      // The block was loaded from disk only 4 times, and after that the counter
      // is not updated anymore.
      EXPECT_EQ(4U, ublock.times_read);
    }
  }
}

TEST_F(BlockMappingTest, MapPartitionBlocks) {
  // A string with 10 blocks where all the blocks are different.
  string old_contents(10 * block_size_, '\0');
  for (size_t i = 0; i < old_contents.size(); ++i)
    old_contents[i] = 4 + i / block_size_;
  test_utils::WriteFileString(old_part_.path(), old_contents);

  // A string including the block with all zeros and overlapping some of the
  // other blocks in old_contents.
  string new_contents(6 * block_size_, '\0');
  for (size_t i = 0; i < new_contents.size(); ++i)
    new_contents[i] = i / block_size_;
  test_utils::WriteFileString(new_part_.path(), new_contents);

  vector<BlockMapping::BlockId> old_ids, new_ids;
  EXPECT_TRUE(MapPartitionBlocks(old_part_.path(),
                                 new_part_.path(),
                                 old_contents.size(),
                                 new_contents.size(),
                                 block_size_,
                                 &old_ids,
                                 &new_ids));

  EXPECT_EQ((vector<BlockMapping::BlockId>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}),
            old_ids);
  EXPECT_EQ((vector<BlockMapping::BlockId>{0, 11, 12, 13, 1, 2}), new_ids);
}

}  // namespace chromeos_update_engine
