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

#include "chre/util/buffer_base.h"

#include <cstring>

#include "chre/util/container_support.h"

namespace chre {

BufferBase::~BufferBase() {
  reset();
}

void BufferBase::wrap(void *buffer, size_t size) {
  // If buffer is nullptr, size must also be 0.
  CHRE_ASSERT(buffer != nullptr || size == 0);
  reset();
  mBuffer = buffer;
  mSize = size;
}

bool BufferBase::copy_array(const void *buffer, size_t size,
                            size_t elementSize) {
  // If buffer is nullptr, size must also be 0.
  CHRE_ASSERT(buffer != nullptr || size == 0);

  reset();
  bool success = (size == 0);
  if (!success) {
    size_t copyBytes = size * elementSize;
    mBuffer = memoryAlloc(copyBytes);
    if (mBuffer != nullptr) {
      mBufferRequiresFree = true;
      memcpy(mBuffer, buffer, copyBytes);
      mSize = size;
      success = true;
    }
  }

  return success;
}

void BufferBase::reset() {
  if (mBufferRequiresFree) {
    mBufferRequiresFree = false;
    memoryFree(mBuffer);
  }

  mBuffer = nullptr;
  mSize = 0;
}

}  // namespace chre
