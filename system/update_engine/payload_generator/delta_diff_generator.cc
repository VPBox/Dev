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

#include "update_engine/payload_generator/delta_diff_generator.h"

#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <base/logging.h>

#include "update_engine/common/utils.h"
#include "update_engine/payload_consumer/delta_performer.h"
#include "update_engine/payload_consumer/payload_constants.h"
#include "update_engine/payload_generator/ab_generator.h"
#include "update_engine/payload_generator/blob_file_writer.h"
#include "update_engine/payload_generator/delta_diff_utils.h"
#include "update_engine/payload_generator/full_update_generator.h"
#include "update_engine/payload_generator/inplace_generator.h"
#include "update_engine/payload_generator/payload_file.h"

using std::string;
using std::unique_ptr;
using std::vector;

namespace chromeos_update_engine {

// bytes
const size_t kRootFSPartitionSize = static_cast<size_t>(2) * 1024 * 1024 * 1024;
const size_t kBlockSize = 4096;  // bytes

bool GenerateUpdatePayloadFile(const PayloadGenerationConfig& config,
                               const string& output_path,
                               const string& private_key_path,
                               uint64_t* metadata_size) {
  if (!config.version.Validate()) {
    LOG(ERROR) << "Unsupported major.minor version: " << config.version.major
               << "." << config.version.minor;
    return false;
  }

  // Create empty payload file object.
  PayloadFile payload;
  TEST_AND_RETURN_FALSE(payload.Init(config));

  const string kTempFileTemplate("CrAU_temp_data.XXXXXX");
  string temp_file_path;
  int data_file_fd;
  TEST_AND_RETURN_FALSE(
      utils::MakeTempFile(kTempFileTemplate, &temp_file_path, &data_file_fd));
  ScopedPathUnlinker temp_file_unlinker(temp_file_path);
  TEST_AND_RETURN_FALSE(data_file_fd >= 0);

  {
    off_t data_file_size = 0;
    ScopedFdCloser data_file_fd_closer(&data_file_fd);
    BlobFileWriter blob_file(data_file_fd, &data_file_size);
    if (config.is_delta) {
      TEST_AND_RETURN_FALSE(config.source.partitions.size() ==
                            config.target.partitions.size());
    }
    PartitionConfig empty_part("");
    for (size_t i = 0; i < config.target.partitions.size(); i++) {
      const PartitionConfig& old_part =
          config.is_delta ? config.source.partitions[i] : empty_part;
      const PartitionConfig& new_part = config.target.partitions[i];
      LOG(INFO) << "Partition name: " << new_part.name;
      LOG(INFO) << "Partition size: " << new_part.size;
      LOG(INFO) << "Block count: " << new_part.size / config.block_size;

      // Select payload generation strategy based on the config.
      unique_ptr<OperationsGenerator> strategy;
      if (!old_part.path.empty()) {
        // Delta update.
        if (config.version.minor == kInPlaceMinorPayloadVersion) {
          LOG(INFO) << "Using generator InplaceGenerator().";
          strategy.reset(new InplaceGenerator());
        } else {
          LOG(INFO) << "Using generator ABGenerator().";
          strategy.reset(new ABGenerator());
        }
      } else {
        LOG(INFO) << "Using generator FullUpdateGenerator().";
        strategy.reset(new FullUpdateGenerator());
      }

      vector<AnnotatedOperation> aops;
      // Generate the operations using the strategy we selected above.
      TEST_AND_RETURN_FALSE(strategy->GenerateOperations(
          config, old_part, new_part, &blob_file, &aops));

      // Filter the no-operations. OperationsGenerators should not output this
      // kind of operations normally, but this is an extra step to fix that if
      // happened.
      diff_utils::FilterNoopOperations(&aops);

      TEST_AND_RETURN_FALSE(payload.AddPartition(old_part, new_part, aops));
    }
  }

  LOG(INFO) << "Writing payload file...";
  // Write payload file to disk.
  TEST_AND_RETURN_FALSE(payload.WritePayload(
      output_path, temp_file_path, private_key_path, metadata_size));

  LOG(INFO) << "All done. Successfully created delta file with "
            << "metadata size = " << *metadata_size;
  return true;
}

};  // namespace chromeos_update_engine
