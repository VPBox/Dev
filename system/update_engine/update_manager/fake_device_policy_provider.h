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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_FAKE_DEVICE_POLICY_PROVIDER_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_FAKE_DEVICE_POLICY_PROVIDER_H_

#include <set>
#include <string>

#include "update_engine/update_manager/device_policy_provider.h"
#include "update_engine/update_manager/fake_variable.h"

namespace chromeos_update_manager {

// Fake implementation of the DevicePolicyProvider base class.
class FakeDevicePolicyProvider : public DevicePolicyProvider {
 public:
  FakeDevicePolicyProvider() {}

  FakeVariable<bool>* var_device_policy_is_loaded() override {
    return &var_device_policy_is_loaded_;
  }

  FakeVariable<std::string>* var_release_channel() override {
    return &var_release_channel_;
  }

  FakeVariable<bool>* var_release_channel_delegated() override {
    return &var_release_channel_delegated_;
  }

  FakeVariable<bool>* var_update_disabled() override {
    return &var_update_disabled_;
  }

  FakeVariable<std::string>* var_target_version_prefix() override {
    return &var_target_version_prefix_;
  }

  FakeVariable<RollbackToTargetVersion>* var_rollback_to_target_version()
      override {
    return &var_rollback_to_target_version_;
  }

  FakeVariable<int>* var_rollback_allowed_milestones() override {
    return &var_rollback_allowed_milestones_;
  }

  FakeVariable<base::TimeDelta>* var_scatter_factor() override {
    return &var_scatter_factor_;
  }

  FakeVariable<std::set<chromeos_update_engine::ConnectionType>>*
  var_allowed_connection_types_for_update() override {
    return &var_allowed_connection_types_for_update_;
  }

  FakeVariable<std::string>* var_owner() override { return &var_owner_; }

  FakeVariable<bool>* var_http_downloads_enabled() override {
    return &var_http_downloads_enabled_;
  }

  FakeVariable<bool>* var_au_p2p_enabled() override {
    return &var_au_p2p_enabled_;
  }

  FakeVariable<bool>* var_allow_kiosk_app_control_chrome_version() override {
    return &var_allow_kiosk_app_control_chrome_version_;
  }

  FakeVariable<std::string>* var_auto_launched_kiosk_app_id() override {
    return &var_auto_launched_kiosk_app_id_;
  }

  FakeVariable<WeeklyTimeIntervalVector>* var_disallowed_time_intervals()
      override {
    return &var_disallowed_time_intervals_;
  }

 private:
  FakeVariable<bool> var_device_policy_is_loaded_{"policy_is_loaded",
                                                  kVariableModePoll};
  FakeVariable<std::string> var_release_channel_{"release_channel",
                                                 kVariableModePoll};
  FakeVariable<bool> var_release_channel_delegated_{"release_channel_delegated",
                                                    kVariableModePoll};
  FakeVariable<bool> var_update_disabled_{"update_disabled", kVariableModePoll};
  FakeVariable<std::string> var_target_version_prefix_{"target_version_prefix",
                                                       kVariableModePoll};
  FakeVariable<RollbackToTargetVersion> var_rollback_to_target_version_{
      "rollback_to_target_version", kVariableModePoll};
  FakeVariable<int> var_rollback_allowed_milestones_{
      "rollback_allowed_milestones", kVariableModePoll};
  FakeVariable<base::TimeDelta> var_scatter_factor_{"scatter_factor",
                                                    kVariableModePoll};
  FakeVariable<std::set<chromeos_update_engine::ConnectionType>>
      var_allowed_connection_types_for_update_{
          "allowed_connection_types_for_update", kVariableModePoll};
  FakeVariable<std::string> var_owner_{"owner", kVariableModePoll};
  FakeVariable<bool> var_http_downloads_enabled_{"http_downloads_enabled",
                                                 kVariableModePoll};
  FakeVariable<bool> var_au_p2p_enabled_{"au_p2p_enabled", kVariableModePoll};
  FakeVariable<bool> var_allow_kiosk_app_control_chrome_version_{
      "allow_kiosk_app_control_chrome_version", kVariableModePoll};
  FakeVariable<std::string> var_auto_launched_kiosk_app_id_{
      "auto_launched_kiosk_app_id", kVariableModePoll};
  FakeVariable<WeeklyTimeIntervalVector> var_disallowed_time_intervals_{
      "disallowed_time_intervals", kVariableModePoll};

  DISALLOW_COPY_AND_ASSIGN(FakeDevicePolicyProvider);
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_FAKE_DEVICE_POLICY_PROVIDER_H_
