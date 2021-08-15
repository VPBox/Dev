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

#include "update_engine/payload_consumer/xz_extent_writer.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>

#include <base/memory/ptr_util.h>
#include <gtest/gtest.h>

#include "update_engine/common/test_utils.h"
#include "update_engine/common/utils.h"
#include "update_engine/payload_consumer/fake_extent_writer.h"

namespace chromeos_update_engine {

namespace {

const char kSampleData[] = "Redundaaaaaaaaaaaaaant\n";

// Compressed data with CRC-32 check, generated with:
// echo "Redundaaaaaaaaaaaaaant" | xz -9 --check=crc32 |
// hexdump -v -e '"    " 12/1 "0x%02x, " "\n"'
const uint8_t kCompressedDataCRC32[] = {
    0xfd, 0x37, 0x7a, 0x58, 0x5a, 0x00, 0x00, 0x01, 0x69, 0x22, 0xde, 0x36,
    0x02, 0x00, 0x21, 0x01, 0x1c, 0x00, 0x00, 0x00, 0x10, 0xcf, 0x58, 0xcc,
    0xe0, 0x00, 0x16, 0x00, 0x10, 0x5d, 0x00, 0x29, 0x19, 0x48, 0x87, 0x88,
    0xec, 0x49, 0x88, 0x73, 0x8b, 0x5d, 0xa6, 0x46, 0xb4, 0x00, 0x00, 0x00,
    0x68, 0xfc, 0x7b, 0x25, 0x00, 0x01, 0x28, 0x17, 0x46, 0x9e, 0x08, 0xfe,
    0x90, 0x42, 0x99, 0x0d, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x59, 0x5a,
};

// Compressed data without checksum, generated with:
// echo "Redundaaaaaaaaaaaaaant" | xz -9 --check=none |
// hexdump -v -e '"    " 12/1 "0x%02x, " "\n"'
const uint8_t kCompressedDataNoCheck[] = {
    0xfd, 0x37, 0x7a, 0x58, 0x5a, 0x00, 0x00, 0x00, 0xff, 0x12, 0xd9, 0x41,
    0x02, 0x00, 0x21, 0x01, 0x1c, 0x00, 0x00, 0x00, 0x10, 0xcf, 0x58, 0xcc,
    0xe0, 0x00, 0x16, 0x00, 0x10, 0x5d, 0x00, 0x29, 0x19, 0x48, 0x87, 0x88,
    0xec, 0x49, 0x88, 0x73, 0x8b, 0x5d, 0xa6, 0x46, 0xb4, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x24, 0x17, 0x4a, 0xd1, 0xbd, 0x52, 0x06, 0x72, 0x9e, 0x7a,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x59, 0x5a,
};

// Highly redundant data bigger than the internal buffer, generated with:
// dd if=/dev/zero bs=30K count=1 | tr '\0' 'a' | xz -9 --check=crc32 |
// hexdump -v -e '"    " 12/1 "0x%02x, " "\n"'
const uint8_t kCompressed30KiBofA[] = {
    0xfd, 0x37, 0x7a, 0x58, 0x5a, 0x00, 0x00, 0x01, 0x69, 0x22, 0xde, 0x36,
    0x02, 0x00, 0x21, 0x01, 0x1c, 0x00, 0x00, 0x00, 0x10, 0xcf, 0x58, 0xcc,
    0xe0, 0x77, 0xff, 0x00, 0x41, 0x5d, 0x00, 0x30, 0xef, 0xfb, 0xbf, 0xfe,
    0xa3, 0xb1, 0x5e, 0xe5, 0xf8, 0x3f, 0xb2, 0xaa, 0x26, 0x55, 0xf8, 0x68,
    0x70, 0x41, 0x70, 0x15, 0x0f, 0x8d, 0xfd, 0x1e, 0x4c, 0x1b, 0x8a, 0x42,
    0xb7, 0x19, 0xf4, 0x69, 0x18, 0x71, 0xae, 0x66, 0x23, 0x8a, 0x8a, 0x4d,
    0x2f, 0xa3, 0x0d, 0xd9, 0x7f, 0xa6, 0xe3, 0x8c, 0x23, 0x11, 0x53, 0xe0,
    0x59, 0x18, 0xc5, 0x75, 0x8a, 0xe2, 0x76, 0x4c, 0xee, 0x30, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xf9, 0x47, 0xb5, 0xee, 0x00, 0x01, 0x59, 0x80,
    0xf0, 0x01, 0x00, 0x00, 0xe0, 0x41, 0x96, 0xde, 0x3e, 0x30, 0x0d, 0x8b,
    0x02, 0x00, 0x00, 0x00, 0x00, 0x01, 0x59, 0x5a,
};

}  // namespace

class XzExtentWriterTest : public ::testing::Test {
 protected:
  void SetUp() override {
    fake_extent_writer_ = new FakeExtentWriter();
    xz_writer_.reset(new XzExtentWriter(base::WrapUnique(fake_extent_writer_)));
  }

  void WriteAll(const brillo::Blob& compressed) {
    EXPECT_TRUE(xz_writer_->Init(fd_, {}, 1024));
    EXPECT_TRUE(xz_writer_->Write(compressed.data(), compressed.size()));

    EXPECT_TRUE(fake_extent_writer_->InitCalled());
  }

  // Owned by |xz_writer_|. This object is invalidated after |xz_writer_| is
  // deleted.
  FakeExtentWriter* fake_extent_writer_{nullptr};
  std::unique_ptr<XzExtentWriter> xz_writer_;

  const brillo::Blob sample_data_{
      std::begin(kSampleData), std::begin(kSampleData) + strlen(kSampleData)};
  FileDescriptorPtr fd_;
};

TEST_F(XzExtentWriterTest, CreateAndDestroy) {
  // Test that no Init() or End() called doesn't crash the program.
  EXPECT_FALSE(fake_extent_writer_->InitCalled());
}

TEST_F(XzExtentWriterTest, CompressedSampleData) {
  WriteAll(brillo::Blob(std::begin(kCompressedDataNoCheck),
                        std::end(kCompressedDataNoCheck)));
  EXPECT_EQ(sample_data_, fake_extent_writer_->WrittenData());
}

TEST_F(XzExtentWriterTest, CompressedSampleDataWithCrc) {
  WriteAll(brillo::Blob(std::begin(kCompressedDataCRC32),
                        std::end(kCompressedDataCRC32)));
  EXPECT_EQ(sample_data_, fake_extent_writer_->WrittenData());
}

TEST_F(XzExtentWriterTest, CompressedDataBiggerThanTheBuffer) {
  // Test that even if the output data is bigger than the internal buffer, all
  // the data is written.
  WriteAll(brillo::Blob(std::begin(kCompressed30KiBofA),
                        std::end(kCompressed30KiBofA)));
  brillo::Blob expected_data(30 * 1024, 'a');
  EXPECT_EQ(expected_data, fake_extent_writer_->WrittenData());
}

TEST_F(XzExtentWriterTest, GarbageDataRejected) {
  EXPECT_TRUE(xz_writer_->Init(fd_, {}, 1024));
  // The sample_data_ is an uncompressed string.
  EXPECT_FALSE(xz_writer_->Write(sample_data_.data(), sample_data_.size()));
}

TEST_F(XzExtentWriterTest, PartialDataIsKept) {
  brillo::Blob compressed(std::begin(kCompressed30KiBofA),
                          std::end(kCompressed30KiBofA));
  EXPECT_TRUE(xz_writer_->Init(fd_, {}, 1024));
  for (uint8_t byte : compressed) {
    EXPECT_TRUE(xz_writer_->Write(&byte, 1));
  }

  // The sample_data_ is an uncompressed string.
  brillo::Blob expected_data(30 * 1024, 'a');
  EXPECT_EQ(expected_data, fake_extent_writer_->WrittenData());
}

}  // namespace chromeos_update_engine
