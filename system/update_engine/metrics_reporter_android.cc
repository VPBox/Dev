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

#include "update_engine/metrics_reporter_android.h"

#include <memory>
#include <string>

#include <metricslogger/metrics_logger.h>

#include "update_engine/common/constants.h"

namespace {
void LogHistogram(const std::string& metrics, int value) {
  android::metricslogger::LogHistogram(metrics, value);
  LOG(INFO) << "uploading " << value << " to histogram for metric " << metrics;
}
}  // namespace

namespace chromeos_update_engine {

namespace metrics {

// The histograms are defined in:
// depot/google3/analysis/uma/configs/clearcut/TRON/histograms.xml
constexpr char kMetricsUpdateEngineAttemptNumber[] =
    "ota_update_engine_attempt_number";
constexpr char kMetricsUpdateEngineAttemptResult[] =
    "ota_update_engine_attempt_result";
constexpr char kMetricsUpdateEngineAttemptDurationInMinutes[] =
    "ota_update_engine_attempt_fixed_duration_boottime_in_minutes";
constexpr char kMetricsUpdateEngineAttemptDurationUptimeInMinutes[] =
    "ota_update_engine_attempt_duration_monotonic_in_minutes";
constexpr char kMetricsUpdateEngineAttemptErrorCode[] =
    "ota_update_engine_attempt_error_code";
constexpr char kMetricsUpdateEngineAttemptPayloadSizeMiB[] =
    "ota_update_engine_attempt_payload_size_mib";
constexpr char kMetricsUpdateEngineAttemptPayloadType[] =
    "ota_update_engine_attempt_payload_type";
constexpr char kMetricsUpdateEngineAttemptCurrentBytesDownloadedMiB[] =
    "ota_update_engine_attempt_fixed_current_bytes_downloaded_mib";

constexpr char kMetricsUpdateEngineSuccessfulUpdateAttemptCount[] =
    "ota_update_engine_successful_update_attempt_count";
constexpr char kMetricsUpdateEngineSuccessfulUpdateTotalDurationInMinutes[] =
    "ota_update_engine_successful_update_fixed_total_duration_in_minutes";
constexpr char kMetricsUpdateEngineSuccessfulUpdatePayloadSizeMiB[] =
    "ota_update_engine_successful_update_payload_size_mib";
constexpr char kMetricsUpdateEngineSuccessfulUpdatePayloadType[] =
    "ota_update_engine_successful_update_payload_type";
constexpr char kMetricsUpdateEngineSuccessfulUpdateRebootCount[] =
    "ota_update_engine_successful_update_reboot_count";
constexpr char kMetricsUpdateEngineSuccessfulUpdateTotalBytesDownloadedMiB[] =
    "ota_update_engine_successful_update_total_bytes_downloaded_mib";
constexpr char
    kMetricsUpdateEngineSuccessfulUpdateDownloadOverheadPercentage[] =
        "ota_update_engine_successful_update_download_overhead_percentage";

std::unique_ptr<MetricsReporterInterface> CreateMetricsReporter() {
  return std::make_unique<MetricsReporterAndroid>();
}

}  // namespace metrics

void MetricsReporterAndroid::ReportUpdateAttemptMetrics(
    SystemState* /* system_state */,
    int attempt_number,
    PayloadType payload_type,
    base::TimeDelta duration,
    base::TimeDelta duration_uptime,
    int64_t payload_size,
    metrics::AttemptResult attempt_result,
    ErrorCode error_code) {
  LogHistogram(metrics::kMetricsUpdateEngineAttemptNumber, attempt_number);
  LogHistogram(metrics::kMetricsUpdateEngineAttemptPayloadType,
               static_cast<int>(payload_type));
  LogHistogram(metrics::kMetricsUpdateEngineAttemptDurationInMinutes,
               duration.InMinutes());
  LogHistogram(metrics::kMetricsUpdateEngineAttemptDurationUptimeInMinutes,
               duration_uptime.InMinutes());

  int64_t payload_size_mib = payload_size / kNumBytesInOneMiB;
  LogHistogram(metrics::kMetricsUpdateEngineAttemptPayloadSizeMiB,
               payload_size_mib);

  LogHistogram(metrics::kMetricsUpdateEngineAttemptResult,
               static_cast<int>(attempt_result));
  LogHistogram(metrics::kMetricsUpdateEngineAttemptErrorCode,
               static_cast<int>(error_code));
}

void MetricsReporterAndroid::ReportUpdateAttemptDownloadMetrics(
    int64_t payload_bytes_downloaded,
    int64_t /* payload_download_speed_bps */,
    DownloadSource /* download_source */,
    metrics::DownloadErrorCode /* payload_download_error_code */,
    metrics::ConnectionType /* connection_type */) {
  LogHistogram(metrics::kMetricsUpdateEngineAttemptCurrentBytesDownloadedMiB,
               payload_bytes_downloaded / kNumBytesInOneMiB);
}

void MetricsReporterAndroid::ReportSuccessfulUpdateMetrics(
    int attempt_count,
    int /* updates_abandoned_count */,
    PayloadType payload_type,
    int64_t payload_size,
    int64_t num_bytes_downloaded[kNumDownloadSources],
    int download_overhead_percentage,
    base::TimeDelta total_duration,
    base::TimeDelta /* total_duration_uptime */,
    int reboot_count,
    int /* url_switch_count */) {
  LogHistogram(metrics::kMetricsUpdateEngineSuccessfulUpdateAttemptCount,
               attempt_count);
  LogHistogram(metrics::kMetricsUpdateEngineSuccessfulUpdatePayloadType,
               static_cast<int>(payload_type));

  int64_t payload_size_mib = payload_size / kNumBytesInOneMiB;
  LogHistogram(metrics::kMetricsUpdateEngineSuccessfulUpdatePayloadSizeMiB,
               payload_size_mib);

  int64_t total_bytes_downloaded = 0;
  for (size_t i = 0; i < kNumDownloadSources; i++) {
    total_bytes_downloaded += num_bytes_downloaded[i] / kNumBytesInOneMiB;
  }
  LogHistogram(
      metrics::kMetricsUpdateEngineSuccessfulUpdateTotalBytesDownloadedMiB,
      total_bytes_downloaded);
  LogHistogram(
      metrics::kMetricsUpdateEngineSuccessfulUpdateDownloadOverheadPercentage,
      download_overhead_percentage);

  LogHistogram(
      metrics::kMetricsUpdateEngineSuccessfulUpdateTotalDurationInMinutes,
      total_duration.InMinutes());
  LogHistogram(metrics::kMetricsUpdateEngineSuccessfulUpdateRebootCount,
               reboot_count);
}

void MetricsReporterAndroid::ReportAbnormallyTerminatedUpdateAttemptMetrics() {
  int attempt_result =
      static_cast<int>(metrics::AttemptResult::kAbnormalTermination);
  LogHistogram(metrics::kMetricsUpdateEngineAttemptResult, attempt_result);
}

};  // namespace chromeos_update_engine
