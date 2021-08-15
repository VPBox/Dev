//
// Copyright (C) 2009 The Android Open Source Project
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

#include "update_engine/payload_generator/extent_utils.h"

#include <inttypes.h>

#include <string>
#include <utility>
#include <vector>

#include <base/logging.h>
#include <base/macros.h>
#include <base/strings/stringprintf.h>

#include "update_engine/payload_consumer/payload_constants.h"
#include "update_engine/payload_generator/annotated_operation.h"
#include "update_engine/payload_generator/extent_ranges.h"

using std::string;
using std::vector;

namespace chromeos_update_engine {

void AppendBlockToExtents(vector<Extent>* extents, uint64_t block) {
  // First try to extend the last extent in |extents|, if any.
  if (!extents->empty()) {
    Extent& extent = extents->back();
    uint64_t next_block = extent.start_block() == kSparseHole
                              ? kSparseHole
                              : extent.start_block() + extent.num_blocks();
    if (next_block == block) {
      extent.set_num_blocks(extent.num_blocks() + 1);
      return;
    }
  }
  // If unable to extend the last extent, append a new single-block extent.
  Extent new_extent;
  new_extent.set_start_block(block);
  new_extent.set_num_blocks(1);
  extents->push_back(new_extent);
}

void ExtendExtents(
    google::protobuf::RepeatedPtrField<Extent>* extents,
    const google::protobuf::RepeatedPtrField<Extent>& extents_to_add) {
  vector<Extent> extents_vector;
  vector<Extent> extents_to_add_vector;
  ExtentsToVector(*extents, &extents_vector);
  ExtentsToVector(extents_to_add, &extents_to_add_vector);
  extents_vector.insert(extents_vector.end(),
                        extents_to_add_vector.begin(),
                        extents_to_add_vector.end());
  NormalizeExtents(&extents_vector);
  extents->Clear();
  StoreExtents(extents_vector, extents);
}

// Stores all Extents in 'extents' into 'out'.
void StoreExtents(const vector<Extent>& extents,
                  google::protobuf::RepeatedPtrField<Extent>* out) {
  for (const Extent& extent : extents) {
    Extent* new_extent = out->Add();
    *new_extent = extent;
  }
}

// Stores all extents in |extents| into |out_vector|.
void ExtentsToVector(const google::protobuf::RepeatedPtrField<Extent>& extents,
                     vector<Extent>* out_vector) {
  out_vector->clear();
  for (int i = 0; i < extents.size(); i++) {
    out_vector->push_back(extents.Get(i));
  }
}

string ExtentsToString(const vector<Extent>& extents) {
  string ext_str;
  for (const Extent& e : extents)
    ext_str += base::StringPrintf("[%" PRIu64 ", %" PRIu64 "] ",
                                  static_cast<uint64_t>(e.start_block()),
                                  static_cast<uint64_t>(e.num_blocks()));
  return ext_str;
}

void NormalizeExtents(vector<Extent>* extents) {
  vector<Extent> new_extents;
  for (const Extent& curr_ext : *extents) {
    if (new_extents.empty()) {
      new_extents.push_back(curr_ext);
      continue;
    }
    Extent& last_ext = new_extents.back();
    if (last_ext.start_block() + last_ext.num_blocks() ==
        curr_ext.start_block()) {
      // If the extents are touching, we want to combine them.
      last_ext.set_num_blocks(last_ext.num_blocks() + curr_ext.num_blocks());
    } else {
      // Otherwise just include the extent as is.
      new_extents.push_back(curr_ext);
    }
  }
  *extents = new_extents;
}

vector<Extent> ExtentsSublist(const vector<Extent>& extents,
                              uint64_t block_offset,
                              uint64_t block_count) {
  vector<Extent> result;
  uint64_t scanned_blocks = 0;
  if (block_count == 0)
    return result;
  uint64_t end_block_offset = block_offset + block_count;
  for (const Extent& extent : extents) {
    // The loop invariant is that if |extents| has enough blocks, there's
    // still some extent to add to |result|. This implies that at the beginning
    // of the loop scanned_blocks < block_offset + block_count.
    if (scanned_blocks + extent.num_blocks() > block_offset) {
      // This case implies that |extent| has some overlapping with the requested
      // subsequence.
      uint64_t new_start = extent.start_block();
      uint64_t new_num_blocks = extent.num_blocks();
      if (scanned_blocks + new_num_blocks > end_block_offset) {
        // Cut the end part of the extent.
        new_num_blocks = end_block_offset - scanned_blocks;
      }
      if (block_offset > scanned_blocks) {
        // Cut the begin part of the extent.
        new_num_blocks -= block_offset - scanned_blocks;
        new_start += block_offset - scanned_blocks;
      }
      result.push_back(ExtentForRange(new_start, new_num_blocks));
    }
    scanned_blocks += extent.num_blocks();
    if (scanned_blocks >= end_block_offset)
      break;
  }
  return result;
}

bool operator==(const Extent& a, const Extent& b) {
  return a.start_block() == b.start_block() && a.num_blocks() == b.num_blocks();
}

}  // namespace chromeos_update_engine
