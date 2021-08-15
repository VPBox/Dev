//
// Copyright (C) 2018 The Android Open Source Project
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

#include "update_engine/update_boot_flags_action.h"

#include <base/bind.h>
#include <base/logging.h>

#include "update_engine/common/boot_control.h"

namespace chromeos_update_engine {

bool UpdateBootFlagsAction::updated_boot_flags_ = false;
bool UpdateBootFlagsAction::is_running_ = false;

void UpdateBootFlagsAction::PerformAction() {
  if (is_running_) {
    LOG(INFO) << "Update boot flags running, nothing to do.";
    processor_->ActionComplete(this, ErrorCode::kSuccess);
    return;
  }
  if (updated_boot_flags_) {
    LOG(INFO) << "Already updated boot flags. Skipping.";
    processor_->ActionComplete(this, ErrorCode::kSuccess);
    return;
  }

  // This is purely best effort. Failures should be logged by Subprocess. Run
  // the script asynchronously to avoid blocking the event loop regardless of
  // the script runtime.
  is_running_ = true;
  LOG(INFO) << "Marking booted slot as good.";
  if (!boot_control_->MarkBootSuccessfulAsync(
          base::Bind(&UpdateBootFlagsAction::CompleteUpdateBootFlags,
                     base::Unretained(this)))) {
    CompleteUpdateBootFlags(false);
  }
}

void UpdateBootFlagsAction::TerminateProcessing() {
  is_running_ = false;
}

void UpdateBootFlagsAction::CompleteUpdateBootFlags(bool successful) {
  if (!successful) {
    // We ignore the failure for now because if the updating boot flags is flaky
    // or has a bug in a specific release, then blocking the update can cause
    // devices to stay behind even though we could have updated the system and
    // fixed the issue regardless of this failure.
    //
    // TODO(ahassani): Add new error code metric for kUpdateBootFlagsFailed.
    LOG(ERROR) << "Updating boot flags failed, but ignoring its failure.";
  }

  // As the callback to MarkBootSuccessfulAsync, this function can still be
  // called even after the current UpdateBootFlagsAction object get destroyed by
  // the action processor. In this case, check the value of the static variable
  // |is_running_| and skip executing the callback function.
  if (!is_running_) {
    LOG(INFO) << "UpdateBootFlagsAction is no longer running.";
    return;
  }

  is_running_ = false;

  updated_boot_flags_ = true;
  processor_->ActionComplete(this, ErrorCode::kSuccess);
}

}  // namespace chromeos_update_engine
