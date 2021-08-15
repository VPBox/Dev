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

#ifndef _GTS_NANOAPPS_SHARED_NANO_ENDIAN_H_
#define _GTS_NANOAPPS_SHARED_NANO_ENDIAN_H_

// If the platform has no endian.h, then have the build system set
// CHRE_NO_ENDIAN_H, and set __BYTE_ORDER, __LITTLE_ENDIAN, and
// __BIG_ENDIAN appropriately.
#ifndef CHRE_NO_ENDIAN_H
#include <endian.h>
#endif

#include <cstddef>
#include <cstdint>


#if !(defined(__BYTE_ORDER) && defined(__LITTLE_ENDIAN) && \
      defined(__BIG_ENDIAN))
#error "Need to define the preprocessor defines __BYTE_ORDER, __LITTLE_ENDIAN" \
       " and __BIG_ENDIAN."
#endif

#if __LITTLE_ENDIAN == __BIG_ENDIAN
#error "__LITTLE_ENDIAN and __BIG_ENDIAN must have different values."
#endif

#if ((__BYTE_ORDER != __LITTLE_ENDIAN) && (__BYTE_ORDER != __BIG_ENDIAN))
#error "__BYTE_ORDER must be either __LITTLE_ENDIAN or __BIG_ENDIAN."
#endif


namespace nanoapp_testing {

void swapBytes(uint8_t *bytes, size_t size);

// Note: The 'static' with these 'inline' methods is required for our
// unit tests to work, since they compile this header with different
// endianness.  Without the 'static', we'll just use the first version
// of this we compile with, and fail some of the tests.

template<typename T>
static inline T hostToLittleEndian(T value) {
#if (__BYTE_ORDER == __BIG_ENDIAN)
  swapBytes(reinterpret_cast<uint8_t*>(&value), sizeof(T));
#endif
  return value;
}

template<typename T>
static inline T littleEndianToHost(T value) {
  // This has identical behavior to hostToLittleEndian.  We provide both
  // so code reads more cleanly.
  return hostToLittleEndian(value);
}

}  // namespace nanoapp_testing

#endif  // _GTS_NANOAPPS_SHARED_NANO_ENDIAN_H_
