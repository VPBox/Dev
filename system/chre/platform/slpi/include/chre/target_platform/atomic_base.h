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

#ifndef CHRE_PLATFORM_SLPI_ATOMIC_BASE_H_
#define CHRE_PLATFORM_SLPI_ATOMIC_BASE_H_

namespace chre {

/**
 * The SLPI implementation of AtomicBase. The underlying atomic
 * value is a unsigned int because the QURT atomic APIs require
 * unsigned ints.
 */
class AtomicBase {
  static_assert(sizeof(unsigned int) == sizeof(uint32_t),
                "Unsigned int must be the same size as uint32_t or"
                " atomic operations may fail.");
 protected:
  //! The underlying atomic value.
  unsigned int mValue;
};

typedef AtomicBase AtomicBoolBase;
typedef AtomicBase AtomicUint32Base;

}  // namespace chre

#endif  // CHRE_PLATFORM_SLPI_ATOMIC_BASE_H_
