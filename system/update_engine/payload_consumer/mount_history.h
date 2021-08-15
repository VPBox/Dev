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

#ifndef UPDATE_ENGINE_PAYLOAD_CONSUMER_MOUNT_HISTORY_H_
#define UPDATE_ENGINE_PAYLOAD_CONSUMER_MOUNT_HISTORY_H_

#include "update_engine/payload_consumer/file_descriptor.h"

namespace chromeos_update_engine {
// Try to parse an ext4 from the partition specified by |blockdevice_fd|.
// If ext4 header exists and remount is detected, log mount count and date.
void LogMountHistory(const FileDescriptorPtr blockdevice_fd);
}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_CONSUMER_MOUNT_HISTORY_H_
