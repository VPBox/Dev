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

#ifndef UPDATE_ENGINE_METRICS_REPORTER_INTERFACE_H_
#define UPDATE_ENGINE_METRICS_REPORTER_INTERFACE_H_

#include <memory>
#include <string>

#include <base/time/time.h>

#include "update_engine/common/constants.h"
#include "update_engine/common/error_code.h"
#include "update_engine/metrics_constants.h"
#include "update_engine/system_state.h"

namespace chromeos_update_engine {

enum class ServerToCheck;
enum class CertificateCheckResult;

namespace metrics {

std::unique_ptr<MetricsReporterInterface> CreateMetricsReporter();

}  // namespace metrics

class MetricsReporterInterface {
 public:
  virtual ~MetricsReporterInterface() = default;

  virtual void Initialize() = 0;

  // Helper function to report metrics related to user-initiated rollback. The
  // following metrics are reported:
  //
  //  |kMetricRollbackResult|
  virtual void ReportRollbackMetrics(metrics::RollbackResult result) = 0;

  // Helper function to report metrics related to enterprise (admin-initiated)
  // rollback:
  //
  //  |kMetricEnterpriseRollbackSuccess|
  //  |kMetricEnterpriseRollbackFailure|
  virtual void ReportEnterpriseRollbackMetrics(
      bool success, const std::string& rollback_version) = 0;

  // Helper function to report metrics reported once a day. The
  // following metrics are reported:
  //
  //  |kMetricDailyOSAgeDays|
  virtual void ReportDailyMetrics(base::TimeDelta os_age) = 0;

  // Helper function to report metrics after completing an update check
  // with the ChromeOS update server ("Omaha"). The following metrics
  // are reported:
  //
  //  |kMetricCheckResult|
  //  |kMetricCheckReaction|
  //  |kMetricCheckDownloadErrorCode|
  //  |kMetricCheckTimeSinceLastCheckMinutes|
  //  |kMetricCheckTimeSinceLastCheckUptimeMinutes|
  //  |kMetricCheckTargetVersion|
  //  |kMetricCheckRollbackTargetVersion|
  //
  // The |kMetricCheckResult| metric will only be reported if |result|
  // is not |kUnset|.
  //
  // The |kMetricCheckReaction| metric will only be reported if
  // |reaction| is not |kUnset|.
  //
  // The |kMetricCheckDownloadErrorCode| will only be reported if
  // |download_error_code| is not |kUnset|.
  //
  // The values for the |kMetricCheckTimeSinceLastCheckMinutes| and
  // |kMetricCheckTimeSinceLastCheckUptimeMinutes| metrics are
  // automatically reported and calculated by maintaining persistent
  // and process-local state variables.
  //
  // |kMetricCheckTargetVersion| reports the first section of the target version
  // if it's set, |kMetricCheckRollbackTargetVersion| reports the same, but only
  // if rollback is also allowed using enterprise policy.
  virtual void ReportUpdateCheckMetrics(
      SystemState* system_state,
      metrics::CheckResult result,
      metrics::CheckReaction reaction,
      metrics::DownloadErrorCode download_error_code) = 0;

  // Helper function to report metrics after the completion of each
  // update attempt. The following metrics are reported:
  //
  //  |kMetricAttemptNumber|
  //  |kMetricAttemptPayloadType|
  //  |kMetricAttemptPayloadSizeMiB|
  //  |kMetricAttemptDurationMinutes|
  //  |kMetricAttemptDurationUptimeMinutes|
  //  |kMetricAttemptTimeSinceLastAttemptMinutes|
  //  |kMetricAttemptTimeSinceLastAttemptUptimeMinutes|
  //  |kMetricAttemptResult|
  //  |kMetricAttemptInternalErrorCode|
  //
  // The |kMetricAttemptInternalErrorCode| metric will only be reported
  // if |internal_error_code| is not |kErrorSuccess|.
  //
  // The |kMetricAttemptDownloadErrorCode| metric will only be
  // reported if |payload_download_error_code| is not |kUnset|.
  //
  // The values for the |kMetricAttemptTimeSinceLastAttemptMinutes| and
  // |kMetricAttemptTimeSinceLastAttemptUptimeMinutes| metrics are
  // automatically calculated and reported by maintaining persistent and
  // process-local state variables.
  virtual void ReportUpdateAttemptMetrics(SystemState* system_state,
                                          int attempt_number,
                                          PayloadType payload_type,
                                          base::TimeDelta duration,
                                          base::TimeDelta duration_uptime,
                                          int64_t payload_size,
                                          metrics::AttemptResult attempt_result,
                                          ErrorCode internal_error_code) = 0;

  // Helper function to report download metrics after the completion of each
  // update attempt. The following metrics are reported:
  //
  // |kMetricAttemptPayloadBytesDownloadedMiB|
  // |kMetricAttemptPayloadDownloadSpeedKBps|
  // |kMetricAttemptDownloadSource|
  // |kMetricAttemptDownloadErrorCode|
  // |kMetricAttemptConnectionType|
  virtual void ReportUpdateAttemptDownloadMetrics(
      int64_t payload_bytes_downloaded,
      int64_t payload_download_speed_bps,
      DownloadSource download_source,
      metrics::DownloadErrorCode payload_download_error_code,
      metrics::ConnectionType connection_type) = 0;

  // Reports the |kAbnormalTermination| for the |kMetricAttemptResult|
  // metric. No other metrics in the UpdateEngine.Attempt.* namespace
  // will be reported.
  virtual void ReportAbnormallyTerminatedUpdateAttemptMetrics() = 0;

  // Helper function to report the after the completion of a successful
  // update attempt. The following metrics are reported:
  //
  //  |kMetricSuccessfulUpdateAttemptCount|
  //  |kMetricSuccessfulUpdateUpdatesAbandonedCount|
  //  |kMetricSuccessfulUpdatePayloadType|
  //  |kMetricSuccessfulUpdatePayloadSizeMiB|
  //  |kMetricSuccessfulUpdateBytesDownloadedMiBHttpsServer|
  //  |kMetricSuccessfulUpdateBytesDownloadedMiBHttpServer|
  //  |kMetricSuccessfulUpdateBytesDownloadedMiBHttpPeer|
  //  |kMetricSuccessfulUpdateBytesDownloadedMiB|
  //  |kMetricSuccessfulUpdateDownloadSourcesUsed|
  //  |kMetricSuccessfulUpdateDownloadOverheadPercentage|
  //  |kMetricSuccessfulUpdateTotalDurationMinutes|
  //  |kMetricSuccessfulUpdateTotalDurationUptimeMinutes|
  //  |kMetricSuccessfulUpdateRebootCount|
  //  |kMetricSuccessfulUpdateUrlSwitchCount|
  //
  // The values for the |kMetricSuccessfulUpdateDownloadSourcesUsed| are
  // |kMetricSuccessfulUpdateBytesDownloadedMiB| metrics automatically
  // calculated from examining the |num_bytes_downloaded| array.
  virtual void ReportSuccessfulUpdateMetrics(
      int attempt_count,
      int updates_abandoned_count,
      PayloadType payload_type,
      int64_t payload_size,
      int64_t num_bytes_downloaded[kNumDownloadSources],
      int download_overhead_percentage,
      base::TimeDelta total_duration,
      base::TimeDelta total_duration_uptime,
      int reboot_count,
      int url_switch_count) = 0;

  // Helper function to report the after the completion of a SSL certificate
  // check. One of the following metrics is reported:
  //
  //  |kMetricCertificateCheckUpdateCheck|
  //  |kMetricCertificateCheckDownload|
  virtual void ReportCertificateCheckMetrics(ServerToCheck server_to_check,
                                             CertificateCheckResult result) = 0;

  // Helper function to report the number failed update attempts. The following
  // metrics are reported:
  //
  // |kMetricFailedUpdateCount|
  virtual void ReportFailedUpdateCount(int target_attempt) = 0;

  // Helper function to report the time interval in minutes between a
  // successful update and the reboot into the updated system. The following
  // metrics are reported:
  //
  // |kMetricTimeToRebootMinutes|
  virtual void ReportTimeToReboot(int time_to_reboot_minutes) = 0;

  // Helper function to report the source of installation data. The following
  // metrics are reported:
  //
  // |kMetricInstallDateProvisioningSource|
  virtual void ReportInstallDateProvisioningSource(int source, int max) = 0;

  // Helper function to report an internal error code. The following metrics are
  // reported:
  //
  // |kMetricAttemptInternalErrorCode|
  virtual void ReportInternalErrorCode(ErrorCode error_code) = 0;

  // Helper function to report metrics related to the verified boot key
  // versions:
  //
  //  |kMetricKernelMinVersion|
  //  |kMetricKernelMaxRollforwardVersion|
  //  |kMetricKernelMaxRollforwardSetSuccess|
  virtual void ReportKeyVersionMetrics(int kernel_min_version,
                                       int kernel_max_rollforward_version,
                                       bool kernel_max_rollforward_success) = 0;

  // Helper function to report the duration between an update being seen by the
  // client to the update being applied. Updates are not always immediately
  // applied when seen, several enterprise policies can affect when an update
  // would actually be downloaded and applied.
  //
  // This metric should only be reported for enterprise enrolled devices.
  //
  // The following metrics are reported from this function:
  //   If |has_time_restriction_policy| is false:
  //     |kMetricSuccessfulUpdateDurationFromSeenDays|
  //   If |has_time_restriction_policy| is true:
  //     |kMetricSuccessfulUpdateDurationFromSeenTimeRestrictedDays|
  //
  virtual void ReportEnterpriseUpdateSeenToDownloadDays(
      bool has_time_restriction_policy, int time_to_update_days) = 0;
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_METRICS_REPORTER_INTERFACE_H_
