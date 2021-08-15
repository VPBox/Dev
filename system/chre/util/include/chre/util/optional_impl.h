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

#ifndef UTIL_CHRE_OPTIONAL_IMPL_H_
#define UTIL_CHRE_OPTIONAL_IMPL_H_

#include <new>
#include <utility>

#include "chre/util/optional.h"

namespace chre {

template<typename ObjectType>
Optional<ObjectType>::Optional(const ObjectType& object) {
  new (objectAddr()) ObjectType(object);
  mHasValue = true;
}

template<typename ObjectType>
Optional<ObjectType>::Optional(ObjectType&& object) {
  new (objectAddr()) ObjectType(std::move(object));
  mHasValue = true;
}

template<typename ObjectType>
Optional<ObjectType>::~Optional() {
  reset();
}

template<typename ObjectType>
bool Optional<ObjectType>::has_value() const {
  return mHasValue;
}

template<typename ObjectType>
void Optional<ObjectType>::reset() {
  if (mHasValue) {
    object().~ObjectType();
    mHasValue = false;
  }
}

template<typename ObjectType>
ObjectType& Optional<ObjectType>::value() {
  return object();
}

template<typename ObjectType>
const ObjectType& Optional<ObjectType>::value() const {
  return object();
}

template<typename ObjectType>
Optional<ObjectType>& Optional<ObjectType>::operator=(ObjectType&& other) {
  if (mHasValue) {
    object() = std::move(other);
  } else {
    new (objectAddr()) ObjectType(std::move(other));
  }

  mHasValue = true;
  return *this;
}

template<typename ObjectType>
Optional<ObjectType>& Optional<ObjectType>::operator=(
    Optional<ObjectType>&& other) {
  if (mHasValue) {
    if (other.mHasValue) {
      object() = std::move(other.object());
    } else {
      reset();
    }
  } else if (other.mHasValue) {
    new (objectAddr()) ObjectType(std::move(other.object()));
  }

  mHasValue = other.mHasValue;
  return *this;
}

template<typename ObjectType>
Optional<ObjectType>& Optional<ObjectType>::operator=(const ObjectType& other) {
  if (mHasValue) {
    object() = std::move(other);
  } else {
    new (objectAddr()) ObjectType(other);
  }

  mHasValue = true;
  return *this;
}

template<typename ObjectType>
Optional<ObjectType>& Optional<ObjectType>::operator=(
    const Optional<ObjectType>& other) {
  if (mHasValue) {
    if (other.mHasValue) {
      object() = other.object();
    } else {
      reset();
    }
  } else if (other.mHasValue) {
    new (objectAddr()) ObjectType(other.object());
  }

  mHasValue = other.mHasValue;
  return *this;
}

template<typename ObjectType>
ObjectType& Optional<ObjectType>::operator*() {
  return object();
}

template<typename ObjectType>
const ObjectType& Optional<ObjectType>::operator*() const {
  return object();
}

template<typename ObjectType>
ObjectType *Optional<ObjectType>::operator->() {
  return objectAddr();
}

template<typename ObjectType>
const ObjectType *Optional<ObjectType>::operator->() const {
  return objectAddr();
}

template<typename ObjectType>
ObjectType& Optional<ObjectType>::object() {
  return *objectAddr();
}

template<typename ObjectType>
const ObjectType& Optional<ObjectType>::object() const {
  return *objectAddr();
}

template<typename ObjectType>
ObjectType *Optional<ObjectType>::objectAddr() {
  return reinterpret_cast<ObjectType *>(&mObject);
}

template<typename ObjectType>
const ObjectType *Optional<ObjectType>::objectAddr() const {
  return reinterpret_cast<const ObjectType *>(&mObject);
}

}  // namespace chre

#endif  // UTIL_CHRE_OPTIONAL_IMPL_H_
