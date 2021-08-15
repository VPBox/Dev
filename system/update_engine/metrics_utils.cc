//
// Copyright (C) 2015 The Android Open Source Project
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

#include "update_engine/metrics_utils.h"

#include <string>

#include <base/time/time.h>

#include "update_engine/common/clock_interface.h"
#include "update_engine/common/constants.h"
#include "update_engine/common/utils.h"
#include "update_engine/system_state.h"

using base::Time;
using base::TimeDelta;

namespace chromeos_update_engine {
namespace metrics_utils {

metrics::AttemptResult GetAttemptResult(ErrorCode code) {
  ErrorCode base_code = static_cast<ErrorCode>(
      static_cast<int>(code) & ~static_cast<int>(ErrorCode::kSpecialFlags));

  switch (base_code) {
    case ErrorCode::kSuccess:
      return metrics::AttemptResult::kUpdateSucceeded;

    case ErrorCode::kUpdatedButNotActive:
      return metrics::AttemptResult::kUpdateSucceededNotActive;

    case ErrorCode::kDownloadTransferError:
      return metrics::AttemptResult::kPayloadDownloadError;

    case ErrorCode::kDownloadInvalidMetadataSize:
    case ErrorCode::kDownloadInvalidMetadataMagicString:
    case ErrorCode::kDownloadMetadataSignatureError:
    case ErrorCode::kDownloadMetadataSignatureVerificationError:
    case ErrorCode::kPayloadMismatchedType:
    case ErrorCode::kUnsupportedMajorPayloadVersion:
    case ErrorCode::kUnsupportedMinorPayloadVersion:
    case ErrorCode::kDownloadNewPartitionInfoError:
    case ErrorCode::kDownloadSignatureMissingInManifest:
    case ErrorCode::kDownloadManifestParseError:
    case ErrorCode::kDownloadOperationHashMissingError:
      return metrics::AttemptResult::kMetadataMalformed;

    case ErrorCode::kDownloadOperationHashMismatch:
    case ErrorCode::kDownloadOperationHashVerificationError:
      return metrics::AttemptResult::kOperationMalformed;

    case ErrorCode::kDownloadOperationExecutionError:
    case ErrorCode::kInstallDeviceOpenError:
    case ErrorCode::kKernelDeviceOpenError:
    case ErrorCode::kDownloadWriteError:
    case ErrorCode::kFilesystemCopierError:
    case ErrorCode::kFilesystemVerifierError:
    case ErrorCode::kVerityCalculationError:
      return metrics::AttemptResult::kOperationExecutionError;

    case ErrorCode::kDownloadMetadataSignatureMismatch:
      return metrics::AttemptResult::kMetadataVerificationFailed;

    case ErrorCode::kPayloadSizeMismatchError:
    case ErrorCode::kPayloadHashMismatchError:
    case ErrorCode::kDownloadPayloadVerificationError:
    case ErrorCode::kSignedDeltaPayloadExpectedError:
    case ErrorCode::kDownloadPayloadPubKeyVerificationError:
    case ErrorCode::kPayloadTimestampError:
      return metrics::AttemptResult::kPayloadVerificationFailed;

    case ErrorCode::kNewRootfsVerificationError:
    case ErrorCode::kNewKernelVerificationError:
    case ErrorCode::kRollbackNotPossible:
      return metrics::AttemptResult::kVerificationFailed;

    case ErrorCode::kPostinstallRunnerError:
    case ErrorCode::kPostinstallBootedFromFirmwareB:
    case ErrorCode::kPostinstallFirmwareRONotUpdatable:
      return metrics::AttemptResult::kPostInstallFailed;

    case ErrorCode::kUserCanceled:
      return metrics::AttemptResult::kUpdateCanceled;

    // We should never get these errors in the update-attempt stage so
    // return internal error if this happens.
    case ErrorCode::kError:
    case ErrorCode::kOmahaRequestXMLParseError:
    case ErrorCode::kOmahaRequestError:
    case ErrorCode::kOmahaResponseHandlerError:
    case ErrorCode::kDownloadStateInitializationError:
    case ErrorCode::kOmahaRequestEmptyResponseError:
    case ErrorCode::kDownloadInvalidMetadataSignature:
    case ErrorCode::kOmahaResponseInvalid:
    case ErrorCode::kOmahaUpdateIgnoredPerPolicy:
    // TODO(deymo): The next two items belong in their own category; they
    // should not be counted as internal errors. b/27112092
    case ErrorCode::kOmahaUpdateDeferredPerPolicy:
    case ErrorCode::kNonCriticalUpdateInOOBE:
    case ErrorCode::kOmahaErrorInHTTPResponse:
    case ErrorCode::kDownloadMetadataSignatureMissingError:
    case ErrorCode::kOmahaUpdateDeferredForBackoff:
    case ErrorCode::kPostinstallPowerwashError:
    case ErrorCode::kUpdateCanceledByChannelChange:
    case ErrorCode::kOmahaRequestXMLHasEntityDecl:
    case ErrorCode::kOmahaUpdateIgnoredOverCellular:
    case ErrorCode::kNoUpdate:
    case ErrorCode::kFirstActiveOmahaPingSentPersistenceError:
      return metrics::AttemptResult::kInternalError;

    // Special flags. These can't happen (we mask them out above) but
    // the compiler doesn't know that. Just break out so we can warn and
    // return |kInternalError|.
    case ErrorCode::kUmaReportedMax:
    case ErrorCode::kOmahaRequestHTTPResponseBase:
    case ErrorCode::kDevModeFlag:
    case ErrorCode::kResumedFlag:
    case ErrorCode::kTestImageFlag:
    case ErrorCode::kTestOmahaUrlFlag:
    case ErrorCode::kSpecialFlags:
      break;
  }

  LOG(ERROR) << "Unexpected error code " << base_code;
  return metrics::AttemptResult::kInternalError;
}

metrics::DownloadErrorCode GetDownloadErrorCode(ErrorCode code) {
  ErrorCode base_code = static_cast<ErrorCode>(
      static_cast<int>(code) & ~static_cast<int>(ErrorCode::kSpecialFlags));

  if (base_code >= ErrorCode::kOmahaRequestHTTPResponseBase) {
    int http_status =
        static_cast<int>(base_code) -
        static_cast<int>(ErrorCode::kOmahaRequestHTTPResponseBase);
    if (http_status >= 200 && http_status <= 599) {
      return static_cast<metrics::DownloadErrorCode>(
          static_cast<int>(metrics::DownloadErrorCode::kHttpStatus200) +
          http_status - 200);
    } else if (http_status == 0) {
      // The code is using HTTP Status 0 for "Unable to get http
      // response code."
      return metrics::DownloadErrorCode::kDownloadError;
    }
    LOG(WARNING) << "Unexpected HTTP status code " << http_status;
    return metrics::DownloadErrorCode::kHttpStatusOther;
  }

  switch (base_code) {
    // Unfortunately, ErrorCode::kDownloadTransferError is returned for a wide
    // variety of errors (proxy errors, host not reachable, timeouts etc.).
    //
    // For now just map that to kDownloading. See http://crbug.com/355745
    // for how we plan to add more detail in the future.
    case ErrorCode::kDownloadTransferError:
      return metrics::DownloadErrorCode::kDownloadError;

    // All of these error codes are not related to downloading so break
    // out so we can warn and return InputMalformed.
    case ErrorCode::kSuccess:
    case ErrorCode::kError:
    case ErrorCode::kOmahaRequestError:
    case ErrorCode::kOmahaResponseHandlerError:
    case ErrorCode::kFilesystemCopierError:
    case ErrorCode::kPostinstallRunnerError:
    case ErrorCode::kPayloadMismatchedType:
    case ErrorCode::kInstallDeviceOpenError:
    case ErrorCode::kKernelDeviceOpenError:
    case ErrorCode::kPayloadHashMismatchError:
    case ErrorCode::kPayloadSizeMismatchError:
    case ErrorCode::kDownloadPayloadVerificationError:
    case ErrorCode::kDownloadNewPartitionInfoError:
    case ErrorCode::kDownloadWriteError:
    case ErrorCode::kNewRootfsVerificationError:
    case ErrorCode::kNewKernelVerificationError:
    case ErrorCode::kSignedDeltaPayloadExpectedError:
    case ErrorCode::kDownloadPayloadPubKeyVerificationError:
    case ErrorCode::kPostinstallBootedFromFirmwareB:
    case ErrorCode::kDownloadStateInitializationError:
    case ErrorCode::kDownloadInvalidMetadataMagicString:
    case ErrorCode::kDownloadSignatureMissingInManifest:
    case ErrorCode::kDownloadManifestParseError:
    case ErrorCode::kDownloadMetadataSignatureError:
    case ErrorCode::kDownloadMetadataSignatureVerificationError:
    case ErrorCode::kDownloadMetadataSignatureMismatch:
    case ErrorCode::kDownloadOperationHashVerificationError:
    case ErrorCode::kDownloadOperationExecutionError:
    case ErrorCode::kDownloadOperationHashMismatch:
    case ErrorCode::kOmahaRequestEmptyResponseError:
    case ErrorCode::kOmahaRequestXMLParseError:
    case ErrorCode::kDownloadInvalidMetadataSize:
    case ErrorCode::kDownloadInvalidMetadataSignature:
    case ErrorCode::kOmahaResponseInvalid:
    case ErrorCode::kOmahaUpdateIgnoredPerPolicy:
    case ErrorCode::kOmahaUpdateDeferredPerPolicy:
    case ErrorCode::kNonCriticalUpdateInOOBE:
    case ErrorCode::kOmahaErrorInHTTPResponse:
    case ErrorCode::kDownloadOperationHashMissingError:
    case ErrorCode::kDownloadMetadataSignatureMissingError:
    case ErrorCode::kOmahaUpdateDeferredForBackoff:
    case ErrorCode::kPostinstallPowerwashError:
    case ErrorCode::kUpdateCanceledByChannelChange:
    case ErrorCode::kPostinstallFirmwareRONotUpdatable:
    case ErrorCode::kUnsupportedMajorPayloadVersion:
    case ErrorCode::kUnsupportedMinorPayloadVersion:
    case ErrorCode::kOmahaRequestXMLHasEntityDecl:
    case ErrorCode::kFilesystemVerifierError:
    case ErrorCode::kUserCanceled:
    case ErrorCode::kOmahaUpdateIgnoredOverCellular:
    case ErrorCode::kPayloadTimestampError:
    case ErrorCode::kUpdatedButNotActive:
    case ErrorCode::kNoUpdate:
    case ErrorCode::kRollbackNotPossible:
    case ErrorCode::kFirstActiveOmahaPingSentPersistenceError:
    case ErrorCode::kVerityCalculationError:
      break;

    // Special flags. These can't happen (we mask them out above) but
    // the compiler doesn't know that. Just break out so we can warn and
    // return |kInputMalformed|.
    case ErrorCode::kUmaReportedMax:
    case ErrorCode::kOmahaRequestHTTPResponseBase:
    case ErrorCode::kDevModeFlag:
    case ErrorCode::kResumedFlag:
    case ErrorCode::kTestImageFlag:
    case ErrorCode::kTestOmahaUrlFlag:
    case ErrorCode::kSpecialFlags:
      LOG(ERROR) << "Unexpected error code " << base_code;
      break;
  }

  return metrics::DownloadErrorCode::kInputMalformed;
}

metrics::ConnectionType GetConnectionType(ConnectionType type,
                                          ConnectionTethering tethering) {
  switch (type) {
    case ConnectionType::kUnknown:
      return metrics::ConnectionType::kUnknown;

    case ConnectionType::kDisconnected:
      return metrics::ConnectionType::kDisconnected;

    case ConnectionType::kEthernet:
      if (tethering == ConnectionTethering::kConfirmed)
        return metrics::ConnectionType::kTetheredEthernet;
      else
        return metrics::ConnectionType::kEthernet;

    case ConnectionType::kWifi:
      if (tethering == ConnectionTethering::kConfirmed)
        return metrics::ConnectionType::kTetheredWifi;
      else
        return metrics::ConnectionType::kWifi;

    case ConnectionType::kWimax:
      return metrics::ConnectionType::kWimax;

    case ConnectionType::kBluetooth:
      return metrics::ConnectionType::kBluetooth;

    case ConnectionType::kCellular:
      return metrics::ConnectionType::kCellular;
  }

  LOG(ERROR) << "Unexpected network connection type: type="
             << static_cast<int>(type)
             << ", tethering=" << static_cast<int>(tethering);

  return metrics::ConnectionType::kUnknown;
}

bool WallclockDurationHelper(SystemState* system_state,
                             const std::string& state_variable_key,
                             TimeDelta* out_duration) {
  bool ret = false;

  Time now = system_state->clock()->GetWallclockTime();
  int64_t stored_value;
  if (system_state->prefs()->GetInt64(state_variable_key, &stored_value)) {
    Time stored_time = Time::FromInternalValue(stored_value);
    if (stored_time > now) {
      LOG(ERROR) << "Stored time-stamp used for " << state_variable_key
                 << " is in the future.";
    } else {
      *out_duration = now - stored_time;
      ret = true;
    }
  }

  if (!system_state->prefs()->SetInt64(state_variable_key,
                                       now.ToInternalValue())) {
    LOG(ERROR) << "Error storing time-stamp in " << state_variable_key;
  }

  return ret;
}

bool MonotonicDurationHelper(SystemState* system_state,
                             int64_t* storage,
                             TimeDelta* out_duration) {
  bool ret = false;

  Time now = system_state->clock()->GetMonotonicTime();
  if (*storage != 0) {
    Time stored_time = Time::FromInternalValue(*storage);
    *out_duration = now - stored_time;
    ret = true;
  }
  *storage = now.ToInternalValue();

  return ret;
}

int64_t GetPersistedValue(const std::string& key, PrefsInterface* prefs) {
  CHECK(prefs);
  if (!prefs->Exists(key))
    return 0;

  int64_t stored_value;
  if (!prefs->GetInt64(key, &stored_value))
    return 0;

  if (stored_value < 0) {
    LOG(ERROR) << key << ": Invalid value (" << stored_value
               << ") in persisted state. Defaulting to 0";
    return 0;
  }

  return stored_value;
}

void SetNumReboots(int64_t num_reboots, PrefsInterface* prefs) {
  CHECK(prefs);
  prefs->SetInt64(kPrefsNumReboots, num_reboots);
  LOG(INFO) << "Number of Reboots during current update attempt = "
            << num_reboots;
}

void SetPayloadAttemptNumber(int64_t payload_attempt_number,
                             PrefsInterface* prefs) {
  CHECK(prefs);
  prefs->SetInt64(kPrefsPayloadAttemptNumber, payload_attempt_number);
  LOG(INFO) << "Payload Attempt Number = " << payload_attempt_number;
}

void SetSystemUpdatedMarker(ClockInterface* clock, PrefsInterface* prefs) {
  CHECK(prefs);
  CHECK(clock);
  Time update_finish_time = clock->GetMonotonicTime();
  prefs->SetInt64(kPrefsSystemUpdatedMarker,
                  update_finish_time.ToInternalValue());
  LOG(INFO) << "Updated Marker = " << utils::ToString(update_finish_time);
}

void SetUpdateTimestampStart(const Time& update_start_time,
                             PrefsInterface* prefs) {
  CHECK(prefs);
  prefs->SetInt64(kPrefsUpdateTimestampStart,
                  update_start_time.ToInternalValue());
  LOG(INFO) << "Update Monotonic Timestamp Start = "
            << utils::ToString(update_start_time);
}

void SetUpdateBootTimestampStart(const base::Time& update_start_boot_time,
                                 PrefsInterface* prefs) {
  CHECK(prefs);
  prefs->SetInt64(kPrefsUpdateBootTimestampStart,
                  update_start_boot_time.ToInternalValue());
  LOG(INFO) << "Update Boot Timestamp Start = "
            << utils::ToString(update_start_boot_time);
}

bool LoadAndReportTimeToReboot(MetricsReporterInterface* metrics_reporter,
                               PrefsInterface* prefs,
                               ClockInterface* clock) {
  CHECK(prefs);
  CHECK(clock);
  int64_t stored_value = GetPersistedValue(kPrefsSystemUpdatedMarker, prefs);
  if (stored_value == 0)
    return false;

  Time system_updated_at = Time::FromInternalValue(stored_value);
  base::TimeDelta time_to_reboot =
      clock->GetMonotonicTime() - system_updated_at;
  if (time_to_reboot.ToInternalValue() < 0) {
    LOG(ERROR) << "time_to_reboot is negative - system_updated_at: "
               << utils::ToString(system_updated_at);
    return false;
  }
  metrics_reporter->ReportTimeToReboot(time_to_reboot.InMinutes());
  return true;
}

}  // namespace metrics_utils
}  // namespace chromeos_update_engine
