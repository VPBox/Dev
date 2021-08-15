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

#include <fcntl.h>

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include <brillo/secure_blob.h>
#include <gtest/gtest.h>

#include "update_engine/common/test_utils.h"
#include "update_engine/common/utils.h"
#include "update_engine/payload_consumer/payload_constants.h"
#include "update_engine/payload_generator/extent_ranges.h"

using chromeos_update_engine::test_utils::ExpectVectorsEq;
using std::min;
using std::string;
using std::vector;

namespace chromeos_update_engine {

static_assert(sizeof(off_t) == 8, "off_t not 64 bit");

namespace {
const size_t kBlockSize = 4096;
}

class ExtentWriterTest : public ::testing::Test {
 protected:
  void SetUp() override {
    fd_.reset(new EintrSafeFileDescriptor);
    ASSERT_TRUE(fd_->Open(temp_file_.path().c_str(), O_RDWR, 0600));
  }
  void TearDown() override { fd_->Close(); }

  // Writes data to an extent writer in 'chunk_size' chunks with
  // the first chunk of size first_chunk_size. It calculates what the
  // resultant file should look like and ensure that the extent writer
  // wrote the file correctly.
  void WriteAlignedExtents(size_t chunk_size, size_t first_chunk_size);

  FileDescriptorPtr fd_;
  test_utils::ScopedTempFile temp_file_{"ExtentWriterTest-file.XXXXXX"};
};

TEST_F(ExtentWriterTest, SimpleTest) {
  vector<Extent> extents = {ExtentForRange(1, 1)};
  const string bytes = "1234";
  DirectExtentWriter direct_writer;
  EXPECT_TRUE(
      direct_writer.Init(fd_, {extents.begin(), extents.end()}, kBlockSize));
  EXPECT_TRUE(direct_writer.Write(bytes.data(), bytes.size()));

  EXPECT_EQ(static_cast<off_t>(kBlockSize + bytes.size()),
            utils::FileSize(temp_file_.path()));

  brillo::Blob result_file;
  EXPECT_TRUE(utils::ReadFile(temp_file_.path(), &result_file));

  brillo::Blob expected_file(kBlockSize);
  expected_file.insert(
      expected_file.end(), bytes.data(), bytes.data() + bytes.size());
  ExpectVectorsEq(expected_file, result_file);
}

TEST_F(ExtentWriterTest, ZeroLengthTest) {
  vector<Extent> extents = {ExtentForRange(1, 1)};
  DirectExtentWriter direct_writer;
  EXPECT_TRUE(
      direct_writer.Init(fd_, {extents.begin(), extents.end()}, kBlockSize));
  EXPECT_TRUE(direct_writer.Write(nullptr, 0));
}

TEST_F(ExtentWriterTest, OverflowExtentTest) {
  WriteAlignedExtents(kBlockSize * 3, kBlockSize * 3);
}

TEST_F(ExtentWriterTest, UnalignedWriteTest) {
  WriteAlignedExtents(7, 7);
}

TEST_F(ExtentWriterTest, LargeUnalignedWriteTest) {
  WriteAlignedExtents(kBlockSize * 2, kBlockSize / 2);
}

void ExtentWriterTest::WriteAlignedExtents(size_t chunk_size,
                                           size_t first_chunk_size) {
  vector<Extent> extents = {
      ExtentForRange(1, 1), ExtentForRange(0, 1), ExtentForRange(2, 1)};
  brillo::Blob data(kBlockSize * 3);
  test_utils::FillWithData(&data);

  DirectExtentWriter direct_writer;
  EXPECT_TRUE(
      direct_writer.Init(fd_, {extents.begin(), extents.end()}, kBlockSize));

  size_t bytes_written = 0;
  while (bytes_written < data.size()) {
    size_t bytes_to_write = min(data.size() - bytes_written, chunk_size);
    if (bytes_written == 0) {
      bytes_to_write = min(data.size() - bytes_written, first_chunk_size);
    }
    EXPECT_TRUE(direct_writer.Write(&data[bytes_written], bytes_to_write));
    bytes_written += bytes_to_write;
  }

  EXPECT_EQ(static_cast<off_t>(data.size()),
            utils::FileSize(temp_file_.path()));

  brillo::Blob result_file;
  EXPECT_TRUE(utils::ReadFile(temp_file_.path(), &result_file));

  brillo::Blob expected_file;
  expected_file.insert(expected_file.end(),
                       data.begin() + kBlockSize,
                       data.begin() + kBlockSize * 2);
  expected_file.insert(
      expected_file.end(), data.begin(), data.begin() + kBlockSize);
  expected_file.insert(
      expected_file.end(), data.begin() + kBlockSize * 2, data.end());
  ExpectVectorsEq(expected_file, result_file);
}

TEST_F(ExtentWriterTest, SparseFileTest) {
  vector<Extent> extents = {ExtentForRange(1, 1),
                            ExtentForRange(kSparseHole, 2),
                            ExtentForRange(0, 1)};
  const int block_count = 4;
  const int on_disk_count = 2;

  brillo::Blob data(17);
  test_utils::FillWithData(&data);

  DirectExtentWriter direct_writer;
  EXPECT_TRUE(
      direct_writer.Init(fd_, {extents.begin(), extents.end()}, kBlockSize));

  size_t bytes_written = 0;
  while (bytes_written < (block_count * kBlockSize)) {
    size_t bytes_to_write =
        min(block_count * kBlockSize - bytes_written, data.size());
    EXPECT_TRUE(direct_writer.Write(data.data(), bytes_to_write));
    bytes_written += bytes_to_write;
  }

  // check file size, then data inside
  ASSERT_EQ(static_cast<off_t>(2 * kBlockSize),
            utils::FileSize(temp_file_.path()));

  brillo::Blob resultant_data;
  EXPECT_TRUE(utils::ReadFile(temp_file_.path(), &resultant_data));

  // Create expected data
  brillo::Blob expected_data(on_disk_count * kBlockSize);
  brillo::Blob big(block_count * kBlockSize);
  for (brillo::Blob::size_type i = 0; i < big.size(); i++) {
    big[i] = data[i % data.size()];
  }
  memcpy(&expected_data[kBlockSize], &big[0], kBlockSize);
  memcpy(&expected_data[0], &big[3 * kBlockSize], kBlockSize);
  ExpectVectorsEq(expected_data, resultant_data);
}

}  // namespace chromeos_update_engine
