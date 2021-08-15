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

#include "update_engine/payload_consumer/file_descriptor_utils.h"

#include <algorithm>

#include <base/logging.h>

#include "update_engine/common/hash_calculator.h"
#include "update_engine/common/utils.h"
#include "update_engine/payload_consumer/extent_reader.h"
#include "update_engine/payload_consumer/extent_writer.h"

using google::protobuf::RepeatedPtrField;
using std::min;

namespace chromeos_update_engine {

namespace {

// Size of the buffer used to copy blocks.
const uint64_t kMaxCopyBufferSize = 1024 * 1024;

bool CommonHashExtents(FileDescriptorPtr source,
                       const RepeatedPtrField<Extent>& src_extents,
                       DirectExtentWriter* writer,
                       uint64_t block_size,
                       brillo::Blob* hash_out) {
  auto total_blocks = utils::BlocksInExtents(src_extents);
  auto buffer_blocks = kMaxCopyBufferSize / block_size;
  // Ensure we copy at least one block at a time.
  if (buffer_blocks < 1)
    buffer_blocks = 1;
  brillo::Blob buf(buffer_blocks * block_size);

  DirectExtentReader reader;
  TEST_AND_RETURN_FALSE(reader.Init(source, src_extents, block_size));

  HashCalculator source_hasher;
  while (total_blocks > 0) {
    auto read_blocks = std::min(total_blocks, buffer_blocks);
    TEST_AND_RETURN_FALSE(reader.Read(buf.data(), read_blocks * block_size));
    if (hash_out != nullptr) {
      TEST_AND_RETURN_FALSE(
          source_hasher.Update(buf.data(), read_blocks * block_size));
    }
    if (writer) {
      TEST_AND_RETURN_FALSE(
          writer->Write(buf.data(), read_blocks * block_size));
    }
    total_blocks -= read_blocks;
  }

  if (hash_out != nullptr) {
    TEST_AND_RETURN_FALSE(source_hasher.Finalize());
    *hash_out = source_hasher.raw_hash();
  }
  return true;
}

}  // namespace

namespace fd_utils {

bool CopyAndHashExtents(FileDescriptorPtr source,
                        const RepeatedPtrField<Extent>& src_extents,
                        FileDescriptorPtr target,
                        const RepeatedPtrField<Extent>& tgt_extents,
                        uint64_t block_size,
                        brillo::Blob* hash_out) {
  DirectExtentWriter writer;
  TEST_AND_RETURN_FALSE(writer.Init(target, tgt_extents, block_size));
  TEST_AND_RETURN_FALSE(utils::BlocksInExtents(src_extents) ==
                        utils::BlocksInExtents(tgt_extents));
  TEST_AND_RETURN_FALSE(
      CommonHashExtents(source, src_extents, &writer, block_size, hash_out));
  return true;
}

bool ReadAndHashExtents(FileDescriptorPtr source,
                        const RepeatedPtrField<Extent>& extents,
                        uint64_t block_size,
                        brillo::Blob* hash_out) {
  return CommonHashExtents(source, extents, nullptr, block_size, hash_out);
}

}  // namespace fd_utils

}  // namespace chromeos_update_engine
