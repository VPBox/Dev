/*
 * Copyright (C) 2017 The Android Open Source Project
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

#ifndef NETUTILS_MATH_H
#define NETUTILS_MATH_H

#include <algorithm>
#include <cstdint>

namespace android {
namespace netdutils {

template <class T>
inline constexpr const T mask(const int shift) {
    return (1 << shift) - 1;
}

// Align x up to the nearest integer multiple of 2^shift
template <class T>
inline constexpr const T align(const T& x, const int shift) {
    return (x + mask<T>(shift)) & ~mask<T>(shift);
}

}  // namespace netdutils
}  // namespace android

#endif /* NETUTILS_MATH_H */
