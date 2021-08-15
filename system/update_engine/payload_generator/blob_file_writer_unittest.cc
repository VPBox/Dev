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

#include "update_engine/payload_generator/blob_file_writer.h"

#include <string>

#include <gtest/gtest.h>

#include "update_engine/common/test_utils.h"
#include "update_engine/common/utils.h"

using chromeos_update_engine::test_utils::FillWithData;
using std::string;

namespace chromeos_update_engine {

class BlobFileWriterTest : public ::testing::Test {};

TEST(BlobFileWriterTest, SimpleTest) {
  string blob_path;
  int blob_fd;
  EXPECT_TRUE(
      utils::MakeTempFile("BlobFileWriterTest.XXXXXX", &blob_path, &blob_fd));
  off_t blob_file_size = 0;
  BlobFileWriter blob_file(blob_fd, &blob_file_size);

  off_t blob_size = 1024;
  brillo::Blob blob(blob_size);
  FillWithData(&blob);
  EXPECT_EQ(0, blob_file.StoreBlob(blob));
  EXPECT_EQ(blob_size, blob_file.StoreBlob(blob));

  brillo::Blob stored_blob(blob_size);
  ssize_t bytes_read;
  ASSERT_TRUE(
      utils::PReadAll(blob_fd, stored_blob.data(), blob_size, 0, &bytes_read));
  EXPECT_EQ(bytes_read, blob_size);
  EXPECT_EQ(blob, stored_blob);
}

}  // namespace chromeos_update_engine
