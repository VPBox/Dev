//
// Copyright (C) 2012 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_PAYLOAD_GENERATOR_DELTA_DIFF_GENERATOR_H_
#define UPDATE_ENGINE_PAYLOAD_GENERATOR_DELTA_DIFF_GENERATOR_H_

#include <string>

#include "update_engine/payload_generator/payload_generation_config.h"

namespace chromeos_update_engine {

extern const size_t kBlockSize;
extern const size_t kRootFSPartitionSize;

// The |config| describes the payload generation request, describing both
// old and new images for delta payloads and only the new image for full
// payloads.
// For delta payloads, the images should be already mounted read-only at
// the respective rootfs_mountpt.
// |private_key_path| points to a private key used to sign the update.
// Pass empty string to not sign the update.
// |output_path| is the filename where the delta update should be written.
// Returns true on success. Also writes the size of the metadata into
// |metadata_size|.
bool GenerateUpdatePayloadFile(const PayloadGenerationConfig& config,
                               const std::string& output_path,
                               const std::string& private_key_path,
                               uint64_t* metadata_size);

};  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_GENERATOR_DELTA_DIFF_GENERATOR_H_
