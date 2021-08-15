//
// Copyright (C) 2017 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_PAYLOAD_CONSUMER_FILE_DESCRIPTOR_UTILS_H_
#define UPDATE_ENGINE_PAYLOAD_CONSUMER_FILE_DESCRIPTOR_UTILS_H_

#include <brillo/secure_blob.h>

#include "update_engine/payload_consumer/file_descriptor.h"
#include "update_engine/update_metadata.pb.h"

namespace chromeos_update_engine {
namespace fd_utils {

// Copy blocks from the |source| file to the |target| file and hashes the
// contents. The blocks to copy from the |source| to the |target| files are
// specified by the |src_extents| and |tgt_extents| list of Extents, which
// must have the same length in number of blocks. Stores the hash of the
// copied blocks in Blob pointed by |hash_out| if not null. The block size
// is passed as |block_size|. In case of error reading or writing, returns
// false and the value pointed by |hash_out| is undefined.
// The |source| and |target| files must be different, or otherwise |src_extents|
// and |tgt_extents| must not overlap.
bool CopyAndHashExtents(
    FileDescriptorPtr source,
    const google::protobuf::RepeatedPtrField<Extent>& src_extents,
    FileDescriptorPtr target,
    const google::protobuf::RepeatedPtrField<Extent>& tgt_extents,
    uint64_t block_size,
    brillo::Blob* hash_out);

// Reads blocks from |source| and calculates the hash. The blocks to read are
// specified by |extents|. Stores the hash in |hash_out| if it is not null. The
// block sizes are passed as |block_size|. In case of error reading, it returns
// false and the value pointed by |hash_out| is undefined.
bool ReadAndHashExtents(
    FileDescriptorPtr source,
    const google::protobuf::RepeatedPtrField<Extent>& extents,
    uint64_t block_size,
    brillo::Blob* hash_out);

}  // namespace fd_utils
}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_CONSUMER_FILE_DESCRIPTOR_UTILS_H_
