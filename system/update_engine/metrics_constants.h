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

#ifndef UPDATE_ENGINE_METRICS_CONSTANTS_H_
#define UPDATE_ENGINE_METRICS_CONSTANTS_H_

namespace chromeos_update_engine {

namespace metrics {
// The possible outcomes when checking for updates.
//
// This is used in the UpdateEngine.Check.Result histogram.
enum class CheckResult {
  kUpdateAvailable,    // Response indicates an update is available.
  kNoUpdateAvailable,  // Response indicates no updates are available.
  kDownloadError,      // Error downloading response from Omaha.
  kParsingError,       // Error parsing response.
  kRebootPending,      // No update check was performed a reboot is pending.

  kNumConstants,
  kUnset = -1
};

// Possible ways a device can react to a new update being available.
//
// This is used in the UpdateEngine.Check.Reaction histogram.
enum class CheckReaction {
  kUpdating,    // Device proceeds to download and apply update.
  kIgnored,     // Device-policy dictates ignoring the update.
  kDeferring,   // Device-policy dictates waiting.
  kBackingOff,  // Previous errors dictates waiting.

  kNumConstants,
  kUnset = -1
};

// The possible ways that downloading from a HTTP or HTTPS server can fail.
//
// This is used in the UpdateEngine.Check.DownloadErrorCode and
// UpdateEngine.Attempt.DownloadErrorCode histograms.
enum class DownloadErrorCode {
  // Errors that can happen in the field. See http://crbug.com/355745
  // for how we plan to add more detail in the future.
  kDownloadError = 0,  // Error downloading data from server.

  // IMPORTANT: When adding a new error code, add at the bottom of the
  // above block and before the kInputMalformed field. This
  // is to ensure that error codes are not reordered.

  // This error code is used to convey that malformed input was given
  // to the utils::GetDownloadErrorCode() function. This should never
  // happen but if it does it's because of an internal update_engine
  // error and we're interested in knowing this.
  kInputMalformed = 100,

  // Bucket for capturing HTTP status codes not in the 200-599
  // range. This should never happen in practice but if it does we
  // want to know.
  kHttpStatusOther = 101,

  // Above 200 and below 600, the value is the HTTP status code.
  kHttpStatus200 = 200,

  kNumConstants = 600,

  kUnset = -1
};

// Possible ways an update attempt can end.
//
// This is used in the UpdateEngine.Attempt.Result histogram.
enum class AttemptResult {
  kUpdateSucceeded,             // The update succeeded.
  kInternalError,               // An internal error occurred.
  kPayloadDownloadError,        // Failure while downloading payload.
  kMetadataMalformed,           // Metadata was malformed.
  kOperationMalformed,          // An operation was malformed.
  kOperationExecutionError,     // An operation failed to execute.
  kMetadataVerificationFailed,  // Metadata verification failed.
  kPayloadVerificationFailed,   // Payload verification failed.
  kVerificationFailed,          // Root or Kernel partition verification failed.
  kPostInstallFailed,           // The postinstall step failed.
  kAbnormalTermination,         // The attempt ended abnormally.
  kUpdateCanceled,              // Update canceled by the user.
  kUpdateSucceededNotActive,    // Update succeeded but the new slot is not
                                // active.

  kNumConstants,

  kUnset = -1
};

// Possible ways the device is connected to the Internet.
//
// This is used in the UpdateEngine.Attempt.ConnectionType histogram.
enum class ConnectionType {
  kUnknown = 0,           // Unknown.
  kEthernet = 1,          // Ethernet.
  kWifi = 2,              // Wireless.
  kWimax = 3,             // WiMax.
  kBluetooth = 4,         // Bluetooth.
  kCellular = 5,          // Cellular.
  kTetheredEthernet = 6,  // Tethered (Ethernet).
  kTetheredWifi = 7,      // Tethered (Wifi).
  kDisconnected = 8,      // Disconnected.

  kNumConstants,
  kUnset = -1
};

// Possible ways a rollback can end.
//
// This is used in the UpdateEngine.Rollback histogram.
enum class RollbackResult {
  kFailed,
  kSuccess,

  kNumConstants
};

}  // namespace metrics

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_METRICS_CONSTANTS_H_
