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

#include "update_engine/payload_generator/boot_img_filesystem.h"

#include <vector>

#include <brillo/secure_blob.h>
#include <gtest/gtest.h>

#include "update_engine/common/test_utils.h"
#include "update_engine/common/utils.h"

namespace chromeos_update_engine {

using std::unique_ptr;
using std::vector;

class BootImgFilesystemTest : public ::testing::Test {
 protected:
  brillo::Blob GetBootImg(const brillo::Blob& kernel,
                          const brillo::Blob& ramdisk) {
    brillo::Blob boot_img(16 * 1024);
    BootImgFilesystem::boot_img_hdr hdr;
    memcpy(hdr.magic, BOOT_MAGIC, BOOT_MAGIC_SIZE);
    hdr.kernel_size = kernel.size();
    hdr.ramdisk_size = ramdisk.size();
    hdr.page_size = 4096;
    size_t offset = 0;
    memcpy(boot_img.data() + offset, &hdr, sizeof(hdr));
    offset += utils::RoundUp(sizeof(hdr), hdr.page_size);
    memcpy(boot_img.data() + offset, kernel.data(), kernel.size());
    offset += utils::RoundUp(kernel.size(), hdr.page_size);
    memcpy(boot_img.data() + offset, ramdisk.data(), ramdisk.size());
    return boot_img;
  }

  test_utils::ScopedTempFile boot_file_;
};

TEST_F(BootImgFilesystemTest, SimpleTest) {
  test_utils::WriteFileVector(
      boot_file_.path(),
      GetBootImg(brillo::Blob(1234, 'k'), brillo::Blob(5678, 'r')));
  unique_ptr<BootImgFilesystem> fs =
      BootImgFilesystem::CreateFromFile(boot_file_.path());
  EXPECT_NE(nullptr, fs);

  vector<FilesystemInterface::File> files;
  EXPECT_TRUE(fs->GetFiles(&files));
  ASSERT_EQ(2u, files.size());

  EXPECT_EQ("<kernel>", files[0].name);
  EXPECT_EQ(1u, files[0].extents.size());
  EXPECT_EQ(1u, files[0].extents[0].start_block());
  EXPECT_EQ(1u, files[0].extents[0].num_blocks());
  EXPECT_TRUE(files[0].deflates.empty());

  EXPECT_EQ("<ramdisk>", files[1].name);
  EXPECT_EQ(1u, files[1].extents.size());
  EXPECT_EQ(2u, files[1].extents[0].start_block());
  EXPECT_EQ(2u, files[1].extents[0].num_blocks());
  EXPECT_TRUE(files[1].deflates.empty());
}

TEST_F(BootImgFilesystemTest, BadImageTest) {
  brillo::Blob boot_img = GetBootImg({}, {});
  boot_img[7] = '?';
  test_utils::WriteFileVector(boot_file_.path(), boot_img);
  unique_ptr<BootImgFilesystem> fs =
      BootImgFilesystem::CreateFromFile(boot_file_.path());
  EXPECT_EQ(nullptr, fs);
}

TEST_F(BootImgFilesystemTest, GZipRamdiskTest) {
  // echo ramdisk | gzip | hexdump -v -e '/1 "0x%02x, "'
  const brillo::Blob ramdisk = {0x1f, 0x8b, 0x08, 0x00, 0x3a, 0x83, 0x35,
                                0x5b, 0x00, 0x03, 0x2b, 0x4a, 0xcc, 0x4d,
                                0xc9, 0x2c, 0xce, 0xe6, 0x02, 0x00, 0x2e,
                                0xf6, 0x0b, 0x08, 0x08, 0x00, 0x00, 0x00};
  test_utils::WriteFileVector(boot_file_.path(),
                              GetBootImg(brillo::Blob(5678, 'k'), ramdisk));
  unique_ptr<BootImgFilesystem> fs =
      BootImgFilesystem::CreateFromFile(boot_file_.path());
  EXPECT_NE(nullptr, fs);

  vector<FilesystemInterface::File> files;
  EXPECT_TRUE(fs->GetFiles(&files));
  ASSERT_EQ(2u, files.size());

  EXPECT_EQ("<kernel>", files[0].name);
  EXPECT_EQ(1u, files[0].extents.size());
  EXPECT_EQ(1u, files[0].extents[0].start_block());
  EXPECT_EQ(2u, files[0].extents[0].num_blocks());
  EXPECT_TRUE(files[0].deflates.empty());

  EXPECT_EQ("<ramdisk>", files[1].name);
  EXPECT_EQ(1u, files[1].extents.size());
  EXPECT_EQ(3u, files[1].extents[0].start_block());
  EXPECT_EQ(1u, files[1].extents[0].num_blocks());
  EXPECT_EQ(1u, files[1].deflates.size());
}

}  // namespace chromeos_update_engine
