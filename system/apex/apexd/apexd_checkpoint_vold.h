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

#ifndef ANDROID_APEXD_APEXD_CHECKPOINT_VOLD_H_
#define ANDROID_APEXD_APEXD_CHECKPOINT_VOLD_H_

#include <string>

#include <utils/StrongPointer.h>

#include "apexd_checkpoint.h"
#include "status_or.h"

namespace android {

namespace os {
class IVold;
}

namespace apex {

class VoldCheckpointInterface : public CheckpointInterface {
 public:
  ~VoldCheckpointInterface();

  StatusOr<bool> SupportsFsCheckpoints() override;

  StatusOr<bool> NeedsCheckpoint() override;
  StatusOr<bool> NeedsRollback() override;
  Status StartCheckpoint(int32_t retry) override;

  Status AbortChanges(const std::string& msg, bool numRetries) override;

  static StatusOr<VoldCheckpointInterface> Create();

  VoldCheckpointInterface(VoldCheckpointInterface&& other) noexcept;

 private:
  VoldCheckpointInterface(sp<os::IVold>&& vold_service);

  sp<os::IVold> vold_service_;
  bool supports_fs_checkpoints_;
};

}  // namespace apex
}  // namespace android

#endif  // ANDROID_APEXD_APEXD_CHECKPOINT_VOLD_H_
