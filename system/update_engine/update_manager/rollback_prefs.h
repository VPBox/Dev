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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_ROLLBACK_PREFS_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_ROLLBACK_PREFS_H_

namespace chromeos_update_manager {

// Value used to represent that kernel key versions can always roll-forward.
// This is the maximum value of a kernel key version.
constexpr int kRollforwardInfinity = 0xfffffffe;

// Whether the device should roll back to the target version, and if yes, which
// type of rollback should it do. Matches chrome_device_policy.proto's
// AutoUpdateSettingsProto::RollbackToTargetVersion.
enum class RollbackToTargetVersion {
  kUnspecified = 0,
  kDisabled = 1,
  kRollbackAndPowerwash = 2,
  kRollbackAndRestoreIfPossible = 3,
  kRollbackOnlyIfRestorePossible = 4,
  // This value must be the last entry.
  kMaxValue = 5
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_ROLLBACK_PREFS_H_
