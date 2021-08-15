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

#ifndef _GTS_NANOAPPS_SHARED_ARRAY_LENGTH_H_
#define _GTS_NANOAPPS_SHARED_ARRAY_LENGTH_H_

#include <stddef.h>

// We chose to have this at the global name scope, as this method
// isn't exclusive to nanoapp testing, and hopefully someday will
// move to some place more central.

/**
 * Determine the length of a statically-sized array at compile time using
 * template trickery which makes sure we only match statically-sized arrays.
 */
template <typename T, size_t N>
constexpr size_t arrayLength(T (&)[N]) { return N; }


#endif  // _GTS_NANOAPPS_SHARED_ARRAY_LENGTH_H_
