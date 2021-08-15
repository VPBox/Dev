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

#ifndef UPDATE_ENGINE_PAYLOAD_GENERATOR_PAYLOAD_FILE_H_
#define UPDATE_ENGINE_PAYLOAD_GENERATOR_PAYLOAD_FILE_H_

#include <string>
#include <vector>

#include <brillo/secure_blob.h>
#include <gtest/gtest_prod.h>  // for FRIEND_TEST

#include "update_engine/payload_generator/annotated_operation.h"
#include "update_engine/payload_generator/payload_generation_config.h"
#include "update_engine/update_metadata.pb.h"

namespace chromeos_update_engine {

// Class to handle the creation of a payload file. This class is the only one
// dealing with writing the payload and its format, but has no logic about what
// should be on it.
class PayloadFile {
 public:
  // Initialize the payload file with the payload generation config. It computes
  // required hashes of the requested partitions.
  bool Init(const PayloadGenerationConfig& config);

  // Add a partition to the payload manifest. Including partition name, list of
  // operations and partition info. The operations in |aops|
  // reference a blob stored in the file provided to WritePayload().
  bool AddPartition(const PartitionConfig& old_conf,
                    const PartitionConfig& new_conf,
                    const std::vector<AnnotatedOperation>& aops);

  // Write the payload to the |payload_file| file. The operations reference
  // blobs in the |data_blobs_path| file and the blobs will be reordered in the
  // payload file to match the order of the operations. The size of the metadata
  // section of the payload is stored in |metadata_size_out|.
  bool WritePayload(const std::string& payload_file,
                    const std::string& data_blobs_path,
                    const std::string& private_key_path,
                    uint64_t* metadata_size_out);

 private:
  FRIEND_TEST(PayloadFileTest, ReorderBlobsTest);

  // Computes a SHA256 hash of the given buf and sets the hash value in the
  // operation so that update_engine could verify. This hash should be set
  // for all operations that have a non-zero data blob. One exception is the
  // dummy operation for signature blob because the contents of the signature
  // blob will not be available at payload creation time. So, update_engine will
  // gracefully ignore the dummy signature operation.
  static bool AddOperationHash(InstallOperation* op, const brillo::Blob& buf);

  // Install operations in the manifest may reference data blobs, which
  // are in data_blobs_path. This function creates a new data blobs file
  // with the data blobs in the same order as the referencing install
  // operations in the manifest. E.g. if manifest[0] has a data blob
  // "X" at offset 1, manifest[1] has a data blob "Y" at offset 0,
  // and data_blobs_path's file contains "YX", new_data_blobs_path
  // will set to be a file that contains "XY".
  bool ReorderDataBlobs(const std::string& data_blobs_path,
                        const std::string& new_data_blobs_path);

  // Print in stderr the Payload usage report.
  void ReportPayloadUsage(uint64_t metadata_size) const;

  // The major_version of the requested payload.
  uint64_t major_version_;

  DeltaArchiveManifest manifest_;

  // Struct has necessary information to write PartitionUpdate in protobuf.
  struct Partition {
    // The name of the partition.
    std::string name;

    // The operations to be performed to this partition.
    std::vector<AnnotatedOperation> aops;

    PartitionInfo old_info;
    PartitionInfo new_info;

    PostInstallConfig postinstall;
    VerityConfig verity;
  };

  std::vector<Partition> part_vec_;
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_GENERATOR_PAYLOAD_FILE_H_
