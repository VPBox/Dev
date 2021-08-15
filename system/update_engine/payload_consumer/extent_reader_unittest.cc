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

#include <fcntl.h>

#include <algorithm>
#include <string>
#include <vector>

#include <brillo/secure_blob.h>
#include <gtest/gtest.h>

#include "update_engine/common/test_utils.h"
#include "update_engine/common/utils.h"
#include "update_engine/payload_consumer/file_descriptor.h"
#include "update_engine/payload_consumer/payload_constants.h"
#include "update_engine/payload_generator/extent_ranges.h"
#include "update_engine/payload_generator/extent_utils.h"

using chromeos_update_engine::test_utils::ExpectVectorsEq;
using std::min;
using std::string;
using std::vector;

namespace chromeos_update_engine {

namespace {
const size_t kBlockSize = 8;
const size_t kRandomIterations = 1000;
}  // namespace

class ExtentReaderTest : public ::testing::Test {
 protected:
  void SetUp() override {
    sample_.resize(4096 * 10);
    srand(time(nullptr));
    unsigned int rand_seed;
    for (size_t i = 0; i < sample_.size(); i++) {
      sample_[i] = rand_r(&rand_seed) % 256;
    }
    ASSERT_TRUE(utils::WriteFile(
        temp_file_.path().c_str(), sample_.data(), sample_.size()));

    fd_.reset(new EintrSafeFileDescriptor());
    ASSERT_TRUE(fd_->Open(temp_file_.path().c_str(), O_RDONLY, 0600));
  }
  void TearDown() override { fd_->Close(); }

  void ReadExtents(vector<Extent> extents, brillo::Blob* blob) {
    blob->clear();
    for (const auto& extent : extents) {
      blob->insert(
          blob->end(),
          &sample_[extent.start_block() * kBlockSize],
          &sample_[(extent.start_block() + extent.num_blocks()) * kBlockSize]);
    }
  }

  FileDescriptorPtr fd_;
  test_utils::ScopedTempFile temp_file_{"ExtentReaderTest-file.XXXXXX"};
  brillo::Blob sample_;
};

TEST_F(ExtentReaderTest, SimpleTest) {
  vector<Extent> extents = {ExtentForRange(1, 1)};
  DirectExtentReader reader;
  EXPECT_TRUE(reader.Init(fd_, {extents.begin(), extents.end()}, kBlockSize));
  EXPECT_TRUE(reader.Seek(0));
  brillo::Blob blob1(utils::BlocksInExtents(extents) * kBlockSize);
  EXPECT_TRUE(reader.Read(blob1.data(), blob1.size()));
  brillo::Blob blob2;
  ReadExtents(extents, &blob2);
  ExpectVectorsEq(blob1, blob2);
}

TEST_F(ExtentReaderTest, ZeroExtentLengthTest) {
  vector<Extent> extents = {ExtentForRange(1, 0)};
  DirectExtentReader reader;
  EXPECT_TRUE(reader.Init(fd_, {extents.begin(), extents.end()}, kBlockSize));
  EXPECT_TRUE(reader.Seek(0));
  brillo::Blob blob(1);
  EXPECT_TRUE(reader.Read(blob.data(), 0));
  EXPECT_FALSE(reader.Read(blob.data(), 1));
}

TEST_F(ExtentReaderTest, NoExtentTest) {
  DirectExtentReader reader;
  EXPECT_TRUE(reader.Init(fd_, {}, kBlockSize));
  EXPECT_TRUE(reader.Seek(0));
  brillo::Blob blob(1);
  EXPECT_TRUE(reader.Read(blob.data(), 0));
  EXPECT_FALSE(reader.Read(blob.data(), 1));
}

TEST_F(ExtentReaderTest, OverflowExtentTest) {
  vector<Extent> extents = {ExtentForRange(1, 1)};
  DirectExtentReader reader;
  EXPECT_TRUE(reader.Init(fd_, {extents.begin(), extents.end()}, kBlockSize));
  EXPECT_TRUE(reader.Seek(0));
  brillo::Blob blob(utils::BlocksInExtents(extents) * kBlockSize + 1);
  EXPECT_FALSE(reader.Read(blob.data(), blob.size()));
}

TEST_F(ExtentReaderTest, SeekOverflow1Test) {
  vector<Extent> extents = {ExtentForRange(1, 0)};
  DirectExtentReader reader;
  EXPECT_TRUE(reader.Init(fd_, {extents.begin(), extents.end()}, kBlockSize));
  EXPECT_TRUE(reader.Seek(0));
  EXPECT_FALSE(reader.Seek(1));
}

TEST_F(ExtentReaderTest, SeekOverflow2Test) {
  DirectExtentReader reader;
  reader.Init(fd_, {}, kBlockSize);
  EXPECT_TRUE(reader.Seek(0));
  EXPECT_FALSE(reader.Seek(1));
}

TEST_F(ExtentReaderTest, SeekOverflow3Test) {
  vector<Extent> extents = {ExtentForRange(1, 1)};
  DirectExtentReader reader;
  EXPECT_TRUE(reader.Init(fd_, {extents.begin(), extents.end()}, kBlockSize));
  // Seek to the end of the extents should be fine as long as nothing is read.
  EXPECT_TRUE(reader.Seek(kBlockSize));
  EXPECT_FALSE(reader.Seek(kBlockSize + 1));
}

TEST_F(ExtentReaderTest, RandomReadTest) {
  vector<Extent> extents = {ExtentForRange(0, 0),
                            ExtentForRange(1, 1),
                            ExtentForRange(3, 0),
                            ExtentForRange(4, 2),
                            ExtentForRange(7, 1)};
  DirectExtentReader reader;
  EXPECT_TRUE(reader.Init(fd_, {extents.begin(), extents.end()}, kBlockSize));

  brillo::Blob result;
  ReadExtents(extents, &result);

  brillo::Blob blob(utils::BlocksInExtents(extents) * kBlockSize);
  srand(time(nullptr));
  uint32_t rand_seed;
  for (size_t idx = 0; idx < kRandomIterations; idx++) {
    // zero to full size available.
    size_t start = rand_r(&rand_seed) % blob.size();
    size_t size = rand_r(&rand_seed) % (blob.size() - start);
    EXPECT_TRUE(reader.Seek(start));
    EXPECT_TRUE(reader.Read(blob.data(), size));
    for (size_t i = 0; i < size; i++) {
      ASSERT_EQ(blob[i], result[start + i]);
    }
  }
}

}  // namespace chromeos_update_engine
