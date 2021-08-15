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

#include "update_engine/omaha_request_params.h"

#include <stdio.h>

#include <string>

#include <base/files/file_util.h>
#include <base/files/scoped_temp_dir.h>
#include <gtest/gtest.h>

#include "update_engine/common/constants.h"
#include "update_engine/common/fake_prefs.h"
#include "update_engine/common/platform_constants.h"
#include "update_engine/common/test_utils.h"
#include "update_engine/common/utils.h"
#include "update_engine/fake_system_state.h"

using chromeos_update_engine::test_utils::WriteFileString;
using std::string;

namespace chromeos_update_engine {

class OmahaRequestParamsTest : public ::testing::Test {
 public:
  OmahaRequestParamsTest() : params_(&fake_system_state_) {}

 protected:
  void SetUp() override {
    // Create a uniquely named test directory.
    ASSERT_TRUE(tempdir_.CreateUniqueTempDir());
    params_.set_root(tempdir_.GetPath().value());
    SetLockDown(false);
    fake_system_state_.set_prefs(&fake_prefs_);
  }

  void SetLockDown(bool locked_down) {
    fake_system_state_.fake_hardware()->SetIsOfficialBuild(locked_down);
    fake_system_state_.fake_hardware()->SetIsNormalBootMode(locked_down);
  }

  FakeSystemState fake_system_state_;
  OmahaRequestParams params_{&fake_system_state_};
  FakePrefs fake_prefs_;

  base::ScopedTempDir tempdir_;
};

namespace {
string GetMachineType() {
  string machine_type;
  if (!utils::ReadPipe("uname -m", &machine_type))
    return "";
  // Strip anything from the first newline char.
  size_t newline_pos = machine_type.find('\n');
  if (newline_pos != string::npos)
    machine_type.erase(newline_pos);
  return machine_type;
}
}  // namespace

TEST_F(OmahaRequestParamsTest, MissingChannelTest) {
  EXPECT_TRUE(params_.Init("", "", false));
  // By default, if no channel is set, we should track the stable-channel.
  EXPECT_EQ("stable-channel", params_.target_channel());
}

TEST_F(OmahaRequestParamsTest, ForceVersionTest) {
  EXPECT_TRUE(params_.Init("ForcedVersion", "", false));
  EXPECT_EQ(string("ForcedVersion_") + GetMachineType(), params_.os_sp());
  EXPECT_EQ("ForcedVersion", params_.app_version());
}

TEST_F(OmahaRequestParamsTest, ForcedURLTest) {
  EXPECT_TRUE(params_.Init("", "http://forced.google.com", false));
  EXPECT_EQ("http://forced.google.com", params_.update_url());
}

TEST_F(OmahaRequestParamsTest, MissingURLTest) {
  EXPECT_TRUE(params_.Init("", "", false));
  EXPECT_EQ(constants::kOmahaDefaultProductionURL, params_.update_url());
}

TEST_F(OmahaRequestParamsTest, DeltaOKTest) {
  EXPECT_TRUE(params_.Init("", "", false));
  EXPECT_TRUE(params_.delta_okay());
}

TEST_F(OmahaRequestParamsTest, NoDeltasTest) {
  ASSERT_TRUE(
      WriteFileString(tempdir_.GetPath().Append(".nodelta").value(), ""));
  EXPECT_TRUE(params_.Init("", "", false));
  EXPECT_FALSE(params_.delta_okay());
}

TEST_F(OmahaRequestParamsTest, SetTargetChannelTest) {
  {
    OmahaRequestParams params(&fake_system_state_);
    params.set_root(tempdir_.GetPath().value());
    EXPECT_TRUE(params.Init("", "", false));
    EXPECT_TRUE(params.SetTargetChannel("canary-channel", false, nullptr));
    EXPECT_FALSE(params.mutable_image_props_.is_powerwash_allowed);
  }
  params_.set_root(tempdir_.GetPath().value());
  EXPECT_TRUE(params_.Init("", "", false));
  EXPECT_EQ("canary-channel", params_.target_channel());
  EXPECT_FALSE(params_.mutable_image_props_.is_powerwash_allowed);
}

TEST_F(OmahaRequestParamsTest, SetIsPowerwashAllowedTest) {
  {
    OmahaRequestParams params(&fake_system_state_);
    params.set_root(tempdir_.GetPath().value());
    EXPECT_TRUE(params.Init("", "", false));
    EXPECT_TRUE(params.SetTargetChannel("canary-channel", true, nullptr));
    EXPECT_TRUE(params.mutable_image_props_.is_powerwash_allowed);
  }
  params_.set_root(tempdir_.GetPath().value());
  EXPECT_TRUE(params_.Init("", "", false));
  EXPECT_EQ("canary-channel", params_.target_channel());
  EXPECT_TRUE(params_.mutable_image_props_.is_powerwash_allowed);
}

TEST_F(OmahaRequestParamsTest, SetTargetChannelInvalidTest) {
  {
    OmahaRequestParams params(&fake_system_state_);
    params.set_root(tempdir_.GetPath().value());
    SetLockDown(true);
    EXPECT_TRUE(params.Init("", "", false));
    params.image_props_.allow_arbitrary_channels = false;
    string error_message;
    EXPECT_FALSE(
        params.SetTargetChannel("dogfood-channel", true, &error_message));
    // The error message should include a message about the valid channels.
    EXPECT_NE(string::npos, error_message.find("stable-channel"));
    EXPECT_FALSE(params.mutable_image_props_.is_powerwash_allowed);
  }
  params_.set_root(tempdir_.GetPath().value());
  EXPECT_TRUE(params_.Init("", "", false));
  EXPECT_EQ("stable-channel", params_.target_channel());
  EXPECT_FALSE(params_.mutable_image_props_.is_powerwash_allowed);
}

TEST_F(OmahaRequestParamsTest, IsValidChannelTest) {
  EXPECT_TRUE(params_.IsValidChannel("canary-channel"));
  EXPECT_TRUE(params_.IsValidChannel("stable-channel"));
  EXPECT_TRUE(params_.IsValidChannel("beta-channel"));
  EXPECT_TRUE(params_.IsValidChannel("dev-channel"));
  EXPECT_FALSE(params_.IsValidChannel("testimage-channel"));
  EXPECT_FALSE(params_.IsValidChannel("dogfood-channel"));
  EXPECT_FALSE(params_.IsValidChannel("some-channel"));
  EXPECT_FALSE(params_.IsValidChannel(""));
  params_.image_props_.allow_arbitrary_channels = true;
  EXPECT_TRUE(params_.IsValidChannel("some-channel"));
  EXPECT_FALSE(params_.IsValidChannel("wrong-suffix"));
  EXPECT_FALSE(params_.IsValidChannel(""));
}

TEST_F(OmahaRequestParamsTest, SetTargetChannelWorks) {
  params_.set_target_channel("dev-channel");
  EXPECT_EQ("dev-channel", params_.target_channel());

  // When an invalid value is set, it should be ignored.
  EXPECT_FALSE(params_.SetTargetChannel("invalid-channel", false, nullptr));
  EXPECT_EQ("dev-channel", params_.target_channel());

  // When set to a valid value, it should take effect.
  EXPECT_TRUE(params_.SetTargetChannel("beta-channel", true, nullptr));
  EXPECT_EQ("beta-channel", params_.target_channel());

  // When set to the same value, it should be idempotent.
  EXPECT_TRUE(params_.SetTargetChannel("beta-channel", true, nullptr));
  EXPECT_EQ("beta-channel", params_.target_channel());

  // When set to a valid value while a change is already pending, it should
  // succeed.
  EXPECT_TRUE(params_.SetTargetChannel("stable-channel", true, nullptr));
  EXPECT_EQ("stable-channel", params_.target_channel());

  // Set a different channel in mutable_image_props_.
  params_.set_target_channel("stable-channel");

  // When set to a valid value while a change is already pending, it should
  // succeed.
  params_.Init("", "", false);
  EXPECT_TRUE(params_.SetTargetChannel("beta-channel", true, nullptr));
  // The target channel should reflect the change, but the download channel
  // should continue to retain the old value ...
  EXPECT_EQ("beta-channel", params_.target_channel());
  EXPECT_EQ("stable-channel", params_.download_channel());

  // ... until we update the download channel explicitly.
  params_.UpdateDownloadChannel();
  EXPECT_EQ("beta-channel", params_.download_channel());
  EXPECT_EQ("beta-channel", params_.target_channel());
}

TEST_F(OmahaRequestParamsTest, ChannelIndexTest) {
  int canary = params_.GetChannelIndex("canary-channel");
  int dev = params_.GetChannelIndex("dev-channel");
  int beta = params_.GetChannelIndex("beta-channel");
  int stable = params_.GetChannelIndex("stable-channel");
  EXPECT_LE(canary, dev);
  EXPECT_LE(dev, beta);
  EXPECT_LE(beta, stable);

  // testimage-channel or other names are not recognized, so index will be -1.
  int testimage = params_.GetChannelIndex("testimage-channel");
  int bogus = params_.GetChannelIndex("bogus-channel");
  EXPECT_EQ(-1, testimage);
  EXPECT_EQ(-1, bogus);
}

TEST_F(OmahaRequestParamsTest, ToMoreStableChannelFlagTest) {
  params_.image_props_.current_channel = "canary-channel";
  params_.download_channel_ = "stable-channel";
  EXPECT_TRUE(params_.ToMoreStableChannel());
  params_.image_props_.current_channel = "stable-channel";
  EXPECT_FALSE(params_.ToMoreStableChannel());
  params_.download_channel_ = "beta-channel";
  EXPECT_FALSE(params_.ToMoreStableChannel());
}

TEST_F(OmahaRequestParamsTest, ShouldPowerwashTest) {
  params_.mutable_image_props_.is_powerwash_allowed = false;
  EXPECT_FALSE(params_.ShouldPowerwash());
  params_.mutable_image_props_.is_powerwash_allowed = true;
  params_.image_props_.allow_arbitrary_channels = true;
  params_.image_props_.current_channel = "foo-channel";
  params_.download_channel_ = "bar-channel";
  EXPECT_TRUE(params_.ShouldPowerwash());
  params_.image_props_.allow_arbitrary_channels = false;
  params_.image_props_.current_channel = "canary-channel";
  params_.download_channel_ = "stable-channel";
  EXPECT_TRUE(params_.ShouldPowerwash());
}

TEST_F(OmahaRequestParamsTest, CollectECFWVersionsTest) {
  params_.hwid_ = string("STUMPY ALEX 12345");
  EXPECT_FALSE(params_.CollectECFWVersions());

  params_.hwid_ = string("SNOW 12345");
  EXPECT_TRUE(params_.CollectECFWVersions());
}

}  // namespace chromeos_update_engine
