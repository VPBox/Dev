//
// Copyright (C) 2011 The Android Open Source Project
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

#include <string.h>
#include <unistd.h>

#include <memory>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "update_engine/common/test_utils.h"
#include "update_engine/payload_consumer/bzip_extent_writer.h"
#include "update_engine/payload_consumer/extent_writer.h"
#include "update_engine/payload_consumer/xz_extent_writer.h"
#include "update_engine/payload_generator/bzip.h"
#include "update_engine/payload_generator/xz.h"

using chromeos_update_engine::test_utils::kRandomString;
using google::protobuf::RepeatedPtrField;
using std::string;
using std::vector;

namespace chromeos_update_engine {

namespace {

// ExtentWriter class that writes to memory, used to test the decompression
// step with the corresponding extent writer.
class MemoryExtentWriter : public ExtentWriter {
 public:
  // Creates the ExtentWriter that will write all the bytes to the passed |data|
  // blob.
  explicit MemoryExtentWriter(brillo::Blob* data) : data_(data) {
    data_->clear();
  }
  ~MemoryExtentWriter() override = default;

  bool Init(FileDescriptorPtr fd,
            const RepeatedPtrField<Extent>& extents,
            uint32_t block_size) override {
    return true;
  }
  bool Write(const void* bytes, size_t count) override {
    data_->reserve(data_->size() + count);
    data_->insert(data_->end(),
                  static_cast<const uint8_t*>(bytes),
                  static_cast<const uint8_t*>(bytes) + count);
    return true;
  }

 private:
  brillo::Blob* data_;
};

template <typename W>
bool DecompressWithWriter(const brillo::Blob& in, brillo::Blob* out) {
  std::unique_ptr<ExtentWriter> writer(
      new W(std::make_unique<MemoryExtentWriter>(out)));
  // Init() parameters are ignored by the testing MemoryExtentWriter.
  bool ok = writer->Init(nullptr, {}, 1);
  ok = writer->Write(in.data(), in.size()) && ok;
  return ok;
}

}  // namespace

template <typename T>
class ZipTest : public ::testing::Test {
 public:
  bool ZipCompress(const brillo::Blob& in, brillo::Blob* out) const = 0;
  bool ZipDecompress(const brillo::Blob& in, brillo::Blob* out) const = 0;
};

class BzipTest {};

template <>
class ZipTest<BzipTest> : public ::testing::Test {
 public:
  bool ZipCompress(const brillo::Blob& in, brillo::Blob* out) const {
    return BzipCompress(in, out);
  }
  bool ZipDecompress(const brillo::Blob& in, brillo::Blob* out) const {
    return DecompressWithWriter<BzipExtentWriter>(in, out);
  }
};

class XzTest {};

template <>
class ZipTest<XzTest> : public ::testing::Test {
 public:
  bool ZipCompress(const brillo::Blob& in, brillo::Blob* out) const {
    return XzCompress(in, out);
  }
  bool ZipDecompress(const brillo::Blob& in, brillo::Blob* out) const {
    return DecompressWithWriter<XzExtentWriter>(in, out);
  }
};

typedef ::testing::Types<BzipTest, XzTest> ZipTestTypes;

TYPED_TEST_CASE(ZipTest, ZipTestTypes);

TYPED_TEST(ZipTest, SimpleTest) {
  string in_str(
      "this should compress well xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
      "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
      "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
      "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
      "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
      "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
  brillo::Blob in(in_str.begin(), in_str.end());
  brillo::Blob out;
  EXPECT_TRUE(this->ZipCompress(in, &out));
  EXPECT_LT(out.size(), in.size());
  EXPECT_GT(out.size(), 0U);
  brillo::Blob decompressed;
  EXPECT_TRUE(this->ZipDecompress(out, &decompressed));
  EXPECT_EQ(in.size(), decompressed.size());
  EXPECT_EQ(0, memcmp(in.data(), decompressed.data(), in.size()));
}

TYPED_TEST(ZipTest, PoorCompressionTest) {
  brillo::Blob in(std::begin(kRandomString), std::end(kRandomString));
  brillo::Blob out;
  EXPECT_TRUE(this->ZipCompress(in, &out));
  EXPECT_GT(out.size(), in.size());
  brillo::Blob decompressed;
  EXPECT_TRUE(this->ZipDecompress(out, &decompressed));
  EXPECT_EQ(in.size(), decompressed.size());
  EXPECT_EQ(in, decompressed);
}

TYPED_TEST(ZipTest, MalformedZipTest) {
  brillo::Blob in(std::begin(kRandomString), std::end(kRandomString));
  brillo::Blob out;
  EXPECT_FALSE(this->ZipDecompress(in, &out));
}

TYPED_TEST(ZipTest, EmptyInputsTest) {
  brillo::Blob in;
  brillo::Blob out;
  EXPECT_TRUE(this->ZipDecompress(in, &out));
  EXPECT_EQ(0U, out.size());

  EXPECT_TRUE(this->ZipCompress(in, &out));
  EXPECT_EQ(0U, out.size());
}

TYPED_TEST(ZipTest, CompressELFTest) {
  string path = test_utils::GetBuildArtifactsPath("delta_generator");
  brillo::Blob in;
  utils::ReadFile(path, &in);
  brillo::Blob out;
  EXPECT_TRUE(this->ZipCompress(in, &out));
  EXPECT_LT(out.size(), in.size());
  EXPECT_GT(out.size(), 0U);
  brillo::Blob decompressed;
  EXPECT_TRUE(this->ZipDecompress(out, &decompressed));
  EXPECT_EQ(in.size(), decompressed.size());
  EXPECT_EQ(0, memcmp(in.data(), decompressed.data(), in.size()));
}

}  // namespace chromeos_update_engine
