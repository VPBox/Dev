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

#include <nvram/core/storage.h>

namespace nvram {
namespace storage {
namespace {

// Maximum number of space blobs supported.
const int kMaxSpaces = 32;

class StorageSlot {
 public:
  bool present() const { return blob_.size() != 0; }

  Status Load(Blob* blob) const {
    if (blob_.size() == 0) {
      return Status::kNotFound;
    }

    if (!blob->Assign(blob_.data(), blob_.size())) {
      return Status::kStorageError;
    }

    return Status::kSuccess;
  }

  Status Store(const Blob& blob) {
    if (!blob_.Assign(blob.data(), blob.size())) {
      return Status::kStorageError;
    }

    return Status::kSuccess;
  }

  Status Delete() {
    return blob_.Resize(0) ? Status::kSuccess : Status::kStorageError;
  }

 private:
  Blob blob_;
};

// Stores the header blob.
StorageSlot g_header;

// Stores the space blobs.
struct {
  uint32_t index;
  StorageSlot slot;
} g_spaces[kMaxSpaces];

// Find the storage slot in |g_spaces| that corresponds to |index|. Returns
// |nullptr| if no matching slot exists.
StorageSlot* FindSpaceSlot(uint32_t index) {
  for (size_t i = 0; i < kMaxSpaces; ++i) {
    if (g_spaces[i].slot.present() && g_spaces[i].index == index) {
      return &g_spaces[i].slot;
    }
  }

  return nullptr;
}

}  // namespace

Status LoadHeader(Blob* blob) {
  return g_header.Load(blob);
}

Status StoreHeader(const Blob& blob) {
  return g_header.Store(blob);
}

Status LoadSpace(uint32_t index, Blob* blob) {
  StorageSlot* slot = FindSpaceSlot(index);
  return slot ? slot->Load(blob) : Status::kNotFound;
}

Status StoreSpace(uint32_t index, const Blob& blob) {
  StorageSlot* slot = FindSpaceSlot(index);
  if (slot) {
    return slot->Store(blob);
  }

  // Allocate a new slot.
  for (size_t i = 0; i < kMaxSpaces; ++i) {
    if (!g_spaces[i].slot.present()) {
      g_spaces[i].index = index;
      return g_spaces[i].slot.Store(blob);
    }
  }

  return Status::kStorageError;
}

Status DeleteSpace(uint32_t index) {
  StorageSlot* slot = FindSpaceSlot(index);
  if (slot) {
    slot->Delete();
  }

  return Status::kSuccess;
}

}  // namespace storage
}  // namespace nvram
