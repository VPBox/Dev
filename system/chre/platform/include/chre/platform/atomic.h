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

#ifndef CHRE_PLATFORM_ATOMIC_H_
#define CHRE_PLATFORM_ATOMIC_H_

#include "chre/target_platform/atomic_base.h"
#include "chre/util/non_copyable.h"

namespace chre {

/**
 * Provides an implementation of an atomic boolean. AtomicBoolBase is
 * subclassed here to allow platforms to use their own underlying atomic APIs.
 */
class AtomicBool : public AtomicBoolBase,
                   public NonCopyable {
 public:
  /**
   * Allows the platform to do any atomic initialization at construction time.
   *
   * @param The starting value for the object.
   */
  AtomicBool(bool startingValue);

  /**
   * Atomically assigns the desired value to the atomic object. Equivalent to
   * store().
   *
   * @param The value the object will be replaced with.
   *
   * @return The desired value.
   */
  bool operator=(bool desired);

  /**
   * Atomically loads the current value of the atomic object. Equivalent to
   * load().
   *
   * @return The current value of the object.
   */
  operator bool() const {
    return load();
  };

  /**
   * Atomically loads the current value of the atomic object.
   *
   * @return The current value of the object.
   */
  bool load() const;

  /**
   * Atomically replaces the current value of the atomic object.
   *
   * @param The value the object will be replaced with.
   */
  void store(bool desired);

  /**
   * Atomically replaces the value of the atomic object.
   *
   * @param The value the object should have when the method returns.
   *
   * @return The previous value of the object.
   */
  bool exchange(bool desired);
};

/**
 * Provides an implementation of an atomic uint32_t. AtomicUint32Base is
 * subclassed here to allow platforms to use their own underlying atomic APIs.
 */
class AtomicUint32 : public AtomicUint32Base,
                     public NonCopyable {
 public:
  /**
   * Allows the platform to do any atomic initialization at construction time.
   *
   * @param The starting value for the object.
   */
  AtomicUint32(uint32_t startingValue);

  /**
   * Atomically assigns the desired value to the atomic object. Equivalent to
   * store().
   *
   * @param The value the object will be replaced with.
   *
   * @return The desired value.
   */
  uint32_t operator=(uint32_t desired);

  /**
   * Atomically loads the current value of the atomic object. Equivalent to
   * load().
   *
   * @return The current value of the object.
   */
  operator uint32_t() const {
    return load();
  }

  /**
   * Atomically loads the current value of the atomic object.
   *
   * @return The current value of the object.
   */
  uint32_t load() const;

  /**
   * Atomically replaces the current value of the atomic object.
   *
   * @param The value the object will be replaced with.
   */
  void store(uint32_t desired);

  /**
   * Atomically replaces the value of the atomic object.
   *
   * @param The value the object should have when the method returns.
   *
   * @return The previous value of the object.
   */
  uint32_t exchange(uint32_t desired);

  /**
   * Atomically adds the argument to the current value of the object.
   *
   * @param The amount which the object should be increased by.
   *
   * @return The previous value of the object.
   */
  uint32_t fetch_add(uint32_t arg);

  /**
   * Atomically increments the value stored in the atomic object by 1.
   *
   * @return The previous value of the object.
   */
  uint32_t fetch_increment();

  /**
   * Atomically subtracts the argument from the current value of the object.
   *
   * @param The amount which the object should be decreased by.
   *
   * @return The previous value of the object.
   */
  uint32_t fetch_sub(uint32_t arg);

  /**
   * Atomically decrements the value stored in the atomic object by 1.
   *
   * @return The previous value of the object.
   */
  uint32_t fetch_decrement();
};

}  // namespace chre

#include "chre/target_platform/atomic_base_impl.h"

#endif  // CHRE_PLATFORM_ATOMIC_H_
