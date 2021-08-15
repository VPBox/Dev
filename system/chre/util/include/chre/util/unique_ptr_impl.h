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

#ifndef CHRE_UTIL_UNIQUE_PTR_IMPL_H_
#define CHRE_UTIL_UNIQUE_PTR_IMPL_H_

#include "chre/util/unique_ptr.h"

#include <string.h>
#include <type_traits>
#include <utility>

#include "chre/util/container_support.h"
#include "chre/util/memory.h"

namespace chre {

template<typename ObjectType>
UniquePtr<ObjectType>::UniquePtr() : mObject(nullptr) {}

template<typename ObjectType>
UniquePtr<ObjectType>::UniquePtr(ObjectType *object) : mObject(object) {}

template<typename ObjectType>
UniquePtr<ObjectType>::UniquePtr(UniquePtr<ObjectType>&& other) {
  mObject = other.mObject;
  other.mObject = nullptr;
}

template<typename ObjectType>
template<typename OtherObjectType>
UniquePtr<ObjectType>::UniquePtr(UniquePtr<OtherObjectType>&& other) {
  mObject = other.mObject;
  other.mObject = nullptr;
}

template<typename ObjectType>
UniquePtr<ObjectType>::~UniquePtr() {
  reset();
}

template<typename ObjectType>
bool UniquePtr<ObjectType>::isNull() const {
  return (mObject == nullptr);
}

template<typename ObjectType>
ObjectType *UniquePtr<ObjectType>::get() const {
  return mObject;
}

template<typename ObjectType>
ObjectType *UniquePtr<ObjectType>::release() {
  ObjectType *obj = mObject;
  mObject = nullptr;
  return obj;
}

template<typename ObjectType>
void UniquePtr<ObjectType>::reset(ObjectType *object) {
  CHRE_ASSERT(object == nullptr || mObject != object);

  reset();
  mObject = object;
}

template<typename ObjectType>
void UniquePtr<ObjectType>::reset()  {
  if (mObject != nullptr) {
    mObject->~ObjectType();
    memoryFree(mObject);
    mObject = nullptr;
  }
}

template<typename ObjectType>
ObjectType *UniquePtr<ObjectType>::operator->() const {
  return get();
}

template<typename ObjectType>
ObjectType& UniquePtr<ObjectType>::operator*() const {
  return *get();
}

template<typename ObjectType>
ObjectType& UniquePtr<ObjectType>::operator[](size_t index) const {
  return get()[index];
}

template<typename ObjectType>
bool UniquePtr<ObjectType>::operator==(
    const UniquePtr<ObjectType> &other) const {
  return mObject == other.get();
}

template<typename ObjectType>
bool UniquePtr<ObjectType>::operator!=(
    const UniquePtr<ObjectType> &other) const {
  return !(*this == other);
}

template<typename ObjectType>
UniquePtr<ObjectType>& UniquePtr<ObjectType>::operator=(
    UniquePtr<ObjectType>&& other) {
  reset();
  mObject = other.mObject;
  other.mObject = nullptr;
  return *this;
}

template<typename ObjectType, typename... Args>
inline UniquePtr<ObjectType> MakeUnique(Args&&... args) {
  return UniquePtr<ObjectType>(memoryAlloc<ObjectType>(
      std::forward<Args>(args)...));
}

template<typename ObjectType>
inline UniquePtr<ObjectType> MakeUniqueZeroFill() {
  // For simplicity, we call memset *after* memoryAlloc<ObjectType>() - this is
  // only valid for types that have a trivial constructor. This utility function
  // is really meant to be used with trivial types only - if there's a desire to
  // zero things out in a non-trivial type, the right place for that is in its
  // constructor.
  static_assert(std::is_trivial<ObjectType>::value,
                "MakeUniqueZeroFill is only supported for trivial types");
  auto ptr = UniquePtr<ObjectType>(memoryAlloc<ObjectType>());
  if (!ptr.isNull()) {
    memset(ptr.get(), 0, sizeof(ObjectType));
  }
  return ptr;
}

}  // namespace chre

#endif  // CHRE_UTIL_UNIQUE_PTR_IMPL_H_
