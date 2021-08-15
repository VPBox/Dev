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

#ifndef UPDATE_ENGINE_METRICS_REPORTER_OMAHA_H_
#define UPDATE_ENGINE_METRICS_REPORTER_OMAHA_H_

#include <memory>
#include <string>

#include <base/time/time.h>
#include <metrics/metrics_library.h>

#include "update_engine/certificate_checker.h"
#include "update_engine/common/constants.h"
#include "update_engine/common/error_code.h"
#include "update_engine/metrics_constants.h"
#include "update_engine/metrics_reporter_interface.h"
#include "update_engine/system_state.h"

namespace chromeos_update_engine {

class SystemState;

namespace metrics {

// UpdateEngine.Daily.* metrics.
extern const char kMetricDailyOSAgeDays[];

// UpdateEngine.Check.* metrics.
extern const char kMetricCheckDownloadErrorCode[];
extern const char kMetricCheckReaction[];
extern const char kMetricCheckResult[];
extern const char kMetricCheckTargetVersion[];
extern const char kMetricCheckRollbackTargetVersion[];
extern const char kMetricCheckTimeSinceLastCheckMinutes[];
extern const char kMetricCheckTimeSinceLastCheckUptimeMinutes[];

// UpdateEngine.Attempt.* metrics.
extern const char kMetricAttemptNumber[];
extern const char kMetricAttemptPayloadType[];
extern const char kMetricAttemptPayloadSizeMiB[];
extern const char kMetricAttemptConnectionType[];
extern const char kMetricAttemptDurationMinutes[];
extern const char kMetricAttemptDurationUptimeMinutes[];
extern const char kMetricAttemptTimeSinceLastAttemptMinutes[];
extern const char kMetricAttemptTimeSinceLastAttemptUptimeMinutes[];
extern const char kMetricAttemptPayloadBytesDownloadedMiB[];
extern const char kMetricAttemptPayloadDownloadSpeedKBps[];
extern const char kMetricAttemptDownloadSource[];
extern const char kMetricAttemptResult[];
extern const char kMetricAttemptInternalErrorCode[];
extern const char kMetricAttemptDownloadErrorCode[];

// UpdateEngine.SuccessfulUpdate.* metrics.
extern const char kMetricSuccessfulUpdateAttemptCount[];
extern const char kMetricSuccessfulUpdateBytesDownloadedMiB[];
extern const char kMetricSuccessfulUpdateDownloadOverheadPercentage[];
extern const char kMetricSuccessfulUpdateDownloadSourcesUsed[];
extern const char kMetricSuccessfulUpdateDurationFromSeenDays[];
extern const char kMetricSuccessfulUpdateDurationFromSeenTimeRestrictedDays[];
extern const char kMetricSuccessfulUpdatePayloadType[];
extern const char kMetricSuccessfulUpdatePayloadSizeMiB[];
extern const char kMetricSuccessfulUpdateRebootCount[];
extern const char kMetricSuccessfulUpdateTotalDurationMinutes[];
extern const char kMetricSuccessfulUpdateTotalDurationUptimeMinutes[];
extern const char kMetricSuccessfulUpdateUpdatesAbandonedCount[];
extern const char kMetricSuccessfulUpdateUrlSwitchCount[];

// UpdateEngine.Rollback.* metric.
extern const char kMetricRollbackResult[];

// UpdateEngine.EnterpriseRollback.* metrics.
extern const char kMetricEnterpriseRollbackFailure[];
extern const char kMetricEnterpriseRollbackSuccess[];

// UpdateEngine.CertificateCheck.* metrics.
extern const char kMetricCertificateCheckUpdateCheck[];
extern const char kMetricCertificateCheckDownload[];

// UpdateEngine.KernelKey.* metrics.
extern const char kMetricKernelMinVersion[];
extern const char kMetricKernelMaxRollforwardVersion[];
extern const char kMetricKernelMaxRollforwardSetSuccess[];

// UpdateEngine.* metrics.
extern const char kMetricFailedUpdateCount[];
extern const char kMetricInstallDateProvisioningSource[];
extern const char kMetricTimeToRebootMinutes[];

}  // namespace metrics

class MetricsReporterOmaha : public MetricsReporterInterface {
 public:
  MetricsReporterOmaha();

  ~MetricsReporterOmaha() override = default;

  void Initialize() override;

  void ReportRollbackMetrics(metrics::RollbackResult result) override;

  void ReportEnterpriseRollbackMetrics(
      bool success, const std::string& rollback_version) override;

  void ReportDailyMetrics(base::TimeDelta os_age) override;

  void ReportUpdateCheckMetrics(
      SystemState* system_state,
      metrics::CheckResult result,
      metrics::CheckReaction reaction,
      metrics::DownloadErrorCode download_error_code) override;

  void ReportUpdateAttemptMetrics(SystemState* system_state,
                                  int attempt_number,
                                  PayloadType payload_type,
                                  base::TimeDelta duration,
                                  base::TimeDelta duration_uptime,
                                  int64_t payload_size,
                                  metrics::AttemptResult attempt_result,
                                  ErrorCode internal_error_code) override;

  void ReportUpdateAttemptDownloadMetrics(
      int64_t payload_bytes_downloaded,
      int64_t payload_download_speed_bps,
      DownloadSource download_source,
      metrics::DownloadErrorCode payload_download_error_code,
      metrics::ConnectionType connection_type) override;

  void ReportAbnormallyTerminatedUpdateAttemptMetrics() override;

  void ReportSuccessfulUpdateMetrics(
      int attempt_count,
      int updates_abandoned_count,
      PayloadType payload_type,
      int64_t payload_size,
      int64_t num_bytes_downloaded[kNumDownloadSources],
      int download_overhead_percentage,
      base::TimeDelta total_duration,
      base::TimeDelta total_duration_uptime,
      int reboot_count,
      int url_switch_count) override;

  void ReportCertificateCheckMetrics(ServerToCheck server_to_check,
                                     CertificateCheckResult result) override;

  void ReportFailedUpdateCount(int target_attempt) override;

  void ReportTimeToReboot(int time_to_reboot_minutes) override;

  void ReportInstallDateProvisioningSource(int source, int max) override;

  void ReportInternalErrorCode(ErrorCode error_code) override;

  void ReportKeyVersionMetrics(int kernel_min_version,
                               int kernel_max_rollforward_version,
                               bool kernel_max_rollforward_success) override;

  void ReportEnterpriseUpdateSeenToDownloadDays(
      bool has_time_restriction_policy, int time_to_update_days) override;

 private:
  friend class MetricsReporterOmahaTest;

  std::unique_ptr<MetricsLibraryInterface> metrics_lib_;

  DISALLOW_COPY_AND_ASSIGN(MetricsReporterOmaha);
};  // class metrics

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_METRICS_REPORTER_OMAHA_H_
