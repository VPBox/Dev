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

#include "update_engine/payload_generator/ext2_filesystem.h"

#include <unistd.h>

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

using chromeos_update_engine::test_utils::GetBuildArtifactsPath;
using std::map;
using std::set;
using std::string;
using std::unique_ptr;
using std::vector;

namespace chromeos_update_engine {

namespace {

uint64_t kDefaultFilesystemSize = 4 * 1024 * 1024;
size_t kDefaultFilesystemBlockCount = 1024;
size_t kDefaultFilesystemBlockSize = 4096;

// Checks that all the blocks in |extents| are in the range [0, total_blocks).
void ExpectBlocksInRange(const vector<Extent>& extents, uint64_t total_blocks) {
  for (const Extent& extent : extents) {
    EXPECT_LE(0U, extent.start_block());
    EXPECT_LE(extent.start_block() + extent.num_blocks(), total_blocks);
  }
}

}  // namespace

class Ext2FilesystemTest : public ::testing::Test {};

TEST_F(Ext2FilesystemTest, InvalidFilesystem) {
  test_utils::ScopedTempFile fs_filename_{"Ext2FilesystemTest-XXXXXX"};
  ASSERT_EQ(0, truncate(fs_filename_.path().c_str(), kDefaultFilesystemSize));
  unique_ptr<Ext2Filesystem> fs =
      Ext2Filesystem::CreateFromFile(fs_filename_.path());
  ASSERT_EQ(nullptr, fs.get());

  fs = Ext2Filesystem::CreateFromFile("/path/to/invalid/file");
  ASSERT_EQ(nullptr, fs.get());
}

TEST_F(Ext2FilesystemTest, EmptyFilesystem) {
  unique_ptr<Ext2Filesystem> fs = Ext2Filesystem::CreateFromFile(
      GetBuildArtifactsPath("gen/disk_ext2_4k_empty.img"));

  ASSERT_NE(nullptr, fs.get());
  EXPECT_EQ(kDefaultFilesystemBlockCount, fs->GetBlockCount());
  EXPECT_EQ(kDefaultFilesystemBlockSize, fs->GetBlockSize());

  vector<FilesystemInterface::File> files;
  EXPECT_TRUE(fs->GetFiles(&files));

  map<string, FilesystemInterface::File> map_files;
  for (const auto& file : files) {
    EXPECT_EQ(map_files.end(), map_files.find(file.name))
        << "File " << file.name << " repeated in the list.";
    map_files[file.name] = file;
    ExpectBlocksInRange(file.extents, fs->GetBlockCount());
  }
  EXPECT_EQ(2U, map_files["/"].file_stat.st_ino);
  EXPECT_FALSE(map_files["<free-space>"].extents.empty());
}

// This test parses the sample images generated during build time with the
// "generate_image.sh" script. The expected conditions of each file in these
// images is encoded in the file name, as defined in the mentioned script.
TEST_F(Ext2FilesystemTest, ParseGeneratedImages) {
  const vector<string> kGeneratedImages = {"disk_ext2_1k.img",
                                           "disk_ext2_4k.img"};
  base::FilePath build_path = GetBuildArtifactsPath().Append("gen");
  for (const string& fs_name : kGeneratedImages) {
    LOG(INFO) << "Testing " << fs_name;
    unique_ptr<Ext2Filesystem> fs =
        Ext2Filesystem::CreateFromFile(build_path.Append(fs_name).value());
    ASSERT_NE(nullptr, fs.get());

    vector<FilesystemInterface::File> files;
    map<string, FilesystemInterface::File> map_files;
    set<string> filenames;
    EXPECT_TRUE(fs->GetFiles(&files));
    for (const auto& file : files) {
      // Check no repeated files. We should parse hard-links with two different
      // names.
      EXPECT_EQ(map_files.end(), map_files.find(file.name))
          << "File " << file.name << " repeated in the list.";
      map_files[file.name] = file;
      filenames.insert(file.name);
      ExpectBlocksInRange(file.extents, fs->GetBlockCount());
    }

    // Check that all the files are parsed, and the /removed file should not
    // be included in the list.
    set<string> kExpectedFiles = {
        "/",
        "/cdev",
        "/dir1",
        "/dir1/file",
        "/dir1/dir2",
        "/dir1/dir2/file",
        "/dir1/dir2/dir1",
        "/empty-file",
        "/fifo",
        "/link-hard-regular-16k",
        "/link-long_symlink",
        "/link-short_symlink",
        "/lost+found",
        "/regular-small",
        "/regular-16k",
        "/regular-32k-zeros",
        "/regular-with_net_cap",
        "/sparse_empty-10k",
        "/sparse_empty-2blocks",
        "/sparse-10000blocks",
        "/sparse-16k-last_block",
        "/sparse-16k-first_block",
        "/sparse-16k-holes",
        "<inode-blocks>",
        "<free-space>",
        "<group-descriptors>",
    };
    EXPECT_EQ(kExpectedFiles, filenames);

    FilesystemInterface::File file;

    // Small symlinks don't actually have data blocks.
    EXPECT_TRUE(map_files["/link-short_symlink"].extents.empty());
    EXPECT_EQ(1U,
              utils::BlocksInExtents(map_files["/link-long_symlink"].extents));

    // Hard-links report the same list of blocks.
    EXPECT_EQ(map_files["/link-hard-regular-16k"].extents,
              map_files["/regular-16k"].extents);
    EXPECT_FALSE(map_files["/regular-16k"].extents.empty());

    // The number of blocks in these files doesn't depend on the
    // block size.
    EXPECT_TRUE(map_files["/empty-file"].extents.empty());
    EXPECT_EQ(1U, utils::BlocksInExtents(map_files["/regular-small"].extents));
    EXPECT_EQ(
        1U, utils::BlocksInExtents(map_files["/regular-with_net_cap"].extents));
    EXPECT_TRUE(map_files["/sparse_empty-10k"].extents.empty());
    EXPECT_TRUE(map_files["/sparse_empty-2blocks"].extents.empty());
    EXPECT_EQ(
        1U,
        utils::BlocksInExtents(map_files["/sparse-16k-last_block"].extents));
    EXPECT_EQ(
        1U,
        utils::BlocksInExtents(map_files["/sparse-16k-first_block"].extents));
    EXPECT_EQ(2U,
              utils::BlocksInExtents(map_files["/sparse-16k-holes"].extents));
  }
}

TEST_F(Ext2FilesystemTest, LoadSettingsFailsTest) {
  unique_ptr<Ext2Filesystem> fs = Ext2Filesystem::CreateFromFile(
      GetBuildArtifactsPath("gen/disk_ext2_1k.img"));
  ASSERT_NE(nullptr, fs.get());

  brillo::KeyValueStore store;
  // disk_ext2_1k.img doesn't have the /etc/update_engine.conf file.
  EXPECT_FALSE(fs->LoadSettings(&store));
}

TEST_F(Ext2FilesystemTest, LoadSettingsWorksTest) {
  unique_ptr<Ext2Filesystem> fs = Ext2Filesystem::CreateFromFile(
      GetBuildArtifactsPath("gen/disk_ext2_unittest.img"));
  ASSERT_NE(nullptr, fs.get());

  brillo::KeyValueStore store;
  EXPECT_TRUE(fs->LoadSettings(&store));
  string minor_version;
  EXPECT_TRUE(store.GetString("PAYLOAD_MINOR_VERSION", &minor_version));
  EXPECT_EQ("1234", minor_version);
}

}  // namespace chromeos_update_engine
