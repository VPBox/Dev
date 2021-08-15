/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef NVRAM_CORE_PERSISTENCE_H_
#define NVRAM_CORE_PERSISTENCE_H_

extern "C" {
#include <stdint.h>
}  // extern "C"

#include <nvram/messages/optional.h>
#include <nvram/messages/struct.h>
#include <nvram/messages/vector.h>

#include <nvram/core/storage.h>

namespace nvram {

// The NVRAM header data structure, which holds global information used by the
// NVRAM service, such as version and a list of defined spaces.
struct NvramHeader {
  // State flags affecting all spaces.
  enum Flags {
    kFlagDisableCreate = 1 << 0,
  };

  // Check whether a flag is present.
  bool HasFlag(Flags flag) const {
    return (flags & flag) != 0;
  }

  // Set a flag.
  void SetFlag(Flags flag) {
    flags |= flag;
  }

  // The current major header version. Bump this upon making
  // forward-incompatible changes to the storage format. Old versions will
  // reject the header on load and refuse to operate when they encounter a
  // version that is larger than the compile-time one.
  static constexpr uint32_t kVersion = 1;

  // The header version, indicating the data format revision used when the
  // header was last written. On load, if the version is more recent then what
  // the code can handle, we bail out. This allows making forward-incompatible
  // changes to the data format with the guarantee that old code versions won't
  // clobber new data. This is a last resort kill switch for old code, in
  // general we should aim for maximum compatibility between versions.
  uint32_t version = kVersion;

  // Current header flags. Bitwise OR of |NvramHeader::Flags| values.
  uint32_t flags = 0;

  // A list of allocated indices, in no particular order.
  Vector<uint32_t> allocated_indices;

  // An index that is in the process of being created or deleted. This field is
  // used as follows:
  //  * On space creation, we add the new space's index both to
  //    |allocated_indices| and set it as the |provisional_index|. Then, the
  //    header is written, followed by the space data. If we crash in between,
  //    the next load will find |provisional_index| present and will check
  //    whether that space is present on disk or not. If not, it'll clear the
  //    index from |allocated_indices| and reset |provisional_index|, hence the
  //    space is as good as never created.
  //  * On space deletion, the deleted space's index is removed from
  //    |allocated_indices|, but stored in |provisional_index|. The, the header
  //    gets written, followed by the space deletion. If we crash in between ,
  //    the next load will find the provisional index set, but the space is
  //    absent in |allocated_indices|. If this is the case, the initialization
  //    code will make sure to delete the space data if it's still around and
  //    clear |provisional_index| afterwards.
  Optional<uint32_t> provisional_index;
};

// All data corresponding to a single NVRAM space is held in an NvramSpace
// structure. There is one structure per allocated index.
struct NvramSpace {
  // Flags indicating internal status in effect for a space.
  enum Flags {
    kFlagWriteLocked = 1 << 0,
  };

  // Check whether a given flag is set.
  bool HasFlag(Flags flag) const {
    return (flags & flag) != 0;
  }

  // Set a flag.
  void SetFlag(Flags flag) {
    flags |= flag;
  }

  // A helper to simplify checking control flags.
  bool HasControl(uint32_t control) const {
    return (controls & (1 << control)) != 0;
  }

  // Persistent space flags. Bitwise OR of |NvramSpace::Flags| values.
  uint32_t flags = 0;

  // A bitmask of CONTROL_XYZ values in effect for the space. These are set at
  // space creation time and generally not touched afterwards.
  uint32_t controls = 0;

  // The authorization value for the space. This is a shared secret that must be
  // provided to read and write the space as specified by the appropriate
  // |controls| flags.
  Blob authorization_value;

  // The space payload data.
  Blob contents;
};

namespace persistence {

// Load NVRAM header from storage.
storage::Status LoadHeader(NvramHeader* header);

// Write the NVRAM header to storage.
storage::Status StoreHeader(const NvramHeader& header);

// Load NVRAM space data for a given index from storage.
storage::Status LoadSpace(uint32_t index, NvramSpace* space);

// Write the NVRAM space data for the given index to storage.
storage::Status StoreSpace(uint32_t index, const NvramSpace& space);

// Delete the stored NVRAM space data for the given index.
storage::Status DeleteSpace(uint32_t index);

}  // namespace persistence

}  // namespace nvram

#endif  // NVRAM_CORE_PERSISTENCE_H_
