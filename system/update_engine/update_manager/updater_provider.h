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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_UPDATER_PROVIDER_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_UPDATER_PROVIDER_H_

#include <string>

#include <base/time/time.h>

#include "update_engine/update_manager/provider.h"
#include "update_engine/update_manager/variable.h"

namespace chromeos_update_manager {

enum class Stage {
  kIdle,
  kCheckingForUpdate,
  kUpdateAvailable,
  kDownloading,
  kVerifying,
  kFinalizing,
  kUpdatedNeedReboot,
  kReportingErrorEvent,
  kAttemptingRollback,
};

enum class UpdateRequestStatus {
  kNone,
  kInteractive,
  kPeriodic,
};

// These enum values are a bit-field.
enum UpdateRestrictions : int {
  kNone,
  kRestrictDownloading = (1 << 0),
};

// Provider for Chrome OS update related information.
class UpdaterProvider : public Provider {
 public:
  ~UpdaterProvider() override {}

  // A variable returning the timestamp when the update engine was started in
  // wallclock time.
  virtual Variable<base::Time>* var_updater_started_time() = 0;

  // A variable returning the last update check time.
  virtual Variable<base::Time>* var_last_checked_time() = 0;

  // A variable reporting the time when an update was last completed in the
  // current boot cycle. Returns an error if an update completed time could not
  // be read (e.g. no update was completed in the current boot cycle) or is
  // invalid.
  //
  // IMPORTANT: The time reported is not the wallclock time reading at the time
  // of the update, rather it is the point in time when the update completed
  // relative to the current wallclock time reading. Therefore, the gap between
  // the reported value and the current wallclock time is guaranteed to be
  // monotonically increasing.
  virtual Variable<base::Time>* var_update_completed_time() = 0;

  // A variable returning the update progress (0.0 to 1.0).
  virtual Variable<double>* var_progress() = 0;

  // A variable returning the current update status.
  virtual Variable<Stage>* var_stage() = 0;

  // A variable returning the update target version.
  virtual Variable<std::string>* var_new_version() = 0;

  // A variable returning the update payload size. The payload size is
  // guaranteed to be non-negative.
  virtual Variable<uint64_t>* var_payload_size() = 0;

  // A variable returning the current channel.
  virtual Variable<std::string>* var_curr_channel() = 0;

  // A variable returning the update target channel.
  virtual Variable<std::string>* var_new_channel() = 0;

  // A variable indicating whether user settings allow P2P updates.
  virtual Variable<bool>* var_p2p_enabled() = 0;

  // A variable indicating whether user settings allow updates over a cellular
  // network.
  virtual Variable<bool>* var_cellular_enabled() = 0;

  // A variable returning the number of consecutive failed update checks.
  virtual Variable<unsigned int>* var_consecutive_failed_update_checks() = 0;

  // A server-dictated update check interval in seconds, if one was given.
  virtual Variable<unsigned int>* var_server_dictated_poll_interval() = 0;

  // A variable denoting whether a forced update was request but no update check
  // performed yet; also tells whether this request is for an interactive or
  // scheduled update.
  virtual Variable<UpdateRequestStatus>* var_forced_update_requested() = 0;

  // A variable that returns the update restriction flags that are set
  // for all updates.
  virtual Variable<UpdateRestrictions>* var_update_restrictions() = 0;

 protected:
  UpdaterProvider() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(UpdaterProvider);
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_UPDATER_PROVIDER_H_
