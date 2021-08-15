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

#include "update_engine/parcelable_update_engine_status.h"
#include "update_engine/update_status_utils.h"

#include <binder/Parcel.h>

using update_engine::UpdateEngineStatus;

namespace android {
namespace brillo {

ParcelableUpdateEngineStatus::ParcelableUpdateEngineStatus(
    const UpdateEngineStatus& status)
    : last_checked_time_(status.last_checked_time),
      current_operation_(
          chromeos_update_engine::UpdateStatusToString(status.status)),
      progress_(status.progress),
      current_version_(String16{status.current_version.c_str()}),
      current_system_version_(String16{status.current_system_version.c_str()}),
      new_size_(status.new_size_bytes),
      new_version_(String16{status.new_version.c_str()}),
      new_system_version_(String16{status.new_system_version.c_str()}) {}

status_t ParcelableUpdateEngineStatus::writeToParcel(Parcel* parcel) const {
  status_t status;

  status = parcel->writeInt64(last_checked_time_);
  if (status != OK) {
    return status;
  }

  status = parcel->writeString16(current_operation_);
  if (status != OK) {
    return status;
  }

  status = parcel->writeDouble(progress_);
  if (status != OK) {
    return status;
  }

  status = parcel->writeString16(current_version_);
  if (status != OK) {
    return status;
  }

  status = parcel->writeString16(current_system_version_);
  if (status != OK) {
    return status;
  }

  status = parcel->writeInt64(new_size_);
  if (status != OK) {
    return status;
  }

  status = parcel->writeString16(new_version_);
  if (status != OK) {
    return status;
  }

  return parcel->writeString16(new_system_version_);
}

status_t ParcelableUpdateEngineStatus::readFromParcel(const Parcel* parcel) {
  status_t status;

  status = parcel->readInt64(&last_checked_time_);
  if (status != OK) {
    return status;
  }

  status = parcel->readString16(&current_operation_);
  if (status != OK) {
    return status;
  }

  status = parcel->readDouble(&progress_);
  if (status != OK) {
    return status;
  }

  status = parcel->readString16(&current_version_);
  if (status != OK) {
    return status;
  }

  status = parcel->readString16(&current_system_version_);
  if (status != OK) {
    return status;
  }

  status = parcel->readInt64(&new_size_);
  if (status != OK) {
    return status;
  }

  status = parcel->readString16(&new_version_);
  if (status != OK) {
    return status;
  }

  return parcel->readString16(&new_system_version_);
}

}  // namespace brillo
}  // namespace android
