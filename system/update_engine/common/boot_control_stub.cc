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

#include "update_engine/common/boot_control_stub.h"

#include <base/logging.h>

using std::string;

namespace chromeos_update_engine {

unsigned int BootControlStub::GetNumSlots() const {
  return 0;
}

BootControlInterface::Slot BootControlStub::GetCurrentSlot() const {
  LOG(ERROR) << __FUNCTION__ << " should never be called.";
  return 0;
}

bool BootControlStub::GetPartitionDevice(const string& partition_name,
                                         Slot slot,
                                         string* device) const {
  LOG(ERROR) << __FUNCTION__ << " should never be called.";
  return false;
}

bool BootControlStub::IsSlotBootable(Slot slot) const {
  LOG(ERROR) << __FUNCTION__ << " should never be called.";
  return false;
}

bool BootControlStub::MarkSlotUnbootable(Slot slot) {
  LOG(ERROR) << __FUNCTION__ << " should never be called.";
  return false;
}

bool BootControlStub::SetActiveBootSlot(Slot slot) {
  LOG(ERROR) << __FUNCTION__ << " should never be called.";
  return false;
}

bool BootControlStub::MarkBootSuccessfulAsync(
    base::Callback<void(bool)> callback) {
  // This is expected to be called on update_engine startup.
  return false;
}

bool BootControlStub::InitPartitionMetadata(
    Slot slot,
    const PartitionMetadata& partition_metadata,
    bool update_metadata) {
  LOG(ERROR) << __FUNCTION__ << " should never be called.";
  return false;
}

void BootControlStub::Cleanup() {
  LOG(ERROR) << __FUNCTION__ << " should never be called.";
}

}  // namespace chromeos_update_engine
