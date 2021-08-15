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

#include "update_engine/payload_generator/squashfs_filesystem.h"

#include <unistd.h>

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>

#include <base/format_macros.h>
#include <base/logging.h>
#include <base/strings/string_number_conversions.h>
#include <base/strings/string_util.h>
#include <base/strings/stringprintf.h>
#include <gtest/gtest.h>

#include "update_engine/common/test_utils.h"
#include "update_engine/common/utils.h"
#include "update_engine/payload_generator/extent_utils.h"

namespace chromeos_update_engine {

using std::map;
using std::set;
using std::string;
using std::unique_ptr;
using std::vector;

using test_utils::GetBuildArtifactsPath;

namespace {

constexpr uint64_t kTestBlockSize = 4096;
constexpr uint64_t kTestSqfsBlockSize = 1 << 15;

// Checks that all the blocks in |extents| are in the range [0, total_blocks).
void ExpectBlocksInRange(const vector<Extent>& extents, uint64_t total_blocks) {
  for (const Extent& extent : extents) {
    EXPECT_LE(0U, extent.start_block());
    EXPECT_LE(extent.start_block() + extent.num_blocks(), total_blocks);
  }
}

SquashfsFilesystem::SquashfsHeader GetSimpleHeader() {
  // These properties are enough for now. Add more as needed.
  return {
      .magic = 0x73717368,
      .block_size = kTestSqfsBlockSize,
      .compression_type = 1,  // For gzip.
      .major_version = 4,
  };
}

}  // namespace

class SquashfsFilesystemTest : public ::testing::Test {
 public:
  void CheckSquashfs(const unique_ptr<SquashfsFilesystem>& fs) {
    ASSERT_TRUE(fs);
    EXPECT_EQ(kTestBlockSize, fs->GetBlockSize());

    vector<FilesystemInterface::File> files;
    ASSERT_TRUE(fs->GetFiles(&files));

    map<string, FilesystemInterface::File> map_files;
    for (const auto& file : files) {
      EXPECT_EQ(map_files.end(), map_files.find(file.name))
          << "File " << file.name << " repeated in the list.";
      map_files[file.name] = file;
      ExpectBlocksInRange(file.extents, fs->GetBlockCount());
    }

    // Checking the sortness.
    EXPECT_TRUE(std::is_sorted(files.begin(),
                               files.end(),
                               [](const FilesystemInterface::File& a,
                                  const FilesystemInterface::File& b) {
                                 return a.extents[0].start_block() <
                                        b.extents[0].start_block();
                               }));

    auto overlap_check = [](const FilesystemInterface::File& a,
                            const FilesystemInterface::File& b) {
      // Return true if overlapping.
      return a.extents[0].start_block() + a.extents[0].num_blocks() >
             b.extents[0].start_block();
    };
    // Check files are not overlapping.
    EXPECT_EQ(std::adjacent_find(files.begin(), files.end(), overlap_check),
              files.end());
  }
};

// CreateFromFile() depends on unsquashfs -m, which only exists in Chrome OS.
#ifdef __CHROMEOS__
TEST_F(SquashfsFilesystemTest, EmptyFilesystemTest) {
  unique_ptr<SquashfsFilesystem> fs = SquashfsFilesystem::CreateFromFile(
      GetBuildArtifactsPath("gen/disk_sqfs_empty.img"), true);
  CheckSquashfs(fs);

  // Even an empty squashfs filesystem is rounded up to 4K.
  EXPECT_EQ(4096 / kTestBlockSize, fs->GetBlockCount());

  vector<FilesystemInterface::File> files;
  ASSERT_TRUE(fs->GetFiles(&files));
  ASSERT_EQ(files.size(), 1u);

  FilesystemInterface::File file;
  file.name = "<metadata-0>";
  file.extents.emplace_back();
  file.extents[0].set_start_block(0);
  file.extents[0].set_num_blocks(1);
  EXPECT_EQ(files[0].name, file.name);
  EXPECT_EQ(files[0].extents, file.extents);
}

TEST_F(SquashfsFilesystemTest, DefaultFilesystemTest) {
  unique_ptr<SquashfsFilesystem> fs = SquashfsFilesystem::CreateFromFile(
      GetBuildArtifactsPath("gen/disk_sqfs_default.img"), true);
  CheckSquashfs(fs);

  vector<FilesystemInterface::File> files;
  ASSERT_TRUE(fs->GetFiles(&files));
  ASSERT_EQ(files.size(), 1u);

  FilesystemInterface::File file;
  file.name = "<fragment-0>";
  file.extents.emplace_back();
  file.extents[0].set_start_block(0);
  file.extents[0].set_num_blocks(1);
  EXPECT_EQ(files[0].name, file.name);
  EXPECT_EQ(files[0].extents, file.extents);
}
#endif  // __CHROMEOS__

TEST_F(SquashfsFilesystemTest, SimpleFileMapTest) {
  string filemap = R"(dir1/file1 96 4000
                      dir1/file2 4096 100)";
  unique_ptr<SquashfsFilesystem> fs = SquashfsFilesystem::CreateFromFileMap(
      filemap, kTestBlockSize * 2, GetSimpleHeader());
  CheckSquashfs(fs);

  vector<FilesystemInterface::File> files;
  ASSERT_TRUE(fs->GetFiles(&files));
  EXPECT_EQ(files.size(), 2u);
}

TEST_F(SquashfsFilesystemTest, FileMapZeroSizeFileTest) {
  // The second file's size is zero.
  string filemap = R"(dir1/file1 96 4000
                      dir1/file2 4096
                      dir1/file3 4096 100)";
  unique_ptr<SquashfsFilesystem> fs = SquashfsFilesystem::CreateFromFileMap(
      filemap, kTestBlockSize * 2, GetSimpleHeader());
  CheckSquashfs(fs);

  vector<FilesystemInterface::File> files;
  ASSERT_TRUE(fs->GetFiles(&files));
  // The second and third files are removed. The file with size zero is removed.
  EXPECT_EQ(files.size(), 2u);
}

// Testing the compressed bit.
TEST_F(SquashfsFilesystemTest, CompressedBitTest) {
  string filemap = "dir1/file1 0 " + std::to_string(4000 | (1 << 24)) + "\n";
  unique_ptr<SquashfsFilesystem> fs = SquashfsFilesystem::CreateFromFileMap(
      filemap, kTestBlockSize, GetSimpleHeader());
  CheckSquashfs(fs);

  vector<FilesystemInterface::File> files;
  ASSERT_TRUE(fs->GetFiles(&files));
  ASSERT_EQ(files.size(), 1u);
  EXPECT_EQ(files[0].extents[0].num_blocks(), 1u);
}

// Test overlap.
TEST_F(SquashfsFilesystemTest, OverlapingFiles1Test) {
  string filemap = R"(file1 0 6000
                      file2 5000 5000)";
  unique_ptr<SquashfsFilesystem> fs = SquashfsFilesystem::CreateFromFileMap(
      filemap, kTestBlockSize * 3, GetSimpleHeader());
  CheckSquashfs(fs);

  vector<FilesystemInterface::File> files;
  ASSERT_TRUE(fs->GetFiles(&files));
  ASSERT_EQ(files.size(), 2u);
  EXPECT_EQ(files[0].extents[0].num_blocks(), 1u);
  EXPECT_EQ(files[1].extents[0].num_blocks(), 2u);
}

// Test overlap, first inside second.
TEST_F(SquashfsFilesystemTest, OverlapingFiles2Test) {
  string filemap = R"(file1 0 4000
                      file2 0 6000)";
  unique_ptr<SquashfsFilesystem> fs = SquashfsFilesystem::CreateFromFileMap(
      filemap, kTestBlockSize * 2, GetSimpleHeader());
  CheckSquashfs(fs);

  vector<FilesystemInterface::File> files;
  ASSERT_TRUE(fs->GetFiles(&files));
  ASSERT_EQ(files.size(), 1u);
  EXPECT_EQ(files[0].name, "file2");
  EXPECT_EQ(files[0].extents[0].num_blocks(), 2u);
}

// Test overlap, second inside first.
TEST_F(SquashfsFilesystemTest, OverlapingFiles3Test) {
  string filemap = R"(file1 0 8000
                      file2 100 100)";
  unique_ptr<SquashfsFilesystem> fs = SquashfsFilesystem::CreateFromFileMap(
      filemap, kTestBlockSize * 2, GetSimpleHeader());
  CheckSquashfs(fs);

  vector<FilesystemInterface::File> files;
  ASSERT_TRUE(fs->GetFiles(&files));
  ASSERT_EQ(files.size(), 1u);
  EXPECT_EQ(files[0].name, "file1");
  EXPECT_EQ(files[0].extents[0].num_blocks(), 2u);
}

// Fail a line with only one argument.
TEST_F(SquashfsFilesystemTest, FailOnlyFileNameTest) {
  string filemap = "dir1/file1\n";
  unique_ptr<SquashfsFilesystem> fs = SquashfsFilesystem::CreateFromFileMap(
      filemap, kTestBlockSize, GetSimpleHeader());
  EXPECT_FALSE(fs);
}

// Fail a line with space separated filen name
TEST_F(SquashfsFilesystemTest, FailSpaceInFileNameTest) {
  string filemap = "dir1 file1 0 10\n";
  unique_ptr<SquashfsFilesystem> fs = SquashfsFilesystem::CreateFromFileMap(
      filemap, kTestBlockSize, GetSimpleHeader());
  EXPECT_FALSE(fs);
}

// Fail empty line
TEST_F(SquashfsFilesystemTest, FailEmptyLineTest) {
  // The second file's size is zero.
  string filemap = R"(
  /t
                      dir1/file3 4096 100)";
  unique_ptr<SquashfsFilesystem> fs = SquashfsFilesystem::CreateFromFileMap(
      filemap, kTestBlockSize * 2, GetSimpleHeader());
  EXPECT_FALSE(fs);
}

// Fail on bad magic or major
TEST_F(SquashfsFilesystemTest, FailBadMagicOrMajorTest) {
  string filemap = "dir1/file1 0 10\n";
  auto header = GetSimpleHeader();
  header.magic = 1;
  EXPECT_FALSE(
      SquashfsFilesystem::CreateFromFileMap(filemap, kTestBlockSize, header));

  header = GetSimpleHeader();
  header.major_version = 3;
  EXPECT_FALSE(
      SquashfsFilesystem::CreateFromFileMap(filemap, kTestBlockSize, header));
}

// Fail size with larger than block_size
TEST_F(SquashfsFilesystemTest, FailLargerThanBlockSizeTest) {
  string filemap = "file1 0 " + std::to_string(kTestSqfsBlockSize + 1) + "\n";
  unique_ptr<SquashfsFilesystem> fs = SquashfsFilesystem::CreateFromFileMap(
      filemap, kTestBlockSize, GetSimpleHeader());
  EXPECT_FALSE(fs);
}

// Test is squashfs image.
TEST_F(SquashfsFilesystemTest, IsSquashfsImageTest) {
  // Some sample from a recent squashfs file.
  brillo::Blob super_block = {
      0x68, 0x73, 0x71, 0x73, 0x59, 0x05, 0x00, 0x00, 0x09, 0x3a, 0x89, 0x58,
      0x00, 0x00, 0x02, 0x00, 0x9a, 0x00, 0x00, 0x00, 0x01, 0x00, 0x11, 0x00,
      0xc0, 0x00, 0x06, 0x00, 0x04, 0x00, 0x00, 0x00, 0x89, 0x18, 0xf7, 0x7c,
      0x00, 0x00, 0x00, 0x00, 0x2e, 0x33, 0xcd, 0x16, 0x00, 0x00, 0x00, 0x00,
      0x3a, 0x30, 0xcd, 0x16, 0x00, 0x00, 0x00, 0x00, 0x16, 0x33, 0xcd, 0x16,
      0x00, 0x00, 0x00, 0x00, 0x07, 0x62, 0xcc, 0x16, 0x00, 0x00, 0x00, 0x00,
      0x77, 0xe6, 0xcc, 0x16, 0x00, 0x00, 0x00, 0x00, 0x1a, 0x25, 0xcd, 0x16,
      0x00, 0x00, 0x00, 0x00, 0x10, 0x30, 0xcd, 0x16, 0x00, 0x00, 0x00, 0x00};

  EXPECT_TRUE(SquashfsFilesystem::IsSquashfsImage(super_block));

  // Bad magic
  auto bad_super_block = super_block;
  bad_super_block[1] = 0x02;
  EXPECT_FALSE(SquashfsFilesystem::IsSquashfsImage(bad_super_block));

  // Bad major
  bad_super_block = super_block;
  bad_super_block[28] = 0x03;
  EXPECT_FALSE(SquashfsFilesystem::IsSquashfsImage(bad_super_block));

  // Small size;
  bad_super_block = super_block;
  bad_super_block.resize(10);
  EXPECT_FALSE(SquashfsFilesystem::IsSquashfsImage(bad_super_block));
}

}  // namespace chromeos_update_engine
