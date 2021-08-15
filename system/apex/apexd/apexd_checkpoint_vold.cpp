/*
 * Copyright (C) 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "apexd"

#include "apexd_checkpoint_vold.h"

#include <android-base/logging.h>
#include <android/os/IVold.h>
#include <binder/IServiceManager.h>

using android::sp;
using android::os::IVold;

namespace android {
namespace apex {

StatusOr<VoldCheckpointInterface> VoldCheckpointInterface::Create() {
  auto voldService =
      defaultServiceManager()->getService(android::String16("vold"));
  if (voldService != nullptr) {
    return StatusOr<VoldCheckpointInterface>(VoldCheckpointInterface(
        android::interface_cast<android::os::IVold>(voldService)));
  }
  return StatusOr<VoldCheckpointInterface>::Fail(
      "Failed to retrieve vold service.");
}

VoldCheckpointInterface::VoldCheckpointInterface(sp<IVold>&& vold_service) {
  vold_service_ = vold_service;
  supports_fs_checkpoints_ = false;
  android::binder::Status status =
      vold_service_->supportsCheckpoint(&supports_fs_checkpoints_);
  if (!status.isOk()) {
    LOG(ERROR) << "Failed to check if filesystem checkpoints are supported: "
               << status.toString8().c_str();
  }
}

VoldCheckpointInterface::VoldCheckpointInterface(
    VoldCheckpointInterface&& other) noexcept {
  vold_service_ = std::move(other.vold_service_);
  supports_fs_checkpoints_ = other.supports_fs_checkpoints_;
}

VoldCheckpointInterface::~VoldCheckpointInterface() {
  // Just here to be able to forward-declare IVold.
}

StatusOr<bool> VoldCheckpointInterface::SupportsFsCheckpoints() {
  return StatusOr<bool>(supports_fs_checkpoints_);
}

StatusOr<bool> VoldCheckpointInterface::NeedsCheckpoint() {
  if (supports_fs_checkpoints_) {
    bool needs_checkpoint = false;
    android::binder::Status status =
        vold_service_->needsCheckpoint(&needs_checkpoint);
    if (!status.isOk()) {
      return StatusOr<bool>::Fail(status.toString8().c_str());
    }
    return StatusOr<bool>(needs_checkpoint);
  }
  return StatusOr<bool>(false);
}

StatusOr<bool> VoldCheckpointInterface::NeedsRollback() {
  if (supports_fs_checkpoints_) {
    bool needs_rollback = false;
    android::binder::Status status =
        vold_service_->needsRollback(&needs_rollback);
    if (!status.isOk()) {
      return StatusOr<bool>::Fail(status.toString8().c_str());
    }
    return StatusOr<bool>(needs_rollback);
  }
  return StatusOr<bool>(false);
}

Status VoldCheckpointInterface::StartCheckpoint(int32_t numRetries) {
  if (supports_fs_checkpoints_) {
    android::binder::Status status = vold_service_->startCheckpoint(numRetries);
    if (!status.isOk()) {
      return Status::Fail(status.toString8().c_str());
    }
    return Status::Success();
  }
  return Status::Fail("Device does not support filesystem checkpointing");
}

Status VoldCheckpointInterface::AbortChanges(const std::string& msg,
                                             bool retry) {
  vold_service_->abortChanges(msg, retry);
  return Status::Success();
}

}  // namespace apex
}  // namespace android
