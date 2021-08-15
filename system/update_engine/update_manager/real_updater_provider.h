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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_REAL_UPDATER_PROVIDER_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_REAL_UPDATER_PROVIDER_H_

#include <memory>
#include <string>

#include "update_engine/system_state.h"
#include "update_engine/update_manager/generic_variables.h"
#include "update_engine/update_manager/updater_provider.h"

namespace chromeos_update_manager {

// A concrete UpdaterProvider implementation using local (in-process) bindings.
class RealUpdaterProvider : public UpdaterProvider {
 public:
  // We assume that any other object handle we get from the system state is
  // "volatile", and so must be re-acquired whenever access is needed; this
  // guarantees that parts of the system state can be mocked out at any time
  // during testing. We further assume that, by the time Init() is called, the
  // system state object is fully populated and usable.
  explicit RealUpdaterProvider(
      chromeos_update_engine::SystemState* system_state);

  // Initializes the provider and returns whether it succeeded.
  bool Init() { return true; }

  Variable<base::Time>* var_updater_started_time() override {
    return &var_updater_started_time_;
  }

  Variable<base::Time>* var_last_checked_time() override {
    return var_last_checked_time_.get();
  }

  Variable<base::Time>* var_update_completed_time() override {
    return var_update_completed_time_.get();
  }

  Variable<double>* var_progress() override { return var_progress_.get(); }

  Variable<Stage>* var_stage() override { return var_stage_.get(); }

  Variable<std::string>* var_new_version() override {
    return var_new_version_.get();
  }

  Variable<uint64_t>* var_payload_size() override {
    return var_payload_size_.get();
  }

  Variable<std::string>* var_curr_channel() override {
    return var_curr_channel_.get();
  }

  Variable<std::string>* var_new_channel() override {
    return var_new_channel_.get();
  }

  Variable<bool>* var_p2p_enabled() override { return var_p2p_enabled_.get(); }

  Variable<bool>* var_cellular_enabled() override {
    return var_cellular_enabled_.get();
  }

  Variable<unsigned int>* var_consecutive_failed_update_checks() override {
    return var_consecutive_failed_update_checks_.get();
  }

  Variable<unsigned int>* var_server_dictated_poll_interval() override {
    return var_server_dictated_poll_interval_.get();
  }

  Variable<UpdateRequestStatus>* var_forced_update_requested() override {
    return var_forced_update_requested_.get();
  }

  Variable<UpdateRestrictions>* var_update_restrictions() override {
    return var_update_restrictions_.get();
  }

 private:
  // A pointer to the update engine's system state aggregator.
  chromeos_update_engine::SystemState* system_state_;

  // Variable implementations.
  ConstCopyVariable<base::Time> var_updater_started_time_;
  std::unique_ptr<Variable<base::Time>> var_last_checked_time_;
  std::unique_ptr<Variable<base::Time>> var_update_completed_time_;
  std::unique_ptr<Variable<double>> var_progress_;
  std::unique_ptr<Variable<Stage>> var_stage_;
  std::unique_ptr<Variable<std::string>> var_new_version_;
  std::unique_ptr<Variable<uint64_t>> var_payload_size_;
  std::unique_ptr<Variable<std::string>> var_curr_channel_;
  std::unique_ptr<Variable<std::string>> var_new_channel_;
  std::unique_ptr<Variable<bool>> var_p2p_enabled_;
  std::unique_ptr<Variable<bool>> var_cellular_enabled_;
  std::unique_ptr<Variable<unsigned int>> var_consecutive_failed_update_checks_;
  std::unique_ptr<Variable<unsigned int>> var_server_dictated_poll_interval_;
  std::unique_ptr<Variable<UpdateRequestStatus>> var_forced_update_requested_;
  std::unique_ptr<Variable<UpdateRestrictions>> var_update_restrictions_;

  DISALLOW_COPY_AND_ASSIGN(RealUpdaterProvider);
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_REAL_UPDATER_PROVIDER_H_
