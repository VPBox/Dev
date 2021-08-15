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

#include "update_engine/metrics_reporter_omaha.h"

#include <memory>
#include <string>

#include <base/time/time.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <metrics/metrics_library_mock.h>

#include "update_engine/common/fake_clock.h"
#include "update_engine/common/fake_prefs.h"
#include "update_engine/fake_system_state.h"

using base::TimeDelta;
using testing::_;
using testing::AnyNumber;
using testing::Return;

namespace chromeos_update_engine {
class MetricsReporterOmahaTest : public ::testing::Test {
 protected:
  MetricsReporterOmahaTest() = default;

  // Reset the metrics_lib_ to a mock library.
  void SetUp() override {
    mock_metrics_lib_ = new testing::NiceMock<MetricsLibraryMock>();
    reporter_.metrics_lib_.reset(mock_metrics_lib_);
  }

  testing::NiceMock<MetricsLibraryMock>* mock_metrics_lib_;
  MetricsReporterOmaha reporter_;
};

TEST_F(MetricsReporterOmahaTest, ReportDailyMetrics) {
  TimeDelta age = TimeDelta::FromDays(10);
  EXPECT_CALL(*mock_metrics_lib_,
              SendToUMA(metrics::kMetricDailyOSAgeDays, _, _, _, _))
      .Times(1);

  reporter_.ReportDailyMetrics(age);
}

TEST_F(MetricsReporterOmahaTest, ReportUpdateCheckMetrics) {
  FakeSystemState fake_system_state;
  FakeClock fake_clock;
  FakePrefs fake_prefs;

  // We need to execute the report twice to test the time since last report.
  fake_system_state.set_clock(&fake_clock);
  fake_system_state.set_prefs(&fake_prefs);
  fake_clock.SetWallclockTime(base::Time::FromInternalValue(1000000));
  fake_clock.SetMonotonicTime(base::Time::FromInternalValue(1000000));

  metrics::CheckResult result = metrics::CheckResult::kUpdateAvailable;
  metrics::CheckReaction reaction = metrics::CheckReaction::kIgnored;
  metrics::DownloadErrorCode error_code =
      metrics::DownloadErrorCode::kHttpStatus200;

  EXPECT_CALL(
      *mock_metrics_lib_,
      SendEnumToUMA(metrics::kMetricCheckResult, static_cast<int>(result), _))
      .Times(2);
  EXPECT_CALL(*mock_metrics_lib_,
              SendEnumToUMA(
                  metrics::kMetricCheckReaction, static_cast<int>(reaction), _))
      .Times(2);
  EXPECT_CALL(*mock_metrics_lib_,
              SendSparseToUMA(metrics::kMetricCheckDownloadErrorCode,
                              static_cast<int>(error_code)))
      .Times(2);

  // Not pinned nor rollback
  EXPECT_CALL(*mock_metrics_lib_,
              SendSparseToUMA(metrics::kMetricCheckTargetVersion, _))
      .Times(0);
  EXPECT_CALL(*mock_metrics_lib_,
              SendSparseToUMA(metrics::kMetricCheckRollbackTargetVersion, _))
      .Times(0);

  EXPECT_CALL(
      *mock_metrics_lib_,
      SendToUMA(metrics::kMetricCheckTimeSinceLastCheckMinutes, 1, _, _, _))
      .Times(1);
  EXPECT_CALL(
      *mock_metrics_lib_,
      SendToUMA(
          metrics::kMetricCheckTimeSinceLastCheckUptimeMinutes, 1, _, _, _))
      .Times(1);

  reporter_.ReportUpdateCheckMetrics(
      &fake_system_state, result, reaction, error_code);

  // Advance the clock by 1 minute and report the same metrics again.
  fake_clock.SetWallclockTime(base::Time::FromInternalValue(61000000));
  fake_clock.SetMonotonicTime(base::Time::FromInternalValue(61000000));
  // Allow rollback
  reporter_.ReportUpdateCheckMetrics(
      &fake_system_state, result, reaction, error_code);
}

TEST_F(MetricsReporterOmahaTest, ReportUpdateCheckMetricsPinned) {
  FakeSystemState fake_system_state;

  OmahaRequestParams params(&fake_system_state);
  params.set_target_version_prefix("10575.");
  params.set_rollback_allowed(false);
  fake_system_state.set_request_params(&params);

  metrics::CheckResult result = metrics::CheckResult::kUpdateAvailable;
  metrics::CheckReaction reaction = metrics::CheckReaction::kIgnored;
  metrics::DownloadErrorCode error_code =
      metrics::DownloadErrorCode::kHttpStatus200;

  EXPECT_CALL(*mock_metrics_lib_,
              SendSparseToUMA(metrics::kMetricCheckDownloadErrorCode, _));
  // Target version set, but not a rollback.
  EXPECT_CALL(*mock_metrics_lib_,
              SendSparseToUMA(metrics::kMetricCheckTargetVersion, 10575))
      .Times(1);
  EXPECT_CALL(*mock_metrics_lib_,
              SendSparseToUMA(metrics::kMetricCheckRollbackTargetVersion, _))
      .Times(0);

  reporter_.ReportUpdateCheckMetrics(
      &fake_system_state, result, reaction, error_code);
}

TEST_F(MetricsReporterOmahaTest, ReportUpdateCheckMetricsRollback) {
  FakeSystemState fake_system_state;

  OmahaRequestParams params(&fake_system_state);
  params.set_target_version_prefix("10575.");
  params.set_rollback_allowed(true);
  fake_system_state.set_request_params(&params);

  metrics::CheckResult result = metrics::CheckResult::kUpdateAvailable;
  metrics::CheckReaction reaction = metrics::CheckReaction::kIgnored;
  metrics::DownloadErrorCode error_code =
      metrics::DownloadErrorCode::kHttpStatus200;

  EXPECT_CALL(*mock_metrics_lib_,
              SendSparseToUMA(metrics::kMetricCheckDownloadErrorCode, _));
  // Rollback.
  EXPECT_CALL(*mock_metrics_lib_,
              SendSparseToUMA(metrics::kMetricCheckTargetVersion, 10575))
      .Times(1);
  EXPECT_CALL(
      *mock_metrics_lib_,
      SendSparseToUMA(metrics::kMetricCheckRollbackTargetVersion, 10575))
      .Times(1);

  reporter_.ReportUpdateCheckMetrics(
      &fake_system_state, result, reaction, error_code);
}

TEST_F(MetricsReporterOmahaTest,
       ReportAbnormallyTerminatedUpdateAttemptMetrics) {
  EXPECT_CALL(*mock_metrics_lib_,
              SendEnumToUMA(metrics::kMetricAttemptResult,
                            static_cast<int>(
                                metrics::AttemptResult::kAbnormalTermination),
                            _))
      .Times(1);

  reporter_.ReportAbnormallyTerminatedUpdateAttemptMetrics();
}

TEST_F(MetricsReporterOmahaTest, ReportUpdateAttemptMetrics) {
  FakeSystemState fake_system_state;
  FakeClock fake_clock;
  FakePrefs fake_prefs;

  fake_system_state.set_clock(&fake_clock);
  fake_system_state.set_prefs(&fake_prefs);
  fake_clock.SetWallclockTime(base::Time::FromInternalValue(1000000));
  fake_clock.SetMonotonicTime(base::Time::FromInternalValue(1000000));

  int attempt_number = 1;
  PayloadType payload_type = kPayloadTypeFull;
  TimeDelta duration = TimeDelta::FromMinutes(1000);
  TimeDelta duration_uptime = TimeDelta::FromMinutes(1000);

  int64_t payload_size = 100 * kNumBytesInOneMiB;

  metrics::AttemptResult attempt_result =
      metrics::AttemptResult::kInternalError;
  ErrorCode internal_error_code = ErrorCode::kDownloadInvalidMetadataSignature;

  EXPECT_CALL(*mock_metrics_lib_,
              SendToUMA(metrics::kMetricAttemptNumber, attempt_number, _, _, _))
      .Times(2);
  EXPECT_CALL(*mock_metrics_lib_,
              SendEnumToUMA(metrics::kMetricAttemptPayloadType,
                            static_cast<int>(payload_type),
                            _))
      .Times(2);
  EXPECT_CALL(*mock_metrics_lib_,
              SendToUMA(metrics::kMetricAttemptDurationMinutes,
                        duration.InMinutes(),
                        _,
                        _,
                        _))
      .Times(2);
  EXPECT_CALL(*mock_metrics_lib_,
              SendToUMA(metrics::kMetricAttemptDurationUptimeMinutes,
                        duration_uptime.InMinutes(),
                        _,
                        _,
                        _))
      .Times(2);

  // Check the report of attempt result.
  EXPECT_CALL(
      *mock_metrics_lib_,
      SendEnumToUMA(
          metrics::kMetricAttemptResult, static_cast<int>(attempt_result), _))
      .Times(2);
  EXPECT_CALL(*mock_metrics_lib_,
              SendEnumToUMA(metrics::kMetricAttemptInternalErrorCode,
                            static_cast<int>(internal_error_code),
                            _))
      .Times(2);
  EXPECT_CALL(*mock_metrics_lib_,
              SendToUMA(metrics::kMetricAttemptPayloadSizeMiB, 100, _, _, _))
      .Times(2);

  // Check the duration between two reports.
  EXPECT_CALL(
      *mock_metrics_lib_,
      SendToUMA(metrics::kMetricAttemptTimeSinceLastAttemptMinutes, 1, _, _, _))
      .Times(1);
  EXPECT_CALL(
      *mock_metrics_lib_,
      SendToUMA(
          metrics::kMetricAttemptTimeSinceLastAttemptUptimeMinutes, 1, _, _, _))
      .Times(1);

  reporter_.ReportUpdateAttemptMetrics(&fake_system_state,
                                       attempt_number,
                                       payload_type,
                                       duration,
                                       duration_uptime,
                                       payload_size,
                                       attempt_result,
                                       internal_error_code);

  // Advance the clock by 1 minute and report the same metrics again.
  fake_clock.SetWallclockTime(base::Time::FromInternalValue(61000000));
  fake_clock.SetMonotonicTime(base::Time::FromInternalValue(61000000));
  reporter_.ReportUpdateAttemptMetrics(&fake_system_state,
                                       attempt_number,
                                       payload_type,
                                       duration,
                                       duration_uptime,
                                       payload_size,
                                       attempt_result,
                                       internal_error_code);
}

TEST_F(MetricsReporterOmahaTest, ReportUpdateAttemptDownloadMetrics) {
  int64_t payload_bytes_downloaded = 200 * kNumBytesInOneMiB;
  int64_t payload_download_speed_bps = 100 * 1000;
  DownloadSource download_source = kDownloadSourceHttpServer;
  metrics::DownloadErrorCode payload_download_error_code =
      metrics::DownloadErrorCode::kDownloadError;
  metrics::ConnectionType connection_type = metrics::ConnectionType::kCellular;

  EXPECT_CALL(
      *mock_metrics_lib_,
      SendToUMA(metrics::kMetricAttemptPayloadBytesDownloadedMiB, 200, _, _, _))
      .Times(1);
  EXPECT_CALL(
      *mock_metrics_lib_,
      SendToUMA(metrics::kMetricAttemptPayloadDownloadSpeedKBps, 100, _, _, _))
      .Times(1);
  EXPECT_CALL(*mock_metrics_lib_,
              SendEnumToUMA(metrics::kMetricAttemptDownloadSource,
                            static_cast<int>(download_source),
                            _))
      .Times(1);
  EXPECT_CALL(*mock_metrics_lib_,
              SendSparseToUMA(metrics::kMetricAttemptDownloadErrorCode,
                              static_cast<int>(payload_download_error_code)))
      .Times(1);
  EXPECT_CALL(*mock_metrics_lib_,
              SendEnumToUMA(metrics::kMetricAttemptConnectionType,
                            static_cast<int>(connection_type),
                            _))
      .Times(1);

  reporter_.ReportUpdateAttemptDownloadMetrics(payload_bytes_downloaded,
                                               payload_download_speed_bps,
                                               download_source,
                                               payload_download_error_code,
                                               connection_type);
}

TEST_F(MetricsReporterOmahaTest, ReportSuccessfulUpdateMetrics) {
  int attempt_count = 3;
  int updates_abandoned_count = 2;
  PayloadType payload_type = kPayloadTypeDelta;
  int64_t payload_size = 200 * kNumBytesInOneMiB;
  int64_t num_bytes_downloaded[kNumDownloadSources] = {};
  // 200MiB payload downloaded from HttpsServer.
  num_bytes_downloaded[0] = 200 * kNumBytesInOneMiB;
  int download_overhead_percentage = 20;
  TimeDelta total_duration = TimeDelta::FromMinutes(30);
  TimeDelta total_duration_uptime = TimeDelta::FromMinutes(20);
  int reboot_count = 2;
  int url_switch_count = 2;

  EXPECT_CALL(
      *mock_metrics_lib_,
      SendToUMA(metrics::kMetricSuccessfulUpdatePayloadSizeMiB, 200, _, _, _))
      .Times(1);

  // Check the report to both BytesDownloadedMiBHttpsServer and
  // BytesDownloadedMiB
  std::string DownloadedMiBMetric =
      metrics::kMetricSuccessfulUpdateBytesDownloadedMiB;
  DownloadedMiBMetric += "HttpsServer";
  EXPECT_CALL(*mock_metrics_lib_, SendToUMA(DownloadedMiBMetric, 200, _, _, _))
      .Times(1);
  EXPECT_CALL(
      *mock_metrics_lib_,
      SendToUMA(
          metrics::kMetricSuccessfulUpdateBytesDownloadedMiB, 200, _, _, _))
      .Times(1);

  EXPECT_CALL(
      *mock_metrics_lib_,
      SendToUMA(
          metrics::kMetricSuccessfulUpdateDownloadSourcesUsed, 1, _, _, _))
      .Times(1);
  EXPECT_CALL(
      *mock_metrics_lib_,
      SendToUMA(metrics::kMetricSuccessfulUpdateDownloadOverheadPercentage,
                20,
                _,
                _,
                _));

  EXPECT_CALL(*mock_metrics_lib_,
              SendToUMA(metrics::kMetricSuccessfulUpdateUrlSwitchCount,
                        url_switch_count,
                        _,
                        _,
                        _))
      .Times(1);
  EXPECT_CALL(
      *mock_metrics_lib_,
      SendToUMA(
          metrics::kMetricSuccessfulUpdateTotalDurationMinutes, 30, _, _, _))
      .Times(1);
  EXPECT_CALL(
      *mock_metrics_lib_,
      SendToUMA(metrics::kMetricSuccessfulUpdateTotalDurationUptimeMinutes,
                20,
                _,
                _,
                _))
      .Times(1);
  EXPECT_CALL(
      *mock_metrics_lib_,
      SendToUMA(
          metrics::kMetricSuccessfulUpdateRebootCount, reboot_count, _, _, _))
      .Times(1);
  EXPECT_CALL(*mock_metrics_lib_,
              SendEnumToUMA(
                  metrics::kMetricSuccessfulUpdatePayloadType, payload_type, _))
      .Times(1);
  EXPECT_CALL(
      *mock_metrics_lib_,
      SendToUMA(
          metrics::kMetricSuccessfulUpdateAttemptCount, attempt_count, _, _, _))
      .Times(1);
  EXPECT_CALL(*mock_metrics_lib_,
              SendToUMA(metrics::kMetricSuccessfulUpdateUpdatesAbandonedCount,
                        updates_abandoned_count,
                        _,
                        _,
                        _))
      .Times(1);

  reporter_.ReportSuccessfulUpdateMetrics(attempt_count,
                                          updates_abandoned_count,
                                          payload_type,
                                          payload_size,
                                          num_bytes_downloaded,
                                          download_overhead_percentage,
                                          total_duration,
                                          total_duration_uptime,
                                          reboot_count,
                                          url_switch_count);
}

TEST_F(MetricsReporterOmahaTest, ReportRollbackMetrics) {
  metrics::RollbackResult result = metrics::RollbackResult::kSuccess;
  EXPECT_CALL(*mock_metrics_lib_,
              SendEnumToUMA(
                  metrics::kMetricRollbackResult, static_cast<int>(result), _))
      .Times(1);

  reporter_.ReportRollbackMetrics(result);
}

TEST_F(MetricsReporterOmahaTest, ReportEnterpriseRollbackMetrics) {
  EXPECT_CALL(*mock_metrics_lib_,
              SendSparseToUMA(metrics::kMetricEnterpriseRollbackSuccess, 10575))
      .Times(1);
  EXPECT_CALL(*mock_metrics_lib_,
              SendSparseToUMA(metrics::kMetricEnterpriseRollbackFailure, 10323))
      .Times(1);

  reporter_.ReportEnterpriseRollbackMetrics(/*success=*/true, "10575.39.2");
  reporter_.ReportEnterpriseRollbackMetrics(/*success=*/false, "10323.67.7");
}

TEST_F(MetricsReporterOmahaTest, ReportCertificateCheckMetrics) {
  ServerToCheck server_to_check = ServerToCheck::kUpdate;
  CertificateCheckResult result = CertificateCheckResult::kValid;
  EXPECT_CALL(*mock_metrics_lib_,
              SendEnumToUMA(metrics::kMetricCertificateCheckUpdateCheck,
                            static_cast<int>(result),
                            _))
      .Times(1);

  reporter_.ReportCertificateCheckMetrics(server_to_check, result);
}

TEST_F(MetricsReporterOmahaTest, ReportFailedUpdateCount) {
  int target_attempt = 3;
  EXPECT_CALL(
      *mock_metrics_lib_,
      SendToUMA(metrics::kMetricFailedUpdateCount, target_attempt, _, _, _))
      .Times(1);

  reporter_.ReportFailedUpdateCount(target_attempt);
}

TEST_F(MetricsReporterOmahaTest, ReportTimeToReboot) {
  int time_to_reboot_minutes = 1000;
  EXPECT_CALL(
      *mock_metrics_lib_,
      SendToUMA(
          metrics::kMetricTimeToRebootMinutes, time_to_reboot_minutes, _, _, _))
      .Times(1);

  reporter_.ReportTimeToReboot(time_to_reboot_minutes);
}

TEST_F(MetricsReporterOmahaTest, ReportInstallDateProvisioningSource) {
  int source = 2;
  int max = 5;
  EXPECT_CALL(
      *mock_metrics_lib_,
      SendEnumToUMA(metrics::kMetricInstallDateProvisioningSource, source, max))
      .Times(1);

  reporter_.ReportInstallDateProvisioningSource(source, max);
}

TEST_F(MetricsReporterOmahaTest, ReportKeyVersionMetrics) {
  int kernel_min_version = 0x00040002;
  int kernel_max_rollforward_version = 0xfffffffe;
  bool kernel_max_rollforward_success = true;
  EXPECT_CALL(
      *mock_metrics_lib_,
      SendSparseToUMA(metrics::kMetricKernelMinVersion, kernel_min_version))
      .Times(1);
  EXPECT_CALL(*mock_metrics_lib_,
              SendSparseToUMA(metrics::kMetricKernelMaxRollforwardVersion,
                              kernel_max_rollforward_version))
      .Times(1);
  EXPECT_CALL(*mock_metrics_lib_,
              SendBoolToUMA(metrics::kMetricKernelMaxRollforwardSetSuccess,
                            kernel_max_rollforward_success))
      .Times(1);

  reporter_.ReportKeyVersionMetrics(kernel_min_version,
                                    kernel_max_rollforward_version,
                                    kernel_max_rollforward_success);
}

TEST_F(MetricsReporterOmahaTest, ReportEnterpriseUpdateSeenToDownloadDays) {
  constexpr int kDaysToUpdate = 10;
  constexpr int kMinBucket = 1;
  constexpr int kMaxBucket = 6 * 30;  // approximately 6 months
  constexpr int kNumBuckets = 50;

  EXPECT_CALL(*mock_metrics_lib_,
              SendToUMA(metrics::kMetricSuccessfulUpdateDurationFromSeenDays,
                        kDaysToUpdate,
                        kMinBucket,
                        kMaxBucket,
                        kNumBuckets))
      .Times(1);

  reporter_.ReportEnterpriseUpdateSeenToDownloadDays(
      false /* has_time_restriction_policy */, kDaysToUpdate);
}

TEST_F(MetricsReporterOmahaTest,
       ReportEnterpriseTimeRestrictedUpdateSeenToDownloadTime) {
  const int kDaysToUpdate = 15;
  constexpr int kMinBucket = 1;
  constexpr int kMaxBucket = 6 * 30;  // approximately 6 months
  constexpr int kNumBuckets = 50;

  EXPECT_CALL(
      *mock_metrics_lib_,
      SendToUMA(
          metrics::kMetricSuccessfulUpdateDurationFromSeenTimeRestrictedDays,
          kDaysToUpdate,
          kMinBucket,
          kMaxBucket,
          kNumBuckets))
      .Times(1);

  reporter_.ReportEnterpriseUpdateSeenToDownloadDays(
      true /* has_time_restriction_policy */, kDaysToUpdate);
}

}  // namespace chromeos_update_engine
