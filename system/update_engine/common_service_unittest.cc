//
// Copyright (C) 2014 The Android Open Source Project
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

#include "update_engine/common_service.h"

#include <gtest/gtest.h>
#include <string>
#include <vector>

#include <brillo/errors/error.h>
#include <policy/libpolicy.h>
#include <policy/mock_device_policy.h>

#include "update_engine/common/fake_prefs.h"
#include "update_engine/fake_system_state.h"
#include "update_engine/omaha_utils.h"

using std::string;
using std::vector;
using testing::_;
using testing::Return;
using testing::SetArgPointee;
using update_engine::UpdateAttemptFlags;

namespace chromeos_update_engine {

class UpdateEngineServiceTest : public ::testing::Test {
 protected:
  UpdateEngineServiceTest()
      : mock_update_attempter_(fake_system_state_.mock_update_attempter()),
        common_service_(&fake_system_state_) {}

  void SetUp() override { fake_system_state_.set_device_policy(nullptr); }

  // Fake/mock infrastructure.
  FakeSystemState fake_system_state_;
  policy::MockDevicePolicy mock_device_policy_;

  // Shortcut for fake_system_state_.mock_update_attempter().
  MockUpdateAttempter* mock_update_attempter_;

  brillo::ErrorPtr error_;
  UpdateEngineService common_service_;
};

TEST_F(UpdateEngineServiceTest, AttemptUpdate) {
  EXPECT_CALL(
      *mock_update_attempter_,
      CheckForUpdate("app_ver", "url", UpdateAttemptFlags::kFlagNonInteractive))
      .WillOnce(Return(true));

  // The non-interactive flag needs to be passed through to CheckForUpdate.
  bool result = false;
  EXPECT_TRUE(
      common_service_.AttemptUpdate(&error_,
                                    "app_ver",
                                    "url",
                                    UpdateAttemptFlags::kFlagNonInteractive,
                                    &result));
  EXPECT_EQ(nullptr, error_);
  EXPECT_TRUE(result);
}

TEST_F(UpdateEngineServiceTest, AttemptUpdateReturnsFalse) {
  EXPECT_CALL(*mock_update_attempter_,
              CheckForUpdate("app_ver", "url", UpdateAttemptFlags::kNone))
      .WillOnce(Return(false));
  bool result = true;
  EXPECT_TRUE(common_service_.AttemptUpdate(
      &error_, "app_ver", "url", UpdateAttemptFlags::kNone, &result));
  EXPECT_EQ(nullptr, error_);
  EXPECT_FALSE(result);
}

TEST_F(UpdateEngineServiceTest, AttemptInstall) {
  EXPECT_CALL(*mock_update_attempter_, CheckForInstall(_, _))
      .WillOnce(Return(true));

  EXPECT_TRUE(common_service_.AttemptInstall(&error_, "", {}));
  EXPECT_EQ(nullptr, error_);
}

TEST_F(UpdateEngineServiceTest, AttemptInstallReturnsFalse) {
  EXPECT_CALL(*mock_update_attempter_, CheckForInstall(_, _))
      .WillOnce(Return(false));

  EXPECT_FALSE(common_service_.AttemptInstall(&error_, "", {}));
}

// SetChannel is allowed when there's no device policy (the device is not
// enterprise enrolled).
TEST_F(UpdateEngineServiceTest, SetChannelWithNoPolicy) {
  EXPECT_CALL(*mock_update_attempter_, RefreshDevicePolicy());
  // If SetTargetChannel is called it means the policy check passed.
  EXPECT_CALL(*fake_system_state_.mock_request_params(),
              SetTargetChannel("stable-channel", true, _))
      .WillOnce(Return(true));
  EXPECT_TRUE(common_service_.SetChannel(&error_, "stable-channel", true));
  ASSERT_EQ(nullptr, error_);
}

// When the policy is present, the delegated value should be checked.
TEST_F(UpdateEngineServiceTest, SetChannelWithDelegatedPolicy) {
  policy::MockDevicePolicy mock_device_policy;
  fake_system_state_.set_device_policy(&mock_device_policy);
  EXPECT_CALL(mock_device_policy, GetReleaseChannelDelegated(_))
      .WillOnce(DoAll(SetArgPointee<0>(true), Return(true)));
  EXPECT_CALL(*fake_system_state_.mock_request_params(),
              SetTargetChannel("beta-channel", true, _))
      .WillOnce(Return(true));

  EXPECT_TRUE(common_service_.SetChannel(&error_, "beta-channel", true));
  ASSERT_EQ(nullptr, error_);
}

// When passing an invalid value (SetTargetChannel fails) an error should be
// raised.
TEST_F(UpdateEngineServiceTest, SetChannelWithInvalidChannel) {
  EXPECT_CALL(*mock_update_attempter_, RefreshDevicePolicy());
  EXPECT_CALL(*fake_system_state_.mock_request_params(),
              SetTargetChannel("foo-channel", true, _))
      .WillOnce(Return(false));

  EXPECT_FALSE(common_service_.SetChannel(&error_, "foo-channel", true));
  ASSERT_NE(nullptr, error_);
  EXPECT_TRUE(error_->HasError(UpdateEngineService::kErrorDomain,
                               UpdateEngineService::kErrorFailed));
}

TEST_F(UpdateEngineServiceTest, GetChannel) {
  fake_system_state_.mock_request_params()->set_current_channel("current");
  fake_system_state_.mock_request_params()->set_target_channel("target");
  string channel;
  EXPECT_TRUE(common_service_.GetChannel(
      &error_, true /* get_current_channel */, &channel));
  EXPECT_EQ(nullptr, error_);
  EXPECT_EQ("current", channel);

  EXPECT_TRUE(common_service_.GetChannel(
      &error_, false /* get_current_channel */, &channel));
  EXPECT_EQ(nullptr, error_);
  EXPECT_EQ("target", channel);
}

TEST_F(UpdateEngineServiceTest, ResetStatusSucceeds) {
  EXPECT_CALL(*mock_update_attempter_, ResetStatus()).WillOnce(Return(true));
  EXPECT_TRUE(common_service_.ResetStatus(&error_));
  EXPECT_EQ(nullptr, error_);
}

TEST_F(UpdateEngineServiceTest, ResetStatusFails) {
  EXPECT_CALL(*mock_update_attempter_, ResetStatus()).WillOnce(Return(false));
  EXPECT_FALSE(common_service_.ResetStatus(&error_));
  ASSERT_NE(nullptr, error_);
  EXPECT_TRUE(error_->HasError(UpdateEngineService::kErrorDomain,
                               UpdateEngineService::kErrorFailed));
}

TEST_F(UpdateEngineServiceTest, GetEolStatusTest) {
  FakePrefs fake_prefs;
  fake_system_state_.set_prefs(&fake_prefs);
  // The default value should be "supported".
  int32_t eol_status = static_cast<int32_t>(EolStatus::kEol);
  EXPECT_TRUE(common_service_.GetEolStatus(&error_, &eol_status));
  EXPECT_EQ(nullptr, error_);
  EXPECT_EQ(EolStatus::kSupported, static_cast<EolStatus>(eol_status));

  fake_prefs.SetString(kPrefsOmahaEolStatus, "security-only");
  EXPECT_TRUE(common_service_.GetEolStatus(&error_, &eol_status));
  EXPECT_EQ(nullptr, error_);
  EXPECT_EQ(EolStatus::kSecurityOnly, static_cast<EolStatus>(eol_status));
}

}  // namespace chromeos_update_engine
