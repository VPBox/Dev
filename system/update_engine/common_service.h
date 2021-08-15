//
// Copyright (C) 2016 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_COMMON_SERVICE_H_
#define UPDATE_ENGINE_COMMON_SERVICE_H_

#include <inttypes.h>

#include <string>
#include <vector>

#include <base/memory/ref_counted.h>
#include <brillo/errors/error.h>

#include "update_engine/client_library/include/update_engine/update_status.h"
#include "update_engine/system_state.h"

namespace chromeos_update_engine {

class UpdateEngineService {
 public:
  // Error domain for all the service errors.
  static const char* const kErrorDomain;

  // Generic service error.
  static const char* const kErrorFailed;

  explicit UpdateEngineService(SystemState* system_state);
  virtual ~UpdateEngineService() = default;

  // Set flags that influence how updates and checks are performed.  These
  // influence all future checks and updates until changed or the device
  // reboots.  The |in_flags_as_int| values are a union of values from
  // |UpdateAttemptFlags|
  bool SetUpdateAttemptFlags(brillo::ErrorPtr* error, int32_t in_flags_as_int);

  bool AttemptUpdate(brillo::ErrorPtr* error,
                     const std::string& in_app_version,
                     const std::string& in_omaha_url,
                     int32_t in_flags_as_int,
                     bool* out_result);

  // Attempts a DLC module install operation.
  // |omaha_url|: the URL to query for update.
  // |dlc_module_ids|: a list of DLC module IDs.
  bool AttemptInstall(brillo::ErrorPtr* error,
                      const std::string& omaha_url,
                      const std::vector<std::string>& dlc_module_ids);

  bool AttemptRollback(brillo::ErrorPtr* error, bool in_powerwash);

  // Checks if the system rollback is available by verifying if the secondary
  // system partition is valid and bootable.
  bool CanRollback(brillo::ErrorPtr* error, bool* out_can_rollback);

  // Resets the status of the update_engine to idle, ignoring any applied
  // update. This is used for development only.
  bool ResetStatus(brillo::ErrorPtr* error);

  // Returns the current status of the Update Engine. If an update is in
  // progress, the number of operations, size to download and overall progress
  // is reported.
  bool GetStatus(brillo::ErrorPtr* error,
                 update_engine::UpdateEngineStatus* out_status);

  // Reboots the device if an update is applied and a reboot is required.
  bool RebootIfNeeded(brillo::ErrorPtr* error);

  // Changes the current channel of the device to the target channel. If the
  // target channel is a less stable channel than the current channel, then the
  // channel change happens immediately (at the next update check).  If the
  // target channel is a more stable channel, then if is_powerwash_allowed is
  // set to true, then also the change happens immediately but with a powerwash
  // if required. Otherwise, the change takes effect eventually (when the
  // version on the target channel goes above the version number of what the
  // device currently has).
  bool SetChannel(brillo::ErrorPtr* error,
                  const std::string& in_target_channel,
                  bool in_is_powerwash_allowed);

  // If get_current_channel is set to true, populates |channel| with the name of
  // the channel that the device is currently on. Otherwise, it populates it
  // with the name of the channel the device is supposed to be (in case of a
  // pending channel change).
  bool GetChannel(brillo::ErrorPtr* error,
                  bool in_get_current_channel,
                  std::string* out_channel);

  // Sets the current "cohort hint" value to |in_cohort_hint|. The cohort hint
  // is sent back to Omaha on every request and can be used as a hint of what
  // cohort should we be put on.
  bool SetCohortHint(brillo::ErrorPtr* error, std::string in_cohort_hint);

  // Return the current cohort hint. This value can be set with SetCohortHint()
  // and can also be updated from Omaha on every update check request.
  bool GetCohortHint(brillo::ErrorPtr* error, std::string* out_cohort_hint);

  // Enables or disables the sharing and consuming updates over P2P feature
  // according to the |enabled| argument passed.
  bool SetP2PUpdatePermission(brillo::ErrorPtr* error, bool in_enabled);

  // Returns the current value for the P2P enabled setting. This involves both
  // sharing and consuming updates over P2P.
  bool GetP2PUpdatePermission(brillo::ErrorPtr* error, bool* out_enabled);

  // If there's no device policy installed, sets the update over cellular
  // networks permission to the |allowed| value. Otherwise, this method returns
  // with an error since this setting is overridden by the applied policy.
  bool SetUpdateOverCellularPermission(brillo::ErrorPtr* error,
                                       bool in_allowed);

  // If there's no device policy installed, sets the update over cellular
  // target. Otherwise, this method returns with an error.
  bool SetUpdateOverCellularTarget(brillo::ErrorPtr* error,
                                   const std::string& target_version,
                                   int64_t target_size);

  // Returns the current value of the update over cellular network setting,
  // either forced by the device policy if the device is enrolled or the current
  // user preference otherwise.
  bool GetUpdateOverCellularPermission(brillo::ErrorPtr* error,
                                       bool* out_allowed);

  // Returns the duration since the last successful update, as the
  // duration on the wallclock. Returns an error if the device has not
  // updated.
  bool GetDurationSinceUpdate(brillo::ErrorPtr* error,
                              int64_t* out_usec_wallclock);

  // Returns the version string of OS that was used before the last reboot
  // into an updated version. This is available only when rebooting into an
  // update from previous version, otherwise an empty string is returned.
  bool GetPrevVersion(brillo::ErrorPtr* error, std::string* out_prev_version);

  // Returns the name of kernel partition that can be rolled back into.
  bool GetRollbackPartition(brillo::ErrorPtr* error,
                            std::string* out_rollback_partition_name);

  // Returns the last UpdateAttempt error.
  bool GetLastAttemptError(brillo::ErrorPtr* error,
                           int32_t* out_last_attempt_error);

  // Returns the current end-of-life status of the device. This value is updated
  // on every update check and persisted on disk across reboots.
  bool GetEolStatus(brillo::ErrorPtr* error, int32_t* out_eol_status);

 private:
  SystemState* system_state_;
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_COMMON_SERVICE_H_
