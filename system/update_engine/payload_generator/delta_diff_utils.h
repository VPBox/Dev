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

#ifndef UPDATE_ENGINE_PAYLOAD_GENERATOR_DELTA_DIFF_UTILS_H_
#define UPDATE_ENGINE_PAYLOAD_GENERATOR_DELTA_DIFF_UTILS_H_

#include <map>
#include <string>
#include <vector>

#include <brillo/secure_blob.h>
#include <puffin/puffdiff.h>

#include "update_engine/payload_generator/annotated_operation.h"
#include "update_engine/payload_generator/extent_ranges.h"
#include "update_engine/payload_generator/payload_generation_config.h"
#include "update_engine/update_metadata.pb.h"

namespace chromeos_update_engine {

namespace diff_utils {

// Create operations in |aops| to produce all the blocks in the |new_part|
// partition using the filesystem opened in that PartitionConfig.
// It uses the files reported by the filesystem in |old_part| and the data
// blocks in that partition (if available) to determine the best way to compress
// the new files (REPLACE, REPLACE_BZ, COPY, BSDIFF) and writes any necessary
// data to |blob_file|. |hard_chunk_blocks| and |soft_chunk_blocks| are the hard
// and soft chunk limits in number of blocks respectively. The soft chunk limit
// is used to split MOVE and SOURCE_COPY operations and REPLACE_BZ of zeroed
// blocks, while the hard limit is used to split a file when generating other
// operations. A value of -1 in |hard_chunk_blocks| means whole files.
bool DeltaReadPartition(std::vector<AnnotatedOperation>* aops,
                        const PartitionConfig& old_part,
                        const PartitionConfig& new_part,
                        ssize_t hard_chunk_blocks,
                        size_t soft_chunk_blocks,
                        const PayloadVersion& version,
                        BlobFileWriter* blob_file);

// Create operations in |aops| for identical blocks that moved around in the old
// and new partition and also handle zeroed blocks. The old and new partition
// are stored in the |old_part| and |new_part| files and have |old_num_blocks|
// and |new_num_blocks| respectively. The maximum operation size is
// |chunk_blocks| blocks, or unlimited if |chunk_blocks| is -1. The blobs of the
// produced operations are stored in the |blob_file|.
// The collections |old_visited_blocks| and |new_visited_blocks| state what
// blocks already have operations reading or writing them and only operations
// for unvisited blocks are produced by this function updating both collections
// with the used blocks.
bool DeltaMovedAndZeroBlocks(std::vector<AnnotatedOperation>* aops,
                             const std::string& old_part,
                             const std::string& new_part,
                             size_t old_num_blocks,
                             size_t new_num_blocks,
                             ssize_t chunk_blocks,
                             const PayloadVersion& version,
                             BlobFileWriter* blob_file,
                             ExtentRanges* old_visited_blocks,
                             ExtentRanges* new_visited_blocks,
                             ExtentRanges* old_zero_blocks);

// For a given file |name| append operations to |aops| to produce it in the
// |new_part|. The file will be split in chunks of |chunk_blocks| blocks each
// or treated as a single chunk if |chunk_blocks| is -1. The file data is
// stored in |new_part| in the blocks described by |new_extents| and, if it
// exists, the old version exists in |old_part| in the blocks described by
// |old_extents|. The operations added to |aops| reference the data blob
// in the |blob_file|. |old_deflates| and |new_deflates| are all deflate
// locations in |old_part| and |new_part|. Returns true on success.
bool DeltaReadFile(std::vector<AnnotatedOperation>* aops,
                   const std::string& old_part,
                   const std::string& new_part,
                   const std::vector<Extent>& old_extents,
                   const std::vector<Extent>& new_extents,
                   const std::vector<puffin::BitExtent>& old_deflates,
                   const std::vector<puffin::BitExtent>& new_deflates,
                   const std::string& name,
                   ssize_t chunk_blocks,
                   const PayloadVersion& version,
                   BlobFileWriter* blob_file);

// Reads the blocks |old_extents| from |old_part| (if it exists) and the
// |new_extents| from |new_part| and determines the smallest way to encode
// this |new_extents| for the diff. It stores necessary data in |out_data| and
// fills in |out_op|. If there's no change in old and new files, it creates a
// MOVE or SOURCE_COPY operation. If there is a change, the smallest of the
// operations allowed in the given |version| (REPLACE, REPLACE_BZ, BSDIFF,
// SOURCE_BSDIFF, or PUFFDIFF) wins.
// |new_extents| must not be empty. |old_deflates| and |new_deflates| are all
// the deflate locations in |old_part| and |new_part|. Returns true on success.
bool ReadExtentsToDiff(const std::string& old_part,
                       const std::string& new_part,
                       const std::vector<Extent>& old_extents,
                       const std::vector<Extent>& new_extents,
                       const std::vector<puffin::BitExtent>& old_deflates,
                       const std::vector<puffin::BitExtent>& new_deflates,
                       const PayloadVersion& version,
                       brillo::Blob* out_data,
                       InstallOperation* out_op);

// Generates the best allowed full operation to produce |new_data|. The allowed
// operations are based on |payload_version|. The operation blob will be stored
// in |out_blob| and the resulting operation type in |out_type|. Returns whether
// a valid full operation was generated.
bool GenerateBestFullOperation(const brillo::Blob& new_data,
                               const PayloadVersion& version,
                               brillo::Blob* out_blob,
                               InstallOperation::Type* out_type);

// Returns whether |op_type| is one of the REPLACE full operations.
bool IsAReplaceOperation(InstallOperation::Type op_type);

// Returns true if an operation with type |op_type| has no |src_extents|.
bool IsNoSourceOperation(InstallOperation::Type op_type);

// Returns true if |op| is a no-op operation that doesn't do any useful work
// (e.g., a move operation that copies blocks onto themselves).
bool IsNoopOperation(const InstallOperation& op);

// Filters all the operations that are no-op, maintaining the relative order
// of the rest of the operations.
void FilterNoopOperations(std::vector<AnnotatedOperation>* ops);

bool InitializePartitionInfo(const PartitionConfig& partition,
                             PartitionInfo* info);

// Compare two AnnotatedOperations by the start block of the first Extent in
// their destination extents.
bool CompareAopsByDestination(AnnotatedOperation first_aop,
                              AnnotatedOperation second_aop);

// Returns whether the filesystem is an ext[234] filesystem. In case of failure,
// such as if the file |device| doesn't exists or can't be read, it returns
// false.
bool IsExtFilesystem(const std::string& device);

// Returns the max number of threads to process the files(chunks) in parallel.
size_t GetMaxThreads();

// Returns the old file which file name has the shortest levenshtein distance to
// |new_file_name|.
FilesystemInterface::File GetOldFile(
    const std::map<std::string, FilesystemInterface::File>& old_files_map,
    const std::string& new_file_name);

}  // namespace diff_utils

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_GENERATOR_DELTA_DIFF_UTILS_H_
