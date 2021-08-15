//
// Copyright (C) 2018 The Android Open Source Project
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

#include "update_engine/payload_consumer/verity_writer_android.h"

#include <brillo/secure_blob.h>
#include <gtest/gtest.h>

#include "update_engine/common/test_utils.h"
#include "update_engine/common/utils.h"

namespace chromeos_update_engine {

class VerityWriterAndroidTest : public ::testing::Test {
 protected:
  void SetUp() override {
    partition_.target_path = temp_file_.path();
    partition_.block_size = 4096;
    partition_.hash_tree_data_offset = 0;
    partition_.hash_tree_data_size = 4096;
    partition_.hash_tree_offset = 4096;
    partition_.hash_tree_size = 4096;
    partition_.hash_tree_algorithm = "sha1";
    partition_.fec_roots = 2;
  }

  VerityWriterAndroid verity_writer_;
  InstallPlan::Partition partition_;
  test_utils::ScopedTempFile temp_file_;
};

TEST_F(VerityWriterAndroidTest, SimpleTest) {
  brillo::Blob part_data(8192);
  test_utils::WriteFileVector(partition_.target_path, part_data);
  ASSERT_TRUE(verity_writer_.Init(partition_));
  EXPECT_TRUE(verity_writer_.Update(0, part_data.data(), 4096));
  EXPECT_TRUE(verity_writer_.Update(4096, part_data.data() + 4096, 4096));
  brillo::Blob actual_part;
  utils::ReadFile(partition_.target_path, &actual_part);
  // dd if=/dev/zero bs=4096 count=1 2>/dev/null | sha1sum | xxd -r -p |
  //     hexdump -v -e '/1 "0x%02x, "'
  brillo::Blob hash = {0x1c, 0xea, 0xf7, 0x3d, 0xf4, 0x0e, 0x53,
                       0x1d, 0xf3, 0xbf, 0xb2, 0x6b, 0x4f, 0xb7,
                       0xcd, 0x95, 0xfb, 0x7b, 0xff, 0x1d};
  memcpy(part_data.data() + 4096, hash.data(), hash.size());
  EXPECT_EQ(part_data, actual_part);
}

TEST_F(VerityWriterAndroidTest, NoOpTest) {
  partition_.hash_tree_data_size = 0;
  partition_.hash_tree_size = 0;
  brillo::Blob part_data(4096);
  ASSERT_TRUE(verity_writer_.Init(partition_));
  EXPECT_TRUE(verity_writer_.Update(0, part_data.data(), part_data.size()));
  EXPECT_TRUE(verity_writer_.Update(4096, part_data.data(), part_data.size()));
  EXPECT_TRUE(verity_writer_.Update(8192, part_data.data(), part_data.size()));
}

TEST_F(VerityWriterAndroidTest, InvalidHashAlgorithmTest) {
  partition_.hash_tree_algorithm = "sha123";
  EXPECT_FALSE(verity_writer_.Init(partition_));
}

TEST_F(VerityWriterAndroidTest, WrongHashTreeSizeTest) {
  partition_.hash_tree_size = 8192;
  EXPECT_FALSE(verity_writer_.Init(partition_));
}

TEST_F(VerityWriterAndroidTest, SHA256Test) {
  partition_.hash_tree_algorithm = "sha256";
  brillo::Blob part_data(8192);
  test_utils::WriteFileVector(partition_.target_path, part_data);
  ASSERT_TRUE(verity_writer_.Init(partition_));
  EXPECT_TRUE(verity_writer_.Update(0, part_data.data(), 4096));
  EXPECT_TRUE(verity_writer_.Update(4096, part_data.data() + 4096, 4096));
  brillo::Blob actual_part;
  utils::ReadFile(partition_.target_path, &actual_part);
  // dd if=/dev/zero bs=4096 count=1 2>/dev/null | sha256sum | xxd -r -p |
  //     hexdump -v -e '/1 "0x%02x, "'
  brillo::Blob hash = {0xad, 0x7f, 0xac, 0xb2, 0x58, 0x6f, 0xc6, 0xe9,
                       0x66, 0xc0, 0x04, 0xd7, 0xd1, 0xd1, 0x6b, 0x02,
                       0x4f, 0x58, 0x05, 0xff, 0x7c, 0xb4, 0x7c, 0x7a,
                       0x85, 0xda, 0xbd, 0x8b, 0x48, 0x89, 0x2c, 0xa7};
  memcpy(part_data.data() + 4096, hash.data(), hash.size());
  EXPECT_EQ(part_data, actual_part);
}

TEST_F(VerityWriterAndroidTest, FECTest) {
  partition_.fec_data_offset = 0;
  partition_.fec_data_size = 4096;
  partition_.fec_offset = 4096;
  partition_.fec_size = 2 * 4096;
  brillo::Blob part_data(3 * 4096, 0x1);
  test_utils::WriteFileVector(partition_.target_path, part_data);
  ASSERT_TRUE(verity_writer_.Init(partition_));
  EXPECT_TRUE(verity_writer_.Update(0, part_data.data(), part_data.size()));
  brillo::Blob actual_part;
  utils::ReadFile(partition_.target_path, &actual_part);
  // Write FEC data.
  for (size_t i = 4096; i < part_data.size(); i += 2) {
    part_data[i] = 0x8e;
    part_data[i + 1] = 0x8f;
  }
  EXPECT_EQ(part_data, actual_part);
}

}  // namespace chromeos_update_engine
