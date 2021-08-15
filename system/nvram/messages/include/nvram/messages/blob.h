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

#ifndef NVRAM_MESSAGES_BLOB_H_
#define NVRAM_MESSAGES_BLOB_H_

extern "C" {
#include <stddef.h>
#include <stdint.h>
}

#include <nvram/messages/compiler.h>

namespace nvram {

// A simple wrapper class holding binary data of fixed size.
//
// This is intended for use in restricted environments where there is no full
// C++ standard library available and/or memory allocation failure must be
// handled gracefully.
class NVRAM_EXPORT Blob {
 public:
  Blob();
  ~Blob();

  // Blob is movable, but not copyable since the latter requires memory
  // allocations, which can fail.
  Blob(const Blob& other) = delete;
  Blob& operator=(const Blob& other) = delete;
  Blob(Blob&& other);
  Blob& operator=(Blob&& other);
  friend void swap(Blob& first, Blob& second);

  uint8_t* data() { return data_; }
  const uint8_t* data() const { return data_; }

  size_t size() const { return size_; }

  // Reallocate the underlying buffer to hold |size| bytes and copy in |data|.
  // Returns true on success, false if memory allocation fails. Blob size and
  // contents remain unchanged upon failure.
  bool Assign(const void* data, size_t size) NVRAM_WARN_UNUSED_RESULT;

  // Resize the blob to |size|. Existing data within the new |size| limit is
  // retained. If |size| increases, new contents are unspecified. Returns true
  // on success, false if memory allocation fails. Blob size and contents remain
  // unchanged upon failure.
  //
  // Note that calling this function invalidates pointers to the memory block
  // backing this |Blob|. You must call |data()| after |Resize()| returns to
  // obtain fresh valid pointers.
  bool Resize(size_t size) NVRAM_WARN_UNUSED_RESULT;

 private:
  uint8_t* data_ = nullptr;
  size_t size_ = 0;
};

}  // namespace nvram

#endif  // NVRAM_MESSAGES_BLOB_H_
