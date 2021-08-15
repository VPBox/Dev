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

#include "update_engine/payload_consumer/file_writer.h"

#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <string>

#include <brillo/secure_blob.h>
#include <gtest/gtest.h>

#include "update_engine/common/test_utils.h"
#include "update_engine/common/utils.h"

using std::string;

namespace chromeos_update_engine {

class FileWriterTest : public ::testing::Test {};

TEST(FileWriterTest, SimpleTest) {
  // Create a uniquely named file for testing.
  test_utils::ScopedTempFile file("FileWriterTest-XXXXXX");
  DirectFileWriter file_writer;
  EXPECT_EQ(0,
            file_writer.Open(file.path().c_str(),
                             O_CREAT | O_LARGEFILE | O_TRUNC | O_WRONLY,
                             0644));
  EXPECT_TRUE(file_writer.Write("test", 4));
  brillo::Blob actual_data;
  EXPECT_TRUE(utils::ReadFile(file.path(), &actual_data));

  EXPECT_EQ("test", string(actual_data.begin(), actual_data.end()));
  EXPECT_EQ(0, file_writer.Close());
}

TEST(FileWriterTest, ErrorTest) {
  DirectFileWriter file_writer;
  const string path("/tmp/ENOENT/FileWriterTest");
  EXPECT_EQ(
      -ENOENT,
      file_writer.Open(path.c_str(), O_CREAT | O_LARGEFILE | O_TRUNC, 0644));
}

TEST(FileWriterTest, WriteErrorTest) {
  // Create a uniquely named file for testing.
  test_utils::ScopedTempFile file("FileWriterTest-XXXXXX");
  DirectFileWriter file_writer;
  EXPECT_EQ(0,
            file_writer.Open(file.path().c_str(),
                             O_CREAT | O_LARGEFILE | O_TRUNC | O_RDONLY,
                             0644));
  EXPECT_FALSE(file_writer.Write("x", 1));
  EXPECT_EQ(0, file_writer.Close());
}

}  // namespace chromeos_update_engine
