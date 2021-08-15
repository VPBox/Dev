//
// Copyright (C) 2010 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_DBUS_SERVICE_H_
#define UPDATE_ENGINE_DBUS_SERVICE_H_

#include <inttypes.h>

#include <memory>
#include <string>

#include <base/memory/ref_counted.h>
#include <brillo/errors/error.h>

#include "update_engine/common_service.h"
#include "update_engine/service_observer_interface.h"
#include "update_engine/update_attempter.h"

#include "dbus_bindings/org.chromium.UpdateEngineInterface.h"

namespace chromeos_update_engine {

class DBusUpdateEngineService
    : public org::chromium::UpdateEngineInterfaceInterface {
 public:
  explicit DBusUpdateEngineService(SystemState* system_state);
  virtual ~DBusUpdateEngineService() = default;

  // Implementation of org::chromium::UpdateEngineInterfaceInterface.
  bool AttemptUpdate(brillo::ErrorPtr* error,
                     const std::string& in_app_version,
                     const std::string& in_omaha_url) override;

  bool AttemptUpdateWithFlags(brillo::ErrorPtr* error,
                              const std::string& in_app_version,
                              const std::string& in_omaha_url,
                              int32_t in_flags_as_int) override;

  bool AttemptInstall(brillo::ErrorPtr* error,
                      const std::string& dlc_request) override;

  bool AttemptRollback(brillo::ErrorPtr* error, bool in_powerwash) override;

  // Checks if the system rollback is available by verifying if the secondary
  // system partition is valid and bootable.
  bool CanRollback(brillo::ErrorPtr* error, bool* out_can_rollback) override;

  // Resets the status of the update_engine to idle, ignoring any applied
  // update. This is used for development only.
  bool ResetStatus(brillo::ErrorPtr* error) override;

  // Returns the current status of the Update Engine. If an update is in
  // progress, the number of operations, size to download and overall progress
  // is reported.
  bool GetStatus(brillo::ErrorPtr* error,
                 int64_t* out_last_checked_time,
                 double* out_progress,
                 std::string* out_current_operation,
                 std::string* out_new_version,
                 int64_t* out_new_size) override;

  // Reboots the device if an update is applied and a reboot is required.
  bool RebootIfNeeded(brillo::ErrorPtr* error) override;

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
                  bool in_is_powerwash_allowed) override;

  // If get_current_channel is set to true, populates |channel| with the name of
  // the channel that the device is currently on. Otherwise, it populates it
  // with the name of the channel the device is supposed to be (in case of a
  // pending channel change).
  bool GetChannel(brillo::ErrorPtr* error,
                  bool in_get_current_channel,
                  std::string* out_channel) override;

  bool SetCohortHint(brillo::ErrorPtr* error,
                     const std::string& in_cohort_hint) override;

  bool GetCohortHint(brillo::ErrorPtr* error,
                     std::string* out_cohort_hint) override;

  // Enables or disables the sharing and consuming updates over P2P feature
  // according to the |enabled| argument passed.
  bool SetP2PUpdatePermission(brillo::ErrorPtr* error,
                              bool in_enabled) override;

  // Returns the current value for the P2P enabled setting. This involves both
  // sharing and consuming updates over P2P.
  bool GetP2PUpdatePermission(brillo::ErrorPtr* error,
                              bool* out_enabled) override;

  // If there's no device policy installed, sets the update over cellular
  // networks permission to the |allowed| value. Otherwise, this method returns
  // with an error since this setting is overridden by the applied policy.
  bool SetUpdateOverCellularPermission(brillo::ErrorPtr* error,
                                       bool in_allowed) override;

  // If there's no device policy installed, sets the update over cellular
  // target. Otherwise, this method returns with an error.
  bool SetUpdateOverCellularTarget(brillo::ErrorPtr* error,
                                   const std::string& target_version,
                                   int64_t target_size) override;

  // Returns the current value of the update over cellular network setting,
  // either forced by the device policy if the device is enrolled or the current
  // user preference otherwise.
  bool GetUpdateOverCellularPermission(brillo::ErrorPtr* error,
                                       bool* out_allowed) override;

  // Returns the duration since the last successful update, as the
  // duration on the wallclock. Returns an error if the device has not
  // updated.
  bool GetDurationSinceUpdate(brillo::ErrorPtr* error,
                              int64_t* out_usec_wallclock) override;

  // Returns the version string of OS that was used before the last reboot
  // into an updated version. This is available only when rebooting into an
  // update from previous version, otherwise an empty string is returned.
  bool GetPrevVersion(brillo::ErrorPtr* error,
                      std::string* out_prev_version) override;

  // Returns the name of kernel partition that can be rolled back into.
  bool GetRollbackPartition(brillo::ErrorPtr* error,
                            std::string* out_rollback_partition_name) override;

  // Returns the last UpdateAttempt error. If not updated yet, default success
  // ErrorCode will be returned.
  bool GetLastAttemptError(brillo::ErrorPtr* error,
                           int32_t* out_last_attempt_error) override;

  // Returns the current end-of-life status of the device in |out_eol_status|.
  bool GetEolStatus(brillo::ErrorPtr* error, int32_t* out_eol_status) override;

 private:
  std::unique_ptr<UpdateEngineService> common_;
};

// The UpdateEngineAdaptor class runs the UpdateEngineInterface in the fixed
// object path, without an ObjectManager notifying the interfaces, since it is
// all static and clients don't expect it to be implemented.
class UpdateEngineAdaptor : public org::chromium::UpdateEngineInterfaceAdaptor,
                            public ServiceObserverInterface {
 public:
  explicit UpdateEngineAdaptor(SystemState* system_state);
  ~UpdateEngineAdaptor() = default;

  // Register the DBus object with the update engine service asynchronously.
  // Calls |copmletion_callback| when done passing a boolean indicating if the
  // registration succeeded.
  void RegisterAsync(const base::Callback<void(bool)>& completion_callback);

  // Takes ownership of the well-known DBus name and returns whether it
  // succeeded.
  bool RequestOwnership();

  // ServiceObserverInterface overrides.
  void SendStatusUpdate(
      const update_engine::UpdateEngineStatus& update_engine_status) override;

  void SendPayloadApplicationComplete(ErrorCode error_code) override {}

 private:
  scoped_refptr<dbus::Bus> bus_;
  DBusUpdateEngineService dbus_service_;
  brillo::dbus_utils::DBusObject dbus_object_;
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_DBUS_SERVICE_H_
