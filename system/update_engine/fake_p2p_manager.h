//
// Copyright (C) 2013 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_FAKE_P2P_MANAGER_H_
#define UPDATE_ENGINE_FAKE_P2P_MANAGER_H_

#include <string>

#include "update_engine/p2p_manager.h"

namespace chromeos_update_engine {

// A fake implementation of P2PManager.
class FakeP2PManager : public P2PManager {
 public:
  FakeP2PManager()
      : is_p2p_enabled_(false),
        ensure_p2p_running_result_(false),
        ensure_p2p_not_running_result_(false),
        perform_housekeeping_result_(false),
        count_shared_files_result_(0) {}

  // P2PManager overrides.
  void SetDevicePolicy(const policy::DevicePolicy* device_policy) override {}

  bool IsP2PEnabled() override { return is_p2p_enabled_; }

  bool EnsureP2PRunning() override { return ensure_p2p_running_result_; }

  bool EnsureP2PNotRunning() override { return ensure_p2p_not_running_result_; }

  bool PerformHousekeeping() override { return perform_housekeeping_result_; }

  void LookupUrlForFile(const std::string& file_id,
                        size_t minimum_size,
                        base::TimeDelta max_time_to_wait,
                        LookupCallback callback) override {
    callback.Run(lookup_url_for_file_result_);
  }

  bool FileShare(const std::string& file_id, size_t expected_size) override {
    return false;
  }

  base::FilePath FileGetPath(const std::string& file_id) override {
    return base::FilePath();
  }

  ssize_t FileGetSize(const std::string& file_id) override { return -1; }

  ssize_t FileGetExpectedSize(const std::string& file_id) override {
    return -1;
  }

  bool FileGetVisible(const std::string& file_id, bool* out_result) override {
    return false;
  }

  bool FileMakeVisible(const std::string& file_id) override { return false; }

  int CountSharedFiles() override { return count_shared_files_result_; }

  // Methods for controlling what the fake returns and how it acts.
  void SetP2PEnabled(bool is_p2p_enabled) { is_p2p_enabled_ = is_p2p_enabled; }

  void SetEnsureP2PRunningResult(bool ensure_p2p_running_result) {
    ensure_p2p_running_result_ = ensure_p2p_running_result;
  }

  void SetEnsureP2PNotRunningResult(bool ensure_p2p_not_running_result) {
    ensure_p2p_not_running_result_ = ensure_p2p_not_running_result;
  }

  void SetPerformHousekeepingResult(bool perform_housekeeping_result) {
    perform_housekeeping_result_ = perform_housekeeping_result;
  }

  void SetCountSharedFilesResult(int count_shared_files_result) {
    count_shared_files_result_ = count_shared_files_result;
  }

  void SetLookupUrlForFileResult(const std::string& url) {
    lookup_url_for_file_result_ = url;
  }

 private:
  bool is_p2p_enabled_;
  bool ensure_p2p_running_result_;
  bool ensure_p2p_not_running_result_;
  bool perform_housekeeping_result_;
  int count_shared_files_result_;
  std::string lookup_url_for_file_result_;

  DISALLOW_COPY_AND_ASSIGN(FakeP2PManager);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_FAKE_P2P_MANAGER_H_
