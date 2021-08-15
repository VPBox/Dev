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
#include <android-base/logging.h>
#include <gtest/gtest.h>

#include "apex_manifest.h"

namespace android {
namespace apex {

TEST(ApexManifestTest, SimpleTest) {
  auto apex_manifest = ParseManifest(
      "{\"name\": \"com.android.example.apex\", \"version\": 1}\n");
  ASSERT_TRUE(apex_manifest.Ok());
  EXPECT_EQ("com.android.example.apex", std::string(apex_manifest->name()));
  EXPECT_EQ(1u, apex_manifest->version());
}

TEST(ApexManifestTest, NameMissing) {
  auto apex_manifest = ParseManifest("{\"version\": 1}\n");
  ASSERT_FALSE(apex_manifest.Ok());
  EXPECT_EQ(apex_manifest.ErrorMessage(),
            std::string("Missing required field \"name\" from APEX manifest."))
      << apex_manifest.ErrorMessage();
}

TEST(ApexManifestTest, VersionMissing) {
  auto apex_manifest =
      ParseManifest("{\"name\": \"com.android.example.apex\"}\n");
  ASSERT_FALSE(apex_manifest.Ok());
  EXPECT_EQ(
      apex_manifest.ErrorMessage(),
      std::string("Missing required field \"version\" from APEX manifest."))
      << apex_manifest.ErrorMessage();
}

TEST(ApexManifestTest, VersionNotNumber) {
  auto apex_manifest = ParseManifest(
      "{\"name\": \"com.android.example.apex\", \"version\": \"a\"}\n");

  ASSERT_FALSE(apex_manifest.Ok());
  EXPECT_EQ(apex_manifest.ErrorMessage(),
            std::string("Failed to parse APEX Manifest JSON config: "
                        "version: invalid value \"a\" for type TYPE_INT64"))
      << apex_manifest.ErrorMessage();
}

TEST(ApexManifestTest, NoPreInstallHook) {
  auto apex_manifest = ParseManifest(
      "{\"name\": \"com.android.example.apex\", \"version\": 1}\n");
  ASSERT_TRUE(apex_manifest.Ok());
  EXPECT_EQ("", std::string(apex_manifest->preinstallhook()));
}

TEST(ApexManifestTest, PreInstallHook) {
  auto apex_manifest = ParseManifest(
      "{\"name\": \"com.android.example.apex\", \"version\": 1, "
      "\"preInstallHook\": \"bin/preInstallHook\"}\n");
  ASSERT_TRUE(apex_manifest.Ok());
  EXPECT_EQ("bin/preInstallHook", std::string(apex_manifest->preinstallhook()));
}

TEST(ApexManifestTest, NoPostInstallHook) {
  auto apex_manifest = ParseManifest(
      "{\"name\": \"com.android.example.apex\", \"version\": 1}\n");
  ASSERT_TRUE(apex_manifest.Ok());
  EXPECT_EQ("", std::string(apex_manifest->postinstallhook()));
}

TEST(ApexManifestTest, PostInstallHook) {
  auto apex_manifest = ParseManifest(
      "{\"name\": \"com.android.example.apex\", \"version\": 1, "
      "\"postInstallHook\": \"bin/postInstallHook\"}\n");
  ASSERT_TRUE(apex_manifest.Ok());
  EXPECT_EQ("bin/postInstallHook",
            std::string(apex_manifest->postinstallhook()));
}

TEST(ApexManifestTest, UnparsableManifest) {
  auto apex_manifest = ParseManifest("This is an invalid pony");
  ASSERT_FALSE(apex_manifest.Ok());
  EXPECT_EQ(apex_manifest.ErrorMessage(),
            std::string("Failed to parse APEX Manifest JSON config: Unexpected "
                        "token.\nThis is an invalid p\n^"))
      << apex_manifest.ErrorMessage();
}

}  // namespace apex
}  // namespace android

int main(int argc, char** argv) {
  android::base::InitLogging(argv, &android::base::StderrLogger);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
