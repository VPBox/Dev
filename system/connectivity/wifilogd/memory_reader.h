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

#ifndef MEMORY_READER_H_
#define MEMORY_READER_H_

#include <cstdint>
#include <tuple>

#include "wifilogd/local_utils.h"

namespace android {
namespace wifilogd {

// A non-owning adapter, which helps with reading protocol messages out
// of a buffer. This is, essentially, the inverse of ByteBuffer.
//
// The user is responsible for ensuring that a MemoryReader does not
// outlive the underlying buffer.
class MemoryReader {
 public:
  MemoryReader(const void* buf, size_t buflen)
      : head_(static_cast<const uint8_t*>(buf)), n_bytes_avail_(buflen) {}

  explicit MemoryReader(std::tuple<const uint8_t*, size_t> buf)
      : head_(std::get<0>(buf)), n_bytes_avail_(std::get<1>(buf)) {}

  // Copies a |T| out of our referenced memory, aborting if sizeof(T) exceeds
  // the number of bytes available. On success, updates the read position, and
  // the number of bytes available.
  template <typename T>
  T CopyOutOrDie() {
    CHECK(head_);
    const T out{local_utils::CopyFromBufferOrDie<T>(head_, n_bytes_avail_)};
    head_ += sizeof(T);
    n_bytes_avail_ -= sizeof(T);
    return out;
  }

  // Returns a pointer to the next bytes available for reading. Aborts if
  // the number of available bytes is less than |n_bytes|. On success, updates
  // the read position, and the number of bytes available.
  RETURNS_NONNULL const uint8_t* GetBytesOrDie(size_t n_bytes);

  // Returns true if the referenced memory is non-NULL, and has at least
  // one byte available for reading.
  explicit operator bool() const { return head_ && n_bytes_avail_; }

  // Returns the number of bytes available for reading.
  size_t size() const { return (head_ ? n_bytes_avail_ : 0); }

 private:
  const uint8_t* head_;  // non-owned
  size_t n_bytes_avail_;
};

}  // namespace wifilogd
}  // namespace android

#endif  // MEMORY_READER_H_
