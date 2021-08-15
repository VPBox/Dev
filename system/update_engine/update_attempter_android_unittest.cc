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

#include "update_engine/update_attempter_android.h"

#include <memory>
#include <string>

#include <android-base/properties.h>
#include <base/time/time.h>
#include <gtest/gtest.h>

#include "update_engine/common/fake_boot_control.h"
#include "update_engine/common/fake_clock.h"
#include "update_engine/common/fake_hardware.h"
#include "update_engine/common/fake_prefs.h"
#include "update_engine/common/mock_action_processor.h"
#include "update_engine/common/test_utils.h"
#include "update_engine/common/utils.h"
#include "update_engine/daemon_state_android.h"
#include "update_engine/mock_metrics_reporter.h"

using base::Time;
using base::TimeDelta;
using testing::_;
using update_engine::UpdateStatus;

namespace chromeos_update_engine {

class UpdateAttempterAndroidTest : public ::testing::Test {
 protected:
  UpdateAttempterAndroidTest() = default;

  void SetUp() override {
    clock_ = new FakeClock();
    metrics_reporter_ = new testing::NiceMock<MockMetricsReporter>();
    update_attempter_android_.metrics_reporter_.reset(metrics_reporter_);
    update_attempter_android_.clock_.reset(clock_);
    update_attempter_android_.processor_.reset(
        new testing::NiceMock<MockActionProcessor>());
  }

  void SetUpdateStatus(update_engine::UpdateStatus status) {
    update_attempter_android_.status_ = status;
  }

  UpdateAttempterAndroid update_attempter_android_{
      &daemon_state_, &prefs_, &boot_control_, &hardware_};

  DaemonStateAndroid daemon_state_;
  FakePrefs prefs_;
  FakeBootControl boot_control_;
  FakeHardware hardware_;

  FakeClock* clock_;
  testing::NiceMock<MockMetricsReporter>* metrics_reporter_;
};

TEST_F(UpdateAttempterAndroidTest, UpdatePrefsSameBuildVersionOnInit) {
  std::string build_version =
      android::base::GetProperty("ro.build.version.incremental", "");
  prefs_.SetString(kPrefsPreviousVersion, build_version);
  prefs_.SetString(kPrefsBootId, "oldboot");
  prefs_.SetInt64(kPrefsNumReboots, 1);

  EXPECT_CALL(*metrics_reporter_, ReportTimeToReboot(_)).Times(0);
  update_attempter_android_.Init();

  // Check that the boot_id and reboot_count are updated.
  std::string boot_id;
  utils::GetBootId(&boot_id);
  EXPECT_TRUE(prefs_.Exists(kPrefsBootId));
  std::string prefs_boot_id;
  EXPECT_TRUE(prefs_.GetString(kPrefsBootId, &prefs_boot_id));
  EXPECT_EQ(boot_id, prefs_boot_id);

  EXPECT_TRUE(prefs_.Exists(kPrefsNumReboots));
  int64_t reboot_count;
  EXPECT_TRUE(prefs_.GetInt64(kPrefsNumReboots, &reboot_count));
  EXPECT_EQ(2, reboot_count);
}

TEST_F(UpdateAttempterAndroidTest, UpdatePrefsBuildVersionChangeOnInit) {
  prefs_.SetString(kPrefsPreviousVersion, "00001");  // Set the fake version
  prefs_.SetInt64(kPrefsPayloadAttemptNumber, 1);
  prefs_.SetInt64(kPrefsSystemUpdatedMarker, 23456);

  EXPECT_CALL(*metrics_reporter_,
              ReportAbnormallyTerminatedUpdateAttemptMetrics())
      .Times(1);

  Time now = Time::FromInternalValue(34456);
  clock_->SetMonotonicTime(now);
  TimeDelta duration = now - Time::FromInternalValue(23456);
  EXPECT_CALL(*metrics_reporter_, ReportTimeToReboot(duration.InMinutes()))
      .Times(1);

  update_attempter_android_.Init();
  // Check that we reset the metric prefs.
  EXPECT_FALSE(prefs_.Exists(kPrefsNumReboots));
  EXPECT_FALSE(prefs_.Exists(kPrefsUpdateTimestampStart));
  EXPECT_FALSE(prefs_.Exists(kPrefsSystemUpdatedMarker));
  // PayloadAttemptNumber should persist across reboots.
  EXPECT_TRUE(prefs_.Exists(kPrefsPayloadAttemptNumber));
}

TEST_F(UpdateAttempterAndroidTest, ReportMetricsOnUpdateTerminated) {
  prefs_.SetInt64(kPrefsNumReboots, 3);
  prefs_.SetInt64(kPrefsPayloadAttemptNumber, 2);
  prefs_.SetString(kPrefsPreviousVersion, "56789");
  prefs_.SetInt64(kPrefsUpdateBootTimestampStart, 10000);
  prefs_.SetInt64(kPrefsUpdateTimestampStart, 12345);

  Time boot_time = Time::FromInternalValue(22345);
  Time up_time = Time::FromInternalValue(21345);
  clock_->SetBootTime(boot_time);
  clock_->SetMonotonicTime(up_time);
  TimeDelta duration = boot_time - Time::FromInternalValue(10000);
  TimeDelta duration_uptime = up_time - Time::FromInternalValue(12345);
  EXPECT_CALL(
      *metrics_reporter_,
      ReportUpdateAttemptMetrics(_,
                                 2,
                                 _,
                                 duration,
                                 duration_uptime,
                                 _,
                                 metrics::AttemptResult::kUpdateSucceeded,
                                 ErrorCode::kSuccess))
      .Times(1);
  EXPECT_CALL(*metrics_reporter_,
              ReportSuccessfulUpdateMetrics(
                  2, 0, _, _, _, _, duration, duration_uptime, 3, _))
      .Times(1);

  SetUpdateStatus(UpdateStatus::UPDATE_AVAILABLE);
  update_attempter_android_.ProcessingDone(nullptr, ErrorCode::kSuccess);

  EXPECT_FALSE(prefs_.Exists(kPrefsNumReboots));
  EXPECT_FALSE(prefs_.Exists(kPrefsPayloadAttemptNumber));
  EXPECT_FALSE(prefs_.Exists(kPrefsUpdateTimestampStart));
  EXPECT_TRUE(prefs_.Exists(kPrefsSystemUpdatedMarker));
}

TEST_F(UpdateAttempterAndroidTest, ReportMetricsForBytesDownloaded) {
  // Check both prefs are updated correctly.
  update_attempter_android_.BytesReceived(20, 50, 200);
  EXPECT_EQ(
      20,
      metrics_utils::GetPersistedValue(kPrefsCurrentBytesDownloaded, &prefs_));
  EXPECT_EQ(
      20,
      metrics_utils::GetPersistedValue(kPrefsTotalBytesDownloaded, &prefs_));

  EXPECT_CALL(*metrics_reporter_,
              ReportUpdateAttemptDownloadMetrics(50, _, _, _, _))
      .Times(1);
  EXPECT_CALL(*metrics_reporter_,
              ReportUpdateAttemptDownloadMetrics(40, _, _, _, _))
      .Times(1);

  int64_t total_bytes[kNumDownloadSources] = {};
  total_bytes[kDownloadSourceHttpsServer] = 90;
  EXPECT_CALL(*metrics_reporter_,
              ReportSuccessfulUpdateMetrics(
                  _,
                  _,
                  _,
                  _,
                  test_utils::DownloadSourceMatcher(total_bytes),
                  125,
                  _,
                  _,
                  _,
                  _))
      .Times(1);

  // The first update fails after receiving 50 bytes in total.
  update_attempter_android_.BytesReceived(30, 50, 200);
  update_attempter_android_.ProcessingDone(nullptr, ErrorCode::kError);
  EXPECT_EQ(
      0,
      metrics_utils::GetPersistedValue(kPrefsCurrentBytesDownloaded, &prefs_));
  EXPECT_EQ(
      50,
      metrics_utils::GetPersistedValue(kPrefsTotalBytesDownloaded, &prefs_));

  // The second update succeeds after receiving 40 bytes, which leads to a
  // overhead of 50 / 40 = 125%.
  update_attempter_android_.BytesReceived(40, 40, 50);
  update_attempter_android_.ProcessingDone(nullptr, ErrorCode::kSuccess);
  // Both prefs should be cleared.
  EXPECT_EQ(
      0,
      metrics_utils::GetPersistedValue(kPrefsCurrentBytesDownloaded, &prefs_));
  EXPECT_EQ(
      0, metrics_utils::GetPersistedValue(kPrefsTotalBytesDownloaded, &prefs_));
}

}  // namespace chromeos_update_engine
