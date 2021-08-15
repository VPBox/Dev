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

#include "update_engine/update_manager/android_things_policy.h"

#include <memory>

#include "update_engine/update_manager/next_update_check_policy_impl.h"
#include "update_engine/update_manager/policy_test_utils.h"

using base::Time;
using base::TimeDelta;
using chromeos_update_engine::ErrorCode;
using chromeos_update_engine::InstallPlan;

namespace chromeos_update_manager {

class UmAndroidThingsPolicyTest : public UmPolicyTestBase {
 protected:
  UmAndroidThingsPolicyTest() {
    policy_ = std::make_unique<AndroidThingsPolicy>();
  }

  void SetUpDefaultState() override {
    UmPolicyTestBase::SetUpDefaultState();

    // For the purpose of the tests, this is an official build
    fake_state_.system_provider()->var_is_official_build()->reset(
        new bool(true));
    // NOLINTNEXTLINE(readability/casting)
    fake_state_.system_provider()->var_num_slots()->reset(new unsigned int(2));
  }

  // Configures the policy to return a desired value from UpdateCheckAllowed by
  // faking the current wall clock time as needed. Restores the default state.
  // This is used when testing policies that depend on this one.
  virtual void SetUpdateCheckAllowed(bool allow_check) {
    Time next_update_check;
    CallMethodWithContext(&NextUpdateCheckTimePolicyImpl::NextUpdateCheckTime,
                          &next_update_check,
                          AndroidThingsPolicy::kNextUpdateCheckPolicyConstants);
    SetUpDefaultState();
    Time curr_time = next_update_check;
    if (allow_check)
      curr_time += TimeDelta::FromSeconds(1);
    else
      curr_time -= TimeDelta::FromSeconds(1);
    fake_clock_.SetWallclockTime(curr_time);
  }
};

TEST_F(UmAndroidThingsPolicyTest, UpdateCheckAllowedWaitsForTheTimeout) {
  // We get the next update_check timestamp from the policy's private method
  // and then we check the public method respects that value on the normal
  // case.
  Time next_update_check;
  Time last_checked_time =
      fake_clock_.GetWallclockTime() + TimeDelta::FromMinutes(1234);

  LOG(INFO) << "last_checked_time: " << last_checked_time;
  fake_state_.updater_provider()->var_last_checked_time()->reset(
      new Time(last_checked_time));
  CallMethodWithContext(&NextUpdateCheckTimePolicyImpl::NextUpdateCheckTime,
                        &next_update_check,
                        AndroidThingsPolicy::kNextUpdateCheckPolicyConstants);
  LOG(INFO) << "Next check allowed at: " << next_update_check;

  // Check that the policy blocks until the next_update_check is reached.
  SetUpDefaultClock();
  SetUpDefaultState();
  fake_state_.updater_provider()->var_last_checked_time()->reset(
      new Time(last_checked_time));
  fake_clock_.SetWallclockTime(next_update_check - TimeDelta::FromSeconds(1));

  UpdateCheckParams result;
  ExpectPolicyStatus(
      EvalStatus::kAskMeAgainLater, &Policy::UpdateCheckAllowed, &result);

  SetUpDefaultClock();
  SetUpDefaultState();
  fake_state_.updater_provider()->var_last_checked_time()->reset(
      new Time(last_checked_time));
  fake_clock_.SetWallclockTime(next_update_check + TimeDelta::FromSeconds(1));
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCheckAllowed, &result);
  EXPECT_TRUE(result.updates_enabled);
  EXPECT_FALSE(result.interactive);
}

TEST_F(UmAndroidThingsPolicyTest,
       UpdateCheckAllowedUpdatesDisabledForUnofficialBuilds) {
  // UpdateCheckAllowed should return kAskMeAgainLater if this is an unofficial
  // build; we don't want periodic update checks on developer images.

  fake_state_.system_provider()->var_is_official_build()->reset(
      new bool(false));

  UpdateCheckParams result;
  ExpectPolicyStatus(
      EvalStatus::kAskMeAgainLater, &Policy::UpdateCheckAllowed, &result);
}

TEST_F(UmAndroidThingsPolicyTest,
       UpdateCheckAllowedUpdatesDisabledWhenNotEnoughSlotsAbUpdates) {
  // UpdateCheckAllowed should return false (kSucceeded) if the image booted
  // without enough slots to do A/B updates.

  // NOLINTNEXTLINE(readability/casting)
  fake_state_.system_provider()->var_num_slots()->reset(new unsigned int(1));

  UpdateCheckParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCheckAllowed, &result);
  EXPECT_FALSE(result.updates_enabled);
}

TEST_F(UmAndroidThingsPolicyTest,
       UpdateCheckAllowedForcedUpdateRequestedInteractive) {
  // UpdateCheckAllowed should return true because a forced update request was
  // signaled for an interactive update.

  SetUpdateCheckAllowed(true);
  fake_state_.updater_provider()->var_forced_update_requested()->reset(
      new UpdateRequestStatus(UpdateRequestStatus::kInteractive));

  UpdateCheckParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCheckAllowed, &result);
  EXPECT_TRUE(result.updates_enabled);
  EXPECT_TRUE(result.interactive);
}

TEST_F(UmAndroidThingsPolicyTest,
       UpdateCheckAllowedForcedUpdateRequestedPeriodic) {
  // UpdateCheckAllowed should return true because a forced update request was
  // signaled for a periodic check.

  SetUpdateCheckAllowed(true);
  fake_state_.updater_provider()->var_forced_update_requested()->reset(
      new UpdateRequestStatus(UpdateRequestStatus::kPeriodic));

  UpdateCheckParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCheckAllowed, &result);
  EXPECT_TRUE(result.updates_enabled);
  EXPECT_FALSE(result.interactive);
}

TEST_F(UmAndroidThingsPolicyTest, UpdateCanBeAppliedOk) {
  // UpdateCanBeApplied should return kSucceeded in the base case

  InstallPlan plan;
  ErrorCode result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanBeApplied, &result, &plan);

  EXPECT_EQ(ErrorCode::kSuccess, result);
}

TEST_F(UmAndroidThingsPolicyTest, UpdateCanBeAppliedRestricted) {
  // UpdateCanBeApplied should return kOmahaUpdateDeferredPerPolicy in
  // when the restricted flag is set in the Updater.

  fake_state_.updater_provider()->var_update_restrictions()->reset(
      new UpdateRestrictions(UpdateRestrictions::kRestrictDownloading));

  InstallPlan plan;
  ErrorCode result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanBeApplied, &result, &plan);

  EXPECT_EQ(ErrorCode::kOmahaUpdateDeferredPerPolicy, result);
}

}  // namespace chromeos_update_manager
