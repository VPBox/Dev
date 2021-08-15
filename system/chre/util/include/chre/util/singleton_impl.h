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

#ifndef CHRE_UTIL_SINGLETON_IMPL_H_
#define CHRE_UTIL_SINGLETON_IMPL_H_

#include <new>
#include <utility>

#include "chre/util/singleton.h"

namespace chre {

template<typename ObjectType>
typename std::aligned_storage<sizeof(ObjectType), alignof(ObjectType)>::type
    Singleton<ObjectType>::sObject;

template<typename ObjectType>
bool Singleton<ObjectType>::sIsInitialized = false;

template<typename ObjectType>
template<typename... Args>
void Singleton<ObjectType>::init(Args&&... args) {
  if (!sIsInitialized) {
    sIsInitialized = true;
    new (get()) ObjectType(std::forward<Args>(args)...);
  }
}

template<typename ObjectType>
void Singleton<ObjectType>::deinit() {
  if (sIsInitialized) {
    get()->~ObjectType();
    sIsInitialized = false;
  }
}

template<typename ObjectType>
bool Singleton<ObjectType>::isInitialized() {
  return sIsInitialized;
}

template<typename ObjectType>
ObjectType *Singleton<ObjectType>::get() {
  return reinterpret_cast<ObjectType *>(&sObject);
}

template<typename ObjectType>
ObjectType *Singleton<ObjectType>::safeGet() {
  if (sIsInitialized) {
    return reinterpret_cast<ObjectType *>(&sObject);
  } else {
    return nullptr;
  }
}

}  // namespace chre

#endif  // CHRE_UTIL_SINGLETON_IMPL_H_
