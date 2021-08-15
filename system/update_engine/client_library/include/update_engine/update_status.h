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

#ifndef UPDATE_ENGINE_CLIENT_LIBRARY_INCLUDE_UPDATE_ENGINE_UPDATE_STATUS_H_
#define UPDATE_ENGINE_CLIENT_LIBRARY_INCLUDE_UPDATE_ENGINE_UPDATE_STATUS_H_

#include <string>

#include <brillo/enum_flags.h>

namespace update_engine {

// ATTENTION: When adding a new enum value here, always append at the end and
// make sure to make proper adjustments in UpdateAttempter:ActionCompleted(). If
// any enum memeber is deprecated, the assigned value of other members should
// not change. See b/62842358.
enum class UpdateStatus {
  IDLE = 0,
  CHECKING_FOR_UPDATE = 1,
  UPDATE_AVAILABLE = 2,
  DOWNLOADING = 3,
  VERIFYING = 4,
  FINALIZING = 5,
  UPDATED_NEED_REBOOT = 6,
  REPORTING_ERROR_EVENT = 7,
  ATTEMPTING_ROLLBACK = 8,
  DISABLED = 9,
  // Broadcast this state when an update aborts because user preferences do not
  // allow updates, e.g. over cellular network.
  NEED_PERMISSION_TO_UPDATE = 10,
};

// Enum of bit-wise flags for controlling how updates are attempted.
enum UpdateAttemptFlags : int32_t {
  kNone = 0,
  // Treat the update like a non-interactive update, even when being triggered
  // by the interactive APIs.
  kFlagNonInteractive = (1 << 0),
  // Restrict (disallow) downloading of updates.
  kFlagRestrictDownload = (1 << 1),
};

// Enable bit-wise operators for the above enumeration of flag values.
DECLARE_FLAGS_ENUM(UpdateAttemptFlags);

struct UpdateEngineStatus {
  // When the update_engine last checked for updates (time_t: seconds from unix
  // epoch)
  int64_t last_checked_time;
  // the current status/operation of the update_engine
  UpdateStatus status;
  // the current product version (oem bundle id)
  std::string current_version;
  // the current system version
  std::string current_system_version;
  // The current progress (0.0f-1.0f).
  double progress;
  // the size of the update (bytes)
  uint64_t new_size_bytes;
  // the new product version
  std::string new_version;
  // the new system version, if there is one (empty, otherwise)
  std::string new_system_version;
};

}  // namespace update_engine

#endif  // UPDATE_ENGINE_CLIENT_LIBRARY_INCLUDE_UPDATE_ENGINE_UPDATE_STATUS_H_
