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

#include "update_engine/binder_service_brillo.h"

#include <base/bind.h>

#include <binderwrapper/binder_wrapper.h>

#include <utils/String16.h>
#include <utils/StrongPointer.h>

#include "update_engine/update_status_utils.h"

using android::sp;
using android::String16;
using android::String8;
using android::binder::Status;
using android::brillo::IUpdateEngineStatusCallback;
using android::brillo::ParcelableUpdateEngineStatus;
using brillo::ErrorPtr;
using std::string;
using update_engine::UpdateEngineStatus;

namespace chromeos_update_engine {

namespace {
string NormalString(const String16& in) {
  return string{String8{in}.string()};
}

Status ToStatus(ErrorPtr* error) {
  return Status::fromServiceSpecificError(
      1, String8{error->get()->GetMessage().c_str()});
}
}  // namespace

template <typename... Parameters, typename... Arguments>
Status BinderUpdateEngineBrilloService::CallCommonHandler(
    bool (UpdateEngineService::*Handler)(ErrorPtr*, Parameters...),
    Arguments... arguments) {
  ErrorPtr error;
  if (((common_.get())->*Handler)(&error, arguments...))
    return Status::ok();
  return ToStatus(&error);
}

Status BinderUpdateEngineBrilloService::SetUpdateAttemptFlags(int flags) {
  return CallCommonHandler(&UpdateEngineService::SetUpdateAttemptFlags, flags);
}

Status BinderUpdateEngineBrilloService::AttemptUpdate(
    const String16& app_version,
    const String16& omaha_url,
    int flags,
    bool* out_result) {
  return CallCommonHandler(&UpdateEngineService::AttemptUpdate,
                           NormalString(app_version),
                           NormalString(omaha_url),
                           flags,
                           out_result);
}

Status BinderUpdateEngineBrilloService::AttemptRollback(bool powerwash) {
  return CallCommonHandler(&UpdateEngineService::AttemptRollback, powerwash);
}

Status BinderUpdateEngineBrilloService::CanRollback(bool* out_can_rollback) {
  return CallCommonHandler(&UpdateEngineService::CanRollback, out_can_rollback);
}

Status BinderUpdateEngineBrilloService::ResetStatus() {
  return CallCommonHandler(&UpdateEngineService::ResetStatus);
}

Status BinderUpdateEngineBrilloService::GetStatus(
    ParcelableUpdateEngineStatus* status) {
  UpdateEngineStatus update_engine_status;
  auto ret =
      CallCommonHandler(&UpdateEngineService::GetStatus, &update_engine_status);

  if (ret.isOk()) {
    *status = ParcelableUpdateEngineStatus(update_engine_status);
  }

  return ret;
}

Status BinderUpdateEngineBrilloService::RebootIfNeeded() {
  return CallCommonHandler(&UpdateEngineService::RebootIfNeeded);
}

Status BinderUpdateEngineBrilloService::SetChannel(
    const String16& target_channel, bool powerwash) {
  return CallCommonHandler(&UpdateEngineService::SetChannel,
                           NormalString(target_channel),
                           powerwash);
}

Status BinderUpdateEngineBrilloService::GetChannel(bool get_current_channel,
                                                   String16* out_channel) {
  string channel_string;
  auto ret = CallCommonHandler(
      &UpdateEngineService::GetChannel, get_current_channel, &channel_string);

  *out_channel = String16(channel_string.c_str());
  return ret;
}

Status BinderUpdateEngineBrilloService::SetCohortHint(
    const String16& in_cohort_hint) {
  return CallCommonHandler(&UpdateEngineService::SetCohortHint,
                           NormalString(in_cohort_hint));
}

Status BinderUpdateEngineBrilloService::GetCohortHint(
    String16* out_cohort_hint) {
  string cohort_hint;
  auto ret =
      CallCommonHandler(&UpdateEngineService::GetCohortHint, &cohort_hint);

  *out_cohort_hint = String16(cohort_hint.c_str());
  return ret;
}

Status BinderUpdateEngineBrilloService::SetP2PUpdatePermission(bool enabled) {
  return CallCommonHandler(&UpdateEngineService::SetP2PUpdatePermission,
                           enabled);
}

Status BinderUpdateEngineBrilloService::GetP2PUpdatePermission(
    bool* out_p2p_permission) {
  return CallCommonHandler(&UpdateEngineService::GetP2PUpdatePermission,
                           out_p2p_permission);
}

Status BinderUpdateEngineBrilloService::SetUpdateOverCellularPermission(
    bool enabled) {
  return CallCommonHandler(
      &UpdateEngineService::SetUpdateOverCellularPermission, enabled);
}

Status BinderUpdateEngineBrilloService::SetUpdateOverCellularTarget(
    const String16& target_version, int64_t target_size) {
  return CallCommonHandler(&UpdateEngineService::SetUpdateOverCellularTarget,
                           NormalString(target_version),
                           target_size);
}

Status BinderUpdateEngineBrilloService::GetUpdateOverCellularPermission(
    bool* out_cellular_permission) {
  return CallCommonHandler(
      &UpdateEngineService::GetUpdateOverCellularPermission,
      out_cellular_permission);
}

Status BinderUpdateEngineBrilloService::GetDurationSinceUpdate(
    int64_t* out_duration) {
  return CallCommonHandler(&UpdateEngineService::GetDurationSinceUpdate,
                           out_duration);
}

Status BinderUpdateEngineBrilloService::GetPrevVersion(
    String16* out_prev_version) {
  string version_string;
  auto ret =
      CallCommonHandler(&UpdateEngineService::GetPrevVersion, &version_string);

  *out_prev_version = String16(version_string.c_str());
  return ret;
}

Status BinderUpdateEngineBrilloService::GetRollbackPartition(
    String16* out_rollback_partition) {
  string partition_string;
  auto ret = CallCommonHandler(&UpdateEngineService::GetRollbackPartition,
                               &partition_string);

  if (ret.isOk()) {
    *out_rollback_partition = String16(partition_string.c_str());
  }

  return ret;
}

Status BinderUpdateEngineBrilloService::RegisterStatusCallback(
    const sp<IUpdateEngineStatusCallback>& callback) {
  callbacks_.emplace_back(callback);

  auto binder_wrapper = android::BinderWrapper::Get();

  binder_wrapper->RegisterForDeathNotifications(
      IUpdateEngineStatusCallback::asBinder(callback),
      base::Bind(&BinderUpdateEngineBrilloService::UnregisterStatusCallback,
                 base::Unretained(this),
                 base::Unretained(callback.get())));

  return Status::ok();
}

Status BinderUpdateEngineBrilloService::GetLastAttemptError(
    int* out_last_attempt_error) {
  return CallCommonHandler(&UpdateEngineService::GetLastAttemptError,
                           out_last_attempt_error);
}

Status BinderUpdateEngineBrilloService::GetEolStatus(int* out_eol_status) {
  return CallCommonHandler(&UpdateEngineService::GetEolStatus, out_eol_status);
}

void BinderUpdateEngineBrilloService::UnregisterStatusCallback(
    IUpdateEngineStatusCallback* callback) {
  auto it = callbacks_.begin();
  while (it != callbacks_.end() && it->get() != callback)
    it++;

  if (it == callbacks_.end()) {
    LOG(ERROR) << "Got death notification for unknown callback.";
    return;
  }

  LOG(INFO) << "Erasing orphan callback";
  callbacks_.erase(it);
}

void BinderUpdateEngineBrilloService::SendStatusUpdate(
    const UpdateEngineStatus& update_engine_status) {
  ParcelableUpdateEngineStatus parcelable_status(update_engine_status);
  for (auto& callback : callbacks_) {
    callback->HandleStatusUpdate(parcelable_status);
  }
}

}  // namespace chromeos_update_engine
