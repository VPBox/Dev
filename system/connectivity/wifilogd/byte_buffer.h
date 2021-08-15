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

#ifndef BYTE_BUFFER_H_
#define BYTE_BUFFER_H_

#include <array>
#include <cstring>

#include "android-base/logging.h"

#include "wifilogd/local_utils.h"

namespace android {
namespace wifilogd {

// A fixed-size buffer, which provides the ability to accumulate bytes.
// The buffer tracks its (populated) size, and does not require dynamic
// memory allocation.
//
// Usage could be as follows:
//     const auto buffer = ByteBuffer<1024>()
//         .AppendOrDie(header.data(), header.size())
//         .AppendOrDie(body.data(), body.size());
//     write(fd, buffer.data(), buffer.size());
template <size_t SizeBytes>
class ByteBuffer {
 public:
  ByteBuffer() : write_pos_(0) {}

  // Appends data to the end of this buffer. Aborts if the available
  // space in the buffer is less than |data_len|. Returns a reference to
  // the ByteBuffer, to support chaining.
  ByteBuffer<SizeBytes>& AppendOrDie(NONNULL const void* data,
                                     size_t data_len) {
    CHECK(data_len <= raw_buffer_.size() - write_pos_);
    std::memcpy(raw_buffer_.data() + write_pos_, data, data_len);
    write_pos_ += data_len;
    return *this;
  }

  // Returns a pointer to the head of this buffer.
  RETURNS_NONNULL const uint8_t* data() const { return raw_buffer_.data(); }

  // Returns the number of bytes written to this buffer.
  size_t size() const { return write_pos_; }

 private:
  std::array<uint8_t, SizeBytes> raw_buffer_;
  size_t write_pos_;
};

}  // namespace wifilogd
}  // namespace android
#endif  // BYTE_BUFFER_H_
