//
// Copyright (C) 2016 The Android Open Source Project
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

#include "update_engine/image_properties.h"

#include <string>

#include <base/files/file_util.h>
#include <base/files/scoped_temp_dir.h>
#include <gtest/gtest.h>

#include "update_engine/common/constants.h"
#include "update_engine/common/test_utils.h"
#include "update_engine/fake_system_state.h"

using chromeos_update_engine::test_utils::WriteFileString;
using std::string;

namespace chromeos_update_engine {

class ImagePropertiesTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // Create a uniquely named test directory.
    ASSERT_TRUE(tempdir_.CreateUniqueTempDir());
    EXPECT_TRUE(base::CreateDirectory(tempdir_.GetPath().Append("etc")));
    EXPECT_TRUE(base::CreateDirectory(base::FilePath(
        tempdir_.GetPath().value() + kStatefulPartition + "/etc")));
    test::SetImagePropertiesRootPrefix(tempdir_.GetPath().value().c_str());
    SetLockDown(false);
  }

  void SetLockDown(bool locked_down) {
    fake_system_state_.fake_hardware()->SetIsOfficialBuild(locked_down);
    fake_system_state_.fake_hardware()->SetIsNormalBootMode(locked_down);
  }

  FakeSystemState fake_system_state_;

  base::ScopedTempDir tempdir_;
};

TEST_F(ImagePropertiesTest, SimpleTest) {
  ASSERT_TRUE(
      WriteFileString(tempdir_.GetPath().Append("etc/lsb-release").value(),
                      "CHROMEOS_RELEASE_BOARD=arm-generic\n"
                      "CHROMEOS_RELEASE_FOO=bar\n"
                      "CHROMEOS_RELEASE_VERSION=0.2.2.3\n"
                      "CHROMEOS_RELEASE_TRACK=dev-channel\n"
                      "CHROMEOS_AUSERVER=http://www.google.com"));
  ImageProperties props = LoadImageProperties(&fake_system_state_);
  EXPECT_EQ("arm-generic", props.board);
  EXPECT_EQ("{87efface-864d-49a5-9bb3-4b050a7c227a}", props.product_id);
  EXPECT_EQ("0.2.2.3", props.version);
  EXPECT_EQ("dev-channel", props.current_channel);
  EXPECT_EQ("http://www.google.com", props.omaha_url);
}

TEST_F(ImagePropertiesTest, AppIDTest) {
  ASSERT_TRUE(WriteFileString(
      tempdir_.GetPath().Append("etc/lsb-release").value(),
      "CHROMEOS_RELEASE_APPID={58c35cef-9d30-476e-9098-ce20377d535d}"));
  ImageProperties props = LoadImageProperties(&fake_system_state_);
  EXPECT_EQ("{58c35cef-9d30-476e-9098-ce20377d535d}", props.product_id);
}

TEST_F(ImagePropertiesTest, ConfusingReleaseTest) {
  ASSERT_TRUE(
      WriteFileString(tempdir_.GetPath().Append("etc/lsb-release").value(),
                      "CHROMEOS_RELEASE_FOO=CHROMEOS_RELEASE_VERSION=1.2.3.4\n"
                      "CHROMEOS_RELEASE_VERSION=0.2.2.3"));
  ImageProperties props = LoadImageProperties(&fake_system_state_);
  EXPECT_EQ("0.2.2.3", props.version);
}

TEST_F(ImagePropertiesTest, MissingVersionTest) {
  ImageProperties props = LoadImageProperties(&fake_system_state_);
  EXPECT_EQ("", props.version);
}

TEST_F(ImagePropertiesTest, OverrideTest) {
  ASSERT_TRUE(
      WriteFileString(tempdir_.GetPath().Append("etc/lsb-release").value(),
                      "CHROMEOS_RELEASE_BOARD=arm-generic\n"
                      "CHROMEOS_RELEASE_FOO=bar\n"
                      "CHROMEOS_RELEASE_TRACK=dev-channel\n"
                      "CHROMEOS_AUSERVER=http://www.google.com"));
  ASSERT_TRUE(WriteFileString(
      tempdir_.GetPath().value() + kStatefulPartition + "/etc/lsb-release",
      "CHROMEOS_RELEASE_BOARD=x86-generic\n"
      "CHROMEOS_RELEASE_TRACK=beta-channel\n"
      "CHROMEOS_AUSERVER=https://www.google.com"));
  ImageProperties props = LoadImageProperties(&fake_system_state_);
  EXPECT_EQ("x86-generic", props.board);
  EXPECT_EQ("dev-channel", props.current_channel);
  EXPECT_EQ("https://www.google.com", props.omaha_url);
  MutableImageProperties mutable_props =
      LoadMutableImageProperties(&fake_system_state_);
  EXPECT_EQ("beta-channel", mutable_props.target_channel);
}

TEST_F(ImagePropertiesTest, OverrideLockDownTest) {
  ASSERT_TRUE(
      WriteFileString(tempdir_.GetPath().Append("etc/lsb-release").value(),
                      "CHROMEOS_RELEASE_BOARD=arm-generic\n"
                      "CHROMEOS_RELEASE_FOO=bar\n"
                      "CHROMEOS_RELEASE_TRACK=dev-channel\n"
                      "CHROMEOS_AUSERVER=https://www.google.com"));
  ASSERT_TRUE(WriteFileString(
      tempdir_.GetPath().value() + kStatefulPartition + "/etc/lsb-release",
      "CHROMEOS_RELEASE_BOARD=x86-generic\n"
      "CHROMEOS_RELEASE_TRACK=stable-channel\n"
      "CHROMEOS_AUSERVER=http://www.google.com"));
  SetLockDown(true);
  ImageProperties props = LoadImageProperties(&fake_system_state_);
  EXPECT_EQ("arm-generic", props.board);
  EXPECT_EQ("dev-channel", props.current_channel);
  EXPECT_EQ("https://www.google.com", props.omaha_url);
  MutableImageProperties mutable_props =
      LoadMutableImageProperties(&fake_system_state_);
  EXPECT_EQ("stable-channel", mutable_props.target_channel);
}

TEST_F(ImagePropertiesTest, BoardAppIdUsedForNonCanaryChannelTest) {
  ASSERT_TRUE(
      WriteFileString(tempdir_.GetPath().Append("etc/lsb-release").value(),
                      "CHROMEOS_RELEASE_APPID=r\n"
                      "CHROMEOS_BOARD_APPID=b\n"
                      "CHROMEOS_CANARY_APPID=c\n"
                      "CHROMEOS_RELEASE_TRACK=stable-channel\n"));
  ImageProperties props = LoadImageProperties(&fake_system_state_);
  EXPECT_EQ("stable-channel", props.current_channel);
  EXPECT_EQ("b", props.product_id);
}

TEST_F(ImagePropertiesTest, CanaryAppIdUsedForCanaryChannelTest) {
  ASSERT_TRUE(
      WriteFileString(tempdir_.GetPath().Append("etc/lsb-release").value(),
                      "CHROMEOS_RELEASE_APPID=r\n"
                      "CHROMEOS_BOARD_APPID=b\n"
                      "CHROMEOS_CANARY_APPID=c\n"
                      "CHROMEOS_RELEASE_TRACK=canary-channel\n"));
  ImageProperties props = LoadImageProperties(&fake_system_state_);
  EXPECT_EQ("canary-channel", props.current_channel);
  EXPECT_EQ("c", props.canary_product_id);
}

TEST_F(ImagePropertiesTest, ReleaseAppIdUsedAsDefaultTest) {
  ASSERT_TRUE(
      WriteFileString(tempdir_.GetPath().Append("etc/lsb-release").value(),
                      "CHROMEOS_RELEASE_APPID=r\n"
                      "CHROMEOS_CANARY_APPID=c\n"
                      "CHROMEOS_RELEASE_TRACK=stable-channel\n"));
  ImageProperties props = LoadImageProperties(&fake_system_state_);
  EXPECT_EQ("stable-channel", props.current_channel);
  EXPECT_EQ("r", props.product_id);
}

}  // namespace chromeos_update_engine
