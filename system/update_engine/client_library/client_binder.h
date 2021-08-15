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

#ifndef UPDATE_ENGINE_CLIENT_LIBRARY_CLIENT_BINDER_H_
#define UPDATE_ENGINE_CLIENT_LIBRARY_CLIENT_BINDER_H_

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <base/macros.h>
#include <utils/String16.h>
#include <utils/StrongPointer.h>

#include <brillo/binder_watcher.h>

#include "android/brillo/BnUpdateEngineStatusCallback.h"
#include "android/brillo/IUpdateEngine.h"

#include "update_engine/client_library/include/update_engine/client.h"

namespace update_engine {
namespace internal {

class BinderUpdateEngineClient : public UpdateEngineClient {
 public:
  BinderUpdateEngineClient() = default;
  bool Init();

  virtual ~BinderUpdateEngineClient() = default;

  bool AttemptUpdate(const std::string& app_version,
                     const std::string& omaha_url,
                     bool at_user_request) override;

  bool AttemptInstall(const std::string& omaha_url,
                      const std::vector<std::string>& dlc_module_ids) override;

  bool GetStatus(int64_t* out_last_checked_time,
                 double* out_progress,
                 UpdateStatus* out_update_status,
                 std::string* out_new_version,
                 int64_t* out_new_size) const override;

  bool SetCohortHint(const std::string& in_cohort_hint) override;
  bool GetCohortHint(std::string* out_cohort_hint) const override;

  bool SetUpdateOverCellularPermission(bool allowed) override;
  bool GetUpdateOverCellularPermission(bool* allowed) const override;

  bool SetP2PUpdatePermission(bool enabled) override;
  bool GetP2PUpdatePermission(bool* enabled) const override;

  bool Rollback(bool powerwash) override;

  bool GetRollbackPartition(std::string* rollback_partition) const override;

  void RebootIfNeeded() override;

  bool GetPrevVersion(std::string* prev_version) const override;

  bool ResetStatus() override;

  bool SetTargetChannel(const std::string& target_channel,
                        bool allow_powerwash) override;

  bool GetTargetChannel(std::string* out_channel) const override;

  bool GetChannel(std::string* out_channel) const override;

  bool RegisterStatusUpdateHandler(StatusUpdateHandler* handler) override;
  bool UnregisterStatusUpdateHandler(StatusUpdateHandler* handler) override;

  bool GetLastAttemptError(int32_t* last_attempt_error) const override;

  bool GetEolStatus(int32_t* eol_status) const override;

 private:
  class StatusUpdateCallback
      : public android::brillo::BnUpdateEngineStatusCallback {
   public:
    explicit StatusUpdateCallback(BinderUpdateEngineClient* client)
        : client_(client) {}

    android::binder::Status HandleStatusUpdate(
        const android::brillo::ParcelableUpdateEngineStatus& status) override;

   private:
    BinderUpdateEngineClient* client_;
  };

  android::sp<android::brillo::IUpdateEngine> service_;
  android::sp<android::brillo::IUpdateEngineStatusCallback> status_callback_;
  std::vector<update_engine::StatusUpdateHandler*> handlers_;
  brillo::BinderWatcher binder_watcher_;

  DISALLOW_COPY_AND_ASSIGN(BinderUpdateEngineClient);
};  // class BinderUpdateEngineClient

}  // namespace internal
}  // namespace update_engine

#endif  // UPDATE_ENGINE_CLIENT_LIBRARY_CLIENT_BINDER_H_
