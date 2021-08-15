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

#ifndef UPDATE_ENGINE_PAYLOAD_GENERATOR_DEFLATE_UTILS_H_
#define UPDATE_ENGINE_PAYLOAD_GENERATOR_DEFLATE_UTILS_H_

#include <puffin/puffdiff.h>
#include <vector>

#include "update_engine/payload_generator/filesystem_interface.h"
#include "update_engine/payload_generator/payload_generation_config.h"

namespace chromeos_update_engine {
namespace deflate_utils {

// Gets the files from the partition and processes all its files. Processing
// includes:
//  - splitting large Squashfs containers into its smaller files.
//  - extracting deflates in zip and gzip files.
bool PreprocessPartitionFiles(const PartitionConfig& part,
                              std::vector<FilesystemInterface::File>* result,
                              bool extract_deflates);

// Spreads all extents in |over_extents| over |base_extents|. Here we assume the
// |over_extents| are non-overlapping and sorted by their offset.
//
// |base_extents|:
// |               -----------------------        ------         --------------
// |over_extents|:
// |  ==========  ====    ==========  ======
// |over_extents| is transforms to:
// |                 ==========  ====    =        ======         ===  ======
//
bool ShiftExtentsOverExtents(const std::vector<Extent>& base_extents,
                             std::vector<Extent>* over_extents);

// Spreads all extents in |over_extents| over |base_extents|. Here we assume the
// |over_extents| are non-overlapping and sorted by their offset. An item in
// |over_extents| is removed if it is spread in two or more extents in
// |base_extents|.
//
// |base_extents|:
// |               -----------------------        ------         --------------
// |over_extents|:
// |  ==========  ====    ==========  ======
// |over_extents| is transforms to:
// |                 ==========  ====                                 ======
//
bool ShiftBitExtentsOverExtents(const std::vector<Extent>& base_extents,
                                std::vector<puffin::BitExtent>* over_extents);

// Finds all deflate locations in |deflates| that are inside an Extent in
// |extents|. This function should not change the order of deflates.
std::vector<puffin::BitExtent> FindDeflates(
    const std::vector<Extent>& extents,
    const std::vector<puffin::BitExtent>& deflates);

// Creates a new list of deflate locations (|out_deflates|) from |in_deflates|
// by assuming all extents in the |extents| have been put together
// linearly. This function assumes that all deflate locations given in
// |in_deflates| are located somewhere in the |extents|. |out_deflates| should
// be empty on call.
//
// |extents|:
// |               -----------------------        ------         --------------
// |in_deflates|:
// |                   ========  ====              ====           ======
// |out_deflates|:
// |    ========  ====      ====  ======
//
bool CompactDeflates(const std::vector<Extent>& extents,
                     const std::vector<puffin::BitExtent>& in_deflates,
                     std::vector<puffin::BitExtent>* out_deflates);

// Combines |FindDeflates| and |CompcatDeflates| for ease of use.
bool FindAndCompactDeflates(const std::vector<Extent>& extents,
                            const std::vector<puffin::BitExtent>& in_deflates,
                            std::vector<puffin::BitExtent>* out_deflates);

// Expands a BitExtents to a ByteExtent.
puffin::ByteExtent ExpandToByteExtent(const puffin::BitExtent& extent);

}  // namespace deflate_utils
}  // namespace chromeos_update_engine
#endif  // UPDATE_ENGINE_PAYLOAD_GENERATOR_DEFLATE_UTILS_H_
