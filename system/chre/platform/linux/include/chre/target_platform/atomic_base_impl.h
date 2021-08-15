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

#ifndef CHRE_PLATFORM_LINUX_ATOMIC_BASE_IMPL_H_
#define CHRE_PLATFORM_LINUX_ATOMIC_BASE_IMPL_H_

#include "chre/platform/atomic.h"

namespace chre {

inline AtomicBool::AtomicBool(bool startingValue) {
  std::atomic_init(&mAtomic, startingValue);
}

inline bool AtomicBool::operator=(bool desired) {
  return mAtomic = desired;
}

inline bool AtomicBool::load() const {
  return mAtomic.load();
}

inline void AtomicBool::store(bool desired) {
  mAtomic.store(desired);
}

inline bool AtomicBool::exchange(bool desired) {
  return mAtomic.exchange(desired);
}

inline AtomicUint32::AtomicUint32(uint32_t startingValue) {
  std::atomic_init(&mAtomic, startingValue);
}

inline uint32_t AtomicUint32::operator=(uint32_t desired) {
  return mAtomic = desired;
}

inline uint32_t AtomicUint32::load() const {
  return mAtomic.load();
}

inline void AtomicUint32::store(uint32_t desired) {
  mAtomic.store(desired);
}

inline uint32_t AtomicUint32::exchange(uint32_t desired) {
  return mAtomic.exchange(desired);
}

inline uint32_t AtomicUint32::fetch_add(uint32_t arg) {
  return mAtomic.fetch_add(arg);
}

inline uint32_t AtomicUint32::fetch_increment() {
  return mAtomic.fetch_add(1);
}

inline uint32_t AtomicUint32::fetch_sub(uint32_t arg) {
  return mAtomic.fetch_sub(arg);
}

inline uint32_t AtomicUint32::fetch_decrement() {
  return mAtomic.fetch_sub(1);
}

}  // namespace chre

#endif  // CHRE_PLATFORM_LINUX_ATOMIC_BASE_IMPL_H_
