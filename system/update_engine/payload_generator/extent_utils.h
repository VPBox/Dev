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

#ifndef UPDATE_ENGINE_PAYLOAD_GENERATOR_EXTENT_UTILS_H_
#define UPDATE_ENGINE_PAYLOAD_GENERATOR_EXTENT_UTILS_H_

#include <string>
#include <vector>

#include "update_engine/payload_consumer/payload_constants.h"
#include "update_engine/update_metadata.pb.h"

// Utility functions for manipulating Extents and lists of blocks.

namespace chromeos_update_engine {

// |block| must either be the next block in the last extent or a block
// in the next extent. This function will not handle inserting block
// into an arbitrary place in the extents.
void AppendBlockToExtents(std::vector<Extent>* extents, uint64_t block);

// Takes a collection (vector or RepeatedPtrField) of Extent and
// returns a vector of the blocks referenced, in order.
template <typename T>
std::vector<uint64_t> ExpandExtents(const T& extents) {
  std::vector<uint64_t> ret;
  for (const auto& extent : extents) {
    if (extent.start_block() == kSparseHole) {
      ret.resize(ret.size() + extent.num_blocks(), kSparseHole);
    } else {
      for (uint64_t block = extent.start_block();
           block < (extent.start_block() + extent.num_blocks());
           block++) {
        ret.push_back(block);
      }
    }
  }
  return ret;
}

// Stores all Extents in 'extents' into 'out'.
void StoreExtents(const std::vector<Extent>& extents,
                  google::protobuf::RepeatedPtrField<Extent>* out);

// Stores all extents in |extents| into |out_vector|.
void ExtentsToVector(const google::protobuf::RepeatedPtrField<Extent>& extents,
                     std::vector<Extent>* out_vector);

// Returns a string representing all extents in |extents|.
std::string ExtentsToString(const std::vector<Extent>& extents);

// Takes a pointer to extents |extents| and extents |extents_to_add|, and
// merges them by adding |extents_to_add| to |extents| and normalizing.
void ExtendExtents(
    google::protobuf::RepeatedPtrField<Extent>* extents,
    const google::protobuf::RepeatedPtrField<Extent>& extents_to_add);

// Takes a vector of extents and normalizes those extents. Expects the extents
// to be sorted by start block. E.g. if |extents| is [(1, 2), (3, 5), (10, 2)]
// then |extents| will be changed to [(1, 7), (10, 2)].
void NormalizeExtents(std::vector<Extent>* extents);

// Return a subsequence of the list of blocks passed. Both the passed list of
// blocks |extents| and the return value are expressed as a list of Extent, not
// blocks. The returned list skips the first |block_offset| blocks from the
// |extents| and cotains |block_count| blocks (or less if |extents| is shorter).
std::vector<Extent> ExtentsSublist(const std::vector<Extent>& extents,
                                   uint64_t block_offset,
                                   uint64_t block_count);

bool operator==(const Extent& a, const Extent& b);

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_GENERATOR_EXTENT_UTILS_H_
