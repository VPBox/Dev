//
// Copyright (C) 2016 The Android Open Source Project
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

#include "update_engine/payload_generator/mapfile_filesystem.h"

#include <unistd.h>

#include <map>
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
#include "update_engine/payload_generator/extent_ranges.h"
#include "update_engine/payload_generator/extent_utils.h"

using std::map;
using std::string;
using std::unique_ptr;
using std::vector;

namespace chromeos_update_engine {

namespace {

// Checks that all the blocks in |extents| are in the range [0, total_blocks).
void ExpectBlocksInRange(const vector<Extent>& extents, uint64_t total_blocks) {
  for (const Extent& extent : extents) {
    EXPECT_LE(0U, extent.start_block());
    EXPECT_LE(extent.start_block() + extent.num_blocks(), total_blocks);
  }
}

}  // namespace

class MapfileFilesystemTest : public ::testing::Test {
 protected:
  test_utils::ScopedTempFile temp_file_{"mapfile_file.XXXXXX"};
  test_utils::ScopedTempFile temp_mapfile_{"mapfile_mapfile.XXXXXX"};
};

TEST_F(MapfileFilesystemTest, EmptyFilesystem) {
  unique_ptr<MapfileFilesystem> fs = MapfileFilesystem::CreateFromFile(
      temp_file_.path(), temp_mapfile_.path());
  ASSERT_NE(nullptr, fs.get());

  EXPECT_EQ(0U, fs->GetBlockCount());
  // .map files are always 4KiB blocks.
  EXPECT_EQ(4096U, fs->GetBlockSize());
}

TEST_F(MapfileFilesystemTest, SeveralFileFormatTest) {
  string text =
      "/fileA 1\n"
      "/fileB 2-4\n"
      "/fileC 5-6 9 11-12\n"
      "/file with spaces 14 19\n"
      "/1234 7\n";
  test_utils::WriteFileString(temp_mapfile_.path(), text);
  EXPECT_EQ(0, HANDLE_EINTR(truncate(temp_file_.path().c_str(), 4096 * 20)));

  unique_ptr<MapfileFilesystem> fs = MapfileFilesystem::CreateFromFile(
      temp_file_.path(), temp_mapfile_.path());
  ASSERT_NE(nullptr, fs.get());

  vector<FilesystemInterface::File> files;
  EXPECT_TRUE(fs->GetFiles(&files));

  map<string, FilesystemInterface::File> map_files;
  for (const auto& file : files) {
    EXPECT_EQ(map_files.end(), map_files.find(file.name))
        << "File " << file.name << " repeated in the list.";
    map_files[file.name] = file;
    ExpectBlocksInRange(file.extents, fs->GetBlockCount());
  }

  EXPECT_EQ(map_files["/fileA"].extents,
            (vector<Extent>{ExtentForRange(1, 1)}));
  EXPECT_EQ(map_files["/fileB"].extents,
            (vector<Extent>{ExtentForRange(2, 3)}));
  EXPECT_EQ(
      map_files["/fileC"].extents,
      (vector<Extent>{
          ExtentForRange(5, 2), ExtentForRange(9, 1), ExtentForRange(11, 2)}));
  EXPECT_EQ(map_files["/file with spaces"].extents,
            (vector<Extent>{ExtentForRange(14, 1), ExtentForRange(19, 1)}));
  EXPECT_EQ(map_files["/1234"].extents, (vector<Extent>{ExtentForRange(7, 1)}));
}

TEST_F(MapfileFilesystemTest, BlockNumberTooBigTest) {
  test_utils::WriteFileString(temp_mapfile_.path(), "/some/file 1-4\n");
  EXPECT_EQ(0, HANDLE_EINTR(truncate(temp_file_.path().c_str(), 4096 * 3)));

  unique_ptr<MapfileFilesystem> fs = MapfileFilesystem::CreateFromFile(
      temp_file_.path(), temp_mapfile_.path());
  ASSERT_NE(nullptr, fs.get());

  vector<FilesystemInterface::File> files;
  EXPECT_FALSE(fs->GetFiles(&files));
}

TEST_F(MapfileFilesystemTest, EndBeforeStartTest) {
  test_utils::WriteFileString(temp_mapfile_.path(), "/some/file 2-1\n");
  EXPECT_EQ(0, HANDLE_EINTR(truncate(temp_file_.path().c_str(), 4096 * 3)));

  unique_ptr<MapfileFilesystem> fs = MapfileFilesystem::CreateFromFile(
      temp_file_.path(), temp_mapfile_.path());
  ASSERT_NE(nullptr, fs.get());

  vector<FilesystemInterface::File> files;
  EXPECT_FALSE(fs->GetFiles(&files));
}

}  // namespace chromeos_update_engine
