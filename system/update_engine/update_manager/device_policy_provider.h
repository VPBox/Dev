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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_DEVICE_POLICY_PROVIDER_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_DEVICE_POLICY_PROVIDER_H_

#include <set>
#include <string>

#include <base/time/time.h>
#include <policy/libpolicy.h>

#include "update_engine/update_manager/provider.h"
#include "update_engine/update_manager/rollback_prefs.h"
#include "update_engine/update_manager/shill_provider.h"
#include "update_engine/update_manager/variable.h"
#include "update_engine/update_manager/weekly_time.h"

namespace chromeos_update_manager {

// Provides access to the current DevicePolicy.
class DevicePolicyProvider : public Provider {
 public:
  ~DevicePolicyProvider() override {}

  // Variable stating whether the DevicePolicy was loaded.
  virtual Variable<bool>* var_device_policy_is_loaded() = 0;

  // Variables mapping the information received on the DevicePolicy protobuf.
  virtual Variable<std::string>* var_release_channel() = 0;

  virtual Variable<bool>* var_release_channel_delegated() = 0;

  virtual Variable<bool>* var_update_disabled() = 0;

  virtual Variable<std::string>* var_target_version_prefix() = 0;

  // Variable returning what should happen if the target_version_prefix is
  // earlier than the current Chrome OS version.
  virtual Variable<RollbackToTargetVersion>*
  var_rollback_to_target_version() = 0;

  // Variable returning the number of Chrome milestones rollback should be
  // possible. Rollback protection will be postponed by this many versions.
  virtual Variable<int>* var_rollback_allowed_milestones() = 0;

  // Returns a non-negative scatter interval used for updates.
  virtual Variable<base::TimeDelta>* var_scatter_factor() = 0;

  // Variable returning the set of connection types allowed for updates. The
  // identifiers returned are consistent with the ones returned by the
  // ShillProvider.
  virtual Variable<std::set<chromeos_update_engine::ConnectionType>>*
  var_allowed_connection_types_for_update() = 0;

  // Variable stating the name of the device owner. For enterprise enrolled
  // devices, this will be an empty string.
  virtual Variable<std::string>* var_owner() = 0;

  virtual Variable<bool>* var_http_downloads_enabled() = 0;

  virtual Variable<bool>* var_au_p2p_enabled() = 0;

  virtual Variable<bool>* var_allow_kiosk_app_control_chrome_version() = 0;

  // Variable that contains the app that is to be run when launched in kiosk
  // mode. If the device is not in kiosk-mode this should be empty.
  virtual Variable<std::string>* var_auto_launched_kiosk_app_id() = 0;

  // Variable that contains the time intervals during the week for which update
  // checks are disallowed.
  virtual Variable<WeeklyTimeIntervalVector>*
  var_disallowed_time_intervals() = 0;

 protected:
  DevicePolicyProvider() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(DevicePolicyProvider);
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_DEVICE_POLICY_PROVIDER_H_
