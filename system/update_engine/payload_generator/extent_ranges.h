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

#ifndef UPDATE_ENGINE_PAYLOAD_GENERATOR_EXTENT_RANGES_H_
#define UPDATE_ENGINE_PAYLOAD_GENERATOR_EXTENT_RANGES_H_

#include <map>
#include <set>
#include <vector>

#include <base/macros.h>

#include "update_engine/update_metadata.pb.h"

// An ExtentRanges object represents an unordered collection of extents (and
// therefore blocks). Such an object may be modified by adding or subtracting
// blocks (think: set addition or set subtraction). Note that ExtentRanges
// ignores sparse hole extents mostly to avoid confusion between extending a
// sparse hole range vs. set addition but also to ensure that the delta
// generator doesn't use sparse holes as scratch space.

namespace chromeos_update_engine {

struct ExtentLess {
  bool operator()(const Extent& x, const Extent& y) const {
    return x.start_block() < y.start_block();
  }
};

Extent ExtentForRange(uint64_t start_block, uint64_t num_blocks);
Extent ExtentForBytes(uint64_t block_size,
                      uint64_t start_bytes,
                      uint64_t size_bytes);

class ExtentRanges {
 public:
  typedef std::set<Extent, ExtentLess> ExtentSet;

  ExtentRanges() : blocks_(0) {}
  void AddBlock(uint64_t block);
  void SubtractBlock(uint64_t block);
  void AddExtent(Extent extent);
  void SubtractExtent(const Extent& extent);
  void AddExtents(const std::vector<Extent>& extents);
  void SubtractExtents(const std::vector<Extent>& extents);
  void AddRepeatedExtents(
      const ::google::protobuf::RepeatedPtrField<Extent>& exts);
  void SubtractRepeatedExtents(
      const ::google::protobuf::RepeatedPtrField<Extent>& exts);
  void AddRanges(const ExtentRanges& ranges);
  void SubtractRanges(const ExtentRanges& ranges);

  // Returns whether the block |block| is in this ExtentRange.
  bool ContainsBlock(uint64_t block) const;

  static bool ExtentsOverlapOrTouch(const Extent& a, const Extent& b);
  static bool ExtentsOverlap(const Extent& a, const Extent& b);

  // Dumps contents to the log file. Useful for debugging.
  void Dump() const;

  uint64_t blocks() const { return blocks_; }
  const ExtentSet& extent_set() const { return extent_set_; }

  // Returns an ordered vector of extents for |count| blocks,
  // using extents in extent_set_. The returned extents are not
  // removed from extent_set_. |count| must be less than or equal to
  // the number of blocks in this extent set.
  std::vector<Extent> GetExtentsForBlockCount(uint64_t count) const;

 private:
  ExtentSet extent_set_;
  uint64_t blocks_;
};

// Filters out from the passed list of extents |extents| all the blocks in the
// ExtentRanges set. Note that the order of the blocks in |extents| is preserved
// omitting blocks present in the ExtentRanges |ranges|.
std::vector<Extent> FilterExtentRanges(const std::vector<Extent>& extents,
                                       const ExtentRanges& ranges);

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_GENERATOR_EXTENT_RANGES_H_
