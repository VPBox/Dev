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

#include "update_engine/update_manager/real_updater_provider.h"

#include <memory>
#include <string>

#include <base/time/time.h>
#include <gtest/gtest.h>
#include <update_engine/dbus-constants.h>

#include "update_engine/common/fake_clock.h"
#include "update_engine/common/fake_prefs.h"
#include "update_engine/fake_system_state.h"
#include "update_engine/mock_update_attempter.h"
#include "update_engine/omaha_request_params.h"
#include "update_engine/update_manager/umtest_utils.h"

using base::Time;
using base::TimeDelta;
using chromeos_update_engine::FakeClock;
using chromeos_update_engine::FakePrefs;
using chromeos_update_engine::FakeSystemState;
using chromeos_update_engine::OmahaRequestParams;
using std::string;
using std::unique_ptr;
using testing::_;
using testing::DoAll;
using testing::Return;
using testing::SetArgPointee;
using update_engine::UpdateAttemptFlags;

namespace {

// Generates a fixed timestamp for use in faking the current time.
Time FixedTime() {
  Time::Exploded now_exp;
  now_exp.year = 2014;
  now_exp.month = 3;
  now_exp.day_of_week = 2;
  now_exp.day_of_month = 18;
  now_exp.hour = 8;
  now_exp.minute = 5;
  now_exp.second = 33;
  now_exp.millisecond = 675;
  Time time;
  ignore_result(Time::FromLocalExploded(now_exp, &time));
  return time;
}

// Rounds down a timestamp to the nearest second. This is useful when faking
// times that are converted to time_t (no sub-second resolution).
Time RoundedToSecond(Time time) {
  Time::Exploded exp;
  time.LocalExplode(&exp);
  exp.millisecond = 0;
  Time rounded_time;
  ignore_result(Time::FromLocalExploded(exp, &rounded_time));
  return rounded_time;
}

ACTION_P(ActionSetUpdateEngineStatusLastCheckedTime, time) {
  arg0->last_checked_time = time;
};

ACTION_P(ActionSetUpdateEngineStatusProgress, progress) {
  arg0->progress = progress;
};

ACTION_P(ActionSetUpdateEngineStatusStatus, status) {
  arg0->status = status;
}

ACTION_P(ActionSetUpdateEngineStatusNewVersion, new_version) {
  arg0->new_version = new_version;
}

ACTION_P(ActionSetUpdateEngineStatusNewSizeBytes, new_size_bytes) {
  arg0->new_size_bytes = new_size_bytes;
}

}  // namespace

namespace chromeos_update_manager {

class UmRealUpdaterProviderTest : public ::testing::Test {
 protected:
  void SetUp() override {
    fake_clock_ = fake_sys_state_.fake_clock();
    fake_sys_state_.set_prefs(&fake_prefs_);
    provider_.reset(new RealUpdaterProvider(&fake_sys_state_));
    ASSERT_NE(nullptr, provider_.get());
    // Check that provider initializes correctly.
    ASSERT_TRUE(provider_->Init());
  }

  // Sets up mock expectations for testing the update completed time reporting.
  // |valid| determines whether the returned time is valid. Returns the expected
  // update completed time value.
  Time SetupUpdateCompletedTime(bool valid) {
    const TimeDelta kDurationSinceUpdate = TimeDelta::FromMinutes(7);
    const Time kUpdateBootTime = Time() + kDurationSinceUpdate * 2;
    const Time kCurrBootTime = (valid ? kUpdateBootTime + kDurationSinceUpdate
                                      : kUpdateBootTime - kDurationSinceUpdate);
    const Time kCurrWallclockTime = FixedTime();
    EXPECT_CALL(*fake_sys_state_.mock_update_attempter(),
                GetBootTimeAtUpdate(_))
        .WillOnce(DoAll(SetArgPointee<0>(kUpdateBootTime), Return(true)));
    fake_clock_->SetBootTime(kCurrBootTime);
    fake_clock_->SetWallclockTime(kCurrWallclockTime);
    return kCurrWallclockTime - kDurationSinceUpdate;
  }

  FakeSystemState fake_sys_state_;
  FakeClock* fake_clock_;  // Short for fake_sys_state_.fake_clock()
  FakePrefs fake_prefs_;
  unique_ptr<RealUpdaterProvider> provider_;
};

TEST_F(UmRealUpdaterProviderTest, UpdaterStartedTimeIsWallclockTime) {
  fake_clock_->SetWallclockTime(Time::FromDoubleT(123.456));
  fake_clock_->SetMonotonicTime(Time::FromDoubleT(456.123));
  // Run SetUp again to re-setup the provider under test to use these values.
  SetUp();
  UmTestUtils::ExpectVariableHasValue(Time::FromDoubleT(123.456),
                                      provider_->var_updater_started_time());
}

TEST_F(UmRealUpdaterProviderTest, GetLastCheckedTimeOkay) {
  EXPECT_CALL(*fake_sys_state_.mock_update_attempter(), GetStatus(_))
      .WillOnce(DoAll(
          ActionSetUpdateEngineStatusLastCheckedTime(FixedTime().ToTimeT()),
          Return(true)));
  UmTestUtils::ExpectVariableHasValue(RoundedToSecond(FixedTime()),
                                      provider_->var_last_checked_time());
}

TEST_F(UmRealUpdaterProviderTest, GetLastCheckedTimeFailNoValue) {
  EXPECT_CALL(*fake_sys_state_.mock_update_attempter(), GetStatus(_))
      .WillOnce(Return(false));
  UmTestUtils::ExpectVariableNotSet(provider_->var_last_checked_time());
}

TEST_F(UmRealUpdaterProviderTest, GetProgressOkayMin) {
  EXPECT_CALL(*fake_sys_state_.mock_update_attempter(), GetStatus(_))
      .WillOnce(DoAll(ActionSetUpdateEngineStatusProgress(0.0), Return(true)));
  UmTestUtils::ExpectVariableHasValue(0.0, provider_->var_progress());
}

TEST_F(UmRealUpdaterProviderTest, GetProgressOkayMid) {
  EXPECT_CALL(*fake_sys_state_.mock_update_attempter(), GetStatus(_))
      .WillOnce(DoAll(ActionSetUpdateEngineStatusProgress(0.3), Return(true)));
  UmTestUtils::ExpectVariableHasValue(0.3, provider_->var_progress());
}

TEST_F(UmRealUpdaterProviderTest, GetProgressOkayMax) {
  EXPECT_CALL(*fake_sys_state_.mock_update_attempter(), GetStatus(_))
      .WillOnce(DoAll(ActionSetUpdateEngineStatusProgress(1.0), Return(true)));
  UmTestUtils::ExpectVariableHasValue(1.0, provider_->var_progress());
}

TEST_F(UmRealUpdaterProviderTest, GetProgressFailNoValue) {
  EXPECT_CALL(*fake_sys_state_.mock_update_attempter(), GetStatus(_))
      .WillOnce(Return(false));
  UmTestUtils::ExpectVariableNotSet(provider_->var_progress());
}

TEST_F(UmRealUpdaterProviderTest, GetProgressFailTooSmall) {
  EXPECT_CALL(*fake_sys_state_.mock_update_attempter(), GetStatus(_))
      .WillOnce(DoAll(ActionSetUpdateEngineStatusProgress(-2.0), Return(true)));
  UmTestUtils::ExpectVariableNotSet(provider_->var_progress());
}

TEST_F(UmRealUpdaterProviderTest, GetProgressFailTooBig) {
  EXPECT_CALL(*fake_sys_state_.mock_update_attempter(), GetStatus(_))
      .WillOnce(DoAll(ActionSetUpdateEngineStatusProgress(2.0), Return(true)));
  UmTestUtils::ExpectVariableNotSet(provider_->var_progress());
}

TEST_F(UmRealUpdaterProviderTest, GetStageOkayIdle) {
  EXPECT_CALL(*fake_sys_state_.mock_update_attempter(), GetStatus(_))
      .WillOnce(DoAll(
          ActionSetUpdateEngineStatusStatus(update_engine::UpdateStatus::IDLE),
          Return(true)));
  UmTestUtils::ExpectVariableHasValue(Stage::kIdle, provider_->var_stage());
}

TEST_F(UmRealUpdaterProviderTest, GetStageOkayCheckingForUpdate) {
  EXPECT_CALL(*fake_sys_state_.mock_update_attempter(), GetStatus(_))
      .WillOnce(DoAll(ActionSetUpdateEngineStatusStatus(
                          update_engine::UpdateStatus::CHECKING_FOR_UPDATE),
                      Return(true)));
  UmTestUtils::ExpectVariableHasValue(Stage::kCheckingForUpdate,
                                      provider_->var_stage());
}

TEST_F(UmRealUpdaterProviderTest, GetStageOkayUpdateAvailable) {
  EXPECT_CALL(*fake_sys_state_.mock_update_attempter(), GetStatus(_))
      .WillOnce(DoAll(ActionSetUpdateEngineStatusStatus(
                          update_engine::UpdateStatus::UPDATE_AVAILABLE),
                      Return(true)));
  UmTestUtils::ExpectVariableHasValue(Stage::kUpdateAvailable,
                                      provider_->var_stage());
}

TEST_F(UmRealUpdaterProviderTest, GetStageOkayDownloading) {
  EXPECT_CALL(*fake_sys_state_.mock_update_attempter(), GetStatus(_))
      .WillOnce(DoAll(ActionSetUpdateEngineStatusStatus(
                          update_engine::UpdateStatus::DOWNLOADING),
                      Return(true)));
  UmTestUtils::ExpectVariableHasValue(Stage::kDownloading,
                                      provider_->var_stage());
}

TEST_F(UmRealUpdaterProviderTest, GetStageOkayVerifying) {
  EXPECT_CALL(*fake_sys_state_.mock_update_attempter(), GetStatus(_))
      .WillOnce(DoAll(ActionSetUpdateEngineStatusStatus(
                          update_engine::UpdateStatus::VERIFYING),
                      Return(true)));
  UmTestUtils::ExpectVariableHasValue(Stage::kVerifying,
                                      provider_->var_stage());
}

TEST_F(UmRealUpdaterProviderTest, GetStageOkayFinalizing) {
  EXPECT_CALL(*fake_sys_state_.mock_update_attempter(), GetStatus(_))
      .WillOnce(DoAll(ActionSetUpdateEngineStatusStatus(
                          update_engine::UpdateStatus::FINALIZING),
                      Return(true)));
  UmTestUtils::ExpectVariableHasValue(Stage::kFinalizing,
                                      provider_->var_stage());
}

TEST_F(UmRealUpdaterProviderTest, GetStageOkayUpdatedNeedReboot) {
  EXPECT_CALL(*fake_sys_state_.mock_update_attempter(), GetStatus(_))
      .WillOnce(DoAll(ActionSetUpdateEngineStatusStatus(
                          update_engine::UpdateStatus::UPDATED_NEED_REBOOT),
                      Return(true)));
  UmTestUtils::ExpectVariableHasValue(Stage::kUpdatedNeedReboot,
                                      provider_->var_stage());
}

TEST_F(UmRealUpdaterProviderTest, GetStageOkayReportingErrorEvent) {
  EXPECT_CALL(*fake_sys_state_.mock_update_attempter(), GetStatus(_))
      .WillOnce(DoAll(ActionSetUpdateEngineStatusStatus(
                          update_engine::UpdateStatus::REPORTING_ERROR_EVENT),
                      Return(true)));
  UmTestUtils::ExpectVariableHasValue(Stage::kReportingErrorEvent,
                                      provider_->var_stage());
}

TEST_F(UmRealUpdaterProviderTest, GetStageOkayAttemptingRollback) {
  EXPECT_CALL(*fake_sys_state_.mock_update_attempter(), GetStatus(_))
      .WillOnce(DoAll(ActionSetUpdateEngineStatusStatus(
                          update_engine::UpdateStatus::ATTEMPTING_ROLLBACK),
                      Return(true)));
  UmTestUtils::ExpectVariableHasValue(Stage::kAttemptingRollback,
                                      provider_->var_stage());
}

TEST_F(UmRealUpdaterProviderTest, GetStageFailNoValue) {
  EXPECT_CALL(*fake_sys_state_.mock_update_attempter(), GetStatus(_))
      .WillOnce(Return(false));
  UmTestUtils::ExpectVariableNotSet(provider_->var_stage());
}

TEST_F(UmRealUpdaterProviderTest, GetNewVersionOkay) {
  EXPECT_CALL(*fake_sys_state_.mock_update_attempter(), GetStatus(_))
      .WillOnce(
          DoAll(ActionSetUpdateEngineStatusNewVersion("1.2.0"), Return(true)));
  UmTestUtils::ExpectVariableHasValue(string("1.2.0"),
                                      provider_->var_new_version());
}

TEST_F(UmRealUpdaterProviderTest, GetNewVersionFailNoValue) {
  EXPECT_CALL(*fake_sys_state_.mock_update_attempter(), GetStatus(_))
      .WillOnce(Return(false));
  UmTestUtils::ExpectVariableNotSet(provider_->var_new_version());
}

TEST_F(UmRealUpdaterProviderTest, GetPayloadSizeOkayZero) {
  EXPECT_CALL(*fake_sys_state_.mock_update_attempter(), GetStatus(_))
      .WillOnce(DoAll(
          ActionSetUpdateEngineStatusNewSizeBytes(static_cast<uint64_t>(0)),
          Return(true)));
  UmTestUtils::ExpectVariableHasValue(static_cast<uint64_t>(0),
                                      provider_->var_payload_size());
}

TEST_F(UmRealUpdaterProviderTest, GetPayloadSizeOkayArbitrary) {
  EXPECT_CALL(*fake_sys_state_.mock_update_attempter(), GetStatus(_))
      .WillOnce(DoAll(ActionSetUpdateEngineStatusNewSizeBytes(
                          static_cast<uint64_t>(567890)),
                      Return(true)));
  UmTestUtils::ExpectVariableHasValue(static_cast<uint64_t>(567890),
                                      provider_->var_payload_size());
}

TEST_F(UmRealUpdaterProviderTest, GetPayloadSizeOkayTwoGigabytes) {
  EXPECT_CALL(*fake_sys_state_.mock_update_attempter(), GetStatus(_))
      .WillOnce(DoAll(ActionSetUpdateEngineStatusNewSizeBytes(
                          static_cast<uint64_t>(1) << 31),
                      Return(true)));
  UmTestUtils::ExpectVariableHasValue(static_cast<uint64_t>(1) << 31,
                                      provider_->var_payload_size());
}

TEST_F(UmRealUpdaterProviderTest, GetPayloadSizeFailNoValue) {
  EXPECT_CALL(*fake_sys_state_.mock_update_attempter(), GetStatus(_))
      .WillOnce(Return(false));
  UmTestUtils::ExpectVariableNotSet(provider_->var_payload_size());
}

TEST_F(UmRealUpdaterProviderTest, GetCurrChannelOkay) {
  const string kChannelName("foo-channel");
  OmahaRequestParams request_params(&fake_sys_state_);
  request_params.Init("", "", false);
  request_params.set_current_channel(kChannelName);
  fake_sys_state_.set_request_params(&request_params);
  UmTestUtils::ExpectVariableHasValue(kChannelName,
                                      provider_->var_curr_channel());
}

TEST_F(UmRealUpdaterProviderTest, GetCurrChannelFailEmpty) {
  OmahaRequestParams request_params(&fake_sys_state_);
  request_params.Init("", "", false);
  request_params.set_current_channel("");
  fake_sys_state_.set_request_params(&request_params);
  UmTestUtils::ExpectVariableNotSet(provider_->var_curr_channel());
}

TEST_F(UmRealUpdaterProviderTest, GetNewChannelOkay) {
  const string kChannelName("foo-channel");
  OmahaRequestParams request_params(&fake_sys_state_);
  request_params.Init("", "", false);
  request_params.set_target_channel(kChannelName);
  fake_sys_state_.set_request_params(&request_params);
  UmTestUtils::ExpectVariableHasValue(kChannelName,
                                      provider_->var_new_channel());
}

TEST_F(UmRealUpdaterProviderTest, GetNewChannelFailEmpty) {
  OmahaRequestParams request_params(&fake_sys_state_);
  request_params.Init("", "", false);
  request_params.set_target_channel("");
  fake_sys_state_.set_request_params(&request_params);
  UmTestUtils::ExpectVariableNotSet(provider_->var_new_channel());
}

TEST_F(UmRealUpdaterProviderTest, GetP2PEnabledOkayPrefDoesntExist) {
  UmTestUtils::ExpectVariableHasValue(false, provider_->var_p2p_enabled());
}

TEST_F(UmRealUpdaterProviderTest, GetP2PEnabledOkayPrefReadsFalse) {
  fake_prefs_.SetBoolean(chromeos_update_engine::kPrefsP2PEnabled, false);
  UmTestUtils::ExpectVariableHasValue(false, provider_->var_p2p_enabled());
}

TEST_F(UmRealUpdaterProviderTest, GetP2PEnabledReadWhenInitialized) {
  fake_prefs_.SetBoolean(chromeos_update_engine::kPrefsP2PEnabled, true);
  SetUp();
  UmTestUtils::ExpectVariableHasValue(true, provider_->var_p2p_enabled());
}

TEST_F(UmRealUpdaterProviderTest, GetP2PEnabledUpdated) {
  fake_prefs_.SetBoolean(chromeos_update_engine::kPrefsP2PEnabled, false);
  UmTestUtils::ExpectVariableHasValue(false, provider_->var_p2p_enabled());
  fake_prefs_.SetBoolean(chromeos_update_engine::kPrefsP2PEnabled, true);
  UmTestUtils::ExpectVariableHasValue(true, provider_->var_p2p_enabled());
  fake_prefs_.Delete(chromeos_update_engine::kPrefsP2PEnabled);
  UmTestUtils::ExpectVariableHasValue(false, provider_->var_p2p_enabled());
}

TEST_F(UmRealUpdaterProviderTest, GetCellularEnabledOkayPrefDoesntExist) {
  UmTestUtils::ExpectVariableHasValue(false, provider_->var_cellular_enabled());
}

TEST_F(UmRealUpdaterProviderTest, GetCellularEnabledOkayPrefReadsTrue) {
  fake_prefs_.SetBoolean(
      chromeos_update_engine::kPrefsUpdateOverCellularPermission, true);
  UmTestUtils::ExpectVariableHasValue(true, provider_->var_cellular_enabled());
}

TEST_F(UmRealUpdaterProviderTest, GetUpdateCompletedTimeOkay) {
  Time expected = SetupUpdateCompletedTime(true);
  UmTestUtils::ExpectVariableHasValue(expected,
                                      provider_->var_update_completed_time());
}

TEST_F(UmRealUpdaterProviderTest, GetUpdateCompletedTimeFailNoValue) {
  EXPECT_CALL(*fake_sys_state_.mock_update_attempter(), GetBootTimeAtUpdate(_))
      .WillOnce(Return(false));
  UmTestUtils::ExpectVariableNotSet(provider_->var_update_completed_time());
}

TEST_F(UmRealUpdaterProviderTest, GetUpdateCompletedTimeFailInvalidValue) {
  SetupUpdateCompletedTime(false);
  UmTestUtils::ExpectVariableNotSet(provider_->var_update_completed_time());
}

TEST_F(UmRealUpdaterProviderTest, GetConsecutiveFailedUpdateChecks) {
  const unsigned int kNumFailedChecks = 3;
  EXPECT_CALL(*fake_sys_state_.mock_update_attempter(),
              consecutive_failed_update_checks())
      .WillRepeatedly(Return(kNumFailedChecks));
  UmTestUtils::ExpectVariableHasValue(
      kNumFailedChecks, provider_->var_consecutive_failed_update_checks());
}

TEST_F(UmRealUpdaterProviderTest, GetServerDictatedPollInterval) {
  const unsigned int kPollInterval = 2 * 60 * 60;  // Two hours.
  EXPECT_CALL(*fake_sys_state_.mock_update_attempter(),
              server_dictated_poll_interval())
      .WillRepeatedly(Return(kPollInterval));
  UmTestUtils::ExpectVariableHasValue(
      kPollInterval, provider_->var_server_dictated_poll_interval());
}

TEST_F(UmRealUpdaterProviderTest, GetUpdateRestrictions) {
  EXPECT_CALL(*fake_sys_state_.mock_update_attempter(),
              GetCurrentUpdateAttemptFlags())
      .WillRepeatedly(Return(UpdateAttemptFlags::kFlagRestrictDownload |
                             UpdateAttemptFlags::kFlagNonInteractive));
  UmTestUtils::ExpectVariableHasValue(UpdateRestrictions::kRestrictDownloading,
                                      provider_->var_update_restrictions());
}

TEST_F(UmRealUpdaterProviderTest, GetUpdateRestrictionsNone) {
  EXPECT_CALL(*fake_sys_state_.mock_update_attempter(),
              GetCurrentUpdateAttemptFlags())
      .WillRepeatedly(Return(UpdateAttemptFlags::kNone));
  UmTestUtils::ExpectVariableHasValue(UpdateRestrictions::kNone,
                                      provider_->var_update_restrictions());
}
}  // namespace chromeos_update_manager
