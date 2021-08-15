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

#include "chre/util/dynamic_vector_base.h"

#include <cstdint>
#include <cstring>

#include "chre/util/container_support.h"

namespace chre {

DynamicVectorBase::DynamicVectorBase(DynamicVectorBase&& other)
    : mData(other.mData),
      mSize(other.mSize),
      mCapacity(other.mCapacity) {
  other.mData = nullptr;
  other.mSize = 0;
  other.mCapacity = 0;
}

bool DynamicVectorBase::doReserve(size_t newCapacity, size_t elementSize) {
  bool success = (newCapacity <= mCapacity);
  if (!success) {
    void *newData = memoryAlloc(newCapacity * elementSize);
    if (newData != nullptr) {
      memcpy(newData, mData, mSize * elementSize);
      memoryFree(mData);
      mData = newData;
      mCapacity = newCapacity;
      success = true;
    }
  }

  return success;
}

bool DynamicVectorBase::doPrepareForPush(size_t elementSize) {
  return doReserve(getNextGrowthCapacity(), elementSize);
}

size_t DynamicVectorBase::getNextGrowthCapacity() const {
  size_t newCapacity;
  if (mCapacity == 0) {
    newCapacity = 1;
  } else if (mSize == mCapacity) {
    newCapacity = mCapacity * 2;
  } else {
    newCapacity = mCapacity;
  }

  return newCapacity;
}

void DynamicVectorBase::doErase(size_t index, size_t elementSize) {
  mSize--;
  size_t moveAmount = (mSize - index) * elementSize;
  memmove(static_cast<uint8_t *>(mData) + (index * elementSize),
          static_cast<uint8_t *>(mData) + ((index + 1) * elementSize),
          moveAmount);
}

bool DynamicVectorBase::doPushBack(const void *element, size_t elementSize) {
  bool spaceAvailable = doPrepareForPush(elementSize);
  if (spaceAvailable) {
    memcpy(static_cast<uint8_t *>(mData) + (mSize * elementSize),
           element, elementSize);
    mSize++;
  }

  return spaceAvailable;
}

}  // namespace chre
