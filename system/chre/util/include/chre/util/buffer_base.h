/*
 * Copyright (C) 2018 The Android Open Source Project
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

#ifndef CHRE_UTIL_BUFFER_BASE_H_
#define CHRE_UTIL_BUFFER_BASE_H_

#include <cstddef>

#include "chre/util/non_copyable.h"

namespace chre {

class BufferBase : public NonCopyable {
 protected:
  /**
   * Cleans up for the buffer. If the buffer is currently owned by this object,
   * it is released.
   */
  ~BufferBase();

  //! The buffer to manage.
  void *mBuffer = nullptr;

  //! The number of elements in the buffer.
  size_t mSize = 0;

  //! Set to true when mBuffer needs to be released by the destructor.
  bool mBufferRequiresFree = false;

  /**
   * @see Buffer::wrap.
   */
  void wrap(void *buffer, size_t size);

  /**
   * @see Buffer::copy_array.
   *
   * @param elementSize The size of each element. This is multiplied by size
   *        to determine the effective size of the buffer to copy.
   */
  bool copy_array(const void *buffer, size_t size, size_t elementSize);

 private:
  /**
   * Cleans up the buffer so a new one can be wrapped or copied. If the current
   * buffer is owned by this object, it is released.
   */
  void reset();
};

}  // namespace chre

#endif  // CHRE_UTIL_BUFFER_BASE_H_
