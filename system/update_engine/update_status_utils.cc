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
#include "update_engine/update_status_utils.h"

#include <base/logging.h>
#include <update_engine/dbus-constants.h>

using update_engine::UpdateStatus;

namespace chromeos_update_engine {

const char* UpdateStatusToString(const UpdateStatus& status) {
  switch (status) {
    case UpdateStatus::IDLE:
      return update_engine::kUpdateStatusIdle;
    case UpdateStatus::CHECKING_FOR_UPDATE:
      return update_engine::kUpdateStatusCheckingForUpdate;
    case UpdateStatus::UPDATE_AVAILABLE:
      return update_engine::kUpdateStatusUpdateAvailable;
    case UpdateStatus::NEED_PERMISSION_TO_UPDATE:
      return update_engine::kUpdateStatusNeedPermissionToUpdate;
    case UpdateStatus::DOWNLOADING:
      return update_engine::kUpdateStatusDownloading;
    case UpdateStatus::VERIFYING:
      return update_engine::kUpdateStatusVerifying;
    case UpdateStatus::FINALIZING:
      return update_engine::kUpdateStatusFinalizing;
    case UpdateStatus::UPDATED_NEED_REBOOT:
      return update_engine::kUpdateStatusUpdatedNeedReboot;
    case UpdateStatus::REPORTING_ERROR_EVENT:
      return update_engine::kUpdateStatusReportingErrorEvent;
    case UpdateStatus::ATTEMPTING_ROLLBACK:
      return update_engine::kUpdateStatusAttemptingRollback;
    case UpdateStatus::DISABLED:
      return update_engine::kUpdateStatusDisabled;
  }

  NOTREACHED();
  return nullptr;
}

bool StringToUpdateStatus(const std::string& s, UpdateStatus* status) {
  if (s == update_engine::kUpdateStatusIdle) {
    *status = UpdateStatus::IDLE;
    return true;
  } else if (s == update_engine::kUpdateStatusCheckingForUpdate) {
    *status = UpdateStatus::CHECKING_FOR_UPDATE;
    return true;
  } else if (s == update_engine::kUpdateStatusUpdateAvailable) {
    *status = UpdateStatus::UPDATE_AVAILABLE;
    return true;
  } else if (s == update_engine::kUpdateStatusNeedPermissionToUpdate) {
    *status = UpdateStatus::NEED_PERMISSION_TO_UPDATE;
    return true;
  } else if (s == update_engine::kUpdateStatusDownloading) {
    *status = UpdateStatus::DOWNLOADING;
    return true;
  } else if (s == update_engine::kUpdateStatusVerifying) {
    *status = UpdateStatus::VERIFYING;
    return true;
  } else if (s == update_engine::kUpdateStatusFinalizing) {
    *status = UpdateStatus::FINALIZING;
    return true;
  } else if (s == update_engine::kUpdateStatusUpdatedNeedReboot) {
    *status = UpdateStatus::UPDATED_NEED_REBOOT;
    return true;
  } else if (s == update_engine::kUpdateStatusReportingErrorEvent) {
    *status = UpdateStatus::REPORTING_ERROR_EVENT;
    return true;
  } else if (s == update_engine::kUpdateStatusAttemptingRollback) {
    *status = UpdateStatus::ATTEMPTING_ROLLBACK;
    return true;
  } else if (s == update_engine::kUpdateStatusDisabled) {
    *status = UpdateStatus::DISABLED;
    return true;
  }
  return false;
}

}  // namespace chromeos_update_engine
