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

#ifndef UPDATE_ENGINE_PAYLOAD_GENERATOR_AB_GENERATOR_H_
#define UPDATE_ENGINE_PAYLOAD_GENERATOR_AB_GENERATOR_H_

#include <string>
#include <vector>

#include <base/macros.h>
#include <brillo/secure_blob.h>

#include "update_engine/payload_consumer/payload_constants.h"
#include "update_engine/payload_generator/blob_file_writer.h"
#include "update_engine/payload_generator/extent_utils.h"
#include "update_engine/payload_generator/filesystem_interface.h"
#include "update_engine/payload_generator/operations_generator.h"
#include "update_engine/payload_generator/payload_generation_config.h"
#include "update_engine/update_metadata.pb.h"

namespace chromeos_update_engine {

// The ABGenerator is an operations generator that generates payloads using the
// A-to-B operations SOURCE_COPY and SOURCE_BSDIFF introduced in the payload
// minor version 2 format.
class ABGenerator : public OperationsGenerator {
 public:
  ABGenerator() = default;

  // Generate the update payload operations for the given partition using
  // SOURCE_* operations, used for generating deltas for the minor version
  // kSourceMinorPayloadVersion. This function will generate operations in the
  // partition that will read blocks from the source partition in random order
  // and write the new image on the target partition, also possibly in random
  // order. The operations are stored in |aops| and should be executed in that
  // order. All the offsets in the operations reference the data written to
  // |blob_file|.
  bool GenerateOperations(const PayloadGenerationConfig& config,
                          const PartitionConfig& old_part,
                          const PartitionConfig& new_part,
                          BlobFileWriter* blob_file,
                          std::vector<AnnotatedOperation>* aops) override;

  // Split the operations in the vector of AnnotatedOperations |aops| such that
  // for every operation there is only one dst extent and updates |aops| with
  // the new list of operations. All kinds of operations are fragmented except
  // BSDIFF and SOURCE_BSDIFF, PUFFDIFF and BROTLI_BSDIFF operations.  The
  // |target_part_path| is the filename of the new image, where the destination
  // extents refer to. The blobs of the operations in |aops| should reference
  // |blob_file|. |blob_file| are updated if needed.
  static bool FragmentOperations(const PayloadVersion& version,
                                 std::vector<AnnotatedOperation>* aops,
                                 const std::string& target_part_path,
                                 BlobFileWriter* blob_file);

  // Takes a vector of AnnotatedOperations |aops| and sorts them by the first
  // start block in their destination extents. Sets |aops| to a vector of the
  // sorted operations.
  static void SortOperationsByDestination(
      std::vector<AnnotatedOperation>* aops);

  // Takes an SOURCE_COPY install operation, |aop|, and adds one operation for
  // each dst extent in |aop| to |ops|. The new operations added to |ops| will
  // have only one dst extent. The src extents are split so the number of blocks
  // in the src and dst extents are equal.
  // E.g. we have a SOURCE_COPY operation:
  //   src extents: [(1, 3), (5, 1), (7, 1)], dst extents: [(2, 2), (6, 3)]
  // Then we will get 2 new operations:
  //   1. src extents: [(1, 2)], dst extents: [(2, 2)]
  //   2. src extents: [(3, 1),(5, 1),(7, 1)], dst extents: [(6, 3)]
  static bool SplitSourceCopy(const AnnotatedOperation& original_aop,
                              std::vector<AnnotatedOperation>* result_aops);

  // Takes a REPLACE, REPLACE_BZ or REPLACE_XZ operation |aop|, and adds one
  // operation for each dst extent in |aop| to |ops|. The new operations added
  // to |ops| will have only one dst extent each, and may be of a different
  // type depending on whether compression is advantageous.
  static bool SplitAReplaceOp(const PayloadVersion& version,
                              const AnnotatedOperation& original_aop,
                              const std::string& target_part,
                              std::vector<AnnotatedOperation>* result_aops,
                              BlobFileWriter* blob_file);

  // Takes a sorted (by first destination extent) vector of operations |aops|
  // and merges SOURCE_COPY, REPLACE, REPLACE_BZ and REPLACE_XZ, operations in
  // that vector.
  // It will merge two operations if:
  //   - They are both REPLACE_*, or they are both SOURCE_COPY,
  //   - Their destination blocks are contiguous.
  //   - Their combined blocks do not exceed |chunk_blocks| blocks.
  // Note that unlike other methods, you can't pass a negative number in
  // |chunk_blocks|.
  static bool MergeOperations(std::vector<AnnotatedOperation>* aops,
                              const PayloadVersion& version,
                              size_t chunk_blocks,
                              const std::string& target_part,
                              BlobFileWriter* blob_file);

  // Takes a vector of AnnotatedOperations |aops|, adds source hash to all
  // operations that have src_extents.
  static bool AddSourceHash(std::vector<AnnotatedOperation>* aops,
                            const std::string& source_part_path);

 private:
  // Adds the data payload for a REPLACE/REPLACE_BZ/REPLACE_XZ operation |aop|
  // by reading its output extents from |target_part_path| and appending a
  // corresponding data blob to |blob_file|. The blob will be compressed if this
  // is smaller than the uncompressed form, and the operation type will be set
  // accordingly. |*blob_file| will be updated as well. If the operation happens
  // to have the right type and already points to a data blob, nothing is
  // written. Caller should only set type and data blob if it's valid.
  static bool AddDataAndSetType(AnnotatedOperation* aop,
                                const PayloadVersion& version,
                                const std::string& target_part_path,
                                BlobFileWriter* blob_file);

  DISALLOW_COPY_AND_ASSIGN(ABGenerator);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_GENERATOR_AB_GENERATOR_H_
