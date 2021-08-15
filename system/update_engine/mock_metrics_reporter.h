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

#ifndef UPDATE_ENGINE_MOCK_METRICS_REPORTER_H_
#define UPDATE_ENGINE_MOCK_METRICS_REPORTER_H_

#include <string>

#include <gmock/gmock.h>

#include "update_engine/metrics_reporter_interface.h"

namespace chromeos_update_engine {

class MockMetricsReporter : public MetricsReporterInterface {
 public:
  MOCK_METHOD0(Initialize, void());

  MOCK_METHOD1(ReportRollbackMetrics, void(metrics::RollbackResult result));

  MOCK_METHOD2(ReportEnterpriseRollbackMetrics,
               void(bool success, const std::string& rollback_version));

  MOCK_METHOD1(ReportDailyMetrics, void(base::TimeDelta os_age));

  MOCK_METHOD4(ReportUpdateCheckMetrics,
               void(SystemState* system_state,
                    metrics::CheckResult result,
                    metrics::CheckReaction reaction,
                    metrics::DownloadErrorCode download_error_code));

  MOCK_METHOD8(ReportUpdateAttemptMetrics,
               void(SystemState* system_state,
                    int attempt_number,
                    PayloadType payload_type,
                    base::TimeDelta duration,
                    base::TimeDelta duration_uptime,
                    int64_t payload_size,
                    metrics::AttemptResult attempt_result,
                    ErrorCode internal_error_code));

  MOCK_METHOD5(ReportUpdateAttemptDownloadMetrics,
               void(int64_t payload_bytes_downloaded,
                    int64_t payload_download_speed_bps,
                    DownloadSource download_source,
                    metrics::DownloadErrorCode payload_download_error_code,
                    metrics::ConnectionType connection_type));

  MOCK_METHOD0(ReportAbnormallyTerminatedUpdateAttemptMetrics, void());

  MOCK_METHOD10(ReportSuccessfulUpdateMetrics,
                void(int attempt_count,
                     int updates_abandoned_count,
                     PayloadType payload_type,
                     int64_t payload_size,
                     int64_t num_bytes_downloaded[kNumDownloadSources],
                     int download_overhead_percentage,
                     base::TimeDelta total_duration,
                     base::TimeDelta total_duration_uptime,
                     int reboot_count,
                     int url_switch_count));

  MOCK_METHOD2(ReportCertificateCheckMetrics,
               void(ServerToCheck server_to_check,
                    CertificateCheckResult result));

  MOCK_METHOD1(ReportFailedUpdateCount, void(int target_attempt));

  MOCK_METHOD1(ReportTimeToReboot, void(int time_to_reboot_minutes));

  MOCK_METHOD2(ReportInstallDateProvisioningSource, void(int source, int max));

  MOCK_METHOD1(ReportInternalErrorCode, void(ErrorCode error_code));

  MOCK_METHOD3(ReportKeyVersionMetrics,
               void(int kernel_min_version,
                    int kernel_max_rollforward_version,
                    bool kernel_max_rollforward_success));

  MOCK_METHOD2(ReportEnterpriseUpdateSeenToDownloadDays,
               void(bool has_time_restriction_policy, int time_to_update_days));
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_MOCK_METRICS_REPORTER_H_
