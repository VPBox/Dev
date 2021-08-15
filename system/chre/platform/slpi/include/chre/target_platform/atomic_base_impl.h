/*
 * Copyright (C) 2019 The Android Open Source Project
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

#ifndef CHRE_PLATFORM_SLPI_ATOMIC_BASE_IMPL_H_
#define CHRE_PLATFORM_SLPI_ATOMIC_BASE_IMPL_H_

#include "chre/platform/atomic.h"

extern "C" {

#include "qurt_atomic_ops.h"

}  // extern "C"

namespace chre {

inline AtomicBool::AtomicBool(bool startingValue) {
  mValue = startingValue;
}

inline bool AtomicBool::operator=(bool desired) {
  store(desired);
  return desired;
}

inline bool AtomicBool::load() const {
  qurt_atomic_barrier();
  return mValue;
}

inline void AtomicBool::store(bool desired) {
  exchange(desired);
}

inline bool AtomicBool::exchange(bool desired) {
  qurt_atomic_barrier();
  return qurt_atomic_set(&mValue,
                         static_cast<unsigned int>(desired));
}

inline AtomicUint32::AtomicUint32(uint32_t startingValue) {
  mValue = startingValue;
}

inline uint32_t AtomicUint32::operator=(uint32_t desired) {
  store(desired);
  return desired;
}

inline uint32_t AtomicUint32::load() const {
  qurt_atomic_barrier();
  return mValue;
}

inline void AtomicUint32::store(uint32_t desired) {
  exchange(desired);
}

inline uint32_t AtomicUint32::exchange(uint32_t desired) {
  qurt_atomic_barrier();
  return qurt_atomic_set(&mValue, desired);
}

inline uint32_t AtomicUint32::fetch_add(uint32_t arg) {
  qurt_atomic_barrier();
  return qurt_atomic_add_return(&mValue, arg);
}

inline uint32_t AtomicUint32::fetch_increment() {
  qurt_atomic_barrier();
  return qurt_atomic_add_return(&mValue, 1);
}

inline uint32_t AtomicUint32::fetch_sub(uint32_t arg) {
  qurt_atomic_barrier();
  return qurt_atomic_sub_return(&mValue, arg);
}

inline uint32_t AtomicUint32::fetch_decrement() {
  qurt_atomic_barrier();
  return qurt_atomic_sub_return(&mValue, 1);
}

}  // namespace chre

#endif  // CHRE_PLATFORM_SLPI_ATOMIC_BASE_IMPL_H_
