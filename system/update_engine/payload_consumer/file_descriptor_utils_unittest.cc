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

#include "update_engine/payload_consumer/file_descriptor_utils.h"

#include <fcntl.h>

#include <string>
#include <utility>
#include <vector>

#include <brillo/data_encoding.h>
#include <gtest/gtest.h>

#include "update_engine/common/hash_calculator.h"
#include "update_engine/common/test_utils.h"
#include "update_engine/common/utils.h"
#include "update_engine/payload_consumer/fake_file_descriptor.h"
#include "update_engine/payload_consumer/file_descriptor.h"
#include "update_engine/payload_generator/extent_ranges.h"

using google::protobuf::RepeatedPtrField;

namespace chromeos_update_engine {

namespace {

RepeatedPtrField<Extent> CreateExtentList(
    const std::vector<std::pair<uint64_t, uint64_t>>& lst) {
  RepeatedPtrField<Extent> result;
  for (const auto& item : lst) {
    *result.Add() = ExtentForRange(item.first, item.second);
  }
  return result;
}

}  // namespace

class FileDescriptorUtilsTest : public ::testing::Test {
 protected:
  void SetUp() override {
    EXPECT_TRUE(utils::MakeTempFile("fd_tgt.XXXXXX", &tgt_path_, nullptr));
    EXPECT_TRUE(target_->Open(tgt_path_.c_str(), O_RDWR));
  }

  // Check that the |target_| file contains |expected_contents|.
  void ExpectTarget(const std::string& expected_contents) {
    std::string target_contents;
    EXPECT_TRUE(utils::ReadFile(tgt_path_, &target_contents));
    EXPECT_EQ(expected_contents.size(), target_contents.size());
    if (target_contents != expected_contents) {
      ADD_FAILURE() << "Contents don't match.";
      LOG(INFO) << "Expected contents:";
      utils::HexDumpString(expected_contents);
      LOG(INFO) << "Actual contents:";
      utils::HexDumpString(target_contents);
    }
  }

  // Path to the target temporary file.
  std::string tgt_path_;

  // Source and target file descriptor used for testing the tools.
  FakeFileDescriptor* fake_source_{new FakeFileDescriptor()};
  FileDescriptorPtr source_{fake_source_};
  FileDescriptorPtr target_{new EintrSafeFileDescriptor()};
};

// Source and target extents should have the same number of blocks.
TEST_F(FileDescriptorUtilsTest, CopyAndHashExtentsMismatchBlocksTest) {
  auto src_extents = CreateExtentList({{1, 4}});
  auto tgt_extents = CreateExtentList({{0, 5}});

  EXPECT_FALSE(fd_utils::CopyAndHashExtents(
      source_, src_extents, target_, tgt_extents, 4, nullptr));
}

// Failing to read from the source should fail the copy.
TEST_F(FileDescriptorUtilsTest, CopyAndHashExtentsReadFailureTest) {
  auto extents = CreateExtentList({{0, 5}});
  fake_source_->AddFailureRange(10, 5);

  EXPECT_FALSE(fd_utils::CopyAndHashExtents(
      source_, extents, target_, extents, 4, nullptr));
}

// Failing to write to the target should fail the copy.
TEST_F(FileDescriptorUtilsTest, CopyAndHashExtentsWriteFailureTest) {
  auto src_extents = CreateExtentList({{0, 2}});
  auto tgt_extents = CreateExtentList({{5, 2}});
  fake_source_->AddFailureRange(5 * 4, 10);

  // Note that we pass |source_| as the target as well, which should fail to
  // write.
  EXPECT_FALSE(fd_utils::CopyAndHashExtents(
      source_, src_extents, source_, tgt_extents, 4, nullptr));
}

// Test that we can copy extents without hashing them, allowing a nullptr
// pointer as hash_out.
TEST_F(FileDescriptorUtilsTest, CopyAndHashExtentsWithoutHashingTest) {
  auto extents = CreateExtentList({{0, 5}});

  EXPECT_TRUE(fd_utils::CopyAndHashExtents(
      source_, extents, target_, extents, 4, nullptr));
  ExpectTarget("00000001000200030004");
}

// CopyAndHash() can take different number of extents in the source and target
// files, as long as the number of blocks is the same. Test that it handles it
// properly.
TEST_F(FileDescriptorUtilsTest, CopyAndHashExtentsManyToOneTest) {
  brillo::Blob hash_out;
  // Reorder the input as 1 4 2 3 0.
  auto src_extents = CreateExtentList({{1, 1}, {4, 1}, {2, 2}, {0, 1}});
  auto tgt_extents = CreateExtentList({{0, 5}});

  EXPECT_TRUE(fd_utils::CopyAndHashExtents(
      source_, src_extents, target_, tgt_extents, 4, &hash_out));
  const char kExpectedResult[] = "00010004000200030000";
  ExpectTarget(kExpectedResult);

  brillo::Blob expected_hash;
  EXPECT_TRUE(HashCalculator::RawHashOfBytes(
      kExpectedResult, strlen(kExpectedResult), &expected_hash));
  EXPECT_EQ(expected_hash, hash_out);
}

TEST_F(FileDescriptorUtilsTest, CopyAndHashExtentsManyToManyTest) {
  brillo::Blob hash_out;
  auto src_extents = CreateExtentList({{1, 1}, {4, 1}, {2, 2}, {0, 1}});
  auto tgt_extents = CreateExtentList({{2, 3}, {0, 2}});

  EXPECT_TRUE(fd_utils::CopyAndHashExtents(
      source_, src_extents, target_, tgt_extents, 4, &hash_out));
  // The reads always match the source extent list of blocks (up to the
  // internal buffer size).
  std::vector<std::pair<uint64_t, uint64_t>> kExpectedOps = {
      {4, 4}, {16, 4}, {8, 8}, {0, 4}};
  EXPECT_EQ(kExpectedOps, fake_source_->GetReadOps());

  // The output here is as in the previous test but the first 3 4-byte blocks
  // are at the end of the stream. The expected hash is as in the previous
  // example anyway since the hash doesn't depend on the order of the target
  // blocks.
  const char kExpectedResult[] = "00030000000100040002";
  ExpectTarget(kExpectedResult);

  // The data in the order that the reader processes (and hashes) it.
  const char kExpectedOrderedData[] = "00010004000200030000";
  brillo::Blob expected_hash;
  EXPECT_TRUE(HashCalculator::RawHashOfBytes(
      kExpectedOrderedData, strlen(kExpectedOrderedData), &expected_hash));
  EXPECT_EQ(expected_hash, hash_out);
}

// Failing to read from the source should fail the hash calculation.
TEST_F(FileDescriptorUtilsTest, ReadAndHashExtentsReadFailureTest) {
  auto extents = CreateExtentList({{0, 5}});
  fake_source_->AddFailureRange(10, 5);
  brillo::Blob hash_out;
  EXPECT_FALSE(fd_utils::ReadAndHashExtents(source_, extents, 4, &hash_out));
}

// Test that if hash_out is null, it still works.
TEST_F(FileDescriptorUtilsTest, ReadAndHashExtentsWithoutHashingTest) {
  auto extents = CreateExtentList({{0, 5}});
  EXPECT_TRUE(fd_utils::ReadAndHashExtents(source_, extents, 4, nullptr));
}

// Tests that it can calculate the hash properly.
TEST_F(FileDescriptorUtilsTest, ReadAndHashExtentsTest) {
  // Reorder the input as 1 4 2 3 0.
  auto extents = CreateExtentList({{1, 1}, {4, 1}, {2, 2}, {0, 1}});
  brillo::Blob hash_out;
  EXPECT_TRUE(fd_utils::ReadAndHashExtents(source_, extents, 4, &hash_out));

  const char kExpectedResult[] = "00010004000200030000";
  brillo::Blob expected_hash;
  EXPECT_TRUE(HashCalculator::RawHashOfBytes(
      kExpectedResult, strlen(kExpectedResult), &expected_hash));
  EXPECT_EQ(expected_hash, hash_out);
}

}  // namespace chromeos_update_engine
