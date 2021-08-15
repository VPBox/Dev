/*
 * Copyright 2015 The Android Open Source Project
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
 *
 */
#ifndef GOOGLE_GATEKEEPER_UTILS_H_
#define GOOGLE_GATEKEEPER_UTILS_H_

#include <string.h>

namespace gatekeeper {
/**
 * Variant of memset() that uses GCC-specific pragmas to disable optimizations, so effect is not
 * optimized away.  This is important because we often need to wipe blocks of sensitive data from
 * memory.  As an additional convenience, this implementation avoids writing to NULL pointers.
 */
#ifdef __clang__
#define OPTNONE __attribute__((optnone))
#else  // not __clang__
#define OPTNONE __attribute__((optimize("O0")))
#endif  // not __clang__
inline OPTNONE void* memset_s(void* s, int c, size_t n) {
    if (!s)
        return s;
    return memset(s, c, n);
}
#undef OPTNONE

/**
 * Return the number of elements in array \p a.
 */
template <typename T, size_t N> inline size_t array_length(const T (&)[N]) {
    return N;
}

static inline int memcmp_s(const void* p1, const void* p2, size_t length) {
    const uint8_t* s1 = static_cast<const uint8_t*>(p1);
    const uint8_t* s2 = static_cast<const uint8_t*>(p2);
    uint8_t result = 0;
    while (length-- > 0)
        result |= *s1++ ^ *s2++;
    return result == 0 ? 0 : 1;
}

};
#endif //GOOGLE_GATEKEEPER_UTILS_H_
