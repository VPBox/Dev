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

#include "update_engine/payload_consumer/bzip_extent_writer.h"

#include <fcntl.h>

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "update_engine/common/test_utils.h"
#include "update_engine/common/utils.h"
#include "update_engine/payload_generator/extent_ranges.h"

using google::protobuf::RepeatedPtrField;
using std::min;
using std::string;
using std::vector;

namespace chromeos_update_engine {

namespace {
const uint32_t kBlockSize = 4096;
}

class BzipExtentWriterTest : public ::testing::Test {
 protected:
  void SetUp() override {
    fd_.reset(new EintrSafeFileDescriptor);
    ASSERT_TRUE(fd_->Open(temp_file_.path().c_str(), O_RDWR, 0600));
  }
  void TearDown() override { fd_->Close(); }

  FileDescriptorPtr fd_;
  test_utils::ScopedTempFile temp_file_{"BzipExtentWriterTest-file.XXXXXX"};
};

TEST_F(BzipExtentWriterTest, SimpleTest) {
  vector<Extent> extents = {ExtentForRange(0, 1)};

  // 'echo test | bzip2 | hexdump' yields:
  static const char test_uncompressed[] = "test\n";
  static const uint8_t test[] = {
      0x42, 0x5a, 0x68, 0x39, 0x31, 0x41, 0x59, 0x26, 0x53, 0x59, 0xcc, 0xc3,
      0x71, 0xd4, 0x00, 0x00, 0x02, 0x41, 0x80, 0x00, 0x10, 0x02, 0x00, 0x0c,
      0x00, 0x20, 0x00, 0x21, 0x9a, 0x68, 0x33, 0x4d, 0x19, 0x97, 0x8b, 0xb9,
      0x22, 0x9c, 0x28, 0x48, 0x66, 0x61, 0xb8, 0xea, 0x00,
  };

  BzipExtentWriter bzip_writer(std::make_unique<DirectExtentWriter>());
  EXPECT_TRUE(
      bzip_writer.Init(fd_, {extents.begin(), extents.end()}, kBlockSize));
  EXPECT_TRUE(bzip_writer.Write(test, sizeof(test)));

  brillo::Blob buf;
  EXPECT_TRUE(utils::ReadFile(temp_file_.path(), &buf));
  EXPECT_EQ(strlen(test_uncompressed), buf.size());
  EXPECT_EQ(string(buf.begin(), buf.end()), string(test_uncompressed));
}

TEST_F(BzipExtentWriterTest, ChunkedTest) {
  // Generated with:
  //   yes "ABC" | head -c 819200 | bzip2 -9 |
  //     hexdump -v -e '"      " 11/1 "0x%02x, " "\n"'
  static const uint8_t kCompressedData[] = {
      0x42, 0x5a, 0x68, 0x39, 0x31, 0x41, 0x59, 0x26, 0x53, 0x59, 0xbe,
      0x1c, 0xda, 0xee, 0x03, 0x1f, 0xff, 0xc4, 0x00, 0x00, 0x10, 0x38,
      0x00, 0x20, 0x00, 0x50, 0x66, 0x9a, 0x05, 0x28, 0x38, 0x00, 0x11,
      0x60, 0x00, 0x22, 0xd0, 0x00, 0x45, 0xc0, 0x00, 0x8b, 0xc5, 0xdc,
      0x91, 0x4e, 0x14, 0x24, 0x2f, 0x87, 0x36, 0xbb, 0x80};
  brillo::Blob compressed_data(std::begin(kCompressedData),
                               std::end(kCompressedData));

  const brillo::Blob::size_type kDecompressedLength = 800 * 1024;  // 800 KiB
  const size_t kChunkSize = 3;

  brillo::Blob decompressed_data(kDecompressedLength);
  for (size_t i = 0; i < decompressed_data.size(); ++i)
    decompressed_data[i] = static_cast<uint8_t>("ABC\n"[i % 4]);

  vector<Extent> extents = {ExtentForBytes(kBlockSize, 0, kDecompressedLength)};

  BzipExtentWriter bzip_writer(std::make_unique<DirectExtentWriter>());
  EXPECT_TRUE(
      bzip_writer.Init(fd_, {extents.begin(), extents.end()}, kBlockSize));

  brillo::Blob original_compressed_data = compressed_data;
  for (brillo::Blob::size_type i = 0; i < compressed_data.size();
       i += kChunkSize) {
    size_t this_chunk_size = min(kChunkSize, compressed_data.size() - i);
    EXPECT_TRUE(bzip_writer.Write(&compressed_data[i], this_chunk_size));
  }

  // Check that the const input has not been clobbered.
  test_utils::ExpectVectorsEq(original_compressed_data, compressed_data);

  brillo::Blob output;
  EXPECT_TRUE(utils::ReadFile(temp_file_.path(), &output));
  EXPECT_EQ(kDecompressedLength, output.size());
  test_utils::ExpectVectorsEq(decompressed_data, output);
}

}  // namespace chromeos_update_engine
