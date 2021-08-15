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

#include "update_engine/update_manager/chromeos_policy.h"

#include <memory>
#include <set>

#include "update_engine/update_manager/next_update_check_policy_impl.h"
#include "update_engine/update_manager/policy_test_utils.h"
#include "update_engine/update_manager/weekly_time.h"

using base::Time;
using base::TimeDelta;
using chromeos_update_engine::ConnectionTethering;
using chromeos_update_engine::ConnectionType;
using chromeos_update_engine::ErrorCode;
using chromeos_update_engine::InstallPlan;
using std::set;
using std::string;

namespace chromeos_update_manager {

class UmChromeOSPolicyTest : public UmPolicyTestBase {
 protected:
  UmChromeOSPolicyTest() : UmPolicyTestBase() {
    policy_ = std::make_unique<ChromeOSPolicy>();
  }

  void SetUp() override {
    UmPolicyTestBase::SetUp();
    SetUpDefaultDevicePolicy();
  }

  void SetUpDefaultState() override {
    UmPolicyTestBase::SetUpDefaultState();

    // OOBE is enabled by default.
    fake_state_.config_provider()->var_is_oobe_enabled()->reset(new bool(true));

    // For the purpose of the tests, this is an official build and OOBE was
    // completed.
    fake_state_.system_provider()->var_is_official_build()->reset(
        new bool(true));
    fake_state_.system_provider()->var_is_oobe_complete()->reset(
        new bool(true));
    // NOLINTNEXTLINE(readability/casting)
    fake_state_.system_provider()->var_num_slots()->reset(new unsigned int(2));

    // Connection is wifi, untethered.
    fake_state_.shill_provider()->var_conn_type()->reset(
        new ConnectionType(ConnectionType::kWifi));
    fake_state_.shill_provider()->var_conn_tethering()->reset(
        new ConnectionTethering(ConnectionTethering::kNotDetected));
  }

  // Sets up a default device policy that does not impose any restrictions
  // (HTTP) nor enables any features (P2P).
  void SetUpDefaultDevicePolicy() {
    fake_state_.device_policy_provider()->var_device_policy_is_loaded()->reset(
        new bool(true));
    fake_state_.device_policy_provider()->var_update_disabled()->reset(
        new bool(false));
    fake_state_.device_policy_provider()
        ->var_allowed_connection_types_for_update()
        ->reset(nullptr);
    fake_state_.device_policy_provider()->var_scatter_factor()->reset(
        new TimeDelta());
    fake_state_.device_policy_provider()->var_http_downloads_enabled()->reset(
        new bool(true));
    fake_state_.device_policy_provider()->var_au_p2p_enabled()->reset(
        new bool(false));
    fake_state_.device_policy_provider()
        ->var_release_channel_delegated()
        ->reset(new bool(true));
    fake_state_.device_policy_provider()
        ->var_disallowed_time_intervals()
        ->reset(new WeeklyTimeIntervalVector());
  }

  // Configures the policy to return a desired value from UpdateCheckAllowed by
  // faking the current wall clock time as needed. Restores the default state.
  // This is used when testing policies that depend on this one.
  //
  // Note that the default implementation relies on NextUpdateCheckPolicyImpl to
  // set the FakeClock to the appropriate time.
  virtual void SetUpdateCheckAllowed(bool allow_check) {
    Time next_update_check;
    CallMethodWithContext(&NextUpdateCheckTimePolicyImpl::NextUpdateCheckTime,
                          &next_update_check,
                          ChromeOSPolicy::kNextUpdateCheckPolicyConstants);
    SetUpDefaultState();
    SetUpDefaultDevicePolicy();
    Time curr_time = next_update_check;
    if (allow_check)
      curr_time += TimeDelta::FromSeconds(1);
    else
      curr_time -= TimeDelta::FromSeconds(1);
    fake_clock_.SetWallclockTime(curr_time);
  }

  // Sets the policies required for a kiosk app to control Chrome OS version:
  // - AllowKioskAppControlChromeVersion = True
  // - UpdateDisabled = True
  // In the kiosk app manifest:
  // - RequiredPlatformVersion = 1234.
  void SetKioskAppControlsChromeOsVersion() {
    fake_state_.device_policy_provider()
        ->var_allow_kiosk_app_control_chrome_version()
        ->reset(new bool(true));
    fake_state_.device_policy_provider()->var_update_disabled()->reset(
        new bool(true));
    fake_state_.system_provider()->var_kiosk_required_platform_version()->reset(
        new string("1234."));
  }

  // Sets up a test with the value of RollbackToTargetVersion policy (and
  // whether it's set), and returns the value of
  // UpdateCheckParams.rollback_allowed.
  bool TestRollbackAllowed(bool set_policy,
                           RollbackToTargetVersion rollback_to_target_version) {
    // Update check is allowed, response includes attributes for use in the
    // request.
    SetUpdateCheckAllowed(true);

    if (set_policy) {
      // Override RollbackToTargetVersion device policy attribute.
      fake_state_.device_policy_provider()
          ->var_rollback_to_target_version()
          ->reset(new RollbackToTargetVersion(rollback_to_target_version));
    }

    UpdateCheckParams result;
    ExpectPolicyStatus(
        EvalStatus::kSucceeded, &Policy::UpdateCheckAllowed, &result);
    return result.rollback_allowed;
  }

  // Sets up a test with the given intervals and the current fake wallclock
  // time.
  void TestDisallowedTimeIntervals(const WeeklyTimeIntervalVector& intervals,
                                   const ErrorCode& expected_error_code,
                                   bool kiosk) {
    SetUpDefaultTimeProvider();
    if (kiosk)
      fake_state_.device_policy_provider()
          ->var_auto_launched_kiosk_app_id()
          ->reset(new string("myapp"));
    fake_state_.device_policy_provider()
        ->var_disallowed_time_intervals()
        ->reset(new WeeklyTimeIntervalVector(intervals));

    // Check that |expected_status| matches the value of UpdateCheckAllowed
    ErrorCode result;
    InstallPlan install_plan;
    ExpectPolicyStatus(EvalStatus::kSucceeded,
                       &Policy::UpdateCanBeApplied,
                       &result,
                       &install_plan);
    EXPECT_EQ(result, expected_error_code);
  }
};

TEST_F(UmChromeOSPolicyTest, UpdateCheckAllowedWaitsForTheTimeout) {
  // We get the next update_check timestamp from the policy's private method
  // and then we check the public method respects that value on the normal
  // case.
  Time next_update_check;
  Time last_checked_time =
      fake_clock_.GetWallclockTime() + TimeDelta::FromMinutes(1234);

  fake_state_.updater_provider()->var_last_checked_time()->reset(
      new Time(last_checked_time));
  CallMethodWithContext(&NextUpdateCheckTimePolicyImpl::NextUpdateCheckTime,
                        &next_update_check,
                        ChromeOSPolicy::kNextUpdateCheckPolicyConstants);

  UpdateCheckParams result;

  // Check that the policy blocks until the next_update_check is reached.
  SetUpDefaultClock();
  SetUpDefaultState();
  fake_state_.updater_provider()->var_last_checked_time()->reset(
      new Time(last_checked_time));
  fake_clock_.SetWallclockTime(next_update_check - TimeDelta::FromSeconds(1));
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

TEST_F(UmChromeOSPolicyTest, UpdateCheckAllowedWaitsForOOBE) {
  // Update checks are deferred until OOBE is completed.

  // Ensure that update is not allowed even if wait period is satisfied.
  Time next_update_check;
  Time last_checked_time =
      fake_clock_.GetWallclockTime() + TimeDelta::FromMinutes(1234);

  fake_state_.updater_provider()->var_last_checked_time()->reset(
      new Time(last_checked_time));
  CallMethodWithContext(&NextUpdateCheckTimePolicyImpl::NextUpdateCheckTime,
                        &next_update_check,
                        ChromeOSPolicy::kNextUpdateCheckPolicyConstants);

  SetUpDefaultClock();
  SetUpDefaultState();
  fake_state_.updater_provider()->var_last_checked_time()->reset(
      new Time(last_checked_time));
  fake_clock_.SetWallclockTime(next_update_check + TimeDelta::FromSeconds(1));
  fake_state_.system_provider()->var_is_oobe_complete()->reset(new bool(false));

  UpdateCheckParams result;
  ExpectPolicyStatus(
      EvalStatus::kAskMeAgainLater, &Policy::UpdateCheckAllowed, &result);

  // Now check that it is allowed if OOBE is completed.
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

TEST_F(UmChromeOSPolicyTest, UpdateCheckAllowedWithAttributes) {
  // Update check is allowed, response includes attributes for use in the
  // request.
  SetUpdateCheckAllowed(true);

  // Override specific device policy attributes.
  fake_state_.device_policy_provider()->var_target_version_prefix()->reset(
      new string("1.2"));
  fake_state_.device_policy_provider()
      ->var_rollback_allowed_milestones()
      ->reset(new int(5));
  fake_state_.device_policy_provider()->var_release_channel_delegated()->reset(
      new bool(false));
  fake_state_.device_policy_provider()->var_release_channel()->reset(
      new string("foo-channel"));

  UpdateCheckParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCheckAllowed, &result);
  EXPECT_TRUE(result.updates_enabled);
  EXPECT_EQ("1.2", result.target_version_prefix);
  EXPECT_EQ(5, result.rollback_allowed_milestones);
  EXPECT_EQ("foo-channel", result.target_channel);
  EXPECT_FALSE(result.interactive);
}

TEST_F(UmChromeOSPolicyTest, UpdateCheckAllowedRollbackAndPowerwash) {
  EXPECT_TRUE(TestRollbackAllowed(
      true, RollbackToTargetVersion::kRollbackAndPowerwash));
}

TEST_F(UmChromeOSPolicyTest, UpdateCheckAllowedRollbackAndRestoreIfPossible) {
  // We're doing rollback even if we don't support data save and restore.
  EXPECT_TRUE(TestRollbackAllowed(
      true, RollbackToTargetVersion::kRollbackAndRestoreIfPossible));
}

TEST_F(UmChromeOSPolicyTest, UpdateCheckAllowedRollbackOnlyIfRestorePossible) {
  // We're not allowed to do rollback until we support data save and restore.
  EXPECT_FALSE(TestRollbackAllowed(
      true, RollbackToTargetVersion::kRollbackOnlyIfRestorePossible));
}

TEST_F(UmChromeOSPolicyTest, UpdateCheckAllowedRollbackDisabled) {
  EXPECT_FALSE(TestRollbackAllowed(true, RollbackToTargetVersion::kDisabled));
}

TEST_F(UmChromeOSPolicyTest, UpdateCheckAllowedRollbackUnspecified) {
  EXPECT_FALSE(
      TestRollbackAllowed(true, RollbackToTargetVersion::kUnspecified));
}

TEST_F(UmChromeOSPolicyTest, UpdateCheckAllowedRollbackNotSet) {
  EXPECT_FALSE(
      TestRollbackAllowed(false, RollbackToTargetVersion::kUnspecified));
}

TEST_F(UmChromeOSPolicyTest, UpdateCheckAllowedKioskRollbackAllowed) {
  SetKioskAppControlsChromeOsVersion();

  EXPECT_TRUE(TestRollbackAllowed(
      true, RollbackToTargetVersion::kRollbackAndPowerwash));
}

TEST_F(UmChromeOSPolicyTest, UpdateCheckAllowedKioskRollbackDisabled) {
  SetKioskAppControlsChromeOsVersion();

  EXPECT_FALSE(TestRollbackAllowed(true, RollbackToTargetVersion::kDisabled));
}

TEST_F(UmChromeOSPolicyTest, UpdateCheckAllowedKioskRollbackUnspecified) {
  SetKioskAppControlsChromeOsVersion();

  EXPECT_FALSE(
      TestRollbackAllowed(true, RollbackToTargetVersion::kUnspecified));
}

TEST_F(UmChromeOSPolicyTest, UpdateCheckAllowedKioskRollbackNotSet) {
  SetKioskAppControlsChromeOsVersion();

  EXPECT_FALSE(
      TestRollbackAllowed(false, RollbackToTargetVersion::kUnspecified));
}

TEST_F(UmChromeOSPolicyTest,
       UpdateCheckAllowedUpdatesDisabledForUnofficialBuilds) {
  // UpdateCheckAllowed should return kAskMeAgainLater if this is an unofficial
  // build; we don't want periodic update checks on developer images.

  fake_state_.system_provider()->var_is_official_build()->reset(
      new bool(false));

  UpdateCheckParams result;
  ExpectPolicyStatus(
      EvalStatus::kAskMeAgainLater, &Policy::UpdateCheckAllowed, &result);
}

TEST_F(UmChromeOSPolicyTest,
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

TEST_F(UmChromeOSPolicyTest, UpdateCheckAllowedUpdatesDisabledByPolicy) {
  // UpdateCheckAllowed should return kAskMeAgainLater because a device policy
  // is loaded and prohibits updates.

  SetUpdateCheckAllowed(false);
  fake_state_.device_policy_provider()->var_update_disabled()->reset(
      new bool(true));

  UpdateCheckParams result;
  ExpectPolicyStatus(
      EvalStatus::kAskMeAgainLater, &Policy::UpdateCheckAllowed, &result);
}

TEST_F(UmChromeOSPolicyTest,
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

TEST_F(UmChromeOSPolicyTest, UpdateCheckAllowedForcedUpdateRequestedPeriodic) {
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

TEST_F(UmChromeOSPolicyTest, UpdateCheckAllowedKioskPin) {
  // Update check is allowed.
  SetUpdateCheckAllowed(true);

  SetKioskAppControlsChromeOsVersion();

  UpdateCheckParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCheckAllowed, &result);
  EXPECT_TRUE(result.updates_enabled);
  EXPECT_EQ("1234.", result.target_version_prefix);
  EXPECT_FALSE(result.interactive);
}

TEST_F(UmChromeOSPolicyTest, UpdateCheckAllowedDisabledWhenNoKioskPin) {
  // Update check is allowed.
  SetUpdateCheckAllowed(true);

  // Disable AU policy is set but kiosk pin policy is set to false. Update is
  // disabled in such case.
  fake_state_.device_policy_provider()->var_update_disabled()->reset(
      new bool(true));
  fake_state_.device_policy_provider()
      ->var_allow_kiosk_app_control_chrome_version()
      ->reset(new bool(false));

  UpdateCheckParams result;
  ExpectPolicyStatus(
      EvalStatus::kAskMeAgainLater, &Policy::UpdateCheckAllowed, &result);
}

TEST_F(UmChromeOSPolicyTest, UpdateCheckAllowedKioskPinWithNoRequiredVersion) {
  // Update check is allowed.
  SetUpdateCheckAllowed(true);

  // AU disabled, allow kiosk to pin but there is no kiosk required platform
  // version (i.e. app does not provide the info). Update to latest in such
  // case.
  fake_state_.device_policy_provider()->var_update_disabled()->reset(
      new bool(true));
  fake_state_.device_policy_provider()
      ->var_allow_kiosk_app_control_chrome_version()
      ->reset(new bool(true));
  fake_state_.system_provider()->var_kiosk_required_platform_version()->reset(
      new string());

  UpdateCheckParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCheckAllowed, &result);
  EXPECT_TRUE(result.updates_enabled);
  EXPECT_TRUE(result.target_version_prefix.empty());
  EXPECT_FALSE(result.interactive);
}

TEST_F(UmChromeOSPolicyTest,
       UpdateCheckAllowedKioskPinWithFailedGetRequiredVersionCall) {
  // AU disabled, allow kiosk to pin but D-Bus call to get required platform
  // version failed. Defer update check in this case.
  fake_state_.device_policy_provider()->var_update_disabled()->reset(
      new bool(true));
  fake_state_.device_policy_provider()
      ->var_allow_kiosk_app_control_chrome_version()
      ->reset(new bool(true));
  fake_state_.system_provider()->var_kiosk_required_platform_version()->reset(
      nullptr);

  UpdateCheckParams result;
  ExpectPolicyStatus(
      EvalStatus::kAskMeAgainLater, &Policy::UpdateCheckAllowed, &result);
}

TEST_F(UmChromeOSPolicyTest, UpdateCanStartFailsCheckAllowedError) {
  // The UpdateCanStart policy fails, not being able to query
  // UpdateCheckAllowed.

  // Configure the UpdateCheckAllowed policy to fail.
  fake_state_.updater_provider()->var_updater_started_time()->reset(nullptr);

  // Check that the UpdateCanStart fails.
  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromMinutes(10));
  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kFailed, &Policy::UpdateCanStart, &result, update_state);
}

TEST_F(UmChromeOSPolicyTest, UpdateCanStartNotAllowedCheckDue) {
  // The UpdateCanStart policy returns false because we are due for another
  // update check. Ensure that download related values are still returned.

  SetUpdateCheckAllowed(true);

  // Check that the UpdateCanStart returns false.
  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromMinutes(10));
  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanStart, &result, update_state);
  EXPECT_FALSE(result.update_can_start);
  EXPECT_EQ(UpdateCannotStartReason::kCheckDue, result.cannot_start_reason);
  EXPECT_EQ(0, result.download_url_idx);
  EXPECT_EQ(0, result.download_url_num_errors);
}

TEST_F(UmChromeOSPolicyTest, UpdateCanStartAllowedNoDevicePolicy) {
  // The UpdateCanStart policy returns true; no device policy is loaded.

  SetUpdateCheckAllowed(false);
  fake_state_.device_policy_provider()->var_device_policy_is_loaded()->reset(
      new bool(false));

  // Check that the UpdateCanStart returns true with no further attributes.
  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromMinutes(10));
  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanStart, &result, update_state);
  EXPECT_TRUE(result.update_can_start);
  EXPECT_FALSE(result.p2p_downloading_allowed);
  EXPECT_FALSE(result.p2p_sharing_allowed);
  EXPECT_EQ(0, result.download_url_idx);
  EXPECT_TRUE(result.download_url_allowed);
  EXPECT_EQ(0, result.download_url_num_errors);
  EXPECT_FALSE(result.do_increment_failures);
}

TEST_F(UmChromeOSPolicyTest, UpdateCanStartAllowedBlankPolicy) {
  // The UpdateCanStart policy returns true; device policy is loaded but imposes
  // no restrictions on updating.

  SetUpdateCheckAllowed(false);

  // Check that the UpdateCanStart returns true.
  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromMinutes(10));
  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanStart, &result, update_state);
  EXPECT_TRUE(result.update_can_start);
  EXPECT_FALSE(result.p2p_downloading_allowed);
  EXPECT_FALSE(result.p2p_sharing_allowed);
  EXPECT_EQ(0, result.download_url_idx);
  EXPECT_TRUE(result.download_url_allowed);
  EXPECT_EQ(0, result.download_url_num_errors);
  EXPECT_FALSE(result.do_increment_failures);
}

TEST_F(UmChromeOSPolicyTest,
       UpdateCanStartNotAllowedBackoffNewWaitPeriodApplies) {
  // The UpdateCanStart policy returns false; failures are reported and a new
  // backoff period is enacted.

  SetUpdateCheckAllowed(false);

  const Time curr_time = fake_clock_.GetWallclockTime();
  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromSeconds(10));
  update_state.download_errors_max = 1;
  update_state.download_errors.emplace_back(
      0,
      ErrorCode::kDownloadTransferError,
      curr_time - TimeDelta::FromSeconds(8));
  update_state.download_errors.emplace_back(
      0,
      ErrorCode::kDownloadTransferError,
      curr_time - TimeDelta::FromSeconds(2));

  // Check that UpdateCanStart returns false and a new backoff expiry is
  // generated.
  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanStart, &result, update_state);
  EXPECT_FALSE(result.update_can_start);
  EXPECT_EQ(UpdateCannotStartReason::kBackoff, result.cannot_start_reason);
  EXPECT_TRUE(result.do_increment_failures);
  EXPECT_LT(curr_time, result.backoff_expiry);
}

TEST_F(UmChromeOSPolicyTest,
       UpdateCanStartNotAllowedBackoffPrevWaitPeriodStillApplies) {
  // The UpdateCanStart policy returns false; a previously enacted backoff
  // period still applies.

  SetUpdateCheckAllowed(false);

  const Time curr_time = fake_clock_.GetWallclockTime();
  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromSeconds(10));
  update_state.download_errors_max = 1;
  update_state.download_errors.emplace_back(
      0,
      ErrorCode::kDownloadTransferError,
      curr_time - TimeDelta::FromSeconds(8));
  update_state.download_errors.emplace_back(
      0,
      ErrorCode::kDownloadTransferError,
      curr_time - TimeDelta::FromSeconds(2));
  update_state.failures_last_updated = curr_time;
  update_state.backoff_expiry = curr_time + TimeDelta::FromMinutes(3);

  // Check that UpdateCanStart returns false and a new backoff expiry is
  // generated.
  UpdateDownloadParams result;
  ExpectPolicyStatus(EvalStatus::kAskMeAgainLater,
                     &Policy::UpdateCanStart,
                     &result,
                     update_state);
  EXPECT_FALSE(result.update_can_start);
  EXPECT_EQ(UpdateCannotStartReason::kBackoff, result.cannot_start_reason);
  EXPECT_FALSE(result.do_increment_failures);
  EXPECT_LT(curr_time, result.backoff_expiry);
}

TEST_F(UmChromeOSPolicyTest, UpdateCanStartAllowedBackoffSatisfied) {
  // The UpdateCanStart policy returns true; a previously enacted backoff period
  // has elapsed, we're good to go.

  SetUpdateCheckAllowed(false);

  const Time curr_time = fake_clock_.GetWallclockTime();
  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromSeconds(10));
  update_state.download_errors_max = 1;
  update_state.download_errors.emplace_back(
      0,
      ErrorCode::kDownloadTransferError,
      curr_time - TimeDelta::FromSeconds(8));
  update_state.download_errors.emplace_back(
      0,
      ErrorCode::kDownloadTransferError,
      curr_time - TimeDelta::FromSeconds(2));
  update_state.failures_last_updated = curr_time - TimeDelta::FromSeconds(1);
  update_state.backoff_expiry = curr_time - TimeDelta::FromSeconds(1);

  // Check that UpdateCanStart returns false and a new backoff expiry is
  // generated.
  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanStart, &result, update_state);
  EXPECT_TRUE(result.update_can_start);
  EXPECT_EQ(UpdateCannotStartReason::kUndefined, result.cannot_start_reason);
  EXPECT_EQ(0, result.download_url_idx);
  EXPECT_TRUE(result.download_url_allowed);
  EXPECT_EQ(0, result.download_url_num_errors);
  EXPECT_FALSE(result.do_increment_failures);
  EXPECT_EQ(Time(), result.backoff_expiry);
}

TEST_F(UmChromeOSPolicyTest, UpdateCanStartAllowedBackoffDisabled) {
  // The UpdateCanStart policy returns false; failures are reported but backoff
  // is disabled.

  SetUpdateCheckAllowed(false);

  const Time curr_time = fake_clock_.GetWallclockTime();
  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromSeconds(10));
  update_state.download_errors_max = 1;
  update_state.download_errors.emplace_back(
      0,
      ErrorCode::kDownloadTransferError,
      curr_time - TimeDelta::FromSeconds(8));
  update_state.download_errors.emplace_back(
      0,
      ErrorCode::kDownloadTransferError,
      curr_time - TimeDelta::FromSeconds(2));
  update_state.is_backoff_disabled = true;

  // Check that UpdateCanStart returns false and a new backoff expiry is
  // generated.
  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanStart, &result, update_state);
  EXPECT_TRUE(result.update_can_start);
  EXPECT_EQ(UpdateCannotStartReason::kUndefined, result.cannot_start_reason);
  EXPECT_EQ(0, result.download_url_idx);
  EXPECT_TRUE(result.download_url_allowed);
  EXPECT_EQ(0, result.download_url_num_errors);
  EXPECT_TRUE(result.do_increment_failures);
  EXPECT_EQ(Time(), result.backoff_expiry);
}

TEST_F(UmChromeOSPolicyTest, UpdateCanStartAllowedNoBackoffInteractive) {
  // The UpdateCanStart policy returns false; failures are reported but this is
  // an interactive update check.

  SetUpdateCheckAllowed(false);

  const Time curr_time = fake_clock_.GetWallclockTime();
  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromSeconds(10));
  update_state.download_errors_max = 1;
  update_state.download_errors.emplace_back(
      0,
      ErrorCode::kDownloadTransferError,
      curr_time - TimeDelta::FromSeconds(8));
  update_state.download_errors.emplace_back(
      0,
      ErrorCode::kDownloadTransferError,
      curr_time - TimeDelta::FromSeconds(2));
  update_state.interactive = true;

  // Check that UpdateCanStart returns false and a new backoff expiry is
  // generated.
  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanStart, &result, update_state);
  EXPECT_TRUE(result.update_can_start);
  EXPECT_EQ(UpdateCannotStartReason::kUndefined, result.cannot_start_reason);
  EXPECT_EQ(0, result.download_url_idx);
  EXPECT_TRUE(result.download_url_allowed);
  EXPECT_EQ(0, result.download_url_num_errors);
  EXPECT_TRUE(result.do_increment_failures);
  EXPECT_EQ(Time(), result.backoff_expiry);
}

TEST_F(UmChromeOSPolicyTest, UpdateCanStartAllowedNoBackoffDelta) {
  // The UpdateCanStart policy returns false; failures are reported but this is
  // a delta payload.

  SetUpdateCheckAllowed(false);

  const Time curr_time = fake_clock_.GetWallclockTime();
  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromSeconds(10));
  update_state.download_errors_max = 1;
  update_state.download_errors.emplace_back(
      0,
      ErrorCode::kDownloadTransferError,
      curr_time - TimeDelta::FromSeconds(8));
  update_state.download_errors.emplace_back(
      0,
      ErrorCode::kDownloadTransferError,
      curr_time - TimeDelta::FromSeconds(2));
  update_state.is_delta_payload = true;

  // Check that UpdateCanStart returns false and a new backoff expiry is
  // generated.
  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanStart, &result, update_state);
  EXPECT_TRUE(result.update_can_start);
  EXPECT_EQ(UpdateCannotStartReason::kUndefined, result.cannot_start_reason);
  EXPECT_EQ(0, result.download_url_idx);
  EXPECT_TRUE(result.download_url_allowed);
  EXPECT_EQ(0, result.download_url_num_errors);
  EXPECT_TRUE(result.do_increment_failures);
  EXPECT_EQ(Time(), result.backoff_expiry);
}

TEST_F(UmChromeOSPolicyTest, UpdateCanStartAllowedNoBackoffUnofficialBuild) {
  // The UpdateCanStart policy returns false; failures are reported but this is
  // an unofficial build.

  SetUpdateCheckAllowed(false);

  const Time curr_time = fake_clock_.GetWallclockTime();
  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromSeconds(10));
  update_state.download_errors_max = 1;
  update_state.download_errors.emplace_back(
      0,
      ErrorCode::kDownloadTransferError,
      curr_time - TimeDelta::FromSeconds(8));
  update_state.download_errors.emplace_back(
      0,
      ErrorCode::kDownloadTransferError,
      curr_time - TimeDelta::FromSeconds(2));

  fake_state_.system_provider()->var_is_official_build()->reset(
      new bool(false));

  // Check that UpdateCanStart returns false and a new backoff expiry is
  // generated.
  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanStart, &result, update_state);
  EXPECT_TRUE(result.update_can_start);
  EXPECT_EQ(UpdateCannotStartReason::kUndefined, result.cannot_start_reason);
  EXPECT_EQ(0, result.download_url_idx);
  EXPECT_TRUE(result.download_url_allowed);
  EXPECT_EQ(0, result.download_url_num_errors);
  EXPECT_TRUE(result.do_increment_failures);
  EXPECT_EQ(Time(), result.backoff_expiry);
}

TEST_F(UmChromeOSPolicyTest, UpdateCanStartFailsScatteringFailed) {
  // The UpdateCanStart policy fails because the UpdateScattering policy it
  // depends on fails (unset variable).

  SetUpdateCheckAllowed(false);

  // Override the default seed variable with a null value so that the policy
  // request would fail.
  // TODO(garnold) This failure may or may not fail a number
  // sub-policies/decisions, like scattering and backoff. We'll need a more
  // deliberate setup to ensure that we're failing what we want to be failing.
  fake_state_.random_provider()->var_seed()->reset(nullptr);

  // Check that the UpdateCanStart fails.
  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromSeconds(1));
  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kFailed, &Policy::UpdateCanStart, &result, update_state);
}

TEST_F(UmChromeOSPolicyTest,
       UpdateCanStartNotAllowedScatteringNewWaitPeriodApplies) {
  // The UpdateCanStart policy returns false; device policy is loaded and
  // scattering applies due to an unsatisfied wait period, which was newly
  // generated.

  SetUpdateCheckAllowed(false);
  fake_state_.device_policy_provider()->var_scatter_factor()->reset(
      new TimeDelta(TimeDelta::FromMinutes(2)));

  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromSeconds(1));

  // Check that the UpdateCanStart returns false and a new wait period
  // generated.
  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanStart, &result, update_state);
  EXPECT_FALSE(result.update_can_start);
  EXPECT_EQ(UpdateCannotStartReason::kScattering, result.cannot_start_reason);
  EXPECT_LT(TimeDelta(), result.scatter_wait_period);
  EXPECT_EQ(0, result.scatter_check_threshold);
}

TEST_F(UmChromeOSPolicyTest,
       UpdateCanStartNotAllowedScatteringPrevWaitPeriodStillApplies) {
  // The UpdateCanStart policy returns false w/ kAskMeAgainLater; device policy
  // is loaded and a previously generated scattering period still applies, none
  // of the scattering values has changed.

  SetUpdateCheckAllowed(false);
  fake_state_.device_policy_provider()->var_scatter_factor()->reset(
      new TimeDelta(TimeDelta::FromMinutes(2)));

  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromSeconds(1));
  update_state.scatter_wait_period = TimeDelta::FromSeconds(35);

  // Check that the UpdateCanStart returns false and a new wait period
  // generated.
  UpdateDownloadParams result;
  ExpectPolicyStatus(EvalStatus::kAskMeAgainLater,
                     &Policy::UpdateCanStart,
                     &result,
                     update_state);
  EXPECT_FALSE(result.update_can_start);
  EXPECT_EQ(UpdateCannotStartReason::kScattering, result.cannot_start_reason);
  EXPECT_EQ(TimeDelta::FromSeconds(35), result.scatter_wait_period);
  EXPECT_EQ(0, result.scatter_check_threshold);
}

TEST_F(UmChromeOSPolicyTest,
       UpdateCanStartNotAllowedScatteringNewCountThresholdApplies) {
  // The UpdateCanStart policy returns false; device policy is loaded and
  // scattering applies due to an unsatisfied update check count threshold.
  //
  // This ensures a non-zero check threshold, which may or may not be combined
  // with a non-zero wait period (for which we cannot reliably control).

  SetUpdateCheckAllowed(false);
  fake_state_.device_policy_provider()->var_scatter_factor()->reset(
      new TimeDelta(TimeDelta::FromSeconds(1)));

  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromSeconds(1));
  update_state.scatter_check_threshold_min = 2;
  update_state.scatter_check_threshold_max = 5;

  // Check that the UpdateCanStart returns false.
  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanStart, &result, update_state);
  EXPECT_FALSE(result.update_can_start);
  EXPECT_EQ(UpdateCannotStartReason::kScattering, result.cannot_start_reason);
  EXPECT_LE(2, result.scatter_check_threshold);
  EXPECT_GE(5, result.scatter_check_threshold);
}

TEST_F(UmChromeOSPolicyTest,
       UpdateCanStartNotAllowedScatteringPrevCountThresholdStillApplies) {
  // The UpdateCanStart policy returns false; device policy is loaded and
  // scattering due to a previously generated count threshold still applies.

  SetUpdateCheckAllowed(false);
  fake_state_.device_policy_provider()->var_scatter_factor()->reset(
      new TimeDelta(TimeDelta::FromSeconds(1)));

  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromSeconds(1));
  update_state.scatter_check_threshold = 3;
  update_state.scatter_check_threshold_min = 2;
  update_state.scatter_check_threshold_max = 5;

  // Check that the UpdateCanStart returns false.
  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanStart, &result, update_state);
  EXPECT_FALSE(result.update_can_start);
  EXPECT_EQ(UpdateCannotStartReason::kScattering, result.cannot_start_reason);
  EXPECT_EQ(3, result.scatter_check_threshold);
}

TEST_F(UmChromeOSPolicyTest, UpdateCanStartAllowedScatteringSatisfied) {
  // The UpdateCanStart policy returns true; device policy is loaded and
  // scattering is enabled, but both wait period and check threshold are
  // satisfied.

  SetUpdateCheckAllowed(false);
  fake_state_.device_policy_provider()->var_scatter_factor()->reset(
      new TimeDelta(TimeDelta::FromSeconds(120)));

  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromSeconds(75));
  update_state.num_checks = 4;
  update_state.scatter_wait_period = TimeDelta::FromSeconds(60);
  update_state.scatter_check_threshold = 3;
  update_state.scatter_check_threshold_min = 2;
  update_state.scatter_check_threshold_max = 5;

  // Check that the UpdateCanStart returns true.
  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanStart, &result, update_state);
  EXPECT_TRUE(result.update_can_start);
  EXPECT_EQ(TimeDelta(), result.scatter_wait_period);
  EXPECT_EQ(0, result.scatter_check_threshold);
  EXPECT_EQ(0, result.download_url_idx);
  EXPECT_TRUE(result.download_url_allowed);
  EXPECT_EQ(0, result.download_url_num_errors);
  EXPECT_FALSE(result.do_increment_failures);
}

TEST_F(UmChromeOSPolicyTest,
       UpdateCanStartAllowedInteractivePreventsScattering) {
  // The UpdateCanStart policy returns true; device policy is loaded and
  // scattering would have applied, except that the update check is interactive
  // and so it is suppressed.

  SetUpdateCheckAllowed(false);
  fake_state_.device_policy_provider()->var_scatter_factor()->reset(
      new TimeDelta(TimeDelta::FromSeconds(1)));

  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromSeconds(1));
  update_state.interactive = true;
  update_state.scatter_check_threshold = 0;
  update_state.scatter_check_threshold_min = 2;
  update_state.scatter_check_threshold_max = 5;

  // Check that the UpdateCanStart returns true.
  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanStart, &result, update_state);
  EXPECT_TRUE(result.update_can_start);
  EXPECT_EQ(TimeDelta(), result.scatter_wait_period);
  EXPECT_EQ(0, result.scatter_check_threshold);
  EXPECT_EQ(0, result.download_url_idx);
  EXPECT_TRUE(result.download_url_allowed);
  EXPECT_EQ(0, result.download_url_num_errors);
  EXPECT_FALSE(result.do_increment_failures);
}

TEST_F(UmChromeOSPolicyTest, UpdateCanStartAllowedOobePreventsScattering) {
  // The UpdateCanStart policy returns true; device policy is loaded and
  // scattering would have applied, except that OOBE was not completed and so it
  // is suppressed.

  SetUpdateCheckAllowed(false);
  fake_state_.device_policy_provider()->var_scatter_factor()->reset(
      new TimeDelta(TimeDelta::FromSeconds(1)));
  fake_state_.system_provider()->var_is_oobe_complete()->reset(new bool(false));

  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromSeconds(1));
  update_state.interactive = true;
  update_state.scatter_check_threshold = 0;
  update_state.scatter_check_threshold_min = 2;
  update_state.scatter_check_threshold_max = 5;

  // Check that the UpdateCanStart returns true.
  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanStart, &result, update_state);
  EXPECT_TRUE(result.update_can_start);
  EXPECT_EQ(TimeDelta(), result.scatter_wait_period);
  EXPECT_EQ(0, result.scatter_check_threshold);
  EXPECT_EQ(0, result.download_url_idx);
  EXPECT_TRUE(result.download_url_allowed);
  EXPECT_EQ(0, result.download_url_num_errors);
  EXPECT_FALSE(result.do_increment_failures);
}

TEST_F(UmChromeOSPolicyTest, UpdateCanStartAllowedWithAttributes) {
  // The UpdateCanStart policy returns true; device policy permits both HTTP and
  // P2P updates, as well as a non-empty target channel string.

  SetUpdateCheckAllowed(false);

  // Override specific device policy attributes.
  fake_state_.device_policy_provider()->var_http_downloads_enabled()->reset(
      new bool(true));
  fake_state_.device_policy_provider()->var_au_p2p_enabled()->reset(
      new bool(true));

  // Check that the UpdateCanStart returns true.
  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromMinutes(10));
  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanStart, &result, update_state);
  EXPECT_TRUE(result.update_can_start);
  EXPECT_TRUE(result.p2p_downloading_allowed);
  EXPECT_TRUE(result.p2p_sharing_allowed);
  EXPECT_EQ(0, result.download_url_idx);
  EXPECT_TRUE(result.download_url_allowed);
  EXPECT_EQ(0, result.download_url_num_errors);
  EXPECT_FALSE(result.do_increment_failures);
}

TEST_F(UmChromeOSPolicyTest, UpdateCanStartAllowedWithP2PFromUpdater) {
  // The UpdateCanStart policy returns true; device policy forbids both HTTP and
  // P2P updates, but the updater is configured to allow P2P and overrules the
  // setting.

  SetUpdateCheckAllowed(false);

  // Override specific device policy attributes.
  fake_state_.updater_provider()->var_p2p_enabled()->reset(new bool(true));

  // Check that the UpdateCanStart returns true.
  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromMinutes(10));
  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanStart, &result, update_state);
  EXPECT_TRUE(result.update_can_start);
  EXPECT_TRUE(result.p2p_downloading_allowed);
  EXPECT_TRUE(result.p2p_sharing_allowed);
  EXPECT_EQ(0, result.download_url_idx);
  EXPECT_TRUE(result.download_url_allowed);
  EXPECT_EQ(0, result.download_url_num_errors);
  EXPECT_FALSE(result.do_increment_failures);
}

TEST_F(UmChromeOSPolicyTest,
       UpdateCanStartAllowedP2PDownloadingBlockedDueToOmaha) {
  // The UpdateCanStart policy returns true; device policy permits HTTP, but
  // policy blocks P2P downloading because Omaha forbids it.  P2P sharing is
  // still permitted.

  SetUpdateCheckAllowed(false);

  // Override specific device policy attributes.
  fake_state_.device_policy_provider()->var_http_downloads_enabled()->reset(
      new bool(true));
  fake_state_.device_policy_provider()->var_au_p2p_enabled()->reset(
      new bool(true));

  // Check that the UpdateCanStart returns true.
  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromMinutes(10));
  update_state.p2p_downloading_disabled = true;
  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanStart, &result, update_state);
  EXPECT_TRUE(result.update_can_start);
  EXPECT_FALSE(result.p2p_downloading_allowed);
  EXPECT_TRUE(result.p2p_sharing_allowed);
}

TEST_F(UmChromeOSPolicyTest, UpdateCanStartAllowedP2PSharingBlockedDueToOmaha) {
  // The UpdateCanStart policy returns true; device policy permits HTTP, but
  // policy blocks P2P sharing because Omaha forbids it.  P2P downloading is
  // still permitted.

  SetUpdateCheckAllowed(false);

  // Override specific device policy attributes.
  fake_state_.device_policy_provider()->var_http_downloads_enabled()->reset(
      new bool(true));
  fake_state_.device_policy_provider()->var_au_p2p_enabled()->reset(
      new bool(true));

  // Check that the UpdateCanStart returns true.
  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromMinutes(10));
  update_state.p2p_sharing_disabled = true;
  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanStart, &result, update_state);
  EXPECT_TRUE(result.update_can_start);
  EXPECT_TRUE(result.p2p_downloading_allowed);
  EXPECT_FALSE(result.p2p_sharing_allowed);
}

TEST_F(UmChromeOSPolicyTest,
       UpdateCanStartAllowedP2PDownloadingBlockedDueToNumAttempts) {
  // The UpdateCanStart policy returns true; device policy permits HTTP but
  // blocks P2P download, because the max number of P2P downloads have been
  // attempted. P2P sharing is still permitted.

  SetUpdateCheckAllowed(false);

  // Override specific device policy attributes.
  fake_state_.device_policy_provider()->var_http_downloads_enabled()->reset(
      new bool(true));
  fake_state_.device_policy_provider()->var_au_p2p_enabled()->reset(
      new bool(true));

  // Check that the UpdateCanStart returns true.
  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromMinutes(10));
  update_state.p2p_num_attempts = ChromeOSPolicy::kMaxP2PAttempts;
  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanStart, &result, update_state);
  EXPECT_TRUE(result.update_can_start);
  EXPECT_FALSE(result.p2p_downloading_allowed);
  EXPECT_TRUE(result.p2p_sharing_allowed);
}

TEST_F(UmChromeOSPolicyTest,
       UpdateCanStartAllowedP2PDownloadingBlockedDueToAttemptsPeriod) {
  // The UpdateCanStart policy returns true; device policy permits HTTP but
  // blocks P2P download, because the max period for attempt to download via P2P
  // has elapsed. P2P sharing is still permitted.

  SetUpdateCheckAllowed(false);

  // Override specific device policy attributes.
  fake_state_.device_policy_provider()->var_http_downloads_enabled()->reset(
      new bool(true));
  fake_state_.device_policy_provider()->var_au_p2p_enabled()->reset(
      new bool(true));

  // Check that the UpdateCanStart returns true.
  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromMinutes(10));
  update_state.p2p_num_attempts = 1;
  update_state.p2p_first_attempted =
      fake_clock_.GetWallclockTime() -
      TimeDelta::FromSeconds(ChromeOSPolicy::kMaxP2PAttemptsPeriodInSeconds +
                             1);
  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanStart, &result, update_state);
  EXPECT_TRUE(result.update_can_start);
  EXPECT_FALSE(result.p2p_downloading_allowed);
  EXPECT_TRUE(result.p2p_sharing_allowed);
}

TEST_F(UmChromeOSPolicyTest,
       UpdateCanStartAllowedWithHttpUrlForUnofficialBuild) {
  // The UpdateCanStart policy returns true; device policy forbids both HTTP and
  // P2P updates, but marking this an unofficial build overrules the HTTP
  // setting.

  SetUpdateCheckAllowed(false);

  // Override specific device policy attributes.
  fake_state_.device_policy_provider()->var_http_downloads_enabled()->reset(
      new bool(false));
  fake_state_.system_provider()->var_is_official_build()->reset(
      new bool(false));

  // Check that the UpdateCanStart returns true.
  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromMinutes(10));
  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanStart, &result, update_state);
  EXPECT_TRUE(result.update_can_start);
  EXPECT_EQ(0, result.download_url_idx);
  EXPECT_TRUE(result.download_url_allowed);
  EXPECT_EQ(0, result.download_url_num_errors);
  EXPECT_FALSE(result.do_increment_failures);
}

TEST_F(UmChromeOSPolicyTest, UpdateCanStartAllowedWithHttpsUrl) {
  // The UpdateCanStart policy returns true; device policy forbids both HTTP and
  // P2P updates, but an HTTPS URL is provided and selected for download.

  SetUpdateCheckAllowed(false);

  // Override specific device policy attributes.
  fake_state_.device_policy_provider()->var_http_downloads_enabled()->reset(
      new bool(false));

  // Add an HTTPS URL.
  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromMinutes(10));
  update_state.download_urls.emplace_back("https://secure/url/");

  // Check that the UpdateCanStart returns true.
  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanStart, &result, update_state);
  EXPECT_TRUE(result.update_can_start);
  EXPECT_EQ(1, result.download_url_idx);
  EXPECT_TRUE(result.download_url_allowed);
  EXPECT_EQ(0, result.download_url_num_errors);
  EXPECT_FALSE(result.do_increment_failures);
}

TEST_F(UmChromeOSPolicyTest, UpdateCanStartAllowedMaxErrorsNotExceeded) {
  // The UpdateCanStart policy returns true; the first URL has download errors
  // but does not exceed the maximum allowed number of failures, so it is stilli
  // usable.

  SetUpdateCheckAllowed(false);

  // Add a second URL; update with this URL attempted and failed enough times to
  // disqualify the current (first) URL.
  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromMinutes(10));
  update_state.num_checks = 5;
  update_state.download_urls.emplace_back("http://another/fake/url/");
  Time t = fake_clock_.GetWallclockTime() - TimeDelta::FromSeconds(12);
  for (int i = 0; i < 5; i++) {
    update_state.download_errors.emplace_back(
        0, ErrorCode::kDownloadTransferError, t);
    t += TimeDelta::FromSeconds(1);
  }

  // Check that the UpdateCanStart returns true.
  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanStart, &result, update_state);
  EXPECT_TRUE(result.update_can_start);
  EXPECT_EQ(0, result.download_url_idx);
  EXPECT_TRUE(result.download_url_allowed);
  EXPECT_EQ(5, result.download_url_num_errors);
  EXPECT_FALSE(result.do_increment_failures);
}

TEST_F(UmChromeOSPolicyTest, UpdateCanStartAllowedWithSecondUrlMaxExceeded) {
  // The UpdateCanStart policy returns true; the first URL exceeded the maximum
  // allowed number of failures, but a second URL is available.

  SetUpdateCheckAllowed(false);

  // Add a second URL; update with this URL attempted and failed enough times to
  // disqualify the current (first) URL.
  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromMinutes(10));
  update_state.num_checks = 10;
  update_state.download_urls.emplace_back("http://another/fake/url/");
  Time t = fake_clock_.GetWallclockTime() - TimeDelta::FromSeconds(12);
  for (int i = 0; i < 11; i++) {
    update_state.download_errors.emplace_back(
        0, ErrorCode::kDownloadTransferError, t);
    t += TimeDelta::FromSeconds(1);
  }

  // Check that the UpdateCanStart returns true.
  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanStart, &result, update_state);
  EXPECT_TRUE(result.update_can_start);
  EXPECT_EQ(1, result.download_url_idx);
  EXPECT_TRUE(result.download_url_allowed);
  EXPECT_EQ(0, result.download_url_num_errors);
  EXPECT_FALSE(result.do_increment_failures);
}

TEST_F(UmChromeOSPolicyTest, UpdateCanStartAllowedWithSecondUrlHardError) {
  // The UpdateCanStart policy returns true; the first URL fails with a hard
  // error, but a second URL is available.

  SetUpdateCheckAllowed(false);

  // Add a second URL; update with this URL attempted and failed in a way that
  // causes it to switch directly to the next URL.
  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromMinutes(10));
  update_state.num_checks = 10;
  update_state.download_urls.emplace_back("http://another/fake/url/");
  update_state.download_errors.emplace_back(
      0,
      ErrorCode::kPayloadHashMismatchError,
      fake_clock_.GetWallclockTime() - TimeDelta::FromSeconds(1));

  // Check that the UpdateCanStart returns true.
  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanStart, &result, update_state);
  EXPECT_TRUE(result.update_can_start);
  EXPECT_EQ(1, result.download_url_idx);
  EXPECT_TRUE(result.download_url_allowed);
  EXPECT_EQ(0, result.download_url_num_errors);
  EXPECT_FALSE(result.do_increment_failures);
}

TEST_F(UmChromeOSPolicyTest, UpdateCanStartAllowedUrlWrapsAround) {
  // The UpdateCanStart policy returns true; URL search properly wraps around
  // the last one on the list.

  SetUpdateCheckAllowed(false);

  // Add a second URL; update with this URL attempted and failed in a way that
  // causes it to switch directly to the next URL. We must disable backoff in
  // order for it not to interfere.
  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromMinutes(10));
  update_state.num_checks = 1;
  update_state.is_backoff_disabled = true;
  update_state.download_urls.emplace_back("http://another/fake/url/");
  update_state.download_errors.emplace_back(
      1,
      ErrorCode::kPayloadHashMismatchError,
      fake_clock_.GetWallclockTime() - TimeDelta::FromSeconds(1));

  // Check that the UpdateCanStart returns true.
  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanStart, &result, update_state);
  EXPECT_TRUE(result.update_can_start);
  EXPECT_EQ(0, result.download_url_idx);
  EXPECT_TRUE(result.download_url_allowed);
  EXPECT_EQ(0, result.download_url_num_errors);
  EXPECT_TRUE(result.do_increment_failures);
}

TEST_F(UmChromeOSPolicyTest, UpdateCanStartNotAllowedNoUsableUrls) {
  // The UpdateCanStart policy returns false; there's a single HTTP URL but its
  // use is forbidden by policy.
  //
  // Note: In the case where no usable URLs are found, the policy should not
  // increment the number of failed attempts! Doing so would result in a
  // non-idempotent semantics, and does not fall within the intended purpose of
  // the backoff mechanism anyway.

  SetUpdateCheckAllowed(false);

  // Override specific device policy attributes.
  fake_state_.device_policy_provider()->var_http_downloads_enabled()->reset(
      new bool(false));

  // Check that the UpdateCanStart returns false.
  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromMinutes(10));
  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanStart, &result, update_state);
  EXPECT_FALSE(result.update_can_start);
  EXPECT_EQ(UpdateCannotStartReason::kCannotDownload,
            result.cannot_start_reason);
  EXPECT_FALSE(result.do_increment_failures);
}

TEST_F(UmChromeOSPolicyTest, UpdateCanStartAllowedNoUsableUrlsButP2PEnabled) {
  // The UpdateCanStart policy returns true; there's a single HTTP URL but its
  // use is forbidden by policy, however P2P is enabled. The result indicates
  // that no URL can be used.
  //
  // Note: The number of failed attempts should not increase in this case (see
  // above test).

  SetUpdateCheckAllowed(false);

  // Override specific device policy attributes.
  fake_state_.device_policy_provider()->var_au_p2p_enabled()->reset(
      new bool(true));
  fake_state_.device_policy_provider()->var_http_downloads_enabled()->reset(
      new bool(false));

  // Check that the UpdateCanStart returns true.
  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromMinutes(10));
  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanStart, &result, update_state);
  EXPECT_TRUE(result.update_can_start);
  EXPECT_TRUE(result.p2p_downloading_allowed);
  EXPECT_TRUE(result.p2p_sharing_allowed);
  EXPECT_GT(0, result.download_url_idx);
  EXPECT_TRUE(result.download_url_allowed);
  EXPECT_EQ(0, result.download_url_num_errors);
  EXPECT_FALSE(result.do_increment_failures);
}

TEST_F(UmChromeOSPolicyTest,
       UpdateCanStartAllowedNoUsableUrlsButEnterpriseEnrolled) {
  // The UpdateCanStart policy returns true; there's a single HTTP URL but its
  // use is forbidden by policy, and P2P is unset on the policy, however the
  // device is enterprise-enrolled so P2P is allowed. The result indicates that
  // no URL can be used.
  //
  // Note: The number of failed attempts should not increase in this case (see
  // above test).

  SetUpdateCheckAllowed(false);

  // Override specific device policy attributes.
  fake_state_.device_policy_provider()->var_au_p2p_enabled()->reset(nullptr);
  fake_state_.device_policy_provider()->var_owner()->reset(nullptr);
  fake_state_.device_policy_provider()->var_http_downloads_enabled()->reset(
      new bool(false));

  // Check that the UpdateCanStart returns true.
  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromMinutes(10));
  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanStart, &result, update_state);
  EXPECT_TRUE(result.update_can_start);
  EXPECT_TRUE(result.p2p_downloading_allowed);
  EXPECT_TRUE(result.p2p_sharing_allowed);
  EXPECT_GT(0, result.download_url_idx);
  EXPECT_TRUE(result.download_url_allowed);
  EXPECT_EQ(0, result.download_url_num_errors);
  EXPECT_FALSE(result.do_increment_failures);
}

TEST_F(UmChromeOSPolicyTest, UpdateDownloadAllowedEthernetDefault) {
  // Ethernet is always allowed.

  fake_state_.shill_provider()->var_conn_type()->reset(
      new ConnectionType(ConnectionType::kEthernet));

  bool result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateDownloadAllowed, &result);
  EXPECT_TRUE(result);
}

TEST_F(UmChromeOSPolicyTest, UpdateDownloadAllowedWifiDefault) {
  // Wifi is allowed if not tethered.

  fake_state_.shill_provider()->var_conn_type()->reset(
      new ConnectionType(ConnectionType::kWifi));

  bool result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateDownloadAllowed, &result);
  EXPECT_TRUE(result);
}

TEST_F(UmChromeOSPolicyTest,
       UpdateCurrentConnectionNotAllowedWifiTetheredDefault) {
  // Tethered wifi is not allowed by default.

  fake_state_.shill_provider()->var_conn_type()->reset(
      new ConnectionType(ConnectionType::kWifi));
  fake_state_.shill_provider()->var_conn_tethering()->reset(
      new ConnectionTethering(ConnectionTethering::kConfirmed));

  bool result;
  ExpectPolicyStatus(
      EvalStatus::kAskMeAgainLater, &Policy::UpdateDownloadAllowed, &result);
}

TEST_F(UmChromeOSPolicyTest, UpdateDownloadAllowedWifiTetheredPolicyOverride) {
  // Tethered wifi can be allowed by policy.

  fake_state_.shill_provider()->var_conn_type()->reset(
      new ConnectionType(ConnectionType::kWifi));
  fake_state_.shill_provider()->var_conn_tethering()->reset(
      new ConnectionTethering(ConnectionTethering::kConfirmed));
  set<ConnectionType> allowed_connections;
  allowed_connections.insert(ConnectionType::kCellular);
  fake_state_.device_policy_provider()
      ->var_allowed_connection_types_for_update()
      ->reset(new set<ConnectionType>(allowed_connections));

  bool result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateDownloadAllowed, &result);
  EXPECT_TRUE(result);
}

TEST_F(UmChromeOSPolicyTest, UpdateDownloadAllowedWimaxDefault) {
  // Wimax is always allowed.

  fake_state_.shill_provider()->var_conn_type()->reset(
      new ConnectionType(ConnectionType::kWifi));

  bool result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateDownloadAllowed, &result);
  EXPECT_TRUE(result);
}

TEST_F(UmChromeOSPolicyTest,
       UpdateCurrentConnectionNotAllowedBluetoothDefault) {
  // Bluetooth is never allowed.

  fake_state_.shill_provider()->var_conn_type()->reset(
      new ConnectionType(ConnectionType::kBluetooth));

  bool result;
  ExpectPolicyStatus(
      EvalStatus::kAskMeAgainLater, &Policy::UpdateDownloadAllowed, &result);
}

TEST_F(UmChromeOSPolicyTest,
       UpdateCurrentConnectionNotAllowedBluetoothPolicyCannotOverride) {
  // Bluetooth cannot be allowed even by policy.

  fake_state_.shill_provider()->var_conn_type()->reset(
      new ConnectionType(ConnectionType::kBluetooth));
  set<ConnectionType> allowed_connections;
  allowed_connections.insert(ConnectionType::kBluetooth);
  fake_state_.device_policy_provider()
      ->var_allowed_connection_types_for_update()
      ->reset(new set<ConnectionType>(allowed_connections));

  bool result;
  ExpectPolicyStatus(
      EvalStatus::kAskMeAgainLater, &Policy::UpdateDownloadAllowed, &result);
}

TEST_F(UmChromeOSPolicyTest, UpdateCurrentConnectionNotAllowedCellularDefault) {
  // Cellular is not allowed by default.

  fake_state_.shill_provider()->var_conn_type()->reset(
      new ConnectionType(ConnectionType::kCellular));

  bool result;
  ExpectPolicyStatus(
      EvalStatus::kAskMeAgainLater, &Policy::UpdateDownloadAllowed, &result);
}

TEST_F(UmChromeOSPolicyTest, UpdateDownloadAllowedCellularPolicyOverride) {
  // Update over cellular can be enabled by policy.

  fake_state_.shill_provider()->var_conn_type()->reset(
      new ConnectionType(ConnectionType::kCellular));
  set<ConnectionType> allowed_connections;
  allowed_connections.insert(ConnectionType::kCellular);
  fake_state_.device_policy_provider()
      ->var_allowed_connection_types_for_update()
      ->reset(new set<ConnectionType>(allowed_connections));

  bool result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateDownloadAllowed, &result);
  EXPECT_TRUE(result);
}

TEST_F(UmChromeOSPolicyTest, UpdateDownloadAllowedCellularUserOverride) {
  // Update over cellular can be enabled by user settings, but only if policy
  // is present and does not determine allowed connections.

  fake_state_.shill_provider()->var_conn_type()->reset(
      new ConnectionType(ConnectionType::kCellular));
  set<ConnectionType> allowed_connections;
  allowed_connections.insert(ConnectionType::kCellular);
  fake_state_.updater_provider()->var_cellular_enabled()->reset(new bool(true));

  bool result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateDownloadAllowed, &result);
  EXPECT_TRUE(result);
}

TEST_F(UmChromeOSPolicyTest, UpdateCanStartAllowedScatteringSupressedDueToP2P) {
  // The UpdateCanStart policy returns true; scattering should have applied, but
  // P2P download is allowed. Scattering values are nonetheless returned, and so
  // are download URL values, albeit the latter are not allowed to be used.

  SetUpdateCheckAllowed(false);
  fake_state_.device_policy_provider()->var_scatter_factor()->reset(
      new TimeDelta(TimeDelta::FromMinutes(2)));
  fake_state_.updater_provider()->var_p2p_enabled()->reset(new bool(true));

  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromSeconds(1));
  update_state.scatter_wait_period = TimeDelta::FromSeconds(35);

  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanStart, &result, update_state);
  EXPECT_TRUE(result.update_can_start);
  EXPECT_EQ(0, result.download_url_idx);
  EXPECT_FALSE(result.download_url_allowed);
  EXPECT_EQ(0, result.download_url_num_errors);
  EXPECT_TRUE(result.p2p_downloading_allowed);
  EXPECT_TRUE(result.p2p_sharing_allowed);
  EXPECT_FALSE(result.do_increment_failures);
  EXPECT_EQ(TimeDelta::FromSeconds(35), result.scatter_wait_period);
  EXPECT_EQ(0, result.scatter_check_threshold);
}

TEST_F(UmChromeOSPolicyTest, UpdateCanStartAllowedBackoffSupressedDueToP2P) {
  // The UpdateCanStart policy returns true; backoff should have applied, but
  // P2P download is allowed. Backoff values are nonetheless returned, and so
  // are download URL values, albeit the latter are not allowed to be used.

  SetUpdateCheckAllowed(false);

  const Time curr_time = fake_clock_.GetWallclockTime();
  UpdateState update_state = GetDefaultUpdateState(TimeDelta::FromSeconds(10));
  update_state.download_errors_max = 1;
  update_state.download_errors.emplace_back(
      0,
      ErrorCode::kDownloadTransferError,
      curr_time - TimeDelta::FromSeconds(8));
  update_state.download_errors.emplace_back(
      0,
      ErrorCode::kDownloadTransferError,
      curr_time - TimeDelta::FromSeconds(2));
  fake_state_.updater_provider()->var_p2p_enabled()->reset(new bool(true));

  UpdateDownloadParams result;
  ExpectPolicyStatus(
      EvalStatus::kSucceeded, &Policy::UpdateCanStart, &result, update_state);
  EXPECT_TRUE(result.update_can_start);
  EXPECT_EQ(0, result.download_url_idx);
  EXPECT_FALSE(result.download_url_allowed);
  EXPECT_EQ(0, result.download_url_num_errors);
  EXPECT_TRUE(result.p2p_downloading_allowed);
  EXPECT_TRUE(result.p2p_sharing_allowed);
  EXPECT_TRUE(result.do_increment_failures);
  EXPECT_LT(curr_time, result.backoff_expiry);
}

TEST_F(UmChromeOSPolicyTest, P2PEnabledNotAllowed) {
  bool result;
  ExpectPolicyStatus(EvalStatus::kSucceeded, &Policy::P2PEnabled, &result);
  EXPECT_FALSE(result);
}

TEST_F(UmChromeOSPolicyTest, P2PEnabledAllowedByDevicePolicy) {
  fake_state_.device_policy_provider()->var_au_p2p_enabled()->reset(
      new bool(true));

  bool result;
  ExpectPolicyStatus(EvalStatus::kSucceeded, &Policy::P2PEnabled, &result);
  EXPECT_TRUE(result);
}

TEST_F(UmChromeOSPolicyTest, P2PEnabledAllowedByUpdater) {
  fake_state_.updater_provider()->var_p2p_enabled()->reset(new bool(true));

  bool result;
  ExpectPolicyStatus(EvalStatus::kSucceeded, &Policy::P2PEnabled, &result);
  EXPECT_TRUE(result);
}

TEST_F(UmChromeOSPolicyTest, P2PEnabledAllowedDeviceEnterpriseEnrolled) {
  fake_state_.device_policy_provider()->var_au_p2p_enabled()->reset(nullptr);
  fake_state_.device_policy_provider()->var_owner()->reset(nullptr);

  bool result;
  ExpectPolicyStatus(EvalStatus::kSucceeded, &Policy::P2PEnabled, &result);
  EXPECT_TRUE(result);
}

TEST_F(UmChromeOSPolicyTest, P2PEnabledChangedBlocks) {
  bool result;
  ExpectPolicyStatus(
      EvalStatus::kAskMeAgainLater, &Policy::P2PEnabledChanged, &result, false);
}

TEST_F(UmChromeOSPolicyTest,
       UpdateCanBeAppliedForcedUpdatesDisablesTimeRestrictions) {
  Time curr_time = fake_clock_.GetWallclockTime();
  fake_state_.updater_provider()->var_forced_update_requested()->reset(
      new UpdateRequestStatus(UpdateRequestStatus::kInteractive));
  // Should return kAskMeAgainLater when updated are not forced.
  TestDisallowedTimeIntervals(
      {WeeklyTimeInterval(
          WeeklyTime::FromTime(curr_time),
          WeeklyTime::FromTime(curr_time + TimeDelta::FromMinutes(1)))},
      ErrorCode::kSuccess,
      /* kiosk = */ true);
}

TEST_F(UmChromeOSPolicyTest, UpdateCanBeAppliedFailsInDisallowedTime) {
  Time curr_time = fake_clock_.GetWallclockTime();
  TestDisallowedTimeIntervals(
      {WeeklyTimeInterval(
          WeeklyTime::FromTime(curr_time),
          WeeklyTime::FromTime(curr_time + TimeDelta::FromMinutes(1)))},
      ErrorCode::kOmahaUpdateDeferredPerPolicy,
      /* kiosk = */ true);
}

TEST_F(UmChromeOSPolicyTest, UpdateCanBeAppliedOutsideDisallowedTime) {
  Time curr_time = fake_clock_.GetWallclockTime();
  TestDisallowedTimeIntervals(
      {WeeklyTimeInterval(
          WeeklyTime::FromTime(curr_time - TimeDelta::FromHours(3)),
          WeeklyTime::FromTime(curr_time))},
      ErrorCode::kSuccess,
      /* kiosk = */ true);
}

TEST_F(UmChromeOSPolicyTest, UpdateCanBeAppliedPassesOnNonKiosk) {
  Time curr_time = fake_clock_.GetWallclockTime();
  TestDisallowedTimeIntervals(
      {WeeklyTimeInterval(
          WeeklyTime::FromTime(curr_time),
          WeeklyTime::FromTime(curr_time + TimeDelta::FromMinutes(1)))},
      ErrorCode::kSuccess,
      /* kiosk = */ false);
}

}  // namespace chromeos_update_manager
