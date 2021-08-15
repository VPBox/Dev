/*
 * Copyright (C) 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/scopeguard.h>
#include <gtest/gtest.h>
#include <libavb/libavb.h>
#include <ziparchive/zip_archive.h>

#include "apex_file.h"
#include "apex_key.h"

static std::string testDataDir = android::base::GetExecutableDirectory() + "/";

namespace android {
namespace apex {
namespace {

TEST(ApexFileTest, GetOffsetOfSimplePackage) {
  const std::string filePath = testDataDir + "apex.apexd_test.apex";
  StatusOr<ApexFile> apexFile = ApexFile::Open(filePath);
  ASSERT_TRUE(apexFile.Ok());

  int32_t zip_image_offset;
  size_t zip_image_size;
  {
    ZipArchiveHandle handle;
    int32_t rc = OpenArchive(filePath.c_str(), &handle);
    ASSERT_EQ(0, rc);
    auto close_guard =
        android::base::make_scope_guard([&handle]() { CloseArchive(handle); });

    ZipEntry entry;
    rc = FindEntry(handle, ZipString("apex_payload.img"), &entry);
    ASSERT_EQ(0, rc);

    zip_image_offset = entry.offset;
    EXPECT_EQ(zip_image_offset % 4096, 0);
    zip_image_size = entry.uncompressed_length;
    EXPECT_EQ(zip_image_size, entry.compressed_length);
  }

  EXPECT_EQ(zip_image_offset, apexFile->GetImageOffset());
  EXPECT_EQ(zip_image_size, apexFile->GetImageSize());
}

TEST(ApexFileTest, GetOffsetMissingFile) {
  const std::string filePath = testDataDir + "missing.apex";
  StatusOr<ApexFile> apexFile = ApexFile::Open(filePath);
  ASSERT_FALSE(apexFile.Ok());
  EXPECT_NE(std::string::npos,
            apexFile.ErrorMessage().find("Failed to open package"))
      << apexFile.ErrorMessage();
}

TEST(ApexFileTest, GetApexManifest) {
  const std::string filePath = testDataDir + "apex.apexd_test.apex";
  StatusOr<ApexFile> apexFile = ApexFile::Open(filePath);
  ASSERT_TRUE(apexFile.Ok());
  EXPECT_EQ("com.android.apex.test_package", apexFile->GetManifest().name());
  EXPECT_EQ(1u, apexFile->GetManifest().version());
}

TEST(ApexFileTest, VerifyApexVerity) {
  const std::string filePath = testDataDir + "apex.apexd_test.apex";
  StatusOr<ApexFile> apexFile = ApexFile::Open(filePath);
  ASSERT_TRUE(apexFile.Ok()) << apexFile.ErrorMessage();

  auto verity_or = apexFile->VerifyApexVerity();
  ASSERT_TRUE(verity_or.Ok()) << verity_or.ErrorMessage();

  const ApexVerityData& data = *verity_or;
  EXPECT_NE(nullptr, data.desc.get());
  EXPECT_EQ(std::string("1772301d454698dd155205b7851959c625d8a3e6"
                        "d39360122693bad804b70007"),
            data.salt);
  EXPECT_EQ(std::string("f6139829a01059be55b13e09c4fddbb5565a8626"),
            data.root_digest);
}

// TODO: May consider packaging a debug key in debug builds (again).
#if 0
TEST(ApexFileTest, VerifyApexVerityNoKeyDir) {
  const std::string filePath = testDataDir + "apex.apexd_test.apex";
  StatusOr<ApexFile> apexFile = ApexFile::Open(filePath);
  ASSERT_TRUE(apexFile.Ok()) << apexFile.ErrorMessage();

  auto verity_or = apexFile->VerifyApexVerity({"/tmp/"});
  ASSERT_FALSE(verity_or.Ok());
}
#endif

// TODO(jiyong): re-enable this test. This test is disabled because the build
// system now always bundles the public key that was used to sign the APEX.
// In debuggable build, the bundled public key is used as the last fallback.
// As a result, the verification is always successful (and thus test fails).
// In order to re-enable this test, we have to manually create an APEX
// where public key is not bundled.
#if 0
TEST(ApexFileTest, VerifyApexVerityNoKeyInst) {
  const std::string filePath = testDataDir + "apex.apexd_test_no_inst_key.apex";
  StatusOr<ApexFile> apexFile = ApexFile::Open(filePath);
  ASSERT_TRUE(apexFile.Ok()) << apexFile.ErrorMessage();

  auto verity_or = apexFile->VerifyApexVerity();
  ASSERT_FALSE(verity_or.Ok());
}
#endif

TEST(ApexFileTest, GetBundledPublicKey) {
  const std::string filePath = testDataDir + "apex.apexd_test.apex";
  StatusOr<ApexFile> apexFile = ApexFile::Open(filePath);
  ASSERT_TRUE(apexFile.Ok());

  const std::string keyPath =
      testDataDir + "apexd_testdata/com.android.apex.test_package.avbpubkey";
  std::string keyContent;
  ASSERT_TRUE(android::base::ReadFileToString(keyPath, &keyContent))
      << "Failed to read " << keyPath;

  EXPECT_EQ(keyContent, apexFile->GetBundledPublicKey());
}

}  // namespace
}  // namespace apex
}  // namespace android

int main(int argc, char** argv) {
  android::base::InitLogging(argv, &android::base::StderrLogger);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
