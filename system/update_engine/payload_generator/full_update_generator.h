//
// Copyright (C) 2010 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_PAYLOAD_GENERATOR_FULL_UPDATE_GENERATOR_H_
#define UPDATE_ENGINE_PAYLOAD_GENERATOR_FULL_UPDATE_GENERATOR_H_

#include <string>
#include <vector>

#include <base/macros.h>

#include "update_engine/payload_generator/blob_file_writer.h"
#include "update_engine/payload_generator/operations_generator.h"
#include "update_engine/payload_generator/payload_generation_config.h"

namespace chromeos_update_engine {

class FullUpdateGenerator : public OperationsGenerator {
 public:
  FullUpdateGenerator() = default;

  // OperationsGenerator override.
  // Creates a full update for the target image defined in |config|. |config|
  // must be a valid payload generation configuration for a full payload.
  // Populates |aops|, with data about the update operations, and writes
  // relevant data to |blob_file|.
  bool GenerateOperations(const PayloadGenerationConfig& config,
                          const PartitionConfig& old_part,
                          const PartitionConfig& new_part,
                          BlobFileWriter* blob_file,
                          std::vector<AnnotatedOperation>* aops) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(FullUpdateGenerator);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_GENERATOR_FULL_UPDATE_GENERATOR_H_
