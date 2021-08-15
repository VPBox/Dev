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

#include "update_engine/payload_consumer/cached_file_descriptor.h"

#include <fcntl.h>

#include <algorithm>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "update_engine/common/test_utils.h"
#include "update_engine/common/utils.h"

using chromeos_update_engine::test_utils::ExpectVectorsEq;
using std::min;
using std::string;
using std::vector;

namespace chromeos_update_engine {

namespace {
const size_t kCacheSize = 100;
const size_t kFileSize = 1024;
const size_t kRandomIterations = 1000;
}  // namespace

class CachedFileDescriptorTest : public ::testing::Test {
 public:
  void Open() {
    cfd_.reset(new CachedFileDescriptor(fd_, kCacheSize));
    EXPECT_TRUE(cfd_->Open(temp_file_.path().c_str(), O_RDWR, 0600));
  }

  void Write(uint8_t* buffer, size_t count) {
    size_t total_bytes_wrote = 0;
    while (total_bytes_wrote < count) {
      auto bytes_wrote =
          cfd_->Write(buffer + total_bytes_wrote, count - total_bytes_wrote);
      ASSERT_NE(bytes_wrote, -1);
      total_bytes_wrote += bytes_wrote;
    }
  }

  void Close() { EXPECT_TRUE(cfd_->Close()); }

  void SetUp() override {
    brillo::Blob zero_blob(kFileSize, 0);
    EXPECT_TRUE(utils::WriteFile(
        temp_file_.path().c_str(), zero_blob.data(), zero_blob.size()));
    Open();
  }

  void TearDown() override {
    Close();
    EXPECT_FALSE(cfd_->IsOpen());
  }

 protected:
  FileDescriptorPtr fd_{new EintrSafeFileDescriptor};
  test_utils::ScopedTempFile temp_file_{"CachedFileDescriptor-file.XXXXXX"};
  int value_{1};
  FileDescriptorPtr cfd_;
};

TEST_F(CachedFileDescriptorTest, IsOpenTest) {
  EXPECT_TRUE(cfd_->IsOpen());
}

TEST_F(CachedFileDescriptorTest, SimpleWriteTest) {
  EXPECT_EQ(cfd_->Seek(0, SEEK_SET), 0);
  brillo::Blob blob_in(kFileSize, value_);
  Write(blob_in.data(), blob_in.size());
  EXPECT_TRUE(cfd_->Flush());

  brillo::Blob blob_out;
  EXPECT_TRUE(utils::ReadFile(temp_file_.path(), &blob_out));
  EXPECT_EQ(blob_in, blob_out);
}

TEST_F(CachedFileDescriptorTest, OneBytePerWriteTest) {
  EXPECT_EQ(cfd_->Seek(0, SEEK_SET), 0);
  brillo::Blob blob_in(kFileSize, value_);
  for (size_t idx = 0; idx < blob_in.size(); idx++) {
    Write(&blob_in[idx], 1);
  }
  EXPECT_TRUE(cfd_->Flush());

  brillo::Blob blob_out;
  EXPECT_TRUE(utils::ReadFile(temp_file_.path(), &blob_out));
  EXPECT_EQ(blob_in, blob_out);
}

TEST_F(CachedFileDescriptorTest, RandomWriteTest) {
  EXPECT_EQ(cfd_->Seek(0, SEEK_SET), 0);

  brillo::Blob blob_in(kFileSize, 0);
  srand(time(nullptr));
  uint32_t rand_seed;
  for (size_t idx = 0; idx < kRandomIterations; idx++) {
    // zero to full size available.
    size_t start = rand_r(&rand_seed) % blob_in.size();
    size_t size = rand_r(&rand_seed) % (blob_in.size() - start);
    std::fill_n(&blob_in[start], size, idx % 256);
    EXPECT_EQ(cfd_->Seek(start, SEEK_SET), static_cast<off64_t>(start));
    Write(&blob_in[start], size);
  }
  EXPECT_TRUE(cfd_->Flush());

  brillo::Blob blob_out;
  EXPECT_TRUE(utils::ReadFile(temp_file_.path(), &blob_out));
  EXPECT_EQ(blob_in, blob_out);
}

TEST_F(CachedFileDescriptorTest, SeekTest) {
  EXPECT_EQ(cfd_->Seek(0, SEEK_SET), 0);
  EXPECT_EQ(cfd_->Seek(1, SEEK_SET), 1);
  EXPECT_EQ(cfd_->Seek(kFileSize - 1, SEEK_SET),
            static_cast<off64_t>(kFileSize - 1));
  EXPECT_EQ(cfd_->Seek(kFileSize, SEEK_SET), static_cast<off64_t>(kFileSize));
  EXPECT_EQ(cfd_->Seek(kFileSize + 1, SEEK_SET),
            static_cast<off64_t>(kFileSize + 1));

  EXPECT_EQ(cfd_->Seek(0, SEEK_SET), 0);
  EXPECT_EQ(cfd_->Seek(1, SEEK_CUR), 1);
  EXPECT_EQ(cfd_->Seek(1, SEEK_CUR), 2);
  EXPECT_EQ(cfd_->Seek(kFileSize - 1, SEEK_SET),
            static_cast<off64_t>(kFileSize - 1));
  EXPECT_EQ(cfd_->Seek(1, SEEK_CUR), static_cast<off64_t>(kFileSize));
  EXPECT_EQ(cfd_->Seek(1, SEEK_CUR), static_cast<off64_t>(kFileSize + 1));
}

TEST_F(CachedFileDescriptorTest, NoFlushTest) {
  EXPECT_EQ(cfd_->Seek(0, SEEK_SET), 0);
  brillo::Blob blob_in(kFileSize, value_);
  Write(blob_in.data(), blob_in.size());

  brillo::Blob blob_out;
  EXPECT_TRUE(utils::ReadFile(temp_file_.path(), &blob_out));
  EXPECT_NE(blob_in, blob_out);
}

TEST_F(CachedFileDescriptorTest, CacheSizeWriteTest) {
  off64_t seek = 10;
  brillo::Blob blob_in(kFileSize, 0);
  std::fill_n(&blob_in[seek], kCacheSize, value_);
  // We are writing exactly one cache size; Then it should be committed.
  EXPECT_EQ(cfd_->Seek(seek, SEEK_SET), seek);
  Write(&blob_in[seek], kCacheSize);

  brillo::Blob blob_out;
  EXPECT_TRUE(utils::ReadFile(temp_file_.path(), &blob_out));
  EXPECT_EQ(blob_in, blob_out);
}

TEST_F(CachedFileDescriptorTest, UnderCacheSizeWriteTest) {
  off64_t seek = 100;
  size_t less_than_cache_size = kCacheSize - 1;
  EXPECT_EQ(cfd_->Seek(seek, SEEK_SET), seek);
  brillo::Blob blob_in(kFileSize, 0);
  std::fill_n(&blob_in[seek], less_than_cache_size, value_);
  // We are writing less than one cache size; then it should not be committed.
  Write(&blob_in[seek], less_than_cache_size);

  // Revert the changes in |blob_in|.
  std::fill_n(&blob_in[seek], less_than_cache_size, 0);
  brillo::Blob blob_out;
  EXPECT_TRUE(utils::ReadFile(temp_file_.path(), &blob_out));
  EXPECT_EQ(blob_in, blob_out);
}

TEST_F(CachedFileDescriptorTest, SeekAfterWriteTest) {
  off64_t seek = 100;
  size_t less_than_cache_size = kCacheSize - 3;
  EXPECT_EQ(cfd_->Seek(seek, SEEK_SET), seek);
  brillo::Blob blob_in(kFileSize, 0);
  std::fill_n(&blob_in[seek], less_than_cache_size, value_);
  // We are writing less than  one cache size; then it should not be committed.
  Write(&blob_in[seek], less_than_cache_size);

  // Then we seek, it should've written the cache after seek.
  EXPECT_EQ(cfd_->Seek(200, SEEK_SET), 200);

  brillo::Blob blob_out;
  EXPECT_TRUE(utils::ReadFile(temp_file_.path(), &blob_out));
  EXPECT_EQ(blob_in, blob_out);
}

}  // namespace chromeos_update_engine
